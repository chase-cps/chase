"""The diagrams of the files of the demos: each draws, each box and each term
knows where it is written, and a file broken as by typing (a line or a piece
of a line gone) gives a diagram or an error, never a crash.

The page itself, with a browser, is tested in test_viz_page.py."""

import glob
import json
import os
import random
import re
import subprocess
import sys

import pytest

from chase_cps import viz

try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False
try:
    from chase_cps.rtl import yosys as _yosys
    HAS_YOSYS = _yosys() is not None
except Exception:
    HAS_YOSYS = False

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DEMOS = os.path.join(ROOT, "demos")
pytestmark = [pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not installed"),
              pytest.mark.skipif(not os.path.isdir(DEMOS) or not os.listdir(DEMOS),
                                 reason="the demos are not checked out")]


def _files(pattern):
    return sorted(f for f in glob.glob(os.path.join(DEMOS, pattern), recursive=True))


SPECS = _files("**/*.ltl")
# SystemVerilog with assertions; the others are RTL
SVA = [f for f in _files("**/*.sv") if re.search(r"\b(assert|assume)\s+property\b", open(f).read())]


def _id(f):
    return os.path.relpath(f, DEMOS)


def _code(line):
    return re.sub(r"(//|#).*", "", line).strip()


def _blocks(lines, head):
    """{name: (first, last)}: the lines of each contract or module."""
    starts = [(i + 1, m.group(1)) for i, l in enumerate(lines) for m in [re.match(head, l)] if m]
    return {name: (a, (starts[k + 1][0] - 1) if k + 1 < len(starts) else len(lines))
            for k, (a, name) in enumerate(starts)}


def _check_places(path, d, head, words):
    lines = open(path, encoding="utf-8").read().splitlines()
    blocks = _blocks(lines, head)
    checked = 0
    for n in d["nodes"]:
        c, src = n.get("contract"), n.get("source")
        if not c or c["name"] not in blocks:
            continue
        assert src, f"{c['name']}: no line"
        first, last = blocks[c["name"]]
        assert src["line"] == first
        for p in "AG":
            assert p in src, f"{c['name']}: the terms of {p} have no lines"
            assert len(src[p]) == len(c[p])
            for a, b in src[p]:
                assert first < a <= b <= last, f"{c['name']} {p}: lines {a}-{b} outside {first}-{last}"
                assert _code(lines[a - 1]), f"{c['name']} {p}: line {a} is empty"
                assert ";" in lines[b - 1] or ")" in lines[b - 1]
            checked += len(src[p])
        # the words of a term are on its lines (constants are written as values)
        for p in "AG":
            for t, (a, b) in zip(c[p], src[p]):
                text = " ".join(lines[a - 1:b])
                for w in re.findall(words, t):
                    assert w in text or w.isdigit() or w in ("true", "false"), f"{w} of {t} not in lines {a}-{b}"
    return checked


@pytest.mark.parametrize("path", SPECS, ids=_id)
def test_each_term_of_a_spec_is_placed(path, monkeypatch):
    monkeypatch.chdir(os.path.dirname(path))
    d = viz.from_spec(path)
    assert _check_places(path, d, r"\s*CONTRACT\s+([A-Za-z_]\w*)", r"[A-Za-z_]\w{2,}") > 0


@pytest.mark.parametrize("path", SVA, ids=_id)
def test_each_assertion_is_placed(path, monkeypatch):
    monkeypatch.chdir(os.path.dirname(path))
    d = viz.from_spec(path)
    # SVA terms are written in LTL: only their count and lines are compared
    assert _check_places(path, d, r"\s*module\s+([A-Za-z_]\w*)", r"(?!)") > 0


def _broken(text, rng):
    """text as it is while one types: a line gone, or the end of a line."""
    lines = text.splitlines()
    k = rng.randrange(len(lines))
    if rng.random() < 0.5:
        del lines[k]
    else:
        lines[k] = lines[k][:rng.randrange(len(lines[k]) + 1)]
    return "\n".join(lines) + "\n"


@pytest.mark.parametrize("path", SPECS, ids=_id)
def test_a_spec_broken_by_typing_never_crashes(path, tmp_path):
    """A broken file gives a diagram or an error with its line, in a process
    of its own: a crash of the parser would stop the tests. More cases:
    CHASE_FUZZ=N runs N per file."""
    rng = random.Random(_id(path))
    text = open(path, encoding="utf-8").read()
    env = dict(os.environ, PYTHONPATH=os.pathsep.join(sys.path))
    for i in range(int(os.environ.get("CHASE_FUZZ", "3"))):
        spec = tmp_path / os.path.basename(path)
        spec.write_text(_broken(text, rng))
        out = tmp_path / "d.json"
        p = subprocess.run([sys.executable, "-m", "chase_cps", "view", str(spec), "--format", "json",
                            "--no-checks", "-o", str(out)], cwd=os.path.dirname(path), env=env,
                           capture_output=True, text=True, timeout=120)
        case = f"case {i} of {_id(path)}:\n{p.stderr[-2000:]}"
        assert p.returncode in (0, 1, 2), case
        assert "Traceback" not in p.stderr, case
        if p.returncode == 0:
            json.loads(out.read_text())
        else:
            assert p.stderr.strip().splitlines()[-1].startswith("chase-cps view:"), case


@pytest.mark.skipif(not HAS_YOSYS, reason="Yosys not installed")
@pytest.mark.parametrize("path", [f for f in _files("**/*.v")], ids=_id)
def test_a_module_is_drawn_at_its_line(path):
    d = viz.from_spec(path)
    lines = open(path).read().splitlines()
    for n in d["nodes"]:
        assert re.match(r"\s*module\s+" + re.escape(n["title"]) + r"\b", lines[n["source"]["line"] - 1])
