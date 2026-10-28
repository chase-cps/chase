"""Tests of the engines of nuXmv and NuSMV (the option ``engine``, and the
back end written as "nuxmv/bdd"): every engine gives the same verdicts, and
the portfolio stops the engines that lose. Skipped without nuXmv."""

import json
import subprocess
import time

import pytest

from chase_cps import Contract, implies, is_receptive, is_satisfiable, ltl, verify
from chase_cps.backends import ADAPTERS, split
from chase_cps.backends.nusmv import NuSMVAdapter
from chase_cps.queries import Implies

HAS_MC = ADAPTERS["nuxmv"].available()
needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv not installed")

V = {"a": "bool", "b": "bool", "n": (0, 7), "cnt": "word[8]"}

# (query, expected status): proofs, counterexamples, sat and unsat.
CASES = [
    (lambda b, **o: implies("G (a -> X b)", "G (a -> F b)", V, b, **o), "valid"),
    (lambda b, **o: implies("G F a", "F G a", V, b, **o), "invalid"),
    (lambda b, **o: implies("n = 2 & G (n' = n)", "G (n < 3)", V, b, **o), "valid"),
    (lambda b, **o: implies("G (n' = n + 1 | n = 7)", "G (n < 7)", V, b, **o), "invalid"),
    (lambda b, **o: is_satisfiable("G F a & G F !a", V, b, **o), "sat"),
    (lambda b, **o: is_satisfiable("G a & F !a", V, b, **o), "unsat"),
    (lambda b, **o: implies("cnt = 255 & G (cnt' = cnt + 1)", "X (cnt = 0)", V, b, **o),
     "valid"),
]


def test_split():
    assert split("nuxmv/bdd") == ("nuxmv", "bdd")
    assert split("nuxmv") == ("nuxmv", None) and split("auto") == ("auto", None)


@needs_mc
@pytest.mark.parametrize("engine", ["auto", "ic3", "bdd", "portfolio"])
def test_every_engine_gives_the_same_verdicts(engine):
    for case, expected in CASES:
        r = case("nuxmv", engine=engine)
        assert r.status == expected, (engine, expected, r)
        if engine in ("ic3", "bdd"):
            assert r.backend == f"nuxmv/{engine}"
        if r.status in ("invalid", "sat"):
            assert r.trace is not None


@needs_mc
def test_bmc_finds_counterexamples_but_proves_nothing():
    for case, expected in CASES:
        r = case("nuxmv", engine="bmc")
        if expected in ("invalid", "sat"):
            assert r.status == expected
        else:
            assert r.status == "unknown" and "no counterexample up to bound" in r.reason


@needs_mc
def test_the_engine_in_the_name_of_the_back_end():
    r = implies("G F a", "F G a", V, "nuxmv/bdd")
    assert r.status == "invalid" and r.backend == "nuxmv/bdd"
    c = Contract.from_strings({"a": "bool"}, {"b": "bool"}, ["G F a"], ["G (a -> b)"])
    assert verify(c, "nuxmv/bdd").status == "sat"


@needs_mc
def test_the_engine_from_the_environment(monkeypatch):
    monkeypatch.setenv("CHASE_BACKEND", "nuxmv/bdd")
    assert implies("G F a", "F G a", V).backend == "nuxmv/bdd"


@needs_mc
def test_the_portfolio_answers_with_the_fastest_engine():
    # The counterexample is a loop of 256 steps: IC3 takes minutes, BDDs a moment.
    start = time.monotonic()
    r = implies("cnt = 254 & G (cnt' = cnt + 1)", "X (cnt = 0)", V, "nuxmv/portfolio",
                timeout=60)
    assert r.status == "invalid" and r.backend == "nuxmv/bdd"
    assert len(r.trace.loop) == 256
    assert time.monotonic() - start < 30
    # The IC3 run was stopped.
    assert subprocess.run(["pgrep", "-f", "check_ltlspec_ic3"], capture_output=True).stdout \
        in (b"", None)
    r = implies("cnt = 254 & G (cnt' = cnt + 1)", "X (cnt = 0)", V, "nuxmv/ic3", timeout=2)
    assert r.status == "unknown" and r.reason == "timeout after 2 s"


@needs_mc
def test_engines_that_do_not_apply():
    with pytest.raises(ValueError, match="unknown engine 'sat'"):
        implies("a", "a", V, engine="sat")
    # Real variables compared with constants become a finite model: any engine.
    assert implies("x <= 1.5", "x <= 2", {"x": "real"}, "nuxmv/bdd").status == "valid"
    # The SMT engine of nuXmv only works with IC3 and BMC.
    q = Implies(ltl.parse("x + y <= 1 & y >= 0"), ltl.parse("x <= 1"),
                {"x": "real", "y": "real"})
    r = NuSMVAdapter("nuxmv", engine="bdd").run(q)
    assert r.status == "unknown" and "does not check real variables" in r.reason
    assert NuSMVAdapter("nuxmv", engine="portfolio").run(q).status == "valid"


@needs_mc
def test_nusmv_has_no_ic3(monkeypatch):
    path, _ = ADAPTERS["nuxmv"].binary()
    monkeypatch.setattr(NuSMVAdapter, "binary", lambda self: (path, "nusmv"))
    r = implies("a", "a", V, "nusmv/ic3")
    assert r.status == "unknown" and "NuSMV has no IC3" in r.reason


@needs_mc
def test_receptiveness_ignores_the_engine():
    c = Contract.from_strings({"a": "bool"}, {"b": "bool"}, [], ["G (b <-> a)"])
    smv = "MODULE main\nIVAR a : boolean;\nVAR b : boolean;\nASSIGN next(b) := a;\n"
    assert is_receptive(smv, c, "nuxmv/ic3").status == "receptive"


@needs_mc
def test_the_command_line(tmp_path, capsys):
    from chase_cps.cli import main
    spec = tmp_path / "spec.json"
    spec.write_text(json.dumps({"contracts": [
        {"name": "C", "inputs": {"a": "bool"}, "outputs": {"b": "bool"},
         "assumptions": ["G F a"], "guarantees": ["G (a -> b)"]}]}))
    assert main(["check", str(spec), "--engine", "bdd", "--json"]) == 0
    assert "nuxmv/bdd" in capsys.readouterr().out


@needs_mc
def test_casts_are_checked_with_bdds():
    # IC3 of nuXmv 2.2.0 finds this valid formula false
    v = {"x": "word[4]", "s": "signed word[4]"}
    r = implies("G X (s < signed(x))", "X G (s < signed(x))", v)
    assert r.status == "valid" and r.backend == "nuxmv/bdd"
    assert implies("G X (s < signed(x))", "X G (s < signed(x))", v, "nuxmv/portfolio").status \
        == "valid"
    r = implies("G X (s < signed(x))", "X G (s < signed(x))", v, "nuxmv/ic3")
    assert r.status == "unknown" and "signed()" in r.reason


@needs_mc
def test_variables_named_as_smv_keywords():
    v = {"count": (0, 3), "abs": (0, 5), "next": "bool", "E": "bool"}
    r = implies("G (count = 1 | abs > 2)", "count = 1", v)
    assert r.status == "invalid" and set((r.trace.prefix + r.trace.loop)[0]) == {"count", "abs"}
    assert is_satisfiable("next & !E & X !next", v).status == "sat"
