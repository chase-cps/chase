"""Diagrams of contracts, components, and systems (chase_cps.viz), the
origin of the results of the algebra, and the state graphs of models."""

import json
import re

import pytest

from chase_cps import Component, Contract, Model, Result, System, Verification, viz
from chase_cps.cli import main
from chase_cps.backends import ADAPTERS
from chase_cps.logic import Core

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_SLUGS = ADAPTERS["slugs"].available()
except ImportError:
    HAS_SLUGS = False
from chase_cps.explicit import _cover, state_graph


def contracts():
    a = Contract.from_strings(name="A", inputs=["x"], outputs=["y"], guarantees=["G (x -> y)"])
    b = Contract.from_strings(name="B", inputs=["y"], outputs=["z"], guarantees=["G (y -> z)"])
    top = Contract.from_strings(name="Top", inputs=["x"], outputs=["y", "z"],
                                guarantees=["G (x -> z)"])
    return a, b, top


def data(page: str) -> dict:
    return json.loads(re.search(r"const DATA = (.*?);\n", page).group(1))


def ids(nodes):
    for n in nodes:
        yield n["id"]
        yield from ids(n.get("children", []))


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
    group = viz.diagram(a.compose(b, name="AB"))["nodes"][0]
    assert group["kind"] == "group" and [c["title"] for c in group["children"]] == ["A", "B"]
    wires = {(w["from"][0], w["to"][0], w["var"]) for w in group["edges"] if "from" in w}
    kids = {c["title"]: c["id"] for c in group["children"]}
    assert (kids["A"], kids["B"], "y") in wires
    assert (None, kids["A"], "x") in wires            # the input of the box


def test_refinement_is_drawn_as_nesting():
    a, b, top = contracts()
    ab = a.compose(b, name="AB")
    # a decomposition: the group is the box of the contract it refines
    d = viz.diagram(ab, top, refines=[(ab, top, "valid")])
    group = d["nodes"][0]
    assert group["title"] == "Top" and group["contract"]["G"] == ["G (x -> z)"]
    assert group["badge"] == {"text": "⊑", "status": "valid"}
    # a single contract: inside the box of the other one, wired by name
    other = Contract.from_strings(name="A2", inputs=["x"], outputs=["y"], guarantees=["G y"])
    box = viz.diagram(refines=[(other, a, "invalid")])["nodes"][0]
    assert box["title"] == "A" and [c["title"] for c in box["children"]] == ["A2"]
    assert box["badge"]["status"] == "invalid" and len(box["edges"]) == 2
    # every box has its own id
    every = list(ids(d["nodes"]))
    assert len(every) == len(set(every))
    # a chain: each box inside the one it refines, drawn once
    a3 = Contract.from_strings(name="A3", inputs=["x"], outputs=["y"], guarantees=["G y"])
    chain = viz.diagram(refines=[(a3, other, "valid"), (other, a, "valid")])["nodes"]
    assert len(chain) == 1 and chain[0]["title"] == "A"
    assert chain[0]["children"][0]["title"] == "A2"
    assert chain[0]["children"][0]["children"][0]["title"] == "A3"


def test_a_quotient_is_a_hole_that_holds_what_refines_it():
    a, b, top = contracts()
    q = top.divide(a, name="Q")
    node = viz.diagram(q, refines=[(b, q, "valid")])["nodes"][0]
    assert node["op"] == "quotient" and node["title"] == "Top"
    assert [c["kind"] for c in node["children"]] == ["contract", "hole"]
    hole = node["children"][1]
    assert hole["title"] == "Q" and [c["title"] for c in hole["children"]] == ["B"]
    assert hole["badge"] == {"text": "⊑", "status": "valid"}


def test_merged_viewpoints_are_stacked_without_wires():
    a, b, _ = contracts()
    node = viz.diagram(a.merge(b, name="M"))["nodes"][0]
    assert node["layout"] == "stack" and node["edges"] == []


def test_a_system_marks_its_components_and_open_inputs():
    a, b, top = contracts()
    c = Contract.from_strings(name="C", inputs=["w"], outputs=["v"], guarantees=["G v"])
    m = Model.from_dict({"name": "M", "inputs": {"x": "bool"}, "outputs": {"y": "bool"},
                         "states": ["s"], "transitions": [{"from": "s", "outputs": {"y": True},
                                                            "to": "s"}]})
    s = System("S", [Component("a", a, m, status="valid"), Component("b", b),
                     Component("c", c)], contract=top)
    assert s.connections() == [("a", "b", "y")]
    group = viz.diagram(s)["nodes"][0]
    kinds = [(k["kind"], k.get("badge", {}).get("status")) for k in group["children"]]
    assert kinds == [("component", "valid"), ("contract", None), ("contract", None)]
    assert group["children"][0]["model"]["fsm"]["count"] == 1
    assert any(w.get("open", [None])[0] == group["children"][2]["id"] for w in group["edges"])
    assert group["title"] == "Top"                    # the system, inside its contract


def test_checks_and_cores_are_marked_on_the_box():
    c = Contract.from_strings(name="C", inputs=["x"], outputs=["y", "z"], assumptions=["G F x"],
                              guarantees=["G (x -> y)", "G z", "G (x -> !y)"])
    core = Contract("C_core", dict(c.inputs), dict(c.outputs), c.assumptions[:1],
                    [c.guarantees[0], c.guarantees[2]])
    v = Verification(Result("sat"), Result("sat"), Result("unsat", core=Core("found", core)))
    node = viz.diagram(c, checks=[(c, v), (c, Result("realizable"))])["nodes"][0]
    assert [(k["name"], k["status"]) for k in node["checks"]] == [
        ("A∧G", "invalid"), ("A", "valid"), ("G", "valid"), ("R", "valid")]
    assert node["checks"][0]["core"] == {"A": [0], "G": [0, 2], "part": "contract"}
    # an unrealizable core keeps all the assumptions: only its guarantees count
    r = Result("unrealizable", core=Core("found", Contract("C_core", dict(c.inputs), dict(c.outputs),
                                                           list(c.assumptions), [c.guarantees[1]])))
    mark = viz.diagram(c, checks=[(c, r)])["nodes"][0]["checks"][0]
    assert mark == {"name": "R", "status": "invalid", "reason": "",
                    "core": {"A": [], "G": [1], "part": "contract"}}
    # a check on the contract of a component goes to the box of the component
    m = Model.from_dict({"name": "M", "inputs": {"x": "bool"}, "outputs": {"y": "bool"},
                         "states": ["s"], "transitions": [{"from": "s", "to": "s"}]})
    k = Component("K", c, m)
    s = System("S", [k])
    group = viz.diagram(s, checks=[(c, "compatible", "sat")])["nodes"][0]
    assert group["children"][0]["checks"] == [{"name": "compatible", "status": "valid", "reason": ""}]


def test_environments_drive_the_inputs_and_the_context_is_kept():
    a, b, top = contracts()
    s = System("S", [Component("a", a), Component("b", b)], contract=top)
    pulse = Model.from_dict({"name": "Pulse", "inputs": {}, "outputs": {"x": "bool"},
                             "states": ["lo", "hi"], "initial": "lo",
                             "transitions": [{"from": "lo", "outputs": {"x": False}, "to": "hi"},
                                             {"from": "hi", "outputs": {"x": True}, "to": "lo"}]})
    d = viz.diagram(s, a, environments=[(pulse, s, "valid"), ("G F x", a)], context="G !(x & y)")
    kinds = [(n["kind"], n["title"]) for n in d["nodes"]]
    assert kinds == [("environment", "Environment"), ("group", "Top"),
                     ("environment", "Environment"), ("contract", "A")]
    env, system = d["nodes"][0], d["nodes"][1]
    assert env["outputs"] == ["x"] and env["model"]["fsm"]["count"] == 2
    assert env["badge"] == {"text": "E ⊨ A", "status": "valid"}
    assert d["nodes"][2]["contract"]["E"] == ["G F x"]
    assert {"from": [env["id"], "out", "x"], "to": [system["id"], "in", "x"], "var": "x"} in d["edges"]
    assert d["context"] == ["G !(x & y)"]
    named = viz.diagram(a, environments=[(Component("Pulse", None, pulse), a)])
    assert named["nodes"][0]["title"] == "Pulse"


def test_state_graphs_of_models():
    m = Model.from_dict({"name": "T", "inputs": {"x": "bool"}, "outputs": {"y": "bool"},
                         "states": ["off", "on"], "initial": "off",
                         "transitions": [{"from": "off", "when": "x", "to": "on"},
                                         {"from": "on", "outputs": {"y": True}, "to": "off"}]})
    g = state_graph(m)
    assert [s["id"] for s in g["states"] if s["init"]] == ["off"]
    assert {(e["from"], e["to"], e["label"]) for e in g["edges"]} == {
        ("off", "on", "x"), ("on", "off", "/ y=1")}
    assert _cover([(0, 1), (1, 1)], ["a", "b"]) == "b"
    # the types of the ports: a bus of 8 bits, an integer range, a real
    w = Contract.from_strings(name="Wd", inputs={"d": "word[8]", "n": (0, 5)},
                              outputs={"r": "real", "b": "bool"}, guarantees=["G (r >= 0.0)"])
    types = viz.diagram(w)["nodes"][0]["types"]
    assert types == {"d": {"label": "[7:0]", "bus": 8}, "n": {"label": "0..5"},
                     "r": {"label": "real"}}
    assert _cover([(0, 0), (0, 1), (1, 0), (1, 1)], ["a", "b"]) == ""


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
    assert len(d["nodes"]) == 1 and d["nodes"][0]["title"] == "Top"


@pytest.mark.skipif(not (HAS_MC and HAS_SLUGS), reason="nuXmv/NuSMV or slugs not installed")
def test_the_command_line_marks_the_checks_and_draws_environments(tmp_path):
    greedy = Contract.from_strings(name="Greedy", inputs=["r1", "r2"], outputs=["g1", "g2"],
                                   guarantees=["G !(g1 & g2)", "G (r1 -> g1)", "G (r2 -> g2)"])
    spec = tmp_path / "s.json"
    spec.write_text(json.dumps({"contracts": [greedy.to_dict()]}))
    env = tmp_path / "env.json"
    env.write_text(json.dumps({"name": "Requests", "inputs": {}, "outputs": {"r1": "bool", "r2": "bool"},
                               "states": ["s"], "transitions": [{"from": "s", "to": "s",
                                                                 "outputs": {"r1": True, "r2": False}}]}))
    out = tmp_path / "s.html"
    assert main(["view", str(spec), "--verify", "--realizable", "--environment", str(env), "Greedy",
                 "--check", "-o", str(out)]) == 0
    d = data(out.read_text())
    env_box, box = d["nodes"]
    assert env_box["kind"] == "environment" and env_box["badge"]["status"] == "valid"
    assert [(k["name"], k["status"]) for k in box["checks"]] == [
        ("A", "valid"), ("G", "valid"), ("R", "invalid")]
    assert box["checks"][2]["core"]["G"] == [0, 1, 2]
