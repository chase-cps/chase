"""An SMV model with its specifications (chase_cps.smvspec): the module
main as a component, its LTLSPEC and INVARSPEC as the guarantees, the
model without them as the implementation, and the file drawn."""

import pytest

from chase_cps import is_implementation, ltl
from chase_cps.backends import ADAPTERS
from chase_cps.smvspec import has_specs, load_smv, places

NUXMV = pytest.mark.skipif(not (ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()),
                           reason="nuXmv or NuSMV not installed")

RELAY = """-- A relay: the grant follows the request one step later.
MODULE main
IVAR
  req : boolean;
VAR
  grant : boolean;
  n : 0..3;
ASSIGN
  init(grant) := FALSE;
  next(grant) := req;
  init(n) := 0;
  next(n) := (n < 3 & req) ? n + 1 : n;
LTLSPEC G (req -> X grant)
LTLSPEC G (n <= 3) & F G (n <= 3)
INVARSPEC !(grant & n = 4)
LTLSPEC NAME live := G F grant
"""


@pytest.fixture
def smv(tmp_path):
    p = tmp_path / "relay.smv"
    p.write_text(RELAY)
    return str(p)


def test_the_specifications_are_the_guarantees(smv):
    assert has_specs(smv)
    c, m = load_smv(smv)["relay"]
    assert c.inputs == {"req": "bool"} and c.outputs == {"grant": "bool", "n": (0, 3)}
    assert [ltl.to_str(g) for g in c.guarantees] == [
        "G (req -> X grant)", "G (n <= 3)", "F G (n <= 3)", "G !(grant & n = 4)", "G F grant"]
    assert "SPEC" not in m.smv and m.variables["n"] == (0, 3)      # the model has none
    assert places(smv) == ({"relay": 2}, {"relay": {"A": [], "G": [(13, 13), (14, 14), (14, 14),
                                                                  (15, 15), (16, 16)]}})


@NUXMV
def test_the_model_against_them(smv, tmp_path):
    c, m = load_smv(smv)["relay"]
    assert is_implementation(m, c).status == "invalid"            # G F grant: not with no request
    p = tmp_path / "relay2.smv"
    p.write_text(RELAY.replace("LTLSPEC NAME live := G F grant\n", ""))
    c, m = load_smv(str(p))["relay2"]
    assert is_implementation(m, c).status == "valid"


@NUXMV
def test_a_smv_file_is_drawn(smv):
    from chase_cps import viz
    d = viz.from_spec(smv, check=True)
    (n,) = d["nodes"]
    assert n["title"] == "relay" and n["badge"]["text"] == "M ⊨ C" and n["badge"]["status"] == "invalid"
    assert n["badge"]["why"]["violated"] == [4] and n["source"]["line"] == 2


@pytest.mark.parametrize("spec, why", [("SPEC AG grant", "CTL"), ("LTLSPEC G (Y grant)", "past"),
                                       ("LTLSPEC G ok", "DEFINE")])
def test_what_is_not_read(tmp_path, spec, why):
    p = tmp_path / "t.smv"
    p.write_text(RELAY.split("LTLSPEC")[0] + "DEFINE ok := grant;\n" + spec + "\n")
    with pytest.raises(ValueError, match=why):
        load_smv(str(p))


def test_a_model_with_no_specification_is_not_a_contract(tmp_path):
    from chase_cps.cli import _read
    p = tmp_path / "plain.smv"
    p.write_text(RELAY.split("LTLSPEC")[0])
    assert not has_specs(str(p))
    with pytest.raises(ValueError, match="not a specification"):
        _read(str(p))
