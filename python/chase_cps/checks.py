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
from typing import Optional

from . import backends, cores, ltl
from .contract import Contract, VarType
from .queries import (INVALID, NOT_SEPARATED, REALIZABLE, SAT, SEPARATED, UNKNOWN,
                      UNREALIZABLE, UNSAT, VALID, Implies, Realizable, Result, Satisfiable)


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


def is_satisfiable(phi, variables: dict[str, VarType], backend: str = "auto",
                   core: bool = False, **options) -> Result:
    """Is phi satisfiable? ``sat`` with a witness trace, or ``unsat``. With
    core, an unsat result carries ``unsat_core`` of the conjuncts of phi."""
    r = backends.check(Satisfiable(_formula(phi), dict(variables)), backend, **options)
    if core and r.status == UNSAT:
        r.core = cores.unsat_core(_formula(phi), variables, backend, **options)
    return r


def is_realizable(contract: Contract, backend: str = "auto", core: bool = False,
                  **options) -> Result:
    """GR(1) realizability of the contract: ``realizable``, or ``unrealizable``
    with the counter-strategy of the environment. With core, an unrealizable
    result carries ``unrealizable_core`` of the contract."""
    r = backends.check(Realizable(contract), backend, **options)
    if core and r.status == UNREALIZABLE:
        r.core = cores.unrealizable_core(contract, backend=backend, **options)
    return r


def synthesize(contract: Contract, backend: str = "auto", **options) -> Result:
    """GR(1) synthesis: ``realizable`` with the controller, or ``unrealizable``
    with the counter-strategy of the environment."""
    return backends.check(Realizable(contract), backend, controller=True, **options)


def is_well_separated(assumptions, inputs=None, outputs=None, guarantees=(),
                      with_guarantees: bool = False, core: bool = False,
                      backend: str = "auto", **options) -> Result:
    """Can the component be forced to win by making the environment violate
    its assumptions? ``separated`` when it cannot, ``not_separated`` with the
    ``controller`` that forces the violation, or ``unknown``.

    Takes a contract, or the assumptions with the inputs and the outputs.
    The check is GR(1) realizability with slugs of the system specification
    (true, true, {false}) (Klein and Pnueli, HVC 2010; Maoz and Ringert, FSE
    2016, Def. 2). With ``with_guarantees`` the component must also keep its
    initial and safety guarantees (Def. 4). Only the case where the violation
    can be forced from every initial state is detected (P-all): slugs does
    not give the winning region needed for the case of some reachable state.
    With core, a ``not_separated`` result carries a minimal set of
    assumptions that is still not separated.
    """
    if isinstance(assumptions, Contract):
        c = assumptions
    else:
        if inputs is None or outputs is None:
            raise TypeError("is_well_separated of formulas needs the inputs and the outputs")
        c = Contract.from_strings(inputs=inputs, outputs=outputs,
                                  assumptions=cores._terms(assumptions) if assumptions else [],
                                  guarantees=cores._terms(guarantees) if guarantees else [],
                                  name="C")
    for t in list(c.assumptions) + (list(c.guarantees) if with_guarantees else []):
        if cores.gr1_kind(t) is None:
            return Result(UNKNOWN, "slugs", reason=f"not a GR(1) term: {ltl.to_str(t)}")
    kept = [t for t in c.guarantees
            if cores.gr1_kind(t) in ("init", "safety")] if with_guarantees else []
    goal = kept + [ltl.Unary("G", ltl.Unary("F", ltl.Const(False)))]

    def query(a, controller):
        return backends.check(Realizable(Contract(c.name, dict(c.inputs), dict(c.outputs),
                                                  list(a), goal)),
                              backend, controller=controller, counter_strategy=False,
                              **options)

    r = query(c.assumptions, True)
    if r.status == REALIZABLE:
        r.status = NOT_SEPARATED
    elif r.status == UNREALIZABLE:
        r.status = SEPARATED
    if core and r.status == NOT_SEPARATED:
        terms = list(c.assumptions)
        search = cores._Search(lambda idx: {REALIZABLE: True, UNREALIZABLE: False}.get(
            query([terms[i] for i in idx], False).status))
        found = search.ddmin(list(range(len(terms))), [])
        if search.reason:
            r.core = cores.Core(cores.UNKNOWN, reason=search.reason, checks=search.checks)
        else:
            sub = [terms[i] for i in found]
            r.core = cores.Core(cores.FOUND, Contract(f"{c.name}_core", dict(c.inputs),
                                                      dict(c.outputs), sub, [])
                                if isinstance(assumptions, Contract) else sub,
                                checks=search.checks)
    return r


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


def is_consistent(contract: Contract, backend: str = "auto", core: bool = False,
                  **options) -> Result:
    """Is there a behavior allowed by the contract (A -> G satisfiable)? A -> G
    is unsatisfiable only when A is valid and G unsatisfiable; with core, an
    unsat result carries ``unsat_core`` of the guarantees."""
    r = is_satisfiable(contract.saturated_guarantee, contract.variables, backend, **options)
    if core and r.status == UNSAT:
        r.core = cores.unsat_core(_only(contract, guarantees=True), backend=backend, **options)
    return r


def is_compatible(contract: Contract, backend: str = "auto", core: bool = False,
                  **options) -> Result:
    """Is there an environment allowed by the contract (A satisfiable)? With
    core, an unsat result carries ``unsat_core`` of the assumptions."""
    r = is_satisfiable(contract.assumption, contract.variables, backend, **options)
    if core and r.status == UNSAT:
        r.core = cores.unsat_core(_only(contract, guarantees=False), backend=backend, **options)
    return r


def _only(c: Contract, guarantees: bool) -> Contract:
    """The contract with its guarantees only, or its assumptions only."""
    if guarantees:
        return Contract(c.name, dict(c.inputs), dict(c.outputs), [], list(c.guarantees))
    return Contract(c.name, dict(c.inputs), dict(c.outputs), list(c.assumptions), [])


@dataclass
class Verification:
    """Outcome of ``verify``: compatibility and consistency of a contract."""
    compatible: Result
    consistent: Result

    @property
    def status(self) -> str:
        statuses = {self.compatible.status, self.consistent.status}
        if statuses == {SAT}:
            return SAT
        return UNSAT if UNSAT in statuses else UNKNOWN

    def __bool__(self) -> bool:
        return self.status == SAT

    def to_dict(self) -> dict:
        return {"status": self.status, "compatible": self.compatible.to_dict(),
                "consistent": self.consistent.to_dict()}


def verify(contract: Contract, backend: str = "auto", **options) -> Verification:
    """Compatibility and consistency of the contract (the verify command of
    logics_tool). The status is sat when both hold."""
    return Verification(is_compatible(contract, backend, **options),
                        is_consistent(contract, backend, **options))


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


@dataclass
class VacuousTerm:
    """A term removed by ``remove_vacuous``: ``part`` is assumption or
    guarantee, ``kind`` init, safety, or justice; ``core`` (with core=True)
    the terms that make it vacuous."""
    term: ltl.Node
    part: str
    kind: str
    core: Optional[list] = None

    def to_dict(self) -> dict:
        d = {"term": ltl.to_str(self.term), "part": self.part, "kind": self.kind}
        if self.core is not None:
            d["core"] = [ltl.to_str(t) for t in self.core]
        return d


@dataclass
class Vacuity:
    """Outcome of ``remove_vacuous``: the contract without the vacuous terms,
    the terms removed in order, the terms whose check did not finish (kept),
    and the reason when nothing could be checked."""
    contract: Contract
    removed: list[VacuousTerm] = field(default_factory=list)
    unknown: list[ltl.Node] = field(default_factory=list)
    reason: str = ""

    @property
    def assumptions(self) -> list[ltl.Node]:
        return self.contract.assumptions

    @property
    def guarantees(self) -> list[ltl.Node]:
        return self.contract.guarantees

    def to_dict(self) -> dict:
        d = {"contract": self.contract.to_dict(),
             "removed": [t.to_dict() for t in self.removed],
             "unknown": [ltl.to_str(f) for f in self.unknown]}
        if self.reason:
            d["reason"] = self.reason
        return d


def remove_vacuous(guarantees, inputs=None, outputs=None, assumptions=(), core: bool = False,
                   backend: str = "auto", **options) -> Vacuity:
    """Remove the terms whose removal does not change the GR(1)
    specification: its realizability and the controllers that realize it
    (inherent vacuity, Maoz and Shalom, ESEC/FSE 2020, Table 1).

    A term is vacuous when its premise set implies it, one check with nuXmv:
    an initial term, by the other initial terms (and, for a guarantee, the
    initial assumptions); a safety term G t, by the bodies of the other
    safety terms of its side (and, for a guarantee, of the safety
    assumptions), as a propositional implication over the current and next
    values; a justice term G F j, by the initial and safety terms of both
    sides and the other justice terms (for a guarantee, also the justice
    assumptions). The terms are checked one at a time, in order, against the
    terms not yet removed. Unlike ``remove_redundant``, which keeps A -> G,
    this keeps realizability: a guarantee implied only with the help of an
    initial or a justice term is not removed when that would change the game.

    Takes a contract, or the guarantees with the inputs, the outputs, and the
    assumptions. With core, each removed term carries a minimal subset of its
    premise set that implies it.
    """
    if isinstance(guarantees, Contract):
        c = guarantees
    else:
        if inputs is None or outputs is None:
            raise TypeError("remove_vacuous of formulas needs the inputs and the outputs")
        c = Contract.from_strings(inputs=inputs, outputs=outputs,
                                  assumptions=cores._terms(assumptions) if assumptions else [],
                                  guarantees=cores._terms(guarantees), name="C")
    terms = [("assumption", t) for t in c.assumptions] + [("guarantee", t) for t in c.guarantees]
    kinds = [cores.gr1_kind(t) for _, t in terms]
    out = Vacuity(Contract(c.name, dict(c.inputs), dict(c.outputs),
                           list(c.assumptions), list(c.guarantees)))
    for (_, t), k in zip(terms, kinds):
        if k is None:
            out.reason = f"not a GR(1) term: {ltl.to_str(t)}"
            return out
    variables = c.variables
    alive = list(range(len(terms)))

    def body(i):     # the formula a premise contributes
        t = terms[i][1]
        return t.arg if kinds[i] == "safety" else t

    def premises(i):
        part, kind = terms[i][0], kinds[i]
        env = part == "assumption"
        same_side = lambda j: terms[j][0] == part
        if kind == "init":
            return [j for j in alive if j != i and kinds[j] == "init" and (same_side(j) or not env)]
        if kind == "safety":
            return [j for j in alive if j != i and kinds[j] == "safety" and (same_side(j) or not env)]
        return [j for j in alive if j != i and (kinds[j] in ("init", "safety") or
                                                (kinds[j] == "justice" and (same_side(j) or not env)))]

    def implied(prem, i):
        if kinds[i] == "safety":   # propositional parts: the bodies of the G
            phi, psi = ltl.conjunction([body(j) for j in prem]), body(i)
        else:
            phi, psi = ltl.conjunction([terms[j][1] for j in prem]), terms[i][1]
        return {VALID: True, INVALID: False}.get(
            implies(phi, psi, variables, backend, **options).status)

    for i in list(alive):
        prem = premises(i)
        verdict = implied(prem, i)
        if verdict is None:
            out.unknown.append(terms[i][1])
            continue
        if not verdict:
            continue
        alive.remove(i)
        found = None
        if core:
            search = cores._Search(lambda idx: implied([prem[k] for k in idx], i))
            idx = search.ddmin(list(range(len(prem))), [])
            found = None if search.reason else [terms[prem[k]][1] for k in idx]
        out.removed.append(VacuousTerm(terms[i][1], terms[i][0], kinds[i], found))
    out.contract.assumptions = [t for j, (p, t) in enumerate(terms) if p == "assumption" and j in alive]
    out.contract.guarantees = [t for j, (p, t) in enumerate(terms) if p == "guarantee" and j in alive]
    return out


def accepts(contract: Contract, environment, backend: str = "auto", **options) -> Result:
    """Does the contract accept the environment (an LTL formula over the
    variables of the contract), i.e. E -> A?"""
    return implies(_formula(environment), contract.assumption, contract.variables, backend,
                   **options)


def implements(implementation, contract: Contract, backend: str = "auto",
               **options) -> Result:
    """Does the implementation (an LTL formula describing the behaviors of a
    component) implement the contract, i.e. M & A -> G?"""
    return implies(ltl.Binary("&", _formula(implementation), contract.assumption),
                   contract.guarantee, contract.variables, backend, **options)

