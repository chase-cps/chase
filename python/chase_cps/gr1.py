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
- ``is_vacuous``: whether the component can win without an output that a
  liveness guarantee asks for (vacuity in synthesis, Bloem, Chockler,
  Ebrahimi, and Strichman, FMSD 2021);
- the terms implied by others in the GR(1) game, whose removal keeps the
  synthesis problem, for ``remove_redundant(c, synth=True)`` (the inherent
  vacuity of Maoz and Shalom, ESEC/FSE 2020).

These checks read every behavior: they do not take a context.
"""

from __future__ import annotations

import time
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
                      with_guarantees: bool = True, core: bool = False,
                      backend: str = "auto", **options) -> Result:
    """Can the component be forced to win by making the environment violate
    its assumptions? ``separated`` when it cannot, ``not_separated`` with the
    ``controller`` that forces the violation, or ``unknown``.

    Takes a contract, or the assumptions with the inputs and the outputs.
    The check is GR(1) realizability with slugs of the system specification
    (safe(G), {false}): the component must keep its initial and safety
    guarantees while it forces the violation (Klein and Pnueli, HVC 2010;
    Maoz and Ringert, FSE 2016, Def. 4). A violation that the component can
    force only by breaking its own guarantees is not a defect of the
    assumptions. With ``with_guarantees=False`` the guarantees are left out,
    as in Def. 2 (the default before 0.3.2). Only the case where the violation
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


def _redundant_in_game(c: Contract, core: bool, backend: str, **options):
    """The terms of c implied by others in the GR(1) game, for
    ``remove_redundant(c, synth=True)``: removing them keeps realizability
    and the controllers (the inherent vacuity of Maoz and Shalom, ESEC/FSE
    2020, Table 1).

    A term is redundant when its premise set implies it, one check with
    nuXmv: an initial term, by the other initial terms (and, for a guarantee,
    the initial assumptions); a safety term G t, by the bodies of the other
    safety terms of its side (and, for a guarantee, of the safety
    assumptions), as a propositional implication over the current and next
    values; a justice term G F j, by the initial and safety terms of both
    sides and the other justice terms (for a guarantee, also the justice
    assumptions). The terms are checked one at a time, in order, against the
    terms not yet removed.

    Returns the removed terms as (part, term, kind, core), the terms whose
    check did not finish, the reason when nothing could be checked, and the
    indices of the terms kept in ``c.assumptions + c.guarantees``.
    """
    terms = [("assumption", t) for t in c.assumptions] + [("guarantee", t) for t in c.guarantees]
    kinds = [gr1_kind(t) for _, t in terms]
    alive = list(range(len(terms)))
    for (_, t), k in zip(terms, kinds):
        if k is None:
            return [], [], f"not a GR(1) term: {ltl.to_str(t)}", alive
    variables = c.variables
    removed, unknown = [], []

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
            unknown.append(terms[i][1])
            continue
        if not verdict:
            continue
        alive.remove(i)
        found = None
        if core:
            search = _Search(lambda idx: implied([prem[k] for k in idx], i))
            idx = search.ddmin(list(range(len(prem))), [])
            found = None if search.reason else [terms[prem[k]][1] for k in idx]
        removed.append((terms[i][0], terms[i][1], kinds[i], found))
    return removed, unknown, "", alive


VACUOUS, NOT_VACUOUS = "vacuous", "not_vacuous"


@dataclass
class VacuousOutput:
    """An output that the contract does not oblige the component to use
    (``is_vacuous``). ``mode`` is ``never`` (a winning strategy never sets
    it) or ``eventually`` (a winning strategy stops setting it from some step
    on); ``guarantees`` are the justice guarantees that name it;
    ``controller`` is that strategy; with core, ``core`` is a minimal set of
    assumptions under which it still wins."""
    output: str
    mode: str
    guarantees: list[ltl.Node] = field(default_factory=list)
    controller: Optional[dict] = None
    core: Optional[Core] = None

    def to_dict(self) -> dict:
        d = {"output": self.output, "mode": self.mode,
             "guarantees": [ltl.to_str(t) for t in self.guarantees]}
        if self.controller is not None:
            d["controller"] = self.controller
        if self.core is not None:
            d["core"] = self.core.to_dict()
        return d


@dataclass
class OutputVacuity:
    """Outcome of ``is_vacuous``: ``vacuous`` with the vacuous outputs,
    ``not_vacuous``, ``unrealizable`` (the contract itself), or ``unknown``
    with the reason. ``checked`` are the outputs that were checked."""
    status: str
    vacuous: list[VacuousOutput] = field(default_factory=list)
    checked: list[str] = field(default_factory=list)
    reason: str = ""
    seconds: float = 0.0

    def __bool__(self) -> bool:
        return self.status == VACUOUS

    def to_dict(self) -> dict:
        d = {"status": self.status, "vacuous": [v.to_dict() for v in self.vacuous],
             "checked": list(self.checked), "seconds": round(self.seconds, 3)}
        if self.reason:
            d["reason"] = self.reason
        return d


def _polarity(node: ltl.Node, positive: bool = True, out=None) -> dict[str, set[bool]]:
    """For each variable, the polarities of its occurrences: True under an
    even number of negations, False under an odd one, both inside an
    equivalence or a relation."""
    out = {} if out is None else out
    if isinstance(node, ltl.Var):
        out.setdefault(node.name, set()).add(positive)
    elif isinstance(node, ltl.Unary):
        _polarity(node.arg, not positive if node.op == "!" else positive, out)
    elif isinstance(node, ltl.Binary):
        if node.op == "->":
            _polarity(node.left, not positive, out)
            _polarity(node.right, positive, out)
        elif node.op in ("&", "|", "U", "R", "W"):
            _polarity(node.left, positive, out)
            _polarity(node.right, positive, out)
        else:      # <->, relations, arithmetic
            for side in (node.left, node.right):
                _polarity(side, True, out)
                _polarity(side, False, out)
    return out


def _liveness_outputs(c: Contract) -> dict[str, list[ltl.Node]]:
    """The outputs that occur only positively in a justice guarantee, with
    those guarantees: the outputs that the guarantee asks to set."""
    found: dict[str, list[ltl.Node]] = {}
    for t in c.guarantees:
        if gr1_kind(t) != "justice":
            continue
        for y, signs in _polarity(t).items():
            if y in c.outputs and signs == {True}:
                found.setdefault(y, []).append(t)
    return found


def is_vacuous(contract: Contract, outputs=None, core: bool = False, backend: str = "auto",
               **options) -> OutputVacuity:
    """Does the contract let the component win without using an output that
    a liveness guarantee asks for? Vacuity in synthesis, as a strengthening
    of the guarantees (Bloem, Chockler, Ebrahimi, and Strichman, FMSD 2021):

    - never: Real(A, G + {G !y}), a winning strategy never sets y, so the
      guarantee that names y is discharged by the assumptions;
    - eventually: Real(A, G + {!p, G (p -> X p), G (p -> !y), G F p}) with a
      fresh output p, a winning strategy stops setting y forever from some
      step on (checked only when ``never`` fails).

    Two weaker formulations are not used. The test on a controller M of
    Kupferman and Vardi, M & A -> phi[y <- false], and the rewrite
    Real(A, G[y <- false]) both call a good controller vacuous when an
    assumption ties the release of the environment to y, such as
    G ((req & grant) -> X !req): the controller grants, the release
    follows, and phi[grant <- false] holds because it granted. Forcing y to
    be false in the behavior asks the right question.

    The outputs checked are ``outputs``, or by default those that occur
    only positively in a justice guarantee. ``vacuous`` gives each vacuous
    output with the strategy that wins without it; with core, also a
    minimal set of the temporal assumptions under which it still wins (the
    initial assumptions are always kept). The contract must be GR(1) and
    realizable: otherwise the status is ``unknown`` or ``unrealizable``.
    """
    _no_context(options, "is_vacuous")
    c = contract
    why = gr1_roles(c)
    if why:
        return OutputVacuity(UNKNOWN, reason=why)
    named = _liveness_outputs(c)
    if outputs is None:
        targets = list(named)
    else:
        targets = [outputs] if isinstance(outputs, str) else list(outputs)
        for y in targets:
            if y not in c.outputs:
                raise ValueError(f"{y} is not an output of {c.name}")
    start = time.monotonic()
    base = is_realizable(c, backend, **options)
    if base.status != REALIZABLE:
        return OutputVacuity(base.status, checked=targets, seconds=time.monotonic() - start,
                             reason=base.reason or ("the contract is unrealizable"
                                                    if base.status == UNREALIZABLE else ""))
    out = OutputVacuity(NOT_VACUOUS, checked=targets)
    unknown = []

    def strengthened(y, mode, assumptions):
        if mode == "never":
            return Contract(c.name, dict(c.inputs), dict(c.outputs), list(assumptions),
                            list(c.guarantees) + [ltl.parse(f"G !{y}")])
        p = f"parked_{y}"
        while p in c.variables:
            p = "_" + p
        extra = [f"!{p}", f"G ({p} -> X {p})", f"G ({p} -> !{y})", f"G F {p}"]
        return Contract(c.name, dict(c.inputs), {**c.outputs, p: "bool"}, list(assumptions),
                        list(c.guarantees) + [ltl.parse(f) for f in extra])

    def realizable(y, mode, assumptions, controller=False):
        return backends.check(Realizable(strengthened(y, mode, assumptions)), backend,
                              controller=controller, counter_strategy=False, **options)

    for y in targets:
        for mode in ("never", "eventually"):
            r = realizable(y, mode, c.assumptions, controller=True)
            if r.status == REALIZABLE:
                v = VacuousOutput(y, mode, named.get(y, []), r.controller)
                if core:
                    v.core = _assumption_core(c, lambda a, y=y, mode=mode:
                                              realizable(y, mode, a).status)
                out.vacuous.append(v)
                break
            if r.status != UNREALIZABLE:
                unknown.append(f"{y}: {r.reason or r.status}")
                break
    if out.vacuous:
        out.status = VACUOUS
    elif unknown:
        out.status, out.reason = UNKNOWN, "; ".join(unknown)
    out.seconds = time.monotonic() - start
    return out


def _assumption_core(c: Contract, status) -> Core:
    """A minimal set of the temporal assumptions with which the strengthened
    contract is still realizable, by one deletion pass (more assumptions
    only help the component); the initial assumptions are kept."""
    pinned = [t for t in c.assumptions if gr1_kind(t) == "init"]
    kept = [t for t in c.assumptions if gr1_kind(t) != "init"]
    checks = 0
    for t in list(kept):
        rest = [k for k in kept if k is not t]
        r = status(pinned + rest)
        checks += 1
        if r == REALIZABLE:
            kept = rest
        elif r != UNREALIZABLE:
            return Core(UNKNOWN, reason=f"a check did not finish ({r})", checks=checks)
    return Core(FOUND, kept, checks=checks)


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


# The results are kept on the contracts checked, for the diagrams (chase_cps.viz).
from .contract import _remembered  # noqa: E402

is_realizable = _remembered("realizable")(is_realizable)
synthesize = _remembered("realizable")(synthesize)
is_well_separated = _remembered("well_separated")(is_well_separated)
is_vacuous = _remembered("vacuous")(is_vacuous)
