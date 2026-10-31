"""C functions as implementations of contracts (chase_cps.cprograms), with
ESBMC and CBMC: the program written for them, and their answers on
functions that are right, wrong, that overflow, and whose loops run long."""

import pytest

from chase_cps import CFunction, CProgramError, Contract, is_implementation, to_c
from chase_cps.backends import ADAPTERS
from chase_cps.cprograms import roles
from chase_cps.robustness import holds

TOOLS = [pytest.param(t, marks=pytest.mark.skipif(not ADAPTERS[t].available(),
                                                  reason=f"{t} not installed"))
         for t in ("esbmc", "cbmc")]

SAT = Contract.from_strings(name="Sat", inputs={"x": (-1000, 1000), "lo": (-100, 100),
                                                "hi": (-100, 100)},
                            outputs={"y": (-1000, 1000)}, assumptions=["lo <= hi"],
                            guarantees=["y >= lo & y <= hi", "(x >= lo & x <= hi) -> y = x"])

RIGHT = """
/* clamps x into [lo, hi] */
int saturate(int x, int lo, int hi) {
    int y = x;
    for (int i = 0; i < 3; i++) {
        if (y < lo) y = lo;
        if (y > hi) y = hi;
    }
    return y;
}
"""
WRONG = RIGHT.replace("if (y > hi) y = hi;", "if (y > hi) y = hi + 1;")
OVERFLOW = RIGHT.replace("int y = x;", "int y = x; int big = (hi - lo) * 100000000; y += big - big;")
LONG = """
int saturate(int x, int lo, int hi) {
    int y = x;
    while (y > hi) y--;
    while (y < lo) y++;
    return y;
}
"""


@pytest.mark.parametrize("tool", TOOLS)
def test_the_answers(tool):
    r = is_implementation(CFunction("sat.c", "saturate", source=RIGHT), SAT, tool)
    assert r.status == "valid" and r.backend == tool
    r = is_implementation(CFunction("sat.c", "saturate", source=WRONG), SAT, tool)
    assert r.status == "invalid" and r.reason == ""
    state = r.trace.loop[0]
    # the call breaks the guarantees: y is hi + 1
    assert state["y"] == state["hi"] + 1 and holds(SAT.assumption, r.trace)
    assert not holds(SAT.guarantee, r.trace)
    r = is_implementation(CFunction("sat.c", "saturate", source=OVERFLOW), SAT, tool)
    assert r.status == "invalid" and "overflow" in r.reason
    assert "chase_cps_" not in r.reason             # no temporary directory
    # loops that run up to 60 times
    small = Contract.from_strings(name="Small", inputs={"x": (-50, 50), "lo": (-10, 10),
                                                        "hi": (-10, 10)},
                                  outputs={"y": (-50, 50)}, assumptions=SAT.assumptions,
                                  guarantees=SAT.guarantees)
    r = is_implementation(CFunction("sat.c", "saturate", source=LONG), small, tool)
    assert r.status == "unknown" and "unwind=20" in r.reason
    r = is_implementation(CFunction("sat.c", "saturate", source=LONG), small, tool, unwind=70)
    assert r.status == "valid"


THERMOSTAT = """
#include <stdbool.h>
/* heat while the temperature is below the target; power in [0, 3] */
void thermostat(double t, double target, bool *heat, double *power) {
    *heat = t < target;
    *power = *heat ? (target - t > 3.0 ? 3.0 : target - t) : 0.0;
}
"""


@pytest.mark.parametrize("tool", TOOLS)
def test_outputs_by_pointer_reals_and_booleans(tool):
    c = Contract.from_strings(name="Thermostat", inputs={"t": "real", "target": "real"},
                              outputs={"heat": "bool", "power": "real"},
                              assumptions=["t >= -40 & t <= 60", "target >= 15 & target <= 25"],
                              guarantees=["heat <-> t < target", "power >= 0 & power <= 3",
                                          "!heat -> power = 0"])
    f = CFunction("thermostat.c", "thermostat", source=THERMOSTAT)
    assert is_implementation(f, c, tool).status == "valid"
    stronger = c.ensure("heat -> power >= 0.5")
    r = is_implementation(f, stronger, tool)
    assert r.status == "invalid" and r.trace.loop[0]["heat"] is True
    assert 0 <= r.trace.loop[0]["power"] < 0.5
    # the values are exact (the checkers print doubles rounded; they are read from their bits)
    assert holds(stronger.assumption, r.trace) and not holds(stronger.guarantee, r.trace)


def test_the_program():
    f = CFunction("sat.c", "saturate", source=RIGHT)
    text = to_c(SAT, f)
    assert "int saturate(int x, int lo, int hi);" in text
    assert "__VERIFIER_assume((((long long)lo) <= ((long long)hi)));" in text
    assert "int y = saturate(x, lo, hi);" in text and "assert(" in text
    # assumptions on an output: the guarantees are asserted when they hold
    odd = SAT.assume("y >= -50")
    assert "assert(!(" in to_c(odd, f)
    sig, result = roles(SAT, f)
    assert result == "y" and [p.name for p in sig.parameters] == ["x", "lo", "hi"]


def test_what_cannot_be_checked():
    f = CFunction("sat.c", "saturate", source=RIGHT)
    with pytest.raises(CProgramError, match="answers once"):
        to_c(SAT.ensure("G (y >= lo)"), f)
    other = Contract.from_strings(name="Other", inputs={"x": (0, 9), "z": (0, 9)},
                                  outputs={"y": (0, 9)}, guarantees=["y = x"])
    with pytest.raises(CProgramError, match="the parameter lo is not an input"):
        to_c(other, f)
    with pytest.raises(CProgramError, match="no definition of the function clamp"):
        to_c(SAT, CFunction("sat.c", "clamp", source=RIGHT))
    words = Contract.from_strings(name="Words", inputs={"x": "word[8]"}, outputs={"y": "word[8]"},
                                  guarantees=["y = x"])
    with pytest.raises(CProgramError, match="words"):
        to_c(words, CFunction("w.c", "f", source="unsigned char f(unsigned char x) { return x; }"))
    r = is_implementation(CFunction("sat.c", "clamp", source=RIGHT), SAT, "esbmc")
    assert r.status == "unknown" and "no definition" in r.reason


@pytest.mark.skipif(not (ADAPTERS["esbmc"].available() or ADAPTERS["cbmc"].available()),
                    reason="neither ESBMC nor CBMC is installed")
def test_the_command_line(tmp_path, capfd):
    pytest.importorskip("chase_cps._native")
    from chase_cps import to_spec
    from chase_cps.cli import main
    spec = tmp_path / "sat.ltl"
    spec.write_text(to_spec([SAT], name="sat"))
    (tmp_path / "right.c").write_text(RIGHT)
    (tmp_path / "wrong.c").write_text(WRONG)
    assert main(["check", str(spec), "--implementation", f"{tmp_path}/right.c:saturate", "Sat"]) == 0
    assert main(["check", str(spec), "--implementation", f"{tmp_path}/wrong.c:saturate", "Sat"]) == 1
    out = capfd.readouterr().out
    assert "implementation saturate |= Sat" in out and "counterexample:" in out
    assert main(["check", str(spec), "--implementation", f"{tmp_path}/wrong.c", "Sat"]) == 2
    assert "FILE.c:FUNCTION" in capfd.readouterr().err
    assert main(["export", str(spec), "--to", "c", "--function", f"{tmp_path}/right.c:saturate"]) == 0
    assert "int y = saturate(x, lo, hi);" in capfd.readouterr().out
    assert main(["export", str(spec), "--to", "c"]) == 2
