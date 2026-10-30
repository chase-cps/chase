"""Diagrams of contracts, components, and systems.

Four rules:

- A contract is a rounded box. Its assumptions and guarantees open under
  its name.
- A component with a model (its implementation: RTL, a state machine in
  JSON or SMV, a synthesized controller) is a square box: its contract,
  then its model, whose states and transitions open under it, and whether
  the model implements the contract.
- Refinement is drawn as nesting: what refines a contract is drawn inside
  its box, a decomposition (contracts or components side by side,
  connected by the names of their variables) or a single contract, with
  whether it refines the contract.
- A quotient is a dashed hole next to the known parts; a contract that
  refines the quotient is drawn inside the hole. Merging and conjunction
  stack their operands in one box.

Around them: an environment is a box on the left that drives the inputs
of what it is the environment of; the results of the checks of a box are
marks under its name (compatible, consistent, realizable); a core is
marked on the terms of the contract; the context of the checks is written
above the diagram.

The structure comes from the contracts themselves: each result of the
algebra remembers how it was computed (``Contract.origin``). The page is
drawn in the browser, laid out by ELK (the Eclipse Layout Kernel, loaded
from a CDN).

    from chase_cps import viz
    viz.save(viz.diagram(system, refines=[(text, q3, "valid")]), "plan.html")
    viz.diagram(c, checks=[(c, verify(c, core=True)), (c, is_realizable(c, core=True))],
                environments=[(env, c, is_environment(env, c))], context="G !(a & b)")
    viz.show(system)            # opens the browser
    system                      # in Jupyter, the diagram
"""

from __future__ import annotations

import html as _html
import json
import os
import tempfile
import webbrowser
from typing import Iterable, Optional

from . import ltl
from .contract import Contract
from .queries import Result
from .system import Component, System

# Operations drawn as a box with its operands inside, and the word used.
SIGNS = {"composition": "⊗", "quotient": "/", "merging": "•",
         "conjunction": "∧", "disjunction": "∨", "separation": "÷"}
STACKED = {"merging", "conjunction", "disjunction", "separation"}


def _type(t) -> Optional[dict]:
    """How a port of a type is drawn: None for a Boolean; a label, and the
    width of a bus for a bit vector."""
    from . import words
    w = words.word_type(t)
    if w is not None:
        width, signed = w
        return {"label": ("s" if signed else "") + f"[{width - 1}:0]", "bus": width}
    if isinstance(t, tuple):
        return {"label": f"{t[0]}..{t[1]}"}
    if t == "real":
        return {"label": "real"}
    return None


def _types(*contracts) -> dict:
    out = {}
    for c in contracts:
        for v, t in c.variables.items():
            k = _type(t)
            if k is not None:
                out[v] = k
    return out


def _status(s):
    return getattr(s, "status", s)


class _Scene:
    def __init__(self):
        self.count = 0
        self.ids = {}           # the first box of an object
        self.objs = {}          # the object of each box
        self.drawn = {}         # the objects drawn, by id

    def fresh(self, prefix="n") -> str:
        self.count += 1
        return f"{prefix}{self.count}"

    def box(self, obj, types=None, **node) -> dict:
        self.drawn.setdefault(id(obj), obj)
        node.setdefault("id", self.fresh())
        node["types"] = types or {}
        self.ids.setdefault(id(obj), node["id"])
        self.objs[node["id"]] = id(obj)
        node.setdefault("children", [])
        node.setdefault("edges", [])
        node.setdefault("layout", "row")
        return node

    @staticmethod
    def terms(c: Contract) -> dict:
        return {"name": c.name, "A": [ltl.to_str(a) for a in c.assumptions],
                "G": [ltl.to_str(g) for g in c.guarantees]}

    # Contracts ---------------------------------------------------------------

    def contract(self, c: Contract, title: Optional[str] = None) -> dict:
        origin = c.origin
        if not origin or origin[0] not in SIGNS:
            note = None
            if origin:
                note = f"{origin[0]} of " + ", ".join(o.name for o in origin[1])
            return self.box(c, types=_types(c), kind="contract", title=title or c.name, note=note,
                            contract=self.terms(c), inputs=sorted(c.inputs),
                            outputs=sorted(c.outputs))
        operation, operands = origin
        if operation == "composition":
            parts = []
            for o in operands:            # a chain of compositions is one level
                node = self.contract(o)
                parts += node["children"] if node.get("op") == "composition" else [node]
            return self.group(c, operation, parts)
        if operation == "quotient":
            dividend, divisor = operands
            known = self.contract(divisor)
            known = known["children"] if known.get("op") == "composition" else [known]
            hole = self.box(c, types=_types(c), kind="hole", title=c.name,
                            note=f"{dividend.name} / {divisor.name}",
                            contract=self.terms(c), inputs=sorted(c.inputs),
                            outputs=sorted(c.outputs))
            node = self.group(dividend, operation, known + [hole], box_of=c)
            node["note"] = " ⊗ ".join([k["title"] for k in known] + ["?"])
            node["contract"] = self.terms(dividend)
            return node
        return self.group(c, operation, [self.contract(o) for o in operands])

    def group(self, c: Contract, operation: str, children: list, box_of=None) -> dict:
        names = [ch["title"] for ch in children]
        if operation in STACKED:      # viewpoints of one component: its ports, no wires inside
            for ch in children:         # a viewpoint shares the ports of the box
                if not ch.get("edges"):
                    ch["bare"] = True
            parts = [ch for ch in children if not ch.get("bare")]   # decompositions: wired to the box
            outputs = sorted(c.outputs)
            return self.box(box_of if box_of is not None else c, types=_types(c), kind="group",
                            op=operation,
                            title=c.name, note=f" {SIGNS[operation]} ".join(names),
                            layout="stack", contract=self.terms(c), inputs=sorted(c.inputs),
                            outputs=outputs, children=children,
                            edges=[w for ch in parts for w in _through(ch, c.inputs, outputs)])
        outputs = _outside(children, c.outputs)
        return self.box(box_of if box_of is not None else c, types=_types(c), kind="group",
                        op=operation, title=c.name, note=f" {SIGNS[operation]} ".join(names),
                        layout="row", contract=self.terms(c), inputs=sorted(c.inputs), outputs=outputs,
                        children=children, edges=_wires(children, c.inputs) + _out(children, outputs))

    # Components and systems ------------------------------------------------------------

    def component(self, k: Component) -> dict:
        if k.implementation is None:          # a contract with a name
            node = self.contract(k.contract, title=k.name)
            self.ids.setdefault(id(k), node["id"])
            return node
        fsm = None
        try:
            from .explicit import state_graph
            fsm = state_graph(k.implementation, inputs=k.contract.inputs)
        except Exception:                     # the diagram does without it
            fsm = None
        name = getattr(k.implementation, "name", None) or type(k.implementation).__name__
        source = (getattr(k.implementation, "source", None) or {}).get("kind")
        node = self.box(k, types=_types(k.contract), kind="component", title=k.name,
                        contract=self.terms(k.contract),
                        model={"name": name, "fsm": fsm, "source": source},
                        badge={"text": "M ⊨ C", "status": k.status},
                        inputs=sorted(k.contract.inputs), outputs=sorted(k.contract.outputs))
        self.ids.setdefault(id(k.contract), self.ids[id(k)])
        self.drawn.setdefault(id(k.contract), k.contract)
        return node

    def system(self, s: System) -> dict:
        children = [self.component(k) for k in s.components]
        if s.contract is not None:
            inputs = s.contract.inputs
        else:
            driven = {v for k in s.components for v in k.contract.outputs}
            inputs = {v for k in s.components for v in k.contract.inputs if v not in driven}
        outputs = _outside(children, s.contract.outputs if s.contract else None)
        return self.box(s, types=_types(*(k.contract for k in s.components),
                                         *([s.contract] if s.contract else [])),
                        kind="group", op="system", title=s.name,
                        note=" ⊗ ".join(k.name for k in s.components), contract=None,
                        inputs=sorted(inputs), outputs=outputs, children=children,
                        edges=_wires(children, inputs) + _out(children, outputs))

    def environment(self, env, target: dict, status) -> dict:
        """An environment of the box target: it drives the inputs of target."""
        from .models import as_model
        name, impl = "Environment", env
        if isinstance(env, Component):
            name, impl = env.name, env.implementation
        model = None if isinstance(impl, ltl.Node) else as_model(impl)
        node = dict(kind="environment", title=name, inputs=[], outputs=list(target.get("inputs", [])),
                    badge={"text": "E ⊨ A", "status": status})
        if model is None:                   # a formula
            from .logic import _formula
            node["contract"] = {"name": name, "A": [], "G": [], "E": [ltl.to_str(_formula(impl))]}
        else:
            fsm = None
            try:
                from .explicit import state_graph
                fsm = state_graph(model, inputs=target.get("outputs", []))
            except Exception:
                fsm = None
            node["model"] = {"name": getattr(model, "name", None) or "model", "fsm": fsm,
                             "source": (getattr(model, "source", None) or {}).get("kind")}
        return self.box(env, types=target.get("types"), **node)

    def node(self, obj) -> dict:
        if isinstance(obj, System):
            return self.system(obj)
        if isinstance(obj, Component):
            return self.component(obj)
        if isinstance(obj, Contract):
            return self.contract(obj)
        raise TypeError(f"cannot draw {type(obj).__name__}")

    # Refinement ---------------------------------------------------------------------

    def wrap(self, inner: dict, high: Contract, status) -> dict:
        """inner, drawn inside the box of the contract it refines."""
        if inner["kind"] == "group":          # a decomposition: the group is the box
            if inner.get("contract"):         # the computed composition stays, under its own icon
                inner["composed"] = inner["contract"]
            outputs = _outside(inner["children"], high.outputs)   # the ports of the contract it refines
            inner.update(kind="group", title=high.name, contract=self.terms(high),
                         badge={"text": "⪯", "status": status}, outputs=outputs,
                         edges=[w for w in inner["edges"] if w.get("to", [0])[0] is not None]
                         + _out(inner["children"], outputs))
            self.ids.setdefault(id(high), inner["id"])
            return inner
        return self.box(high, types=_types(high), kind="contract", title=high.name,
                        contract=self.terms(high), inputs=sorted(high.inputs), outputs=sorted(high.outputs),
                        children=[inner], edges=_through(inner, high.inputs, high.outputs),
                        badge={"text": "⪯", "status": status})


def _wires(children: list, inputs) -> list:
    """The connections inside a box: an output of a child to the inputs of
    the others with the same name; an input of the box to the children that
    read it. An input read by a child that nobody drives is marked open."""
    wires = []
    drivers = {v: ch["id"] for ch in children for v in ch.get("outputs", [])}
    for ch in children:
        for v in ch.get("inputs", []):
            if v in drivers and drivers[v] != ch["id"]:
                wires.append({"from": [drivers[v], "out", v], "to": [ch["id"], "in", v], "var": v})
            elif v in inputs:
                wires.append({"from": [None, "in", v], "to": [ch["id"], "in", v], "var": v})
            else:
                wires.append({"open": [ch["id"], "in", v], "var": v})
    return wires


def _outside(children: list, outputs=None) -> list:
    """The outputs of the box that holds the children: those of its contract
    (all of them when there is none), as the children drive them; a signal
    between two children is an output too, as in the composed contract."""
    driven = {v for ch in children for v in ch.get("outputs", [])}
    return sorted(driven if outputs is None else driven & set(outputs))


def _out(children: list, outputs) -> list:
    """The wires from the outputs of the children to those of the box."""
    return [{"from": [ch["id"], "out", v], "to": [None, "out", v], "var": v}
            for ch in children for v in ch.get("outputs", []) if v in outputs]


def _through(inner: dict, inputs, outputs) -> list:
    """The wires from the ports of a box to the box inside it, by name."""
    wires = [{"from": [None, "in", v], "to": [inner["id"], "in", v], "var": v}
             for v in inner.get("inputs", []) if v in inputs]
    wires += [{"from": [inner["id"], "out", v], "to": [None, "out", v], "var": v}
              for v in inner.get("outputs", []) if v in outputs]
    return wires


def _ancestors(nodes: list, node_id: str, path: tuple = ()) -> Optional[list]:
    """The boxes that hold the box node_id, the outer first ([] at the top),
    or None when it is not drawn."""
    for n in nodes:
        if n["id"] == node_id:
            return list(path)
        found = _ancestors(n.get("children", []), node_id, path + (n,))
        if found is not None:
            return found
    return None


def _parent(nodes: list, node_id: str) -> Optional[dict]:
    """The box that holds the box node_id, or None at the top."""
    path = _ancestors(nodes, node_id) or []
    return path[-1] if path else None


def _holds(node: dict, oid: int, objs: dict) -> bool:
    """Whether the box, or a box inside it, draws the object oid."""
    return objs.get(node["id"]) == oid or any(_holds(c, oid, objs) for c in node.get("children", []))


def _boxes(nodes: list):
    for n in nodes:
        yield n
        yield from _boxes(n.get("children", []))


def _find(nodes: list, node_id: str):
    for n in nodes:
        if n["id"] == node_id:
            return n
        found = _find(n.get("children", []), node_id)
        if found is not None:
            return found
    return None


# Checks ------------------------------------------------------------------------------

def _mark(result) -> str:
    """valid, invalid, or unknown: whether the check holds."""
    status = _status(result)
    if status in (None, "unknown"):
        return "unknown"
    if status in ("vacuous", "not_vacuous"):          # the check holds when nothing is vacuous
        return "valid" if status == "not_vacuous" else "invalid"
    if isinstance(result, str):
        return "valid" if status in ("valid", "sat", "realizable", "separated", "receptive",
                                     "found") else "invalid"
    return "valid" if bool(result) else "invalid"


CHECKS = ("verify, is_realizable, is_well_separated, is_vacuous, or is_receptive; is_compatible "
          "and is_consistent with their name, (item, 'consistent', result)")
# The label of each check, by the name of the check (for a result that is unknown)
KINDS = {"realizable": "R", "well_separated": "WS", "vacuous": "¬V", "receptive": "receptive",
         "compatible": "A", "consistent": "G"}


def _checks(r, kind: Optional[str] = None) -> list:
    """[(label, result, core)]: verify gives A (compatible) and G
    (consistent), and A∧G (feasible) when the two cannot hold together;
    is_realizable R, is_well_separated WS, is_vacuous ¬V (no vacuous output),
    is_receptive receptive. The core explains a failure: a Core, the terms
    (A, G) of a vacuous output (its guarantees, and the assumptions of its
    core), or None."""
    from .checks import Verification
    from .gr1 import OutputVacuity
    from .logic import Core

    def core_of(x):
        c = getattr(x, "core", None)
        return c if isinstance(c, Core) and c else None
    if isinstance(r, Verification):
        out = [("A", r.compatible, core_of(r.compatible)), ("G", r.consistent, core_of(r.consistent))]
        if r.feasible is not None and not r.feasible:   # when they hold, A & G implies both
            out.insert(0, ("A∧G", r.feasible, core_of(r.feasible)))
        return out
    if isinstance(r, OutputVacuity):
        terms = None
        if r.vacuous:
            a = [t for v in r.vacuous if v.core and v.core.core for t in v.core.core]
            terms = ([ltl.to_str(t) for t in a], [ltl.to_str(t) for v in r.vacuous for t in v.guarantees])
        return [("¬V", r, terms)]
    if kind is not None and kind not in KINDS:
        raise ValueError(f"no check {kind!r}: the checks are {', '.join(KINDS)} (and verify)")
    label = KINDS.get(kind) or {"realizable": "R", "unrealizable": "R", "separated": "WS",
                                "not_separated": "WS", "receptive": "receptive",
                                "not_receptive": "receptive"}.get(_status(r))
    if label is None or not isinstance(r, Result):
        raise TypeError(f"checks takes the results of {CHECKS}, not {r!r}"
                        + ("; an unknown result needs the name of its check, as in "
                           "(item, 'realizable', result)" if _status(r) == "unknown" else ""))
    return [(label, r, core_of(r))]


def _core(node: dict, core, unrealizable: bool) -> Optional[dict]:
    """Where the terms of the core are in the box: in its contract or in its
    composed contract, by index. All the assumptions are in an unrealizable
    core: only its guarantees count. The core is a Core, or the texts of its
    assumptions and guarantees."""
    if isinstance(core, tuple):
        want = {"A": set(core[0]), "G": set(core[1])}
    elif isinstance(core.core, Contract):
        c = core.core
        want = {"A": set() if unrealizable else {ltl.to_str(t) for t in c.assumptions},
                "G": {ltl.to_str(t) for t in c.guarantees}}
    else:                                   # formulas, assumptions or guarantees
        fs = {ltl.to_str(t) for t in core.core or []}
        want = {"A": fs, "G": fs}
    for key in ("contract", "composed"):
        terms = node.get(key)
        if not terms:
            continue
        hit = {p: [i for i, t in enumerate(terms[p]) if t in want[p]] for p in "AG"}
        found = {terms[p][i] for p in "AG" for i in hit[p]}
        if found and found >= want["A"] | want["G"]:
            return dict(hit, part=key)
    return None


def diagram(*items, refines: Iterable = (), checks: Iterable = (), environments: Iterable = (),
            context=None, open: Iterable = (), title: Optional[str] = None,
            results: bool = True) -> dict:
    """The diagram of contracts, components, and systems, as data for the
    page. ``refines`` lists (refined, abstract) or (refined, abstract,
    status), where the status is a string or the result of ``refines``: the
    refined one is drawn inside the abstract one, or inside the hole when
    the abstract one is a quotient. A system with a contract is drawn inside
    its contract.

    ``checks`` lists (item, result): the result of ``verify`` (A, G, and
    A∧G when they cannot hold together), ``is_realizable`` (R),
    ``is_well_separated`` (WS), ``is_vacuous`` (¬V), or ``is_receptive``;
    with core=True, the core comes with the check that fails. A result that
    is unknown does not say which check it is: (item, "realizable", result),
    or well_separated, vacuous, receptive; the results of ``is_compatible``
    and ``is_consistent`` (sat or unsat) always come with their name,
    (item, "consistent", result). ``environments`` lists (environment, item) or
    (environment, item, status): an LTL formula, a Model, or a Component
    whose implementation is the environment, drawn on the left of the item
    and driving its inputs; the status is that of ``is_environment``. The
    ``context`` of the checks (a formula or a list of formulas) is written
    above the diagram. The items in ``open`` are drawn open: their contract,
    their model when it is small, and the core of a check that fails.

    The checks run on a contract are kept on it (``Contract.checked``), with
    what it said then: with ``results`` (the default), a contract drawn that
    is not in ``checks`` shows the results kept on it that still hold (for
    the same ``context``), and a refinement given without its status takes
    the one kept. A contract that has changed since a check shows nothing of
    it."""
    scene = _Scene()
    checks = list(checks)
    nodes, top, own = [], {}, {}           # own: the box an item drew, before any nesting

    def add(obj):
        if id(obj) not in top:
            node = scene.node(obj)
            top[id(obj)] = own[id(obj)] = node
            nodes.append(node)
        return top[id(obj)]

    def replace(old, new):
        for i, n in enumerate(nodes):
            if n is old:
                nodes[i] = new
                return
        nodes.append(new)

    for item in items:
        node = add(item)
        if isinstance(item, System) and item.contract is not None:
            wrapped = scene.wrap(node, item.contract, item.status)
            wrapped.update(title=item.name, note=f"contract {item.contract.name} · {node['note']}")
            replace(node, wrapped)
    def after(box, new):                # new, next to the item that holds box
        i = next((i for i, n in enumerate(nodes) if n is box or _find([n], box["id"]) is not None), None)
        nodes.insert(len(nodes) if i is None else i + 1, new)

    def parts(obj, out):                 # the contracts drawn in the box of obj
        out.add(id(obj))
        if isinstance(obj, System):
            for k in obj.components:
                parts(k.contract, out)
        elif isinstance(obj, Component):
            parts(obj.contract, out)
        elif isinstance(obj, Contract) and obj.origin:
            for o in obj.origin[1]:
                parts(o, out)
        return out
    # from the top down: a contract is refined after the refinement that draws it
    todo, ordered = list(refines), []
    while todo:
        first = next((r for r in todo
                      if not any(x is not r and id(r[1]) in parts(x[0], set()) for x in todo)), todo[0])
        ordered.append(first)
        todo.remove(first)
    refined = set()                     # the items already drawn inside what they refine
    for r in ordered:
        low, high = r[0], r[1]
        status = _status(r[2]) if len(r) > 2 else None
        if status is None and results and isinstance(low, Contract) and isinstance(high, Contract):
            kept = [res for kind, other, res in low.checked(context) if kind == "refines" and other is high]
            status = _status(kept[-1]) if kept else None
        if isinstance(high, Contract) and high.origin and high.origin[0] == "quotient":
            if id(high) not in scene.ids:
                add(high)
            hole = _find(nodes, scene.ids[id(high)])
            if hole.get("children"):        # one candidate per hole: another one gets a copy of the quotient
                again = scene.contract(high)
                nodes.append(again)
                hole = next(c for c in again["children"] if c["kind"] == "hole")
            inner = scene.node(low)
            hole["children"].append(inner)
            hole["edges"] += _through(inner, hole["inputs"], hole["outputs"])
            hole["badge"] = {"text": "⪯", "status": status}
            continue
        drawn = _find(nodes, scene.ids[id(high)]) if id(high) in scene.ids else None
        # a part of a composition is refined in place by its decomposition only;
        # another contract that refines it is drawn apart, and the composition stays
        decomposition = isinstance(low, System) or (
            isinstance(low, Contract) and bool(low.origin) and low.origin[0] == "composition")
        part = drawn is not None and (_parent(nodes, drawn["id"]) or {}).get("kind") == "group"
        loop = drawn is not None and any(scene.objs.get(n["id"]) == id(low)    # inside a box of low
                                         for n in _ancestors(nodes, drawn["id"]))
        if (drawn is not None and not drawn.get("children") and top.get(id(low)) is not drawn
                and (decomposition or not part) and not loop):
            # the contract is drawn already (inside another box, or as an item): its box takes what refines it
            inner = top.pop(id(low), None)
            if inner is not None:
                nodes.remove(inner)
            else:
                inner = scene.node(low)
            wrapped = scene.wrap(inner, high, status)
            keep = drawn["id"]
            drawn.clear()
            drawn.update(wrapped)
            drawn["id"] = keep
            for k, v in list(scene.ids.items()):
                if v == wrapped["id"]:
                    scene.ids[k] = keep
            continue
        if id(low) in refined:              # it refines another contract too: drawn again, apart
            after(top[id(low)], scene.wrap(scene.node(low), high, status))
            continue
        inner = top.pop(id(low), None) or next(   # in a chain, the box drawn already
            (n for n in nodes if n["id"] == scene.ids.get(id(low))), None) or scene.node(low)
        if _holds(inner, id(high), scene.objs):  # a loop (each refines the other): drawn apart
            top.setdefault(id(low), inner)
            after(inner, scene.wrap(scene.node(low), high, status))
            continue
        wrapped = scene.wrap(inner, high, status)
        replace(inner, wrapped)
        top[id(low)] = wrapped
        refined.add(id(low))

    def box_of(obj) -> dict:              # an item drawn on its own first, then inside others
        if id(obj) in own:
            return own[id(obj)]
        if id(obj) not in scene.ids:
            return add(obj)
        return _find(nodes, scene.ids[id(obj)])

    if results:                       # the checks kept on the contracts drawn, but those given here
        given = {id(c[0]) for c in checks}
        for oid, obj in list(scene.drawn.items()):
            if oid in given or not isinstance(obj, Contract):
                continue
            for kind, _, res in obj.checked(context):
                if kind != "refines":
                    checks.append((obj, None if kind == "verify" else kind, res))
    for c in checks:
        obj, kind, r = (c[0], None, c[1]) if len(c) == 2 else c
        node = box_of(obj)
        for label, res, core in _checks(r, kind):
            mark = {"name": label, "status": _mark(res), "reason": getattr(res, "reason", "") or ""}
            if core is not None:
                where = _core(node, core, unrealizable=label == "R")
                if where is None:
                    raise ValueError(f"the terms of the core are not those of {node['title']}")
                mark["core"] = where
            node.setdefault("checks", []).append(mark)

    wires = []
    for e in environments:
        env, target = e[0], e[1]
        status = _status(e[2]) if len(e) > 2 else None
        node = box_of(target)
        env_node = scene.environment(env, node, status)
        outer = next(n for n in nodes if n is node or _find([n], node["id"]) is not None)
        nodes.insert(nodes.index(outer), env_node)
        if outer is node:                   # wires between boxes at the top only
            wires += [{"from": [env_node["id"], "out", v], "to": [node["id"], "in", v], "var": v}
                      for v in node.get("inputs", [])]
        else:
            env_node["badge"]["text"] = f"E ⊨ A of {node['title']}"
    for obj in open:                    # its first box, and its other boxes but the parts of a composition
        first = box_of(obj)
        boxes = [first] + [n for n in _boxes(nodes) if scene.objs.get(n["id"]) == id(obj) and n is not first
                           and (_parent(nodes, n["id"]) or {}).get("kind") != "group"]
        holes = [n for n in _boxes(nodes) if n["kind"] == "hole" and scene.objs.get(n["id"]) == id(obj)]
        for n in holes or boxes:        # a quotient: its holes, not the box of the dividend
            n["open"] = True
    if context is not None:
        from .logic import _formula
        context = [context] if isinstance(context, (str, ltl.Node)) else list(context)
        context = [ltl.to_str(_formula(f)) for f in context]
    return {"title": title or "CHASE", "nodes": nodes, "edges": wires, "context": context or []}


ELK = '<script src="https://cdn.jsdelivr.net/npm/elkjs@0.9.3/lib/elk.bundled.js"></script>'


def html(item, elk: Optional[str] = None, **kwargs) -> str:
    """The page that draws a diagram (or a contract, component, or system).
    The page loads ELK from a CDN; ``elk`` is the path of elk.bundled.js
    (elkjs 0.9) to put in the page instead, for a page that works offline."""
    d = item if isinstance(item, dict) and "nodes" in item else diagram(item, **kwargs)
    script = ELK
    if elk is not None:
        with open(elk, encoding="utf-8") as f:
            script = "<script>" + f.read().replace("</script", "<\\/script") + "</script>"
    return (_PAGE.replace("__ELK__", script)
                 .replace("__TITLE__", _html.escape(d["title"]))
                 .replace("__RELOAD__", _RELOAD)
                 .replace("__DATA__", json.dumps(d).replace("</", "<\\/")))


def save(item, path: str, **kwargs) -> str:
    """Writes the page to path and returns the path."""
    with open(path, "w", encoding="utf-8") as f:
        f.write(html(item, **kwargs))
    return path


def show(item, **kwargs) -> str:
    """Writes the page to a temporary file and opens it in the browser. When
    CHASE_VIZ_OUTPUT names a file, the page goes there and no browser opens
    (for an editor that shows the page itself), with the ELK of
    CHASE_VIZ_ELK when it is set."""
    out = os.environ.get("CHASE_VIZ_OUTPUT")
    if out:                            # CHASE_VIZ_ELK: the ELK the page takes, offline
        kwargs.setdefault("elk", os.environ.get("CHASE_VIZ_ELK") or None)
        return save(item, out, **kwargs)
    fd, path = tempfile.mkstemp(prefix="chase-", suffix=".html")
    os.close(fd)
    save(item, path, **kwargs)
    webbrowser.open("file://" + path)
    return path


def from_spec(path: str, contracts: Iterable = (), refines: Iterable = (),
              environments: Iterable = (), check: bool = False, verify: bool = False,
              realizable: bool = False, timeout: float = 60, title: Optional[str] = None,
              open: Iterable = ()) -> dict:
    """The diagram of a specification file (the logics language of CHASE, or
    JSON). ``contracts`` are the contracts to draw (default: all), each a
    name or names joined by + for their composition; ``refines`` lists
    (refined, abstract) names, drawn one inside the other; ``environments``
    lists (model file, contract): an SMV or JSON state machine, or a
    controller of slugs. ``check`` checks the refinements and the
    environments; ``verify`` and ``realizable`` check each contract of the
    file that is drawn, alone or as a part of a composition. ``open`` names
    the contracts drawn open."""
    from . import checks as _checks_
    from . import gr1
    from .cli import _contract, _model, _read
    spec = _read(path)
    made = {}

    def get(expression):              # one box per expression
        if expression not in made:
            made[expression] = _contract(expression, spec)
        return made[expression]
    items = [get(e) for e in (list(contracts) or list(spec))]
    pairs = [(get(a), get(b), _checks_.refines(get(a), get(b), timeout=timeout).status if check else None)
             for a, b in refines]
    def written(c, out):              # the contracts of the file in c, in order
        if c.origin:
            for o in c.origin[1]:
                written(o, out)
        elif all(x is not c for x in out):
            out.append(c)
        return out
    atoms = []
    for c in items + [x for a, b, _ in pairs for x in (a, b)]:
        written(c, atoms)
    marks = []
    for c in atoms:                   # a composition is not GR(1): its parts are checked
        if verify:
            marks.append((c, _checks_.verify(c, core=True, timeout=timeout)))
        if realizable:
            marks.append((c, "realizable", gr1.is_realizable(c, core=True, timeout=timeout)))
    envs = []
    for model, name in environments:
        c = get(name)
        m = _model(model, c)
        envs.append((m, c, _checks_.is_environment(m, c, timeout=timeout).status if check else None))
    return diagram(*items, refines=pairs, checks=marks, environments=envs,
                   open=[get(e) for e in open], title=title or os.path.basename(path))


def notebook(item, **kwargs) -> str:
    """The page in an iframe, for Jupyter."""
    page = _html.escape(html(item, **kwargs), quote=True)
    return f'<iframe srcdoc="{page}" style="width:100%;height:520px;border:0"></iframe>'


def serve(make, path: str, port: int = 8765, host: str = "127.0.0.1",
          elk: Optional[str] = None) -> int:
    """Serves the diagram make() on http://host:port, and draws it again
    whenever the file at path changes: the page in the browser takes the new
    drawing in place, and keeps the view. When the file cannot be drawn, the
    page keeps the last diagram and says why. Stops with Ctrl-C."""
    import http.server
    import threading

    state = {"mtime": None, "version": 0, "data": None, "error": None, "page": None}
    lock = threading.Lock()

    drawing = threading.Lock()          # one drawing at a time: the others serve what is there

    def refresh():
        mtime = os.path.getmtime(path) if os.path.exists(path) else None
        if mtime == state["mtime"] or not drawing.acquire(blocking=False):
            return
        try:
            _draw(mtime)
        finally:
            drawing.release()

    def _draw(mtime):
        try:
            d, error = make(), None
        except Exception as e:          # the error goes to the page, the last diagram stays
            d, error = None, (str(e) if isinstance(e, (ValueError, RuntimeError))
                              else f"{type(e).__name__}: {e}")
        with lock:
            state.update(mtime=mtime, version=state["version"] + 1, error=error)
            if d is not None:
                state["data"] = d
                if state["page"] is None:
                    state["page"] = html(d, elk=elk)

    def body(route):
        if route == "/version":
            return "text/plain", str(state["version"])
        if route == "/data":
            reply = {"error": state["error"]} if state["error"] else {"data": state["data"]}
            return "application/json", json.dumps(reply)
        if state["page"] is None:       # nothing could be drawn yet
            return "text/html; charset=utf-8", (
                "<!doctype html><meta charset=utf-8><pre style='color:#b03a2e;padding:16px'>"
                + _html.escape(state["error"] or "") + "</pre>" + _RELOAD)
        return "text/html; charset=utf-8", state["page"]

    class Handler(http.server.BaseHTTPRequestHandler):
        def do_GET(self):
            refresh()
            with lock:
                kind, text = body(self.path)
            data = text.encode()
            self.send_response(200)
            self.send_header("Content-Type", kind)
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(data)

        def log_message(self, *args):
            pass

    refresh()
    server = http.server.ThreadingHTTPServer((host, port), Handler)
    print(f"Drawing {path} on http://{host}:{port} (Ctrl-C to stop)", flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
    return 0


# Served pages ask for /version every second; when it changes they take the
# new drawing in place (the view stays), or say why the file cannot be drawn.
_RELOAD = """<script>
(function () {
  if (!location.protocol.startsWith("http")) return;
  let seen = null;
  setInterval(async () => {
    try {
      const v = await (await fetch("/version", { cache: "no-store" })).text();
      if (seen === null) { seen = v; return; }
      if (v === seen) return;
      seen = v;
      if (typeof setData !== "function") { location.reload(); return; }
      const r = await (await fetch("/data", { cache: "no-store" })).json();
      if (r.data) await setData(r.data); else stale(r.error);
    } catch (e) {}
  }, 1000);
})();
</script>"""


_PAGE = r"""<!doctype html>
<html><head><meta charset="utf-8"><title>__TITLE__</title>
<style>
  :root { --ink:#1d2526; --soft:#667374; --faint:#97a3a4; --teal:#17393b; --line:#b6c1c1; --fill:#f5f7f7;
          --wire:#7d8c8d; --ok:#2f7d4f; --bad:#b03a2e; --warn:#c07a14; --grid:#d3dbdb; --ground:#fcfdfd; }
  html, body { height:100%; overflow:hidden; }
  body { margin:0; font:13px/1.4 -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; color:var(--ink); background:var(--ground); }
  #top { position:fixed; top:0; left:0; right:0; z-index:5; display:flex; justify-content:space-between; align-items:flex-start;
         padding:10px 12px 0 18px; pointer-events:none; }
  #top > * { pointer-events:auto; }
  .tools { display:flex; gap:2px; align-items:center; background:#fff; border:1px solid #e3e8e8; border-radius:8px;
           padding:2px; box-shadow:0 1px 4px rgba(0,0,0,.06); position:relative; margin-left:auto; }
  .tools button { width:28px; height:26px; border:1px solid transparent; background:none; border-radius:6px;
                  cursor:pointer; color:var(--soft); display:flex; align-items:center; justify-content:center; padding:0; }
  .tools button:hover { background:var(--fill); color:var(--teal); }
  .tools button.on { background:#e6eeee; color:var(--teal); }
  .tools button svg { width:16px; height:16px; fill:none; stroke:currentColor; stroke-width:1.4;
               stroke-linecap:round; stroke-linejoin:round; }
  .tools .sep { width:1px; height:18px; background:#e3e8e8; margin:0 6px; }
  #export { display:none; position:absolute; right:12px; top:46px; background:#fff; border:1px solid #d5dddd;
            border-radius:8px; padding:4px; gap:4px; box-shadow:0 4px 14px rgba(0,0,0,.08); z-index:7; }
  #export button { font:600 12px Helvetica, Arial, sans-serif; border:1px solid #d5dddd; background:#fff;
                   border-radius:6px; padding:4px 10px; cursor:pointer; color:var(--teal); }
  #export button:hover { background:var(--fill); }
  @media print { body > *:not(#print) { display:none !important; } #print { display:block !important; }
                 #print svg { width:100%; height:auto; } @page { margin:12mm; } }
  #help { position:absolute; right:12px; top:46px; background:#fff; border:1px solid #d5dddd; border-radius:8px;
          box-shadow:0 6px 24px rgba(0,0,0,.12); padding:12px 14px; font:12px 'Arial Unicode MS', Helvetica, Arial, sans-serif;
          color:var(--soft); display:none; z-index:11; }
  #help div { display:flex; gap:10px; align-items:center; margin:6px 0; white-space:nowrap; }
  #help svg { flex:none; }
  #context { display:none; gap:8px; flex-wrap:wrap; padding-top:4px; }
  #stale { display:none; position:fixed; left:12px; bottom:12px; max-width:min(720px, 70%); z-index:6;
           background:#fff6f4; border:1px solid var(--bad); border-radius:6px; padding:7px 10px;
           font:12px/1.45 Helvetica, Arial, sans-serif; color:#7a2a20; white-space:pre-wrap; }
  #stale b { display:block; margin-bottom:2px; }
  #tip { display:none; position:fixed; z-index:20; max-width:420px; pointer-events:none;
         background:#1d2526; color:#fff; border-radius:4px; padding:5px 8px;
         font:12px/1.4 Helvetica, Arial, sans-serif; white-space:pre-wrap; }
  #busy { display:none; position:fixed; right:14px; bottom:12px; z-index:6; padding:5px 12px;
          border-radius:12px; background:var(--teal); color:#fff; font:600 12px Helvetica, Arial, sans-serif;
          box-shadow:0 1px 4px rgba(0,0,0,.2); }
  #context span { display:inline-flex; align-items:center; gap:7px; border:1px solid #d5dddd; border-radius:6px;
                  padding:2px 9px 2px 2px; font:11.5px Menlo, Monaco, monospace; color:var(--ink);
                  white-space:nowrap; background:#fff; }
  #context i { font:600 9.5px -apple-system, Helvetica, Arial, sans-serif; font-style:normal; color:#fff;
               background:#6f7f80; border-radius:4px; padding:2px 5px; }
  #view { position:fixed; inset:0; overflow:hidden; cursor:grab; touch-action:none; }
  #view.moving { cursor:grabbing; }
  #view > svg { display:block; width:100%; height:100%; }
  svg text { font-family:-apple-system, "Segoe UI", Helvetica, Arial, sans-serif; }
  .f { font-family:Menlo, Monaco, monospace; font-size:11.5px; fill:var(--ink); }
  .h { font-weight:600; font-size:12.5px; fill:var(--ink); cursor:pointer; }
  .s { font-family:'Arial Unicode MS', Helvetica, Arial, sans-serif; font-size:11px; fill:var(--soft); }
  .w { font-family:Menlo, Monaco, monospace; font-size:10px; fill:var(--soft); }
  .halo { paint-order:stroke; stroke:#fff; stroke-width:3px; stroke-linejoin:round; }
  .icon { cursor:pointer; }
  #pop { position:fixed; left:50%; top:12%; transform:translateX(-50%); max-width:min(860px, 90vw); max-height:72vh;
         background:#fff; border:1px solid #d5dddd; border-radius:10px; box-shadow:0 10px 36px rgba(0,0,0,.16);
         display:none; flex-direction:column; z-index:10; }
  #pop header { padding:9px 16px; border-bottom:1px solid #e3e8e8; display:flex; gap:20px; align-items:center; }
  #pop header b { font-size:13px; font-weight:600; flex:1; }
  #pop button { font:12px -apple-system, Helvetica, Arial, sans-serif; border:1px solid #d5dddd; background:#fff;
                border-radius:6px; padding:2px 10px; cursor:pointer; color:var(--soft); }
  #popview { overflow:auto; padding:6px 18px 16px; }
  table.fsm { border-collapse:collapse; font:11.5px Menlo, Monaco, monospace; }
  table.fsm th { font:600 11.5px -apple-system, Helvetica, Arial, sans-serif; text-align:left; color:var(--soft);
                 padding:6px 16px 6px 0; border-bottom:1px solid #e3e8e8; position:sticky; top:0; background:#fff; }
  table.fsm td { padding:3px 16px 3px 0; border-bottom:1px solid #f1f4f4; vertical-align:top; }
  table.fsm td.st { color:var(--teal); font-weight:600; border-bottom:1px solid #e3e8e8; }
  table.fsm td.when { color:var(--soft); }
</style></head><body>
<div id="top"><div id="context"></div>
  <span class="tools">
    <button id="all" title="Open all" onclick="openAll(!allOpen)"><svg viewBox="0 0 16 16"><path d="M4.5 6.5 8 3l3.5 3.5M4.5 9.5 8 13l3.5-3.5"/></svg></button>
    <span class="sep"></span>
    <button title="Fit the diagram to the window (0); the wheel zooms where it points, a drag moves" onclick="fit()"><svg viewBox="0 0 16 16"><path d="M2 5.5V2h3.5M10.5 2H14v3.5M14 10.5V14h-3.5M5.5 14H2v-3.5"/></svg></button>
    <span class="sep"></span>
    <button class="on" title="Names of the ports" onclick="flip(this, 'names')"><svg viewBox="0 0 16 16"><rect x="4.5" y="3" width="7" height="10" rx="1.5"/><path d="M1.5 6h3M1.5 10h3M11.5 8h3"/></svg></button>
    <button class="on" title="Types of the ports (ranges, reals, bit vectors)" onclick="flip(this, 'types')"><svg viewBox="0 0 16 16"><path d="M1.5 8h13" stroke-width="2.4"/><path d="M6.5 11.5l3-7"/></svg></button>
    <button class="on" title="Transition labels of the models: the inputs that take each transition" onclick="flip(this, 'conditions')"><svg viewBox="0 0 16 16"><circle cx="3" cy="11" r="1.8"/><circle cx="13" cy="11" r="1.8"/><path d="M4.8 11h6.4"/><rect x="5" y="3" width="6" height="4" rx="1"/></svg></button>
    <button class="on" title="Results of the checks (A, G, R, ⪯, M ⊨ C, E ⊨ A) and their cores" onclick="flip(this, 'checks')"><svg viewBox="0 0 16 16"><rect x="1.5" y="4" width="13" height="8" rx="3"/><path d="M5.5 8l1.8 1.8L10.5 6.3"/></svg></button>
    <span class="sep"></span>
    <button title="Export the whole diagram (SVG, PNG, PDF)" onclick="exportMenu()"><svg viewBox="0 0 16 16"><path d="M8 2v8M4.8 6.8 8 10l3.2-3.2M2.5 11.5v2h11v-2"/></svg></button>
    <button title="What the shapes mean" onclick="help()"><svg viewBox="0 0 16 16"><circle cx="8" cy="8" r="6.2"/><path d="M6.3 6.3a1.8 1.8 0 1 1 2.4 1.7c-.5.2-.7.6-.7 1.1v.4"/><circle cx="8" cy="11.6" r=".4"/></svg></button>
  </span><div id="help"></div><div id="export"><button onclick="exportAs('svg')">SVG</button><button onclick="exportAs('png')">PNG</button><button onclick="exportAs('pdf')">PDF</button></div></div>
<div id="view"></div>
<div id="stale"></div>
<div id="busy">checking…</div>
<div id="tip"></div>
<div id="pop"><header><b id="poptitle"></b><button onclick="closePop()">close</button></header><div id="popview"></div></div>
__ELK__
<script>
let DATA = __DATA__;
const LINE = 17, PAD = 10, PORT = 6, HEAD = 28, ICON = 16, INLINE_STATES = 8;
const ctx = document.createElement("canvas").getContext("2d");
function textWidth(s, font) { ctx.font = font; return ctx.measureText(s).width; }
const FONT_F = "11.5px Menlo, Monaco, monospace", FONT_H = "600 12.5px Helvetica, Arial, sans-serif",
      FONT_S = "11px 'Arial Unicode MS', Helvetica, Arial, sans-serif", FONT_W = "10px Menlo, Monaco, monospace";
const show = { names: true, types: true, conditions: true, checks: true };

// What is open: the contract ("c:"), the computed composition ("k:"), and
// the model ("m:") of each box. At first, only the names show.
const open = new Set();
const toggle = key => () => { open.has(key) ? open.delete(key) : open.add(key); render(); };
function flip(b, what) {
  show[what] = !show[what]; b.classList.toggle("on", show[what]);
  if (what === "conditions") for (const k in MODELS) delete MODELS[k];
  render();
}
// The core in focus, by box: the index of the check whose core is shown,
// its terms on a red ground.
const focus = {};
// The boxes whose inner boxes are hidden (folded), by id.
const folded = new Set();
const inside = n => n.children && n.children.length;
const fold = id => () => { folded.has(id) ? folded.delete(id) : folded.add(id); render(); };

const byId = {};
(function index(ns) { ns.forEach(n => { byId[n.id] = n; index(n.children || []); }); })(DATA.nodes);
let boxes = Object.values(byId);
const states = n => n.model && n.model.fsm && n.model.fsm.states;
const inlineModel = n => states(n) && n.model.fsm.count <= INLINE_STATES;
let allOpen = false;
// The boxes the diagram asks to open: the contract, a small model, a core.
function openAsked() {
  Object.values(byId).filter(n => n.open).forEach(n => {
    if (n.contract) open.add("c:" + n.id);
    if (inlineModel(n)) open.add("m:" + n.id);
    const i = (n.checks || []).findIndex(c => c.core && c.status === "invalid");
    if (i >= 0) { focus[n.id] = i; if (n.checks[i].core.part === "composed") open.add("k:" + n.id); }
  });
}
function openAll(on) {
  boxes.forEach(n => ["c:", "k:", "m:"].forEach(p => {
    const key = p + n.id, there = p === "c:" ? n.contract : p === "k:" ? n.composed : inlineModel(n);
    if (on && there) open.add(key); else open.delete(key); }));
  if (on) folded.clear();
  allOpen = on; refit = true;
  const b = document.getElementById("all");
  b.setAttribute("data-tip", on ? "Close all" : "Open all");
  b.querySelector("path").setAttribute("d", on ? "M4.5 3 8 6.5 11.5 3M4.5 13 8 9.5l3.5 3.5" : "M4.5 6.5 8 3l3.5 3.5M4.5 9.5 8 13l3.5-3.5");
  render();
}

// A long formula on several lines, cut before an operator.
const WIDTH = 60;
function wrap(f) {
  const out = []; let rest = f;
  while (rest.length > WIDTH) {
    let cut = -1;
    for (const op of [" & ", " | ", " -> ", " <-> ", " "]) {
      const i = rest.lastIndexOf(op, WIDTH); if (i > WIDTH / 3) { cut = i; break; } }
    if (cut < 0) cut = WIDTH;
    out.push(rest.slice(0, cut)); rest = "  " + rest.slice(cut).trimStart();
  }
  out.push(rest); return out;
}
// Each part opens with a tag: A (assumes), G (guarantees), E (an environment).
// With a core in focus, its terms are on a red ground: [kind, tag, text, in the core].
function terms(c, out, core) {
  const part = (tag, fs, hit) => fs.forEach((f, j) => wrap(f).forEach((l, i) =>
    out.push(["f", i || j ? "" : tag, l, hit.includes(j)])));
  part("A", c.A, core ? core.A : []);
  if (c.E) part("E", c.E, []);
  else part("G", c.G.length ? c.G : ["true"], core ? core.G : []);
}
const focused = n => show.checks && n.id in focus ? n.checks[focus[n.id]].core : null;

// The lines under the name. A long contract (a computed one, often) shows
// its first lines; the rest opens in a panel.
const LONG = 14, SHORT = 8;
function lines(n) {
  const out = [], core = focused(n);
  const part = (c, which) => {
    const ts = [];
    terms(c, ts, core && core.part === which ? core : null);
    if (ts.length <= LONG) { out.push(...ts); return; }
    const parts = [];                  // A and G cut apart: the first lines of each
    ts.forEach(t => (t[1] || !parts.length) ? parts.push([t]) : parts[parts.length - 1].push(t));
    let left = SHORT;
    parts.forEach((p, i) => {
      const rest = parts.slice(i + 1).reduce((s, q) => s + q.length, 0);
      const keep = Math.max(1, i === parts.length - 1 ? left : Math.max(Math.floor(SHORT / parts.length), SHORT - rest));
      let room = keep - p.filter(t => t[3]).length;   // the lines of a core in focus are kept first
      const shown = p.filter(t => t[3] || room-- > 0);
      if (shown[0] !== p[0]) shown[0] = [shown[0][0], p[0][1], shown[0][2], shown[0][3]];
      out.push(...shown);
      if (p.length > shown.length) out.push(["more", "", "⋯ " + (p.length - shown.length) + " more lines", c]);
      left -= Math.min(keep, shown.length);
    });
  };
  if (n.note) out.push(["s", "", n.note]);
  if (n.contract && open.has("c:" + n.id)) {
    if (n.composed) out.push(["s", "", "the contract " + n.contract.name]);
    part(n.contract, "contract");
  }
  if (n.composed && open.has("k:" + n.id)) {
    out.push(["s", "", "the composition " + n.composed.name]);
    part(n.composed, "composed");
  }
  return out;
}
const lineWidth = ([k, tag, t]) => (k === "f" ? 20 : 0) + textWidth(t, k === "f" ? FONT_F : FONT_S);
function showTerms(n, c) {     // the whole contract, in the panel
  const esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;"), ts = [];
  terms(c, ts, null);
  document.getElementById("poptitle").textContent = n.title + " · contract " + c.name;
  document.getElementById("popview").innerHTML = "<table class='fsm'>" + ts.map(([k, tag, t]) =>
    "<tr><td class='st'>" + (tag || "") + "</td><td>" + esc(t) + "</td></tr>").join("") + "</table>";
  document.getElementById("pop").style.display = "flex";
}

function icons(n) {
  const out = [];
  if (n.contract) out.push(["contract", "c:" + n.id, "contract " + n.contract.name]);
  if (n.composed) out.push(["composed", "k:" + n.id, "the composition " + n.composed.name]);
  if (n.model) {             // a model with nothing to draw has its icon, grey
    const f = n.model.fsm;
    const from = { controller: " (synthesized by slugs)", rtl: " (RTL)", json: " (JSON)", smv: " (SMV)" }[n.model.source] || "";
    const kind = n.model.source === "controller" ? "synth" : "model";
    if (states(n)) out.push([kind, "m:" + n.id, "model " + n.model.name + from + ", " + f.count + " states"]);
    else out.push([kind, null, "model " + n.model.name + from + (f && f.too_many
      ? ": more than " + f.count + " states, too many to show" : ": no registers, its outputs are functions of its inputs")]);
  }
  return out;
}
// A tag with the result of a check: green when it holds, red when it does
// not, grey when the check did not finish, white when it was not run.
// Arial Unicode draws ∧, ⊨ and ⊑ as large as the letters; Helvetica does not.
const FONT_T = "600 11px 'Arial Unicode MS', 'Segoe UI', 'DejaVu Sans', system-ui, sans-serif";
const COLOR = { valid: "var(--ok)", invalid: "var(--bad)", unknown: "var(--faint)" };
// ⪯ is drawn: at this size the fonts make it look like ≤
const REF = s => s.startsWith("⪯"), rest = s => s.slice(1).trim();
const tagWidth = s => REF(s) ? 22 + (rest(s) ? textWidth(rest(s), FONT_T) + 2 : 0) : textWidth(s, FONT_T) + 12;
function tag(g, x, y, s, status) {
  const c = COLOR[status], ink = c ? "#fff" : "var(--soft)";
  el("rect", { x, y, width: tagWidth(s), height: 16, rx: 4, fill: c || "#fff", stroke: c || "var(--line)",
               "stroke-width": 1 }, g);
  if (REF(s)) {               // refinement as in the book: ≺, curved, over a bar
    el("path", { d: `M${x + 15.8} ${y + 2.2} C${x + 14.6} ${y + 5.6} ${x + 11} ${y + 6.9} ${x + 6} ${y + 7.1}`
                    + ` C${x + 11} ${y + 7.3} ${x + 14.6} ${y + 8.6} ${x + 15.8} ${y + 12}`
                    + ` M${x + 6.2} ${y + 13.9} L${x + 15.8} ${y + 13.9}`,
                 fill: "none", stroke: ink, "stroke-width": 1.5, "stroke-linecap": "round" }, g);
    if (rest(s)) text(g, x + 20, y + 11.8, "", rest(s)).setAttribute("style", "font:" + FONT_T + ";fill:" + ink); }
  else { const t = text(g, x + 6, y + 11.8, "", s);
         t.setAttribute("style", "font:" + FONT_T + ";fill:" + ink); }
  return tagWidth(s);
}
const SAYS = { valid: "holds", invalid: "does not hold", unknown: "not decided" };
const WHAT = { "A": "compatible: the assumptions A are satisfiable",
               "G": "consistent: A → G is satisfiable",
               "A∧G": "feasible: A ∧ G is satisfiable",
               "R": "realizable (GR(1))",
               "WS": "well-separated: the component cannot win by making the environment break A",
               "¬V": "not vacuous: each liveness guarantee asks something of the component",
               "M ⊨ C": "the model implements the contract",
               "E ⊨ A": "the environment satisfies the assumptions",
               "⪯": "what is inside refines this contract" };
const checks = n => show.checks && n.checks ? n.checks : [];

const MODELS = {};       // the layout of each open model drawn in its box
function size(n) {
  const ls = lines(n);
  const top = textWidth(n.title, FONT_H) + icons(n).length * (ICON + 4) + 14
            + checks(n).reduce((w, c) => w + tagWidth(c.name) + 4, checks(n).length ? 6 : 0)
            + (n.badge && show.checks ? tagWidth(n.badge.text) + 18 : 0) + (inside(n) ? ICON + 6 : 0);
  let w = Math.max(top, ...ls.map(lineWidth)) + 2 * PAD;
  let h = HEAD + ls.length * LINE + (ls.length ? 8 : 0);
  if (folded.has(n.id)) h = Math.max(h, HEAD + 10);
  const m = open.has("m:" + n.id) && MODELS[n.id];
  if (m) { w = Math.max(w, m.w + 2 * PAD); h += m.h + 10; }
  return { ls, w: Math.max(w, 110), h, m };
}

// The label of a port: its name, its type, both, or nothing, as the two buttons say.
const portText = (n, v) => { const t = show.types && n.types && n.types[v];
  return [show.names ? v : null, t ? t.label : null].filter(x => x).join(" "); };
function ports(n) {
  if (n.bare) return [];
  const ps = [], lab = v => portText(n, v) ? [{ text: portText(n, v), width: textWidth(portText(n, v), FONT_W), height: 11 }] : [];
  (n.inputs || []).forEach((v, i) => ps.push({ id: n.id + ":in:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "WEST", "elk.port.index": String(n.inputs.length - i) } }));
  (n.outputs || []).forEach((v, i) => ps.push({ id: n.id + ":out:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "EAST", "elk.port.index": String(i) } }));
  return ps;
}
function typeOf(n, v) {
  if (n.types && n.types[v]) return n.types[v];
  for (const c of n.children || []) { const t = typeOf(c, v); if (t) return t; }
  return null;
}

const FLOW = { "elk.layered.spacing.nodeNodeBetweenLayers": "72", "elk.spacing.nodeNode": "30",
               "elk.layered.spacing.edgeNodeBetweenLayers": "16", "elk.spacing.edgeEdge": "8",
               "elk.layered.nodePlacement.strategy": "NETWORK_SIMPLEX",
               "elk.layered.crossingMinimization.semiInteractive": "true",
               "elk.layered.cycleBreaking.strategy": "INTERACTIVE",
               "elk.layered.layering.strategy": "INTERACTIVE" };
let widen = {};
const KIND = {};
// The column of each box inside n, by the flow of its wires: the longest
// path from the inputs of n, a wire back to an earlier box (a loop) left out.
// Viewpoints stacked in one box share a column.
function columns(n) {
  const col = {}, ids = n.children.map(c => c.id);
  if (n.layout === "stack") { ids.forEach(id => { col[id] = 0; }); return col; }
  const next = {};
  ids.forEach(id => { next[id] = []; });
  (n.edges || []).forEach(w => { if (w.from && w.from[0] && w.to[0] && next[w.from[0]] && w.from[0] !== w.to[0])
    next[w.from[0]].push(w.to[0]); });
  const state = {}, order = [];
  const visit = id => { state[id] = 1;            // depth first, in the order of the list
    next[id] = next[id].filter(t => state[t] !== 1);  // a wire back to a box on the path: a loop
    next[id].forEach(t => { if (!state[t]) visit(t); });
    state[id] = 2; order.unshift(id); };
  ids.forEach(id => { if (!state[id]) visit(id); });
  order.forEach(id => { col[id] = col[id] || 0; next[id].forEach(t => { col[t] = Math.max(col[t] || 0, col[id] + 1); }); });
  return col;
}
// The groups of boxes inside n that are wired to each other (the ports of
// n left out): null when there is one group, or when n stacks viewpoints.
function bands(n) {
  if (n.layout === "stack" || n.children.length < 2) return null;
  const up = {};
  const root = id => up[id] === id ? id : (up[id] = root(up[id]));
  n.children.forEach(c => { up[c.id] = c.id; });
  (n.edges || []).forEach(w => { if (w.from && w.from[0] && w.to[0] && w.from[0] in up && w.to[0] in up)
    up[root(w.from[0])] = root(w.to[0]); });
  const rows = new Map();
  n.children.forEach(c => { const r = root(c.id); if (!rows.has(r)) rows.set(r, []); rows.get(r).push(c); });
  return rows.size > 1 ? [...rows.values()] : null;
}
function toElk(n, column, row) {
  const sz = size(n);
  const base = { id: n.id, ports: ports(n), x: (column || 0) * 600, y: (row || 0) * 40,
    layoutOptions: { "elk.portConstraints": "FIXED_ORDER", "elk.nodeSize.constraints": "MINIMUM_SIZE",
      "elk.portLabels.placement": "OUTSIDE", "elk.spacing.portPort": "13",
      "elk.nodeSize.minimum": "(" + sz.w + "," + Math.max(sz.h, (Math.max((n.inputs || []).length, (n.outputs || []).length) + 1) * 17) + ")" } };
  if (!inside(n) || folded.has(n.id)) { base.width = sz.w; base.height = sz.h; return base; }
  const col = columns(n);
  const edges = (n.edges || []).filter(w => !w.open).map((w, i) => {
    const id = n.id + ":e" + i; KIND[id] = typeOf(n, w.var);
    return { id, w, sources: [(w.from[0] || n.id) + ":" + w.from[1] + ":" + w.from[2]],
             targets: [(w.to[0] || n.id) + ":" + w.to[1] + ":" + w.to[2]] }; });
  const strip = es => es.map(({ w, ...ed }) => ed);
  const rows = bands(n);
  if (!rows) {
    base.children = n.children.map((c, i) => toElk(c, col[c.id], i));
    base.edges = strip(edges);
  } else {                     // each group of connected boxes in a row of its own, laid out apart
    base.children = rows.map((cs, r) => {
      const ids = new Set(cs.map(c => c.id));
      return { id: n.id + ":row" + r, x: 0, y: r * 40, children: cs.map((c, i) => toElk(c, col[c.id], i)),
               edges: strip(edges.filter(ed => ids.has(ed.w.from[0]) && ids.has(ed.w.to[0]))),
               layoutOptions: Object.assign({}, FLOW, { "elk.padding": "[top=0,left=0,bottom=0,right=0]",
                                                        "elk.alignment": "LEFT" }) };
    });
    base.edges = strip(edges.filter(ed => !ed.w.from[0] || !ed.w.to[0]));   // to and from the ports of n
  }
  Object.assign(base.layoutOptions, FLOW, {
    "elk.padding": "[top=" + (sz.h + 10) + ",left=28,bottom=18,right=" + (28 + (widen[n.id] || 0)) + "]" });
  return base;
}

const NS = "http://www.w3.org/2000/svg";
function el(tag, attrs, parent) { const e = document.createElementNS(NS, tag);
  for (const k in attrs) e.setAttribute(k, attrs[k]); if (parent) parent.appendChild(e); return e; }
function text(parent, x, y, cls, s) { const t = el("text", { x, y, class: cls }, parent); t.textContent = s; return t; }
// The explanation of a part, shown when the pointer rests on it (see #tip).
function tip(parent, s) { parent.setAttribute("data-tip", s); }

// The icons: a card for a contract, ⊗ for a computed composition, two
// states and a move for a model, with a spark when it was synthesized.
function icon(g, kind, x, y, on, title, off) {
  const c = el("g", { class: off ? "" : "icon" }, g), ink = off ? "var(--line)" : "var(--teal)", fg = on ? "#fff" : ink;
  el("rect", { x, y, width: ICON, height: ICON, rx: 4, fill: on ? ink : "#fff",
               stroke: ink, "stroke-width": 1.1 }, c);
  if (kind === "contract") {
    el("rect", { x: x + 4, y: y + 3.5, width: 8, height: 9, rx: 1.5, fill: "none", stroke: fg, "stroke-width": 1.1 }, c);
    el("line", { x1: x + 5.8, y1: y + 6.5, x2: x + 10.2, y2: y + 6.5, stroke: fg, "stroke-width": 1 }, c);
    el("line", { x1: x + 5.8, y1: y + 9.3, x2: x + 10.2, y2: y + 9.3, stroke: fg, "stroke-width": 1 }, c);
  } else if (kind === "composed") {        // ⊗, the composition of contracts
    el("circle", { cx: x + 8, cy: y + 8, r: 4.6, fill: "none", stroke: fg, "stroke-width": 1.1 }, c);
    el("path", { d: `M${x + 4.8} ${y + 4.8} L${x + 11.2} ${y + 11.2} M${x + 11.2} ${y + 4.8} L${x + 4.8} ${y + 11.2}`,
                 stroke: fg, "stroke-width": 1.1 }, c);
  } else {                   // a model; synthesized: with a spark at the top left
    if (kind === "synth") el("path", { d: `M${x + 4.5} ${y + 2.8} L${x + 4.5} ${y + 6.8} M${x + 2.5} ${y + 4.8} L${x + 6.5} ${y + 4.8}`,
                                       stroke: fg, "stroke-width": 1.1, "stroke-linecap": "round" }, c);
    el("circle", { cx: x + 4.8, cy: y + 10.8, r: 2.3, fill: "none", stroke: fg, "stroke-width": 1.1 }, c);
    el("circle", { cx: x + 11.2, cy: y + 5.2, r: 2.3, fill: "none", stroke: fg, "stroke-width": 1.1 }, c);
    el("line", { x1: x + 6.6, y1: y + 9.2, x2: x + 9.4, y2: y + 6.8, stroke: fg, "stroke-width": 1.1 }, c);
  }
  tip(c, title);
  return c;
}

const STYLE = {
  contract:  { rx: 10, fill: "#fff", stroke: "var(--teal)", width: 1.2, dash: "none" },
  component: { rx: 2, fill: "#fff", stroke: "var(--ink)", width: 1.8, dash: "none" },
  group:     { rx: 10, fill: "var(--fill)", stroke: "var(--line)", width: 1, dash: "none" },
  hole:      { rx: 10, fill: "url(#hatch)", stroke: "var(--teal)", width: 1.2, dash: "5 4" },
  environment: { rx: 2, fill: "var(--fill)", stroke: "var(--soft)", width: 1.4, dash: "none" } };

function drawNode(g, e, ox, oy) {
  if (!byId[e.id]) {          // a row of boxes: nothing to draw of its own
    (e.children || []).forEach(c => drawNode(g, c, ox + e.x, oy + e.y));
    (e.edges || []).forEach(ed => drawEdge(g, ed, ox + e.x, oy + e.y));
    return;
  }
  const n = byId[e.id], x = ox + e.x, y = oy + e.y, sz = size(n);
  const box = el("g", { "data-box": n.id }, g);
  const st = n.kind === "group" && n.badge ? Object.assign({}, STYLE.group, { stroke: "var(--teal)", width: 1.2 })
                                           : STYLE[n.kind] || STYLE.contract;
  el("rect", { x, y, width: e.width, height: e.height, rx: st.rx, fill: st.fill, stroke: st.stroke,
               "stroke-width": st.width, "stroke-dasharray": st.dash }, box);
  if (n.kind === "component" || n.kind === "environment") el("line", { x1: x, y1: y + HEAD - 2, x2: x + e.width, y2: y + HEAD - 2,
                                           stroke: "#e3e8e8", "stroke-width": 1 }, box);
  const name = text(box, x + PAD, y + 18, "h", n.title);
  if (n.contract) { name.onclick = toggle("c:" + n.id); tip(name, "Click: open or close the contract"); }
  let ix = x + PAD + textWidth(n.title, FONT_H) + 10;
  icons(n).forEach(([kind, key, title]) => {
    const on = key && open.has(key), c = icon(box, kind, ix, y + 6, on, title, !key);
    if (key) c.onclick = (kind === "model" || kind === "synth") && !on && !inlineModel(n) ? () => showModel(n) : toggle(key);
    ix += ICON + 4; });
  if (checks(n).length) ix += 6;
  checks(n).forEach((c, i) => {      // a click on a check that has a core shows the core
    const t = el("g", {}, box), on = focus[n.id] === i;
    ix += tag(t, ix, y + 6, c.name, c.status) + 4;
    tip(t, (WHAT[c.name] || c.name) + " — " + (SAYS[c.status] || "not checked") + (c.reason ? " (" + c.reason + ")" : "")
           + (c.core ? (on ? ". Click: hide the core" : ". Click: show the core") : ""));
    if (on) el("rect", { x: ix - 4 - tagWidth(c.name) - 2, y: y + 4, width: tagWidth(c.name) + 4, height: 20, rx: 5,
                         fill: "none", stroke: COLOR[c.status], "stroke-width": 1 }, t);
    if (c.core) { t.setAttribute("class", "icon");
      t.onclick = () => { if (on) delete focus[n.id];
        else { focus[n.id] = i; open.add((c.core.part === "composed" ? "k:" : "c:") + n.id); } render(); }; } });
  let rx = x + e.width - PAD;
  if (inside(n)) {             // fold: hide or show the boxes inside
    const f = el("g", { class: "icon" }, box), shut = folded.has(n.id), cx = rx - ICON / 2, cy = y + 14;
    el("rect", { x: rx - ICON, y: y + 6, width: ICON, height: ICON, rx: 4, fill: shut ? "var(--teal)" : "#fff",
                 stroke: "var(--teal)", "stroke-width": 1.1 }, f);
    el("path", { d: shut ? `M${cx - 3.5} ${cy - 1.5} L${cx} ${cy + 2} L${cx + 3.5} ${cy - 1.5}`
                         : `M${cx - 3.5} ${cy + 1.5} L${cx} ${cy - 2} L${cx + 3.5} ${cy + 1.5}`,
                 fill: "none", stroke: shut ? "#fff" : "var(--teal)", "stroke-width": 1.4,
                 "stroke-linecap": "round", "stroke-linejoin": "round" }, f);
    tip(f, (shut ? "Show the " : "Hide the ") + n.children.length + " boxes inside");
    f.onclick = fold(n.id);
    rx -= ICON + 6;
  }
  if (n.badge && show.checks) { const b = el("g", {}, box), w = tagWidth(n.badge.text);
    tag(b, rx - w, y + 6, n.badge.text, n.badge.status);
    tip(b, (WHAT[n.badge.text.split(" of ")[0]] || n.badge.text) + " — " + (SAYS[n.badge.status] || "not checked")
           + (inside(n) ? ". Click: hide or show the boxes inside" : ""));
    if (inside(n)) { b.setAttribute("class", "icon"); b.onclick = fold(n.id); } }
  sz.ls.forEach(([k, label, s, core], i) => {
    const ty = y + HEAD + 12 + i * LINE, row = el("g", {}, box);
    if (k === "f") {
      if (core) el("rect", { x: x + 4, y: ty - 12, width: e.width - 8, height: LINE, rx: 3, fill: "#f6dcd9" }, row);
      if (label) { el("rect", { x: x + PAD, y: ty - 10, width: 13, height: 13, rx: 3, fill: "var(--teal)" }, row);
                   const t = text(row, x + PAD + 6.5, ty, "s", label); t.setAttribute("text-anchor", "middle");
                   t.setAttribute("style", "font-size:9.5px;font-weight:700;fill:#fff"); }
      text(row, x + PAD + 20, ty, "f", s); }
    else if (k === "more") {
      const t = text(row, x + PAD + 20, ty, "s", s);
      t.setAttribute("style", "fill:var(--teal);cursor:pointer;text-decoration:underline");
      tip(t, "the whole contract");
      t.onclick = () => showTerms(n, core); }
    else text(row, x + PAD, ty, "s", s); });
  if (sz.m) drawModel(box, sz.m, x + PAD, y + HEAD + sz.ls.length * LINE + (sz.ls.length ? 8 : 0) + 4);
  (e.ports || []).forEach(p => {
    el("rect", { x: x + p.x, y: y + p.y, width: p.width, height: p.height, fill: "var(--teal)" }, g);
    (p.labels || []).forEach(l => text(g, x + p.x + l.x, y + p.y + l.y + 9, "w", l.text)); });
  (e.children || []).forEach(c => drawNode(g, c, x, y));
  (e.edges || []).forEach(ed => drawEdge(g, ed, x, y));
  if (folded.has(n.id)) return;
  (n.edges || []).filter(w => w.open).forEach(w => {
    let c = null, dx = 0, dy = 0;              // the box, directly inside or in a row
    for (const k of e.children || []) {
      if (k.id === w.open[0]) { c = k; break; }
      const inRow = (k.children || []).find(cc => cc.id === w.open[0]);
      if (!byId[k.id] && inRow) { c = inRow; dx = k.x; dy = k.y; break; }
    }
    if (!c) return;
    const p = (c.ports || []).find(p => p.id === w.open[0] + ":in:" + w.var); if (!p) return;
    const px = x + dx + c.x + p.x, py = y + dy + c.y + p.y + PORT / 2;
    el("line", { x1: px - 26, y1: py, x2: px, y2: py, stroke: "var(--warn)", "stroke-width": 1.4,
                 "stroke-dasharray": "4 3" }, g);
    const t = text(g, px - 28, py - 4, "w", "not driven"); t.setAttribute("text-anchor", "end");
    t.setAttribute("style", "fill:var(--warn)"); });
}

function drawEdge(g, ed, ox, oy) {
  const k = KIND[ed.id];
  (ed.sections || []).forEach(s => {
    const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint];
    el("polyline", { points: pts.map(p => (ox + p.x) + "," + (oy + p.y)).join(" "), fill: "none",
                     stroke: "var(--wire)", "stroke-width": k && k.bus ? 2.6 : k ? 1.8 : 1.1,
                     "stroke-linejoin": "round", "marker-end": "url(#arrow)" }, g);
    if (k && k.bus) {         // a bus: a slash and its width, on its longest segment
      let a = pts[0], b = pts[1] || pts[0];
      for (let i = 0; i + 1 < pts.length; i++)
        if (Math.hypot(pts[i + 1].x - pts[i].x, pts[i + 1].y - pts[i].y) > Math.hypot(b.x - a.x, b.y - a.y)) { a = pts[i]; b = pts[i + 1]; }
      const mx = ox + (a.x + b.x) / 2, my = oy + (a.y + b.y) / 2;
      el("line", { x1: mx - 4, y1: my + 5, x2: mx + 4, y2: my - 5, stroke: "var(--wire)", "stroke-width": 1.3 }, g);
      if (!show.types) text(g, mx + 4, my - 6, "w", String(k.bus));
    } });
}

// A small model, laid out on its own and drawn in its box: ELK places the
// states and the labels of the moves; of the two directions, the one whose
// shape is closer to a square wins.
async function layoutModel(n) {
  const f = n.model.fsm;
  const graph = dir => ({ id: "m", layoutOptions: { "elk.algorithm": "layered", "elk.direction": dir,
      "elk.edgeRouting": "SPLINES", "elk.spacing.nodeNode": "26", "elk.layered.spacing.nodeNodeBetweenLayers": "46",
      "elk.spacing.edgeLabel": "3", "elk.spacing.edgeEdge": "10", "elk.edgeLabels.placement": "CENTER" },
    children: f.states.map(s => ({ id: s.id, width: textWidth(s.label, FONT_W) + 22, height: 22 })),
    edges: f.edges.map((e, i) => ({ id: "e" + i, sources: [e.from], targets: [e.to],
      labels: show.conditions && e.label ? [{ text: e.label, width: textWidth(e.label, FONT_W) + 4, height: 12 }] : [] })) });
  const tries = await Promise.all(["DOWN", "RIGHT"].map(d => new ELK().layout(graph(d))));
  const score = o => Math.abs(Math.log((o.width + 1) / (o.height + 1) / 1.3));
  const out = score(tries[0]) <= score(tries[1]) ? tries[0] : tries[1];
  const pos = {};
  out.children.forEach(c => { pos[c.id] = c; });
  const M = 22;             // room for the arrow into the first state
  return { w: out.width + M, h: out.height, dx: M, pos, out, f };
}
function drawModel(g, m, ax, ay) {
  const lg = el("g", {}, g), x0 = ax + m.dx;
  (m.out.edges || []).forEach((ed, i) => {
    const e = m.f.edges[i];
    (ed.sections || []).forEach(s => {
      const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint].map(p => [x0 + p.x, ay + p.y]);
      let d = "M" + pts[0].join(",");
      if (pts.length === 2) d += " L" + pts[1].join(",");
      else {                 // the bend points of a spline are its control points, three by three
        for (let k = 1; k + 2 < pts.length; k += 3) d += " C" + [pts[k], pts[k + 1], pts[k + 2]].map(p => p.join(",")).join(" ");
        const rest = (pts.length - 1) % 3;
        if (rest) d += " L" + pts[pts.length - 1].join(",");
      }
      const p = el("path", { d, fill: "none", stroke: "var(--wire)", "stroke-width": 1.1, "marker-end": "url(#arrow)" }, lg);
      if (e.label) tip(p, e.label); });
    (ed.labels || []).forEach(l => text(lg, x0 + l.x + 2, ay + l.y + 9, "w halo", l.text));
  });
  m.f.states.forEach(s => { const c = m.pos[s.id], x = x0 + c.x, y = ay + c.y;
    el("rect", { x, y, width: c.width, height: c.height, rx: 11, fill: "#fff", stroke: "var(--teal)",
                 "stroke-width": s.init ? 2.2 : 1 }, lg);
    const t = text(lg, x + c.width / 2, y + 15, "w", s.label); t.setAttribute("text-anchor", "middle");
    t.setAttribute("style", "fill:var(--ink)");
    if (s.init) el("path", { d: `M${x - 18},${y + 11} L${x - 3},${y + 11}`, stroke: "var(--teal)",
                             "stroke-width": 1.4, "marker-end": "url(#arrow)" }, lg); });
}

// The camera: the diagram moves and scales in the window, as in a map. A
// drag on the background moves it; the wheel, a scroll or a pinch zooms
// around the pointer; the button and the key 0 fit it, + and - zoom.
let cam = null, size0 = { w: 0, h: 0 }, camera = null, refit = false;
const TOP = 56;               // the room of the tools at the top
// The background is a grid of dots that moves and scales with the diagram;
// zoomed out, every other dot goes, so they stay 16 to 32 pixels apart.
const DOT = 20;
function place() {
  if (!camera || !cam) return;
  camera.setAttribute("transform", `translate(${cam.x},${cam.y}) scale(${cam.k})`);
  const dots = document.getElementById("dots");
  if (dots) {
    let step = DOT * cam.k;
    while (step < 16) step *= 2;
    while (step > 32) step /= 2;
    dots.setAttribute("width", step); dots.setAttribute("height", step);
    dots.setAttribute("patternTransform", `translate(${cam.x % step},${cam.y % step})`);
  }
}
function fit() {
  const v = document.getElementById("view"), W = v.clientWidth, H = v.clientHeight - TOP;
  const k = Math.min(1, (W - 40) / Math.max(size0.w, 1), (H - 30) / Math.max(size0.h, 1));
  cam = { k, x: Math.max(20, (W - size0.w * k) / 2), y: TOP + Math.max(10, (H - size0.h * k) / 2) };
  place();
}
function zoomAt(px, py, f) {
  const k = Math.min(4, Math.max(0.1, cam.k * f));
  cam.x = px - (px - cam.x) * k / cam.k; cam.y = py - (py - cam.y) * k / cam.k; cam.k = k;
  place();
}
function zoomBy(f) { if (!cam) return; const v = document.getElementById("view"); zoomAt(v.clientWidth / 2, v.clientHeight / 2, f); }
(function controls() {
  const v = document.getElementById("view");
  let drag = null;
  const live = t => { for (let e = t; e && e !== v; e = e.parentNode) if (e.onclick) return true; return false; };
  v.addEventListener("pointerdown", e => {
    if (!cam || e.button !== 0 || live(e.target)) return;
    drag = { x: e.clientX, y: e.clientY, cx: cam.x, cy: cam.y };
    v.setPointerCapture(e.pointerId); v.classList.add("moving");
  });
  v.addEventListener("pointermove", e => {
    if (!drag) return;
    cam.x = drag.cx + e.clientX - drag.x; cam.y = drag.cy + e.clientY - drag.y; place();
  });
  const stop = () => { drag = null; v.classList.remove("moving"); };
  v.addEventListener("pointerup", stop); v.addEventListener("pointercancel", stop);
  v.addEventListener("wheel", e => {
    if (!cam) return;
    e.preventDefault();
    const dy = e.deltaMode === 1 ? e.deltaY * 33 : e.deltaMode === 2 ? e.deltaY * 400 : e.deltaY;  // lines, pages
    zoomAt(e.clientX, e.clientY, Math.exp(-dy * (e.ctrlKey ? 0.01 : 0.002)));          // a pinch comes with ctrl
  }, { passive: false });
  document.addEventListener("keydown", e => {
    if (e.target.closest && e.target.closest("input, textarea")) return;
      if (!cam) return;
    if (e.key === "+" || e.key === "=") zoomBy(1.25);
    else if (e.key === "-") zoomBy(1 / 1.25);
    else if (e.key === "0") fit();
  });
  window.addEventListener("resize", () => { if (cam) place(); });
})();

function svgIn(container, width, height) {
  container.innerHTML = "";
  const svg = el("svg", {}, container);
  const defs = el("defs", {}, svg);
  const m = el("marker", { id: "arrow", viewBox: "0 0 10 10", refX: 9, refY: 5, markerWidth: 7,
                           markerHeight: 7, markerUnits: "userSpaceOnUse", orient: "auto" }, defs);
  el("path", { d: "M0,0 L10,5 L0,10 z", fill: "var(--wire)" }, m);
  const h = el("pattern", { id: "hatch", width: 7, height: 7, patternUnits: "userSpaceOnUse",
                            patternTransform: "rotate(45)" }, defs);
  el("rect", { width: 7, height: 7, fill: "#fff" }, h);
  el("line", { x1: 0, y1: 0, x2: 0, y2: 7, stroke: "#e6ecec", "stroke-width": 2 }, h);
  const d = el("pattern", { id: "dots", width: DOT, height: DOT, patternUnits: "userSpaceOnUse" }, defs);
  el("circle", { cx: 1, cy: 1, r: 1, fill: "var(--grid)" }, d);
  el("rect", { width: "100%", height: "100%", fill: "url(#dots)" }, svg);
  camera = el("g", {}, svg);
  size0 = { w: width, h: height };
  if (cam && !refit) place(); else fit();  // the first drawing fits the window, and Open all; then the view stays
  refit = false;
  return camera;
}

function narrow(e, todo) {
  const n = byId[e.id];
  if (n && e.children && e.children.length) {
    const need = size(n).w - e.width;
    if (need > 1) { widen[n.id] = (widen[n.id] || 0) + need; todo.push(n.id); }
  }
  (e.children || []).forEach(c => narrow(c, todo));
}
const ROOT = { id: "root", types: {}, children: DATA.nodes };
function rootEdges() {      // from an environment to the box it drives
  return (DATA.edges || []).map((w, i) => { const id = "root:e" + i; KIND[id] = typeOf(ROOT, w.var);
    return { id, sources: [w.from.join(":")], targets: [w.to.join(":")] }; });
}
// The items of the diagram in rows, when one row would be much wider than
// the window: null for one row, else the ids of the items of each row.
let rootRows = null;
// The rows of the first drawing stay: a box keeps its row and its place in
// it when another one opens, closes, or changes; a new box goes at the end.
let rowPlan;                     // the keys of the items of each row, or null for one row
function planOf(rows) {
  if (!rows) return null;
  const keys = keysOf(DATA.nodes);
  return rows.map(ids => ids.map(id => keys[id]));
}
function rowsFromPlan() {
  if (!rowPlan) return null;
  const keys = keysOf(DATA.nodes), idOf = {}, placed = new Set();
  for (const id in keys) idOf[keys[id]] = id;
  const rows = rowPlan.map(r => r.map(k => idOf[k]).filter(id => id && DATA.nodes.some(n => n.id === id)));
  rows.forEach(r => r.forEach(id => placed.add(id)));
  const extra = DATA.nodes.filter(n => !placed.has(n.id)).map(n => n.id);
  if (extra.length) rows[rows.length - 1].push(...extra);
  const kept = rows.filter(r => r.length);
  return kept.length > 1 ? kept : null;
}
function rowsOf(out) {
  const up = {}, root = id => up[id] === id ? id : (up[id] = root(up[id]));
  DATA.nodes.forEach(n => { up[n.id] = n.id; });
  (DATA.edges || []).forEach(w => { up[root(w.from[0])] = root(w.to[0]); });   // an environment and its item
  const size = {}; (out.children || []).forEach(c => { size[c.id] = c; });
  const units = [];
  DATA.nodes.forEach(n => { const r = root(n.id), u = units.find(u => u.r === r);
    if (u) u.ids.push(n.id); else units.push({ r, ids: [n.id] }); });
  units.forEach(u => { u.w = u.ids.reduce((w, id) => w + size[id].width, 0) + 40 * (u.ids.length - 1);
                       u.h = Math.max(...u.ids.map(id => size[id].height)); });
  const v = document.getElementById("view"), aspect = Math.max(1, v.clientWidth / Math.max(1, v.clientHeight - TOP));
  const area = units.reduce((a, u) => a + u.w * u.h, 0), total = units.reduce((a, u) => a + u.w + 40, -40);
  const width = Math.max(...units.map(u => u.w), Math.sqrt(area * aspect) * 1.2);
  if (units.length < 2 || total <= width) return null;
  const rows = [[]]; let used = 0;
  units.forEach(u => {
    if (used && used + 40 + u.w > width) { rows.push([]); used = 0; }
    rows[rows.length - 1].push(...u.ids); used += (used ? 40 : 0) + u.w; });
  return rows.length > 1 ? rows : null;
}
function layout() {
  const items = rootRows
    ? rootRows.map((ids, r) => ({ id: "root:row" + r, x: 0, y: r * 40, edges: [],
        children: ids.map((id, i) => toElk(byId[id], i, 0)),
        layoutOptions: Object.assign({}, FLOW, { "elk.padding": "[top=0,left=0,bottom=0,right=0]", "elk.alignment": "LEFT" }) }))
    : DATA.nodes.map((n, i) => toElk(n, i, 0));
  return new ELK().layout({ id: "root", children: items, edges: rootEdges(),
    // with INCLUDE_CHILDREN, every level takes the direction of the root
    layoutOptions: Object.assign({ "elk.algorithm": "layered", "elk.direction": "RIGHT",
      "elk.hierarchyHandling": "INCLUDE_CHILDREN", "elk.edgeRouting": "ORTHOGONAL" }, FLOW,
      { "elk.spacing.nodeNode": "40" }) });
}
async function render() {
  for (const n of boxes)
    if (open.has("m:" + n.id) && inlineModel(n) && !MODELS[n.id]) MODELS[n.id] = await layoutModel(n);
  widen = {};
  if (rowPlan === undefined) { rootRows = null; rowPlan = planOf(rowsOf(await layout())); }
  rootRows = rowsFromPlan();
  let out = await layout();
  for (let pass = 0; pass < 3; pass++) {
    const todo = []; (out.children || []).forEach(c => narrow(c, todo));
    if (!todo.length) break;
    out = await layout();
  }
  if (rootRows) {             // the rows in their order, from the top: ELK may swap them
    const rows = rootRows.map((_, r) => (out.children || []).find(c => c.id === "root:row" + r));
    const of = {}; rootRows.forEach((ids, r) => ids.forEach(id => { of[id] = r; }));
    const left = Math.min(...rows.map(c => c.x)), dx = [], dy = [];
    let y = Math.min(...rows.map(c => c.y));
    rows.forEach((c, r) => { dx[r] = left - c.x; dy[r] = y - c.y; c.x = left; c.y = y; y += c.height + 40; });
    (out.edges || []).forEach(ed => {          // a wire from an environment moves with its row
      const r = of[ed.sources[0].split(":")[0]];
      (ed.sections || []).forEach(sec => [sec.startPoint, sec.endPoint, ...(sec.bendPoints || [])]
        .forEach(p => { p.x += dx[r]; p.y += dy[r]; }));
    });
    out.height = y - 40;
  }
  // the items with no wire between them start at the top
  const wired = new Set((DATA.edges || []).flatMap(w => [w.from[0], w.to[0]]));
  const free = rootRows ? [] : (out.children || []).filter(c => !wired.has(c.id));
  if (free.length) {
    const top = Math.min(...(out.children || []).map(c => c.y));
    free.forEach(c => { c.y = top; });
    out.height = Math.max(...out.children.map(c => c.y + c.height));
  }
  const g = svgIn(document.getElementById("view"), out.width, out.height);
  (out.children || []).forEach(c => drawNode(g, c, 0, 0));
  (out.edges || []).forEach(ed => drawEdge(g, ed, 0, 0));
}

// A model with more states is shown in a panel, as a table of moves.
function showModel(n) {
  const f = n.model.fsm, label = {};
  f.states.forEach(st => { label[st.id] = st.label; });
  const esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  let rows = "";
  f.states.forEach(st => {
    const moves = f.edges.filter(e => e.from === st.id);
    rows += moves.map((e, i) => "<tr>" + (i ? "" : "<td rowspan='" + moves.length + "' class='st'>"
        + (st.init ? "→ " : "") + esc(st.label) + "</td>")
      + "<td>" + esc(label[e.to]) + "</td><td class='when'>"
      + esc(e.label || (f.inputs_in_state ? "" : "any input")) + "</td></tr>").join("");
  });
  document.getElementById("poptitle").textContent = n.title + " · model " + n.model.name + " · "
    + f.count + " states" + (f.inputs_in_state ? " (the inputs are part of a state)" : "");
  document.getElementById("popview").innerHTML = "<table class='fsm'><tr><th>state</th><th>next state</th>"
    + "<th>" + (f.inputs_in_state ? "" : "when the inputs are") + "</th></tr>" + rows + "</table>";
  document.getElementById("pop").style.display = "flex";
}
function closePop() { document.getElementById("pop").style.display = "none"; }

function help() {
  const h = document.getElementById("help");
  if (h.style.display === "block") { h.style.display = "none"; return; }
  const sample = (draw, s) => { const d = document.createElement("div");
    const svg = el("svg", { width: 88, height: 20 }, d); draw(el("g", { transform: "translate(20,0)" }, svg)); d.appendChild(document.createTextNode(s)); h.appendChild(d); };
  const box = (rx, fill, stroke, w, dash) => s => el("rect", { x: 4, y: 3, width: 40, height: 14, rx, fill, stroke,
    "stroke-width": w, "stroke-dasharray": dash || "none" }, s);
  h.innerHTML = "<div>drag to move · the wheel zooms where it points · 0 fits the window</div>";
  sample(box(5, "#fff", "#17393b", 1.2), "contract");
  sample(box(1, "#fff", "#1d2526", 1.8), "component with a model");
  sample(box(1, "#f5f7f7", "#667374", 1.4), "environment");
  sample(s => { svgDefsHatch(s); box(5, "url(#hatchh)", "#17393b", 1.2, "4 3")(s); }, "hole of a quotient");
  sample(s => { box(5, "#f5f7f7", "#17393b", 1.2)(s);
                el("rect", { x: 14, y: 6, width: 20, height: 8, rx: 3, fill: "#fff", stroke: "#17393b" }, s); },
         "inside: refines the outer box");
  [["contract", "assumptions A, guarantees G"], ["composed", "composed contract (⊗)"],
   ["model", "model given (RTL, JSON, SMV)"], ["synth", "model synthesized (slugs)"]].forEach(([k, t]) =>
    sample(svg => icon(svg, k, 16, 2, false, t), t));
  sample(s => { el("rect", { x: 16, y: 2, width: 16, height: 16, rx: 4, fill: "#fff", stroke: "#17393b", "stroke-width": 1.1 }, s);
                el("path", { d: "M20.5 11.5 L24 8 L27.5 11.5", fill: "none", stroke: "#17393b", "stroke-width": 1.4 }, s); },
         "hide the boxes inside (or click ⪯)");
  // a check: green when it holds, red when not, grey when undecided
  const pair = (t, d) => { const d2 = document.createElement("div");
    const svg = el("svg", { width: 88, height: 20 }, d2), w = tagWidth(t);
    tag(svg, 44 - w - 2, 2, t, "valid"); tag(svg, 46, 2, t, "invalid");
    d2.appendChild(document.createTextNode(d)); h.appendChild(d2); };
  [["A", "compatible"], ["G", "consistent"], ["A∧G", "feasible"], ["R", "realizable"],
   ["WS", "well-separated"], ["¬V", "no vacuous output"], ["M ⊨ C", "implements"],
   ["E ⊨ A", "environment"], ["⪯", "refines"]].forEach(([t, d]) => pair(t, d));
  sample(s => el("rect", { x: 4, y: 4, width: 40, height: 12, rx: 3, fill: "#f6dcd9" }, s), "core (click a red tag)");
  const wire = (w, d) => sample(s => el("line", { x1: 4, y1: 10, x2: 44, y2: 10, stroke: "#7d8c8d", "stroke-width": w }, s), d);
  wire(1.1, "Boolean");
  wire(1.8, "integer or real");
  sample(s => { el("line", { x1: 4, y1: 10, x2: 44, y2: 10, stroke: "#7d8c8d", "stroke-width": 2.6 }, s);
                el("line", { x1: 20, y1: 15, x2: 28, y2: 5, stroke: "#7d8c8d", "stroke-width": 1.3 }, s);
                text(s, 29, 7, "w", "8"); }, "bus, with its width");
  sample(s => el("line", { x1: 4, y1: 10, x2: 44, y2: 10, stroke: "#c07a14", "stroke-width": 1.4, "stroke-dasharray": "4 3" }, s),
         "input that nobody drives");
  h.style.display = "block";
}
function svgDefsHatch(s) {
  const p = el("pattern", { id: "hatchh", width: 6, height: 6, patternUnits: "userSpaceOnUse", patternTransform: "rotate(45)" },
               el("defs", {}, s));
  el("rect", { width: 6, height: 6, fill: "#fff" }, p);
  el("line", { x1: 0, y1: 0, x2: 0, y2: 6, stroke: "#dfe6e6", "stroke-width": 2 }, p);
}

function drawContext() {
  const c = document.getElementById("context"), esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;");
  const fs = DATA.context || [];
  c.innerHTML = fs.map(f => "<span data-tip='context: holds in every check'><i>context</i>" + esc(f) + "</span>").join("");
  c.style.display = fs.length ? "flex" : "none";
}
// A new drawing of the same file, sent by an editor or by chase-cps view
// --serve as the file changes: the view, and what is open or folded, stay.
// A box is known from one drawing to the next by the names from the top
// down to it (with its rank among the boxes of the same name), not by its
// number, which moves when a contract is added. What it says, without the
// results, tells whether the results of a check still hold for it.
function keysOf(nodes) {
  const keys = {};
  (function walk(ns, prefix) {
    const seen = {};
    ns.forEach(n => { seen[n.title] = (seen[n.title] || 0) + 1;
                      keys[n.id] = prefix + "/" + n.title + "#" + seen[n.title];
                      walk(n.children || [], keys[n.id]); });
  })(nodes, "");
  return keys;
}
function says(n) {
  return JSON.stringify([n.kind, n.title, n.note, n.contract, n.composed, n.inputs, n.outputs,
                         n.model && n.model.name, (n.children || []).map(says)]);
}
// The results of the boxes of results that say the same, by key.
function resultsOf(nodes) {
  const keys = keysOf(nodes), out = {};
  (function walk(ns) { ns.forEach(n => { out[keys[n.id]] = n; walk(n.children || []); }); })(nodes);
  return out;
}
function carry(n, old) {               // the results of old, when n says the same
  if (!old || says(old) !== says(n)) return;
  if (!(n.checks && n.checks.length) && old.checks) n.checks = old.checks;
  if (n.badge && !n.badge.status && old.badge && old.badge.text === n.badge.text) n.badge.status = old.badge.status;
}
let drawing = Promise.resolve(), told = 0, staleAt = 0;   // one drawing at a time, in order
function setData(data) {
  const token = ++told;
  drawing = drawing.catch(() => {}).then(() => load(data, token));
  return drawing;
}
// The results of the checks of the saved file, for the boxes that say the same now.
function setChecks(data) {
  drawing = drawing.catch(() => {}).then(() => {
    const was = resultsOf(data.nodes), keys = keysOf(DATA.nodes);
    for (const id in byId) {
      const n = byId[id], c = was[keys[id]];
      if (c && says(c) === says(n)) { n.checks = c.checks; if (n.badge && c.badge) n.badge.status = c.badge.status; }
    }
    return render();
  });
  return drawing;
}
function load(data, token) {
  const keys = keysOf(DATA.nodes), was = resultsOf(DATA.nodes);
  const openKeys = [...open].map(k => [k.slice(0, 2), keys[k.slice(2)]]);
  const foldedKeys = [...folded].map(id => keys[id]);
  const focusKeys = Object.entries(focus).map(([id, i]) => [keys[id], byId[id] && byId[id].checks && byId[id].checks[i]]);
  DATA = data;
  for (const k in byId) delete byId[k];
  (function index(ns) { ns.forEach(n => { byId[n.id] = n; index(n.children || []); }); })(DATA.nodes);
  boxes = Object.values(byId);
  ROOT.children = DATA.nodes;
  for (const k in MODELS) delete MODELS[k];
  const now = keysOf(DATA.nodes), idOf = {};
  for (const id in now) { idOf[now[id]] = id; carry(byId[id], was[now[id]]); }
  open.clear(); openKeys.forEach(([p, k]) => { if (idOf[k]) open.add(p + idOf[k]); });
  folded.clear(); foldedKeys.forEach(k => { if (idOf[k]) folded.add(idOf[k]); });
  for (const k in focus) delete focus[k];
  focusKeys.forEach(([k, c]) => { const n = byId[idOf[k]];
    const i = n && c ? (n.checks || []).findIndex(x => x.name === c.name && x.core) : -1;
    if (i >= 0) focus[n.id] = i; });
  document.title = DATA.title;
  drawContext();
  if (token > staleAt) stale(null);    // a newer reason stays
  return render();
}
// The file cannot be drawn as it is now: the diagram of the last version
// that could be drawn stays, with the reason.
function stale(reason) {
  if (reason) staleAt = told;
  const b = document.getElementById("stale");
  if (!reason) { b.style.display = "none"; return; }
  b.innerHTML = "<b>The diagram is of the last version that could be drawn.</b>";
  b.appendChild(document.createTextNode(reason));
  b.style.display = "block";
}
window.addEventListener("message", e => {
  const m = e.data || {};
  if (m.type === "chase-data") setData(m.data).catch(err => stale("Cannot draw the diagram: " + err));
  else if (m.type === "chase-checks") setChecks(m.data).catch(err => stale("Cannot draw the checks: " + err));
  else if (m.type === "chase-stale") stale(m.reason);
  else if (m.type === "chase-busy") document.getElementById("busy").style.display = m.on ? "block" : "none";
});
// Export: the whole diagram as it is drawn (what is open stays open), on a
// white ground, the colors written out, so that the file stands alone.
function exportMenu() {
  const m = document.getElementById("export");
  m.style.display = m.style.display === "flex" ? "none" : "flex";
}
function exported() {
  const src = document.querySelector("#view > svg");
  if (!src || !camera) return null;
  const box = camera.getBBox(), pad = 16, w = Math.ceil(box.width + 2 * pad), h = Math.ceil(box.height + 2 * pad);
  const svg = src.cloneNode(true);
  svg.setAttribute("xmlns", NS);
  svg.setAttribute("width", w); svg.setAttribute("height", h); svg.setAttribute("viewBox", `0 0 ${w} ${h}`);
  [...svg.children].filter(c => c.tagName === "rect").forEach(c => c.remove());   // the dots
  const g = [...svg.children].find(c => c.tagName === "g");
  g.setAttribute("transform", `translate(${pad - box.x},${pad - box.y})`);
  const ground = document.createElementNS(NS, "rect");
  ground.setAttribute("width", w); ground.setAttribute("height", h); ground.setAttribute("fill", "#fff");
  svg.insertBefore(ground, g);
  const style = document.createElementNS(NS, "style");
  style.textContent = [...document.querySelectorAll("style")].map(x => x.textContent).join("\n");
  svg.insertBefore(style, svg.firstChild);
  svg.querySelectorAll("[data-tip]").forEach(e => e.removeAttribute("data-tip"));
  const root = getComputedStyle(document.documentElement);
  const text = new XMLSerializer().serializeToString(svg)
    .replace(/var\((--[\w-]+)\)/g, (_, v) => root.getPropertyValue(v).trim() || "#000");
  return { text, w, h };
}
let vscodeApi = null;
function deliver(name, mime, data) {
  if (typeof acquireVsCodeApi === "function") {      // in an editor: it asks where to save
    vscodeApi = vscodeApi || acquireVsCodeApi();
    vscodeApi.postMessage({ type: "chase-export", name, mime, data });
    return;
  }
  const a = document.createElement("a");
  a.download = name;
  a.href = data.startsWith("data:") ? data : URL.createObjectURL(new Blob([data], { type: mime }));
  document.body.appendChild(a); a.click(); a.remove();
}
function exportAs(kind) {
  document.getElementById("export").style.display = "none";
  const e = exported();
  if (!e) return;
  const name = (DATA.title || "diagram").replace(/\.[^.]*$/, "").replace(/[^\w.-]+/g, "_");
  if (kind === "svg") return deliver(name + ".svg", "image/svg+xml", e.text);
  const img = new Image();
  img.onload = () => {
    const k = 2, c = document.createElement("canvas");
    c.width = e.w * k; c.height = e.h * k;
    const x = c.getContext("2d"); x.scale(k, k); x.drawImage(img, 0, 0);
    if (kind === "png") return deliver(name + ".png", "image/png", c.toDataURL("image/png"));
    // PDF: the vector page, through the print dialog of the browser ("Save as PDF"); in an
    // editor, which cannot print, the editor writes it
    if (typeof acquireVsCodeApi === "function")
      return deliver(name + ".pdf", "application/pdf", JSON.stringify({ svg: e.text, w: e.w, h: e.h }));
    let p = document.getElementById("print");
    if (!p) { p = document.createElement("div"); p.id = "print"; p.style.display = "none"; document.body.appendChild(p); }
    p.innerHTML = e.text;
    window.print();
  };
  img.src = "data:image/svg+xml;charset=utf-8," + encodeURIComponent(e.text);
}
// The explanations: shown by the page itself, since the tooltips of the
// browser do not show in the webview of an editor.
(function () {
  const box = document.getElementById("tip");
  document.querySelectorAll("[title]").forEach(b => {
    b.setAttribute("data-tip", b.title); b.setAttribute("aria-label", b.title); b.removeAttribute("title"); });
  let timer = null, x = 0, y = 0, over = null;
  const hide = () => { clearTimeout(timer); box.style.display = "none"; };
  const place = () => {
    box.style.left = "0px"; box.style.top = "0px";          // measured at its full width
    const w = box.offsetWidth, h = box.offsetHeight;
    box.style.left = Math.max(4, Math.min(x + 12, innerWidth - w - 8)) + "px";
    box.style.top = (y + 18 + h > innerHeight ? y - h - 10 : y + 18) + "px";
  };
  document.addEventListener("mouseover", e => {
    const t = e.target.closest && e.target.closest("[data-tip]");
    if (t === over) return;                                  // still on the same part
    over = t;
    hide();
    if (t) timer = setTimeout(() => { box.textContent = t.getAttribute("data-tip"); box.style.display = "block"; place(); }, 400);
  });
  document.addEventListener("mousemove", e => { x = e.clientX; y = e.clientY; if (box.style.display === "block") place(); });
  document.addEventListener("mousedown", () => { hide(); over = null; });
  document.addEventListener("wheel", hide, { passive: true });
  document.addEventListener("mouseleave", hide);
})();
drawContext();
openAsked();
render().catch(e => { document.getElementById("view").textContent = "Cannot draw the diagram: " + e; });
</script>
__RELOAD__
</body></html>
"""
