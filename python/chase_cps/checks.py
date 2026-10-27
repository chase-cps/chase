"""Checks on contracts, built on implication and satisfiability.

Contracts are read as saturated: a contract (A, G) allows the behaviors of
A -> G. ``refined`` refines ``abstract`` when A_abstract -> A_refined and
(A_refined -> G_refined) -> (A_abstract -> G_abstract). A contract is
compatible when some environment satisfies A, consistent when some
behavior satisfies A -> G, and ``verify`` checks that A & G is satisfiable;
a component M implements the contract when M & A -> G holds, and the
contract accepts an environment E when E -> A. M and E are LTL formulas or
state machines (``chase_cps.models``); ``is_receptive`` checks that a state
machine does not refuse the inputs that the assumptions allow.
Each check is one or two queries on formulas (``chase_cps.logic``), so the
checks hold for any theory the back ends decide.

The checks take the back end (``"auto"``, ``"nuxmv"``, ``"nusmv"``) and
options of the back end as keywords: ``timeout`` in seconds (300 by
default), and for NuSMV ``engine`` (``"auto"``, ``"bdd"``, ``"bmc"``) and
``bmc_bound``. They also take a ``context`` (see ``chase_cps.logic``).

The checks of GR(1) synthesis (realizability and the analyses built on it)
are in ``chase_cps.gr1``; they are imported here too, as in the versions
before 0.3.1.
"""

from __future__ import annotations

from dataclasses import dataclass, field, replace
from typing import Optional

from . import ltl
from .contract import Contract, VarType
from .logic import (_context, _formula, _Implications, _within, implies,  # noqa: F401
                    is_satisfiable, unsat_core)
from .queries import INVALID, SAT, UNKNOWN, UNSAT, VALID, Result  # noqa: F401


def _merge_vars(*contracts: Contract) -> dict[str, VarType]:
    out: dict[str, VarType] = {}
    for c in contracts:
        for k, v in c.variables.items():
            if k in out and out[k] != v:
                raise ValueError(f"variable {k} has types {out[k]} and {v}")
            out[k] = v
    return out


def _reals(*results: Result) -> str | None:
    used = sorted({r.reals for r in results if r.reals})
    return ", ".join(used) or None


@dataclass
class RefinementResult:
    """Outcome of ``refines(refined, abstract)``; the trace of a failed part is a
    counterexample to it."""
    status: str                    # valid, invalid, unknown
    assumptions: Result            # A_abstract -> A_refined
    guarantees: Result             # (A_refined -> G_refined) -> (A_abstract -> G_abstract)

    def __bool__(self) -> bool:
        return self.status == VALID

    @property
    def failed(self) -> str | None:
        if self.assumptions.status == INVALID:
            return "assumptions"
        if self.guarantees.status == INVALID:
            return "guarantees"
        return None

    @property
    def reals(self) -> str | None:
        """How the real variables were handled (see ``Result.reals``)."""
        return _reals(self.assumptions, self.guarantees)

    def to_dict(self) -> dict:
        d = {"status": self.status, "failed": self.failed,
             "assumptions": self.assumptions.to_dict(),
             "guarantees": self.guarantees.to_dict()}
        if self.reals:
            d["reals"] = self.reals
        return d


def refines(refined: Contract, abstract: Contract, backend: str = "auto",
            context=None, **options) -> RefinementResult:
    """Does ``refined`` refine ``abstract`` (can it replace it)?"""
    variables = _merge_vars(refined, abstract)
    a = implies(abstract.assumption, refined.assumption, variables, backend, context,
                **options)
    g = implies(refined.saturated_guarantee, abstract.saturated_guarantee, variables, backend,
                context, **options)
    if a.status == VALID and g.status == VALID:
        status = VALID
    elif INVALID in (a.status, g.status):
        status = INVALID
    else:
        status = UNKNOWN
    return RefinementResult(status, a, g)


def is_consistent(contract: Contract, backend: str = "auto", core: bool = False,
                  context=None, **options) -> Result:
    """Is there a behavior allowed by the contract (A -> G satisfiable)? A -> G
    is unsatisfiable only when A is valid and G unsatisfiable; with core, an
    unsat result carries ``unsat_core`` of the guarantees."""
    r = is_satisfiable(contract.saturated_guarantee, contract.variables, backend,
                       context=context, **options)
    if core and r.status == UNSAT:
        r.core = unsat_core(_only(contract, guarantees=True), backend=backend,
                                  context=context, **options)
    return r


def is_compatible(contract: Contract, backend: str = "auto", core: bool = False,
                  context=None, **options) -> Result:
    """Is there an environment allowed by the contract (A satisfiable)? With
    core, an unsat result carries ``unsat_core`` of the assumptions."""
    r = is_satisfiable(contract.assumption, contract.variables, backend, context=context,
                       **options)
    if core and r.status == UNSAT:
        r.core = unsat_core(_only(contract, guarantees=False), backend=backend,
                                  context=context, **options)
    return r


def _only(c: Contract, guarantees: bool) -> Contract:
    """The contract with its guarantees only, or its assumptions only."""
    if guarantees:
        return Contract(c.name, dict(c.inputs), dict(c.outputs), [], list(c.guarantees))
    return Contract(c.name, dict(c.inputs), dict(c.outputs), list(c.assumptions), [])


@dataclass
class Verification:
    """Outcome of ``verify``. The status is that of ``feasible``: some
    behavior satisfies A & G, so the assumptions and the guarantees can hold
    together. ``compatible`` (A satisfiable) and ``consistent`` (A -> G
    satisfiable) follow from it, with the same witness; when A & G is
    unsatisfiable they say whether the assumptions alone cannot hold."""
    compatible: Result
    consistent: Result
    feasible: Optional[Result] = None

    @property
    def status(self) -> str:
        if self.feasible is not None:
            return self.feasible.status
        statuses = {self.compatible.status, self.consistent.status}
        if statuses == {SAT}:
            return SAT
        return UNSAT if UNSAT in statuses else UNKNOWN

    def __bool__(self) -> bool:
        return self.status == SAT

    @property
    def seconds(self) -> float:
        return sum(r.seconds for r in (self.feasible, self.compatible, self.consistent) if r)

    @property
    def reals(self) -> str | None:
        """How the real variables were handled (see ``Result.reals``)."""
        return _reals(*(r for r in (self.feasible, self.compatible, self.consistent) if r))

    def to_dict(self) -> dict:
        d = {"status": self.status}
        if self.feasible is not None:
            d["feasible"] = self.feasible.to_dict()
        d["compatible"] = self.compatible.to_dict()
        d["consistent"] = self.consistent.to_dict()
        if self.reals:
            d["reals"] = self.reals
        return d


def verify(contract: Contract, backend: str = "auto", context=None, core: bool = False,
           **options) -> Verification:
    """Can the assumptions and the guarantees of the contract hold together
    (A & G satisfiable)? This implies that the contract is compatible and
    consistent; when A & G is unsatisfiable, the two are checked to tell
    whether the assumptions alone cannot hold. With core, an unsat result
    carries ``unsat_core`` of the contract (the terms that conflict)."""
    both = ltl.Binary("&", contract.assumption, contract.guarantee)
    feasible = is_satisfiable(both, contract.variables, backend, context=context, **options)
    if feasible.status == SAT:
        def implied():
            return replace(feasible, seconds=0.0, details={"implied by": "A & G"})
        return Verification(implied(), implied(), feasible)
    if core and feasible.status == UNSAT:
        feasible.core = unsat_core(contract, backend=backend, context=context, **options)
    return Verification(is_compatible(contract, backend, core=core, context=context, **options),
                        is_consistent(contract, backend, core=core, context=context, **options),
                        feasible)


@dataclass
class Redundancy:
    """Outcome of ``remove_redundant``."""
    contract: Contract
    removed_assumptions: list[ltl.Node] = field(default_factory=list)
    removed_guarantees: list[ltl.Node] = field(default_factory=list)
    unknown: list[ltl.Node] = field(default_factory=list)   # checks that did not finish

    def to_dict(self) -> dict:
        return {"contract": self.contract.to_dict(),
                "removed_assumptions": [ltl.to_str(f) for f in self.removed_assumptions],
                "removed_guarantees": [ltl.to_str(f) for f in self.removed_guarantees],
                "unknown": [ltl.to_str(f) for f in self.unknown]}


def remove_redundant(contract: Contract, backend: str = "auto", context=None,
                     **options) -> Redundancy:
    """Drop each term implied by the other terms and the context.

    An assumption is dropped when the other assumptions imply it; a guarantee
    when the assumptions and the other guarantees imply it. Each test is one
    implication check, with the terms that share variables with the term
    (see ``logic._Implications``); a term whose check does not finish is
    kept.
    """
    c = contract
    out = Redundancy(Contract(c.name, dict(c.inputs), dict(c.outputs),
                                  list(c.assumptions), list(c.guarantees)))
    check = _Implications(list(c.assumptions) + list(c.guarantees), c.variables, backend,
                          context, **options)
    kept = list(c.assumptions)
    for term in list(kept):
        others = [t for t in kept if t is not term]
        r = check(others, term)
        if r.status == VALID:
            kept.remove(term)
            out.removed_assumptions.append(term)
        elif r.status == UNKNOWN:
            out.unknown.append(term)
    out.contract.assumptions = kept
    kept_g = list(c.guarantees)
    for term in list(kept_g):
        others = [t for t in kept_g if t is not term]
        r = check(kept + others, term)
        if r.status == VALID:
            kept_g.remove(term)
            out.removed_guarantees.append(term)
        elif r.status == UNKNOWN:
            out.unknown.append(term)
    out.contract.guarantees = kept_g
    return out


def accepts(contract: Contract, environment, backend: str = "auto", context=None,
            **options) -> Result:
    """Does the contract accept the environment, i.e. E -> A? The environment
    is an LTL formula over the variables of the contract, or a state machine
    (a ``Model``, SMV text, or a state machine in JSON; see
    ``chase_cps.models``) that declares the variables of the assumptions:
    every behavior of the machine must satisfy A."""
    from .models import as_model
    m = as_model(environment, contract)
    if m is None:
        return implies(_formula(environment), contract.assumption, contract.variables,
                       backend, context, **options)
    return _holds(m, contract.assumption, contract, f"the assumptions of {contract.name}",
                  backend, context, **options)


def implements(implementation, contract: Contract, backend: str = "auto", context=None,
               **options) -> Result:
    """Does the implementation implement the contract, i.e. M & A -> G? The
    implementation is an LTL formula that describes the behaviors of a
    component, or a state machine: a ``Model``, SMV text, a state machine in
    JSON, or a controller of ``synthesize`` (see ``chase_cps.models``), whose
    every behavior must satisfy A -> G. A machine that refuses inputs can
    implement a contract only because of that: ``is_receptive`` checks it."""
    from .models import as_model
    m = as_model(implementation, contract)
    if m is None:
        return implies(ltl.Binary("&", _formula(implementation), contract.assumption),
                       contract.guarantee, contract.variables, backend, context, **options)
    return _holds(m, contract.saturated_guarantee, contract, f"the contract {contract.name}",
                  backend, context, **options)


def _holds(model, phi, contract: Contract, what: str, backend: str, context,
           **options) -> Result:
    """Does every behavior of the model satisfy phi (within the context)?"""
    from . import backends
    from .queries import Holds
    k = _context(context, contract.variables)
    spec = phi if k is None else ltl.Binary("->", k, phi)
    model.check_variables(ltl.variables(spec), what, contract.variables)
    return backends.check(Holds(model.smv, spec, dict(model.variables)), backend, **options)



from .models import Model, is_receptive  # noqa: E402,F401

# GR(1) synthesis, in chase_cps.gr1 (kept here for the code written before
# 0.3.1).
from .gr1 import (Vacuity, VacuousTerm, is_realizable, is_well_separated,  # noqa: E402,F401
                  remove_vacuous, synthesize)
