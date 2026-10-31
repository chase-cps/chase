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
                                       ("LTLSPEC G odd", "case")])
def test_what_is_not_read(tmp_path, spec, why):
    p = tmp_path / "t.smv"
    p.write_text(RELAY.split("LTLSPEC")[0] + "DEFINE ok := grant;\n"
                 "odd := case grant : TRUE; TRUE : FALSE; esac;\n" + spec + "\n")
    with pytest.raises(ValueError, match=why):
        load_smv(str(p))


def test_a_model_with_no_specification_is_not_a_contract(tmp_path):
    from chase_cps.cli import _read
    p = tmp_path / "plain.smv"
    p.write_text(RELAY.split("LTLSPEC")[0])
    assert not has_specs(str(p))
    with pytest.raises(ValueError, match="not a specification"):
        _read(str(p))


@NUXMV
def test_the_counterexample_is_kept_with_the_result(smv, tmp_path):
    # drawn twice with the results kept: the second drawing reads the result,
    # and still shows the counterexample and the term it breaks
    from chase_cps import viz
    kept = str(tmp_path / "relay.results.json")
    first = viz.from_spec(smv, check=True, results=kept)["nodes"][0]["badge"]
    second = viz.from_spec(smv, check=True, results=kept)["nodes"][0]["badge"]
    assert second["status"] == "invalid"
    assert second["why"]["trace"] == first["why"]["trace"] and second["why"]["violated"] == [4]


HANDSHAKE = """MODULE main
IVAR req : boolean;
VAR state : {idle, busy, done};
    ack : boolean;
DEFINE ready := state = idle;
       serving := state = busy & !ack;
ASSIGN
  init(state) := idle;
  next(state) := case state = idle & req : busy; state = busy : done; TRUE : idle; esac;
  init(ack) := FALSE;
  next(ack) := (state = busy);
LTLSPEC G ((ready & req) -> X X ack)
LTLSPEC G (serving -> X ack)
LTLSPEC G (req -> X ack)
"""


def test_enumerations_and_defines(tmp_path):
    # the values of an enumeration are 0, 1, ... in the contract and in the model;
    # a DEFINE in a specification is its definition
    from chase_cps import ltl
    p = tmp_path / "hs.smv"
    p.write_text(HANDSHAKE)
    c, m = load_smv(str(p))["hs"]
    assert c.outputs["state"] == (0, 2)
    assert ltl.to_str(c.guarantees[0]) == "G ((state = 0 & req) -> X X ack)"
    if not (ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()):
        return
    from chase_cps import Contract, is_implementation
    status = []
    for i, g in enumerate(c.guarantees):
        one = Contract(f"S{i}", dict(c.inputs), dict(c.outputs))
        one._set_terms([], [g], {})
        status.append(is_implementation(m, one).status)
    assert status == ["valid", "valid", "invalid"]


def test_an_enumeration_value_in_two_orders(tmp_path):
    p = tmp_path / "two.smv"
    p.write_text("MODULE main\nVAR a : {on, off};\n b : {off, on};\nASSIGN init(a) := on;\nLTLSPEC G (a = on)\n")
    with pytest.raises(ValueError, match="same order"):
        load_smv(str(p))
