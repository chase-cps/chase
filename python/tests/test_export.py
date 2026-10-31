"""The checks of a contract written for the tools (chase_cps.export): the
files given to nuXmv and z3 answer as the checks of chase-cps do, and the
command export writes them."""

import json
import random
import re
import subprocess

import pytest

from chase_cps import Contract, ltl, refines, to_smt, to_smv, verify
from chase_cps.backends import ADAPTERS
from chase_cps.export import questions

HAS_Z3 = ADAPTERS["z3"].available()
NUXMV = ADAPTERS["nuxmv"].binary()
needs_z3 = pytest.mark.skipif(not HAS_Z3, reason="z3 not installed")
needs_nuxmv = pytest.mark.skipif(not (NUXMV and NUXMV[1] == "nuxmv"), reason="nuXmv not installed")

GEN = Contract.from_strings(name="Gen", inputs={"n": (0, 20)}, outputs={"p": "real"},
                            assumptions=["n <= 4"], guarantees=["p >= 2.5 * n", "p <= 30"])
SPEC = Contract.from_strings(name="Spec", inputs={"n": (0, 20)}, outputs={"p": "real"},
                             assumptions=["n <= 4"], guarantees=["p >= 12", "p <= 30"])
ARB = Contract.from_strings(name="Arb", inputs={"r": "bool", "next": "bool"},
                            outputs={"g": "bool", "c": (0, 3)}, assumptions=["G F r"],
                            guarantees=["G (r -> F g)", "G (c' = c + 1 | c = 3)", "G (next -> g)"])


def _nuxmv(text, tmp_path, reals=False):
    """The answer of nuXmv to each LTLSPEC of the file, in order."""
    path = tmp_path / "check.smv"
    path.write_text(text)
    if reals:
        out = subprocess.run([NUXMV[0], "-int", str(path)], input="go_msat\ncheck_ltlspec_ic3\nquit\n",
                             capture_output=True, text=True).stdout
    else:
        out = subprocess.run([NUXMV[0], str(path)], capture_output=True, text=True).stdout
    return re.findall(r"specification .* is (true|false)", out)


def _z3(text):
    """The answer of z3 to each block of the file, in order."""
    import z3
    answers = []
    for block in text.split("(reset)"):
        s = z3.Solver()
        s.from_string("\n".join(l for l in block.splitlines()
                                if not l.startswith(("(echo", "(check-sat"))))
        answers.append(str(s.check()))
    return answers


def _expected(contract, abstract=None, backend="auto"):
    """The answers that the file must give, from the checks of chase-cps:
    for a satisfiability question, false (SMV) or sat; for an implication,
    true or unsat when it is valid."""
    if abstract is None:
        v = verify(contract, backend)
        assert "unknown" not in (v.compatible.status, v.consistent.status, v.feasible.status)
        return [r.status == "sat" for r in (v.compatible, v.consistent, v.feasible)]
    r = refines(contract, abstract, backend)
    assert r.status != "unknown"
    return [r.assumptions.status == "valid", r.guarantees.status == "valid"]


def test_the_questions():
    assert [n for n, _ in questions(ARB)] == ["compatible", "consistent", "feasible"]
    assert [n for n, _ in questions(GEN, SPEC)] == ["assumptions", "guarantees"]
    text = to_smv(ARB)
    assert "LTLSPEC NAME feasible := " in text and "next is chase_next" in text
    assert "chase_next : boolean;" in text
    with pytest.raises(ValueError, match="NuSMV takes no real"):
        to_smv(GEN, tool="nusmv")
    with pytest.raises(ValueError, match="one of nuxmv, nusmv"):
        to_smv(ARB, tool="jasper")


@needs_nuxmv
def test_nuxmv_answers_as_chase(tmp_path):
    want = _expected(ARB)
    got = _nuxmv(to_smv(ARB), tmp_path)
    assert got == ["false" if w else "true" for w in want]
    want = _expected(GEN, SPEC)
    got = _nuxmv(to_smv(GEN, refines=SPEC), tmp_path, reals=True)
    assert got == ["true" if w else "false" for w in want] == ["true", "false"]


def _formula(rng, depth):
    atoms = ["x <= 3", "y > x", "x + y = 6", "2 * x - y >= 1", "x != y", "r >= 1.5",
             "r < x", "x / 2 = -1", "b"]
    if depth == 0 or rng.random() < 0.3:
        return rng.choice(atoms)
    op = rng.choice(["!", "&", "|", "->"])
    if op == "!":
        return f"!({_formula(rng, depth - 1)})"
    return f"({_formula(rng, depth - 1)}) {op} ({_formula(rng, depth - 1)})"


@needs_z3
@needs_nuxmv
def test_z3_answers_as_nuxmv():
    rng = random.Random(5)
    seen = set()
    kinds = {"inputs": {"x": (-4, 6), "b": "bool"}, "outputs": {"y": (0, 7), "r": "real"}}
    for i in range(40):
        a, b = (Contract.from_strings(name=f"C{i}{k}", **kinds,
                                      assumptions=[_formula(rng, 2)],
                                      guarantees=[_formula(rng, 2), _formula(rng, 1)])
                for k in "ab")
        want = _expected(a, backend="nuxmv")
        assert _z3(to_smt(a)) == ["sat" if w else "unsat" for w in want]
        seen.update(want)
        want = _expected(a, b, backend="nuxmv")
        assert _z3(to_smt(a, refines=b)) == ["unsat" if w else "sat" for w in want]
        seen.update(want)
    assert seen == {True, False}


@needs_z3
def test_smt_takes_formulas_over_bounded_steps_only():
    with pytest.raises(ValueError, match="bounded number of steps"):
        to_smt(ARB)
    text = to_smt(GEN, refines=SPEC)
    assert text.count("(check-sat)") == 2 and text.count("(reset)") == 1
    steps = Contract.from_strings(name="Steps", inputs={"x": (0, 9)}, outputs={"y": (0, 9)},
                                  assumptions=["x = 0", "G[0,2] (x' = x + 1)"],
                                  guarantees=["X X (y = x + past(x, 1))"])
    assert "x@2" in to_smt(steps) and _z3(to_smt(steps)) == ["sat", "sat", "sat"]
    assert '(echo "guarantees")' in text and text.count("(set-logic ALL)") == 2


def test_the_command(tmp_path, capfd):
    from chase_cps.cli import main
    spec = tmp_path / "two.json"
    spec.write_text(json.dumps({"contracts": [GEN.to_dict(), SPEC.to_dict()]}))
    assert main(["export", str(spec), "--to", "smv"]) == 2
    assert "--contract: one of Gen, Spec" in capfd.readouterr().err
    assert main(["export", str(spec), "--to", "smv", "--contract", "Gen", "--refines", "Spec"]) == 0
    out = capfd.readouterr().out
    assert out.startswith("-- Gen refines Spec") and "LTLSPEC NAME guarantees" in out
    assert main(["export", str(spec), "--to", "sva", "--tool", "jasper", "--contract", "Gen"]) == 2
    assert "takes no --tool" in capfd.readouterr().err
    back = tmp_path / "back.json"
    assert main(["export", str(spec), "--to", "json", "-o", str(back)]) == 0
    assert capfd.readouterr().out == ""
    assert [Contract.from_dict(d).name for d in json.loads(back.read_text())["contracts"]] == \
        ["Gen", "Spec"]


def test_the_command_writes_only_the_file_on_the_standard_output(tmp_path, capfd):
    pytest.importorskip("chase_cps._native")
    from chase_cps.cli import main
    from chase_cps import to_spec
    path = tmp_path / "arb.ltl"     # the library writes what it reads on the standard output
    path.write_text(to_spec([Contract.from_strings(
        name="Arb", inputs={"r": "bool"}, outputs={"g": "bool"}, assumptions=["G F r"],
        guarantees=["G (r -> F g)"])]))
    assert main(["export", str(path), "--to", "smv", "-o", str(tmp_path / "arb.smv")]) == 0
    assert capfd.readouterr().out == ""
    assert main(["export", str(path), "--to", "smv"]) == 0
    assert capfd.readouterr().out.startswith("-- verify Arb")
