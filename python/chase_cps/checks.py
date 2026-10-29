"""Checks on contracts, built on implication and satisfiability.

Contracts are read as saturated: a contract (A, G) allows the behaviors of
A -> G. ``refined`` refines ``abstract`` when A_abstract -> A_refined and
(A_refined -> G_refined) -> (A_abstract -> G_abstract). A contract is
compatible when some environment satisfies A, consistent when some
behavior satisfies A -> G, and ``verify`` checks that A & G is satisfiable;
a component M is an implementation of the contract when M & A -> G
holds (is_implementation), and E is an environment of it when E -> A
(is_environment). M and E are LTL formulas or
state machines (``chase_cps.models``); ``is_receptive`` checks that a state
machine does not refuse the inputs that the assumptions allow.
Each check is one or two queries on formulas (``chase_cps.logic``), so the
checks hold for any theory the back ends decide.

The checks take the back end (``"auto"``, ``"nuxmv"``, ``"nusmv"``, or with
an engine, e.g. ``"nuxmv/bdd"``) and options of the back end as keywords:
``timeout`` in seconds (300 by default), ``engine`` (``"auto"``, ``"ic3"``,
``"bdd"``, ``"bmc"``, ``"portfolio"``, see ``chase_cps.backends.nusmv``) and
``bmc_bound``. They also take a ``context`` (see ``chase_cps.logic``).

The checks of GR(1) synthesis (realizability and the analyses built on it)
are in ``chase_cps.gr1``; they are imported here too, as in the versions
before 0.3.1.
"""

from __future__ import annotations

import warnings
from dataclasses import dataclass, field, replace
from typing import Optional

from . import ltl
from .contract import Contract, VarType
from .logic import (_context, _formula, _Implications, _Search, _within,  # noqa: F401
                    implies, is_satisfiable, unsat_core)
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
class RedundantTerm:
    """A term removed by ``remove_redundant``: ``part`` is assumption or
    guarantee; ``kind`` (with synth=True) init, safety, or justice; ``core``
    (with core=True) a minimal set of the other terms that imply it."""
    term: ltl.Node
    part: str
    kind: Optional[str] = None
    core: Optional[list] = None

    def to_dict(self) -> dict:
        d = {"term": ltl.to_str(self.term), "part": self.part}
        if self.kind is not None:
            d["kind"] = self.kind
        if self.core is not None:
            d["core"] = [ltl.to_str(t) for t in self.core]
        return d


@dataclass
class Redundancy:
    """Outcome of ``remove_redundant``: the contract without the redundant
    terms, the terms removed (in order, and by part), the terms whose check
    did not finish (kept), and the reason when nothing could be checked."""
    contract: Contract
    removed_assumptions: list[ltl.Node] = field(default_factory=list)
    removed_guarantees: list[ltl.Node] = field(default_factory=list)
    unknown: list[ltl.Node] = field(default_factory=list)   # checks that did not finish
    removed: list[RedundantTerm] = field(default_factory=list)
    reason: str = ""

    def to_dict(self) -> dict:
        d = {"contract": self.contract.to_dict(),
             "removed_assumptions": [ltl.to_str(f) for f in self.removed_assumptions],
             "removed_guarantees": [ltl.to_str(f) for f in self.removed_guarantees],
             "removed": [t.to_dict() for t in self.removed],
             "unknown": [ltl.to_str(f) for f in self.unknown]}
        if self.reason:
            d["reason"] = self.reason
        return d


def remove_redundant(contract: Contract, backend: str = "auto", context=None,
                     synth: bool = False, core: bool = False, **options) -> Redundancy:
    """Drop each term implied by the other terms and the context.

    An assumption is dropped when the other assumptions imply it; a guarantee
    when the assumptions and the other guarantees imply it. Each test is one
    implication check, with the terms that share variables with the term
    (see ``logic._Implications``); a term whose check does not finish is
    kept. This keeps the contract, A -> G: use it before the algebra and the
    checks (compose, refines, verify, is_implementation, ...).

    With synth=True, a term is dropped only when it is implied in the GR(1)
    game, which keeps realizability and the controllers: use it before
    synthesis (is_realizable, synthesize, is_well_separated, is_vacuous,
    unrealizable_core). The terms must be GR(1), and there is no context.
    Each kind of term is implied only by some kinds of terms (see
    ``chase_cps.gr1``): a safety guarantee is not dropped because a justice
    assumption implies it, since the component must keep its safety
    guarantees even when the environment breaks its justice assumptions.
    The two differ only when the assumptions are wrong, for instance
    unsatisfiable, or constrain the outputs; each keeps what the other may
    change.

    With core, each removed term carries a minimal set of the terms that
    imply it.
    """
    c = contract
    out = Redundancy(Contract(c.name, dict(c.inputs), dict(c.outputs),
                                  list(c.assumptions), list(c.guarantees)))
    if synth:
        if context is not None:
            raise TypeError("remove_redundant with synth=True does not take a context: "
                            "the realizability checks read every behavior")
        from .gr1 import _redundant_in_game
        removed, out.unknown, out.reason, alive = _redundant_in_game(c, core, backend, **options)
        out.removed = [RedundantTerm(t, part, kind, found) for part, t, kind, found in removed]
        n = len(c.assumptions)
        out.contract.assumptions = [t for j, t in enumerate(c.assumptions) if j in alive]
        out.contract.guarantees = [t for j, t in enumerate(c.guarantees) if j + n in alive]
        out.removed_assumptions = [t.term for t in out.removed if t.part == "assumption"]
        out.removed_guarantees = [t.term for t in out.removed if t.part == "guarantee"]
        return out
    check = _Implications(list(c.assumptions) + list(c.guarantees), c.variables, backend,
                          context, **options)

    def drop(premises, term, part):
        r = check(premises, term)
        if r.status == UNKNOWN:
            out.unknown.append(term)
        if r.status != VALID:
            return False
        found = None
        if core:
            search = _Search(lambda idx: {VALID: True, INVALID: False}.get(
                check([premises[k] for k in idx], term).status))
            idx = search.ddmin(list(range(len(premises))), [])
            found = None if search.reason else [premises[k] for k in idx]
        out.removed.append(RedundantTerm(term, part, None, found))
        return True

    kept = list(c.assumptions)
    for term in list(kept):
        if drop([t for t in kept if t is not term], term, "assumption"):
            kept.remove(term)
            out.removed_assumptions.append(term)
    out.contract.assumptions = kept
    kept_g = list(c.guarantees)
    for term in list(kept_g):
        if drop(kept + [t for t in kept_g if t is not term], term, "guarantee"):
            kept_g.remove(term)
            out.removed_guarantees.append(term)
    out.contract.guarantees = kept_g
    return out


def is_environment(environment, contract: Contract, backend: str = "auto", context=None,
                   **options) -> Result:
    """Is E an environment of the contract (E |=_E C), i.e. does E -> A hold?
    The environment is an LTL formula over the variables of the contract, or
    a state machine (a ``Model``, SMV text, or a state machine in JSON; see
    ``chase_cps.models``) that declares the variables of the assumptions:
    every behavior of the machine must satisfy A."""
    from .models import as_model
    m = as_model(environment, contract)
    if m is None:
        return implies(_formula(environment), contract.assumption, contract.variables,
                       backend, context, **options)
    return _holds(m, contract.assumption, contract, f"the assumptions of {contract.name}",
                  backend, context, **options)


def is_implementation(implementation, contract: Contract, backend: str = "auto",
                      context=None, **options) -> Result:
    """Is M an implementation of the contract (M |=_M C), i.e. does M & A -> G
    hold? The implementation is an LTL formula that describes the behaviors
    of a component, or a state machine: a ``Model``, SMV text, a state
    machine in JSON, or a controller of ``synthesize`` (see
    ``chase_cps.models``), whose every behavior must satisfy A -> G. A
    machine that refuses inputs can implement a contract only because of
    that: ``is_receptive`` checks it."""
    from .models import as_model
    m = as_model(implementation, contract)
    if m is None:
        return implies(ltl.Binary("&", _formula(implementation), contract.assumption),
                       contract.guarantee, contract.variables, backend, context, **options)
    return _holds(m, contract.saturated_guarantee, contract, f"the contract {contract.name}",
                  backend, context, **options)


def accepts(contract: Contract, environment, backend: str = "auto", context=None,
            **options) -> Result:
    """The name of ``is_environment`` before 0.4.2, with the arguments the
    other way round."""
    warnings.warn("accepts(c, e) is now is_environment(e, c)", DeprecationWarning, stacklevel=2)
    return is_environment(environment, contract, backend, context, **options)


def implements(implementation, contract: Contract, backend: str = "auto", context=None,
               **options) -> Result:
    """The name of ``is_implementation`` before 0.4.2."""
    warnings.warn("implements(m, c) is now is_implementation(m, c)", DeprecationWarning,
                  stacklevel=2)
    return is_implementation(implementation, contract, backend, context, **options)


def _holds(model, phi, contract: Contract, what: str, backend: str, context,
           **options) -> Result:
    """Does every behavior of the model satisfy phi (within the context)?"""
    from . import backends
    from .queries import Holds
    k = _context(context, contract.variables)
    spec = phi if k is None else ltl.Binary("->", k, phi)
    for _ in range(getattr(model, "start", 0)):      # the steps of a reset come first
        spec = ltl.Unary("X", spec)
    model.check_variables(ltl.variables(spec), what, contract.variables)
    return backends.check(Holds(model.smv, spec, dict(model.variables)), backend, **options)



from .models import Model, is_receptive  # noqa: E402,F401

# GR(1) synthesis, in chase_cps.gr1 (kept here for the code written before
# 0.3.1).
from .gr1 import is_realizable, is_well_separated, synthesize  # noqa: E402,F401
