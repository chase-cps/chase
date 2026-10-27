"""Checks on formulas of a theory: validity, satisfiability, and cores.

These are the logic queries that the contract checks reduce to. They are
about formulas, not contracts: ``is_satisfiable(phi)`` asks whether some
behavior (an assignment of the variables at each step) satisfies phi, which
is not the satisfaction of a contract by a component (``implements`` in
``chase_cps.checks``). ``implies(phi, psi)`` checks validity: phi -> psi
holds for every behavior.

The theory is LTL over Boolean, bounded integer, and real variables. The
back end is nuXmv, or NuSMV without real variables; the real variables go
through ``chase_cps.reals``.

``unsat_core`` searches with delta debugging (DDMin, Zeller and
Hildebrandt, TSE 2002), one call to the back end per test, and remembers the
sets already tested: a superset of a failing set fails, a subset of a
passing set passes. The cores are minimal: removing any term of the core
makes the check pass. When a test does not finish, the core is not
reported (status unknown).

The queries take a ``context``: a formula, or a list of formulas, that
every behavior satisfies (e.g. an axiom of an encoding of finite traces, or
the physical range of a signal). The behaviors that violate it are left
out: an implication phi -> psi is checked as context & phi -> psi, the
satisfiability of phi as that of context & phi. A context is not an
assumption: a behavior that violates an assumption satisfies the saturated
contract, one that violates the context does not count.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Callable, Optional

from . import backends, ltl
from .contract import Contract, VarType
from .queries import SAT, UNSAT, Implies, Result, Satisfiable

FOUND, NONE, UNKNOWN = "found", "none", "unknown"


def _formula(f) -> ltl.Node:
    return ltl.parse(f) if isinstance(f, str) else f


def _context(context, variables) -> Optional[ltl.Node]:
    """The context as one formula (None when there is none); its variables
    must be among the variables of the check."""
    if context is None:
        return None
    terms = [_formula(context)] if isinstance(context, (str, ltl.Node)) \
        else [_formula(f) for f in context]
    if not terms:
        return None
    k = ltl.conjunction(terms)
    missing = ltl.variables(k) - set(variables)
    if missing:
        raise ValueError(f"undeclared variables {sorted(missing)} in the context "
                         f"{ltl.to_str(k)!r}")
    return k


def _within(context: Optional[ltl.Node], phi: ltl.Node) -> ltl.Node:
    return phi if context is None else ltl.Binary("&", context, phi)


def implies(phi, psi, variables: dict[str, VarType], backend: str = "auto",
            context=None, **options) -> Result:
    """Is phi -> psi valid? ``valid``, or ``invalid`` with a counterexample trace
    (a behavior satisfying phi and violating psi)."""
    k = _context(context, variables)
    return backends.check(Implies(_within(k, _formula(phi)), _formula(psi), dict(variables)),
                          backend, **options)


def is_satisfiable(phi, variables: dict[str, VarType], backend: str = "auto",
                   core: bool = False, context=None, **options) -> Result:
    """Is phi satisfiable? ``sat`` with a witness trace, or ``unsat``. With
    core, an unsat result carries ``unsat_core`` of the conjuncts of phi."""
    k = _context(context, variables)
    r = backends.check(Satisfiable(_within(k, _formula(phi)), dict(variables)), backend,
                       **options)
    if core and r.status == UNSAT:
        r.core = unsat_core(_formula(phi), variables, backend, context=k, **options)
    return r


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
               backend: str = "auto", context=None, **options) -> Core:
    """A minimal set of terms that cannot hold together.

    ``unsat_core(contract)`` searches the assumptions and the guarantees of
    the contract (A & G) and returns a contract with the terms of the core:
    its assumptions alone mean the contract is incompatible, its guarantees
    alone that they contradict each other, both that the guarantees cannot
    hold when the assumptions do. ``unsat_core(formulas, variables)`` takes
    a list of formulas, or one formula split at its conjunctions. The
    context (see ``chase_cps.checks``) holds in every check and is not part
    of the core: an empty core means that the context alone cannot hold.
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

    k = _context(context, variables)

    def fails(idx):
        selected = [terms[i] for i in idx] + ([k] if k is not None else [])
        r = backends.check(Satisfiable(ltl.conjunction(selected), dict(variables)), backend,
                           **options)
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
