"""Robustness: how much a trace satisfies a formula, or violates it.

The quantitative semantics of signal temporal logic, in discrete time, on
the lasso traces of the checks (a prefix, then a loop repeated forever).
A comparison gives the distance to its boundary: ``x <= 30`` is
``30 - x``, ``x >= 10`` is ``x - 10``, ``x = c`` is ``-|x - c|``, and
``x != c`` is ``|x - c|``. Then ``!`` is the opposite, ``&`` the minimum,
``|`` the maximum, ``X`` the value at the next step, ``G`` the minimum over
the steps to come, ``F`` the maximum, and ``U`` the maximum over the steps
j of the minimum of the right side at j and of the left side before j.
The intervals (``G[0,3] p``) are read as their steps.

A positive robustness means that the trace satisfies the formula, a
negative one that it violates it; 0 is on the boundary, and says neither.
A Boolean variable is ``inf`` when true and ``-inf`` when false, so the
robustness of a formula without comparisons of numbers is ``inf`` or
``-inf``: it says no more than whether it holds. The robustness of a
comparison of numbers is a number, in the unit of its terms.

A trace whose loop is empty repeats its last state. ``past(x, n)`` is the
value of x n steps before: the history variable of nuXmv in the trace
(``chase_past2_x``) when it is there, else the state n steps before; in
the first n steps it has none.
"""

from __future__ import annotations

import math
from fractions import Fraction
from typing import Optional, Union

from . import ltl
from .queries import Trace

INF = math.inf
Number = Union[int, Fraction, float]


def robustness(formula, trace: Trace, variables: Optional[dict] = None, at: int = 0) -> Number:
    """The robustness of a formula (text or tree) on a trace, at a step of
    the trace (0: the first state)."""
    node = _node(formula)
    return _Lasso(trace, variables, _history(node)).value(node, at)


def holds(formula, trace: Trace, variables: Optional[dict] = None, at: int = 0) -> bool:
    """Whether a trace satisfies a formula (also on the boundary, where the
    robustness is 0)."""
    node = _node(formula)
    return _Lasso(trace, variables, _history(node)).holds(node, at)


def _history(node) -> int:
    """The most steps a past(x, n) of the formula looks back."""
    return max((n for _, n in ltl.pasts(node)), default=0)


def numeric(formula, variables: Optional[dict] = None) -> bool:
    """Whether a formula compares numbers, so that its robustness can be a
    number, not only inf or -inf. Without the types of the variables, every
    comparison with a variable counts."""
    node = _node(formula)
    if isinstance(node, ltl.Binary):
        if node.op in ltl.RELATIONS:
            names = ltl.variables(node)
            if variables is None:
                return bool(names) or _has_number(node)
            return _has_number(node) or any(variables.get(v, "bool") != "bool" for v in names)
        return numeric(node.left, variables) or numeric(node.right, variables)
    if isinstance(node, ltl.Unary):
        return numeric(node.arg, variables)
    return False


def _has_number(node) -> bool:
    if isinstance(node, ltl.Num):
        return True
    if isinstance(node, ltl.Unary):
        return _has_number(node.arg)
    if isinstance(node, ltl.Binary):
        return _has_number(node.left) or _has_number(node.right)
    return False


def violated(terms, trace: Trace, variables: Optional[dict] = None) -> list[dict]:
    """The terms that a trace violates, each with its robustness when the
    term compares numbers (how much it is violated: a negative number or 0),
    else None."""
    nodes = [_node(t) for t in terms]
    lasso, out = _Lasso(trace, variables, max(map(_history, nodes), default=0)), []
    for node in nodes:
        if not lasso.holds(node, 0):
            r = lasso.value(node, 0) if numeric(node, variables) else None
            out.append({"term": ltl.to_str(node), "robustness": r})
    return out


def _node(formula):
    return ltl.parse(formula) if isinstance(formula, str) else formula


class _Lasso:
    """The states of a lasso trace, with the step after each one."""

    def __init__(self, trace: Trace, variables: Optional[dict], history: int = 0):
        prefix, loop = list(trace.prefix), list(trace.loop)
        if history and loop:
            # the loop once more in the prefix for every len(loop) steps of
            # history: in the loop, the state n steps before is then the same
            # at every turn
            prefix += loop * -(-history // len(loop))
        self.states = prefix + loop
        if not self.states:
            raise ValueError("the trace has no state")
        self.start = len(prefix) if loop else len(self.states) - 1
        self.variables = variables or {}
        self.memo: dict = {}

    def next(self, i: int) -> int:
        return i + 1 if i + 1 < len(self.states) else self.start

    def path(self, i: int) -> list[int]:
        """The steps from i on, each state of the trace once."""
        out, seen = [], set()
        while i not in seen:
            seen.add(i)
            out.append(i)
            i = self.next(i)
        return out

    def check(self, at: int) -> int:
        if not 0 <= at < len(self.states):
            raise ValueError(f"step {at}: the trace has {len(self.states)} states")
        return at

    # Two semantics with the same structure: robustness (min, max, and the
    # distance of a comparison) and satisfaction (all, any, the comparison).

    def value(self, node, at: int) -> Number:
        return self._eval(node, self.check(at), True)

    def holds(self, node, at: int) -> bool:
        return bool(self._eval(node, self.check(at), False))

    def _eval(self, node, i: int, quantitative: bool):
        key = (id(node), i, quantitative)
        if key not in self.memo:
            self.memo[key] = (node, self._compute(node, i, quantitative))
        return self.memo[key][1]

    def _compute(self, node, i: int, q: bool):
        top, bottom = (INF, -INF) if q else (True, False)
        lo, hi = (min, max) if q else ((lambda *a: all(a)), (lambda *a: any(a)))
        neg = (lambda v: -v) if q else (lambda v: not v)
        ev = lambda n, j: self._eval(n, j, q)
        if isinstance(node, ltl.Const):
            return top if node.value else bottom
        if isinstance(node, ltl.Var):
            v = self._read(node, i)
            if not isinstance(v, bool):
                raise ValueError(f"{node.name} is not a Boolean: compare it with a number")
            return top if v else bottom
        if isinstance(node, ltl.Unary):
            if node.op == "!":
                return neg(ev(node.arg, i))
            if node.op == "X":
                return ev(node.arg, self.next(i))
            if node.op in ("G", "F"):
                values = [ev(node.arg, j) for j in self.path(i)]
                if node.op == "G":
                    return min(values) if q else all(values)
                return max(values) if q else any(values)
            raise ValueError(f"{ltl.to_str(node)} is not a formula")
        op = node.op
        if op in ltl.RELATIONS:
            return self._relation(node, i, q)
        if op == "&":
            return lo(ev(node.left, i), ev(node.right, i))
        if op == "|":
            return hi(ev(node.left, i), ev(node.right, i))
        if op == "->":
            return hi(neg(ev(node.left, i)), ev(node.right, i))
        if op in ("<->", "xor"):
            a, b = ev(node.left, i), ev(node.right, i)
            same = lo(hi(neg(a), b), hi(a, neg(b)))
            return same if op == "<->" else neg(same)
        if op == "U":
            return self._until(node.left, node.right, i, q)
        if op == "R":                    # a R b = !(!a U !b)
            return neg(self._until(ltl.Unary("!", node.left), ltl.Unary("!", node.right), i, q))
        raise ValueError(f"{ltl.to_str(node)} is not a formula")

    def _until(self, a, b, i: int, q: bool):
        # the best step j at which b holds, with a at every step before j;
        # after each state was seen once, later steps add nothing
        best, before = (-INF, INF) if q else (False, True)
        for j in self.path(i):
            if q:
                best = max(best, min(self._eval(b, j, q), before))
                before = min(before, self._eval(a, j, q))
            else:
                best = best or (before and self._eval(b, j, q))
                before = before and self._eval(a, j, q)
        return best

    def _relation(self, node, i: int, q: bool):
        left, right = self._term(node.left, i), self._term(node.right, i)
        op = node.op
        if not (_is_number(left) and _is_number(right)):
            if op not in ("=", "!="):
                raise ValueError(f"{ltl.to_str(node)} compares values that are not numbers")
            same = left == right
            ok = same if op == "=" else not same
            return (INF if ok else -INF) if q else ok
        if not q:
            return {"=": left == right, "!=": left != right, "<": left < right,
                    "<=": left <= right, ">": left > right, ">=": left >= right}[op]
        if op in ("<", "<="):
            return right - left
        if op in (">", ">="):
            return left - right
        return -abs(left - right) if op == "=" else abs(left - right)

    def _term(self, node, i: int):
        if isinstance(node, ltl.Num):
            return node.value
        if isinstance(node, ltl.Const):
            return node.value
        if isinstance(node, ltl.Var):
            return self._read(node, i)
        if self._words(node):
            return self._word_term(node, i)
        if isinstance(node, ltl.Unary):
            if node.op == "neg":
                return -self._term(node.arg, i)
            if ltl.function(node) == "past":
                return self._past(node, i)
            raise ValueError(f"{ltl.to_str(node)} is not a term of numbers")
        a, b = self._term(node.left, i), self._term(node.right, i)
        if node.op == "+":
            return a + b
        if node.op == "-":
            return a - b
        if node.op == "*":
            return a * b
        if node.op == "/":
            if b == 0:
                raise ZeroDivisionError(ltl.to_str(node))
            if isinstance(a, int) and isinstance(b, int):      # as nuXmv: toward 0
                return int(Fraction(a, b))
            return Fraction(a) / Fraction(b)
        raise ValueError(f"{ltl.to_str(node)} is not a term of numbers")

    def _past(self, node, i: int):
        name, n = ltl.past_name(node), ltl.extension(node)
        if name in self.states[i]:                  # the history variable of nuXmv
            return self.states[i][name]
        if i < n:
            raise ValueError(f"{ltl.to_str(node)} has no value at step {i} of the trace: "
                             f"give {name} in its states")
        return self.states[i - n][node.arg.name]

    def _read(self, var, i: int):
        state = self.states[self.next(i) if var.primed else i]
        if var.name not in state:
            raise ValueError(f"the trace has no value of {var.name}")
        return state[var.name]

    def _words(self, node) -> bool:
        from .words import word_type
        return any(word_type(self.variables.get(v)) for v in ltl.variables(node))

    def _word_term(self, node, i: int):
        """A term of words, with their arithmetic (see chase_cps.words)."""
        from . import words
        state = dict(self.states[i])
        later = self.states[self.next(i)]
        variables = dict(self.variables)

        def unprime(n):
            if isinstance(n, ltl.Var) and n.primed:
                state[n.name + "'"] = later[n.name]
                variables[n.name + "'"] = self.variables.get(n.name)
                return ltl.Var(n.name + "'")
            if isinstance(n, ltl.Unary):
                return ltl.Unary(n.op, unprime(n.arg))
            if isinstance(n, ltl.Binary):
                return ltl.Binary(n.op, unprime(n.left), unprime(n.right))
            return n
        return words.evaluate(unprime(node), state, variables)


def json_number(v):
    """A robustness for JSON: a number, or None when it is not finite."""
    if v is None or (isinstance(v, float) and math.isinf(v)):
        return None
    from .queries import _json_value
    return _json_value(v)


def _is_number(v) -> bool:
    return isinstance(v, (int, Fraction, float)) and not isinstance(v, bool)
