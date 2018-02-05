from __future__ import absolute_import
import copy
import logging
import pprint
import warnings

import time

import tulip
from tulip.spec import GRSpec
from tulip import transys
from tulip.interfaces import omega
from tulip import synth
from tulip import dumpsmach
try:
    import dd.bdd as _bdd
except ImportError:
    _bdd = None


try:
    from dd import cudd
except ImportError:
    cudd = None
try:
    import omega
    from omega.logic import bitvector as bv
    from omega.games import gr1
    from omega.symbolic import enumeration as sym_enum
    from omega.symbolic import symbolic as sym
    from omega.games import enumeration as enum
except ImportError:
    omega = None
    print("WARNING: Omega is not available.") 
import networkx as nx
from copy import deepcopy



def cs_to_ce(ctrl):
    ce_list=[]
    for state in ctrl.nodes(data=False):
        if ((len(ctrl.successors(state))==0) or (len(ctrl.successors(state))==1 and ctrl.successors(state)[0]==state)): #if deadend or infinite self-loop
            for path in nx.all_simple_paths(ctrl, source=0, target=state):  
                ce=deepcopy(ctrl)                
                for state_rem in ctrl.nodes(data=False):                    
                    if ((not(state_rem in path)) and state_rem!='Sinit' and (state_rem is not None)):
                        ce.remove_node(state_rem)
                ce=remove_init_edges(ce)                
                ce_list.append(ce)
    return ce_list

def remove_init_edges(ctrl):
    neighbors_init = ctrl.neighbors('Sinit')
    for i in neighbors_init:
        if i!=0:
            ctrl.remove_edge('Sinit',i)
    return ctrl

def print_counterexamples(
        ctrl, 
        prefix='/tmp/'):
    ce_list = cs_to_ce(ctrl)

    for index,item in enumerate(ce_list):
        filename = prefix + 'counterexample_' + str(index) + '.jpg';
        if not item.save(filename):
            print(item)

#    print(ctrl);
    if not ctrl.save(prefix + 'counterstrategy.jpg'):
        print(ctrl)
    else:
       print('Counterstrategy pictorial representation saved in ' + 
          prefix + 'counterstrategy.jpg');

    # Create Python code for the Mealy 
    dumpsmach.write_python_case( 
            prefix + 'counterstrategy.py',ctrl)
    print('Counterstratagy implementation saved in ' + 
            prefix + 'counterstrategy.py');



def synthesize_environment(spec, use_cudd=False):
    """Return transducer enumerated as a graph.

    @type spec: `tulip.spec.form.GRSpec`
    @param use_cudd: efficient BDD computations with `dd.cudd`
    @rtype: `networkx.DiGraph`
    """
    aut = tulip.interfaces.omega._grspec_to_automaton(spec)
    sym.fill_blanks(aut)
    bdd = tulip.interfaces.omega._init_bdd(use_cudd)
    aut.bdd = bdd
    a = aut.build()
    assert a.action['sys'][0] != bdd.false
    z, yij, xijk = gr1.solve_streett_game(a)
    if (not gr1.is_realizable(z, a)) :
        aut_rabin = generate_complement(aut) 
        controller_rabin = synthesize_rabin_controller(aut_rabin)
        h = tulip.interfaces.omega._strategy_to_state_annotated(controller_rabin, a)
        return h;
    else:
        print("Warning: the automata is synthesizable")
     
def synthesize_rabin_controller(b):
    """Return enumerated graph with steps as edges.
     after solving Rabin(1) game.

    @param b: game with <>[] & []<> winning
    @type b: `symbolic.Automaton`
    """
    qinit = b.qinit
    aut_rabin = b.build()
    zk, yki, xkijr = gr1.solve_rabin_game(aut_rabin)
    t= gr1.make_rabin_transducer(zk, yki, xkijr, aut_rabin)
    g = enum.action_to_steps(t, qinit=qinit)
    return g

def generate_complement(aut_streett):
    """Return rabin automaton. This function is used to create complement automaton from given streett

    @type aut_streett: `omega.symbolic.symbolic.Automaton`
    @rtype: `omega.symbolic.symbolic.Automaton`
    """
    
    aut_rabin = sym.Automaton()
    aut_rabin.acceptance = 'Rabin(1)'
    
    # Switch variable ownership
    for var, d in aut_streett.vars.items():
        d = d.copy()
        owner =  d['owner']
        owner = 'env' if owner == 'sys' else 'sys'
        d['owner'] = owner
        aut_rabin.vars[var] = d

    aut_rabin.init['env']=aut_streett.init['sys']
    aut_rabin.init['sys']=aut_streett.init['env']
    aut_rabin.action['env'] = aut_streett.action['sys']
    aut_rabin.action['sys'] = aut_streett.action['env']    
    win = ['!({w})'.format(w=w) for w in aut_streett.win['<>[]']]
    aut_rabin.win['[]<>'] = win
    win = ['!({w})'.format(w=w) for w in aut_streett.win['[]<>']]
    aut_rabin.win['<>[]'] = win
    sym.fill_blanks(aut_rabin, rabin=True)
    aut_rabin.qinit = '\E \E'
    
    return aut_rabin


def get_counterstrategy(specs) :
    counterstrategy = synthesize_environment(specs);
    ctrl = synth.strategy2mealy(counterstrategy, specs);
    return ctrl;

