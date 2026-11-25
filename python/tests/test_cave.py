"""Variable elimination for GR(1) contracts (chase_cps.cave) and composition
with ``eliminate=True``: the examples of Narter, Zhao, and Incer (arXiv
2610.37534), and the defining implications on random formulas, checked on
every lasso trace of a few steps."""

import itertools
import os
import random

import pytest

from chase_cps import Contract, ltl, refines
from chase_cps.backends import ADAPTERS
from chase_cps.queries import Trace
from chase_cps.robustness import holds

HAS_Z3 = ADAPTERS["z3"].available()
HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False

needs_z3 = pytest.mark.skipif(not HAS_Z3, reason="z3 not installed")
needs_all = pytest.mark.skipif(not (HAS_Z3 and HAS_MC and HAS_NATIVE),
                               reason="z3, nuXmv/NuSMV, and the CHASE modules are needed")
pytestmark = needs_z3

if HAS_Z3:
    import z3
    from chase_cps import cave
    from chase_cps.backends.z3 import Encoding

RUNS = int(os.environ.get("CHASE_PROPERTY_RUNS", "12"))
P = {"p": "bool", "q": "bool", "s": "bool"}


def terms(*formulas):
    return [ltl.parse(f) for f in formulas]


def buckets(ts, types):
    """The initial, invariant, and liveness cores as z3 formulas over steps."""
    init, inv, liv = cave.split(ts)
    e = Encoding(types)
    return (z3.And(*[e.formula(c) for c in init]), z3.And(*[e.formula(c) for c in inv]),
            [cave._shifted_down(e.formula(c)) for c in liv])


def equivalent(f, g) -> bool:
    s = z3.Solver()
    s.add(f != g)
    return s.check() == z3.unsat


def same_terms(xs, ys, types) -> bool:
    """The same initial term, the same invariant, and the same liveness terms."""
    (i1, v1, l1), (i2, v2, l2) = buckets(xs, types), buckets(ys, types)
    return (equivalent(i1, i2) and equivalent(v1, v2)
            and all(any(equivalent(a, b) for b in l2) for a in l1)
            and all(any(equivalent(a, b) for a in l1) for b in l2))


# The examples of the paper ---------------------------------------------------

# Section 4.2: X p is forced at each falling edge of q and allowed only after q.
CONTEXT = terms("G ((q & X !q -> X p) & (X p -> q))")
TARGET = terms("G (X p | s)", "G F X p")


def test_refinement_example():
    # forall p. (Gamma -> phi) = G((q & X !q) | s) & G F (q & X !q), exact
    psi = cave.refine(TARGET, CONTEXT, {"p"}, P)
    assert "p" not in set().union(*map(ltl.variables, psi))
    assert same_terms(psi, terms("G ((q & X !q) | s)", "G F (q & X !q)"), P)


def test_relaxation_example():
    # exists p. (Gamma & phi) = G(q | s) & G F q, exact
    psi = cave.relax(TARGET, CONTEXT, {"p"}, P)
    assert same_terms(psi, terms("G (q | s)", "G F q"), P)


def test_refinement_without_context():
    # Without the context nothing forces X p: the liveness term cannot be met.
    psi = cave.refine(TARGET, [], {"p"}, P)
    assert same_terms(psi, terms("G s", "G F false"), P)


def test_window_misses_the_step_before():
    # Section 4.1: Gamma = p & G(X p <-> q), phi = G(p <-> s). The exact
    # refinement s & G(X s <-> q) needs the invariant of the step before;
    # the invariant window gives false, which is sound.
    psi = cave.refine(terms("G (p <-> s)"), terms("p", "G (X p <-> q)"), {"p"}, P)
    assert same_terms(psi, terms("G false"), P)


def test_support_branch():
    # With G F a in the context and G (a -> p), G F p is met wherever a recurs:
    # the candidate of the support a is valid, and the term is dropped.
    types = {"a": "bool", "p": "bool"}
    psi = cave.refine(terms("G F p"), terms("G (a -> p)", "G F a"), {"p"}, types)
    assert psi == []


# Section 5: automated emergency braking -------------------------------------

B, R = "bool", "real"
ENVIRONMENT = ["20 <= d & d <= 30 & !URG & !WRN",
               "G (0 <= d & d <= 30)",
               "G (d <= 10 -> URG & !WRN)",
               "G (10 < d & d <= 15 -> WRN & !URG)",
               "G (d > 15 -> !URG & !WRN)",
               "G F (!URG & !WRN)"]


def supervisor():
    return Contract.from_strings(
        name="CS", inputs={"d": R, "URG": B, "WRN": B},
        outputs={"MOV": B, "DCL": B, "STP": B, "u": R}, assumptions=ENVIRONMENT,
        guarantees=["MOV & !DCL & !STP & u = d / 10",
                    "G ((MOV | DCL | STP) & !(MOV & DCL) & !(MOV & STP) & !(DCL & STP))",
                    "G (URG & !WRN -> X STP)",
                    "G (WRN & !URG & (MOV | DCL) -> X DCL)",
                    "G (WRN & !URG & STP -> X STP)",
                    "G (!(URG | WRN) -> X MOV)",
                    "G (0 <= u & u <= 6 & u <= d / 5)",
                    "G (X STP -> u = 0)", "G (X DCL -> u <= 3)", "G (X MOV -> u >= d / 10)",
                    "G F MOV"])


def vehicle():
    return Contract.from_strings(
        name="CV", inputs={"u": R, "MOV": B, "DCL": B, "STP": B}, outputs={"v": R},
        assumptions=["2 <= u & u <= 3", "G (0 <= u & u <= 6)", "G (X STP -> u = 0)",
                     "G (X DCL -> u <= 3)", "G (X MOV -> u >= 2)", "G F MOV"],
        guarantees=["v = u", "G (v >= 0)", "G (u - 0.5 <= v' & v' <= u + 0.5)"])


def prediction():
    return Contract.from_strings(name="CD", inputs={"d": R, "v": R}, outputs={"z": R},
                                 guarantees=["G (z' = d - v')"])


def implied(ts, types, init=None, inv=None) -> bool:
    i, v, _ = buckets(ts, types)
    e = Encoding(types)
    goals = [z3.Implies(i, e.formula(ltl.parse(init)))] if init else []
    goals += [z3.Implies(v, e.formula(ltl.parse(inv)))] if inv else []
    return all(equivalent(g, z3.BoolVal(True)) for g in goals)


def test_aeb_composition():
    cs, cv = supervisor(), vehicle()
    sv = cs.compose(cv, eliminate=True)
    assert sv.inputs == {"d": R, "URG": B, "WRN": B} and sv.outputs == {"v": R}
    hidden = {"u", "MOV", "DCL", "STP"}
    for t in sv.assumptions + sv.guarantees:
        assert not ltl.variables(t) & hidden
    # The environment assumptions of the supervisor are kept, and the
    # vehicle's needs are discharged by the supervisor's guarantees.
    for t in cs.assumptions:
        assert t in sv.assumptions
    # What the composition guarantees of the speed: v = d / 10 at the start,
    # never negative, and the step bound of the supervisor plus the error.
    types = {**sv.variables}
    assert implied(sv.guarantees, types, init="v = d / 10", inv="v >= 0 & v' <= d / 5 + 0.5")
    assert implied(sv.guarantees, types, inv="URG & !WRN -> v' <= 0.5")

    svd = sv.compose(prediction(), eliminate=True)
    assert svd.inputs == {"d": R, "URG": B, "WRN": B} and svd.outputs == {"z": R}
    for t in svd.guarantees:
        assert "v" not in ltl.variables(t)
    # The predicted clearance stays within the distance and its envelope.
    assert implied(svd.guarantees, {**svd.variables}, inv="z' <= d & z' >= 0.8 * d - 0.5")


def test_compose_eliminate_errors():
    a = Contract.from_strings(name="A", inputs={"x": B}, outputs={"y": B},
                              guarantees=["G (x U y)"])
    b = Contract.from_strings(name="B", inputs={"y": B}, outputs={"w": B}, guarantees=["G w"])
    with pytest.raises(cave.EliminationError, match="not a GR"):
        a.compose(b, eliminate=True)
    c = Contract.from_strings(name="C", inputs={"w": B}, outputs={"x": B}, guarantees=["G x"])
    loop = Contract.from_strings(name="L", inputs={"x": B}, outputs={"w": B}, guarantees=["G w"])
    with pytest.raises(cave.EliminationError, match="feedback"):
        c.compose(loop, eliminate=True)
    with pytest.raises(ValueError, match="exclusive"):
        b.compose(c, eliminate=True, discharge=True)


# The definitions on random formulas ----------------------------------------

KEPT, DROPPED = ["q", "s"], ["p"]
TYPES = {n: "bool" for n in KEPT + DROPPED}


def _atom(rnd, names):
    v = rnd.choice(names)
    v = f"X {v}" if rnd.random() < 0.3 else v
    return v if rnd.random() < 0.6 else f"!{v}"


def _core(rnd, names, size=2):
    f = _atom(rnd, names)
    for _ in range(rnd.randint(1, size)):
        f = f"({f} {rnd.choice(['&', '|', '|', '->', '->'])} {_atom(rnd, names)})"
    return f


def _formula(rnd, names):
    out = []
    if rnd.random() < 0.5:
        out.append(_core(rnd, names).replace("X ", ""))
    out.append(f"G {_core(rnd, names)}")
    if rnd.random() < 0.6:
        out.append(f"G F {_core(rnd, names, 1)}")
    return terms(*out)


def _draw(rnd, premise):
    """A context and a target with a lasso that meets the premise (so that
    the test is not vacuous)."""
    while True:
        context, target = _formula(rnd, KEPT + DROPPED), _formula(rnd, KEPT + DROPPED)
        if any(premise(t, context, target) for t in _lassos(KEPT + DROPPED)):
            return context, target


def _lassos(names, prefix=1, loop=2):
    states = [dict(zip(names, bits)) for bits in itertools.product([False, True],
                                                                  repeat=len(names))]
    for n in range(prefix + 1):
        for m in range(1, loop + 1):
            for pre in itertools.product(states, repeat=n):
                for cyc in itertools.product(states, repeat=m):
                    yield Trace(prefix=list(pre), loop=list(cyc))


def _sat(trace, ts):
    return all(holds(t, trace, TYPES) for t in ts)


@pytest.mark.parametrize("seed", range(RUNS))
def test_refinement_is_sound(seed):
    # context & psi -> target on every lasso of up to three states
    rnd = random.Random(seed)
    context, target = _draw(rnd, lambda t, c, _: _sat(t, c))
    psi = cave.refine(target, context, set(DROPPED), TYPES)
    assert not set().union(*map(ltl.variables, psi)) & set(DROPPED)
    for trace in _lassos(KEPT + DROPPED):
        if _sat(trace, context) and _sat(trace, psi):
            assert _sat(trace, target), (seed, trace)


@pytest.mark.parametrize("seed", range(RUNS))
def test_relaxation_is_sound(seed):
    # context & target -> psi on every lasso of up to three states
    rnd = random.Random(1000 + seed)
    context, target = _draw(rnd, lambda t, c, g: _sat(t, c) and _sat(t, g))
    psi = cave.relax(target, context, set(DROPPED), TYPES)
    assert not set().union(*map(ltl.variables, psi)) & set(DROPPED)
    for trace in _lassos(KEPT + DROPPED):
        if _sat(trace, context) and _sat(trace, target):
            assert _sat(trace, psi), (seed, trace)


@pytest.mark.parametrize("seed", range(RUNS))
def test_simplification_is_equivalent_in_its_context(seed):
    # On every lasso of the context, the simplified terms hold iff the terms do.
    rnd = random.Random(5000 + seed)
    context, target = _draw(rnd, lambda t, c, g: _sat(t, c))
    psi = cave.simplify(target, context, TYPES)
    for trace in _lassos(KEPT + DROPPED):
        if _sat(trace, context):
            assert _sat(trace, psi) == _sat(trace, target), (seed, trace)


@needs_all
@pytest.mark.parametrize("seed", range(RUNS))
def test_composition_abstracts_the_closed_form(seed):
    # The closed form refines the composition with eliminated variables.
    rnd = random.Random(2000 + seed)
    c1 = Contract.from_strings(name="C1", inputs={"a": B}, outputs={"m": B},
                               assumptions=[ltl.to_str(t) for t in _formula(rnd, ["a"])],
                               guarantees=[ltl.to_str(t) for t in _formula(rnd, ["a", "m"])])
    c2 = Contract.from_strings(name="C2", inputs={"m": B}, outputs={"b": B},
                               assumptions=[ltl.to_str(t) for t in _formula(rnd, ["m"])],
                               guarantees=[ltl.to_str(t) for t in _formula(rnd, ["m", "b"])])
    assert refines(c1.compose(c2), c1.compose(c2, eliminate=True)).status == "valid"
