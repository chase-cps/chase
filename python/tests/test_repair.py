"""Repair (chase_cps.repair) and the named constants of contracts: the
values found are checked against other ways of finding them (the other
method, every value of an integer parameter, another back end), and on the
braking system of ContrACT (MEMOCODE 2026) against the paper."""

import itertools
import json
import os
import random
from fractions import Fraction

import pytest

from chase_cps import CFunction, Contract, is_implementation, parse_spec, refines, repair, to_spec
from chase_cps.backends import ADAPTERS
from chase_cps.repair import RepairError
from chase_cps.repair.instantiate import on_trace
from chase_cps.queries import Trace
from chase_cps.robustness import holds

HAS_Z3 = ADAPTERS["z3"].available()
needs_z3 = pytest.mark.skipif(not HAS_Z3, reason="z3 not installed")
needs_nuxmv = pytest.mark.skipif(not (HAS_Z3 and ADAPTERS["nuxmv"].available()),
                                 reason="z3 and nuXmv are needed")
needs_esbmc = pytest.mark.skipif(not (HAS_Z3 and ADAPTERS["esbmc"].available()),
                                 reason="z3 and ESBMC are needed")
HERE = os.path.dirname(__file__)


# named constants -------------------------------------------------------------

SPEC = Contract.from_strings(name="Spec", inputs={"x": (0, 9)}, outputs={"y": (0, 20)},
                             guarantees=["y <= x + lam", "y >= 0"], constants={"lam": 4})
IMPL = Contract.from_strings(name="Impl", inputs={"x": (0, 9)}, outputs={"y": (0, 20)},
                             guarantees=["y = x + 5"])


def test_constants_keep_their_names():
    assert [str(t) for t in SPEC.guarantees][0] != [str(t) for t in SPEC.terms[1]][0]
    assert SPEC.to_dict()["guarantees"][0] == "y <= x + lam"
    assert SPEC.to_dict()["constants"] == {"lam": 4}
    assert Contract.from_dict(SPEC.to_dict()).to_dict() == SPEC.to_dict()
    other = SPEC.instantiate({"lam": 6})
    assert other.constants == {"lam": 6} and SPEC.constants == {"lam": 4}
    with pytest.raises(ValueError, match="not constants"):
        SPEC.instantiate({"mu": 1})
    with pytest.raises(ValueError, match="both constants and variables"):
        Contract.from_strings(inputs={"x": (0, 9)}, guarantees=["x <= x"], constants={"x": 1})


def test_constants_in_specification_files():
    pytest.importorskip("chase_cps._native")
    text = to_spec([SPEC.instantiate({"lam": Fraction(9, 2)})], name="t")
    assert "real constant lam 4.5;" in text
    back = parse_spec(text)["Spec"]
    assert back.constants == {"lam": Fraction(9, 2)}
    assert back.to_dict()["guarantees"][0] == "y <= x + lam"


@pytest.mark.skipif(not (HAS_Z3 or ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()),
                    reason="no back end for the refinements (z3, nuXmv, or NuSMV)")
def test_constants_pass_through_composition():
    pytest.importorskip("chase_cps._native")
    k = Contract.from_strings(name="K", inputs={"y": (0, 20)}, outputs={"z": (0, 30)},
                              guarantees=["z <= y + k"], constants={"k": 2})
    both = SPEC.compose(k)
    assert both.constants == {"lam": 4, "k": 2}
    assert refines(both.instantiate({"k": 2, "lam": 4}), both).status == "valid"
    assert refines(both, both.instantiate({"k": 3})).status == "valid"
    assert refines(both.instantiate({"k": 3}), both).status == "invalid"


# repairs that can be checked by hand ------------------------------------------

@needs_z3
def test_a_bound_by_each_method():
    pytest.importorskip("chase_cps._native")
    for method, kind in (("projection", ()), ("counterexamples", ()), ("auto", ("int",))):
        r = repair("refines", IMPL, SPEC, parameters={"lam": (0, 10, *kind)}, method=method)
        assert r.status == "repaired" and r.values == {"lam": 5} and r.optimal, method
        assert r.cost == 1 and r.check.status == "valid"
    r = repair("refines", IMPL, SPEC, parameters={"lam": (0, 3)})
    assert r.status == "impossible"
    r = repair("refines", IMPL, SPEC, parameters={"lam": (0, 3, "int")})
    assert r.status == "impossible" and r.counterexamples
    with pytest.raises(RepairError, match="not a constant"):
        repair("refines", IMPL, SPEC, parameters={"mu": (0, 3)})


@needs_z3
def test_maximize_and_satisfiability():
    pytest.importorskip("chase_cps._native")
    # the largest lam for which the contract can still be met (A & G satisfiable)
    tight = Contract.from_strings(name="T", inputs={"x": (0, 9)}, outputs={"y": (0, 20)},
                                  guarantees=["y >= x + 15", "y <= 20 - lam"],
                                  constants={"lam": 1})
    r = repair("verify", tight, parameters={"lam": (0, 10)}, maximize="lam")
    assert r.status == "repaired" and r.values == {"lam": 5} and r.cost == 5
    assert r.method == "witnesses"
    r = repair("verify", tight, parameters={"lam": (0, 10, "int")}, maximize="lam",
               method="enumeration")
    assert r.status == "repaired" and r.values == {"lam": 5} and r.method == "enumeration"


def test_a_counterexample_becomes_a_condition():
    t = Trace(prefix=[{"x": 1, "y": 7}], loop=[{"x": 3, "y": 9}])
    c = on_trace(on_trace.__globals__["ltl"].parse("G (y <= x + lam)"), t, {"lam"})
    for lam in range(0, 9):
        assert holds(f"G (y <= x + {lam})", t) == holds(
            on_trace.__globals__["ltl"].to_str(c).replace("lam", str(lam)), Trace([], [{}]))


# the braking system (ContrACT, MEMOCODE 2026) ----------------------------------

def _aeb():
    from chase_cps import load_spec
    return load_spec(os.path.join(HERE, "data", "braking.ltl"))


@needs_z3
def test_the_braking_system_as_in_the_paper():
    pytest.importorskip("chase_cps._native")
    c = _aeb()
    pedal = c["BrakePedal"].compose(c["PedalProcessing"]).compose(c["Brake1"])
    camera = c["Camera"].compose(c["Detection"]).compose(c["Link"]).compose(c["Brake2"])
    # Brake 1 waits up to 3.05 on its path plus 1 from the camera path: 4.05 > 4
    assert refines(pedal, c["SysB"]).status == "invalid"
    r = repair("refines", pedal, c["SysB"], parameters={"lam_b": (0, 10)})
    assert r.status == "repaired" and r.values == {"lam_b": Fraction(81, 20)} and r.optimal
    # the paper raises kappa until the camera path no longer delays Brake 1: 11
    r = repair("refines", camera, c["DelayF"].instantiate({"y_f": 0}),
               parameters={"kappa": (0, 20)})
    assert r.values == {"kappa": 11} and r.optimal
    # Brake 1 can absorb 4 - 3.05 = 0.95: kappa = 10.05 is enough, and the least
    r = repair("refines", camera, c["DelayF"], parameters={"kappa": (0, 20)})
    assert r.values == {"kappa": Fraction(201, 20)} and r.optimal
    assert refines(camera.instantiate({"kappa": Fraction(201, 20)}), c["DelayF"]).status == "valid"
    assert refines(camera.instantiate({"kappa": Fraction(1004, 100)}),
                   c["DelayF"]).status == "invalid"
    # the counterexamples find the same values
    r = repair("refines", camera, c["DelayF"], parameters={"kappa": (0, 20)},
               method="counterexamples")
    assert r.status == "repaired" and r.values == {"kappa": Fraction(201, 20)}


# other ways of finding the same values -----------------------------------------

def _problem(rng):
    """A component and a requirement over integers, with two constants."""
    a, b = rng.randint(1, 3), rng.randint(0, 6)
    impl = Contract.from_strings(name="I", inputs={"x": (0, 5)}, outputs={"y": (0, 30)},
                                 assumptions=["x <= 4"], guarantees=[f"y = {a} * x + {b}"])
    form = rng.choice(["y <= x + p & y >= q", "y <= p | x >= q", "y - x <= p & y >= q - x"])
    spec = Contract.from_strings(name="S", inputs={"x": (0, 5)}, outputs={"y": (0, 30)},
                                 assumptions=["x <= 4"], guarantees=[form],
                                 constants={"p": rng.randint(0, 6), "q": rng.randint(0, 6)})
    return impl, spec


@needs_nuxmv
def test_against_every_value_and_another_back_end():
    pytest.importorskip("chase_cps._native")
    rng = random.Random(3)
    for _ in range(40):
        impl, spec = _problem(rng)
        ps = {"p": (0, 12, "int"), "q": (0, 12, "int")}
        r = repair("refines", impl, spec, parameters=ps)
        best = None
        for p, q in itertools.product(range(13), range(13)):
            cost = abs(p - spec.constants["p"]) + abs(q - spec.constants["q"])
            if best is not None and cost >= best:
                continue
            if refines(impl, spec.instantiate({"p": p, "q": q})).status == "valid":
                best = cost
        if best is None:
            assert r.status == "impossible"
            continue
        assert r.status == ("holds" if best == 0 else "repaired") and r.optimal and r.cost == best
        # the values pass with nuXmv too
        assert refines(impl, spec.instantiate(r.values), backend="nuxmv").status == "valid"
        # and with real parameters, by projection, at the same cost
        real = repair("refines", impl, spec, parameters={"p": (0, 12), "q": (0, 12)})
        assert real.status in ("holds", "repaired") and real.cost <= best


@needs_z3
def test_projection_and_counterexamples_agree_over_the_reals():
    pytest.importorskip("chase_cps._native")
    rng = random.Random(8)
    for _ in range(30):
        a, b = Fraction(rng.randint(1, 8), 2), Fraction(rng.randint(0, 12), 2)
        impl = Contract.from_strings(name="I", inputs={"x": "real"}, outputs={"y": "real"},
                                     assumptions=["x >= 0 & x <= 4"],
                                     guarantees=[f"y <= {a} * x + {b}", f"y >= {a} * x"])
        form = rng.choice(["y <= x + p & y >= 2 * x - q - 1", "y <= p | x >= q",
                           "y - p <= 2 * x & x <= q + 3"])
        spec = Contract.from_strings(name="S", inputs={"x": "real"}, outputs={"y": "real"},
                                     assumptions=["x >= 0 & x <= 4"], guarantees=[form],
                                     constants={"p": 1, "q": 1})
        ps = {"p": (-10, 20), "q": (-10, 20)}
        one = repair("refines", impl, spec, parameters=ps)
        two = repair("refines", impl, spec, parameters=ps, method="counterexamples")
        assert one.method == "projection" and one.status == two.status, form
        if one.status == "repaired":
            assert one.cost == two.cost and one.optimal and two.optimal, form
            for r in (one, two):
                assert refines(impl, spec.instantiate(r.values)).status == "valid"


# LTL, with nuXmv: the least bound on the waiting time ----------------------------

ARBITER = """NAME: arbiter;

CONTRACT Client:
    input boolean variable grant;
    output boolean variable req;
    Assumptions:
        true;
    Guarantees:
        G ((req & !grant) -> X req);

CONTRACT Arbiter:
    input boolean variable req;
    output boolean variable grant;
    output integer (0:2) variable c;
    Assumptions:
        true;
    Guarantees:
        c = 0;
        G ((c < 2) -> (c' = c + 1));
        G ((c = 2) -> (c' = 0));
        G (grant <-> (c = 2));

CONTRACT Wait:
    input boolean variable req;
    input boolean variable grant;
    output integer (0:9) variable w;
    Assumptions:
        true;
    Guarantees:
        w = 0;
        G ((req & !grant) -> (w' = w + 1));
        G (!(req & !grant) -> (w' = 0));

CONTRACT Spec:
    input boolean variable req;
    input boolean variable grant;
    input integer (0:9) variable w;
    integer constant k 1;
    Assumptions:
        true;
    Guarantees:
        G (w <= k);
"""


@needs_nuxmv
def test_the_least_bound_of_a_temporal_requirement():
    pytest.importorskip("chase_cps._native")
    c = parse_spec(ARBITER)
    system = c["Client"].compose(c["Arbiter"]).compose(c["Wait"])
    assert refines(system, c["Spec"]).status == "invalid"
    r = repair("refines", system, c["Spec"], parameters={"k": (0, 9, "int")})
    assert r.status == "repaired" and r.values == {"k": 2} and r.optimal
    assert r.method == "counterexamples" and r.check.status == "valid"
    # every value: 2 is the least that passes
    passing = [k for k in range(10)
               if refines(system, c["Spec"].instantiate({"k": k})).status == "valid"]
    assert passing[0] == 2


# C, with ESBMC: the constant of the contract that the code implements ---------------

HEATER = """
/* heat (3 units) while the temperature is at most 70 */
int heater(int temp) {
    if (temp <= 70) return 3;
    return 0;
}
"""


@needs_esbmc
def test_the_constant_that_the_code_implements():
    ctrl = Contract.from_strings(name="Ctrl", inputs={"temp": (0, 120)}, outputs={"u": (0, 3)},
                                 guarantees=["temp <= theta -> u = 3", "temp > theta -> u = 0"],
                                 constants={"theta": 60})
    f = CFunction("heater.c", "heater", source=HEATER)
    assert is_implementation(f, ctrl).status == "invalid"
    r = repair("impl", f, ctrl, parameters={"theta": (0, 120, "int")})
    assert r.status == "repaired" and r.values == {"theta": 70} and r.optimal
    assert r.check.backend == "esbmc" and r.rounds >= 2


@needs_z3
def test_the_command_line(tmp_path, capfd):
    pytest.importorskip("chase_cps._native")
    from chase_cps.cli import main
    path = tmp_path / "spec.ltl"
    path.write_text(to_spec([IMPL, SPEC], name="s"))
    assert main(["repair", str(path), "--refines", "Impl", "Spec", "--param", "lam=0..10"]) == 0
    out = capfd.readouterr().out
    assert "repaired" in out and "lam = 5" in out and "(optimal)" in out
    assert main(["repair", str(path), "--refines", "Impl", "Spec", "--param", "lam=0..3:int",
                 "--json"]) == 1
    assert json.loads(capfd.readouterr().out)["status"] == "impossible"
    assert main(["repair", str(path), "--refines", "Impl", "Spec", "--param", "lam=0-3"]) == 2
    assert "NAME=LOW..HIGH" in capfd.readouterr().err
    assert main(["repair", str(path), "--param", "lam=0..3"]) == 2


def test_without_z3(monkeypatch):
    import chase_cps.backends.z3 as backend_z3
    monkeypatch.setattr(backend_z3, "_z3", None)
    r = repair("refines", IMPL, SPEC, parameters={"lam": (0, 10)})
    assert r.status == "unknown" and "pip install chase-cps[z3]" in r.reason



# repair as a verification: it holds exactly when the check passes ---------------

@needs_nuxmv
def test_repair_holds_exactly_when_the_check_passes():
    pytest.importorskip("chase_cps._native")
    from chase_cps import verify
    rng = random.Random(11)
    seen = set()
    # static contracts over integers and over reals
    for _ in range(40):
        impl, spec = _problem(rng)
        spec = spec.instantiate({"p": rng.randint(0, 12), "q": rng.randint(0, 12)})
        passes = refines(impl, spec).status == "valid"
        for kind in ((), ("int",)):
            r = repair("refines", impl, spec,
                       parameters={"p": (0, 12, *kind), "q": (0, 12, *kind)})
            assert (r.status == "holds") == passes, (spec.terms, r.status)
            if passes:
                assert r.cost == 0 and r.values == spec.constants
        seen.add(passes)
    # some behavior: verify
    for lam in range(0, 9):
        tight = Contract.from_strings(name="T", inputs={"x": (0, 9)}, outputs={"y": (0, 20)},
                                      guarantees=["y >= x + 15", "y <= 20 - lam"],
                                      constants={"lam": lam})
        passes = verify(tight).status == "sat"
        r = repair("verify", tight, parameters={"lam": (0, 10)})
        assert (r.status == "holds") == passes, lam
        seen.add(passes)
    # LTL, with nuXmv
    c = parse_spec(ARBITER)
    system = c["Client"].compose(c["Arbiter"]).compose(c["Wait"])
    for k in range(0, 5):
        spec = c["Spec"].instantiate({"k": k})
        passes = refines(system, spec).status == "valid"
        r = repair("refines", system, spec, parameters={"k": (0, 9, "int")})
        assert (r.status == "holds") == passes, k
        assert r.values == {"k": max(k, 2)}
    assert seen == {True, False}


@needs_esbmc
def test_repair_of_a_c_function_holds_exactly_when_it_implements():
    f = CFunction("heater.c", "heater", source=HEATER)
    for theta in (60, 69, 70, 71, 90):
        ctrl = Contract.from_strings(name="Ctrl", inputs={"temp": (0, 120)},
                                     outputs={"u": (0, 3)},
                                     guarantees=["temp <= theta -> u = 3", "temp > theta -> u = 0"],
                                     constants={"theta": theta})
        passes = is_implementation(f, ctrl).status == "valid"
        r = repair("impl", f, ctrl, parameters={"theta": (0, 120, "int")})
        assert (r.status == "holds") == passes == (theta == 70)
        assert r.values == {"theta": 70}


def test_a_time_limit_stops_with_a_bound(tmp_path):
    # a large timing family (python/benchmarks): stopped after a second, the
    # repair is unknown with the least cost proved so far; without the limit,
    # the least cost found is at least that bound
    pytest.importorskip("z3")
    import sys
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "benchmarks"))
    from repair_scaling import composed, requirement
    c, last = composed(48, 2)
    req = requirement(last, 0)
    params = {"lam": (0, 10000), "x1": (0, 100)}
    stopped = repair("refines", c, req, parameters=params, minimize="lam", time_limit=1)
    assert stopped.status == "unknown" and stopped.bound is not None
    assert stopped.seconds < 5
    assert stopped.to_dict()["bound"] == float(stopped.bound) or stopped.to_dict()["bound"] == stopped.bound
    full = repair("refines", c, req, parameters=params, minimize="lam", time_limit=120)
    assert full.status == "repaired" and full.cost >= stopped.bound


@pytest.mark.skipif(not (ADAPTERS["esbmc"].available() or ADAPTERS["cbmc"].available()),
                    reason="ESBMC/CBMC not installed")
def test_a_c_function_called_once_per_step(tmp_path):
    # a C function, called once per step, implements a contract over steps
    # when each call satisfies the formula of one step; a call that does not,
    # at every step, breaks the contract; a function with a state, or a
    # contract that is not a formula of one step at some steps, is not decided
    from chase_cps.cprograms import step_contract, stateless
    src = tmp_path / "t.c"
    src.write_text("int thermostat(int T) {\n    if (T < 74)\n        return 3;\n    return 0;\n}\n"
                   "int count = 0;\nint counted(int T) { count++; return T < 74 ? 3 : 0; }\n")
    spec = """NAME: t;
CONTRACT Thermostat:
    input integer (0:200) variable T;
    output integer (0:3) variable u;
    integer constant theta 76;
    Assumptions:
        true;
    Guarantees:
        G[0,9] ((T < theta -> u = 3) & (T >= theta -> u = 0));
CONTRACT Room:
    input integer (0:3) variable u;
    output integer (0:200) variable T;
    Assumptions:
        true;
    Guarantees:
        T = 60;
        G[0,9] (T' = T + u - 1);
"""
    c = parse_spec(spec)
    f = CFunction(str(src), "thermostat", result="u")
    step, why = step_contract(c["Thermostat"])
    assert why == "" and len(step.guarantees) == 2 and not step.assumptions
    assert is_implementation(f, c["Thermostat"].instantiate({"theta": 74})).status == "valid"
    r = is_implementation(f, c["Thermostat"])
    assert r.status == "invalid" and r.trace.prefix == [] and r.trace.loop[0]["T"] in (74, 75)
    assert repair("impl", f, c["Thermostat"], parameters={"theta": (0, 200, "int")}).values == {"theta": 74}
    room = is_implementation(f, c["Room"])
    assert room.status == "unknown" and "one step" in room.reason
    g = CFunction(str(src), "counted", result="u")
    assert "count" in stateless(g)
    assert is_implementation(g, c["Thermostat"]).status == "unknown"
    # a static variable of the file is a state too
    filed = src.parent / "filed.c"
    filed.write_text("static int cnt;\nint thermostat_state(int T) { cnt++; if (cnt > 3) return 1; return T < 74 ? 3 : 0; }\n")
    h = CFunction(str(filed), "thermostat_state", result="u")
    assert "cnt" in stateless(h)
    assert is_implementation(h, c["Thermostat"].instantiate({"theta": 74})).status == "unknown"


def test_the_cost_of_a_product_by_zero():
    # the cost is evaluated with the operation asked only: 3 * (1 - wmax) at
    # wmax = 1 is 0, not a division by 0
    from fractions import Fraction
    from chase_cps import ltl
    from chase_cps.contract import substitute
    from chase_cps.repair import _evaluate
    cost = substitute(ltl.parse("(eps - 2) + 3 * (1 - wmax)"), {"eps": Fraction(5, 2), "wmax": 1})
    assert _evaluate(cost) == Fraction(1, 2)
