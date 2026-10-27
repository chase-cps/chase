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
   on a theory or a back end.
2. Checks (``chase_cps.checks`` and ``chase_cps.logic``): the checks on
   contracts (refines, verify, is_compatible, is_consistent, implements,
   accepts, remove_redundant) reduce to queries on formulas of a theory
   (implies: validity; is_satisfiable: satisfiability; unsat_core), which
   the back ends decide (nuXmv, NuSMV; real variables through
   ``chase_cps.reals``).
3. GR(1) synthesis (``chase_cps.gr1``): realizability, synthesis, and the
   analyses built on them (unrealizable_core, is_well_separated,
   remove_vacuous), for contracts in the GR(1) fragment of LTL, with slugs.

All the names are also available from ``chase_cps`` itself.
"""

from . import backends, checks, gr1, logic, ltl, tools
from .backends import check
# 1. Contracts and their algebra.
from .contract import Contract, load_spec, parse_spec, to_spec
from .ltl import LTLSyntaxError
# 2. Checks on contracts, and the queries on formulas they reduce to.
from .checks import (RefinementResult, Redundancy, Verification, accepts, implements,
                     is_compatible, is_consistent, refines, remove_redundant, verify)
from .logic import Core, implies, is_satisfiable, unsat_core
# 3. GR(1) synthesis.
from .gr1 import (Vacuity, VacuousTerm, is_realizable, is_well_separated, remove_vacuous,
                  synthesize, unrealizable_core)
from .queries import Implies, Realizable, Result, Satisfiable, Trace

__version__ = "0.3.1"

__all__ = [
    # 1. Contracts and their algebra
    "Contract", "load_spec", "parse_spec", "to_spec", "LTLSyntaxError",
    # 2. Checks on contracts
    "refines", "verify", "is_compatible", "is_consistent", "implements", "accepts",
    "remove_redundant", "RefinementResult", "Verification", "Redundancy",
    #    and on formulas
    "implies", "is_satisfiable", "unsat_core", "Core",
    # 3. GR(1) synthesis
    "is_realizable", "synthesize", "unrealizable_core", "is_well_separated",
    "remove_vacuous", "Vacuity", "VacuousTerm",
    # Results and back ends
    "Result", "Trace", "check", "backends", "checks", "logic", "gr1", "ltl", "tools",
    "Implies", "Satisfiable", "Realizable",
]
