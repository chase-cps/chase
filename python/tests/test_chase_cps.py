"""Tests of the chase_cps package.

The tests needing a model checker (nuXmv or NuSMV), slugs, or the compiled
CHASE modules are skipped when those are not installed.
"""

import os

import pytest

from chase_cps import ltl
from chase_cps.backends import ADAPTERS
from chase_cps.contract import Contract

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False

needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
needs_native = pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not installed")

ROBOTIC_JOINTS = os.environ.get(
    "CHASE_DEMOS", os.path.join(os.path.dirname(__file__), "..", "..", "demos"))
ROBOTIC_JOINTS = os.path.join(ROBOTIC_JOINTS, "RoboticJoints", "specs.ltl")


# LTL parser and printers ------------------------------------------------------

@pytest.mark.parametrize("text, chase, smv", [
    ("a -> b & c", "(a -> (b & c))", "(a -> (b & c))"),
    ("a & b -> c", "((a & b) -> c)", "((a & b) -> c)"),
    ("a -> b -> c", "(a -> (b -> c))", "(a -> (b -> c))"),
    ("[]<>p", "(G ((F (p))))", "G (F (p))"),
    ("x' = x + 1", "(x' = (x + 1))", "(next(x) = (x + 1))"),
    ("a R b", "(a R b)", "(a V b)"),
    ("!a U b | c", "(((! (a)) U b) | c)", "((! (a) U b) | c)"),
    ("TRUE -> X false", "(true -> (X (false)))", "(TRUE -> X (FALSE))"),
])
def test_parse_and_print(text, chase, smv):
    node = ltl.parse(text)
    assert ltl.to_chase(node) == chase
    assert ltl.to_smv(node) == smv
    assert ltl.parse(ltl.to_str(node)) == node     # round trip


@pytest.mark.parametrize("bad", ["", "a & & b", "(a", "G", "a b", "x @ y"])
def test_syntax_errors(bad):
    with pytest.raises(ltl.LTLSyntaxError):
        ltl.parse(bad)


@pytest.mark.parametrize("bad", ["G (b < X b)", "G (b + F b > 1)", "G (n = (a U b))"])
def test_temporal_operators_only_apply_to_formulas(bad):
    with pytest.raises(ltl.LTLSyntaxError):
        ltl.parse(bad)
    assert ltl.parse("G (b < b')") and ltl.parse("G (X (b = 1))")


def test_weak_until_expands():
    assert ltl.parse("a W b") == ltl.parse("(a U b) | G a")


# Contracts ---------------------------------------------------------------------

def test_from_strings_and_dict_round_trip():
    c = Contract.from_strings(inputs={"req": "bool", "n": (0, 3)}, outputs=["grant"],
                              assumptions="G F !req & n <= 2",
                              guarantees=["G (req -> F grant)"], name="Arbiter")
    assert len(c.assumptions) == 2          # top-level conjunction is split
    assert Contract.from_dict(c.to_dict()) == c
    assert "integer (0:3) variable n" in c.to_logics()


def test_undeclared_variable_is_rejected():
    with pytest.raises(ValueError, match="undeclared"):
        Contract.from_strings(inputs=["a"], guarantees=["G b"])


@pytest.mark.parametrize("name", ["P", "G", "input", "1x", "a-b"])
def test_reserved_or_bad_names_are_rejected(name):
    with pytest.raises(ValueError, match="cannot be used as a name"):
        Contract.from_strings(inputs=[name], name="Ok")


def test_input_and_output_clash_is_rejected():
    with pytest.raises(ValueError, match="both input and output"):
        Contract.from_strings(inputs=["a"], outputs=["a"])


@pytest.mark.parametrize("t, message", [
    ("integer", "unsupported variable type 'integer'"),
    (5, "unsupported variable type 5"),
    ({"a": 1}, "unsupported variable type"),
    ([1.5, 3], "not an integer"),
    (["a", "b"], "not an integer"),
    ([3, 1], "empty integer range"),
])
def test_bad_variable_types_are_rejected(t, message):
    with pytest.raises(ValueError, match=message):
        Contract.from_dict({"name": "A", "outputs": {"y": t}, "guarantees": []})


# Checks (model checker needed) ----------------------------------------------------

V = {"req": "bool", "grant": "bool", "x": (0, 4)}


@needs_mc
def test_implies_valid_and_invalid_with_counterexample():
    from chase_cps import implies
    assert implies("G (req -> X grant)", "G (req -> F grant)", V).status == "valid"
    r = implies("G (req -> F grant)", "G (req -> X grant)", V)
    assert r.status == "invalid"
    states = r.trace.prefix + r.trace.loop
    # The counterexample violates req -> X grant somewhere.
    assert any(s["req"] and not states[i + 1]["grant"]
               for i, s in enumerate(states[:-1])) or r.trace.loop


@needs_mc
def test_primed_variables_mean_next_step():
    from chase_cps import implies
    assert implies("x = 0 & G (x < 4 -> x' = x + 1)", "F x = 4", V).status == "valid"
    assert implies("x = 0 & G (x' = x)", "F x = 4", V).status == "invalid"


@needs_mc
def test_satisfiable():
    from chase_cps import is_satisfiable
    assert is_satisfiable("G req & F !req", V).status == "unsat"
    r = is_satisfiable("G F req & G F !req", V)
    assert r.status == "sat" and r.trace.loop


@needs_mc
def test_refinement_consistency_compatibility():
    from chase_cps import is_compatible, is_consistent, refines
    spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 assumptions=["G F !req"],
                                 guarantees=["G (req -> F grant)"], name="Spec")
    impl = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 guarantees=["G (req -> X grant)"], name="Impl")
    assert refines(impl, spec).status == "valid"
    r = refines(spec, impl)     # impl accepts every environment, spec does not
    assert r.status == "invalid" and r.failed == "assumptions"
    assert is_consistent(spec).status == "sat"
    assert is_compatible(spec).status == "sat"
    bad = Contract.from_strings(inputs=["req"], assumptions=["G req", "F !req"], name="Bad")
    assert is_compatible(bad).status == "unsat"


@needs_mc
def test_remove_redundant():
    from chase_cps import remove_redundant
    c = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              assumptions=["G F !req", "F !req"],
                              guarantees=["G (req -> X grant)", "G (req -> F grant)",
                                          "G F (req -> grant) | !req"],
                              name="Red")
    out = remove_redundant(c)
    assert [ltl.to_str(f) for f in out.removed_assumptions] == ["F !req"]
    assert "G (req -> F grant)" in [ltl.to_str(f) for f in out.removed_guarantees]
    assert out.contract.guarantees[0] == ltl.parse("G (req -> X grant)")


@needs_mc
def test_backends_answer_or_report_unknown():
    from chase_cps import check, ltl as L
    from chase_cps.queries import Project
    r = check(Project(L.parse("a & b"), {"a": "bool", "b": "bool"}, ["a"]))
    assert r.status == "unknown" and "Project" in r.reason


# CHASE core library (compiled modules needed) ------------------------------------

@needs_native
def test_to_pychase_and_back():
    c = Contract.from_strings(inputs={"req": "bool", "n": (0, 3)}, outputs=["grant"],
                              assumptions=["G F !req"],
                              guarantees=["G (req -> F grant)", "n' = n"], name="K")
    back = Contract.from_pychase(c.to_pychase())
    assert back.variables == c.variables
    assert back.guarantees == c.guarantees     # primes survive the round trip


@needs_native
@pytest.mark.parametrize("formula", [
    "G a -> G c", "G F !a -> G F c", "a U c & a", "!G a | X (a -> c)",
    "F a <-> G (a -> X c)", "(a -> c) & G a", "a -> c & a",
])
def test_grouping_survives_the_chase_parser(formula):
    # CHASE gives G/F/X a lower precedence than the binary operators.
    c = Contract.from_strings(inputs=["a"], outputs=["c"], guarantees=[formula], name="Prop")
    back = Contract.from_pychase(c.to_pychase())
    assert ltl.conjunction(back.guarantees) == ltl.conjunction(c.guarantees)


@needs_native
def test_declarations_shared_by_the_contracts():
    from chase_cps import parse_spec
    cs = parse_spec("""NAME: shared;

proposition done;
integer variable n;
boolean variable unused;

CONTRACT One:
    input boolean variable req;
    Assumptions:
        G(F(req));
    Guarantees:
        G(req -> F(done & (n >= 1)));
""")
    c = cs["One"]
    assert set(c.inputs) == {"req"}
    assert c.outputs == {"done": "bool", "n": (0, 16)}   # integer without range: 0..16


@needs_native
def test_shared_proposition_with_a_relation():
    # A proposition of the system defined by a relation, used twice in a
    # contract and in two contracts: each use gets its own copy.
    from chase_cps import parse_spec
    cs = parse_spec("""NAME: shared;

integer (0:30) variable n;
proposition big is n > 20;

CONTRACT One:
    input boolean variable req;
    Assumptions:
        G(F(req));
    Guarantees:
        G(req -> big);
        G(big -> X(big));

CONTRACT Two:
    Guarantees:
        F(big);
""")
    assert cs["One"].outputs == {"n": (0, 30)} and cs["Two"].outputs == {"n": (0, 30)}
    assert cs["One"].guarantees == [ltl.parse("G (req -> n > 20)"),
                                    ltl.parse("G (n > 20 -> X (n > 20))")]
    assert cs["Two"].guarantees == [ltl.parse("F (n > 20)")]
    both = cs["One"].conjoin(cs["Two"])
    assert ltl.variables(both.guarantee) == {"req", "n"}


# AddressSanitizer and Guard Malloc keep the freed memory for a while, to
# catch its later use: the memory of the process grows under them.
CHECKED_MALLOC = any(k in os.environ.get(v, "") for v in ("LD_PRELOAD", "DYLD_INSERT_LIBRARIES")
                     for k in ("asan", "gmalloc"))


@needs_native
@pytest.mark.skipif(CHECKED_MALLOC, reason="the allocator keeps the freed memory")
def test_operations_free_their_memory():
    # The objects that the core library makes for an operation are freed:
    # the memory does not grow with the number of operations (it grew by
    # 2 to 16 KB per operation in 0.2.0, depending on the contracts).
    import resource
    import sys
    a = Contract.from_strings(inputs=["a"], outputs=["b"], name="A",
                              assumptions=["G F a"], guarantees=["G (a -> F b)", "G (b -> X !b)"])
    b = Contract.from_strings(inputs=["b"], outputs=["c"], name="B",
                              assumptions=["G (b -> X !b)"], guarantees=["G (b -> X c)"])

    def run(n):
        for _ in range(n):
            a.compose(b)
            a.conjoin(b)
            a.divide(b)
            a.invert()
            a.simplify()

    def peak():   # bytes
        r = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
        return r if sys.platform == "darwin" else r * 1024

    run(50)
    before = peak()
    run(1000)
    assert (peak() - before) / 5000 < 1000   # bytes per operation


REAL_SPEC = """NAME: reals;

{decl}

CONTRACT One:
    input boolean variable req;
    Assumptions:
        G(F(req));
    Guarantees:
        G(req -> F(n <= {bound}));
"""


@needs_native
@pytest.mark.filterwarnings("ignore:.*outside its range")
def test_real_variables_and_numbers_are_read_exactly():
    # They were read as Boolean variables and as truncated integers.
    from chase_cps import parse_spec
    c = parse_spec(REAL_SPEC.format(decl="real variable n;", bound="20.5"))["One"]
    assert c.outputs == {"n": "real"}
    assert [ltl.to_str(g) for g in c.guarantees] == ["G (req -> F n <= 20.5)"]
    c = parse_spec(REAL_SPEC.format(decl="integer variable n;", bound="20.0"))["One"]
    assert [ltl.to_str(g) for g in c.guarantees] == ["G (req -> F n <= 20)"]
    c = parse_spec(REAL_SPEC.format(decl="real constant K 2.5;\nreal variable n;",
                                    bound="K"))["One"]
    assert [ltl.to_str(g) for g in c.guarantees] == ["G (req -> F n <= 2.5)"]
    with pytest.raises(ValueError, match="range"):
        parse_spec(REAL_SPEC.format(decl="real (0:5) variable n;", bound="2"))
    with pytest.raises(RuntimeError, match="integer constant"):
        parse_spec(REAL_SPEC.format(decl="integer constant K 2.5;\ninteger variable n;",
                                    bound="K"))


def test_numbers_with_a_fraction():
    from fractions import Fraction
    assert ltl.parse("x <= 20.5") == ltl.Binary("<=", ltl.Var("x"), ltl.Num(Fraction(41, 2)))
    assert ltl.parse("x <= 20.0") == ltl.parse("x <= 20")
    assert ltl.parse("x < -0.25") == ltl.Binary("<", ltl.Var("x"), ltl.Num(Fraction(-1, 4)))
    assert ltl.to_smv(ltl.Num(Fraction(1, 3))) == "f'1/3"
    assert ltl.to_chase(ltl.parse("x <= 0.125")) == "(x <= 0.125)"
    with pytest.raises(ValueError):
        ltl.to_chase(ltl.Num(Fraction(1, 3)))   # no decimal for the CHASE language
    c = Contract.from_strings(inputs={"v": "real"}, outputs={"m": "bool"},
                              guarantees=["G (v > 2.5 -> m)"], name="Speed")
    assert Contract.from_dict(c.to_dict()) == c
    assert "input real variable v;" in c.to_logics()


@needs_mc
def test_real_variables_with_nuxmv():
    from fractions import Fraction
    from chase_cps import implies, is_satisfiable
    if not ADAPTERS["nuxmv"].available():
        pytest.skip("nuXmv not installed")
    V = {"v": "real", "m": "bool", "n": (0, 30)}
    assert implies("G (v > 30)", "G (v >= 20)", V).status == "valid"
    r = implies("G (v > 0.25)", "G (v > 0.5)", V)
    assert r.status == "invalid"
    assert all(Fraction(1, 4) < s["v"] for s in r.trace.prefix + r.trace.loop)
    assert any(s["v"] <= Fraction(1, 2) for s in r.trace.prefix + r.trace.loop)
    r = is_satisfiable("G (3 * v = 1)", V)
    assert r.status == "sat" and r.trace.loop[0]["v"] == Fraction(1, 3)
    assert r.trace.to_dict()["loop"][0]["v"] == "1/3"
    # a number with a fraction and no real variable: also the SMT engine
    assert implies("G (n <= 20.5)", "G (n <= 20)", V).status == "valid"


@needs_native
def test_bad_formula_raises_instead_of_exiting():
    from chase_cps.contract import _parse_contracts
    c = Contract(name="X", inputs={"a": "bool"}, guarantees=[ltl.Var("b")])
    with pytest.raises(RuntimeError):
        _parse_contracts([c])


@needs_native
def test_syntax_errors_of_a_spec_give_the_line():
    from chase_cps import parse_spec
    with pytest.raises(RuntimeError, match=r"in the specification: line 3:14 .*'\['"):
        parse_spec("NAME: x;\nCONTRACT A:\n Assumptions: []p;\n")


@needs_native
@needs_mc
def test_composition_refines_and_separation():
    from chase_cps import refines
    c1 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                               guarantees=["G (a -> b)"], name="C1")
    c2 = Contract.from_strings(inputs=["b"], outputs=["c"], assumptions=["G b"],
                               guarantees=["G (b -> c)"], name="C2")
    top = Contract.from_strings(inputs=["a"], outputs=["c"], assumptions=["G a"],
                                guarantees=["G c"], name="Top")
    assert refines(c1.compose(c2), top).status == "valid"
    # separation: Top is refined by (Top / sep C2) merged with C2
    s = top.separate(c2)
    assert refines(top, s.merge(c2)).status == "valid"


@needs_native
@needs_mc
def test_compose_with_discharge():
    from chase_cps import refines
    c1 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                               guarantees=["G (a -> b)"], name="C1")
    c2 = Contract.from_strings(inputs=["b"], outputs=["c"], assumptions=["G b"],
                               guarantees=["G (b -> c)"], name="C2")
    top = Contract.from_strings(inputs=["a"], outputs=["c"], assumptions=["G a"],
                                guarantees=["G c"], name="Top")
    for c in (c1.compose(c2, discharge=True), c2.compose(c1, discharge=True)):
        # G b is implied by the assumption and the guarantee of C1
        assert [ltl.to_str(t) for t in c.assumptions] == ["G a"]
        assert sorted(ltl.to_str(t) for t in c.guarantees) == ["G (a -> b)", "G (b -> c)"]
        assert (set(c.inputs), set(c.outputs)) == ({"a"}, {"b", "c"})
        # an abstraction of the closed form, which still refines Top
        assert refines(c1.compose(c2), c).status == "valid"
        assert refines(c, top).status == "valid"
    # feedback: nothing is discharged
    back = Contract.from_strings(inputs=["c"], outputs=["a"], assumptions=["G c"],
                                 guarantees=["G (c -> a)"], name="Back")
    loop = Contract.from_strings(inputs=["a", "c"], outputs=["b"], assumptions=["G a"],
                                 guarantees=["G (a -> b)"], name="Loop")
    c = c2.compose(back).compose(loop, discharge=True)
    assert refines(c2.compose(back).compose(loop), c).status == "valid"


@needs_native
@needs_mc
def test_compose_with_discharge_under_feedback():
    from chase_cps import refines
    # the arbiter grants the requests of the robot, which requests on low battery:
    # the robot's assumption on the grants is discharged by the arbiter (one direction)
    arb = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                guarantees=["G F (grant | !req)", "!grant"], name="Arb")
    rob = Contract.from_strings(inputs=["grant", "low"], outputs=["req"],
                                assumptions=["G F (grant | !req)", "G F low"],
                                guarantees=["G (low -> F req)", "G (req & !grant -> X req)"],
                                name="Rob")
    top = Contract.from_strings(inputs=["low"], outputs=["req", "grant"],
                                assumptions=["G F low"], guarantees=["G F (grant | !req)"],
                                name="Top")
    for a, b in ((arb, rob), (rob, arb)):
        c = a.compose(b, discharge=True)
        assert [ltl.to_str(t) for t in c.assumptions] == ["G F low"]
        assert refines(a.compose(b), c).status == "valid"
        assert refines(c, top).status == "valid"


@needs_native
@needs_mc
def test_merge_and_divide_with_discharge():
    from chase_cps import refines

    def same(x, y):
        return refines(x, y).status == "valid" == refines(y, x).status

    def strs(terms):
        return sorted(ltl.to_str(t) for t in terms)
    grant = Contract.from_strings(inputs=["r"], outputs=["g"], guarantees=["G F (g | !r)"],
                                  name="Grant")
    lock = Contract.from_strings(inputs=["r", "l"], outputs=["g"], assumptions=["G (l -> r)"],
                                 guarantees=["G (l -> g)", "G F (l -> g)"], name="Lock")
    m = grant.merge(lock, discharge=True)
    # the terms of both; G F (l -> g) is implied by G (l -> g)
    assert strs(m.assumptions) == ["G (l -> r)"]
    assert strs(m.guarantees) == ["G (l -> g)", "G F (g | !r)"]
    assert (set(m.inputs), set(m.outputs)) == ({"r", "l"}, {"g"})
    assert same(m, grant.merge(lock))
    # the system by a part that drives b; its assumption G a is implied by the
    # system's, so its guarantee is assumed as it is
    system = Contract.from_strings(inputs=["a"], outputs=["b", "c"], assumptions=["G a"],
                                   guarantees=["G c"], name="Sys")
    part = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                                 guarantees=["G (a -> b)"], name="Part")
    q = system.divide(part, discharge=True)
    assert strs(q.assumptions) == ["G (a -> b)", "G a"]
    assert strs(q.guarantees) == ["G c"]
    assert (set(q.inputs), set(q.outputs)) == ({"a", "b"}, {"c"})
    assert same(q, system.divide(part))
    # an assumption of the part that the system does not give: it stays the
    # premise of the guarantees of the part, and a guarantee of the quotient
    other = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G F a"],
                                  guarantees=["G F b"], name="Other")
    loose = Contract.from_strings(inputs=["a"], outputs=["b", "c"], guarantees=["G c"],
                                  name="Loose")
    q = loose.divide(other, discharge=True)
    assert strs(q.assumptions) == ["G F a -> G F b"]
    assert strs(q.guarantees) == ["G F a", "G c"]
    assert same(q, loose.divide(other))


@needs_native
@needs_mc
def test_conjoin_with_discharge():
    from chase_cps import refines

    def same(x, y):
        return refines(x, y).status == "valid" == refines(y, x).status

    def strs(terms):
        return sorted(ltl.to_str(t) for t in terms)
    c1 = Contract.from_strings(inputs=["r"], outputs=["g"], assumptions=["G F r"],
                               guarantees=["G F g"], name="C1")
    # the assumptions of C2 imply those of C1: the assumption is A1, and only
    # G F !r is left as the premise of the guarantee of C2
    c2 = Contract.from_strings(inputs=["r"], outputs=["g"], assumptions=["G F r", "G F !r"],
                               guarantees=["G (g -> r)"], name="C2")
    for c in (c1.conjoin(c2, discharge=True), c2.conjoin(c1, discharge=True)):
        assert strs(c.assumptions) == ["G F r"]
        assert strs(c.guarantees) == ["G F !r -> G (g -> r)", "G F g"]
        assert same(c, c1.conjoin(c2))
    # the same assumptions: the conjunction is (A, G1 & G2)
    c3 = Contract.from_strings(inputs=["r"], outputs=["g"], assumptions=["G F r"],
                               guarantees=["G (g -> r)"], name="C3")
    c = c1.conjoin(c3, discharge=True)
    assert strs(c.assumptions) == ["G F r"]
    assert strs(c.guarantees) == ["G (g -> r)", "G F g"]
    # neither implies the other: a disjunction, each guarantee under its premise
    c4 = Contract.from_strings(inputs=["r", "s"], outputs=["g"], assumptions=["G F s"],
                               guarantees=["G (g -> r)"], name="C4")
    c = c1.conjoin(c4, discharge=True)
    assert strs(c.assumptions) == ["G F r | G F s"]
    assert strs(c.guarantees) == ["G F r -> G F g", "G F s -> G (g -> r)"]
    assert same(c, c1.conjoin(c4))


@needs_native
@needs_mc
def test_implication_coimplication_and_reciprocal():
    from chase_cps import refines
    c1 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                               guarantees=["G F b"], name="C1")
    c2 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G F a"],
                               guarantees=["G (a -> b)"], name="C2")
    # C1 -> C2 is the largest C with C & C1 <= C2; C1 ~> C2 the smallest C
    # with C2 <= C | C1
    assert refines(c1.imply(c2).conjoin(c1), c2).status == "valid"
    assert refines(c2, c1.coimply(c2).disjoin(c1)).status == "valid"
    assert refines(c2.conjoin(c1), c1.imply(c2)).status == "valid"
    # reciprocal: (G, A), inputs and outputs exchanged, an involution
    r = c1.invert()
    assert (set(r.inputs), set(r.outputs)) == ({"b"}, {"a"})
    back = r.invert()
    assert refines(back, c1).status == "valid" and refines(c1, back).status == "valid"
    # quotient = C1 . C2^-1 (arXiv 2309.08875, Section 2.6)
    q, mq = c1.divide(c2), c1.merge(c2.invert())
    assert refines(q, mq).status == "valid" and refines(mq, q).status == "valid"


@needs_native
@needs_mc
def test_assume_and_ensure():
    from chase_cps import refines
    c = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                              guarantees=["G (a -> b)"], name="C")
    assert refines(c, c.assume("G F b")).status == "valid"
    assert refines(c.ensure("G (b -> X b)"), c).status == "valid"
    assert refines(c.assume("G F b"), c).status == "invalid"
    with pytest.raises(ValueError):
        c.assume("G z")


@needs_native
@needs_mc
def test_ensure_without_assumptions_keeps_the_terms():
    from chase_cps import refines
    from chase_cps.contract import _algebra, _single_term
    c = Contract.from_strings(inputs=["a"], outputs=["b"], guarantees=["G (a -> b)"], name="C")
    e = c.ensure("G (b -> X b)")
    assert e.assumptions == [] and len(e.guarantees) == 2
    composed = _algebra("composition", c, _single_term(c, "G (b -> X b)", assumption=False), "K")
    assert refines(e, composed).status == "valid" and refines(composed, e).status == "valid"


@needs_native
@needs_mc
def test_quotient_composed_with_divisor_refines():
    from chase_cps import refines
    # C2 || (C1 / C2) <= C1, also when A2 is not implied by A1 and G2.
    c1 = Contract.from_strings(inputs=["a"], outputs=["c"], assumptions=["G a"],
                               guarantees=["G c"], name="C1")
    c2 = Contract.from_strings(inputs=["b"], outputs=["c"], assumptions=["G b"],
                               guarantees=["G (b -> c)"], name="C2")
    q = c1.divide(c2)
    assert refines(c2.compose(q), c1).status == "valid"


@needs_native
@needs_mc
@pytest.mark.skipif(not os.path.isfile(ROBOTIC_JOINTS), reason="demos not found")
def test_robotic_joints_refinement_and_counterexample():
    from chase_cps import refines
    from chase_cps._native import pychase_logicsLang
    system = pychase_logicsLang.LogicsSpecsBuilder().parseSpecificationFile(ROBOTIC_JOINTS)
    C = {c.getName().getString(): Contract.from_pychase(c) for c in system.getContractsSet()}
    composed = C["Joint3"].compose(C["Joint1"].compose(C["Joint2"]))
    nuxmv = (os.environ.get("CHASE_BACKEND", "nuxmv") != "nusmv"
             and ADAPTERS["nuxmv"].available())
    if nuxmv:   # IC3 proves it in seconds
        assert refines(composed, C["Joints"]).status == "valid"
    else:       # NuSMV proves it with BDDs, which take minutes
        assert refines(composed, C["Joints"], timeout=60).status in ("valid", "unknown")
    j1 = C["Joint1"]
    weak = Contract(j1.name, j1.inputs, j1.outputs, j1.assumptions,
                    [g for g in j1.guarantees if ltl.to_str(g) != "G F j1_done"])
    r = refines(C["Joint3"].compose(weak.compose(C["Joint2"])), C["Joints"])
    assert r.status == "invalid" and r.failed == "guarantees"
    assert all(s["req_j1"] and not s["j1_done"] for s in r.guarantees.trace.loop)


def _has_slugs():
    try:
        from chase_cps.backends import SlugsAdapter
        return SlugsAdapter().available()
    except ImportError:
        return False


@needs_native
@pytest.mark.skipif(not _has_slugs(), reason="slugs not installed")
def test_realizability_and_synthesis():
    from chase_cps import is_realizable, synthesize
    ok = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G F !req"],
                               guarantees=["G (req -> X grant)", "G F grant"], name="Arb")
    r = is_realizable(ok)
    assert r.status == "realizable" and r.controller is None
    r = synthesize(ok)
    assert r.status == "realizable" and "nodes" in r.controller
    bad = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                guarantees=["G (req -> X grant)", "G (req -> X !grant)"],
                                name="Bad")
    r = is_realizable(bad)
    assert r.status == "unrealizable" and "State" in r.counter_strategy


@needs_native
@pytest.mark.skipif(not _has_slugs(), reason="slugs not installed")
def test_terms_outside_gr1_are_reported():
    from chase_cps import is_realizable
    # F inside G, or G inside a Boolean term, is not GR(1): the slugs back end
    # must not drop the operator (G (req -> F grant) is realizable here, and
    # (req -> grant) is not).
    for a, g in [(["G F req", "G (stop -> X !stop)", "G F !stop"],
                  ["G (req -> F grant)", "G (stop -> !grant)"]),
                 (["G F req"], ["(G req) -> grant"]),
                 (["G (req -> F !req)"], ["G (req -> X grant)"])]:
        c = Contract.from_strings(inputs=["req", "stop"], outputs=["grant"],
                                  assumptions=a, guarantees=g, name="T")
        r = is_realizable(c)
        assert r.status == "unknown" and "not a GR(1) term" in r.reason
    # X inside a liveness term is accepted, as by slugs
    c = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              assumptions=["G F (req -> X !req)"],
                              guarantees=["G (req -> X grant)", "G F grant"], name="T")
    assert is_realizable(c).status == "realizable"


@needs_native
@pytest.mark.skipif(not _has_slugs(), reason="slugs not installed")
def test_unrealizable_core():
    from chase_cps import is_realizable, unrealizable_core
    g = ["!grant", "G (req -> X grant)", "G (stop -> X !grant)", "G F grant"]
    c = Contract.from_strings(inputs=["req", "stop"], outputs=["grant"],
                              assumptions=["G F req"], guarantees=g, name="Arb")
    expected = ["G (req -> X grant)", "G (stop -> X !grant)"]
    r = unrealizable_core(c)
    assert r.status == "found" and [ltl.to_str(t) for t in r.core.guarantees] == expected
    assert [ltl.to_str(t) for t in r.core.assumptions] == ["G F req"]
    r = unrealizable_core(g, inputs=["req", "stop"], outputs=["grant"], assumptions=["G F req"])
    assert [ltl.to_str(t) for t in r.core] == expected
    r = is_realizable(c, core=True)
    assert r.status == "unrealizable" and r.core.status == "found"
    assert r.to_dict()["core"]["core"]["guarantees"] == expected
    ok = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G F req"],
                               guarantees=["G (req -> X grant)"], name="Ok")
    assert unrealizable_core(ok).status == "none"
    bad = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                guarantees=["G (req -> F grant)"], name="Bad")
    assert unrealizable_core(bad).status == "unknown"


@needs_mc
def test_unsat_core():
    from chase_cps import is_compatible, is_consistent, is_satisfiable, unsat_core
    v = {"p": "bool", "q": "bool", "r": "bool"}
    expected = ["p", "G (p -> X p)", "F !p"]
    r = unsat_core(["p", "G (p -> X p)", "G F q", "F !p", "G (q -> r)"], v)
    assert r.status == "found" and [ltl.to_str(t) for t in r.core] == expected
    r = is_satisfiable("p & G (p -> X p) & G F q & F !p", v, core=True)
    assert r.status == "unsat" and [ltl.to_str(t) for t in r.core.core] == expected
    assert unsat_core(["p", "G F q"], v).status == "none"
    # the guarantees cannot hold when the assumptions do (A & G)
    c = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G req"],
                              guarantees=["G (req -> X !req)", "G grant"], name="C")
    r = unsat_core(c)
    assert [ltl.to_str(t) for t in r.core.assumptions] == ["G req"]
    assert [ltl.to_str(t) for t in r.core.guarantees] == ["G (req -> X !req)"]
    # incompatible: the core is in the assumptions; inconsistent: in the guarantees
    c = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a", "F !a", "G F a"],
                              guarantees=["G b"], name="C")
    r = is_compatible(c, core=True)
    assert [ltl.to_str(t) for t in r.core.core.assumptions] == ["G a", "F !a"]
    c = Contract.from_strings(inputs=["a"], outputs=["b"],
                              guarantees=["G b", "G F a", "F !b"], name="C")
    r = is_consistent(c, core=True)
    assert r.status == "unsat"
    assert [ltl.to_str(t) for t in r.core.core.guarantees] == ["G b", "F !b"]


@needs_native
@pytest.mark.skipif(not _has_slugs(), reason="slugs not installed")
def test_well_separation():
    from chase_cps import is_well_separated
    # granting forever forces the environment to stop requesting
    c = Contract.from_strings(inputs=["req", "x"], outputs=["grant"],
                              assumptions=["G F req", "G (grant -> X !req)", "G F x"],
                              guarantees=["G (req -> X grant)"], name="Arb")
    r = is_well_separated(c, core=True)
    assert r.status == "not_separated" and not r and r.controller
    assert [ltl.to_str(t) for t in r.core.core.assumptions] == ["G F req", "G (grant -> X !req)"]
    r = is_well_separated(["G F req", "G (grant -> X !req)"], inputs=["req"], outputs=["grant"])
    assert r.status == "not_separated"
    ok = Contract.from_strings(inputs=["req"], outputs=["grant"],
                               assumptions=["G F req", "G ((req & grant) -> X !req)"], name="Ok")
    assert is_well_separated(ok).status == "separated" and is_well_separated(ok)
    # keeping the safety guarantee G !grant, the component cannot force it
    d = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              assumptions=["G F req", "G (grant -> X !req)"],
                              guarantees=["G !grant"], name="D")
    assert is_well_separated(d).status == "separated"
    assert is_well_separated(d, with_guarantees=True).status == "separated"
    # without the guarantees (Def. 2), granting forever forces the violation
    assert is_well_separated(d, with_guarantees=False).status == "not_separated"
    n = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              assumptions=["G (req -> F grant)"], name="N")
    assert is_well_separated(n).status == "unknown"


@needs_mc
def test_remove_redundant_for_synthesis():
    from chase_cps import remove_redundant
    c = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G F req"],
                              guarantees=["G (req -> grant)", "G F grant"], name="A")
    v = remove_redundant(c, synth=True, core=True)
    assert [(ltl.to_str(t.term), t.part, t.kind) for t in v.removed] == \
        [("G F grant", "guarantee", "justice")]
    assert [ltl.to_str(t) for t in v.removed[0].core] == ["G F req", "G (req -> grant)"]
    assert [ltl.to_str(t) for t in v.contract.guarantees] == ["G (req -> grant)"]
    assert [ltl.to_str(t) for t in v.removed_guarantees] == ["G F grant"]
    assert v.to_dict()["removed"] == [{"term": "G F grant", "part": "guarantee",
                                       "kind": "justice",
                                       "core": ["G F req", "G (req -> grant)"]}]
    # duplicates: one is kept; a safety term implied by a stronger one goes
    e = Contract.from_strings(inputs=["a"], outputs=["b"],
                              assumptions=["G (a -> X a)", "G (a -> X (a | b))", "!a", "!a"],
                              guarantees=["G b"], name="E")
    v = remove_redundant(e, synth=True)
    assert [ltl.to_str(t) for t in v.contract.assumptions] == ["G (a -> X a)", "!a"]
    assert [ltl.to_str(t) for t in v.removed_assumptions] == ["G (a -> X (a | b))", "!a"]
    # safety terms are compared propositionally: G (b = 3) follows from the
    # other guarantee only over whole traces
    p = Contract.from_strings(inputs=["x"], outputs={"b": (0, 3)},
                              guarantees=["G (b = 3)", "G (b < 3 -> (b < b' & b' < 3))"],
                              name="Paper")
    assert [ltl.to_str(t) for t in remove_redundant(p).removed_guarantees] == ["G b = 3"]
    assert [ltl.to_str(t.term) for t in remove_redundant(p, synth=True).removed] == \
        ["G (b < 3 -> b < b' & b' < 3)"]
    n = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              guarantees=["G (req -> F grant)"], name="N")
    v = remove_redundant(n, synth=True)
    assert v.reason.startswith("not a GR(1) term") and v.contract.guarantees == n.guarantees
    with pytest.raises(TypeError):
        remove_redundant(c, synth=True, context="G req")
    # the two differ when the assumptions are wrong: unsatisfiable, an
    # assumption on the outputs
    bad = Contract.from_strings(inputs=["x"], outputs=["g"], assumptions=["G F (x & !x)"],
                                guarantees=["G g", "G !g"], name="Bad")
    assert remove_redundant(bad).contract.guarantees == []
    assert len(remove_redundant(bad, synth=True).contract.guarantees) == 2
    out = Contract.from_strings(inputs=["x"], outputs=["g"], assumptions=["G F g"],
                                guarantees=["G g"], name="Out")
    assert remove_redundant(out).contract.assumptions == out.assumptions
    assert remove_redundant(out, synth=True).contract.assumptions == []


@needs_mc
def test_remove_redundant_with_cores():
    from chase_cps import remove_redundant
    c = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              assumptions=["G F !req", "F !req"],
                              guarantees=["G (req -> X grant)", "G (req -> F grant)", "G grant"],
                              name="Red")
    out = remove_redundant(c, core=True)
    assert out.to_dict()["removed"] == [
        {"term": "F !req", "part": "assumption", "core": ["G F !req"]},
        {"term": "G (req -> X grant)", "part": "guarantee", "core": ["G grant"]},
        {"term": "G (req -> F grant)", "part": "guarantee", "core": ["G grant"]}]
    assert [ltl.to_str(t) for t in out.contract.guarantees] == ["G grant"]
    # without core, the terms removed are the same
    assert [t.term for t in remove_redundant(c).removed] == [t.term for t in out.removed]


@needs_native
@pytest.mark.skipif(not _has_slugs(), reason="slugs not installed")
def test_gr1_variable_roles_are_reported():
    from chase_cps import is_realizable
    for a, word in [(["!b | a"], "initial assumption"), (["G (a -> X b)"], "next value"),
                    (["G (a -> b')"], "next value")]:
        c = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=a,
                                  guarantees=["G F b"], name="T")
        r = is_realizable(c)
        assert r.status == "unknown" and word in r.reason
    c = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G (b -> X a)"],
                              guarantees=["G F b"], name="T")
    assert is_realizable(c).status == "realizable"


def test_rename_and_signatures():
    c = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                              guarantees=["G (a -> X b')"], name="C")
    r = c.rename({"a": "x", "b": "y"})
    assert r.inputs == {"x": "bool"} and r.outputs == {"y": "bool"}
    assert r.guarantees == [ltl.parse("G (x -> X y')")]
    assert not c.shares_io_with(r) and c.shares_io_with(c)
    d = Contract.from_strings(inputs=["b"], outputs=["c"], name="D")
    e = Contract.from_strings(outputs=["b"], name="E")
    assert c.can_compose_with(d) and not c.can_compose_with(e)
    with pytest.raises(ValueError, match="outputs of both"):
        c.compose(e)


@needs_mc
def test_environment_and_implementation():
    from chase_cps import is_environment, is_implementation
    spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 assumptions=["G F !req"],
                                 guarantees=["G (req -> F grant)"], name="Spec")
    assert is_environment("G (req -> X !req)", spec).status == "valid"
    assert is_environment("G req", spec).status == "invalid"
    assert is_implementation("G (grant <-> X req)", spec).status == "invalid"
    assert is_implementation("G grant", spec).status == "valid"
    # the names before 0.4.2 still work, with a warning (accepts takes the
    # contract first)
    from chase_cps import accepts, implements
    with pytest.warns(DeprecationWarning):
        assert accepts(spec, "G (req -> X !req)").status == "valid"
    with pytest.warns(DeprecationWarning):
        assert implements("G grant", spec).status == "valid"



@needs_native
@needs_mc
def test_conjunction_and_disjunction_bounds():
    from chase_cps import refines
    c1 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                               guarantees=["G b"], name="C1")
    c2 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G F a"],
                               guarantees=["F b"], name="C2")
    meet, join = c1.conjoin(c2), c1.disjoin(c2)
    for c in (c1, c2):
        assert refines(meet, c).status == "valid"
        assert refines(c, join).status == "valid"


@needs_native
def test_spec_text_round_trip():
    from chase_cps import parse_spec, to_spec
    c = Contract.from_strings(inputs={"req": "bool", "n": (0, 3)}, outputs=["grant"],
                              assumptions=["G F !req"],
                              guarantees=["G (req -> F grant)", "G (n < 3)"], name="Arbiter")
    back = parse_spec(to_spec([c]))["Arbiter"]
    assert back.inputs == c.inputs and back.outputs == c.outputs
    assert [ltl.to_str(f) for f in back.guarantees] == [ltl.to_str(f) for f in c.guarantees]


@needs_mc
def test_saturate_gives_an_equivalent_contract():
    from chase_cps import refines
    c = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G F !req"],
                              guarantees=["G (req -> F grant)"], name="C")
    s = c.saturate()
    assert ltl.to_str(s.guarantee) == "G F !req -> G (req -> F grant)"
    assert refines(c, s).status == "valid" and refines(s, c).status == "valid"


def test_doctor_lists_the_back_ends(capsys):
    from chase_cps.cli import main
    code = main(["doctor"])
    out = capsys.readouterr().out
    assert "Back ends:" in out and "nuxmv" in out and "slugs" in out
    assert code in (0, 1)


ARBITER_SPEC = """NAME: arbiter;

CONTRACT Spec:
    input boolean variable req;
    output boolean variable grant;
    Assumptions:
        G(F(!(req)));
    Guarantees:
        G(req -> F(grant));

CONTRACT Impl:
    input boolean variable req;
    output boolean variable grant;
    Assumptions:
        true;
    Guarantees:
        G(req -> X(grant));
"""


@needs_native
@needs_mc
def test_check_command(tmp_path, capsys):
    import json
    from chase_cps.cli import main
    spec = tmp_path / "arbiter.ltl"
    spec.write_text(ARBITER_SPEC)
    assert main(["check", str(spec)]) == 0
    out = capsys.readouterr().out
    assert out.count("sat") == 2 and out.count("verify") == 2
    assert main(["check", str(spec), "--verify", "--contracts", "Spec"]) == 0
    assert capsys.readouterr().out.count("sat") == 1
    assert main(["check", str(spec), "--compatible", "--consistent", "--contracts", "Spec"]) == 0
    assert capsys.readouterr().out.count("sat") == 2
    assert main(["check", str(spec), "--refines", "Impl", "Spec"]) == 0
    assert "valid" in capsys.readouterr().out
    assert main(["check", str(spec), "--refines", "Spec", "Impl", "--json"]) == 1
    report = json.loads(capsys.readouterr().out)
    assert not report["passed"] and report["checks"][0]["failed"] == "assumptions"
    assert main(["check", str(spec), "--refines", "Nope", "Spec"]) == 2
    contracts = tmp_path / "arbiter.json"
    contracts.write_text(json.dumps({"contracts": [
        {"name": "A", "inputs": {"x": "bool"}, "assumptions": ["G x", "F !x"]}]}))
    assert main(["check", str(contracts), "--compatible"]) == 1
    capsys.readouterr()
    assert main(["check", str(contracts), "--compatible", "--core", "--json"]) == 1
    report = json.loads(capsys.readouterr().out)
    assert report["checks"][0]["core"]["core"]["assumptions"] == ["G x", "F !x"]


def test_install_refusals(monkeypatch):
    from chase_cps import tools
    monkeypatch.setattr(tools, "this_platform", lambda: ("Linux", "aarch64"))
    with pytest.raises(tools.InstallError, match="no nuxmv build for Linux aarch64"):
        tools.install("nuxmv", accept_license=True)
    monkeypatch.setattr(tools, "this_platform", lambda: ("Darwin", "arm64"))
    with pytest.raises(tools.InstallError, match="install nuXmv instead"):
        tools.install("nusmv")
    with pytest.raises(tools.InstallError, match="--accept-license"):
        tools.install("nuxmv")


def test_install_downloads_checks_and_links(tmp_path, monkeypatch):
    import hashlib
    import tarfile
    from chase_cps import tools
    exe = tmp_path / "NuSMV-x" / "bin" / "NuSMV"
    exe.parent.mkdir(parents=True)
    exe.write_text("#!/bin/sh\n")
    exe.chmod(0o755)
    archive = tmp_path / "nusmv.tar.gz"
    with tarfile.open(archive, "w:gz") as tar:
        tar.add(tmp_path / "NuSMV-x", arcname="NuSMV-x")
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    monkeypatch.setenv("CHASE_HOME", str(tmp_path / "home"))
    monkeypatch.setattr(tools, "this_platform", lambda: ("Linux", "x86_64"))
    monkeypatch.setattr(tools, "_verify", lambda tool, path, log: path)
    monkeypatch.setitem(tools.NUSMV, ("Linux", "x86_64"),
                        (archive.as_uri(), digest, "NuSMV-x/bin/NuSMV"))
    path = tools.install("nusmv", log=lambda *a: None)
    assert path == str(tmp_path / "home" / "bin" / "NuSMV")
    assert os.path.realpath(path) == str(tmp_path / "home" / "tools" / "nusmv" / "NuSMV-x" / "bin" / "NuSMV")
    assert tools.find("NuSMV")
    monkeypatch.setitem(tools.NUSMV, ("Linux", "x86_64"),
                        (archive.as_uri(), "0" * 64, "NuSMV-x/bin/NuSMV"))
    with pytest.raises(tools.InstallError, match="SHA-256"):
        tools.install("nusmv", log=lambda *a: None)


@needs_native
@needs_mc
def test_simplify_gives_an_equivalent_contract():
    from chase_cps import refines
    c = Contract.from_strings(inputs=["a"], outputs=["b", "d"], assumptions=["G a", "G !(!a)"],
                              guarantees=["!(!b | X d)", "F b | F d"], name="C")
    s = c.simplify()
    assert [ltl.to_str(f) for f in s.guarantees] == ["b", "X !d", "F (b | d)"]
    assert s.inputs == c.inputs and s.outputs == c.outputs
    assert refines(c, s).status == "valid" and refines(s, c).status == "valid"


@needs_mc
def test_verify_is_compatibility_and_consistency():
    from chase_cps import verify
    ok = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G F a"],
                               guarantees=["G (a -> b)"], name="Ok")
    r = verify(ok)
    assert r and r.status == "sat"
    assert r.compatible.status == "sat" and r.consistent.status == "sat"
    assert r.feasible.status == "sat" and r.compatible.trace is r.feasible.trace
    bad = Contract.from_strings(inputs=["a"], assumptions=["G a", "F !a"], name="Bad")
    r = verify(bad)
    assert not r and r.status == "unsat" and r.compatible.status == "unsat"


@needs_mc
def test_verify_checks_that_the_assumptions_and_the_guarantees_hold_together():
    from chase_cps import verify
    # Compatible (A holds when grant is false) and consistent (A -> G holds
    # when A fails), but granting forever stops the requests that A needs.
    c = Contract.from_strings(inputs=["req"], outputs=["grant"], name="C",
                              assumptions=["G (grant -> X !req)", "G F req"],
                              guarantees=["G grant"])
    r = verify(c, core=True)
    assert r.status == "unsat" and not r
    assert r.compatible.status == "sat" and r.consistent.status == "sat"
    k = r.feasible.core.core
    assert [ltl.to_str(t) for t in k.assumptions] == ["G (grant -> X !req)", "G F req"]
    assert [ltl.to_str(t) for t in k.guarantees] == ["G grant"]
    d = r.to_dict()
    assert d["status"] == "unsat" and d["feasible"]["core"]["status"] == "found"
    # the command line reports it too
    from chase_cps.cli import main
    import json, tempfile, os
    with tempfile.TemporaryDirectory() as tmp:
        path = os.path.join(tmp, "c.json")
        with open(path, "w") as f:
            json.dump(c.to_dict(), f)
        assert main(["check", path]) == 1


@needs_native
def test_spec_with_constants_and_propositions():
    from chase_cps import parse_spec
    c = parse_spec("""NAME: k;

CONTRACT K:
    input integer (0:9) variable x;
    integer constant N 5;
    proposition p is x > 3;
    output boolean variable y;
    Assumptions:
        x < N;
    Guarantees:
        G(p -> y);
""")["K"]
    assert c.inputs == {"x": (0, 9)} and c.outputs == {"y": "bool"}
    assert [ltl.to_str(f) for f in c.assumptions] == ["x < 5"]
    assert [ltl.to_str(f) for f in c.guarantees] == ["G (x > 3 -> y)"]


def test_nusmv_does_not_check_real_variables(monkeypatch):
    from chase_cps.backends.nusmv import NuSMVAdapter
    from chase_cps.queries import Implies
    a = NuSMVAdapter(tool="nusmv")
    monkeypatch.setattr(a, "binary", lambda: ("NuSMV", "nusmv"))
    q = Implies(ltl.parse("G (v > 1)"), ltl.parse("G (v > 0)"), {"v": "real"})
    r = a.run(q)
    assert r.status == "unknown" and "nuXmv" in r.reason
    q = Implies(ltl.parse("G (n <= 2.5)"), ltl.parse("G (n <= 2)"), {"n": (0, 4)})
    assert "nuXmv" in a.run(q).reason
    model = NuSMVAdapter().emit(q)
    assert "chase_real : real;" in model and "n : 0..4;" in model


# Context -----------------------------------------------------------------------

# Finite traces encoded with alive: true for the length of the trace, then
# false for ever.
FINITE = "alive & (alive U G !alive)"


@needs_mc
def test_context_leaves_out_the_behaviors_that_violate_it():
    from chase_cps import verify
    c = Contract.from_strings(inputs=["alive"], outputs=["a"], name="C",
                              guarantees=["G (alive -> a)", "G (alive -> !a)"])
    # Without the context, the words where alive is false from the start
    # satisfy both guarantees.
    assert verify(c).status == "sat"
    r = verify(c, context=FINITE)
    assert r.status == "unsat" and r.compatible.status == "sat"
    # As an assumption, the axiom makes consistency (A -> G) hold, since
    # A -> G holds when A fails; verify checks A & G, which fails.
    from chase_cps import is_consistent
    assert is_consistent(c.assume(FINITE)).status == "sat"
    assert verify(c.assume(FINITE)).status == "unsat"


@needs_mc
def test_context_in_refinement_implication_and_redundancy():
    from chase_cps import implies, refines, remove_redundant
    impl = Contract.from_strings(inputs=["a"], outputs=["b"], name="Impl", guarantees=["G a"])
    spec = Contract.from_strings(inputs=["a"], outputs=["b"], name="Spec", guarantees=["G b"])
    law = "G (a -> b)"
    assert refines(impl, spec).status == "invalid"
    assert refines(impl, spec, context=law).status == "valid"
    assert implies("G a", "G b", {"a": "bool", "b": "bool"}, context=[law]).status == "valid"
    both = Contract.from_strings(inputs=["a"], outputs=["b"], name="Both",
                                 guarantees=["G a", "G b"])
    r = remove_redundant(both, context=law)
    assert r.contract.guarantees == [ltl.parse("G a")]


@needs_mc
def test_context_in_cores():
    from chase_cps import is_consistent, unsat_core
    variables = {"a": "bool", "b": "bool", "c": "bool"}
    assert unsat_core(["F a", "G b", "G c"], variables).status == "none"
    core = unsat_core(["F a", "G b", "G c"], variables, context="G (a -> !b)")
    assert core.status == "found" and core.core == [ltl.parse("F a"), ltl.parse("G b")]
    c = Contract.from_strings(outputs=["a", "b", "c"], name="C",
                              guarantees=["F a", "G b", "G c"])
    r = is_consistent(c, core=True, context="G (a -> !b)")
    assert r.status == "unsat" and r.core.core.guarantees == [ltl.parse("F a"),
                                                               ltl.parse("G b")]


def test_context_errors():
    from chase_cps import is_realizable, unrealizable_core, verify
    c = Contract.from_strings(inputs=["a"], outputs=["b"], name="C", guarantees=["G b"])
    with pytest.raises(ValueError, match="undeclared variables \\['x'\\] in the context"):
        verify(c, context="G x")
    with pytest.raises(TypeError, match="is_realizable does not take a context"):
        is_realizable(c, context="G a")
    with pytest.raises(TypeError, match="unrealizable_core does not take a context"):
        unrealizable_core(c, context="G a")


@needs_native
def test_comparison_outside_the_range_is_reported():
    from chase_cps import parse_spec
    text = """NAME: ranges;

CONTRACT Credits:
    input integer variable credits;
    input integer (0:100) variable score;
    output boolean variable ok;
    Guarantees:
        G((credits >= 90) -> ok);
        G((score > 50) -> ok);
"""
    with pytest.warns(UserWarning, match="credits is compared with 90, outside its range "
                                         "0..16 \\(the range of an integer declared"):
        parse_spec(text)
    import warnings
    with warnings.catch_warnings():
        warnings.simplefilter("error")
        parse_spec(text.replace("integer variable credits", "integer (0:200) variable credits"))


# Layers --------------------------------------------------------------------------

def test_layers_and_the_names_before_0_3_1():
    # The checks on contracts (chase_cps.checks), the queries on formulas
    # (chase_cps.logic), and GR(1) synthesis (chase_cps.gr1); the names
    # imported from chase_cps, chase_cps.checks, and chase_cps.cores before
    # 0.3.1 are the same objects.
    import chase_cps
    from chase_cps import checks, cores, gr1, logic
    for name in ("refines", "verify", "is_compatible", "is_consistent", "is_implementation",
                 "is_environment", "remove_redundant"):
        assert getattr(chase_cps, name) is getattr(checks, name)
    for name in ("implies", "is_satisfiable", "unsat_core", "Core"):
        assert getattr(chase_cps, name) is getattr(logic, name)
    for name in ("is_realizable", "synthesize", "unrealizable_core", "is_well_separated",
                 "is_vacuous"):
        assert getattr(chase_cps, name) is getattr(gr1, name)
    for name in ("implies", "is_satisfiable", "is_realizable", "synthesize",
                 "is_well_separated"):
        assert getattr(checks, name) is getattr(chase_cps, name)
    for name in ("unsat_core", "unrealizable_core", "gr1_kind", "gr1_roles", "Core"):
        assert getattr(cores, name) is getattr(logic, name, None) or \
            getattr(cores, name) is getattr(gr1, name)


def _guarantees_of(*terms):
    from chase_cps import parse_spec
    body = "\n".join(f"        {t};" for t in terms)
    spec = ("NAME: t;\nCONTRACT C:\n    input boolean variable x;\n    input boolean variable z;\n"
            f"    output boolean variable y;\n    Assumptions:\n        true;\n    Guarantees:\n{body}\n")
    return [ltl.to_str(g) for g in parse_spec(spec)["C"].guarantees]


@needs_native
def test_bounded_temporal_operators_unfold_into_next():
    assert _guarantees_of("G[0,2] (x -> y)") == ["x -> y", "X (x -> y)", "X X (x -> y)"]
    assert _guarantees_of("G(x -> F]1,2] y)") == ["G (x -> X X y)"]     # ]1: step 1 left out
    assert _guarantees_of("(x U[0,2] y)") == ["y | x & X (y | x & X y)"]
    assert _guarantees_of("(x U[1,2] y)") == ["x", "X (y | x & X y)"]
    assert _guarantees_of("F[2,2[ y") == ["false"]                       # no step
    for bad, why in [("X[0,2] y", "X with an interval"), ("G[0,2.5] y", "whole numbers"),
                     ("G[0,5000] y", "more than 1000 steps")]:
        with pytest.raises(ValueError, match=why):
            _guarantees_of(bad)


@needs_native
@needs_mc
def test_bounded_until_and_release_mean_what_they_say():
    from chase_cps import implies
    v = {"x": "bool", "y": "bool", "z": "bool"}
    u = ltl.parse(_guarantees_of("(x U[0,2] y)")[0])
    assert implies(ltl.parse("x & X x & X X y"), u, v).status == "valid"
    assert implies(u, ltl.parse("y | X y | X X y"), v).status == "valid"
    assert implies(ltl.parse("!y & X !y & X X !y"), ltl.Unary("!", u), v).status == "valid"
    r = ltl.parse(_guarantees_of("(x R[0,1] y)")[0])
    assert implies(r, ltl.parse("y & (x | X y)"), v).status == "valid"
    assert implies(ltl.parse("y & (x | X y)"), r, v).status == "valid"


@needs_native
def test_release_in_a_specification():
    # R crashed the parser of logics_tool: run it apart, so that a crash fails this test only
    import subprocess
    import sys
    code = ("import sys; sys.path.insert(0, %r); import test_chase_cps as t; "
            "print(t._guarantees_of('(x R y)', 'G (x R[0,1] y)'))" % os.path.dirname(__file__))
    p = subprocess.run([sys.executable, "-c", code], capture_output=True, text=True, timeout=120)
    assert p.returncode == 0, f"the parser crashed on R (exit {p.returncode})"
    assert p.stdout.strip().splitlines()[-1] == "['x R y', 'G !(!y | !x & X !y)']"


@needs_native
def test_a_problem_in_the_domain_specific_language():
    from chase_cps import load_dsl
    path = os.path.join(os.environ.get("CHASE_DEMOS", os.path.join(os.path.dirname(__file__), "..", "..", "demos")),
                        "DATE18", "template_0_realizable.txt")
    if not os.path.isfile(path):
        pytest.skip("the demos are not here")
    c = load_dsl(path)             # the architecture graph is left out, the formulas stay
    assert len(c.inputs) == 11 and len(c.outputs) == 7
    assert len(c.assumptions) == 24 and len(c.guarantees) == 26
    assert c.name == "template_0_realizable" and load_dsl(path, name="EPS").name == "EPS"


@needs_native
@pytest.mark.parametrize("formula", [
    "G a -> b", "a | b & c", "X a | b", "a & b U c", "a -> b -> c", "!a U b",
    "a U b U c", "F a <-> G b | c", "n + 2 * m > 3", "n - m - 1 = 0"])
def test_specification_files_read_formulas_as_python(formula):
    # the logics language has the precedence of LTL tools, as the Python syntax
    from chase_cps import ltl, parse_spec
    text = ("NAME: t;\ninput boolean variable a;\ninput boolean variable b;\n"
            "output boolean variable c;\ninput integer variable n;\ninput integer variable m;\n"
            f"CONTRACT C:\nAssumptions:\ntrue;\nGuarantees:\n{formula};\n")
    got = parse_spec(text)["C"].guarantee
    assert ltl.to_str(got) == ltl.to_str(ltl.conjunction(ltl.conjuncts(ltl.parse(formula))))
