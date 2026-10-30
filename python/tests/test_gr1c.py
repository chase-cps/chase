"""The input of gr1c written from a contract (chase_cps.to_gr1c)."""

import re

import pytest

from chase_cps import Contract, Gr1cError, ltl, refines, to_gr1c
from chase_cps.backends import ADAPTERS

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")

SECTIONS = ("ENVINIT", "SYSINIT", "ENVTRANS", "SYSTRANS", "ENVGOAL", "SYSGOAL")


def read_back(c: Contract, text: str) -> Contract:
    """The contract the gr1c text says, read with the parser of CHASE (it
    reads [], <>, and primes too), for domains that start at 0."""
    found = dict(re.findall(r"^(" + "|".join(SECTIONS) + r"):(.*?);", text, re.S | re.M))

    def terms(*names):
        out = []
        for n in names:
            body = found.get(n, "").strip()
            for part in re.split(r"\n\t& ", body) if body else []:
                out.append(ltl.parse(part.replace("True", "true").replace("False", "false")))
        return out
    return Contract(c.name + "_gr1c", dict(c.inputs), dict(c.outputs),
                    terms("ENVINIT", "ENVTRANS", "ENVGOAL"), terms("SYSINIT", "SYSTRANS", "SYSGOAL"))


ARITH = Contract.from_strings(
    name="Arith", inputs={"x": (0, 3), "y": (0, 3)}, outputs={"z": (0, 6), "b": "bool"},
    assumptions=["x = 0", "G (x != y -> x' = y)", "G F (x < y)"],
    guarantees=["z = 0", "G (X z = x + y)", "G (b <-> z >= 3)", "G F (b | z - x = 1)"])


def test_sections_and_declarations():
    text = to_gr1c(ARITH)
    assert "ENV:\n\tx [0, 3]\n\ty [0, 3];" in text
    assert "SYS:\n\tb\n\tz [0, 6];" in text
    assert "ENVINIT:\n\t(x=0);" in text and "SYSGOAL:" in text
    assert "+" not in text and "<" not in text.replace("<->", "").replace("<>", "")


@needs_mc
def test_the_text_says_the_same_contract():
    back = read_back(ARITH, to_gr1c(ARITH))
    assert refines(ARITH, back).status == "valid"
    assert refines(back, ARITH).status == "valid"


def test_integers_are_written_from_zero():
    c = Contract.from_strings(name="Shift", inputs={"x": (2, 5)}, outputs={"y": "bool"},
                              guarantees=["G (y <-> x = 3)"])
    text = to_gr1c(c)
    assert "x [0, 3]" in text and "(x=1)" in text and "x = value - 2" in text


def test_what_gr1c_cannot_take():
    real = Contract.from_strings(name="Real", inputs={"w": "real"}, outputs={"y": "bool"},
                                 guarantees=["G (y -> w > 0.5)"])
    with pytest.raises(Gr1cError, match="Booleans and bounded integers"):
        to_gr1c(real)
    not_gr1 = Contract.from_strings(name="Until", inputs=["a"], outputs=["b"], guarantees=["a U b"])
    with pytest.raises(Gr1cError, match="not a GR"):
        to_gr1c(not_gr1)
    two_steps = Contract.from_strings(name="Two", inputs={"x": (0, 1)}, outputs=["b"],
                                      guarantees=["G (b -> X (x' = 1))"])
    with pytest.raises(Gr1cError, match="one step ahead"):
        to_gr1c(two_steps)
