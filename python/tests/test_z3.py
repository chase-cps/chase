"""The z3 back end: formulas without temporal operators (static contracts),
against nuXmv on random formulas, and its traces against the formulas."""

import random

import pytest

from chase_cps import Contract, implies, is_satisfiable, ltl, refines, verify
from chase_cps.backends import ADAPTERS
from chase_cps.backends.z3 import bounded, static, why_not
from chase_cps.queries import Implies, Satisfiable
from chase_cps.robustness import holds, robustness

HAS_Z3 = ADAPTERS["z3"].available()
HAS_NUXMV = ADAPTERS["nuxmv"].available()
needs_z3 = pytest.mark.skipif(not HAS_Z3, reason="z3 not installed")
needs_both = pytest.mark.skipif(not (HAS_Z3 and HAS_NUXMV), reason="z3 and nuXmv are needed")


def test_static_formulas():
    assert static(ltl.parse("x <= 3 & (b -> y > 2 * x)"))
    for f in ["G (x <= 3)", "x' = x", "X b", "a U b", "past(x, 1) = 0"]:
        assert not static(ltl.parse(f))


@needs_z3
def test_static_contracts_go_to_z3():
    gen = Contract.from_strings(name="Gen", inputs={"n": (0, 20)}, outputs={"p": "real"},
                                assumptions=["n <= 4"], guarantees=["p >= 2.5 * n", "p <= 30"])
    spec = Contract.from_strings(name="Spec", inputs={"n": (0, 20)}, outputs={"p": "real"},
                                 assumptions=["n = 4"], guarantees=["p >= 10"])
    assert verify(gen).feasible.backend == "z3"
    r = refines(gen, spec)
    assert r.status == "valid" and r.guarantees.backend == "z3"
    r = implies("p >= 2.5 * n & n >= 4", "p >= 10.5", {"p": "real", "n": (0, 20)})
    assert r.status == "invalid" and r.backend == "z3"
    assert r.trace.loop[0] == {"n": 4, "p": 10}
    # with a temporal operator, nuXmv
    assert is_satisfiable("G (p >= 0)", {"p": "real"}).backend.startswith("nuxmv")


@needs_z3
def test_what_z3_does_not_take():
    v = {"w": "word[8]"}
    assert "words" in why_not(Satisfiable(ltl.parse("w = 3"), v))
    assert "bounded number of steps" in why_not(Satisfiable(ltl.parse("F (x = 3)"), {"x": (0, 5)}))
    assert is_satisfiable("w = 3", v, backend="z3").status == "unknown"


@needs_z3
def test_smtlib():
    text = ADAPTERS["z3"].emit(Implies(ltl.parse("x >= 2"), ltl.parse("x > 1"), {"x": (0, 9)}))
    assert "(check-sat)" in text and "unsat means that phi -> psi is valid" in text
    with pytest.raises(ValueError, match="bounded number of steps"):
        ADAPTERS["z3"].emit(Satisfiable(ltl.parse("G (x >= 2)"), {"x": (0, 9)}))


def _formula(rng, depth):
    atoms = ["x <= 3", "y > x", "x + y = 6", "2 * x - y >= 1", "x != y", "r >= 1.5",
             "r < x", "x / 2 = 1", "b", "c"]
    if depth == 0 or rng.random() < 0.3:
        return rng.choice(atoms)
    op = rng.choice(["!", "&", "|", "->", "<->"])
    if op == "!":
        return f"!({_formula(rng, depth - 1)})"
    return f"({_formula(rng, depth - 1)}) {op} ({_formula(rng, depth - 1)})"


@needs_both
def test_z3_and_nuxmv_agree():
    rng = random.Random(3)
    v = {"x": (-4, 6), "y": (0, 7), "r": "real", "b": "bool", "c": "bool"}
    for _ in range(150):
        f, g = _formula(rng, 3), _formula(rng, 2)
        for check, args in [(is_satisfiable, (f, v)), (implies, (f, g, v))]:
            a = check(*args)
            b = check(*args, backend="nuxmv")
            assert a.backend == "z3" and a.status == b.status, (check.__name__, args)
            if a.trace is not None:
                state_holds = holds(f, a.trace)
                if check is is_satisfiable:
                    assert state_holds
                else:
                    assert state_holds and not holds(g, a.trace)


@needs_z3
def test_minimize_and_maximize():
    from chase_cps import maximize, minimize
    gen = Contract.from_strings(name="Gen", inputs={"n": (0, 20)}, outputs={"p": "real"},
                                assumptions=["n <= 4"], guarantees=["p >= 2.5 * n", "p <= 30"])
    best = maximize("n", gen)
    assert best.status == "optimal" and best.value == 4 and best.trace.loop[0]["n"] == 4
    # the margin of a guarantee: p - 2.5 n is 0 at best
    assert minimize("p - 2.5 * n", gen).value == 0
    # a disjunction: not a polyhedron
    best = maximize("3 * a + 2 * b", "a + b <= 4 & (a <= 1 | b <= 1)", {"a": (0, 10), "b": (0, 10)})
    assert best.value == 12 and best.trace.loop[0] == {"a": 4, "b": 0}
    v = {"x": "real"}
    assert minimize("x", "x > 1", v).status == "not_attained" and minimize("x", "x > 1", v).value == 1
    assert maximize("x", "x > 1", v).status == "unbounded"
    assert minimize("x", ["x > 1", "x < 0"], v).status == "infeasible"
    assert "bounded number of steps" in maximize("x", "G (x <= 3)", {"x": (0, 9)}).reason
    assert best.to_dict()["values"] == {"a": 4, "b": 0}


def test_bounded_formulas():
    assert bounded(ltl.parse("x' = x + 1 & X X (x - past(x, 2) <= 1) & G[0,4] (x <= 3)"))
    for f in ["G (x <= 3)", "F b", "a U b", "a R b", "X G b"]:
        assert not bounded(ltl.parse(f))


@needs_z3
def test_bounded_formulas_go_to_z3():
    v = {"x": (0, 9)}
    f = "x = 0 & G[0,3] (x' = x + 1) & F[0,4] (x >= 4)"
    r = is_satisfiable(f, v)
    assert r.status == "sat" and r.backend == "z3" and holds(f, r.trace)
    assert [s["x"] for s in r.trace.prefix + r.trace.loop] == [0, 1, 2, 3, 4]
    # past(x, n) before step 0: the history variables, as nuXmv writes them
    r = is_satisfiable("x = 3 & x - past(x, 1) = 2", v)
    assert r.trace.loop[0] == {"x": 3, "chase_past1_x": 1} and holds("x - past(x, 1) = 2", r.trace)


def _bounded_formula(rng, depth):
    atoms = ["x <= 3", "y > x", "x' = x + 1", "x' >= y", "x - past(x, 1) <= 1",
             "past(y, 2) = x", "b", "b' != b"]
    if depth == 0 or rng.random() < 0.25:
        return rng.choice(atoms)
    op = rng.choice(["!", "X", "G[0,2]", "F[1,2]", "&", "|", "->", "U[0,2]", "R[0,1]"])
    if op in ("!", "X") or op.startswith(("G", "F")):
        return f"{op} ({_bounded_formula(rng, depth - 1)})"
    return f"({_bounded_formula(rng, depth - 1)}) {op} ({_bounded_formula(rng, depth - 1)})"


@needs_both
def test_z3_and_nuxmv_agree_over_steps():
    rng = random.Random(9)
    v = {"x": (-2, 5), "y": (0, 4), "b": "bool"}
    for _ in range(120):
        f, g = _bounded_formula(rng, 3), _bounded_formula(rng, 2)
        for check, args in [(is_satisfiable, (f, v)), (implies, (f, g, v))]:
            a = check(*args)
            b = check(*args, backend="nuxmv")
            assert a.backend == "z3" and a.status == b.status, (check.__name__, args)
            if a.trace is not None:
                assert holds(f, a.trace, v)
                if check is implies:
                    assert not holds(g, a.trace, v)


@needs_z3
def test_the_worst_case_of_a_property_is_its_smallest_robustness():
    from chase_cps import maximize, minimize
    v = {"T": "real", "u": "real"}
    # a heater: on (u up to 3) while T <= 70, the room loses 1 per step
    heater = ["T = 60", "G[0,9] (u >= 0 & u <= 3)", "G[0,9] (T' = T + u - 1)",
              "G[0,9] (T > 70 -> u = 0)"]
    worst = minimize("G[0,10] (T <= 75)", heater, v)
    assert worst.status == "optimal" and worst.value == 3       # T reaches 72 at most
    assert robustness("G[0,10] (T <= 75)", worst.trace, v) == 3
    assert holds(ltl.conjunction([ltl.parse(h) for h in heater]), worst.trace, v)
    best = maximize("F[0,10] (T >= 72)", heater, v)
    assert best.status == "optimal" and best.value == 0
    assert robustness("F[0,10] (T >= 72)", best.trace, v) == 0
    # with T >= 70 -> u = 0, T stays below 72: the bound is not reached
    loose = heater[:3] + ["G[0,9] (T >= 70 -> u = 0)"]
    assert minimize("G[0,10] (T <= 75)", loose, v).status == "not_attained"
    r = minimize("G[0,10] (T <= 75) & b", heater, {**v, "b": "bool"})
    assert r.status == "unknown" and "inf or -inf" in r.reason


@needs_z3
def test_robustness_as_z3_computes_it_is_the_robustness_of_the_trace():
    from chase_cps import maximize, minimize
    rng = random.Random(4)
    v = {"x": (-3, 6), "y": "real"}
    atoms = ["x <= 2", "y > x", "x' - x >= 1", "y = 1.5", "x != 0", "y - past(x, 1) <= 2"]

    def formula(depth):
        if depth == 0 or rng.random() < 0.3:
            return rng.choice(atoms)
        op = rng.choice(["!", "X", "G[0,2]", "F[0,2]", "&", "|", "->", "<->"])
        if op in ("!", "X") or op[0] in "GF":
            return f"{op} ({formula(depth - 1)})"
        return f"({formula(depth - 1)}) {op} ({formula(depth - 1)})"
    for _ in range(60):
        f, c = formula(3), formula(2)
        for opt in (minimize, maximize):
            o = opt(f, [c, "G[0,8] (y >= -10 & y <= 10)"], v)
            assert o.status in ("optimal", "not_attained", "infeasible"), (f, c, o.reason)
            if o.status == "optimal":
                assert robustness(f, o.trace, v) == o.value, (f, c)
                assert holds(c, o.trace, v)
