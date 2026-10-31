"""The size of folded compositions: a composition is saturated already, and
is not saturated again (its guarantee would be written twice at each step),
and its assumption X | !G is composed as X. Both are equivalences, checked
here against the closed form written out step by step."""

import random

import pytest

from chase_cps import Contract, ltl, refines
from chase_cps.backends import ADAPTERS

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()


def size(n) -> int:
    return 1 + sum(size(c) for c in (getattr(n, "left", None), getattr(n, "right", None),
                                     getattr(n, "arg", None)) if isinstance(c, ltl.Node))


def closed_form(cs):
    """((A1 & A2) | !(G1' & G2'), G1' & G2'), folded, with G' = A -> G each time."""
    a, g = cs[0].assumption, ltl.Binary("->", cs[0].assumption, cs[0].guarantee)
    for c in cs[1:]:
        g2 = ltl.Binary("->", c.assumption, c.guarantee)
        g = ltl.Binary("&", ltl.Binary("->", a, g), g2)
        a = ltl.Binary("|", ltl.Binary("&", a, c.assumption), ltl.Unary("!", g))
    ins = {v: t for c in cs for v, t in c.inputs.items()}
    outs = {v: t for c in cs for v, t in c.outputs.items()}
    return Contract("closed", {v: t for v, t in ins.items() if v not in outs}, outs, [a], [g])


def chain(seed: int, n: int):
    rnd = random.Random(seed)
    names = [f"v{i}" for i in range(n + 1)]
    pick = lambda x, y: rnd.choice([f"G ({x} -> F {y})", f"G ({x} -> X {y})", f"G F {y}", f"G ({y} -> {x})",
                                    f"F G !{y}", f"G ({x} | {y})"])
    return [Contract.from_strings(name=f"C{i}", inputs={names[i]: "bool"}, outputs={names[i + 1]: "bool"},
                                  assumptions=[rnd.choice([f"G F {names[i]}", f"F {names[i]}", "true"])],
                                  guarantees=[pick(names[i], names[i + 1])]) for i in range(n)]


@pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
@pytest.mark.parametrize("seed", range(6))
def test_folded_composition_is_the_closed_form(seed):
    cs = chain(seed, 3 + seed % 2)
    folded = cs[0]
    for c in cs[1:]:
        folded = folded.compose(c)
    expected = closed_form(cs)
    assert refines(folded, expected).status == "valid"
    assert refines(expected, folded).status == "valid"


def test_folded_compositions_grow_linearly():
    def fold(n):
        cs = [Contract.from_strings(name=f"C{i}", inputs={f"x{i}": "real"}, outputs={f"x{i + 1}": "real"},
                                    assumptions=[f"x{i} >= 0"], guarantees=[f"x{i + 1} >= x{i} + 1"])
              for i in range(n)]
        c = cs[0]
        for x in cs[1:]:
            c = c.compose(x)
        return size(c.assumption) + size(c.guarantee)
    small, large = fold(8), fold(32)
    assert large < 5 * small            # 4 times the contracts: about 4 times the size
