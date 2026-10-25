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

from . import backends, ltl
from .backends import check
from .checks import (RefinementResult, Redundancy, compatible, consistent,
                     contains_environment, contains_implementation, implies,
                     realizable, refines, remove_redundant, satisfiable)
from .contract import Contract
from .ltl import LTLSyntaxError
from .queries import (Implies, Project, Realizable, Result, Satisfiable,
                      Simplify, Trace)

__version__ = "0.1.0"

__all__ = [
    "Contract", "LTLSyntaxError", "Result", "Trace", "RefinementResult", "Redundancy",
    "implies", "satisfiable", "realizable", "refines", "consistent", "compatible",
    "remove_redundant", "contains_environment", "contains_implementation",
    "check", "backends", "ltl",
    "Implies", "Satisfiable", "Realizable", "Simplify", "Project",
]
