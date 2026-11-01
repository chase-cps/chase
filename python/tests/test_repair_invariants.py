"""Repair of constants inside invariants (G of one step) by copies of the
state, checked against the check with G (nuXmv) and against the values
computed by hand; and the repair of a state machine with constants."""

import random
from fractions import Fraction

import pytest

from chase_cps import Contract, ltl, refines, repair
from chase_cps.backends import ADAPTERS
from chase_cps.contract import substitute
from chase_cps.queries import Holds, Implies
from chase_cps.repair import copies
from chase_cps.repair.problem import Parameter, parameters_of, questions, symbolic_args

HAS_Z3 = ADAPTERS["z3"].available()
needs_nuxmv = pytest.mark.skipif(not (HAS_Z3 and ADAPTERS["nuxmv"].available()),
                                 reason="z3 and nuXmv are needed")

# The robot of the skill tests: it stops within 0.5 v + 0.3, at speeds up to
# 2; the safety concept wants it within dmax at speeds up to vmax. The
# least change is vmax 1.6 -> 1.5 and dmax 1.0 -> 1.05 (cost 0.15): 0.5 vmax
# + 0.3 <= dmax, with dmax at most 1.05.
STOP = Contract.from_strings(name="Stop", inputs={"v": "real"}, outputs={"d": "real"},
                             assumptions=["G (v >= 0 & v <= 2)"],
                             guarantees=["G (d <= 0.5 * v + 0.3)"])
CONCEPT = Contract.from_strings(name="Concept", inputs={"v": "real"}, outputs={"d": "real"},
                                constants={"vmax": 1.6, "dmax": 1.0},
                                assumptions=["G (v >= 0 & v <= vmax)"],
                                guarantees=["G (d <= dmax)"])
RANGES = {"vmax": (1.2, 2.0), "dmax": (0.8, 1.05)}
MACHINE = """MODULE main
VAR v : real; d : real;
INIT d = 0.5 * v + 0.3
TRANS next(d) = 0.5 * next(v) + 0.3
"""


def _z3_valid(q):
    from chase_cps.backends import z3 as backend_z3
    z3 = backend_z3._z3
    e = backend_z3.Encoding(q.variables)
    s = z3.Solver()
    s.add(e.formula(q.phi), z3.Not(e.formula(q.psi)), *e.bounds())
    return s.check() == z3.unsat


# copies ----------------------------------------------------------------------

def test_the_forms_that_go_on_copies():
    q = lambda s: Implies(ltl.parse(s), ltl.parse("true"), {"x": "real"})
    assert copies.invariants(q("G (x <= 1) & F (x > 0) & x = 0")) is not None
    assert copies.invariants(q("G (x <= 1 -> X x > 0)")) is None
    assert copies.invariants(q("G F (x > 0)")) is None
    assert copies.invariants(q("(x > 0) U (x < 0)")) is None


@needs_nuxmv
def test_without_g_the_answer_can_be_wrong_and_the_copies_keep_it():
    # (G x <= 0 -> G false) -> G x > 0: without G it reads (x > 0) -> (x > 0),
    # valid; with G it fails on x = 1, 0, 0, ... (G x <= 0 false, G x > 0 false)
    low = Contract.from_strings(name="L", inputs={"x": "real"}, outputs={},
                                assumptions=["G (x <= 0)"], guarantees=["G false"])
    high = Contract.from_strings(name="H", inputs={"x": "real"}, outputs={},
                                 guarantees=["G (x > 0)"])
    assert refines(low, high).status == "invalid"
    asked = questions("refines", (low, high))
    on = dict(copies.on_copies(asked, {}))
    assert not _z3_valid(on["guarantees"])
    drop = lambda f: f.arg if isinstance(f, ltl.Unary) and f.op == "G" else (
        ltl.Unary(f.op, drop(f.arg)) if isinstance(f, ltl.Unary) else
        ltl.Binary(f.op, drop(f.left), drop(f.right)) if isinstance(f, ltl.Binary) else f)
    g = dict(asked)["guarantees"]
    assert _z3_valid(Implies(drop(g.phi), drop(g.psi), g.variables))


@needs_nuxmv
def test_copies_repair_two_reals_inside_g_at_the_least_cost():
    r = repair("refines", STOP, CONCEPT, parameters=RANGES)
    assert r.status == "repaired" and r.method == "copies" and r.optimal
    assert r.values == {"vmax": Fraction(3, 2), "dmax": Fraction(21, 20)}
    assert r.cost == Fraction(3, 20)
    assert repair("refines", STOP, CONCEPT, parameters={"dmax": (0.8, 1.05)}).status == "impossible"


@needs_nuxmv
def test_copies_and_the_check_with_g_agree_on_random_invariants():
    # random contracts of invariants and one constant c: at random values of
    # c, the questions on the copies (z3) have the answers of the check (nuXmv)
    rnd = random.Random(7)
    names = ["x", "y"]

    def cond():
        a, b = rnd.sample(names + ["c"], 2)
        rel = rnd.choice(["<=", "<", ">=", ">"])
        k = rnd.choice(["", " + 1", " - 2", " * 2"])
        return f"{a}{k} {rel} {b}" if rnd.random() < 0.7 else f"{a} {rel} {rnd.randint(-3, 3)}"

    def term():
        f = cond() if rnd.random() < 0.6 else f"({cond()} | {cond()})"
        return rnd.choice([f"G ({f})", f"G ({f})", f"F ({f})", f])
    for _ in range(40):
        c1 = Contract.from_strings(name="L", inputs={"x": "real"}, outputs={"y": "real"},
                                   constants={"c": 0},
                                   assumptions=[term() for _ in range(rnd.randint(0, 2))],
                                   guarantees=[term() for _ in range(rnd.randint(1, 2))])
        c2 = Contract.from_strings(name="H", inputs={"x": "real"}, outputs={"y": "real"},
                                   constants={"c": 0},
                                   assumptions=[term() for _ in range(rnd.randint(0, 2))],
                                   guarantees=[term() for _ in range(rnd.randint(1, 2))])
        if "c" not in c1.constants and "c" not in c2.constants:
            continue
        params = parameters_of("refines", (c1, c2), {"c": (-3, 3)})
        asked = questions("refines", symbolic_args("refines", (c1, c2), params))
        on = copies.on_copies(asked, params)
        for value in (Fraction(rnd.randint(-6, 6), 2),):
            fixed = [Implies(substitute(q.phi, {"c": value}), substitute(q.psi, {"c": value}),
                             {k: t for k, t in q.variables.items() if k != "c"})
                     for _, q in on]
            mine = all(_z3_valid(q) for q in fixed)
            at = lambda k: k.instantiate({"c": value}) if "c" in k.constants else k
            theirs = refines(at(c1), at(c2))
            assert theirs.status in ("valid", "invalid")
            assert mine == (theirs.status == "valid"), (c1.to_dict(), c2.to_dict(), value)


@needs_nuxmv
def test_copies_repair_passes_the_check_on_random_invariants():
    rnd = random.Random(11)
    for _ in range(8):
        k1, k2 = rnd.randint(1, 4), rnd.randint(0, 3)
        low = Contract.from_strings(name="L", inputs={"x": "real"}, outputs={"y": "real"},
                                    assumptions=[f"G (x >= 0 & x <= {k1 + 2})"],
                                    guarantees=[f"G (y <= {k1} * x + {k2})"])
        high = Contract.from_strings(name="H", inputs={"x": "real"}, outputs={"y": "real"},
                                     constants={"a": 1, "b": 0},
                                     assumptions=["G (x >= 0 & x <= a)"],
                                     guarantees=["G (y <= b)"])
        r = repair("refines", low, high, parameters={"a": (0, 10), "b": (0, 30)})
        assert r.status in ("repaired", "holds") and r.method == "copies" and r.optimal
        assert refines(low, high.instantiate(r.values)).status == "valid"
        # the least change: b = k1 a + k2 with a as large as is cheaper (the
        # change of b per unit of a is k1 >= 1, so a goes down to where b is
        # cheapest): with a in [0, 1], the cost is (1 - a) + |k1 a + k2|
        best = min(((1 - Fraction(a, 100)) + Fraction(k1 * a, 100) + k2
                    for a in range(0, 101)))
        assert r.cost == min(best, Fraction(k1 + k2))


@needs_nuxmv
def test_copies_repair_a_check_of_satisfiability():
    # compatible: some environment keeps x <= a forever and reaches 2 once
    c = Contract.from_strings(name="C", inputs={"x": "real"}, outputs={"y": "real"},
                              constants={"a": 1}, assumptions=["G (x <= a)", "F (x >= 2)"],
                              guarantees=["G (y = x)"])
    r = repair("compat", c, parameters={"a": (0, 5)})
    assert r.status == "repaired" and r.method == "copies" and r.optimal
    assert r.values == {"a": 2} and r.cost == 1


# a state machine ---------------------------------------------------------------

@needs_nuxmv
def test_a_state_machine_against_a_contract_with_constants():
    # the constants are declared in the model (FROZENVAR) for the questions of
    # the repair; they failed with "the model does not declare"
    r = repair("impl", MACHINE, CONCEPT, parameters={"dmax": (0.8, 1.2)}, time_limit=120)
    assert r.status == "repaired", r.reason
    assert r.values == {"dmax": Fraction(11, 10)}        # 0.5 * 1.6 + 0.3


# the edges of copies -------------------------------------------------------------

def _c(name, inputs, outputs, assumptions, guarantees, constants):
    return Contract.from_strings(name=name, inputs=inputs, outputs=outputs, constants=constants,
                                 assumptions=assumptions, guarantees=guarantees)


@needs_nuxmv
def test_copies_read_the_context():
    # without the context x <= 1, no c passes; with it, c = 1
    low = _c("L", {"x": "real"}, {"y": "real"}, [], ["G (y <= x)"], {})
    high = _c("H", {"x": "real"}, {"y": "real"}, [], ["G (y <= c)"], {"c": 0})
    r = repair("refines", low, high, parameters={"c": (0, 10)}, context="G (x <= 1)")
    assert r.status == "repaired" and r.method == "copies" and r.values == {"c": 1}


@needs_nuxmv
def test_copies_of_a_variable_do_not_take_the_name_of_a_constant():
    # a constant named x__c1, as the first copy of x would be: compat holds at 0
    c = _c("A", {"x": "real"}, {"y": "real"}, ["G (x < x__c1)"], ["G (y = x)"], {"x__c1": 0})
    r = repair("compat", c, parameters={"x__c1": (0, 10)})
    assert r.status == "holds", r.reason


@needs_nuxmv
def test_a_term_that_z3_does_not_read_is_unknown_with_the_reason():
    low = _c("L", {"n": (0, 7)}, {}, [], ["G (n mod 2 = 0)"], {})
    high = _c("H", {"n": (0, 7)}, {}, [], ["G (n <= c)"], {"c": 0})
    r = repair("refines", low, high, parameters={"c": (0, 10)})
    assert r.status == "unknown" and "mod" in r.reason


@needs_nuxmv
def test_an_integer_variable_and_a_real_constant():
    # each counterexample removes one step of n: the projection of the copies
    # gives the least c at once
    low = _c("L", {"n": (0, 100)}, {}, [], ["G true"], {})
    high = _c("H", {"n": (0, 100)}, {}, [], ["G (n <= c)"], {"c": 0})
    r = repair("refines", low, high, parameters={"c": (0, 200)})
    assert r.status == "repaired" and r.values == {"c": 100} and r.optimal


@needs_nuxmv
def test_copies_with_no_answer_go_on_with_the_other_methods():
    # z3 gives no best value with integers and reals; the present values pass
    c = _c("K", {"n": (-2, 3)}, {}, [], ["G (n + 1 > c2)", "G (c3 + 1 >= c1 | c2 - 1 < 1)"],
           {"c1": 0, "c2": 0, "c3": 0})
    r = repair("consist", c, parameters={"c1": (-2, 3), "c2": (-2, 3), "c3": (-2, 3)})
    assert r.status == "holds" and "copies" in r.reason


@needs_nuxmv
def test_a_check_with_no_answer_is_said_so(monkeypatch):
    import importlib
    from chase_cps.repair.problem import Outcome
    module = importlib.import_module("chase_cps.repair")
    monkeypatch.setattr(module, "run", lambda *a, **k: Outcome(None, None, [], "timeout"))
    r = repair("refines", STOP, CONCEPT, parameters=RANGES, method="copies")
    assert r.status == "unknown" and "gave no answer" in r.reason and "timeout" in r.reason


@needs_nuxmv
def test_the_context_of_a_state_machine_after_its_reset():
    # the check reads the context after the steps of a reset, and so does the
    # repair: u in the reset step is free, so y reaches 5 at step 1, and the
    # least c is 5 (with the context from step 0, the repair saw c = 1)
    import dataclasses
    from chase_cps import Model, is_implementation
    m = dataclasses.replace(Model.from_smv("""MODULE main
IVAR u : 0..5;
VAR y : 0..9;
ASSIGN init(y) := 9; next(y) := u;
"""), start=1)
    c = Contract.from_strings(name="Low", inputs={"u": (0, 5)}, outputs={"y": (0, 9)},
                              constants={"c": 0}, guarantees=["G (y <= c)"])
    r = repair("impl", m, c, parameters={"c": (0, 9, "int")}, context="G (u <= 1)", time_limit=120)
    assert r.status == "repaired" and r.values == {"c": 5}, (r.status, r.values, r.reason)
    assert is_implementation(m, c.instantiate({"c": 5}), context="G (u <= 1)").status == "valid"
    assert is_implementation(m, c.instantiate({"c": 4}), context="G (u <= 1)").status == "invalid"
