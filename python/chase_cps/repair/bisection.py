"""Repair by bisection, for checks that are monotone in each parameter.

A check is monotone in a parameter p when moving p one way can only help
it: in ``d <= dmax`` a larger dmax makes the term weaker, so a guarantee
that is easier to meet, and an assumption that admits more behaviors. The
direction is read from the formulas (``direction``): p appears in linear
comparisons only, with one sign in each, and the sign is turned by the
negations and the left side of the implications around the comparison,
and by the side of the question (the premise of an implication that must
hold counts against). A check monotone in every parameter passes on a set
of values that is closed in the helpful direction: if it does not pass at
the corner of the ranges that helps most, no value passes, and once it
passes at a value it passes at every value beyond it.

So the least change is on the boundary of that set, and bisection finds it:

- one parameter: the boundary between the present value and the helpful
  end of its range, to the tolerance (an integer exactly). The result is
  optimal within the tolerance.
- several: the boundary along rays from the present values toward the
  helpful corner, in the directions of a grid on the simplex (each
  parameter alone, and mixes). The best point of the rays is a repair, but
  the least cost may lie between two rays: it is not shown optimal.

Every point is a run of the check itself, so it works on any check that
the back ends answer, models and C functions included; it needs only
monotonicity, and costs one check per halving.
"""

from __future__ import annotations

import itertools
import math
from fractions import Fraction

from .. import ltl
from ..queries import Holds, Implies, Satisfiable

RESOLUTION = 4          # the grid of directions: weights in steps of 1/RESOLUTION
STEPS = 1000            # the default tolerance: the range over STEPS


def _combine(a, b):
    if a is None or b is None:
        return None
    if a == 0:
        return b
    if b == 0 or a == b:
        return a
    return None


def _coefficient(node, p):
    """The coefficient of p in a linear term, 0 when p is absent, None when
    the term is not linear in p."""
    if isinstance(node, ltl.Num):
        return Fraction(0)
    if isinstance(node, ltl.Var):
        return Fraction(1) if node.name == p else Fraction(0)
    if isinstance(node, ltl.Unary):
        if node.op == "neg":
            c = _coefficient(node.arg, p)
            return None if c is None else -c
        return Fraction(0) if p not in ltl.variables(node) else None
    if isinstance(node, ltl.Binary):
        if node.op in ("+", "-"):
            a, b = _coefficient(node.left, p), _coefficient(node.right, p)
            if a is None or b is None:
                return None
            return a + b if node.op == "+" else a - b
        if node.op in ("*", "/"):
            left_has = p in ltl.variables(node.left)
            right_has = p in ltl.variables(node.right)
            if not left_has and not right_has:
                return Fraction(0)
            if node.op == "*" and right_has and not left_has:
                k, other = _constant(node.left), node.right
            elif not right_has:
                k, other = _constant(node.right), node.left
                if node.op == "/" and k is not None:
                    k = None if k == 0 else 1 / k
            else:
                return None
            c = _coefficient(other, p)
            return None if k is None or c is None else k * c
        return Fraction(0) if p not in ltl.variables(node) else None
    return Fraction(0)


def _constant(node):
    if isinstance(node, ltl.Num):
        return Fraction(node.value)
    if isinstance(node, ltl.Unary) and node.op == "neg":
        c = _constant(node.arg)
        return None if c is None else -c
    if isinstance(node, ltl.Binary) and node.op in ("+", "-", "*", "/"):
        a, b = _constant(node.left), _constant(node.right)
        if a is None or b is None or (node.op == "/" and b == 0):
            return None
        return {"+": a + b, "-": a - b, "*": a * b, "/": a / b if b else None}[node.op]
    return None


def _sign(x):
    return 0 if x == 0 else (1 if x > 0 else -1)


def polarity(node, p):
    """+1 when the formula can only become true as p grows, -1 when it can
    only become false, 0 when p does not matter, None when neither."""
    if isinstance(node, (ltl.Const, ltl.Var, ltl.Num)):
        return 0
    if isinstance(node, ltl.Unary):
        if node.op == "!":
            a = polarity(node.arg, p)
            return None if a is None else -a
        if node.op in ltl.TEMPORAL_UNARY:
            return polarity(node.arg, p)
        return 0 if p not in ltl.variables(node) else None
    if isinstance(node, ltl.Binary):
        if node.op in ("&", "|") + ltl.TEMPORAL_BINARY:
            return _combine(polarity(node.left, p), polarity(node.right, p))
        if node.op == "->":
            a = polarity(node.left, p)
            return _combine(None if a is None else -a, polarity(node.right, p))
        if node.op in ("<->", "xor"):
            a, b = polarity(node.left, p), polarity(node.right, p)
            return 0 if a == 0 and b == 0 else None
        if node.op in ("<", "<=", ">", ">=", "=", "!="):
            a, b = _coefficient(node.left, p), _coefficient(node.right, p)
            if a is None or b is None:
                return None
            grows = b - a if node.op in ("<", "<=") else a - b      # truth grows with this
            if node.op in ("=", "!="):
                return 0 if a == b else None
            return _sign(grows)
        return 0 if p not in ltl.variables(node) else None
    return 0


def direction(asked: list, p: str):
    """+1 when a larger p can only help the check pass, -1 when a smaller,
    0 when p does not matter, None when the check is not monotone in p."""
    d = 0
    for _, q in asked:
        if isinstance(q, Implies):
            a = polarity(q.phi, p)
            here = _combine(None if a is None else -a, polarity(q.psi, p))
        elif isinstance(q, (Satisfiable, Holds)):
            here = polarity(q.phi, p)
        else:
            return None
        d = _combine(d, here)
        if d is None:
            return None
    return d


def _directions(n: int) -> list:
    """Weights on the simplex, in steps of 1/RESOLUTION."""
    out = []
    for w in itertools.product(range(RESOLUTION + 1), repeat=n):
        if sum(w) == RESOLUTION:
            out.append([Fraction(x, RESOLUTION) for x in w])
    return out


def by_bisection(check, args, params, cost, aux, backend, opts, deadline=None,
                 tolerance=None):
    from . import (HOLDS, IMPOSSIBLE, REPAIRED, UNKNOWN, Repair, _cost_of, _left, _stopped)
    from .problem import instantiated, questions, run, symbolic_args
    asked = questions(check, symbolic_args(check, args, params), params)
    signs = {}
    for name in params:
        d = direction(asked, name)
        if d is None:
            return Repair(UNKNOWN, method="bisection",
                          reason=f"the check is not monotone in {name} (or not linear in it)")
        signs[name] = d
    ps = list(params.values())
    start = {p.name: min(max(Fraction(p.value), Fraction(p.low)), Fraction(p.high)) for p in ps}
    best_end = {p.name: (Fraction(p.high) if signs[p.name] >= 0 else Fraction(p.low)) for p in ps}
    checks = [0]

    def snap(values):
        out = {}
        for p in ps:
            v = values[p.name]
            if p.integer:          # toward the helpful side: it can only help
                v = math.ceil(v) if signs[p.name] >= 0 else math.floor(v)
            out[p.name] = ltl.number(v)
        return out

    def passes(values):
        left = _left(deadline, opts.get("timeout", 300))
        if left is None:
            return None
        checks[0] += 1
        out = run(check, instantiated(args, values), backend, {**opts, "timeout": left})
        return out

    first = passes(snap(start))
    if first is None:
        return _stopped("bisection", checks[0], None)
    if first.passed:
        return Repair(HOLDS, snap(start), 0, True, "bisection", first.result, checks[0])
    corner = passes(snap(best_end))
    if corner is None:
        return _stopped("bisection", checks[0], None)
    if corner.passed is None:
        return Repair(UNKNOWN, method="bisection", rounds=checks[0], check=corner.result,
                      reason=corner.reason or "the check gave no answer")
    if not corner.passed:
        return Repair(IMPOSSIBLE, method="bisection", rounds=checks[0], check=corner.result,
                      reason="the check is monotone in the parameters and fails at the corner "
                             "of the ranges that helps it most: no values pass")
    tol = {p.name: Fraction(str(tolerance)) if tolerance is not None
           else (Fraction(p.high) - Fraction(p.low)) / STEPS for p in ps}
    moving = [p for p in ps if signs[p.name] != 0 and start[p.name] != best_end[p.name]]
    rays = _directions(len(moving)) if len(moving) > 1 else [[Fraction(1)]]
    best, best_cost, best_result = None, None, None
    for w in rays:
        top = max(w)
        point = lambda t: {**start, **{p.name: start[p.name] + t * (wi / top) *
                                       (best_end[p.name] - start[p.name])
                                       for p, wi in zip(moving, w)}}
        end = snap(point(Fraction(1)))
        lo, hi, hi_result = Fraction(0), Fraction(1), None
        if end != snap(best_end):
            r = passes(end)
            if r is None:
                break
            if not r.passed:
                continue
            hi_result = r.result
        else:
            hi_result = corner.result

        def close_enough():
            return all(abs((hi - lo) * (wi / top) * (best_end[p.name] - start[p.name])) <=
                       (Fraction(1, 2) if p.integer else tol[p.name])
                       for p, wi in zip(moving, w))
        while not close_enough():
            mid = (lo + hi) / 2
            if snap(point(mid)) == snap(point(hi)):
                hi = mid
                continue
            r = passes(snap(point(mid)))
            if r is None:
                break
            if r.passed:
                hi, hi_result = mid, r.result
            elif r.passed is False:
                lo = mid
            else:
                return Repair(UNKNOWN, method="bisection", rounds=checks[0], check=r.result,
                              reason=r.reason or "the check gave no answer")
        values = snap(point(hi))
        c = _cost_of(cost, aux, values, params)
        if best_cost is None or c < best_cost:
            best, best_cost, best_result = values, c, hi_result
    if best is None:
        return _stopped("bisection", checks[0], None)
    if len(moving) > 1:
        reason = f"the best of {len(rays)} directions: the least cost may lie between two"
    elif moving and not moving[0].integer:
        reason = f"the least change within {ltl.to_str(ltl.Num(ltl.number(tol[moving[0].name])))}"
    else:
        reason = ""
    exact = len(moving) <= 1 and all(p.integer for p in moving)
    return Repair(REPAIRED, best, best_cost, exact, "bisection", best_result, checks[0],
                  reason=reason)
