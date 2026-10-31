"""CHASE: Contract-based Heterogeneous Analysis and Systems Exploration.

Python interface to the CHASE core library and its back ends::

    from chase_cps import Contract, refines

    spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 assumptions=["G F !req"],
                                 guarantees=["G (req -> F grant)"], name="Spec")
    impl = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 guarantees=["G (req -> X grant)"], name="Impl")
    result = refines(impl, spec)    # valid, or invalid with a counterexample

The package has three layers:

1. Contracts and their algebra (``chase_cps.contract``): the contracts, and
   the operations that build contracts from contracts (compose, conjoin,
   disjoin, merge, divide, separate, imply, coimply, invert, saturate,
   assume, ensure), computed by the CHASE core library. They do not depend
   on a theory or a back end. Contracts are read and written in the logics
   language of CHASE (``parse_spec``, ``to_spec``) and in SystemVerilog
   Assertions (``parse_sva``, ``to_sva``, see ``chase_cps.sva``).
2. Checks (``chase_cps.checks`` and ``chase_cps.logic``): the checks on
   contracts (refines, verify, is_compatible, is_consistent,
   is_implementation, is_environment, is_receptive, remove_redundant) reduce
   to queries on formulas of a theory
   (implies: validity; is_satisfiable: satisfiability; unsat_core), which
   the back ends decide (nuXmv, NuSMV; real variables through
   ``chase_cps.reals``). The implementations and environments can be
   state machines (``chase_cps.models``).
3. GR(1) synthesis (``chase_cps.gr1``): realizability, synthesis, and the
   analyses built on them (unrealizable_core, is_well_separated,
   is_vacuous, and remove_redundant with synth=True), for contracts in the
   GR(1) fragment of LTL, with slugs.

All the names are also available from ``chase_cps`` itself.
"""

from . import backends, checks, gr1, logic, ltl, models, tools
from .backends import check
# 1. Contracts and their algebra.
from .contract import Contract, load_dsl, load_spec, parse_spec, to_spec
from .ltl import LTLSyntaxError
from .sva import SVAError, SVAReading, parse_sva, to_sva
from .gr1spec import GR1FormatError, to_gr1
from .export import to_smtlib, to_smv
# 2. Checks on contracts, and the queries on formulas they reduce to.
from .checks import (RedundantTerm, RefinementResult, Redundancy, Verification, accepts,
                     implements, is_environment, is_implementation, is_compatible,
                     is_consistent, refines, remove_redundant,
                     verify)
from .models import Model, is_receptive
from .logic import Core, implies, is_satisfiable, unsat_core
from .optimize import Optimum, maximize, minimize
# 3. GR(1) synthesis.
from .gr1 import (OutputVacuity, VacuousOutput, is_realizable, is_vacuous, is_well_separated,
                  synthesize, unrealizable_core)
from .queries import Implies, Realizable, Result, Satisfiable, Trace
# Components, systems, and their diagrams.
from .system import Component, System
from . import viz

__version__ = "0.5.0"

__all__ = [
    # 1. Contracts and their algebra
    "Contract", "load_dsl", "load_spec", "parse_spec", "to_spec", "LTLSyntaxError",
    "parse_sva", "to_sva", "SVAReading", "SVAError",
    "to_gr1", "GR1FormatError", "to_smv", "to_smtlib",
    # 2. Checks on contracts
    "refines", "verify", "is_compatible", "is_consistent", "is_implementation",
    "is_environment", "implements", "accepts",
    "remove_redundant", "RefinementResult", "Verification", "Redundancy", "RedundantTerm",
    "Model", "is_receptive",
    #    and on formulas
    "implies", "is_satisfiable", "unsat_core", "Core",
    #    best values (formulas over a bounded number of steps)
    "minimize", "maximize", "Optimum",
    # 3. GR(1) synthesis
    "is_realizable", "synthesize", "unrealizable_core", "is_well_separated",
    "is_vacuous", "OutputVacuity", "VacuousOutput",
    # Components, systems, diagrams
    "Component", "System", "viz",
    # Results and back ends
    "Result", "Trace", "check", "backends", "checks", "logic", "gr1", "ltl", "tools",
    "Implies", "Satisfiable", "Realizable",
]
