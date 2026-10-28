"""The examples of the documentation run, and the names it gives exist.

Runs the Python examples of README.md, python/README.md (the page of the
package on PyPI), and docs/usage.md, reads the example
in the CHASE language, and checks the table of the operations in
docs/usage.md: each Python function or method, each option of chase-cps
check, and each command of the console of logics_tool (when it is
installed) must exist.
"""

import contextlib
import io
import os
import re
import shutil
import subprocess

import pytest

import chase_cps
from chase_cps import cli
from chase_cps.contract import Contract

ROOT = os.path.join(os.path.dirname(__file__), "..", "..")
USAGE = os.path.join(ROOT, "docs", "usage.md")
DOCS = [os.path.join(ROOT, "README.md"), os.path.join(ROOT, "python", "README.md"), USAGE]


def blocks(path):
    """The code blocks of a Markdown file (between lines of three or four
    backquotes)."""
    if not os.path.isfile(path):
        return []
    return [m.group(2) for m in
            re.finditer(r"^(```+)[^\n]*\n(.*?)^\1\s*$", open(path).read(), re.M | re.S)]


def python_blocks():
    out = []
    for path in DOCS:
        for i, b in enumerate(blocks(path)):
            if re.search(r"^(from chase_cps|import chase_cps)", b, re.M):
                out.append(pytest.param(b, id=f"{os.path.basename(path)}-{i}"))
    return out


def table(path):
    """The rows of the tables of the operations of usage.md (one per layer),
    as lists of cells, without the header rows."""
    rows, ours = [], False
    lines = open(path).read().splitlines()
    for i, line in enumerate(lines):
        header = i + 1 < len(lines) and lines[i + 1].startswith("|---")
        if header:      # the tables of the operations have a column Python
            ours = "| Python |" in line
        elif line.startswith("| ") and ours:
            rows.append([c.strip() for c in line.strip().strip("|").split("|")])
    return rows


def code(cell):
    return re.findall(r"`([^`]+)`", cell)


@pytest.mark.parametrize("text", python_blocks())
def test_python_examples_run(text, tmp_path, monkeypatch):
    needs = ("refines(", "verify(", "is_", "synthesize(", "remove_", "unsat_core(")
    if any(n in text for n in needs) and not (
            chase_cps.backends.ADAPTERS["nuxmv"].available()
            or chase_cps.backends.ADAPTERS["nusmv"].available()):
        pytest.skip("nuXmv/NuSMV not installed")
    game = ("synthesize(", "is_realizable(", "is_vacuous(", "is_well_separated(",
            "unrealizable_core(", "synth=True")
    if any(n in text for n in game) and not chase_cps.backends.ADAPTERS["slugs"].available():
        pytest.skip("slugs not installed")
    if "from_rtl(" in text:
        pytest.skip("needs the RTL files of the example (see the demos)")
    monkeypatch.chdir(tmp_path)
    with contextlib.redirect_stdout(io.StringIO()):
        exec(compile(text, "<doc>", "exec"), {})


def test_spec_example_reads():
    usage = USAGE
    if not os.path.isfile(usage):
        pytest.skip("docs/usage.md not found")
    try:
        from chase_cps import _native  # noqa: F401
    except ImportError:
        pytest.skip("CHASE modules not installed")
    specs = [b for b in blocks(usage) if b.startswith("NAME:")]
    assert specs
    for text in specs:
        assert chase_cps.parse_spec(text)


def test_table_python_names():
    usage = USAGE
    if not os.path.isfile(usage):
        pytest.skip("docs/usage.md not found")
    rows = table(usage)
    assert len(rows) > 15
    for row in rows:
        for call in code(row[2]):
            m = re.match(r"(\w+)\.(\w+)\(", call)
            if m:
                assert hasattr(Contract, m.group(2)), call
            else:
                name = re.match(r"(\w+)\(", call).group(1)
                assert name in chase_cps.__all__, call


def test_table_command_line_options():
    usage = USAGE
    if not os.path.isfile(usage):
        pytest.skip("docs/usage.md not found")
    out = io.StringIO()
    with contextlib.redirect_stdout(out), pytest.raises(SystemExit):
        cli.main(["check", "--help"])
    for row in table(usage):
        for option in code(row[3]):
            for flag in re.findall(r"--[\w-]+", option):
                assert flag in out.getvalue(), option


def test_table_console_commands(tmp_path):
    usage = USAGE
    tool = os.path.join(os.environ.get("CHASE_INSTALL_DIR", ""), "chase", "bin", "logics_tool")
    built = os.path.isfile(tool)    # the build under test, else one found in the PATH
    if not built:
        tool = shutil.which("logics_tool") or ""
    if not os.path.isfile(usage) or not os.path.isfile(tool):
        pytest.skip("docs/usage.md or logics_tool not found")
    spec = tmp_path / "s.ltl"
    spec.write_text("NAME: s;\nCONTRACT A:\n    output boolean variable x;\n"
                    "    Assumptions:\n        true;\n    Guarantees:\n        x;\n")
    script = tmp_path / "s.chase"
    script.write_text("help;\n")
    run = subprocess.run([tool, "-i", str(spec), "-o", str(tmp_path) + "/", "-c", str(script)],
                         capture_output=True, text=True, timeout=60)
    if run.returncode != 0 and not built:
        pytest.skip(f"{tool} does not run: {run.stderr.strip()[:200]}")
    assert run.returncode == 0, f"{tool}: {run.stderr}"
    out = run.stdout
    for row in table(usage):
        for command in code(row[1]):
            assert re.search(rf"^\s*{command.split()[0]}\b", out, re.M), command
