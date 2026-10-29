"""Diagrams of contracts, components, and systems (chase_cps.viz), and the
origin of the results of the algebra."""

import json
import re

from chase_cps import Component, Contract, System, viz
from chase_cps.cli import main


def contracts():
    a = Contract.from_strings(name="A", inputs=["x"], outputs=["y"], guarantees=["G (x -> y)"])
    b = Contract.from_strings(name="B", inputs=["y"], outputs=["z"], guarantees=["G (y -> z)"])
    top = Contract.from_strings(name="Top", inputs=["x"], outputs=["y", "z"],
                                guarantees=["G (x -> z)"])
    return a, b, top


def data(page: str) -> dict:
    return json.loads(re.search(r"const DATA = (.*?);\n", page).group(1))


def test_the_results_of_the_algebra_remember_their_operands():
    a, b, top = contracts()
    ab = a.compose(b, name="AB")
    assert ab.origin[0] == "composition" and ab.origin[1] == (a, b)
    q = top.divide(a, name="Q")
    assert q.origin[0] == "quotient" and q.origin[1] == (top, a)
    assert a.origin is None
    # the origin is not part of the contract
    assert ab == Contract(ab.name, ab.inputs, ab.outputs, ab.assumptions, ab.guarantees)
    assert "origin" not in ab.to_dict()


def test_a_composition_is_a_box_with_its_operands_connected_by_name():
    a, b, top = contracts()
    ab = a.compose(b, name="AB")
    d = viz.diagram(ab, top, refines=[(ab, top, "valid")])
    group, upper = d["nodes"]
    assert group["kind"] == "group" and [c["title"] for c in group["children"]] == ["A", "B"]
    wires = {(w["from"][0], w["to"][0], w["var"]) for w in group["edges"] if "from" in w}
    ids = {c["title"]: c["id"] for c in group["children"]}
    assert (ids["A"], ids["B"], "y") in wires
    assert (None, ids["A"], "x") in wires            # the input of the box
    # the composition refines Top: it is drawn inside Top, with no arrow
    assert d["refines"] == [] and group["spec"] == {"title": "Top", "status": "valid", "A": [],
                                                     "G": ["G (x -> z)"]}
    # a contract that can replace another points to it
    other = Contract.from_strings(name="A2", inputs=["x"], outputs=["y"], guarantees=["G y"])
    arrows = viz.diagram(refines=[(other, a)])["refines"]
    assert len(arrows) == 1 and arrows[0]["status"] is None
    # every box has its own id
    assert len({group["id"], upper["id"], *ids.values()}) == 4


def test_a_quotient_is_a_box_with_a_hole():
    a, b, top = contracts()
    q = top.divide(a, name="Q")
    node = viz.diagram(q)["nodes"][0]
    assert node["op"] == "quotient" and node["title"] == "Top"
    assert [c["kind"] for c in node["children"]] == ["contract", "hole"]
    assert node["children"][1]["title"] == "Q"


def test_a_system_marks_its_components_and_open_inputs():
    a, b, top = contracts()
    c = Contract.from_strings(name="C", inputs=["w"], outputs=["v"], guarantees=["G v"])
    s = System("S", [Component("a", a, status="valid"), Component("b", b), Component("c", c)],
               contract=top)
    assert s.connections() == [("a", "b", "y")]
    d = viz.diagram(s)
    group = d["nodes"][0]
    assert [k["status"] for k in group["children"]] == ["valid", None, None]
    assert any(w.get("open", [None])[0] == group["children"][2]["id"] for w in group["edges"])
    # the system is drawn inside its contract, not below it
    assert d["refines"] == [] and group["spec"]["title"] == "Top"


def test_the_page_and_the_command_line(tmp_path):
    a, b, top = contracts()
    page = viz.html(a.compose(b, name="AB"))
    assert data(page)["nodes"][0]["title"] == "AB" and "elk.bundled.js" in page
    spec = tmp_path / "s.json"
    spec.write_text(json.dumps({"contracts": [a.to_dict(), b.to_dict(), top.to_dict()]}))
    out = tmp_path / "s.html"
    assert main(["view", str(spec), "--contracts", "A+B", "--refines", "A+B", "Top",
                 "-o", str(out)]) == 0
    d = data(out.read_text())
    # the composition refines Top: it is drawn inside it
    assert len(d["nodes"]) == 1 and d["nodes"][0]["spec"]["title"] == "Top"
