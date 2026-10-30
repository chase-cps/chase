"""Real variables: regions, the SMT engine of nuXmv, predicate abstraction.

The differential tests check the three ways against each other on random
formulas: they must never contradict each other, and the regions and the
SMT engine must agree whenever both answer. Set CHASE_REALS_RUNS to the
number of random formulas (default 25).
"""

import os
import random
from fractions import Fraction

import pytest

from chase_cps import ltl, predicates, reals
from chase_cps.backends import ADAPTERS, dispatch
from chase_cps.contract import Contract
from chase_cps.queries import Implies, Satisfiable, Trace

HAS_NUXMV = ADAPTERS["nuxmv"].available()
HAS_Z3 = predicates.z3 is not None
needs_nuxmv = pytest.mark.skipif(not HAS_NUXMV, reason="nuXmv not installed")
needs_z3 = pytest.mark.skipif(not HAS_Z3, reason="z3 not installed (chase-cps[reals])")
RUNS = int(os.environ.get("CHASE_REALS_RUNS", "25"))


def P(text):
    return ltl.parse(text)


# Without real variables nothing changes ---------------------------------------

def test_no_real_variable_takes_the_old_path(monkeypatch):
    import chase_cps as cc

    def fail(*args, **kwargs):
        raise AssertionError("chase_cps.reals used without real variables")
    monkeypatch.setattr(reals, "check", fail)
    assert not reals.has_real_variables(Implies(P("G a"), P("F a"), {"a": "bool", "n": (0, 3)}))
    if HAS_NUXMV:
        assert cc.implies("G a", "F a", {"a": "bool"}).status == "valid"


def test_negative_constants_in_the_logics_language():
    # the logics language reads -0.8 as one token and rejects it
    assert ltl.to_chase(P("w >= -0.8")) == "(w >= (0 - 0.8))"
    assert ltl.to_chase(P("n > -3")) == "(n > (0 - 3))"
    a = Contract.from_strings(name="A", inputs={"w": "real", "n": (-5, 5)}, outputs={"z": "bool"},
                              guarantees=["G (w >= -0.8 & n > -3 -> z)"])
    b = Contract.from_strings(name="B", inputs={"z": "bool"}, outputs={"u": "bool"},
                              guarantees=["G u"])
    c = a.compose(b, name="C")     # through the C++ library
    assert [ltl.to_str(g) for g in c.guarantees] == ["G ((w >= 0 - 0.8 & n > 0 - 3) -> z)", "G u"]


# Regions ----------------------------------------------------------------------

def test_regions_of_a_query():
    q = Implies(P("G (v > 40 -> X w)"), P("G (v' <= 30.5 | 45 < v)"), {"v": "real", "w": "bool"})
    r = reals.Regions.of(q)
    assert r.problem is None
    assert r.constants == {"v": [Fraction(61, 2), 40, 45]}
    f = r.finite(q)
    assert f.variables == {"v": (0, 6), "w": "bool"}
    assert ltl.to_str(f.phi) == "G (v > 3 -> X w)"
    assert ltl.to_str(f.psi) == "G (v' <= 1 | v > 5)"
    assert [r.value("v", i) for i in range(7)] == \
        [Fraction(59, 2), Fraction(61, 2), Fraction(141, 4), 40, Fraction(85, 2), 45, 46]
    assert r.describe("v", 2) == "30.5 < v < 40" and r.describe("v", 6) == "v > 45"


def test_regions_of_reals_compared_with_each_other():
    q = Implies(P("G (a = b & a > 40)"), P("G (b > 30)"), {"a": "real", "b": "real", "c": "real"})
    r = reals.Regions.of(q)
    assert r.size == {"a": 2, "b": 2, "c": 1}
    assert r.constants["a"] == r.constants["b"] == [30, 40]
    f = r.finite(q)
    assert f.variables == {"a": (0, 7), "b": (0, 7), "c": (0, 0)}
    assert ltl.to_str(f.phi) == "G (a = b & a > 5)" and ltl.to_str(f.psi) == "G (b > 2)"
    # two ordered slots in each interval
    assert [r.value("a", i) for i in (3, 4)] == [Fraction(100, 3), Fraction(110, 3)]


def test_no_regions_with_arithmetic_or_two_steps():
    q = Implies(P("G (v + 1 > 0)"), P("G (v > 0)"), {"v": "real"})
    assert "does not compare" in reals.Regions.of(q).problem
    q = Implies(P("G (v' > u)"), P("G (v > 0)"), {"v": "real", "u": "real"})
    assert reals.Regions.of(q).problem


@needs_nuxmv
def test_regions_answers():
    import chase_cps as cc
    V = {"kias": "real", "wing": "bool"}
    r = cc.implies("G (kias > 40 -> X wing)", "G (kias > 45 -> X wing)", V)
    assert (r.status, r.reals) == ("valid", "regions")
    r = cc.implies("G (kias > 45 -> X wing)", "G (kias > 40 -> X wing)", V)
    assert (r.status, r.reals) == ("invalid", "regions")
    # a counterexample of the real variables: kias between 40 and 45
    assert any(40 < s["kias"] <= 45 and not s.get("wing", True) or 40 < s["kias"] <= 45
               for s in r.trace.prefix + r.trace.loop)


@pytest.mark.skipif(not ADAPTERS["slugs"].available(), reason="slugs not installed")
def test_realizability_and_synthesis_with_regions():
    import chase_cps as cc
    mode = Contract.from_strings(inputs={"kias": "real"}, outputs={"wing": "bool"},
                                 guarantees=["G (kias' > 40 -> wing')",
                                             "G (kias' <= 30.5 -> !wing')"], name="Mode")
    r = cc.is_realizable(mode)
    assert (r.status, r.reals) == ("realizable", "regions")
    r = cc.synthesize(mode)
    assert r.controller["regions"]["kias"][4] == "kias > 40"
    # after the first step (a node reached by a transition), wing follows kias
    reached = {t for node in r.controller["nodes"].values() for t in node["trans"]}
    for k, node in r.controller["nodes"].items():
        if int(k) in reached and node["values"]["kias"] == "kias > 40":
            assert node["values"]["wing"] is True
        if int(k) in reached and node["values"]["kias"] == "kias < 30.5":
            assert node["values"]["wing"] is False
    bad = Contract.from_strings(inputs={"kias": "real"}, outputs={"wing": "bool"},
                                guarantees=["G (kias' > 40 -> wing')",
                                            "G (kias' < 45 -> !wing')"], name="Bad")
    assert cc.is_realizable(bad).status == "unrealizable"
    two = Contract.from_strings(inputs={"a": "real", "b": "real"}, outputs={"w": "bool"},
                                guarantees=["G (a' > b' -> w')", "G (a' < b' -> !w')"], name="Two")
    assert cc.is_realizable(two).status == "realizable"
    both = Contract.from_strings(inputs={"a": "real", "b": "real"}, outputs={"w": "bool"},
                                 guarantees=["G (a' > b' -> w')", "G (a' < 3 -> !w')"],
                                 name="Both")
    assert cc.is_realizable(both).status == "unrealizable"     # a = 2, b = 1
    worse = Contract.from_strings(inputs={"a": "real", "b": "real"}, outputs={"w": "bool"},
                                  guarantees=["G (a' > b' -> w')", "G (b' > 3 -> !w')"],
                                  name="Worse")
    assert cc.is_realizable(worse).status == "unrealizable"     # a > b > 3
    arith = Contract.from_strings(inputs={"a": "real", "b": "real"}, outputs={"w": "bool"},
                                  guarantees=["G (a' - b' > 1 -> w')"], name="Arith")
    r = cc.is_realizable(arith)
    assert r.status == "unknown" and "without arithmetic" in r.reason


# Predicates ---------------------------------------------------------------------

def test_normal_form_of_comparisons():
    a = predicates.Abstraction({"x": "real", "y": "real"})
    f = a.rewrite(P("x <= 10 & x > 10 & x' <= 10 & 10 >= x & 2*x <= 20 & 20 >= 2*x"))
    assert ltl.to_str(f) == "chase_p1 & !chase_p1 & chase_p1' & chase_p1 & chase_p1 & chase_p1"
    f = a.rewrite(P("x - y < 1 & y - x > -1 & y + 1 > x"))       # three times x - y < 1
    assert ltl.to_str(f) == "chase_p2 & chase_p2 & chase_p2"
    assert ltl.to_str(a.rewrite(P("x' - x > 0"))) == "chase_p3"     # two steps: not shifted
    assert ltl.to_str(a.nodes["chase_p3"]) == "x' - x > 0"
    assert ltl.to_str(a.rewrite(P("x' < x"))) == "!chase_p4"        # x - x' <= 0, negated


@needs_nuxmv
@needs_z3
def test_relations_between_predicates():
    V = {"x": "real", "m": "bool"}
    r = predicates.check(Satisfiable(P("G (x > 10) & F (x < 5)"), V))
    assert (r.status, r.reals) == ("unsat", "predicates")
    r = predicates.check(Satisfiable(P("G (x > 10) & F (x < 15)"), V))
    assert r.status == "sat"
    assert all(s["x"] > 10 for s in r.trace.prefix + r.trace.loop)
    assert any(s["x"] < 15 for s in r.trace.prefix + r.trace.loop)


@needs_nuxmv
@needs_z3
def test_nonlinear_with_predicates():
    V = {"a": "real", "b": "real"}
    r = predicates.check(Implies(P("G (a*a + b*b = 1)"), P("G (a <= 1 & a >= -1)"), V))
    assert r.status == "valid"
    r = predicates.check(Implies(P("G (a*a + b*b = 1)"), P("G (a <= 0.5)"), V))
    assert r.status == "invalid"
    for s in r.trace.prefix + r.trace.loop:     # a real counterexample
        assert abs(s["a"] ** 2 + s["b"] ** 2 - 1) < 1e-9
    assert any(s["a"] > Fraction(1, 2) for s in r.trace.prefix + r.trace.loop)


def test_untrusted_answers_are_unknown_without_z3(monkeypatch):
    if not HAS_NUXMV:
        pytest.skip("nuXmv not installed")
    monkeypatch.setattr(predicates, "z3", None)
    V = {"x": "real"}
    r = predicates.check(Implies(P("G (x > 10)"), P("G (x > 15)"), V))
    assert r.status == "unknown" and "z3" in r.reason
    assert "x <= 15" in r.trace.loop[0] or "x <= 10" in str(r.trace.to_dict())
    r = predicates.check(Implies(P("G (x > 15)"), P("G (x > 15 | x < 3)"), V))
    assert r.status == "valid"      # valid without arithmetic: trusted


@needs_z3
@needs_nuxmv
def test_steps_without_real_values_are_unknown():
    V = {"x": "real"}
    r = predicates.check(Implies(P("G (x' > x)"), P("F (x > 100)"), V))
    assert r.status == "unknown"


# Differential tests ------------------------------------------------------------

def _formula(rng, atoms, depth):
    if depth == 0 or rng.random() < 0.3:
        return rng.choice(atoms)
    op = rng.choice(["!", "G", "F", "X", "&", "|", "->"])
    if op in ("!", "G", "F", "X"):
        return ltl.Unary(op, _formula(rng, atoms, depth - 1))
    return ltl.Binary(op, _formula(rng, atoms, depth - 1), _formula(rng, atoms, depth - 1))


_CONSTANTS = [-1, 0, Fraction(1, 2), 2, 5]


def _constant_atoms(rng, k=4):
    atoms = [ltl.Var("w")]
    for _ in range(k):
        v = ltl.Var(rng.choice(["x", "y"]), primed=rng.random() < 0.2)
        atoms.append(ltl.Binary(rng.choice(["<", "<=", ">", ">=", "=", "!="]), v,
                                ltl.Num(ltl.number(rng.choice(_CONSTANTS)))))
    return atoms


def _order_atoms(rng, k=4):
    """Comparisons with constants and between real variables."""
    atoms = _constant_atoms(rng, k - 2)
    for _ in range(2):
        primed = rng.random() < 0.2
        atoms.append(ltl.Binary(rng.choice(["<", "<=", ">", ">=", "=", "!="]),
                                ltl.Var("x", primed), ltl.Var("y", primed)))
    return atoms


def _linear_atoms(rng, k=4):
    atoms = [ltl.Var("w")]
    for _ in range(k):
        x, y = ltl.Var("x"), ltl.Var("y")
        left = rng.choice([ltl.Binary("+", x, y), ltl.Binary("-", x, y),
                           ltl.Binary("*", ltl.Num(2), x), x])
        atoms.append(ltl.Binary(rng.choice(["<", "<=", ">", ">="]), left,
                                ltl.Num(ltl.number(rng.choice(_CONSTANTS)))))
    return atoms


def _hard_atoms(rng, k=4):
    """Nonlinear terms and comparisons of two steps."""
    atoms = [ltl.Var("w")]
    x, y, x1 = ltl.Var("x"), ltl.Var("y"), ltl.Var("x", primed=True)
    for _ in range(k):
        left = rng.choice([ltl.Binary("*", x, x), ltl.Binary("*", x, y),
                           ltl.Binary("-", x1, x), ltl.Binary("+", ltl.Binary("*", x, x), y), y])
        atoms.append(ltl.Binary(rng.choice(["<", "<=", ">", ">="]), left,
                                ltl.Num(ltl.number(rng.choice(_CONSTANTS)))))
    return atoms


V = {"x": "real", "y": "real", "w": "bool"}
_CONTRADICTS = {("valid", "invalid"), ("invalid", "valid"), ("sat", "unsat"), ("unsat", "sat")}


def _mutate(rng, node, atoms):
    """The formula with some of its atoms replaced: close to the original, so
    that whether one implies the other depends on the arithmetic."""
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, _mutate(rng, node.arg, atoms))
    if isinstance(node, ltl.Binary) and node.op not in ltl.RELATIONS:
        return ltl.Binary(node.op, _mutate(rng, node.left, atoms),
                          _mutate(rng, node.right, atoms))
    return rng.choice(atoms) if rng.random() < 0.4 else node


def _query(rng, atoms):
    phi = _formula(rng, atoms, 3)
    psi = _mutate(rng, phi, atoms)
    if rng.random() < 0.5:
        return Implies(phi, psi, V)
    return Satisfiable(ltl.Binary("&", phi, ltl.Unary("!", psi)), V)


def _answers(q, with_regions, timeout=60):
    out = {}
    if with_regions:
        reg = reals.Regions.of(q)
        assert reg.problem is None
        out["regions"] = dispatch(reg.finite(q), timeout=timeout).status
    out["smt"] = dispatch(q, "nuxmv", timeout=timeout).status
    if HAS_Z3:
        out["predicates"] = predicates.check(q, timeout=timeout).status
    return out


def _agree(answers, q):
    known = {k: v for k, v in answers.items() if v != "unknown"}
    for a in known.values():
        for b in known.values():
            assert (a, b) not in _CONTRADICTS, (answers, q)
    if "regions" in known and "smt" in known:
        assert known["regions"] == known["smt"], (answers, q)


@needs_nuxmv
def test_differential_constant_comparisons():
    rng = random.Random(7)
    for _ in range(RUNS):
        q = _query(rng, _constant_atoms(rng))
        _agree(_answers(q, with_regions=True), q)


@needs_nuxmv
def test_differential_order_between_reals():
    rng = random.Random(17)
    for _ in range(RUNS):
        q = _query(rng, _order_atoms(rng))
        _agree(_answers(q, with_regions=True), q)


@needs_nuxmv
def test_differential_linear_arithmetic():
    rng = random.Random(11)
    unknown = 0
    for _ in range(RUNS):
        q = _query(rng, _linear_atoms(rng))
        answers = _answers(q, with_regions=False)
        _agree(answers, q)
        unknown += answers.get("predicates") == "unknown"
    # with z3, most answers of the abstraction are exact for one-step atoms
    if HAS_Z3:
        assert unknown <= RUNS // 2


@needs_nuxmv
def test_differential_nonlinear_and_two_steps():
    # answers may be unknown, never contradictory
    rng = random.Random(13)
    for _ in range(max(RUNS // 3, 5)):
        q = _query(rng, _hard_atoms(rng))
        _agree(_answers(q, with_regions=False, timeout=5), q)
