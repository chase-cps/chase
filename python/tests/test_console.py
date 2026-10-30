"""The console of logics_tool: commands that used to crash or to lose a part
of a formula. The tests run the logics_tool of the build under test
($CHASE_INSTALL_DIR/chase/bin), else the one in the PATH."""

import os
import shutil
import subprocess

import pytest

SPEC = """NAME: s;

CONTRACT A:
    input integer (0:4) variable x;
    input integer (0:4) variable y;
    output boolean variable b;
    Assumptions:
        true;
    Guarantees:
        G[0,2] (b);
        G (b -> x = y);

CONTRACT B:
    input boolean variable b;
    output boolean variable c;
    Assumptions:
        true;
    Guarantees:
        F[1,3] (c);
"""


def logics_tool():
    tool = os.path.join(os.environ.get("CHASE_INSTALL_DIR", ""), "chase", "bin", "logics_tool")
    if not os.path.isfile(tool):
        tool = shutil.which("logics_tool") or ""
    if not os.path.isfile(tool):
        pytest.skip("logics_tool not found")
    return tool


def run(tmp_path, commands):
    spec = tmp_path / "s.ltl"
    spec.write_text(SPEC)
    script = tmp_path / "s.chase"
    script.write_text("".join(c + ";\n" for c in commands))
    r = subprocess.run([logics_tool(), "-i", str(spec), "-o", str(tmp_path) + "/", "-c", str(script)],
                       capture_output=True, text=True, timeout=60)
    if r.returncode not in (0, 1) and "dyld" in r.stderr:
        pytest.skip(f"logics_tool does not run: {r.stderr.strip()[:200]}")
    return r


def test_commands_without_their_contract_do_not_crash(tmp_path):
    r = run(tmp_path, ["stl nosuch", "stl", "synthesize", "synthesize nosuch out"])
    assert r.returncode == 0, r.stderr
    assert "No contract named nosuch" in r.stdout + r.stderr


def test_an_interval_survives_the_operations(tmp_path):
    r = run(tmp_path, ["compose A B AB", "stl AB out"])     # the composition clones the formulas
    assert r.returncode == 0, r.stderr
    text = (tmp_path / "out.py").read_text()
    assert "G[0, 2]" in text and "F[1, 3]" in text


def test_gr1c_is_not_given_a_relation_it_cannot_read(tmp_path):
    r = run(tmp_path, ["synthesize A a gr1c"])
    assert r.returncode == 0, r.stderr
    assert not (tmp_path / "a.spc").exists()
    assert "was not written" in r.stdout + r.stderr
