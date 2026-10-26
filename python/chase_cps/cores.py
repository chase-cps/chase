"""Cores: the smallest sets of terms that explain a failed check.

``unsat_core`` finds a minimal set of terms that cannot hold together;
``unrealizable_core`` a minimal set of guarantees that is unrealizable under
the assumptions. Both take a contract, and return a contract with the terms
of the core, or formulas, and return formulas.

Both search with delta debugging (DDMin, Zeller and Hildebrandt, TSE 2002),
one call to the back end per test, and remember the sets already tested: a
superset of a failing set fails, a subset of a passing set passes.
``unrealizable_core`` minimizes the justice guarantees first, then the
safety, then the initial ones, as QuickCore does (Maoz and Shalom, ICSE
2021). The cores are minimal: removing any term of the core makes the check
pass. When a test does not finish, the core is not reported (status
unknown).
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Callable, Optional

from . import backends, ltl
from .contract import Contract, VarType
from .queries import REALIZABLE, SAT, UNREALIZABLE, UNSAT, Realizable, Satisfiable

FOUND, NONE, UNKNOWN = "found", "none", "unknown"


@dataclass
class Core:
    """Outcome of ``unsat_core`` or ``unrealizable_core``.

    status is found (``core`` holds the terms: a contract when the input was
    a contract, else a list of formulas), none (the check does not fail:
    nothing to explain), or unknown with the reason. ``checks`` is the number
    of calls to the back end.
    """
    status: str
    core: Any = None
    reason: str = ""
    checks: int = 0

    def __bool__(self) -> bool:
        return self.status == FOUND

    def to_dict(self) -> dict:
        core = self.core
        if isinstance(core, Contract):
            core = core.to_dict()
        elif core is not None:
            core = [ltl.to_str(f) for f in core]
        return {"status": self.status, "core": core, "reason": self.reason,
                "checks": self.checks}


class _Search:
    """Delta debugging over item indices, for a monotonic test: a superset of
    a failing set fails."""

    def __init__(self, fails: Callable[[list[int]], Optional[bool]]):
        self._fails = fails
        self.checks = 0
        self.reason = ""
        self._failing: list[frozenset] = []
        self._passing: list[frozenset] = []

    def fails(self, items: list[int]) -> bool:
        s = frozenset(items)
        if any(f <= s for f in self._failing):
            return True
        if any(s <= p for p in self._passing):
            return False
        self.checks += 1
        verdict = self._fails(sorted(s))
        if verdict is None:
            self.reason = self.reason or "a check did not finish"
            return False
        (self._failing if verdict else self._passing).append(s)
        return verdict

    def ddmin(self, items: list[int], base: list[int]) -> list[int]:
        """A 1-minimal subset of items that fails together with base; base +
        items must fail."""
        if self.fails(base):
            return []
        n = 2
        while len(items) >= 2:
            size = -(-len(items) // n)
            chunks = [items[i:i + size] for i in range(0, len(items), size)]
            for c in chunks:
                if self.fails(base + c):
                    items, n = c, 2
                    break
            else:
                for c in chunks:
                    rest = [x for x in items if x not in c]
                    if self.fails(base + rest):
                        items, n = rest, max(n - 1, 2)
                        break
                else:
                    if n >= len(items):
                        break
                    n = min(len(items), 2 * n)
        return items


def _terms(formulas) -> list[ltl.Node]:
    if isinstance(formulas, (str, ltl.Node)):
        f = ltl.parse(formulas) if isinstance(formulas, str) else formulas
        return ltl.conjuncts(f)
    return [ltl.parse(f) if isinstance(f, str) else f for f in formulas]


def unsat_core(formulas, variables: Optional[dict[str, VarType]] = None,
               backend: str = "auto", **options) -> Core:
    """A minimal set of terms that cannot hold together.

    ``unsat_core(contract)`` searches the assumptions and the guarantees of
    the contract (A & G) and returns a contract with the terms of the core:
    its assumptions alone mean the contract is incompatible, its guarantees
    alone that they contradict each other, both that the guarantees cannot
    hold when the assumptions do. ``unsat_core(formulas, variables)`` takes
    a list of formulas, or one formula split at its conjunctions.
    """
    if isinstance(formulas, Contract):
        c = formulas
        terms = list(c.assumptions) + list(c.guarantees)
        variables = c.variables
    else:
        if variables is None:
            raise TypeError("unsat_core of formulas needs the variables and their types")
        c = None
        terms = _terms(formulas)

    def fails(idx):
        r = backends.check(Satisfiable(ltl.conjunction([terms[i] for i in idx]),
                                       dict(variables)), backend, **options)
        return {UNSAT: True, SAT: False}.get(r.status)

    search = _Search(fails)
    every = list(range(len(terms)))
    if not search.fails(every):
        return Core(UNKNOWN if search.reason else NONE, reason=search.reason,
                    checks=search.checks)
    core = search.ddmin(every, [])
    if search.reason:
        return Core(UNKNOWN, reason=search.reason, checks=search.checks)
    if c is None:
        return Core(FOUND, [terms[i] for i in core], checks=search.checks)
    na = len(c.assumptions)
    return Core(FOUND, Contract(f"{c.name}_core", dict(c.inputs), dict(c.outputs),
                                [terms[i] for i in core if i < na],
                                [terms[i] for i in core if i >= na]),
                checks=search.checks)


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
