"""Robustness on lasso traces (chase_cps.robustness): the values of the
operators, against a second evaluation written from the definitions, and
the terms that the counterexample of a failed refinement violates."""

import math
import random
from fractions import Fraction

import pytest

from chase_cps import Contract, ltl, refines
from chase_cps.backends import ADAPTERS
from chase_cps.queries import Trace
from chase_cps.robustness import holds, numeric, robustness, violated

INF = math.inf
needs_nuxmv = pytest.mark.skipif(not ADAPTERS["nuxmv"].available(), reason="nuXmv not installed")

# x: 1, then 5, 3, 5, 3, ...; b: true, then false, true, ...
T = Trace(prefix=[{"x": 1, "b": True}], loop=[{"x": 5, "b": False}, {"x": 3, "b": True}])


@pytest.mark.parametrize("formula, value", [
    ("x <= 4", 3), ("x >= 4", -3), ("x < 4", 3), ("x = 1", 0), ("x = 3", -2), ("x != 3", 2),
    ("x' = 5", 0), ("x' - x >= 4", 0), ("2 * x + 1 <= 5", 2),
    ("G (x <= 6)", 1), ("F (x >= 5)", 0), ("G (x >= 1)", 0), ("F G (x >= 3)", 0),
    ("X X (x = 3)", 0), ("X X X (x = 5)", 0),
    ("x < 2 U x >= 5", 0), ("b R (x <= 5)", 4), ("!b R (x <= 4)", -1),
    ("x <= 4 & x >= 0", 1), ("x <= 0 | x >= 0", 1), ("x >= 3 -> x <= 0", 2),
    ("G F b", INF), ("F G b", -INF), ("b & x <= 4", 3), ("!b | x >= 4", -3),
])
def test_values(formula, value):
    assert robustness(formula, T) == value


def test_reals_are_exact_and_steps_can_be_chosen():
    t = Trace(prefix=[], loop=[{"v": Fraction(1, 3)}, {"v": Fraction(5, 2)}])
    assert robustness("v <= 3", t) == Fraction(8, 3)
    assert robustness("v <= 3", t, at=1) == Fraction(1, 2)
    assert robustness("G (v >= 0.25)", t) == Fraction(1, 12)


def test_an_empty_loop_repeats_the_last_state():
    t = Trace(prefix=[{"x": 0}, {"x": 7}], loop=[])
    assert robustness("G F (x = 7)", t) == 0 and robustness("F G (x >= 5)", t) == 2


def test_sign_and_satisfaction():
    # on the boundary (0) the robustness says nothing; holds says
    assert robustness("x < 1", T) == 0 and not holds("x < 1", T)
    assert robustness("x <= 1", T) == 0 and holds("x <= 1", T)


def test_words_wrap_around():
    t = Trace(prefix=[], loop=[{"w": 250}])
    assert robustness("w + 10 <= 10", t, {"w": "word[8]"}) == 6     # 260 is 4 in 8 bits


def test_errors():
    with pytest.raises(ValueError, match="no value of y"):
        robustness("y <= 1", T)
    with pytest.raises(ValueError, match="not a Boolean"):
        robustness("G x", T)
    with pytest.raises(ValueError, match="no state"):
        robustness("true", Trace(prefix=[], loop=[]))


def test_numeric_and_violated():
    v = {"x": (0, 9), "b": "bool", "c": "bool"}
    assert numeric("G (x <= 4)", v) and not numeric("G (b -> F c)", v) and not numeric("b = c", v)
    assert numeric("b = c") and not numeric("G b")
    assert violated(["G (x <= 4)", "G F b", "F G b", "x >= 0"], T, v) == [
        {"term": "G (x <= 4)", "robustness": -1}, {"term": "F G b", "robustness": None}]


# A second evaluation, from the definitions: the steps of the lasso unrolled
# far enough that every state is seen again, and U by its definition (a
# maximum over j of a minimum over the steps up to j).

def _reference(node, trace, i, depth=0):
    states = trace.prefix + trace.loop
    p, n = len(trace.prefix), len(trace.loop)
    at = lambda k: k if k < p else p + (k - p) % n
    horizon = range(i, i + 2 * (p + n) + 1)

    def term(e, k):
        if isinstance(e, ltl.Num):
            return e.value
        if isinstance(e, ltl.Var):
            return states[at(k + 1 if e.primed else k)][e.name]
        a, b = term(e.left, k), term(e.right, k)
        return {"+": a + b, "-": a - b, "*": a * b}[e.op]

    def r(e, k):
        if isinstance(e, ltl.Const):
            return INF if e.value else -INF
        if isinstance(e, ltl.Var):
            return INF if states[at(k)][e.name] else -INF
        if isinstance(e, ltl.Unary):
            if e.op == "!":
                return -r(e.arg, k)
            if e.op == "X":
                return r(e.arg, k + 1)
            values = [r(e.arg, j) for j in range(k, k + 2 * (p + n) + 1)]
            return min(values) if e.op == "G" else max(values)
        if e.op in ltl.RELATIONS:
            a, b = term(e.left, k), term(e.right, k)
            return {"<": b - a, "<=": b - a, ">": a - b, ">=": a - b,
                    "=": -abs(a - b), "!=": abs(a - b)}[e.op]
        a, b = r(e.left, k), r(e.right, k)
        if e.op == "&":
            return min(a, b)
        if e.op == "|":
            return max(a, b)
        if e.op == "->":
            return max(-a, b)
        if e.op == "<->":
            return min(max(-a, b), max(a, -b))
        js = range(k, k + 2 * (p + n) + 1)
        if e.op == "U":
            return max(min([r(e.right, j)] + [r(e.left, m) for m in range(k, j)]) for j in js)
        if e.op == "R":
            return min(max([r(e.right, j)] + [r(e.left, m) for m in range(k, j)]) for j in js)
        raise AssertionError(e.op)
    return r(node, i)


def _formula(rng, depth):
    if depth == 0 or rng.random() < 0.25:
        return rng.choice(["x <= 3", "x >= 2", "y > x", "x' = y", "b", "!b", "x + y <= 6", "x != 4"])
    op = rng.choice(["!", "X", "G", "F", "&", "|", "->", "<->", "U", "R"])
    if op in ("!", "X", "G", "F"):
        return f"{op} ({_formula(rng, depth - 1)})"
    return f"({_formula(rng, depth - 1)}) {op} ({_formula(rng, depth - 1)})"


def _trace(rng):
    state = lambda: {"x": rng.randint(0, 5), "y": rng.randint(0, 5), "b": rng.random() < 0.5}
    return Trace(prefix=[state() for _ in range(rng.randint(0, 3))],
                 loop=[state() for _ in range(rng.randint(1, 3))])


def test_against_the_definitions():
    rng = random.Random(7)
    for _ in range(400):
        f, t = ltl.parse(_formula(rng, 3)), _trace(rng)
        at = rng.randrange(len(t.prefix) + len(t.loop))
        value = robustness(f, t, at=at)
        assert value == _reference(f, t, at), ltl.to_str(f)
        if value > 0:
            assert holds(f, t, at=at)
        if value < 0:
            assert not holds(f, t, at=at)


def test_duals():
    rng = random.Random(11)
    for _ in range(200):
        a, b, t = _formula(rng, 2), _formula(rng, 2), _trace(rng)
        assert robustness(f"G ({a})", t) == -robustness(f"F !({a})", t)
        assert robustness(f"({a}) R ({b})", t) == -robustness(f"!({a}) U !({b})", t)
        assert robustness(f"F ({b})", t) == robustness(f"true U ({b})", t)


@needs_nuxmv
def test_a_failed_refinement_says_how_much_a_term_is_violated():
    gen = Contract.from_strings(name="Gen", inputs={"n": (0, 20)}, outputs={"p": "real"},
                                assumptions=["n <= 4"], guarantees=["p >= 2.5 * n", "p <= 30"])
    spec = Contract.from_strings(name="Spec", inputs={"n": (0, 20)}, outputs={"p": "real"},
                                 assumptions=["n <= 4"], guarantees=["G (p >= 12)", "p <= 30"])
    r = refines(gen, spec)
    assert r.failed == "guarantees"
    [v] = r.to_dict()["violated"]
    assert v["term"] == "G (p >= 12)" and v["robustness"] < 0
    # the trace is the counterexample: the term is violated on it by that much
    assert robustness("G (p >= 12)", r.guarantees.trace) == v["robustness"]
    assert gen.robustness(r.guarantees.trace)["contract"] >= 0


@needs_nuxmv
def test_a_failed_refinement_of_the_assumptions_and_of_booleans():
    part = Contract.from_strings(name="Part", inputs={"t": "real"}, outputs={"ok": "bool"},
                                 assumptions=["G (t <= 40)"], guarantees=["G ok"])
    spec = Contract.from_strings(name="Spec", inputs={"t": "real"}, outputs={"ok": "bool"},
                                 assumptions=["G (t <= 50)"], guarantees=["G ok"])
    r = refines(part, spec)
    assert r.failed == "assumptions"
    [v] = r.violated
    assert v["term"] == "G (t <= 40)" and -10 <= v["robustness"] < 0
    lazy = Contract.from_strings(name="Lazy", inputs={"t": "real"}, outputs={"ok": "bool"},
                                 assumptions=["G (t <= 50)"], guarantees=["F ok"])
    r = refines(lazy, spec)
    assert r.failed == "guarantees" and r.to_dict()["violated"] == [{"term": "G ok", "robustness": None}]


def test_past_from_the_states_or_from_the_history_of_nuxmv():
    t = Trace(prefix=[{"x": 0}, {"x": 4}], loop=[{"x": 1}, {"x": 7}])
    # x two steps before: at step 2 it is 0, at 3 it is 4, then 1, 7, ...
    assert robustness("X X (x - past(x, 2) <= 1)", t) == 0
    assert robustness("X X G (x - past(x, 2) <= 6)", t) == 3        # 7 - 4 at step 3
    # the step at which the loop starts has another history at each later turn
    t3 = Trace(prefix=[{"x": 0}], loop=[{"x": 1}, {"x": 5}, {"x": 2}])
    assert robustness("X F (x - past(x, 1) = -1)", t3) == 0          # 1 - 2, not 1 - 0
    with pytest.raises(ValueError, match="no value at step 0"):
        robustness("x = past(x, 1)", t)
    t = Trace(prefix=[{"x": 0, "chase_past1_x": 9}], loop=[{"x": 3, "chase_past1_x": 0}])
    assert robustness("x <= past(x, 1)", t) == 9


def test_intervals_of_signal_temporal_logic():
    # x: 1, then 5, 3, 5, 3, ...
    assert robustness("G[0,2] (x <= 5)", T) == 0
    assert robustness("F[0,1] (x >= 4)", T) == 1
    assert robustness("G[1,2] (x >= 2)", T) == 1
    assert robustness("(x <= 4) U[0,2] (x >= 5)", T) == 0
    assert robustness("G (x >= 3 -> F[0,1] (x = 5))", T) == 0
