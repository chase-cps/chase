"""Tests of the word variables (chase_cps.words): parsing, types, the checks
with nuXmv, the contract algebra through the core library, and state
machines. The checks are skipped without nuXmv (and the algebra without the
CHASE modules)."""

import random
import re

import pytest

from chase_cps import Contract, Model, implements, implies, is_receptive, is_satisfiable, ltl
from chase_cps import refines, to_spec, verify, words
from chase_cps.backends import ADAPTERS

HAS_MC = ADAPTERS["nuxmv"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False

needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv not installed")
needs_native = pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not built")

V = {"x": "word[4]", "y": "word[4]", "s": "signed word[4]", "t": "signed word[4]",
     "cnt": "word[8]", "b": "bool", "n": (0, 9)}


def test_types_and_constants():
    assert words.word_type("word[8]") == (8, False)
    assert words.word_type("unsigned word [8]") == (8, False)
    assert words.word_type("signed word[4]") == (4, True)
    assert words.word_type("bool") is None and words.word_type((0, 3)) is None
    assert words.normalize("unsigned word[8]") == "word[8]"
    assert words.literal(5, 8, False) == "0ud8_5"
    assert words.literal(-3, 4, True) == "-0sd4_3"
    with pytest.raises(ValueError):
        words.literal(16, 4, False)
    with pytest.raises(ValueError):
        words.literal(8, 4, True)
    assert words.value("0ud8_5") == 5 and words.value("-0sd4_3") == -3
    assert words.value("0ub4_0101") == 5 and words.value("0sh4_f") == -1
    assert words.value("TRUE") is None
    c = Contract.from_strings(inputs={"x": "unsigned word[4]"}, outputs={"s": "signed word[4]"},
                              guarantees=["G (s < 0 -> x = 0)"], name="T")
    assert c.variables == {"x": "word[4]", "s": "signed word[4]"}
    assert Contract.from_dict(c.to_dict()).to_dict() == c.to_dict()


def test_syntax():
    for text in ["x[3:0] = 3", "(x & y) = 0", "x[3] -> b", "cnt' = cnt + 1", "x :: y = cnt",
                 "x << 2 = y mod 3", "b xor x[0]", "(x + y)[1:0] = 0", "x'[0]"]:
        node = ltl.parse(text)
        assert ltl.parse(ltl.to_str(node)) == node, text
    assert ltl.bits(ltl.parse("x[3]")) == (3, 3)
    for bad in ["x[0:3] = 1", "x[a] = 1", "x[3"]:
        with pytest.raises(ltl.LTLSyntaxError):
            ltl.parse(bad)


@pytest.mark.parametrize("text, error", [
    ("x + cnt = 0", "same width"),
    ("x + s = 0", "same width and signedness"),
    ("x + n = 0", "integer constants"),
    ("x = 16", re.escape("is not a word[4]")),
    ("s = 8", re.escape("is not a signed word[4]")),
    ("cnt", "not a formula"),
    ("x[4] = 1", "bit 4"),
    ("b[0] = 1", "not a word"),
    ("x :: 3 = 0", "concatenates two words"),
])
def test_type_errors(text, error):
    with pytest.raises(ValueError, match=error):
        Contract.from_strings(inputs=dict(V), guarantees=[text], name="B")


def test_printing_for_nuxmv():
    p = lambda t: words.to_smv(ltl.parse(t), V)   # noqa: E731
    assert p("x = 5") == "(x = 0ud4_5)"
    assert p("s = -1") == "(s = -0sd4_1)"
    assert p("x[3] -> b") == "(bool(x[3:3]) -> b)"
    assert p("(x :: y) = 0") == "((x :: y) = 0ud8_0)"
    assert p("(x << 1) = 2") == "((x << 1) = 0ud4_2)"
    assert p("n = 5") == "(n = 5)"
    assert words.to_smv(ltl.parse("n = 5"), {"n": (0, 9)}) == ltl.to_smv(ltl.parse("n = 5"))


def test_evaluate():
    e = lambda t, s: words.evaluate(ltl.parse(t), s, V)   # noqa: E731
    assert e("x + 1", {"x": 15}) == 0
    assert e("s + 1", {"s": 7}) == -8
    assert e("x :: y", {"x": 1, "y": 2}) == 18
    assert e("x[3:2]", {"x": 12}) == 3
    assert e("s >> 1", {"s": -4}) == -2
    assert e("t / s", {"s": 2, "t": -7}) == -3 and e("t mod s", {"s": 2, "t": -7}) == -1
    assert e("!x", {"x": 5}) == 10


@needs_mc
def test_checks():
    assert implies("G (cnt' = cnt + 1) & cnt = 255", "X (cnt = 0)", V).status == "valid"
    assert implies("x = 5", "x[0] & !x[1] & x[2]", V).status == "valid"
    assert implies("x = 3 & y = 1", "(x :: y) = 49", V).status == "valid"
    assert implies("x = 12 & y = 10", "(x & y) = 8 & (x | y) = 14 & (x xor y) = 6", V).status \
        == "valid"
    assert implies("s = -1", "s < 0", V).status == "valid"
    assert implies("x = 15", "x > 0", V).status == "valid"
    r = implies("x = 15", "x + 1 > x", V)
    assert r.status == "invalid" and r.trace.prefix[0]["x"] == 15
    r = is_satisfiable("x + y = 0 & x != 0", V)
    assert r.status == "sat"
    state = r.trace.prefix[0]
    assert (state["x"] + state["y"]) % 16 == 0 and state["x"] != 0


@needs_mc
def test_nuxmv_and_evaluate_agree():
    # random operations on random values: the value computed in Python is
    # the one nuXmv finds
    rnd = random.Random(4)
    ops = ["+", "-", "*", "/", "mod", "&", "|", "xor", "<<", ">>", "::"]
    terms = []
    for _ in range(60):
        op = rnd.choice(ops)
        left, right = rnd.choice([("x", "y"), ("s", "t")])
        if op in ("<<", ">>"):
            right = str(rnd.randrange(4))
        if op == "::":
            left, right = "x", "y"
        terms.append(f"({left} {op} {right})")
    rows = 0
    for _ in range(6):
        state = {"x": rnd.randrange(16), "y": rnd.randrange(1, 16),
                 "s": rnd.randrange(-8, 8), "t": rnd.choice([-3, -2, -1, 1, 2, 5])}
        given = " & ".join(f"{v} = {k}" for v, k in state.items())
        facts = [f"{t} = {words.evaluate(ltl.parse(t), state, V)}" for t in terms]
        r = implies(given, " & ".join(facts), V)
        assert r.status == "valid", (state, r.status, r.reason)
        rows += 1
    assert rows == 6


@needs_mc
@needs_native
def test_algebra_is_the_same_as_on_integers():
    def mk(t, name, ins, outs, a, g):
        return Contract.from_dict({"name": name, "inputs": {v: t for v in ins},
                                   "outputs": {v: t for v in outs},
                                   "assumptions": a, "guarantees": g})
    p1 = ("C1", ["a"], ["b"], ["G (a < 200)", "a = 0"], ["G (b = a + 1)", "G (b' > b | a = 3)"])
    p2 = ("C2", ["b"], ["c"], ["G (b < 201)"], ["G (c = b * 2)", "F (c = 4)"])

    def view(c):
        return ([ltl.to_str(x) for x in c.assumptions], [ltl.to_str(x) for x in c.guarantees],
                sorted(c.inputs), sorted(c.outputs))
    for op in ["compose", "conjoin", "disjoin", "merge", "divide", "separate", "imply",
               "coimply"]:
        w = getattr(mk("word[8]", *p1), op)(mk("word[8]", *p2))
        i = getattr(mk((0, 255), *p1), op)(mk((0, 255), *p2))
        assert view(w) == view(i), op
        assert all(t == "word[8]" for t in w.variables.values())
    for op in ["invert", "simplify"]:
        assert view(getattr(mk("word[8]", *p1), op)()) == view(getattr(mk((0, 255), *p1), op)())
    # with the operators of words, which the core library does not read
    inc = Contract.from_dict({"name": "Inc", "inputs": {"a": "word[8]"},
                              "outputs": {"b": "word[8]"}, "assumptions": ["G (a < 200)"],
                              "guarantees": ["G (b = a + 1)"]})
    dbl = Contract.from_dict({"name": "Dbl", "inputs": {"b": "word[8]"},
                              "outputs": {"c": "word[8]"}, "assumptions": ["G (b < 201)"],
                              "guarantees": ["G (c = b << 1)", "G (c[0] = 0)"]})
    both = inc.compose(dbl, name="Both")
    assert both.inputs == {"a": "word[8]"} and set(both.outputs) == {"b", "c"}
    assert "G c = b << 1" in [ltl.to_str(g) for g in both.guarantees] or \
        any("b << 1" in ltl.to_str(g) for g in both.guarantees)
    spec = Contract.from_dict({"name": "Spec", "inputs": {"a": "word[8]"},
                               "outputs": {"c": "word[8]"}, "assumptions": ["G (a < 100)"],
                               "guarantees": ["G (c = (a + 1) << 1)"]})
    assert refines(both, spec).status == "valid"
    odd = Contract.from_dict({"name": "Odd", "inputs": {"a": "word[8]"},
                              "outputs": {"c": "word[8]"}, "assumptions": ["G (a < 100)"],
                              "guarantees": ["G (c[0] = 1)"]})
    assert refines(both, odd).status == "invalid"
    assert verify(both).status == "sat"


def test_what_words_do_not_go_to():
    c = Contract.from_dict({"name": "Inc", "inputs": {"a": "word[8]"},
                            "outputs": {"b": "word[8]"}, "guarantees": ["G (b = a + 1)"]})
    with pytest.raises(ValueError, match="logics language"):
        to_spec([c])
    from chase_cps import is_realizable
    r = is_realizable(c)
    assert r.status == "unknown" and "words" in r.reason


@needs_mc
def test_state_machines():
    m = Model.from_smv("MODULE main\nIVAR\n  a : unsigned word[8];\nVAR\n  b : unsigned word[8];\n"
                       "ASSIGN\n  init(b) := 0ud8_1;\n  next(b) := a + 0ud8_1;\n", name="M")
    assert m.variables == {"a": "word[8]", "b": "word[8]"}
    lag = Contract.from_dict({"name": "Lag", "inputs": {"a": "word[8]"},
                              "outputs": {"b": "word[8]"}, "guarantees": ["G (b' = a + 1)"]})
    assert implements(m, lag).status == "valid"
    assert is_receptive(m, lag).status == "receptive"
    wrong = Contract.from_dict({"name": "Wrong", "inputs": {"a": "word[4]"},
                                "outputs": {"b": "word[8]"}, "guarantees": ["G (a = 1)"]})
    with pytest.raises(ValueError, match="a is word"):
        implements(m, wrong)
    # an initial assumption on the bits of an input
    x4 = Contract.from_dict({"name": "X4", "inputs": {"x": "word[4]"}, "outputs": {"o": "bool"},
                             "assumptions": ["x[0] = 0"], "guarantees": ["G (o -> x[3])"]})
    blocks = Model.from_smv("MODULE main\nIVAR\n  x : unsigned word[4];\nVAR\n  o : boolean;\n"
                            "INIT !o\nTRANS next(o) = bool(x[3:3])\nTRANS x != 0ud4_2\n")
    assert implements(blocks, x4).status == "invalid"
    r = is_receptive(blocks, x4)
    assert r.status == "not_receptive" and r.details["input"] == {"x": 2}
    machine = {"name": "Echo", "inputs": {"x": "word[4]"}, "outputs": {"o": "bool"},
               "states": ["s"], "initial": "s",
               "transitions": [{"from": "s", "when": "x[3]", "outputs": {"o": True}, "to": "s"},
                               {"from": "s", "when": "!x[3]", "outputs": {"o": False}, "to": "s"}]}
    echo = Contract.from_dict({"name": "E", "inputs": {"x": "word[4]"}, "outputs": {"o": "bool"},
                               "guarantees": ["G (o <-> x[3])"]})
    assert implements(machine, echo).status == "valid"
