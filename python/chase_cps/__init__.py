"""CHASE: Contract-based Heterogeneous Analysis and Systems Exploration.

Python interface to the CHASE core library and its back ends::

    from chase_cps import Contract, refines

    spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 assumptions=["G F !req"],
                                 guarantees=["G (req -> F grant)"], name="Spec")
    impl = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 guarantees=["G (req -> X grant)"], name="Impl")
    result = refines(impl, spec)    # valid, or invalid with a counterexample
"""

from . import backends, ltl, tools
from .backends import check
from .checks import (RefinementResult, Redundancy, Verification, accepts, implements,
                     implies, is_compatible, is_consistent, is_realizable, is_satisfiable,
                     is_well_separated, refines, remove_redundant, synthesize, verify)
from .contract import Contract, load_spec, parse_spec, to_spec
from .cores import Core, unrealizable_core, unsat_core
from .ltl import LTLSyntaxError
from .queries import Implies, Realizable, Result, Satisfiable, Trace

__version__ = "0.1.1"

__all__ = [
    "Contract", "load_spec", "parse_spec", "to_spec", "LTLSyntaxError",
    "Result", "Trace", "RefinementResult", "Verification", "Redundancy",
    "refines", "verify", "is_consistent", "is_compatible", "is_realizable", "synthesize",
    "implies", "is_satisfiable", "implements", "accepts", "remove_redundant",
    "Core", "unsat_core", "unrealizable_core", "is_well_separated",
    "check", "backends", "ltl", "tools",
    "Implies", "Satisfiable", "Realizable",
]
