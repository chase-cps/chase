"""Tests of the state machines (chase_cps.models): is_implementation and is_environment of a
model, and receptiveness. Skipped without nuXmv or NuSMV (and slugs and the
CHASE modules for the controllers)."""

import json
import os

import pytest

from chase_cps import Contract, Model, is_environment, is_implementation, is_receptive
from chase_cps.backends import ADAPTERS

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_SLUGS = ADAPTERS["slugs"].available()
except ImportError:
    HAS_SLUGS = False

needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
needs_slugs = pytest.mark.skipif(not (HAS_MC and HAS_SLUGS),
                                 reason="slugs, nuXmv/NuSMV and the CHASE modules are needed")

CHARGER = os.path.join(os.environ.get("CHASE_DEMOS", os.path.join(
    os.path.dirname(__file__), "..", "..", "demos")), "Charger", "charger.ltl")

SPEC = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Spec",
                             guarantees=["G (req -> X grant)"])

# grant follows the request; the second TRANS refuses a request while granting
BLOCKING = """MODULE main
IVAR
  req : boolean;
VAR
  grant : boolean;
ASSIGN
  init(grant) := FALSE;
TRANS
  next(grant) = req
TRANS
  !(grant & req)
"""
GOOD = BLOCKING.replace("TRANS\n  !(grant & req)\n", "")

MACHINE = {"name": "Slow", "inputs": {"req": "bool"}, "outputs": {"grant": "bool"},
           "states": ["idle", "busy"], "initial": "idle",
           "transitions": [
               {"from": "idle", "when": "req", "outputs": {"grant": False}, "to": "busy"},
               {"from": "idle", "when": "!req", "outputs": {"grant": False}, "to": "idle"},
               {"from": "busy", "outputs": {"grant": True}, "to": "idle"}]}


def test_smv_declarations():
    m = Model.from_smv(BLOCKING, name="B")
    assert m.variables == {"req": "bool", "grant": "bool"} and m.ivars == {"req"}
    with pytest.raises(ValueError):
        Model.from_smv(GOOD + "LTLSPEC G grant\n")
    with pytest.raises(ValueError):
        Model.from_smv("VAR x : boolean;")
    with pytest.raises(ValueError):
        Model.from_dict({**MACHINE, "initial": "nowhere"})
    with pytest.raises(ValueError):
        Model.from_dict({**MACHINE, "transitions": [{"from": "idle", "to": "idle",
                                                      "outputs": {"req": True}}]})


@needs_mc
def test_a_model_that_blocks_the_inputs_implements_the_contract_but_is_not_receptive():
    m = Model.from_smv(BLOCKING, name="Blocking")
    assert is_implementation(m, SPEC).status == "valid"
    r = is_receptive(m, SPEC)
    assert r.status == "not_receptive" and not r
    assert r.details["input"] == {"req": True}
    assert r.details["state"] == {"req": True, "grant": False}
    assert r.trace.prefix[-1] == {"req": True, "grant": False}
    good = Model.from_smv(GOOD, name="Good")
    assert is_implementation(good, SPEC).status == "valid"
    assert is_receptive(good, SPEC).status == "receptive"
    # SMV text is a model too
    assert is_implementation(GOOD, SPEC).status == "valid"


@needs_mc
def test_a_state_machine_in_json():
    m = Model.from_dict(MACHINE)
    r = is_implementation(m, SPEC)
    assert r.status == "invalid" and r.trace is not None    # a request while busy
    assert is_receptive(m, SPEC).status == "receptive"
    spaced = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Spaced",
                                   assumptions=["G (req -> X !req)"],
                                   guarantees=["G (req -> X grant)"])
    assert is_implementation(m, spaced).status == "valid"
    assert is_implementation(MACHINE, spaced).status == "valid"
    # without the transition for !req in idle, the machine refuses !req there
    partial = {**MACHINE, "transitions": [t for t in MACHINE["transitions"]
                                          if t.get("when") != "!req"]}
    r = is_receptive(partial, SPEC)
    assert r.status == "not_receptive" and r.details["input"] == {"req": False}


@needs_mc
def test_an_environment_model():
    env = Model.from_smv("MODULE main\nVAR\n  req : boolean;\n  grant : boolean;\n"
                         "ASSIGN\n  init(req) := FALSE;\n  next(req) := !req;\n")
    spaced = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Spaced",
                                   assumptions=["G (req -> X !req)"], guarantees=["G grant"])
    assert is_environment(env, spaced).status == "valid"
    never = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Never",
                                  assumptions=["G !req"])
    r = is_environment(env, never)
    assert r.status == "invalid" and r.trace is not None
    with pytest.raises(ValueError):
        is_environment(Model.from_smv("MODULE main\nVAR\n  x : boolean;\n"), never)


@needs_mc
def test_the_types_of_the_model_and_the_contract():
    real = Model.from_smv("MODULE main\nVAR\n  x : real;\n  hot : boolean;\nINIT x = 0\n"
                          "TRANS next(x) >= x & next(x) <= x + 1\nINVAR hot <-> x > 10\n")
    boolean = Contract.from_strings(inputs=["x"], outputs=["hot"], name="Heat",
                                    guarantees=["G (hot -> x > 5)"])
    with pytest.raises(ValueError):
        is_implementation(real, boolean)
    heat = Contract.from_dict({"name": "Heat", "inputs": {"x": "real"},
                               "outputs": {"hot": "bool"}, "guarantees": ["G (hot -> x > 5)"]})
    if ADAPTERS["nuxmv"].available():
        assert is_implementation(real, heat).status == "valid"
    assert is_receptive(real, heat).status == "unknown"


@needs_slugs
def test_the_controller_of_fair_implements_it_and_is_receptive():
    from chase_cps import load_spec, synthesize
    c = load_spec(CHARGER)
    fair = c["Fair"]
    r = synthesize(fair)
    assert r.status == "realizable" and r.details["init"] == ["!grant1", "!grant2"]
    m = Model.from_controller(r)
    assert m.variables["chase_node"] == (0, len(r.controller["nodes"]) - 1)
    assert is_implementation(m, fair).status == "valid"
    assert is_implementation(r, fair).status == "valid"
    assert is_implementation(r, c["System"]).status == "valid"
    assert is_receptive(m, fair).status == "receptive"
    # the controller as JSON, with the contract it was synthesized from
    data = json.loads(json.dumps(r.controller))
    assert is_implementation(Model.from_controller(data, fair), fair).status == "valid"
    with pytest.raises(ValueError):
        Model.from_controller(data)


@needs_slugs
def test_a_controller_refuses_the_inputs_that_its_assumptions_exclude():
    from chase_cps import synthesize
    arb = Contract.from_strings(
        inputs=["req"], outputs=["grant"], name="Arb",
        assumptions=["!req", "G ((req & grant) -> X !req)", "G ((req & !grant) -> X req)"],
        guarantees=["!grant", "G (grant -> req)", "G F (!req | grant)"])
    r = synthesize(arb)
    assert is_receptive(r, arb).status == "receptive"
    free = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Free",
                                 guarantees=["G (grant -> req)"])
    q = is_receptive(r, free)
    assert q.status == "not_receptive" and q.details["input"] == {"req": False}
    assert q.details["state"]["req"] is True and q.details["state"]["grant"] is False


@needs_slugs
def test_a_controller_with_integers():
    from chase_cps import synthesize
    c = Contract.from_dict({"name": "I", "inputs": {"n": [1, 6]},
                            "outputs": {"o": "bool", "k": [0, 2]},
                            "guarantees": ["G ((n > 4) -> o)", "G ((n < 2) -> (k = 2))",
                                           "G (o -> (k = 0))"]})
    r = synthesize(c)
    m = Model.from_controller(r)
    assert m.variables["n"] == (1, 6) and m.variables["k"] == (0, 2)
    assert is_implementation(m, c).status == "valid" and is_receptive(m, c).status == "receptive"
    wrong = Contract.from_dict({"name": "Wrong", "inputs": {"n": [1, 6]},
                                "outputs": {"o": "bool", "k": [0, 2]},
                                "guarantees": ["G ((n > 4) -> (k = 1))"]})
    assert is_implementation(m, wrong).status == "invalid"


@needs_slugs
def test_the_command_line(tmp_path, capsys):
    from chase_cps.cli import main
    (tmp_path / "blocking.smv").write_text(BLOCKING)
    (tmp_path / "slow.json").write_text(json.dumps(MACHINE))
    spec = tmp_path / "spec.json"
    spec.write_text(json.dumps({"contracts": [
        SPEC.to_dict(),
        {"name": "Trusting", "inputs": {"req": "bool"}, "outputs": {"grant": "bool"},
         "assumptions": ["G F !req"], "guarantees": ["G F (!req | grant)"]}]}))
    assert main(["check", str(spec), "--implementation", str(tmp_path / "blocking.smv"),
                 "Spec"]) == 0
    assert main(["check", str(spec), "--receptive", str(tmp_path / "blocking.smv"),
                 "Spec"]) == 1
    assert "refuses the input" in capsys.readouterr().out
    assert main(["check", str(spec), "--implementation", str(tmp_path / "slow.json"),
                 "Spec"]) == 1
    capsys.readouterr()
    assert main(["check", str(spec), "--vacuous", "Trusting", "--core", "--json"]) == 1
    report = json.loads(capsys.readouterr().out)
    assert report["checks"][0]["vacuous"][0]["core"]["core"] == ["G F !req"]
    assert main(["check", str(spec), "--implementation", str(tmp_path / "none.smv"), "Spec"]) == 2
