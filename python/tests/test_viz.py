"""Diagrams of contracts, components, and systems (chase_cps.viz), the
origin of the results of the algebra, and the state graphs of models."""

import json
import re

import pytest

from chase_cps import Component, Contract, Model, Result, System, Verification, viz
from chase_cps.cli import main

try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False
needs_native = pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not installed")
try:
    from chase_cps.rtl import yosys as _yosys
    HAS_YOSYS = _yosys() is not None
except Exception:
    HAS_YOSYS = False
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
    return json.loads(re.search(r"(?:const|let) DATA = (.*?);\n", page).group(1))


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
    assert group["badge"] == {"text": "⪯", "status": "valid"}
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


def test_a_decomposition_keeps_every_port_of_the_contract_it_refines():
    # B1 drives yy, not the output y of B: the box of B keeps y, which the box around wires
    a = Contract.from_strings(name="A", inputs={"x": "bool"}, outputs={"m": "bool"},
                              guarantees=["G (x -> X m)"])
    b = Contract.from_strings(name="B", inputs={"m": "bool"}, outputs={"y": "bool"},
                              guarantees=["G (m -> X X y)"])
    b1 = Contract.from_strings(name="B1", inputs={"m": "bool"}, outputs={"yy": "bool"},
                               guarantees=["G (m -> X X yy)"])
    top = Contract.from_strings(name="Top", inputs={"x": "bool"}, outputs={"y": "bool"},
                                guarantees=["G (x -> X X X y)"])
    d = viz.diagram(System("Top", [Component("A", a), Component("B", b)], contract=top),
                    refines=[(System("B", [Component("B1", b1)], contract=b), b, "unknown")])

    def check(box):
        ports = {None: (set(box.get("inputs", [])), set(box.get("outputs", [])))}
        for ch in box.get("children", []):
            ports[ch["id"]] = (set(ch.get("inputs", [])), set(ch.get("outputs", [])))
        for w in box.get("edges", []):
            for end in (w["from"], w["to"]):
                ins, outs = ports[end[0]]
                assert end[2] in (ins if end[1] == "in" else outs), (box["title"], w)
        for ch in box.get("children", []):
            check(ch)
    for n in d["nodes"]:
        check(n)
    group = next(c for c in d["nodes"][0]["children"] if c["title"] == "B")
    assert group["outputs"] == ["y"]


def test_an_input_of_a_part_that_the_contract_does_not_give_is_open():
    # Controller reads enable; Cruise has no input enable and no part drives it
    sensor = Contract.from_strings(name="Sensor", inputs=["raw"], outputs=["speed"], guarantees=["G (raw -> speed)"])
    ctrl = Contract.from_strings(name="Controller", inputs=["speed", "enable"], outputs=["throttle"],
                                 guarantees=["G ((speed & enable) -> throttle)"])
    cruise = Contract.from_strings(name="Cruise", inputs=["raw"], outputs=["throttle"], guarantees=["G (raw -> throttle)"])
    d = viz.diagram(refines=[(sensor.compose(ctrl, name="Parts"), cruise, "invalid")])
    box = d["nodes"][0]
    assert box["title"] == "Cruise" and box["inputs"] == ["raw"]
    # an input of the contract that a part drives is not a port of the box
    watch = Contract.from_strings(name="Watch", inputs=["raw", "speed"], outputs=["throttle"],
                                  guarantees=["G (raw -> throttle)"])
    box = viz.diagram(refines=[(sensor.compose(ctrl, name="Parts"), watch, "invalid")])["nodes"][0]
    assert box["inputs"] == ["raw"]
    assert {"open": [next(c["id"] for c in box["children"] if c["title"] == "Controller"), "in", "enable"],
            "var": "enable"} in box["edges"]


def test_a_refinement_that_fails_says_which_terms_and_at_which_step():
    # on the assumptions: the assumptions of the parts that the trace breaks; on the
    # guarantees: the step where a term fails first, where the panel opens
    from chase_cps import refines
    sensor = Contract.from_strings(name="Sensor", inputs=["high"], outputs=["alarm"], guarantees=["G (high -> alarm)"])
    pump = Contract.from_strings(name="Pump", inputs=["alarm"], outputs=["on"], assumptions=["G F !alarm"],
                                 guarantees=["G (alarm -> X on)"])
    drain = Contract.from_strings(name="Drain", inputs=["high"], outputs=["on"], guarantees=["G (high -> X on)"])
    parts = sensor.compose(pump, name="Parts")
    r = refines(parts, drain)
    if r.status != "invalid":
        pytest.skip("no model checker")
    why = viz.diagram(refines=[(parts, drain, r)])["nodes"][0]["badge"]["why"]
    assert why["failed"] == "assumptions" and why["broken"] == ["Pump: G F !alarm"]
    late = Contract.from_strings(name="Late", inputs=["high"], outputs=["on"], guarantees=["X X G on"])
    early = Contract.from_strings(name="Early", inputs=["high"], outputs=["on"], guarantees=["G on"])
    r = refines(late, early)
    why = viz.diagram(refines=[(late, early, r)])["nodes"][0]["badge"]["why"]
    assert why["at"] == min(i for i, s in enumerate(why["trace"]["steps"]) if s[why["trace"]["vars"].index("on")] is False)


def test_viewpoints_inside_a_composition_and_refinement_on_four_levels():
    a, b, top = contracts()
    t = Contract.from_strings(name="At", inputs=["x"], outputs=["y"], guarantees=["G (x -> X y)"])
    s = System("S", [Component("A", a.conjoin(t, name="A")), Component("B", b)])
    group = viz.diagram(s)["nodes"][0]
    both = group["children"][0]
    assert both["op"] == "conjunction" and both["inputs"] == ["x"] and both["outputs"] == ["y"]
    assert all(v["bare"] for v in both["children"])       # the viewpoints share its ports
    assert {"from": [both["id"], "out", "y"], "to": [group["children"][1]["id"], "in", "y"],
            "var": "y"} in group["edges"]
    # S2 drawn inside L1, then refined in its own box, whatever the order of the pairs
    C = lambda n, i, o: Contract.from_strings(name=n, inputs=[i], outputs=[o], guarantees=[f"G ({i} -> F {o})"])
    t4, s1, s2, s21, s22 = C("T", "a", "e"), C("S1", "a", "b"), C("S2", "b", "e"), C("S21", "b", "c"), C("S22", "c", "e")
    l1, l2 = s1.compose(s2, name="L1"), s21.compose(s22, name="L2")
    for pairs in ([(l1, t4), (l2, s2)], [(l2, s2), (l1, t4)]):
        nodes = viz.diagram(refines=pairs)["nodes"]
        assert len(nodes) == 1 and nodes[0]["title"] == "T"
        inner = nodes[0]["children"][1]
        assert inner["title"] == "S2" and [c["title"] for c in inner["children"]] == ["S21", "S22"]
        assert {"from": [nodes[0]["children"][0]["id"], "out", "b"], "to": [inner["id"], "in", "b"],
                "var": "b"} in nodes[0]["edges"]


def test_a_quotient_is_a_hole_that_holds_what_refines_it():
    a, b, top = contracts()
    q = top.divide(a, name="Q")
    node = viz.diagram(q, refines=[(b, q, "valid")])["nodes"][0]
    assert node["op"] == "quotient" and node["title"] == "Top"
    assert [c["kind"] for c in node["children"]] == ["contract", "hole"]
    hole = node["children"][1]
    assert hole["title"] == "Q" and [c["title"] for c in hole["children"]] == ["B"]
    assert hole["badge"] == {"text": "⪯", "status": "valid"}


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
    # the input w of c, given by neither the contract nor a part: the part not written drives it
    assert kinds == [("component", "valid"), ("contract", None), ("contract", None), ("hole", None)]
    assert group["children"][0]["model"]["fsm"]["count"] == 1
    hole = group["children"][3]
    assert "w" in hole["outputs"]
    assert any(e.get("from") == [hole["id"], "out", "w"] and e.get("to") == [group["children"][2]["id"], "in", "w"]
               for e in group["edges"])
    assert group["title"] == "S" and group["contract"]["name"] == "Top"   # the system, in its contract
    assert group["note"].startswith("Contract Top")


def test_a_system_with_a_part_missing_draws_its_quotient():
    # the outputs of the contract and the inputs of the parts that nobody drives (the logic of a
    # module besides its instances): the quotient of the contract by the parts, as a hole
    top = Contract.from_strings(name="Top", inputs={"x": "bool"}, outputs={"y": "bool", "z": "bool"},
                                guarantees=["G (x -> X y)", "G (x -> X z)"])
    a = Contract.from_strings(name="A", inputs={"x": "bool"}, outputs={"m": "bool"}, guarantees=["G (x -> X m)"])
    b = Contract.from_strings(name="B", inputs={"m": "bool", "k": "bool"}, outputs={"y": "bool"},
                              guarantees=["G (m -> y)"])
    group = viz.diagram(System("Top", [Component("A", a), Component("B", b)], contract=top))["nodes"][0]
    hole = group["children"][-1]
    assert hole["kind"] == "hole" and hole["title"] == "?" and hole["note"] == "Top / A ⊗ B"
    assert {"k", "z"} <= set(hole["outputs"])
    ids = {c["title"]: c["id"] for c in group["children"]}
    assert any(e.get("from") == [ids["?"], "out", "k"] and e.get("to") == [ids["B"], "in", "k"] for e in group["edges"])
    assert group["note"].endswith("⊗ ?")
    # every signal driven: no hole
    full = viz.diagram(System("AB", [Component("A", a), Component("B", b)],
                              contract=Contract.from_strings(name="AB", inputs={"x": "bool", "k": "bool"},
                                                             outputs={"y": "bool"}, guarantees=["G (x -> X y)"])))
    assert all(c["kind"] != "hole" for c in full["nodes"][0]["children"])


def test_a_quotient_the_algebra_cannot_compute_is_a_hole_with_its_ports():
    # two parts drive y: no composition, so no terms, but the ports of the quotient and a warning
    top = Contract.from_strings(name="Top", inputs={"x": "bool"}, outputs={"y": "bool", "z": "bool"},
                                guarantees=["G (x -> X y)", "G (x -> X z)"])
    a = Contract.from_strings(name="A", inputs={"x": "bool"}, outputs={"y": "bool"}, guarantees=["G (x -> y)"])
    b = Contract.from_strings(name="B", inputs={"k": "bool"}, outputs={"y": "bool"}, guarantees=["G (k -> y)"])
    with pytest.warns(UserWarning, match="hole of Top is drawn with its ports only.*outputs of both"):
        group = viz.diagram(System("Top", [Component("A", a), Component("B", b)], contract=top))["nodes"][0]
    hole = group["children"][-1]
    assert hole["kind"] == "hole" and hole["note"] == "Top / A ⊗ B (ports only)" and hole.get("contract") is None
    assert hole["outputs"] == ["k", "z"] and hole["inputs"] == ["x", "y"]
    ids = {c["title"]: c["id"] for c in group["children"]}
    assert any(e.get("from") == [ids["?"], "out", "k"] and e.get("to") == [ids["B"], "in", "k"] for e in group["edges"])


def test_signals_the_parts_do_not_agree_on_are_red_wires():
    # two parts drive y; m is a word in one part and a Boolean in the other: their wires carry what
    # is wrong (drawn red, said in their tips), and a warning says it; the other wires do not
    top = Contract.from_strings(name="Top", inputs={"x": "bool"}, outputs={"y": "bool", "z": "bool"},
                                guarantees=["G (x -> X y)", "G (x -> X z)"])
    a = Contract.from_strings(name="A", inputs={"x": "bool"}, outputs={"y": "bool", "m": "word[4]"}, guarantees=["G (x -> y)"])
    b = Contract.from_strings(name="B", inputs={"m": "bool"}, outputs={"y": "bool", "z": "bool"}, guarantees=["G (m -> y)"])
    with pytest.warns(UserWarning) as said:
        group = viz.diagram(System("Top", [Component("A", a), Component("B", b)], contract=top))["nodes"][0]
    assert group["clash"] == {"y": "y is driven by A and by B", "m": "m has two types: word[4] in A; bool in B"}
    assert {str(w.message) for w in said} >= {"Top: y is driven by A and by B", "Top: m has two types: word[4] in A; bool in B"}
    red = {(w["var"], w["error"]) for w in group["edges"] if "error" in w}
    assert ("y", "y is driven by A and by B") in red and ("m", "m has two types: word[4] in A; bool in B") in red
    assert all("error" not in w for w in group["edges"] if w["var"] in ("x", "z"))


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
    # well-separation and vacuity: the assumptions of the core, the guarantees of the vacuous output
    from chase_cps.gr1 import OutputVacuity, VacuousOutput
    ws = Result("not_separated", core=Core("found", Contract("C_core", dict(c.inputs), dict(c.outputs),
                                                             c.assumptions[:1], [])))
    ov = OutputVacuity("vacuous", [VacuousOutput("z", "never", [c.guarantees[1]],
                                                 core=Core("found", c.assumptions[:1]))])
    marks = viz.diagram(c, checks=[(c, ws), (c, ov), (c, OutputVacuity("not_vacuous"))])["nodes"][0]["checks"]
    assert [(k["name"], k["status"], k.get("core")) for k in marks] == [
        ("WS", "invalid", {"A": [0], "G": [], "part": "contract"}),
        ("¬V", "invalid", {"A": [0], "G": [1], "part": "contract"}),
        ("¬V", "valid", None)]
    # a check on the contract of a component goes to the box of the component
    m = Model.from_dict({"name": "M", "inputs": {"x": "bool"}, "outputs": {"y": "bool"},
                         "states": ["s"], "transitions": [{"from": "s", "to": "s"}]})
    k = Component("K", c, m)
    s = System("S", [k])
    group = viz.diagram(s, checks=[(c, Result("realizable"))])["nodes"][0]
    assert group["children"][0]["checks"] == [{"name": "R", "status": "valid", "reason": ""}]
    # the known checks only; an unknown result says its check
    with pytest.raises(TypeError):
        viz.diagram(c, checks=[(c, Result("valid"))])
    with pytest.raises(TypeError):
        viz.diagram(c, checks=[(c, Result("unknown"))])
    assert viz.diagram(c, checks=[(c, "realizable", Result("unknown"))])["nodes"][0]["checks"][0][
        "name"] == "R"


def test_environments_drive_the_inputs_and_the_context_is_kept():
    a, b, top = contracts()
    s = System("S", [Component("a", a), Component("b", b)], contract=top)
    pulse = Model.from_dict({"name": "Pulse", "inputs": {}, "outputs": {"x": "bool"},
                             "states": ["lo", "hi"], "initial": "lo",
                             "transitions": [{"from": "lo", "outputs": {"x": False}, "to": "hi"},
                                             {"from": "hi", "outputs": {"x": True}, "to": "lo"}]})
    d = viz.diagram(s, a, environments=[(pulse, s, "valid"), ("G F x", a)], context="G !(x & y)")
    kinds = [(n["kind"], n["title"]) for n in d["nodes"]]
    assert kinds == [("environment", "Environment"), ("group", "S"),
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


def test_offline_pages_the_output_variable_and_specification_files(tmp_path, monkeypatch):
    a, b, top = contracts()
    elk = tmp_path / "elk.bundled.js"
    elk.write_text("var ELK = function () {};")
    page = viz.html(a, elk=str(elk))
    assert "var ELK = function () {};" in page and "cdn.jsdelivr" not in page
    assert "cdn.jsdelivr.net/npm/elkjs@0.9.3" in viz.html(a)
    # with CHASE_VIZ_OUTPUT, show writes the page there and opens no browser
    out = tmp_path / "out.html"
    monkeypatch.setenv("CHASE_VIZ_OUTPUT", str(out))
    monkeypatch.setattr("webbrowser.open", lambda *_: pytest.fail("a browser opened"))
    assert viz.show(a) == str(out) and data(out.read_text())["nodes"][0]["title"] == "A"
    # a specification file, as the command line and the MCP server draw it
    spec = tmp_path / "s.json"
    spec.write_text(json.dumps({"contracts": [a.to_dict(), b.to_dict(), top.to_dict()]}))
    d = viz.from_spec(str(spec), ["A+B"], [("A+B", "Top")])
    assert [n["title"] for n in d["nodes"]] == ["Top"] and d["title"] == "s.json"
    assert main(["view", str(spec), "--elk", str(elk), "-o", str(out)]) == 0
    assert "var ELK" in out.read_text()


def test_a_specification_with_probabilities_is_refused_clearly(tmp_path, capsys):
    try:
        from chase_cps import _native  # noqa: F401
    except ImportError:
        pytest.skip("CHASE modules not installed")
    spec = tmp_path / "p.ltl"
    spec.write_text("NAME: p;\nCONTRACT C1:\n    input integer (0:3) variable x;\n"
                    "    output integer (0:7) variable z;\nAssumptions:\n    true;\n"
                    "Guarantees:\n    G(P(x<=2) < 0.2);\n")
    assert main(["view", str(spec), "-o", str(tmp_path / "p.html")]) == 2
    err = capsys.readouterr().err
    assert "contract C1: probabilities, P(...), are not supported" in err and "Traceback" not in err


def test_one_item_refining_two_contracts_and_several_candidates_for_a_quotient():
    a, b, top = contracts()
    t2 = Contract.from_strings(name="Top2", inputs=["x"], outputs=["y", "z"], guarantees=["G (x -> F z)"])
    ab = a.compose(b, name="AB")
    nodes = viz.diagram(refines=[(ab, top, "invalid"), (ab, t2, "valid")])["nodes"]
    assert [(n["title"], n["badge"]["status"]) for n in nodes] == [("Top", "invalid"), ("Top2", "valid")]
    assert all([c["title"] for c in n["children"]] == ["A", "B"] for n in nodes)
    # each candidate in a hole of its own
    q = top.divide(a, name="Q")
    b2 = Contract.from_strings(name="B2", inputs=["y"], outputs=["z"], guarantees=["G z"])
    nodes = viz.diagram(q, refines=[(b, q, "invalid"), (b2, q, "valid")])["nodes"]
    holes = [next(c for c in n["children"] if c["kind"] == "hole") for n in nodes]
    assert [([c["title"] for c in h["children"]], h["badge"]["status"]) for h in holes] == [
        (["B"], "invalid"), (["B2"], "valid")]
    every = list(ids(nodes))
    assert len(every) == len(set(every))
    # compatibility and consistency, with their names
    marks = viz.diagram(a, checks=[(a, "consistent", Result("unsat")), (a, "compatible", Result("sat"))])[
        "nodes"][0]["checks"]
    assert [(k["name"], k["status"]) for k in marks] == [("G", "invalid"), ("A", "valid")]


def test_a_part_refined_by_another_contract_and_a_loop_are_drawn_apart():
    a, b, top = contracts()
    a2 = Contract.from_strings(name="A2", inputs=["x"], outputs=["y"], guarantees=["G (x -> y)", "G y"])
    ab = a.compose(b, name="AB")
    nodes = viz.diagram(ab, refines=[(a2, a, "valid"), (a, a2, "invalid")])["nodes"]
    # the composition keeps its parts; each refinement is a box of its own
    assert [c["title"] for c in nodes[0]["children"]] == ["A", "B"]
    assert [(n["title"], n["badge"]["status"], [c["title"] for c in n["children"]]) for n in nodes[1:]] == [
        ("A", "valid", ["A2"]), ("A2", "invalid", ["A"])]


def test_open_draws_the_boxes_asked_open():
    a, b, top = contracts()
    s = System("S", [Component("a", a), Component("b", b)], contract=top)
    d = viz.diagram(s, open=[b, top])
    group = d["nodes"][0]
    assert group.get("open") and group["children"][1].get("open") and not group["children"][0].get("open")
    assert "openAsked();" in viz.html(d)


def test_a_loop_is_drawn_next_to_its_item_and_open_skips_the_parts():
    x, y, z = {"x": "bool"}, {"y": "bool"}, {"z": "bool"}
    a = Contract.from_strings(name="A", inputs=x, outputs=y, assumptions=["G F x"], guarantees=["G F y"])
    b = Contract.from_strings(name="B", inputs=x, outputs=y, assumptions=["G F x"], guarantees=["G y"])
    c = Contract.from_strings(name="C", inputs=y, outputs=z, guarantees=["G (y -> z)"])
    d = viz.diagram(a, b, b.compose(c), refines=[(b, a, "valid"), (a, b, "invalid")], open=[b])
    assert [n["title"] for n in d["nodes"]] == ["A", "B", "B_C"]
    assert d["nodes"][0]["children"][0].get("open") and d["nodes"][1].get("open")
    assert not d["nodes"][2]["children"][0].get("open")


def test_open_reaches_each_hole_of_a_quotient():
    x, y, z = {"x": "bool"}, {"y": "bool"}, {"z": "bool"}
    a = Contract.from_strings(name="A", inputs=x, outputs=y, guarantees=["G (y -> x)"])
    s = Contract.from_strings(name="S", inputs=x, outputs={**y, **z}, guarantees=["G (z <-> y)"])
    q = s.divide(a, name="Q")
    c1 = Contract.from_strings(name="C1", inputs=y, outputs=z, guarantees=["G z"])
    c2 = Contract.from_strings(name="C2", inputs=y, outputs=z, guarantees=["G (z <-> y)"])
    d = viz.diagram(q, refines=[(c1, q, "invalid"), (c2, q, "valid")], open=[q])
    assert [n.get("open") for n in d["nodes"]] == [None, None]     # not the dividend
    assert all(ch.get("open") for n in d["nodes"] for ch in n["children"] if ch["kind"] == "hole")


def test_an_editor_draws_as_one_types(tmp_path, capsys):
    a, b, top = contracts()
    spec = tmp_path / "s.json"
    spec.write_text(json.dumps({"contracts": [a.to_dict(), b.to_dict()]}))
    # the data only, without the checks: an editor draws it again in place
    assert main(["view", str(spec), "--format", "json", "--no-checks", "--verify", "--realizable"]) == 0
    d = json.loads(capsys.readouterr().out)
    assert [n["title"] for n in d["nodes"]] == ["A", "B"] and not any(n.get("checks") for n in d["nodes"])
    page = viz.html(d)
    assert "function setData" in page and '"chase-data"' in page and '"chase-stale"' in page


def test_what_view_reads(tmp_path, capsys):
    sv = tmp_path / "arbiter.sv"
    sv.write_text("module arbiter(input logic clk, input logic req, output logic gnt);\n"
                  "  assert property (@(posedge clk) req |=> gnt);\nendmodule\n")
    assert main(["view", str(sv), "--format", "json", "-o", "-"]) == 0
    assert json.loads(capsys.readouterr().out)["nodes"][0]["title"] == "arbiter"
    model = tmp_path / "m.smv"
    model.write_text("MODULE main\n")
    assert main(["view", str(model)]) == 2
    assert "is not a specification" in capsys.readouterr().err


@needs_native
def test_an_undeclared_name_is_placed(tmp_path):
    from chase_cps import load_spec
    spec = tmp_path / "s.ltl"
    spec.write_text("NAME: s;\nCONTRACT B:\n    input boolean variable y;\n    output boolean variable z;\n"
                    "    Assumptions:\n        true;\n    Guarantees:\n        G (y -> w);\n")
    with pytest.raises(RuntimeError, match=r"contract B, line 8:16: w is not declared"):
        load_spec(spec)


def test_a_systemverilog_file_says_where(tmp_path, capsys):
    bad = tmp_path / "m.sv"
    bad.write_text("module m(input logic clk, input logic a, output logic b);\n"
                   "  assert property (@(posedge clk) a |=> b);\n"
                   "  assert property (@(posedge clk) s_eventually (!a && || b));\n"
                   "endmodule\n")
    assert main(["view", str(bad), "--format", "json", "-o", str(tmp_path / "m.json")]) == 0
    assert "warning: line 3: not read" in capsys.readouterr().err
    undeclared = tmp_path / "u.sv"
    undeclared.write_text("module u(input logic clk, input logic a, output logic b);\n"
                          "  assert property (@(posedge clk) a |-> c);\nendmodule\n")
    assert main(["view", str(undeclared)]) == 2
    assert "line 2:40:" in capsys.readouterr().err


@pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
def test_the_checks_are_kept_on_the_contracts():
    from chase_cps import refines, verify
    a = Contract.from_strings(name="A", inputs=["x"], outputs=["y"], guarantees=["G (x -> X y)"])
    b = Contract.from_strings(name="B", inputs=["x"], outputs=["y"], guarantees=["G (x -> F y)"])
    refines(a, b)
    verify(a)
    d = viz.diagram(b, refines=[(a, b)])              # no result given: the ones kept
    outer, = d["nodes"]
    inner, = outer["children"]
    assert outer["badge"]["status"] == "valid"
    assert {c["name"]: c["status"] for c in inner["checks"]} == {"A": "valid", "G": "valid"}
    assert not viz.diagram(a, results=False)["nodes"][0].get("checks")
    a.guarantees.append(a.guarantees[0])              # changed: what was checked no longer holds
    assert a.checked() == [] and not viz.diagram(a)["nodes"][0].get("checks")


ARB_SPEC = """# view: --implementation models/arb.json Arb --open Arb
NAME: arb;

CONTRACT Arb:
    input boolean variable req;
    output boolean variable grant;
    Assumptions:
        G F !req;
    Guarantees:
        G (req -> F grant);
"""
ARB_MODEL = {"name": "ArbImpl", "inputs": {"req": "bool"}, "outputs": {"grant": "bool"},
             "states": ["s"], "initial": "s",
             "transitions": [{"from": "s", "to": "s", "outputs": {"grant": True}}]}


@needs_native
def test_the_file_says_how_to_draw_it(tmp_path, monkeypatch):
    (tmp_path / "models").mkdir()
    (tmp_path / "models" / "arb.json").write_text(json.dumps(ARB_MODEL))
    spec = tmp_path / "arb.ltl"
    spec.write_text(ARB_SPEC)
    monkeypatch.chdir("/")                             # the files of the options are next to the spec
    out = tmp_path / "arb.json"
    assert main(["view", str(spec), "--format", "json", "-o", str(out)]) == 0
    box, = json.loads(out.read_text())["nodes"]
    assert box["kind"] == "component" and box["open"] and box["source"]["line"] == 4
    copy = tmp_path / "editor" / "arb.ltl"               # the text of an editor, drawn from a copy
    copy.parent.mkdir()
    copy.write_text(ARB_SPEC)
    monkeypatch.chdir(tmp_path)
    assert main(["view", str(copy), "--format", "json", "-o", str(out)]) == 0
    assert json.loads(out.read_text())["nodes"][0]["kind"] == "component"


@pytest.mark.skipif(not HAS_YOSYS, reason="Yosys not installed")
def test_a_module_is_drawn_with_its_state_machine(tmp_path):
    rtl = tmp_path / "toggle.v"
    rtl.write_text("module toggle(input clk, input en, output reg q);\n"
                   "  initial q = 0;\n  always @(posedge clk) if (en) q <= !q;\nendmodule\n")
    out = tmp_path / "t.json"
    assert main(["view", str(rtl), "--format", "json", "-o", str(out)]) == 0
    box, = json.loads(out.read_text())["nodes"]
    assert box["kind"] == "component" and box["inputs"] == ["en"] and box["outputs"] == ["q"]
    assert box["source"]["line"] == 1 and box["model"]["fsm"]["count"] == 2


def test_the_page_goes_to_the_text_and_back():
    page = viz.html(Contract.from_strings(name="A", inputs=["x"], outputs=["y"], guarantees=["G y"]))
    assert '"chase-reveal"' in page and '"chase-select"' in page and "function exportAs" in page


def test_each_term_knows_its_lines():
    # true gives no term, a conjunction gives one term for each part
    text = ("CONTRACT C:\n  input boolean variable x;\n  output boolean variable y;\n"
            "  Assumptions:\n    true;\n    G (x ->\n       X x);\n"
            "  Guarantees:\n    !y & G (x -> y);  # R1\n    G F y;\n")
    assert viz._term_lines(text) == {"C": {"A": [[6, 7]], "G": [[9, 9], [9, 9], [10, 10]]}}
    sva = ("module m (input logic clk, input logic x, output logic y);\n"
           "  // a comment: assert property (x);\n"
           "  A1: assume property (@(posedge clk)\n    x |=> x);\n"
           "  initial G1: assert property (@(posedge clk) !y);\nendmodule\n")
    assert viz._sva_term_lines(sva) == {"m": {"A": [[3, 4]], "G": [[5, 5]]}}


@needs_native
def test_the_terms_of_a_file_go_to_their_lines(tmp_path):
    spec = tmp_path / "c.ltl"
    spec.write_text("CONTRACT C:\n  input boolean variable x;\n  output boolean variable y;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    !y & G (x -> y);\n    G F y;\n")
    box, = viz.from_spec(str(spec))["nodes"]
    assert box["source"] == {"line": 1, "A": [], "G": [[7, 7], [7, 7], [8, 8]]}
    page = viz.html(viz.from_spec(str(spec)))
    assert 'data-line' in page and '"chase-select"' in page


@needs_native
def test_a_contract_drawn_inside_another_is_not_drawn_again(tmp_path):
    spec = tmp_path / "j.ltl"
    spec.write_text("# view: --refines A+B C\nNAME: j;\n"
                    "CONTRACT A:\n  input boolean variable x;\n  output boolean variable y;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (x -> y);\n"
                    "CONTRACT B:\n  input boolean variable y;\n  output boolean variable z;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (y -> z);\n"
                    "CONTRACT C:\n  input boolean variable x;\n  output boolean variable z;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (x -> z);\n")
    out = tmp_path / "j.json"
    assert main(["view", str(spec), "--format", "json", "--no-checks", "-o", str(out)]) == 0
    tops = [n["title"] for n in json.loads(out.read_text())["nodes"]]
    assert tops == ["C"]
    assert main(["view", str(spec), "--format", "json", "--no-checks", "--contracts", "A", "-o", str(out)]) == 0
    assert "A" in [n["title"] for n in json.loads(out.read_text())["nodes"]]


def _contracts(ports):
    return {n: Contract.from_strings(name=n, inputs=i, outputs=o, guarantees=["true"]) for n, (i, o) in ports.items()}


def test_the_contracts_go_together_by_their_ports():
    # Charger: two robots and the first of the arbiters with the same ports make the system
    spec = _contracts({"Robot1": (["grant1", "low1"], ["req1"]), "Robot2": (["grant2", "low2"], ["req2"]),
                       "Greedy": (["req1", "req2"], ["grant1", "grant2"]),
                       "Fair": (["req1", "req2"], ["grant1", "grant2"]),
                       "System": (["low1", "low2"], ["grant1", "grant2", "req1", "req2"])})
    assert viz._structure(spec) == ([("Robot1+Robot2+Greedy", "System")], ["System"], [])
    # LMFSM: the signals between the parts are not outputs of the system
    spec = _contracts({"Autopilot": (["apfail", "good"], ["MODE", "pullup"]),
                       "Sensor": (["MODE", "limits"], ["good"]),
                       "System": (["apfail", "limits"], ["pullup"])})
    assert viz._structure(spec)[0] == [("Autopilot+Sensor", "System")]
    # a part that reads what the system does not give is not a part of it
    spec = _contracts({"A": (["x"], ["y"]), "B": (["y", "w"], ["z"]), "S": (["x"], ["y", "z"])})
    assert viz._structure(spec) == ([], ["A", "B", "S"], [])
    # the arbiter not written yet: the system with the robots, and a part missing
    spec = _contracts({"Robot1": (["grant1", "low1"], ["req1"]), "Robot2": (["grant2", "low2"], ["req2"]),
                       "System": (["low1", "low2"], ["grant1", "grant2", "req1", "req2"])})
    assert viz._structure(spec) == ([], ["System"], [("Robot1+Robot2", "System")])
    # one part only, or a part that reads a signal nobody gives: not a missing part
    spec = _contracts({"A": (["x", "m"], ["y"]), "S": (["x"], ["y", "z"])})
    assert viz._structure(spec)[2] == []
    spec = _contracts({"A": (["x"], ["y"]), "B": (["w"], ["z"]), "S": (["x"], ["y", "z", "u"])})
    assert viz._structure(spec)[2] == []


@needs_native
def test_a_file_is_drawn_wired_and_checked(tmp_path):
    spec = tmp_path / "w.ltl"
    spec.write_text("NAME: w;\n"
                    "CONTRACT A:\n  input boolean variable x;\n  output boolean variable y;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (x -> y);\n"
                    "CONTRACT B:\n  input boolean variable y;\n  output boolean variable z;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (y -> z);\n"
                    "CONTRACT D:\n  input boolean variable z;\n  output boolean variable u;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (z -> u);\n")
    d = viz.from_spec(str(spec))
    assert [n["title"] for n in d["nodes"]] == ["A", "B", "D"]
    assert sorted(w["var"] for w in d["edges"]) == ["y", "z"]


@needs_native
@pytest.mark.skipif(not ADAPTERS["nuxmv"].available() and not ADAPTERS["nusmv"].available(),
                    reason="no model checker")
def test_the_results_of_the_checks_are_kept_next_to_the_file(tmp_path, monkeypatch):
    from chase_cps import record
    spec = tmp_path / "k.ltl"
    text = ("NAME: k;\n"
            "CONTRACT A:\n  input boolean variable x;\n  output boolean variable y;\n"
            "  Assumptions:\n    F(x);\n  Guarantees:\n    G (x -> y);\n"
            "CONTRACT B:\n  input boolean variable x;\n  output boolean variable y;\n"
            "  Assumptions:\n    true;\n  Guarantees:\n    G (x -> y);\n")
    spec.write_text(text)
    out = tmp_path / "k.json"
    marks = lambda: {n["title"]: [(c["name"], c["status"], c.get("core")) for c in n.get("checks", [])]
                     for n in json.loads(out.read_text())["nodes"]}
    assert main(["view", str(spec), "--check", "--format", "json", "-o", str(out)]) == 0
    kept = tmp_path / "k.results.json"
    first = json.loads(kept.read_text())
    assert set(first["contracts"]) == {"A", "B"} and first["contracts"]["A"]["feasible"]["result"] == "sat"
    # again: from the file, with no check run
    calls = []
    import chase_cps.logic as logic
    real = logic.is_satisfiable
    monkeypatch.setattr(logic, "is_satisfiable", lambda *a, **k: calls.append(1) or real(*a, **k))
    assert main(["view", str(spec), "--check", "--format", "json", "-o", str(out)]) == 0
    assert calls == [] and marks()["A"] == [("A∧G", "valid", None)]
    # with no check at all (as one types): the results kept are shown all the same
    assert main(["view", str(spec), "--no-checks", "--format", "json", "-o", str(out)]) == 0
    assert calls == [] and marks()["A"] == [("A∧G", "valid", None)]
    # a term changed: A is checked again, with its core, and its old result goes
    spec.write_text(text.replace("G (x -> y);\nCONTRACT B", "G (x -> y);\n    G !y;\nCONTRACT B"))
    assert main(["view", str(spec), "--check", "--format", "json", "-o", str(out)]) == 0
    assert calls and marks()["A"] == [("A∧G", "invalid", {"A": [0], "G": [0, 1], "part": "contract"})]
    again = json.loads(kept.read_text())
    assert again["contracts"]["A"]["feasible"]["core"]["guarantees"] == ["G (x -> y)", "G !y"]
    assert len(record.Record(str(kept)).entries) == 2
    # no time left: nothing is checked, nothing is marked
    kept.unlink()
    assert main(["view", str(spec), "--check", "--budget", "0", "--format", "json", "-o", str(out)]) == 0
    assert marks() == {"A": [], "B": []} and not kept.exists()



@needs_native
def test_a_part_not_written_yet_is_a_hole(tmp_path):
    spec = tmp_path / "h.ltl"
    spec.write_text("NAME: h;\n"
                    "CONTRACT A:\n  input boolean variable x;\n  output boolean variable y;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (x -> y);\n"
                    "CONTRACT B:\n  input boolean variable y;\n  output boolean variable z;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (y -> z);\n"
                    "CONTRACT S:\n  input boolean variable x;\n"
                    "  output boolean variable y;\n  output boolean variable z;\n  output boolean variable u;\n"
                    "  Assumptions:\n    true;\n  Guarantees:\n    G (x -> F u);\n")
    top, = viz.from_spec(str(spec))["nodes"]
    assert top["title"] == "S" and [n["kind"] for n in top["children"]] == ["contract", "contract", "hole"]
    hole = top["children"][2]
    assert hole["outputs"] == ["u"]


@pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
def test_a_failed_verify_draws_three_tags():
    # verify checks A and G when A & G is unsat; only verify is kept on the
    # contract, so the box has A, G, A∧G once each
    from chase_cps import verify
    c = Contract.from_strings(inputs=["req"], outputs=["deploy"], assumptions=["G F req"],
                              guarantees=["G (req -> F deploy)", "G !deploy"])
    assert not verify(c)
    assert [kind for kind, _, _ in c.checked()] == ["verify"]
    names = [t["name"] for t in viz.diagram(c)["nodes"][0]["checks"]]
    assert sorted(names) == ["A", "A∧G", "G"]


def test_steps_are_shown_as_intervals():
    # the terms of G[0,3] p are p, X p, X X p, X X X p; the box shows G[0,3] p,
    # and a conjunction split at step 0 is folded with the steps after it
    from chase_cps import ltl
    p = [ltl.parse(t) for t in ["a", "X a", "X X a", "b < 3", "c", "X (b < 3 & c)", "X X (b < 3 & c)"]]
    shown, groups = viz._fold(p)
    assert shown == ["G[0,2] a", "G[0,2] (b < 3 & c)"]
    assert groups == [[0, 1, 2], [3, 4, 5, 6]]
    assert viz._fold([ltl.parse("X a | X X a")])[0] == ["F[1,2] a"]
    assert viz._fold([ltl.parse("a"), ltl.parse("b")]) == (["a", "b"], [[0], [1]])
    # inside: F[0,2] G[0,1] (a & b), unrolled
    nested = ltl.parse("(a & b & X (a & b)) | X (a & b & X (a & b)) | X X (a & b & X (a & b))")
    assert viz._fold([nested])[0] == ["F[0,2] G[0,1] (a & b)"]
    assert viz._fold([ltl.parse("a | X b")])[0] == ["a | X b"]


def test_named_constants_are_shown_and_cores_still_match():
    c = Contract.from_strings(inputs=["x"], outputs=["y"], assumptions=["x <= 5"],
                              guarantees=["y <= x + k"], constants={"k": 2})
    t = viz.diagram(c, open=[c])["nodes"][0]["contract"]
    assert t["G"] == ["y <= x + k"] and t["K"] == ["k = 2"] and t["_G"] == ["y <= x + 2"]
    node = {"contract": t}
    assert viz._core(node, ([], ["y <= x + 2"]), False)["G"] == [0]


COUNT = """NAME: count;
CONTRACT Counter:
    output integer (0:9) variable n;
    integer constant top 3;
    Assumptions:
        true;
    Guarantees:
        n = 0;
        G ((n < top -> n' = n + 1) & (n >= top -> n' = n));
CONTRACT Bound:
    output integer (0:9) variable n;
    real constant k 1;
    Assumptions:
        true;
    Guarantees:
        G (n <= k);
"""


def test_a_check_says_what_repair_needs(tmp_path):
    # the check as chase-cps repair takes it, and each constant: its value,
    # its type as declared, where it is declared, part or requirement
    f = tmp_path / "count.ltl"
    f.write_text(COUNT)
    d = viz.from_spec(str(f), refines=[("Counter", "Bound")])
    bound, = [n for n in d["nodes"] if n["title"] == "Bound"]
    r = bound["repair"]["⪯"]
    assert r["check"] == "refines" and r["args"] == ["Counter", "Bound"]
    cs = {c["name"]: c for c in r["constants"]}
    assert cs["top"]["role"] == "part" and cs["top"]["type"] == "int" and cs["top"]["value"] == "3"
    assert cs["top"]["owners"] == [{"contract": "Counter", "role": "part", "line": 4}]
    assert cs["k"]["role"] == "requirement" and cs["k"]["type"] == "real"   # real constant k 1
    assert cs["k"]["owners"][0]["line"] == 12
    assert bound["repair"]["A∧G"]["check"] == "verify"


def test_constants_are_written_in_their_declarations():
    out = viz.set_constants(COUNT, [{"line": 4, "name": "top", "value": "1"},
                                    {"line": 12, "name": "k", "value": "3.5"}])
    lines = out.split("\n")
    assert lines[3].strip() == "integer constant top 1;" and lines[11].strip() == "real constant k 3.5;"
    assert len(lines) == len(COUNT.split("\n"))
    with pytest.raises(ValueError, match="does not declare"):
        viz.set_constants(COUNT, [{"line": 5, "name": "top", "value": "1"}])
    with pytest.raises(ValueError, match="not a number"):
        viz.set_constants(COUNT, [{"line": 4, "name": "top", "value": "1/3"}])


@pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
def test_a_served_page_repairs_and_writes_the_file(tmp_path):
    # chase-cps view --serve: POST /repair runs the repair, POST /apply writes
    # the value; without the header of the page, neither
    pytest.importorskip("z3")
    import json, socket, subprocess, sys, time, urllib.request
    f = tmp_path / "count.ltl"
    f.write_text(COUNT)
    with socket.socket() as s:
        s.bind(("127.0.0.1", 0))
        port = s.getsockname()[1]
    p = subprocess.Popen([sys.executable, "-m", "chase_cps", "view", str(f), "--refines", "Counter", "Bound",
                          "--no-checks", "--serve", "--port", str(port)], stdout=subprocess.DEVNULL,
                         stderr=subprocess.DEVNULL)
    try:
        def post(route, body, header=True):
            req = urllib.request.Request(f"http://127.0.0.1:{port}{route}", data=json.dumps(body).encode(),
                                         headers={"Content-Type": "application/json",
                                                  **({"X-CHASE": "1"} if header else {})})
            try:
                with urllib.request.urlopen(req, timeout=120) as r:
                    return r.status, json.loads(r.read() or b"{}")
            except urllib.error.HTTPError as e:
                return e.code, None
        for _ in range(100):
            try:
                urllib.request.urlopen(f"http://127.0.0.1:{port}/version", timeout=2)
                break
            except OSError:
                time.sleep(0.2)
        ask = {"check": "refines", "args": ["Counter", "Bound"],
               "params": [{"name": "top", "low": 0, "high": 9, "type": "int"}]}
        assert post("/repair", ask, header=False)[0] == 403
        code, reply = post("/repair", ask)
        assert code == 200 and reply["result"]["status"] == "repaired", reply
        assert reply["result"]["text"] == {"top": "1"}
        code, reply = post("/apply", {"edits": [{"line": 4, "name": "top", "value": "1"}]})
        assert reply == {"ok": True}
        assert "integer constant top 1;" in f.read_text()
        code, reply = post("/apply", {"edits": [{"line": 5, "name": "top", "value": "1"}]})
        assert "does not declare" in reply["error"]
    finally:
        p.terminate()
        p.wait()


def test_a_core_is_found_on_a_term_shown_folded():
    # the term F[0,2] p is shown folded, and its core is still found
    from chase_cps import ltl
    c = Contract.from_strings(inputs=["p"], outputs=["q"], assumptions=["F[0,2] p"], guarantees=["q"])
    node = {"contract": viz.diagram(c, open=[c])["nodes"][0]["contract"]}
    assert node["contract"]["A"] == ["F[0,2] p"]
    raw = ltl.to_str(c.assumptions[0])
    assert viz._core(node, ([raw], []), False)["A"] == [0]


@pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
def test_a_served_repair_stops_on_demand(tmp_path):
    # a repair asked of chase-cps view --serve runs in a process of its own: a
    # stop ends it at once
    pytest.importorskip("z3")
    import json, os, socket, subprocess, sys, threading, time, urllib.request
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "benchmarks"))
    from repair_scaling import family, requirement
    from chase_cps import to_spec
    cs, last = family(64, 2)
    f = tmp_path / "chain.ltl"
    f.write_text(to_spec(cs + [requirement(last, 0)], name="chain"))
    with socket.socket() as s:
        s.bind(("127.0.0.1", 0))
        port = s.getsockname()[1]
    p = subprocess.Popen([sys.executable, "-m", "chase_cps", "view", str(f), "--no-checks", "--serve",
                          "--port", str(port)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    def post(route, body):
        req = urllib.request.Request(f"http://127.0.0.1:{port}{route}", data=json.dumps(body).encode(),
                                     headers={"Content-Type": "application/json", "X-CHASE": "1"})
        with urllib.request.urlopen(req, timeout=120) as r:
            return json.loads(r.read())
    try:
        for _ in range(150):
            try:
                urllib.request.urlopen(f"http://127.0.0.1:{port}/version", timeout=2)
                break
            except OSError:
                time.sleep(0.2)
        parts = "+".join(c.name for c in cs)
        ask = {"id": 7, "check": "refines", "args": [parts, "Req"], "time_limit": 100,
               "params": [{"name": "lam", "low": 0, "high": 10000, "type": "real"},
                          {"name": "x1", "low": 0, "high": 100, "type": "real"}]}
        got = {}
        t = threading.Thread(target=lambda: got.update(post("/repair", ask)))
        start = time.time()
        t.start()
        time.sleep(1.5)
        assert post("/repair-stop", {"id": 7}) == {"ok": True}
        t.join(30)
        assert got == {"error": "stopped"} and time.time() - start < 10
    finally:
        p.terminate()
        p.wait()


def test_a_period_draws_the_steps_as_times():
    c = Contract.from_strings(name="C", outputs={"x": (0, 9)}, guarantees=["G (x <= 3)"])
    assert "period" not in viz.diagram(c)
    assert viz.diagram(c, period=0.5)["period"] == "0.5"
    assert viz.diagram(c, period="1/4")["period"] == "0.25"
