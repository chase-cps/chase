"""Checks on contracts, all built on implication and satisfiability.

Contracts are read as saturated: a contract (A, G) allows the behaviors of
A -> G. ``refined`` refines ``abstract`` when A_abstract -> A_refined and
(A_refined -> G_refined) -> (A_abstract -> G_abstract).

The checks take the back end (``"auto"``, ``"nuxmv"``, ``"nusmv"``) and
options of the back end as keywords: ``timeout`` in seconds (300 by
default), and for NuSMV ``engine`` (``"auto"``, ``"bdd"``, ``"bmc"``) and
``bmc_bound``.
"""

from __future__ import annotations

from dataclasses import dataclass, field

from . import backends, ltl
from .contract import Contract, VarType
from .queries import (INVALID, SAT, UNKNOWN, UNSAT, VALID, Implies, Realizable,
                      Result, Satisfiable)


def _formula(f) -> ltl.Node:
    return ltl.parse(f) if isinstance(f, str) else f


def _merge_vars(*contracts: Contract) -> dict[str, VarType]:
    out: dict[str, VarType] = {}
    for c in contracts:
        for k, v in c.variables.items():
            if k in out and out[k] != v:
                raise ValueError(f"variable {k} has types {out[k]} and {v}")
            out[k] = v
    return out


def implies(phi, psi, variables: dict[str, VarType], backend: str = "auto",
            **options) -> Result:
    """Is phi -> psi valid? ``valid``, or ``invalid`` with a counterexample trace
    (a behavior satisfying phi and violating psi)."""
    return backends.check(Implies(_formula(phi), _formula(psi), dict(variables)),
                          backend, **options)


def satisfiable(phi, variables: dict[str, VarType], backend: str = "auto",
                **options) -> Result:
    """Is phi satisfiable? ``sat`` with a witness trace, or ``unsat``."""
    return backends.check(Satisfiable(_formula(phi), dict(variables)), backend, **options)


def realizable(contract: Contract, backend: str = "auto", **options) -> Result:
    """GR(1) realizability of the contract: ``realizable`` with a controller, or
    ``unrealizable`` with the counter-strategy of the environment."""
    return backends.check(Realizable(contract), backend, **options)


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

    def to_dict(self) -> dict:
        return {"status": self.status, "failed": self.failed,
                "assumptions": self.assumptions.to_dict(),
                "guarantees": self.guarantees.to_dict()}


def refines(refined: Contract, abstract: Contract, backend: str = "auto",
            **options) -> RefinementResult:
    """Does ``refined`` refine ``abstract`` (can it replace it)?"""
    variables = _merge_vars(refined, abstract)
    a = implies(abstract.assumption, refined.assumption, variables, backend, **options)
    g = implies(refined.saturated_guarantee, abstract.saturated_guarantee, variables, backend,
                **options)
    if a.status == VALID and g.status == VALID:
        status = VALID
    elif INVALID in (a.status, g.status):
        status = INVALID
    else:
        status = UNKNOWN
    return RefinementResult(status, a, g)


def consistent(contract: Contract, backend: str = "auto", **options) -> Result:
    """Is there a behavior allowed by the contract (A -> G satisfiable)?"""
    return satisfiable(contract.saturated_guarantee, contract.variables, backend, **options)


def compatible(contract: Contract, backend: str = "auto", **options) -> Result:
    """Is there an environment allowed by the contract (A satisfiable)?"""
    return satisfiable(contract.assumption, contract.variables, backend, **options)


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


def remove_redundant(contract: Contract, backend: str = "auto", **options) -> Redundancy:
    """Drop each term implied by the other terms and the context.

    An assumption is dropped when the other assumptions imply it; a guarantee
    when the assumptions and the other guarantees imply it. Each test is one
    implication check; a term whose check does not finish is kept.
    """
    c = contract
    variables = c.variables
    out = Redundancy(Contract(c.name, dict(c.inputs), dict(c.outputs),
                                  list(c.assumptions), list(c.guarantees)))
    kept = list(c.assumptions)
    for term in list(kept):
        others = [t for t in kept if t is not term]
        r = implies(ltl.conjunction(others), term, variables, backend, **options)
        if r.status == VALID:
            kept.remove(term)
            out.removed_assumptions.append(term)
        elif r.status == UNKNOWN:
            out.unknown.append(term)
    out.contract.assumptions = kept
    context = ltl.conjunction(kept)
    kept_g = list(c.guarantees)
    for term in list(kept_g):
        others = [t for t in kept_g if t is not term]
        r = implies(ltl.conjunction([context] + others), term, variables, backend, **options)
        if r.status == VALID:
            kept_g.remove(term)
            out.removed_guarantees.append(term)
        elif r.status == UNKNOWN:
            out.unknown.append(term)
    out.contract.guarantees = kept_g
    return out


def contains_environment(contract: Contract, environment, backend: str = "auto",
                         **options) -> Result:
    """Is the environment (an LTL formula over the variables of the contract)
    accepted by the contract, i.e. E -> A?"""
    return implies(_formula(environment), contract.assumption, contract.variables, backend,
                   **options)


def contains_implementation(contract: Contract, implementation, backend: str = "auto",
                            **options) -> Result:
    """Does the implementation (an LTL formula describing the behaviors of a
    component) satisfy the contract, i.e. M & A -> G?"""
    return implies(ltl.Binary("&", _formula(implementation), contract.assumption),
                   contract.guarantee, contract.variables, backend, **options)

