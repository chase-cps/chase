"""Sampled signals (chase_cps.sampled): the stricter formula of Fainekos and
Pappas (Corollary 44), and a check of it on random signals: whenever the
samples hold the stricter formula, the signal between the samples holds
the formula."""

import random

import pytest

from chase_cps import ltl
from chase_cps.queries import Trace
from chase_cps.robustness import holds
from chase_cps.sampled import SampledError, continuous, sampled


def text(formula, lipschitz=1, period=1):
    return sampled(formula, lipschitz, period)


def same(formula, lipschitz, period, expected):
    assert ltl.parse(text(formula, lipschitz, period)) == ltl.parse(expected)


def test_the_intervals():
    # G (and R): one step wider at each end, from 0; F (and U): one narrower
    same("G[1,6] (x <= 3)", 1, 1, "G[0,7] (x + 1 < 3)")
    same("F[2,9] (x >= 5)", 1, 1, "F[3,8] (x - 1 > 5)")
    # under a negation, the other one
    same("!F[0,6] (x > 7)", 1, 1, "G[0,7] (x + 1 < 7)")
    same("!G[0,6] (x > 7)", 1, 1, "F[1,5] (x + 1 < 7)")
    same("(x >= 7) -> F[0,6] (y >= 2)", 1, 1, "x + 1 < 7 | F[1,5] (y - 1 > 2)")
    # with no interval: F from the next sample on, G as it is
    same("G (x <= 3)", 1, 1, "G (x + 1 < 3)")
    same("F (x >= 3)", 1, 1, "X F (x - 1 > 3)")
    same("(x <= 4) U (y >= 1)", 1, 1, "x + 1 < 4 & X ((x + 1 < 4) U (y - 1 > 1))")


def test_the_intervals_are_written_as_intervals():
    assert text("G[0,20] (T <= 75)", {"T": 0.5}, 1) == "G[0,21] (T + 0.5 < 75)"
    assert text("(x <= 4) U[0,9] (y >= 1)") == "(x + 1 < 4) U[1,8] (y - 1 > 1)"


def test_the_margins():
    # (|2| Lx + |-1| Ly) period
    same("G[0,6] (2 * x - y <= 3)", {"x": 1, "y": 2}, "0.5", "G[0,7] (2 * x - y + 2 < 3)")
    same("G[0,6] (x / 4 <= 3)", 2, 1, "G[0,7] (x / 4 + 0.5 < 3)")
    # a term that does not change keeps its comparison
    same("G[0,6] (x <= 3)", {"x": 0}, 1, "G[0,7] (x <= 3)")
    # equality is never robust; != is
    same("x = 3", 1, 1, "x + 1 < 3 & x - 1 > 3")
    same("x != 3", 1, 1, "x + 1 < 3 | x - 1 > 3")


@pytest.mark.parametrize("formula, why", [
    ("X (x <= 1)", "X is the next sample"),
    ("x' <= 1", "next sample"),
    ("G[0,3] (x <= 1)", "more than 3 steps"),
    ("F[2,2] (x <= 1)", "one instant"),
    ("b & x <= 1", "Boolean b"),
    ("G (x * y <= 1)", "not linear"),
    ("(x <= 1) <-> (y <= 1)", "both polarities"),
    ("G (past(x, 2) <= 1)", "counts samples"),
])
def test_what_is_not_read_in_continuous_time(formula, why):
    with pytest.raises(SampledError, match=why):
        sampled(formula, 1, 1)
    r = continuous(formula, Trace([{"x": 0, "y": 0, "b": True}], []), 1, 1)
    assert r.holds is None and why in r.reason


def test_no_constant_for_a_variable():
    with pytest.raises(SampledError, match="no Lipschitz constant for y"):
        sampled("G[0,6] (x + y <= 3)", {"x": 1}, 1)


def test_a_trace():
    t = Trace([{"x": i} for i in range(10)], [])          # x goes up by 1 per sample
    assert continuous("G[0,8] (x <= 10)", t, 1, 1).holds is None    # 9 + 1 is not < 10
    r = continuous("G[0,8] (x <= 11)", t, 1, 1)
    assert r.holds is True and r.robustness == 1
    # F[0,8] is F[1,7]: x is at most 7 there
    assert continuous("F[0,8] (x >= 6)", t, 1, 1).holds is None     # 7 - 1 is not > 6
    assert continuous("F[0,8] (x >= 5)", t, 1, 1).holds is True     # 7 - 1 > 5


SAMPLES, FINE, RATE = 12, 4, 4    # 4 instants between two samples; a change of 1 per instant
TEMPLATES = ["G[{a},{b}] (x <= {c})", "F[{a},{b}] (x >= {c})", "!F[{a},{b}] (x > {c})",
             "!G[{a},{b}] (x < {c})", "G[{a},{b}] (x <= {c} -> F[{e},{f}] (y >= {d}))",
             "(x <= {c}) U[{a},{b}] (y >= {d})", "(x >= {c}) R[{a},{b}] (y <= {d})",
             "G (x + y <= {c})", "F (x - y >= {d})", "F[{a},{b}] (x >= {c} & y <= {d})",
             "G[{a},{b}] (2 * x - y <= {c} | y >= {d})", "(x <= {c}) U (y >= {d})"]


def _interval(rng):
    a = rng.randint(0, 4)
    return a, a + rng.randint(4, 7)


def _formulas(rng):
    t = rng.choice(TEMPLATES)
    (a, b), (e, f) = _interval(rng), _interval(rng)
    c, d = rng.randint(-6, 8), rng.randint(-6, 8)
    sampled = t.format(a=a, b=b, c=c, d=d, e=e, f=f)
    fine = t.format(a=a * FINE, b=b * FINE, c=c, d=d, e=e * FINE, f=f * FINE)
    return sampled, fine


def _signal(rng):
    """A signal at every instant, changing by at most 1 per instant (RATE
    per sample), constant after the last sample."""
    out, x, y = [], rng.randint(-4, 4), rng.randint(-4, 4)
    for _ in range((SAMPLES - 1) * FINE + 1):
        out.append({"x": x, "y": y})
        x += rng.choice((-1, 0, 1))
        y += rng.choice((-1, 0, 0, 1))
    return out


def test_what_the_samples_show_holds_between_them():
    rng, shown = random.Random(44), 0
    for _ in range(1500):
        signal = _signal(rng)
        samples = Trace(signal[::FINE], [])
        sampled, fine = _formulas(rng)
        r = continuous(sampled, samples, RATE, 1)
        if r.holds:
            shown += 1
            assert holds(fine, Trace(signal, [])), (sampled, signal)
    assert shown > 200        # enough cases where the samples show it (253)


def test_every_behavior_in_continuous_time():
    # the stricter formula is a formula like any other: checked over every
    # behavior, it shows the property between the samples too
    from chase_cps import Contract, refines
    from chase_cps.backends import ADAPTERS
    if not (ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()):
        pytest.skip("nuXmv or NuSMV not installed")
    ports = dict(inputs={"u": (-1, 1)}, outputs={"x": (-30, 30)})
    plant = Contract.from_strings(name="Plant", guarantees=["x = 0", "G (x' = x + u)"], **ports)

    def check(req, strict):
        g = sampled(req, {"x": 1}, 1) if strict else req
        return refines(plant, Contract.from_strings(name="Req", guarantees=[g], **ports)).status
    assert check("G[0,8] (x <= 10)", False) == "valid"      # at the samples: x <= 9
    assert check("G[0,8] (x <= 10)", True) == "invalid"     # between them, not shown
    assert check("G[0,8] (x <= 11)", True) == "valid"


def test_a_negative_lipschitz_constant_is_refused():
    with pytest.raises(SampledError, match="negative"):
        sampled("G[0,8] (x <= 11)", {"x": -1}, 0.5)
    with pytest.raises(SampledError, match="negative"):
        sampled("G[0,8] (x <= 11)", -1, 0.5)
    assert continuous("G[0,8] (x <= 11)", Trace([{"x": 10.8}], []), {"x": -100}, 1).holds is None
