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
from .checks import (RefinementResult, Simplification, compatible, consistent,
                     contains_environment, contains_implementation, implies,
                     realizable, refines, satisfiable, simplify)
from .contract import Contract, load_spec, parse_spec, to_spec
from .ltl import LTLSyntaxError
from .queries import Implies, Realizable, Result, Satisfiable, Trace

__version__ = "0.1.0"

__all__ = [
    "Contract", "load_spec", "parse_spec", "to_spec", "LTLSyntaxError",
    "Result", "Trace", "RefinementResult", "Simplification",
    "implies", "satisfiable", "realizable", "refines", "consistent", "compatible",
    "simplify", "contains_environment", "contains_implementation",
    "check", "backends", "ltl", "tools",
    "Implies", "Satisfiable", "Realizable",
]
