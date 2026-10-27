"""GR(1) synthesis: realizability, controllers, and the analyses that need it.

These checks are about one way of using contracts, reactive synthesis from
the GR(1) fragment of LTL with slugs, and not about contracts in general:
a contract is realizable when a component that controls the outputs can
satisfy it against every environment that controls the inputs. The terms
must be GR(1): initial (no temporal operator), safety (G f, f with X only),
or justice (G F f).

- ``is_realizable`` and ``synthesize`` (the controller, or the
  counter-strategy of the environment);
- ``unrealizable_core``: a minimal set of guarantees that is unrealizable,
  justice guarantees first, then safety, then initial ones (QuickCore, Maoz
  and Shalom, ICSE 2021);
- ``is_well_separated``: whether the component can win by forcing the
  environment to violate its assumptions (Klein and Pnueli, HVC 2010; Maoz
  and Ringert, FSE 2016);
- ``remove_vacuous``: the terms whose removal keeps the synthesis problem
  (inherent vacuity, Maoz and Shalom, ESEC/FSE 2020).

These checks read every behavior: they do not take a context.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Optional

from . import backends, ltl
from .contract import Contract
from .logic import FOUND, NONE, UNKNOWN, Core, _Search, _terms, implies
from .queries import (INVALID, NOT_SEPARATED, REALIZABLE, SEPARATED, UNREALIZABLE, VALID,
                      Realizable, Result)


def _no_context(options: dict, check: str) -> None:
    if "context" in options:
        raise TypeError(f"{check} does not take a context: the realizability checks "
                        "read every behavior")


def is_realizable(contract: Contract, backend: str = "auto", core: bool = False,
                  **options) -> Result:
    """GR(1) realizability of the contract: ``realizable``, or ``unrealizable``
    with the counter-strategy of the environment. With core, an unrealizable
    result carries ``unrealizable_core`` of the contract."""
    _no_context(options, "is_realizable")
    r = backends.check(Realizable(contract), backend, **options)
    if core and r.status == UNREALIZABLE:
        r.core = unrealizable_core(contract, backend=backend, **options)
    return r


def synthesize(contract: Contract, backend: str = "auto", **options) -> Result:
    """GR(1) synthesis: ``realizable`` with the controller, or ``unrealizable``
    with the counter-strategy of the environment."""
    _no_context(options, "synthesize")
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
    _no_context(options, "is_well_separated")
    if isinstance(assumptions, Contract):
        c = assumptions
    else:
        if inputs is None or outputs is None:
            raise TypeError("is_well_separated of formulas needs the inputs and the outputs")
        c = Contract.from_strings(inputs=inputs, outputs=outputs,
                                  assumptions=_terms(assumptions) if assumptions else [],
                                  guarantees=_terms(guarantees) if guarantees else [],
                                  name="C")
    for t in list(c.assumptions) + (list(c.guarantees) if with_guarantees else []):
        if gr1_kind(t) is None:
            return Result(UNKNOWN, "slugs", reason=f"not a GR(1) term: {ltl.to_str(t)}")
    kept = [t for t in c.guarantees
            if gr1_kind(t) in ("init", "safety")] if with_guarantees else []
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
        search = _Search(lambda idx: {REALIZABLE: True, UNREALIZABLE: False}.get(
            query([terms[i] for i in idx], False).status))
        found = search.ddmin(list(range(len(terms))), [])
        if search.reason:
            r.core = Core(UNKNOWN, reason=search.reason, checks=search.checks)
        else:
            sub = [terms[i] for i in found]
            r.core = Core(FOUND, Contract(f"{c.name}_core", dict(c.inputs),
                                                      dict(c.outputs), sub, [])
                                if isinstance(assumptions, Contract) else sub,
                                checks=search.checks)
    return r


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
    _no_context(options, "remove_vacuous")
    if isinstance(guarantees, Contract):
        c = guarantees
    else:
        if inputs is None or outputs is None:
            raise TypeError("remove_vacuous of formulas needs the inputs and the outputs")
        c = Contract.from_strings(inputs=inputs, outputs=outputs,
                                  assumptions=_terms(assumptions) if assumptions else [],
                                  guarantees=_terms(guarantees), name="C")
    terms = [("assumption", t) for t in c.assumptions] + [("guarantee", t) for t in c.guarantees]
    kinds = [gr1_kind(t) for _, t in terms]
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
            search = _Search(lambda idx: implied([prem[k] for k in idx], i))
            idx = search.ddmin(list(range(len(prem))), [])
            found = None if search.reason else [terms[prem[k]][1] for k in idx]
        out.removed.append(VacuousTerm(terms[i][1], terms[i][0], kinds[i], found))
    out.contract.assumptions = [t for j, (p, t) in enumerate(terms) if p == "assumption" and j in alive]
    out.contract.guarantees = [t for j, (p, t) in enumerate(terms) if p == "guarantee" and j in alive]
    return out


def _temporal(node: ltl.Node, allow_next: bool) -> bool:
    """Does the formula contain a temporal operator (other than X when
    allow_next is set)?"""
    if isinstance(node, ltl.Unary):
        if node.op in ("G", "F") or (node.op == "X" and not allow_next):
            return True
        return _temporal(node.arg, allow_next)
    if isinstance(node, ltl.Binary):
        if node.op in ("U", "R", "W"):
            return True
        return _temporal(node.left, allow_next) or _temporal(node.right, allow_next)
    return False


def gr1_kind(term: ltl.Node) -> Optional[str]:
    """``init``, ``safety`` (G f, f with X only), ``justice`` (G F f, f with X
    only), or None when the term is not GR(1). The same classes as the slugs
    back end."""
    if not _temporal(term, False):
        return "init"
    if isinstance(term, ltl.Unary) and term.op == "G":
        f = term.arg
        if isinstance(f, ltl.Unary) and f.op == "F" and not _temporal(f.arg, True):
            return "justice"
        if not _temporal(f, True):
            return "safety"
    return None


def _next_vars(node: ltl.Node, under_x: bool = False) -> set[str]:
    """The variables read in the next step: primed, or under X."""
    if isinstance(node, ltl.Var):
        return {node.name} if node.primed or under_x else set()
    if isinstance(node, ltl.Unary):
        return _next_vars(node.arg, under_x or node.op == "X")
    if isinstance(node, ltl.Binary):
        return _next_vars(node.left, under_x) | _next_vars(node.right, under_x)
    return set()


def gr1_roles(c: Contract) -> Optional[str]:
    """Why the contract is not a GR(1) specification for slugs, or None: each
    term is GR(1), the initial assumptions read the inputs only, and the
    safety assumptions read the inputs only in the next step."""
    for t in list(c.assumptions) + list(c.guarantees):
        if gr1_kind(t) is None:
            return f"not a GR(1) term: {ltl.to_str(t)}"
    for t in c.assumptions:
        kind = gr1_kind(t)
        if kind == "init":
            out = ltl.variables(t) - set(c.inputs)
            if out:
                return (f"the initial assumption {ltl.to_str(t)} reads {sorted(out)}: initial "
                        "assumptions can only read inputs")
        elif kind == "safety":
            out = _next_vars(t.arg) - set(c.inputs)
            if out:
                return (f"the safety assumption {ltl.to_str(t)} reads the next value of "
                        f"{sorted(out)}: the environment cannot depend on the next outputs")
    return None


def unrealizable_core(guarantees, inputs=None, outputs=None, assumptions=(),
                      backend: str = "auto", **options) -> Core:
    """A minimal set of guarantees that is unrealizable under all the
    assumptions (GR(1), with slugs).

    ``unrealizable_core(contract)`` returns a contract with the assumptions
    and the guarantees of the core. ``unrealizable_core(guarantees, inputs,
    outputs, assumptions)`` takes formulas and returns the formulas of the
    core; inputs and outputs are needed, since realizability depends on who
    controls each variable.
    """
    _no_context(options, "unrealizable_core")
    if isinstance(guarantees, Contract):
        c, from_contract = guarantees, True
    else:
        if inputs is None or outputs is None:
            raise TypeError("unrealizable_core of formulas needs the inputs and the outputs")
        c = Contract.from_strings(inputs=inputs, outputs=outputs,
                                  assumptions=_terms(assumptions) if assumptions else [],
                                  guarantees=_terms(guarantees), name="C")
        from_contract = False
    terms = list(c.guarantees)
    problem = gr1_roles(c)
    if problem:
        return Core(UNKNOWN, reason=problem)
    kinds = [gr1_kind(t) for t in terms]
    env_other = [t for t in c.assumptions if gr1_kind(t) != "justice"]

    def fails(idx):
        g = [terms[i] for i in idx]
        # Without justice guarantees the justice assumptions play no role
        # (QuickCore); leaving them out makes the check cheaper.
        a = c.assumptions if any(kinds[i] == "justice" for i in idx) else env_other
        r = backends.check(Realizable(Contract(c.name, dict(c.inputs), dict(c.outputs),
                                               list(a), g)),
                           backend, counter_strategy=False, **options)
        return {UNREALIZABLE: True, REALIZABLE: False}.get(r.status)

    search = _Search(fails)
    init = [i for i, k in enumerate(kinds) if k == "init"]
    safety = [i for i, k in enumerate(kinds) if k == "safety"]
    justice = [i for i, k in enumerate(kinds) if k == "justice"]
    if not search.fails(init + safety + justice):
        return Core(UNKNOWN if search.reason else NONE, reason=search.reason,
                    checks=search.checks)
    justice = search.ddmin(justice, init + safety)
    safety = search.ddmin(safety, init + justice)
    init = search.ddmin(init, safety + justice)
    if search.reason:
        return Core(UNKNOWN, reason=search.reason, checks=search.checks)
    core = [terms[i] for i in sorted(init + safety + justice)]
    if not from_contract:
        return Core(FOUND, core, checks=search.checks)
    return Core(FOUND, Contract(f"{c.name}_core", dict(c.inputs), dict(c.outputs),
                                list(c.assumptions), core), checks=search.checks)
