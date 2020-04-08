/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "DSLParser/DesignProblem.hh"
#include <cmath>
#include <algorithm>
#include <fstream>
#include "utilities/GraphUtilities.hh"

using namespace chase;
using namespace patternsOnNetworks;

void DesignProblem::_noRecovery(SpecFunction *spec)
{
    std::set< std::string > comps =
            _findComponents( spec->parameters[0]);

    // Iterate over the components.
    std::set< std::string >::iterator cit;
    for(cit = comps.begin(); cit != comps.end(); ++cit)
    {
        auto state = _stateVariables.find(*cit)->second;

        auto p_now = Prop(state);
        auto p_next = Prop(state);
        auto np_now = Not(p_now);
        auto np_next = Not(p_next);

        auto formula = Always(Implies(np_now, Next(np_next)));

        _gr1_env_safety.insert(formula);
    }
}

void DesignProblem::_maxFailures(SpecFunction *spec)
{
    std::set<std::string> comps =
            _findComponents(spec->parameters[0]);

    unsigned int max = std::strtoul(
            spec->parameters[1].c_str() , NULL, 0);

    if( comps.size() <= max )
        return;

    std::vector< std::string > elements(comps.begin(), comps.end());
    std::list< std::vector< std::string > > results;
    std::vector< std::string > first;

    unsigned int safe = elements.size() - max;

    chase::getSubsetBySize(
            elements,
            elements.size(),
            safe, 0 , first, results );


    std::vector< std::shared_ptr<LogicFormula> > v;
    for( auto rit = results.begin(); rit != results.end(); ++rit )
    {
        auto curr = (*rit);
        std::vector< std::shared_ptr<LogicFormula> > wv;
        for( size_t i = 0; i < curr.size(); ++i )
        {
            if(_stateVariables.find(curr[i]) == _stateVariables.end())
                messageError("Component not found: " + curr[i]);

            auto var = _stateVariables.find(curr[i])->second;
            wv.push_back(Prop(var));
        }
        std::shared_ptr<LogicFormula> lf = wv[0];
        for(size_t i = 1; i < wv.size(); ++i)
            lf = And(lf, wv[i]);

        v.push_back(lf);
    }
    std::shared_ptr<LogicFormula> lf = v[0];
    for(size_t i = 1; i < v.size(); ++i)
        lf = Or(lf, v[i]);
    auto formula = Always(lf);
    _gr1_env_safety.insert(formula);
}

void DesignProblem::_initialState(SpecFunction *spec) {
    std::set<std::string> comps =
            _findComponents(spec->parameters[0]);

    bool value;
    value = atoi(spec->parameters[1].c_str());

    std::set<std::string>::iterator it;
    for (it = comps.begin(); it != comps.end(); ++it)
    {
        auto state = _stateVariables.find(*it)->second;
        if(value) _gr1_env_init.insert(Prop(state));
        else _gr1_env_init.insert(Not(Prop(state)));
    }
}

void DesignProblem::_switchTime(SpecFunction *spec, bool onoff)
{
    // Find the number of steps to be generated.
    chase_time timing(
            atoi(spec->parameters[1].c_str()),
            spec->parameters[2]);

    unsigned int steps = _compute_steps(timing);

    // Find the components being affected by the specifications.
    std::set<std::string> comps =
            _findComponents(spec->parameters[0]);
    std::set< std::string >::iterator cit;
    for( cit = comps.begin(); cit != comps.end(); ++cit) {
        // Create a counter variable for each component.
        std::string counter_varname(*cit);
        std::replace(
                counter_varname.begin(), counter_varname.end(), ' ', '_');
        onoff ? counter_varname += "_on_counter"
              : counter_varname += "_off_counter";

        auto counter_var = Var(
                Int(0, steps - 1),
                Nam(counter_varname),
                chase::input);

        std::pair<std::string, std::shared_ptr<Variable> >
                p(*cit, counter_var);
        onoff ? _onTimer.insert(p) : _offTimer.insert(p);

        // Declare the variable in the contract.
        _contract->addDeclaration(counter_var);

        // Create the constraints.
        auto state_var = _stateVariables.find(*cit)->second;
        auto command_var = _commandVariables.find(*cit)->second;

        std::shared_ptr< LogicFormula > state_prop = Prop(state_var);
        std::shared_ptr< LogicFormula > state_prop_copy = Prop(state_var);

        std::shared_ptr< LogicFormula > command_prop = Prop(command_var);

        if(! onoff)
        {
            state_prop = Not(state_prop);
            state_prop_copy = Not(state_prop_copy);
            command_prop = Not(command_prop);
        }

        auto counter_is_zero = Eq(
                Id(counter_var),
                IntVal(0));

        auto formula_base_step = Always(
                Implies(And(state_prop,command_prop),
                        Next(And(
                                Prop(counter_is_zero),
                                state_prop_copy))));



        // Add the initial state.
        _gr1_env_init.insert(Prop(counter_is_zero));
        // Add the safety constraint.
        _gr1_env_safety.insert(formula_base_step);

        // Add the final step.
        auto expr_minus_one = Eq(Id(counter_var),IntVal(steps-1));

        std::shared_ptr< LogicFormula > final_step;

        if( onoff )
        {
            final_step = Always(
                    Implies(And(
                            And(Prop(expr_minus_one),Not(Prop(state_var))),
                            Prop(command_var)
                    ), Next(And(
                            Prop(state_var),
                            Prop(
                                    Eq(Id(counter_var), IntVal(0)))
                            ))
            ));
        } else
        {
            final_step = Always(
                    Implies(And(
                            And(Prop(expr_minus_one),Prop(state_var)),
                            Not(Prop(command_var))
                            ), Next(And(
                            Not(Prop(state_var)),
                            Prop(
                                    Eq(Id(counter_var),
                                       IntVal(0)))
                            ))
                    ));
        }

        _gr1_env_safety.insert(final_step);

        // Generate the different iterations.
        for( unsigned int step = 0; step < steps - 1; ++step )
        {
            auto exp_now = Eq(
                    Id(counter_var),
                    IntVal(step));
            auto exp_next = Eq(
                    Id(counter_var),
                    IntVal(step + 1));

            std::shared_ptr<LogicFormula> f_state = Prop(state_var);
            std::shared_ptr<LogicFormula> f_command = Prop(command_var);
            std::shared_ptr<LogicFormula> f_rev_state = Prop(state_var);
            std::shared_ptr<LogicFormula> f_state_copy = Prop(state_var);

            if(onoff) {
                f_state = Not(f_state);
                f_state_copy = Not(f_state_copy);
            } else {
                f_command = Not(f_command);
                f_rev_state = Not(f_rev_state);
            }

            auto now = And(And(f_command, f_state),Prop(exp_now));

            auto next = Next(Or(
                    And(Prop(exp_next), f_state_copy),
                    And(
                            Prop(Eq(Id(counter_var),
                            IntVal(0))), f_rev_state)));

            auto formula = Always(Implies(now, next));

            _gr1_env_safety.insert(formula);
        }
    }
}

void DesignProblem::_neverConnect(SpecFunction *spec)
{
    std::set<std::string> sources =
            _findComponents(spec->parameters[0]);
    std::set<std::string> targets =
            _findComponents(spec->parameters[1]);

    std::set< std::string > switches =
            _findComponents("contactor");


    std::set< std::string > comps;
    std::set< std::string > tmp;

    comps.insert(sources.begin(), sources.end());
    comps.insert(targets.begin(), targets.end());
    comps.insert(switches.begin(), switches.end());

    comps.insert(tmp.begin(), tmp.end());
    tmp.clear();

    size_t argn = spec->parameters.size();
    for( size_t i = 2; i < argn; ++i )
    {
        tmp = _findComponents(spec->parameters[i]);
        comps.insert(tmp.begin(), tmp.end());
        tmp.clear();
    }

    auto vertexes = std::set< Vertex * >();

    for( auto it = comps.begin(); it != comps.end(); ++it )
    {
        std::string s = *it;
        int index = _architecture->getVertexIndex(s);

        if(index < 0)
            messageError("Node does not exist: " + s);

        auto v = _architecture->getVertex(index);
        vertexes.insert(v.get());
    }

    std::shared_ptr<Graph> subgraph =
            chase::getSubGraph(_architecture, vertexes);

    for(auto sit = sources.begin(); sit != sources.end(); ++sit)
    {
        for(auto tit = targets.begin(); tit != targets.end(); ++tit)
        {
            unsigned source_id = subgraph->getVertexIndex(*sit);
            unsigned target_id = subgraph->getVertexIndex(*tit);

            /// \todo Improve here to avoid equivalent constraints when dealing
            /// with the "never-connect" pattern.
            if( source_id == target_id ) continue;

            std::list< std::vector< unsigned > > result;
            std::vector< unsigned > visited;
            visited.push_back(source_id);

            findAllPathsBetweenNodes(
                    subgraph, visited, target_id, result);

            for(auto lit = result.begin(); lit != result.end(); ++lit)
            {
                std::string vname =
                        subgraph->getVertex(
                                (*lit)[0])->getName()->getString();

                std::shared_ptr< LogicFormula > formula;

                // If not controllable, do not insert it.
                if( _commandVariables.find(vname) != _commandVariables.end()) {
                    auto statevar = _stateVariables.find(vname)->second;
                    formula = Not(Prop(statevar));
                }

                for( size_t v = 1; v < (*lit).size(); ++v)
                {
                    std::string currname(
                            subgraph->getVertex(
                                    (*lit)[v])->getName()->getString());

                    if (_stateVariables.find(currname) != _stateVariables.end()
                        && _commandVariables.find(currname) != _commandVariables.end() )
                    {
                        auto currvar = _stateVariables.find(currname)->second;

                        std::shared_ptr<LogicFormula> f = Not(Prop(currvar));

                        if(formula != nullptr) formula = Or(formula, f);
                        else formula = f;
                    }
                }

                /// \todo Evaluate whether it is or not necessary to
                /// include the formula in the sys init.
                // _gr1_sys_init.insert(formula);
                formula = Always(formula);
                _gr1_sys_safety.insert(formula);
            }
        }
    }
}

void DesignProblem::_preferActiveConnection(SpecFunction *spec)
{
    std::set<std::string> sources =
            _findComponents(spec->parameters[1]);
    std::set<std::string> targets =
            _findComponents(spec->parameters[0]);

    if(sources.size() == 0 || targets.size() == 0 )
    {
        messageWarning("Useless requirement: " + spec->name);
        return;
    }

    std::shared_ptr< LogicFormula > condition;
    //
    for( auto sit = sources.begin(); sit != sources.end(); ++sit )
    {
        auto found = _stateVariables.find(*sit);
        if(found == _stateVariables.end()) continue;

        auto statevar = found->second;

        if(condition == nullptr)
            condition = Prop(statevar);
        else
            condition = Or(condition, Prop(statevar));
    }

    // If the sources are not controllable, than set the condition to true.
    if( condition == nullptr ) condition = True();

    // Find all paths.
    std::shared_ptr<LogicFormula> consequence;

    for( auto tit = targets.begin(); tit != targets.end(); ++tit )
    {
        int target_id = _architecture->getVertexIndex(*tit);



        for( auto sit = sources.begin(); sit != sources.end(); ++sit )
        {
            int source_id = _architecture->getVertexIndex(*sit);
            std::list< std::vector< unsigned > > result;
            std::vector< unsigned > visited;
            visited.push_back(source_id);
            findAllPathsBetweenNodes(_architecture, visited, target_id, result);

            for( auto rit = result.begin(); rit != result.end(); ++rit )
            {
                if(consequence == nullptr)
                    consequence = _pathDoesExist(*rit);
                else
                    consequence = Or(consequence, _pathDoesExist(*rit));
            }
        }
    }
    auto formula = Always(Implies(condition, consequence));

    _gr1_sys_safety.insert(formula);
}

void DesignProblem::_mustDisconnectFailed(SpecFunction *spec)
{
    chase_time timing(
            std::strtoul(spec->parameters[1].c_str(), nullptr, 0),
            spec->parameters[2]);
    unsigned int steps = _compute_steps(timing);

    std::set<std::string> components =
            _findComponents(spec->parameters[0]);

    for( auto comp = components.begin(); comp != components.end(); ++comp )
    {
        if(_stateVariables.find(*comp) == _stateVariables.end())
        {
            messageWarning("Component: " + *comp + " cannot fail.");
            continue;
        }
        // Retrieve the state variable of the component to disconnect.
        auto comp_state_var =  _stateVariables.find(*comp)->second;
        // Create counter variable. It must goes from 0 to steps + 1.
        std::string counter_varname(*comp);
        std::replace(
                counter_varname.begin(),
                counter_varname.end(),' ', '_');
        counter_varname += "_disconnect_counter";
        auto counter_var = Var(
                Int(0, steps - 1),Nam(counter_varname),output);

        // Add variable to contract.
        _contract->addDeclaration(counter_var);

        // Set initial value.

        auto init_prop = Prop(
                Eq(Id(counter_var),IntVal(0)));

        init_prop->setName( Nam(counter_varname + "__eq_0"));
        _gr1_sys_init.insert(init_prop);

        // Create the proposition []( state -> counter = 0 );
        auto zero = Always(
                Implies(
                        Prop(comp_state_var),
                        Prop(Eq(Id(counter_var),IntVal(0)
                                        ))));

        _gr1_sys_safety.insert(zero);

        // Create disconnections.
        unsigned vertex_id = _architecture->getVertexIndex(*comp);
        std::set< unsigned > adjs =
                _architecture->getAdjacentNodes(vertex_id);

        std::shared_ptr< LogicFormula > tbd;
        for( auto a = adjs.begin(); a != adjs.end(); ++a)
        {
            std::string node =
                    _architecture->getVertex(*a)->getName()->getString();
            if(_commandVariables.find(node) == _commandVariables.end())
                messageError(
                        std::string("The architecture does not allow to realize the system.")
                        + "The component " + (*comp) + " cannot be disconnected when it fails.\n"
                        + "No controllable components connected to " + (*comp));

            auto state = _stateVariables.find(node)->second;
            auto unactive = Not(Prop(state));

            if(tbd == nullptr)
                tbd = unactive;
            else
                tbd = And(tbd, unactive);
        }

        _gr1_sys_safety.insert(
                Always(Implies(
                        tbd,
                        Prop(Eq(Id(counter_var), IntVal(0))))));

        // Iterate over the possible values.
        for( unsigned step = 0; step < steps; ++step )
        {

            // Check whether the nodes are controllable.
            // Create the formula with the nodes to be disconnected.
            std::shared_ptr<LogicFormula> tobedisconnected;
            std::shared_ptr<LogicFormula> tobedisconnected_copy;
            std::shared_ptr<LogicFormula> not_disconnected;
            for(auto a = adjs.begin(); a != adjs.end(); ++a)
            {
                std::string node =
                        _architecture->getVertex(*a)->getName()->getString();
                if(_commandVariables.find(node) == _commandVariables.end())
                    messageError(
                            std::string("The architecture does not allow to realize the system.")
                            + "The component " + (*comp) + " cannot be disconnected when it fails.\n"
                            + "No controllable components connected to " + (*comp));

                auto state = _stateVariables.find(node)->second;
                auto unactive = Prop(state);
                auto unactive_copy = Not(Prop(state));

                if(a == adjs.begin())
                {
                    tobedisconnected = unactive;
                    tobedisconnected_copy = unactive_copy;
                    not_disconnected = Prop(state);
                }
                else
                {
                    tobedisconnected =
                            And(tobedisconnected, unactive);

                    tobedisconnected_copy =
                            And(tobedisconnected_copy, unactive_copy);

                    not_disconnected = And(not_disconnected, Not(Prop(state)));

                }
            }

            auto counter_before = Prop(Eq(Id(counter_var),IntVal(step)));

            auto counter_after = Prop(
                    Eq(Id(counter_var),IntVal(step + 1)));

            auto before = And(
                    counter_before,
                    Not(Prop(comp_state_var)));


            std::shared_ptr< LogicFormula > after;
            if( step == steps - 1 )
            {
                after = Next(And(
                        tobedisconnected_copy,
                        Prop(
                                Eq(Id(counter_var), IntVal(0)))));
            }
            else{
                after = Next(Or(
                        And(counter_after, not_disconnected),
                        And(
                                tobedisconnected_copy,
                                Prop(
                                        Eq(Id(counter_var), IntVal(0))))));


            }
            auto f = Always(Implies(before, after));

            _gr1_sys_safety.insert(f);

        }
    }
}

void DesignProblem::_neverDisconnect(SpecFunction *spec)
{
    // Find the number of steps to be generated.
    chase_time timing(
            atoi(spec->parameters[2].c_str()),
            spec->parameters[3]);

    unsigned int steps = _compute_steps(timing);

    // get components.
    std::set<std::string> target =
            _findComponents(spec->parameters[0]);
    std::set<std::string> source =
            _findComponents(spec->parameters[1]);

    for( auto tit = target.begin(); tit != target.end(); ++tit )
    {
        int target_id = _architecture->getVertexIndex(*tit);

        std::string counter_varname(*tit);
        std::replace(
                counter_varname.begin(),
                counter_varname.end(), ' ', '_');

        counter_varname += "_disconnected_from_";

        std::string sourcetype(spec->parameters[1]);
        std::replace(
                sourcetype.begin(),
                sourcetype.end(), ' ', '_');

        counter_varname += sourcetype;

        auto counter_var = Var(
                Int(0, steps - 1), Nam(counter_varname),chase::output);

        _contract->declarations.push_back(counter_var);

        // Generate condition when counter stays to zero.

        std::list< std::vector < unsigned > > paths;
        for( auto sit = source.begin(); sit != source.end(); ++sit )
        {
            std::vector< unsigned > visited;
            int source_id = _architecture->getVertexIndex(*sit);
            visited.push_back(source_id);
            findAllPathsBetweenNodes(
                    _architecture, visited, target_id, paths);
        }

        if(paths.size() == 0) messageError(
                std::string("The architecture does not allow to realize the system.")
                + " the component " + *tit + " is not connected to any " +
                sourcetype);

        std::shared_ptr<LogicFormula> condition = nullptr;
        for( auto pit = paths.begin(); pit != paths.end(); ++pit )
        {
            if(condition == nullptr)
                condition = _pathDoesExist(*pit);
            else
                condition = Or(condition, _pathDoesExist(*pit));
        }

        if(condition.get() == nullptr)
            messageError("Impossible to build a path.");

        auto consequence = Prop(
                Eq(Id(counter_var), IntVal(0)));

        auto counter_zero = Always(Implies(condition, consequence));

        _gr1_sys_safety.insert(counter_zero);

        for( unsigned step = 0; step < steps; ++ step )
        {
            std::shared_ptr<LogicFormula> condition;
            for( auto pit = paths.begin(); pit != paths.end(); ++pit )
            {
                if(condition.get() == nullptr)
                    condition = _pathDoesExist(*pit);
                else
                    condition = Or(condition, _pathDoesExist(*pit));
            }
            condition = Not(condition);
            condition = And(condition,
                    Prop( Eq(Id(counter_var),IntVal(step))));

            std::shared_ptr<LogicFormula> Exist_1;
            for( auto pit = paths.begin(); pit != paths.end(); ++pit )
            {
                if(Exist_1.get() == nullptr)
                    Exist_1 = _pathDoesExist(*pit);
                else
                    Exist_1 = Or(Exist_1, _pathDoesExist(*pit));
            }


            std::shared_ptr<LogicFormula> Exist_2;
            for( auto pit = paths.begin(); pit != paths.end(); ++pit )
            {
                if(Exist_2 == nullptr)
                    Exist_2 = _pathDoesExist(*pit);
                else
                    Exist_2 = Or(Exist_2, _pathDoesExist(*pit));
            }

            std::shared_ptr<LogicFormula> consequence;
            if(step == steps - 1)
            {
                consequence = And(Exist_2, Prop(
                        Eq(Id(counter_var), IntVal(0))));
            }
            else {
                std::shared_ptr<LogicFormula> notExistPath = Not(Exist_1);
                notExistPath = And(notExistPath,
                                   Prop(
                                           Eq(Id(counter_var),
                                                   IntVal(step + 1))
                                   ));

                consequence = Or(
                        notExistPath,
                        And(Exist_2, Prop(
                                Eq(Id(counter_var), IntVal(0)))));
            }
            auto formula = Always(Implies(condition, Next(consequence)));

            _gr1_sys_safety.insert(formula);
        }
    }
}

void DesignProblem::_keepConnectionStable(SpecFunction *spec)
{
    std::set<std::string> sources =
            _findComponents(spec->parameters[0]);
    std::set<std::string> targets =
            _findComponents(spec->parameters[1]);

    for( auto tit = targets.begin(); tit != targets.end(); ++tit )
    {
        int target_id = _architecture->getVertexIndex(*tit);
        if( target_id < 0 )
            messageError("Node does not exist: " + *tit);
        for( auto sit = sources.begin(); sit != sources.end(); ++sit )
        {
            int source_id = _architecture->getVertexIndex(*sit);
            if( source_id < 0 )
                messageError("Node does not exist: " + *sit);

            std::vector< unsigned > visited;
            visited.push_back(source_id);

            std::list< std::vector< unsigned > > result;

            findAllPathsBetweenNodes(_architecture, visited, target_id, result);

            for( auto vit = result.begin(); vit != result.end(); ++vit)
            {
                std::shared_ptr<LogicFormula> livepath = _pathDoesExist(*vit);
                std::shared_ptr<LogicFormula> activatepath = _activatePath(*vit);

                auto constraint = Always(Implies( livepath, activatepath));
                _gr1_sys_safety.insert(constraint);
            }
        }
    }
}
