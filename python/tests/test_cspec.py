"""Contracts written in C (chase_cps.cspec): the clauses of ESBMC and of
CBMC read as contracts, each function checked against its own, the code
alone checked when a function has none, and a C file drawn."""

import pytest

from chase_cps import is_implementation, ltl
from chase_cps.backends import ADAPTERS
from chase_cps.cprograms import CProgramError
from chase_cps.cspec import has_clauses, load_c

TOOLS = [pytest.param(t, marks=pytest.mark.skipif(not ADAPTERS[t].available(),
                                                  reason=f"{t} not installed"))
         for t in ("esbmc", "cbmc")]

FILE = """/* A thermostat, a clamp, and a scale. */
int thermostat(int T)
{
    __ESBMC_requires(T >= 0 && T <= 200);
    __ESBMC_ensures((T < 74 && __ESBMC_return_value == 3) ||
                    (T >= 74 && __ESBMC_return_value == 0));
    if (T < 74)
        return 3;
    return 0;
}

void clamp(int x, int *y)
    __CPROVER_requires(x >= -100 && x <= 100)
    __CPROVER_requires(__CPROVER_is_fresh(y, sizeof(int)))
    __CPROVER_assigns(*y)
    __CPROVER_ensures(*y >= 0 && *y <= 50)
{
    *y = x < 0 ? 0 : (x > 50 ? 50 : x);
}

int scale(int a)
{
    return a * 2 / (a - 3);
}
"""


@pytest.fixture
def cfile(tmp_path):
    p = tmp_path / "parts.c"
    p.write_text(FILE)
    return str(p)


def terms(fs):
    return [ltl.to_str(f) for f in fs]


def test_the_clauses_are_the_contract(cfile):
    fs = load_c(cfile)
    assert list(fs) == ["thermostat", "clamp", "scale"]
    t, _ = fs["thermostat"]
    assert t.inputs == {"T": (0, 200)} and list(t.outputs) == ["thermostat_out"]
    assert terms(t.assumptions) == ["T >= 0", "T <= 200"]
    assert terms(t.guarantees) == ["(T < 74 & thermostat_out = 3) | (T >= 74 & thermostat_out = 0)"]
    c, _ = fs["clamp"]                   # *y is the output y; is_fresh and assigns are not terms
    assert c.inputs == {"x": (-100, 100)} and list(c.outputs) == ["y"]
    assert terms(c.assumptions) == ["x >= -100", "x <= 100"]
    assert terms(c.guarantees) == ["y >= 0", "y <= 50"]
    s, _ = fs["scale"]                   # no clause: no term, the range of the C type
    assert s.assumptions == [] and s.guarantees == []
    assert s.inputs == {"a": (-2 ** 31, 2 ** 31 - 1)}


def test_the_code_checked_has_no_clause(cfile):
    assert has_clauses(cfile)
    _, f = load_c(cfile)["clamp"]
    code = f.text()
    assert "__CPROVER" not in code and "__ESBMC" not in code
    assert len(code.split("\n")) == len(FILE.split("\n"))     # the lines stay where they are
    assert f.signature().parameters[1].pointer


def test_a_conditional_is_not_read(tmp_path):
    p = tmp_path / "t.c"
    p.write_text("int f(int x) { __ESBMC_ensures(x > 0 ? __ESBMC_return_value == 1 : 1); return 1; }")
    with pytest.raises(CProgramError, match="cannot read the clause"):
        load_c(str(p))


@pytest.mark.parametrize("tool", TOOLS)
def test_each_function_against_its_clauses(cfile, tool):
    fs = load_c(cfile)
    assert is_implementation(fs["thermostat"][1], fs["thermostat"][0], tool).status == "valid"
    assert is_implementation(fs["clamp"][1], fs["clamp"][0], tool).status == "valid"
    r = is_implementation(fs["scale"][1], fs["scale"][0], tool)     # the code alone
    assert r.status == "invalid" and "division by zero" in r.reason


@pytest.mark.parametrize("tool", TOOLS)
def test_a_wrong_clause_fails(tmp_path, tool):
    p = tmp_path / "wrong.c"
    p.write_text(FILE.replace("*y <= 50", "*y <= 40").replace("T < 74 &&", "T < 76 &&")
                 .replace("T >= 74 &&", "T >= 76 &&"))
    fs = load_c(str(p))
    assert is_implementation(fs["thermostat"][1], fs["thermostat"][0], tool).status == "invalid"
    assert is_implementation(fs["clamp"][1], fs["clamp"][0], tool).status == "invalid"


@pytest.mark.skipif(not ADAPTERS["esbmc"].available(), reason="esbmc not installed")
def test_a_c_file_is_drawn(cfile):
    from chase_cps import viz
    d = viz.from_spec(cfile, check=True)
    nodes = {n["title"]: n for n in d["nodes"]}
    assert set(nodes) == {"thermostat", "clamp", "scale"}
    assert nodes["thermostat"]["badge"] == {"text": "M ⊨ C", "status": "valid"}
    assert nodes["clamp"]["model"]["code"]["parts"][0]["text"].count("__CPROVER_") == 5   # as written
    s = nodes["scale"]["badge"]
    assert s["text"] == "safe" and s["status"] == "invalid"
    assert s["why"]["trace"]["steps"] == [[3]] and "division by zero" in s["why"]["reason"]
    assert nodes["scale"]["types"]["a"]["label"] == "int"


def test_negative_numbers_print_as_written():
    assert ltl.to_str(ltl.parse("x >= -100")) == "x >= -100"
    assert ltl.parse(ltl.to_str(ltl.parse("x - -3 >= -1"))) == ltl.parse("x - -3 >= -1")


def test_the_places_of_the_functions_and_clauses(cfile):
    from chase_cps import viz
    d = viz.from_spec(cfile)
    at = {n["title"]: n["source"] for n in d["nodes"]}
    assert at["thermostat"] == {"line": 2, "A": [[4, 4], [4, 4]], "G": [[5, 6]]}
    assert at["clamp"] == {"line": 12, "A": [[13, 13], [13, 13]], "G": [[16, 16], [16, 16]]}
    assert at["scale"]["line"] == 21
