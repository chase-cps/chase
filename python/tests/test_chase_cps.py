"""Tests of the chase_cps package.

The tests needing a model checker (nuXmv or NuSMV), slugs, or the compiled
CHASE modules are skipped when those are not installed.
"""

import os

import pytest

from chase_cps import ltl
from chase_cps.backends import ADAPTERS
from chase_cps.contract import Contract

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False

needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
needs_native = pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not installed")

ROBOTIC_JOINTS = os.environ.get(
    "CHASE_DEMOS", os.path.join(os.path.dirname(__file__), "..", "..", "demos"))
ROBOTIC_JOINTS = os.path.join(ROBOTIC_JOINTS, "RoboticJoints", "specs.ltl")


# LTL parser and printers ------------------------------------------------------

@pytest.mark.parametrize("text, chase, smv", [
    ("a -> b & c", "(a -> (b & c))", "(a -> (b & c))"),
    ("a & b -> c", "((a & b) -> c)", "((a & b) -> c)"),
    ("a -> b -> c", "(a -> (b -> c))", "(a -> (b -> c))"),
    ("[]<>p", "(G ((F (p))))", "G (F (p))"),
    ("x' = x + 1", "(x' = (x + 1))", "(next(x) = (x + 1))"),
    ("a R b", "(a R b)", "(a V b)"),
    ("!a U b | c", "(((! (a)) U b) | c)", "((! (a) U b) | c)"),
    ("TRUE -> X false", "(true -> (X (false)))", "(TRUE -> X (FALSE))"),
])
def test_parse_and_print(text, chase, smv):
    node = ltl.parse(text)
    assert ltl.to_chase(node) == chase
    assert ltl.to_smv(node) == smv
    assert ltl.parse(ltl.to_str(node)) == node     # round trip


@pytest.mark.parametrize("bad", ["", "a & & b", "(a", "G", "a b", "x @ y"])
def test_syntax_errors(bad):
    with pytest.raises(ltl.LTLSyntaxError):
        ltl.parse(bad)


def test_weak_until_expands():
    assert ltl.parse("a W b") == ltl.parse("(a U b) | G a")


# Contracts ---------------------------------------------------------------------

def test_from_strings_and_dict_round_trip():
    c = Contract.from_strings(inputs={"req": "bool", "n": (0, 3)}, outputs=["grant"],
                              assumptions="G F !req & n <= 2",
                              guarantees=["G (req -> F grant)"], name="Arbiter")
    assert len(c.assumptions) == 2          # top-level conjunction is split
    assert Contract.from_dict(c.to_dict()) == c
    assert "integer (0:3) variable n" in c.to_logics()


def test_undeclared_variable_is_rejected():
    with pytest.raises(ValueError, match="undeclared"):
        Contract.from_strings(inputs=["a"], guarantees=["G b"])


@pytest.mark.parametrize("name", ["P", "G", "input", "1x", "a-b"])
def test_reserved_or_bad_names_are_rejected(name):
    with pytest.raises(ValueError, match="cannot be used as a name"):
        Contract.from_strings(inputs=[name], name="Ok")


def test_input_and_output_clash_is_rejected():
    with pytest.raises(ValueError, match="both input and output"):
        Contract.from_strings(inputs=["a"], outputs=["a"])


# Checks (model checker needed) ----------------------------------------------------

V = {"req": "bool", "grant": "bool", "x": (0, 4)}


@needs_mc
def test_implies_valid_and_invalid_with_counterexample():
    from chase_cps import implies
    assert implies("G (req -> X grant)", "G (req -> F grant)", V).status == "valid"
    r = implies("G (req -> F grant)", "G (req -> X grant)", V)
    assert r.status == "invalid"
    states = r.trace.prefix + r.trace.loop
    # The counterexample violates req -> X grant somewhere.
    assert any(s["req"] and not states[i + 1]["grant"]
               for i, s in enumerate(states[:-1])) or r.trace.loop


@needs_mc
def test_primed_variables_mean_next_step():
    from chase_cps import implies
    assert implies("x = 0 & G (x < 4 -> x' = x + 1)", "F x = 4", V).status == "valid"
    assert implies("x = 0 & G (x' = x)", "F x = 4", V).status == "invalid"


@needs_mc
def test_satisfiable():
    from chase_cps import satisfiable
    assert satisfiable("G req & F !req", V).status == "unsat"
    r = satisfiable("G F req & G F !req", V)
    assert r.status == "sat" and r.trace.loop


@needs_mc
def test_refinement_consistency_compatibility():
    from chase_cps import compatible, consistent, refines
    spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 assumptions=["G F !req"],
                                 guarantees=["G (req -> F grant)"], name="Spec")
    impl = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 guarantees=["G (req -> X grant)"], name="Impl")
    assert refines(impl, spec).status == "valid"
    r = refines(spec, impl)     # impl accepts every environment, spec does not
    assert r.status == "invalid" and r.failed == "assumptions"
    assert consistent(spec).status == "sat"
    assert compatible(spec).status == "sat"
    bad = Contract.from_strings(inputs=["req"], assumptions=["G req", "F !req"], name="Bad")
    assert compatible(bad).status == "unsat"


@needs_mc
def test_remove_redundant():
    from chase_cps import remove_redundant
    c = Contract.from_strings(inputs=["req"], outputs=["grant"],
                              assumptions=["G F !req", "F !req"],
                              guarantees=["G (req -> X grant)", "G (req -> F grant)",
                                          "G F (req -> grant) | !req"],
                              name="Red")
    out = remove_redundant(c)
    assert [ltl.to_str(f) for f in out.removed_assumptions] == ["F !req"]
    assert "G (req -> F grant)" in [ltl.to_str(f) for f in out.removed_guarantees]
    assert out.contract.guarantees[0] == ltl.parse("G (req -> X grant)")


@needs_mc
def test_backends_answer_or_report_unknown():
    from chase_cps import check, ltl as L
    from chase_cps.queries import Project
    r = check(Project(L.parse("a & b"), {"a": "bool", "b": "bool"}, ["a"]))
    assert r.status == "unknown" and "Project" in r.reason


# CHASE core library (compiled modules needed) ------------------------------------

@needs_native
def test_to_pychase_and_back():
    c = Contract.from_strings(inputs={"req": "bool", "n": (0, 3)}, outputs=["grant"],
                              assumptions=["G F !req"],
                              guarantees=["G (req -> F grant)", "n' = n"], name="K")
    back = Contract.from_pychase(c.to_pychase())
    assert back.variables == c.variables
    assert back.guarantees == c.guarantees     # primes survive the round trip


@needs_native
@pytest.mark.parametrize("formula", [
    "G a -> G c", "G F !a -> G F c", "a U c & a", "!G a | X (a -> c)",
    "F a <-> G (a -> X c)", "(a -> c) & G a", "a -> c & a",
])
def test_grouping_survives_the_chase_parser(formula):
    # CHASE gives G/F/X a lower precedence than the binary operators.
    c = Contract.from_strings(inputs=["a"], outputs=["c"], guarantees=[formula], name="Prop")
    back = Contract.from_pychase(c.to_pychase())
    assert ltl.conjunction(back.guarantees) == ltl.conjunction(c.guarantees)


@needs_native
def test_bad_formula_raises_instead_of_exiting():
    from chase_cps.contract import _parse_contracts
    c = Contract(name="X", inputs={"a": "bool"}, guarantees=[ltl.Var("b")])
    with pytest.raises(RuntimeError):
        _parse_contracts([c])


@needs_native
@needs_mc
def test_composition_refines_and_separation():
    from chase_cps import refines
    c1 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                               guarantees=["G (a -> b)"], name="C1")
    c2 = Contract.from_strings(inputs=["b"], outputs=["c"], assumptions=["G b"],
                               guarantees=["G (b -> c)"], name="C2")
    top = Contract.from_strings(inputs=["a"], outputs=["c"], assumptions=["G a"],
                                guarantees=["G c"], name="Top")
    assert refines(c1.compose(c2), top).status == "valid"
    # separation: Top is refined by (Top / sep C2) merged with C2
    s = top.separation(c2)
    assert refines(top, s.merge(c2)).status == "valid"


@needs_native
@needs_mc
def test_quotient_composed_with_divisor_refines():
    from chase_cps import refines
    # C2 || (C1 / C2) <= C1, also when A2 is not implied by A1 and G2.
    c1 = Contract.from_strings(inputs=["a"], outputs=["c"], assumptions=["G a"],
                               guarantees=["G c"], name="C1")
    c2 = Contract.from_strings(inputs=["b"], outputs=["c"], assumptions=["G b"],
                               guarantees=["G (b -> c)"], name="C2")
    q = c1.quotient(c2)
    assert refines(c2.compose(q), c1).status == "valid"


@needs_native
@needs_mc
@pytest.mark.skipif(not os.path.isfile(ROBOTIC_JOINTS), reason="demos not found")
def test_robotic_joints_refinement_and_counterexample():
    from chase_cps import refines
    from chase_cps._native import pychase_logicsLang
    system = pychase_logicsLang.LogicsSpecsBuilder().parseSpecificationFile(ROBOTIC_JOINTS)
    C = {c.getName().getString(): Contract.from_pychase(c) for c in system.getContractsSet()}
    composed = C["Joint3"].compose(C["Joint1"].compose(C["Joint2"]))
    nuxmv = (os.environ.get("CHASE_BACKEND", "nuxmv") != "nusmv"
             and ADAPTERS["nuxmv"].available())
    if nuxmv:   # IC3 proves it in seconds
        assert refines(composed, C["Joints"]).status == "valid"
    else:       # NuSMV proves it with BDDs, which take minutes
        assert refines(composed, C["Joints"], timeout=60).status in ("valid", "unknown")
    j1 = C["Joint1"]
    weak = Contract(j1.name, j1.inputs, j1.outputs, j1.assumptions,
                    [g for g in j1.guarantees if ltl.to_str(g) != "G F j1_done"])
    r = refines(C["Joint3"].compose(weak.compose(C["Joint2"])), C["Joints"])
    assert r.status == "invalid" and r.failed == "guarantees"
    assert all(s["req_j1"] and not s["j1_done"] for s in r.guarantees.trace.loop)


def _has_slugs():
    try:
        from chase_cps.backends import SlugsAdapter
        return SlugsAdapter().available()
    except ImportError:
        return False


@needs_native
@pytest.mark.skipif(not _has_slugs(), reason="slugs not installed")
def test_realizability_with_controller_and_counter_strategy():
    from chase_cps import realizable
    ok = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G F !req"],
                               guarantees=["G (req -> X grant)", "G F grant"], name="Arb")
    r = realizable(ok)
    assert r.status == "realizable" and "nodes" in r.controller
    bad = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                guarantees=["G (req -> X grant)", "G (req -> X !grant)"],
                                name="Bad")
    r = realizable(bad)
    assert r.status == "unrealizable" and "State" in r.counter_strategy


def test_rename_and_signatures():
    c = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                              guarantees=["G (a -> X b')"], name="C")
    r = c.rename({"a": "x", "b": "y"})
    assert r.inputs == {"x": "bool"} and r.outputs == {"y": "bool"}
    assert r.guarantees == [ltl.parse("G (x -> X y')")]
    assert not c.shares_io_with(r) and c.shares_io_with(c)
    d = Contract.from_strings(inputs=["b"], outputs=["c"], name="D")
    e = Contract.from_strings(outputs=["b"], name="E")
    assert c.can_compose_with(d) and not c.can_compose_with(e)
    with pytest.raises(ValueError, match="outputs of both"):
        c.compose(e)


@needs_mc
def test_environment_and_implementation():
    from chase_cps import contains_environment, contains_implementation
    spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                                 assumptions=["G F !req"],
                                 guarantees=["G (req -> F grant)"], name="Spec")
    assert contains_environment(spec, "G (req -> X !req)").status == "valid"
    assert contains_environment(spec, "G req").status == "invalid"
    assert contains_implementation(spec, "G (grant <-> X req)").status == "invalid"
    assert contains_implementation(spec, "G grant").status == "valid"



@needs_native
@needs_mc
def test_conjunction_and_disjunction_bounds():
    from chase_cps import refines
    c1 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G a"],
                               guarantees=["G b"], name="C1")
    c2 = Contract.from_strings(inputs=["a"], outputs=["b"], assumptions=["G F a"],
                               guarantees=["F b"], name="C2")
    meet, join = c1.conjoin(c2), c1.disjoin(c2)
    for c in (c1, c2):
        assert refines(meet, c).status == "valid"
        assert refines(c, join).status == "valid"


@needs_native
def test_spec_text_round_trip():
    from chase_cps import parse_spec, to_spec
    c = Contract.from_strings(inputs={"req": "bool", "n": (0, 3)}, outputs=["grant"],
                              assumptions=["G F !req"],
                              guarantees=["G (req -> F grant)", "G (n < 3)"], name="Arbiter")
    back = parse_spec(to_spec([c]))["Arbiter"]
    assert back.inputs == c.inputs and back.outputs == c.outputs
    assert [ltl.to_str(f) for f in back.guarantees] == [ltl.to_str(f) for f in c.guarantees]


@needs_mc
def test_saturate_gives_an_equivalent_contract():
    from chase_cps import refines
    c = Contract.from_strings(inputs=["req"], outputs=["grant"], assumptions=["G F !req"],
                              guarantees=["G (req -> F grant)"], name="C")
    s = c.saturate()
    assert ltl.to_str(s.guarantee) == "G F !req -> G (req -> F grant)"
    assert refines(c, s).status == "valid" and refines(s, c).status == "valid"


def test_doctor_lists_the_back_ends(capsys):
    from chase_cps.cli import main
    code = main(["doctor"])
    out = capsys.readouterr().out
    assert "Back ends:" in out and "nuxmv" in out and "slugs" in out
    assert code in (0, 1)


ARBITER_SPEC = """NAME: arbiter;

CONTRACT Spec:
    input boolean variable req;
    output boolean variable grant;
    Assumptions:
        G(F(!(req)));
    Guarantees:
        G(req -> F(grant));

CONTRACT Impl:
    input boolean variable req;
    output boolean variable grant;
    Assumptions:
        true;
    Guarantees:
        G(req -> X(grant));
"""


@needs_native
@needs_mc
def test_check_command(tmp_path, capsys):
    import json
    from chase_cps.cli import main
    spec = tmp_path / "arbiter.ltl"
    spec.write_text(ARBITER_SPEC)
    assert main(["check", str(spec)]) == 0
    out = capsys.readouterr().out
    assert out.count("sat") == 4
    assert main(["check", str(spec), "--refines", "Impl", "Spec"]) == 0
    assert "valid" in capsys.readouterr().out
    assert main(["check", str(spec), "--refines", "Spec", "Impl", "--json"]) == 1
    report = json.loads(capsys.readouterr().out)
    assert not report["passed"] and report["checks"][0]["failed"] == "assumptions"
    assert main(["check", str(spec), "--refines", "Nope", "Spec"]) == 2
    contracts = tmp_path / "arbiter.json"
    contracts.write_text(json.dumps({"contracts": [
        {"name": "A", "inputs": {"x": "bool"}, "assumptions": ["G x", "F !x"]}]}))
    assert main(["check", str(contracts), "--compatible"]) == 1


def test_install_refusals(monkeypatch):
    from chase_cps import tools
    monkeypatch.setattr(tools, "this_platform", lambda: ("Linux", "aarch64"))
    with pytest.raises(tools.InstallError, match="no nuxmv build for Linux aarch64"):
        tools.install("nuxmv", accept_license=True)
    monkeypatch.setattr(tools, "this_platform", lambda: ("Darwin", "arm64"))
    with pytest.raises(tools.InstallError, match="install nuXmv instead"):
        tools.install("nusmv")
    with pytest.raises(tools.InstallError, match="--accept-license"):
        tools.install("nuxmv")


def test_install_downloads_checks_and_links(tmp_path, monkeypatch):
    import hashlib
    import tarfile
    from chase_cps import tools
    exe = tmp_path / "NuSMV-x" / "bin" / "NuSMV"
    exe.parent.mkdir(parents=True)
    exe.write_text("#!/bin/sh\n")
    exe.chmod(0o755)
    archive = tmp_path / "nusmv.tar.gz"
    with tarfile.open(archive, "w:gz") as tar:
        tar.add(tmp_path / "NuSMV-x", arcname="NuSMV-x")
    digest = hashlib.sha256(archive.read_bytes()).hexdigest()
    monkeypatch.setenv("CHASE_HOME", str(tmp_path / "home"))
    monkeypatch.setattr(tools, "this_platform", lambda: ("Linux", "x86_64"))
    monkeypatch.setattr(tools, "_verify", lambda tool, path, log: path)
    monkeypatch.setitem(tools.NUSMV, ("Linux", "x86_64"),
                        (archive.as_uri(), digest, "NuSMV-x/bin/NuSMV"))
    path = tools.install("nusmv", log=lambda *a: None)
    assert path == str(tmp_path / "home" / "bin" / "NuSMV")
    assert os.path.realpath(path) == str(tmp_path / "home" / "tools" / "nusmv" / "NuSMV-x" / "bin" / "NuSMV")
    assert tools.find("NuSMV")
    monkeypatch.setitem(tools.NUSMV, ("Linux", "x86_64"),
                        (archive.as_uri(), "0" * 64, "NuSMV-x/bin/NuSMV"))
    with pytest.raises(tools.InstallError, match="SHA-256"):
        tools.install("nusmv", log=lambda *a: None)


@needs_native
@needs_mc
def test_simplify_gives_an_equivalent_contract():
    from chase_cps import refines
    c = Contract.from_strings(inputs=["a"], outputs=["b", "d"], assumptions=["G a", "G !(!a)"],
                              guarantees=["!(!b | X d)", "F b | F d"], name="C")
    s = c.simplify()
    assert [ltl.to_str(f) for f in s.guarantees] == ["b", "X !d", "F (b | d)"]
    assert s.inputs == c.inputs and s.outputs == c.outputs
    assert refines(c, s).status == "valid" and refines(s, c).status == "valid"
