"""A formula on a trace, with the parameters left as names: the condition on
the parameters under which the trace satisfies the formula.

A counterexample of a check is a trace; with other values of the
parameters it may no longer be one. ``on_trace(phi -> psi, trace, params)``
is the condition that it is not: a formula over the parameters only, with
the values of the trace for every variable. Any repair satisfies it, so
adding it to the problem keeps every repair (chase_cps.repair).

The trace is a lasso: the prefix, then the loop forever. Each step of the
lasso is a position; G, F, U, R range over the positions reached from a
position (the rest of the prefix, then the loop once), which is enough on a
lasso. Before that, the prefix is extended by the loop as many times as the
deepest past(x, n) needs, so that every position of the loop has the same
history at each turn.
"""

from __future__ import annotations

import math
from fractions import Fraction

from .. import ltl
from ..queries import Trace

TRUE, FALSE = ltl.Const(True), ltl.Const(False)


def on_trace(node: ltl.Node, trace: Trace, params, at: int = 0) -> ltl.Node:
    """The formula at step ``at`` of the trace, as a formula over the
    parameters (names in ``params``)."""
    return _Lasso(trace, frozenset(params), node).formula(node, at)


def _depth(node) -> int:
    return max([n for _, n in ltl.pasts(node)] or [0])


class _Lasso:
    def __init__(self, trace: Trace, params, node):
        prefix, loop = list(trace.prefix), list(trace.loop)
        if not loop:                   # an empty loop repeats the last state
            if not prefix:
                raise ValueError("the trace has no state")
            loop = [prefix.pop()]
        depth = _depth(node)
        prefix += loop * math.ceil(depth / len(loop)) if depth else []
        self.states = prefix + loop
        self.start = len(prefix)
        self.params = params
        self.memo: dict = {}

    def next(self, i: int) -> int:
        return i + 1 if i + 1 < len(self.states) else self.start

    def reached(self, i: int) -> list:
        """The positions from i: the rest of the prefix, then the loop once."""
        out, seen = [], set()
        while i not in seen:
            seen.add(i)
            out.append(i)
            i = self.next(i)
        return out

    # formulas ----------------------------------------------------------------

    def formula(self, node, i: int) -> ltl.Node:
        key = (id(node), i)
        if key not in self.memo:
            self.memo[key] = self._formula(node, i)
        return self.memo[key]

    def _formula(self, node, i: int) -> ltl.Node:
        if isinstance(node, ltl.Const):
            return node
        if isinstance(node, ltl.Var):
            if node.name in self.params:
                return node
            v = self._read(node.name, self.next(i) if node.primed else i)
            if not isinstance(v, bool):
                raise ValueError(f"{node.name} is not a Boolean")
            return TRUE if v else FALSE
        if isinstance(node, ltl.Unary):
            op = node.op
            if op == "!":
                return _not(self.formula(node.arg, i))
            if op == "X":
                return self.formula(node.arg, self.next(i))
            if op == "G":
                return _all(self.formula(node.arg, j) for j in self.reached(i))
            if op == "F":
                return _any(self.formula(node.arg, j) for j in self.reached(i))
            raise ValueError(f"{ltl.to_str(node)}: not a formula")
        op = node.op
        if op in ltl.RELATIONS:
            return _relation(op, self.term(node.left, i), self.term(node.right, i))
        if op in ("U", "R"):
            seq = self.reached(i)
            if op == "U":         # some j: right at j, left before
                out, before = FALSE, TRUE
                for j in seq:
                    out = _or(out, _and(before, self.formula(node.right, j)))
                    before = _and(before, self.formula(node.left, j))
                return out
            out, before = TRUE, FALSE  # R: right until (and including) left
            for j in seq:
                out = _and(out, _or(before, self.formula(node.right, j)))
                before = _or(before, self.formula(node.left, j))
            return out
        a, b = self.formula(node.left, i), self.formula(node.right, i)
        if op == "&":
            return _and(a, b)
        if op == "|":
            return _or(a, b)
        if op == "->":
            return _or(_not(a), b)
        if op == "<->":
            return _or(_and(a, b), _and(_not(a), _not(b)))
        if op == "xor":
            return _or(_and(a, _not(b)), _and(_not(a), b))
        raise ValueError(f"{ltl.to_str(node)}: not a formula")

    # terms -------------------------------------------------------------------

    def term(self, node, i: int) -> ltl.Node:
        if isinstance(node, ltl.Num):
            return node
        if isinstance(node, ltl.Const):
            return node
        if isinstance(node, ltl.Var):
            if node.name in self.params:
                return node
            v = self._read(node.name, self.next(i) if node.primed else i)
            return ltl.Const(v) if isinstance(v, bool) else ltl.Num(ltl.number(v))
        if isinstance(node, ltl.Unary):
            if ltl.function(node) == "past":
                n = ltl.extension(node)
                if i - n < 0:
                    hist = f"chase_past{n}_{node.arg.name}"
                    state = self.states[i]
                    if hist in state:
                        return ltl.Num(ltl.number(state[hist]))
                    raise ValueError(f"past({node.arg.name}, {n}) has no value at step {i}")
                return self.term(node.arg, i - n)
            if node.op == "neg":
                a = self.term(node.arg, i)
                return ltl.Num(-a.value) if isinstance(a, ltl.Num) else ltl.Unary("neg", a)
            raise ValueError(f"{ltl.to_str(node)}: not supported in a repair")
        a, b = self.term(node.left, i), self.term(node.right, i)
        if isinstance(a, ltl.Num) and isinstance(b, ltl.Num):
            x, y = Fraction(a.value), Fraction(b.value)
            if node.op == "+":
                return ltl.Num(ltl.number(x + y))
            if node.op == "-":
                return ltl.Num(ltl.number(x - y))
            if node.op == "*":
                return ltl.Num(ltl.number(x * y))
        return ltl.Binary(node.op, a, b)

    def _read(self, name: str, i: int):
        state = self.states[i]
        if name not in state:
            raise ValueError(f"the trace has no value of {name} at step {i}")
        return state[name]


# simplification ----------------------------------------------------------------

def _not(a):
    if a == TRUE:
        return FALSE
    if a == FALSE:
        return TRUE
    return ltl.Unary("!", a)


def _and(a, b):
    if a == FALSE or b == FALSE:
        return FALSE
    if a == TRUE:
        return b
    if b == TRUE:
        return a
    return ltl.Binary("&", a, b)


def _or(a, b):
    if a == TRUE or b == TRUE:
        return TRUE
    if a == FALSE:
        return b
    if b == FALSE:
        return a
    return ltl.Binary("|", a, b)


def _all(items):
    out = TRUE
    for x in items:
        out = _and(out, x)
        if out == FALSE:
            break
    return out


def _any(items):
    out = FALSE
    for x in items:
        out = _or(out, x)
        if out == TRUE:
            break
    return out


def _relation(op, a, b):
    if isinstance(a, ltl.Num) and isinstance(b, ltl.Num):
        x, y = Fraction(a.value), Fraction(b.value)
        ok = {"=": x == y, "!=": x != y, "<": x < y, "<=": x <= y, ">": x > y,
              ">=": x >= y}[op]
        return TRUE if ok else FALSE
    if isinstance(a, ltl.Const) and isinstance(b, ltl.Const):
        same = a.value == b.value
        return TRUE if (same if op == "=" else not same) else FALSE
    return ltl.Binary(op, a, b)
