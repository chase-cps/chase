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

    def fresh(self, prefix="n") -> str:
        self.count += 1
        return f"{prefix}{self.count}"

    def box(self, obj, types=None, **node) -> dict:
        node.setdefault("id", self.fresh())
        node["types"] = types or {}
        self.ids.setdefault(id(obj), node["id"])
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


CHECKS = "verify, is_realizable, is_well_separated, is_vacuous, or is_receptive"
# The label of each check, by the name of the check (for a result that is unknown)
KINDS = {"realizable": "R", "well_separated": "WS", "vacuous": "¬V", "receptive": "receptive"}


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
            context=None, title: Optional[str] = None) -> dict:
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
    or well_separated, vacuous, receptive. ``environments`` lists (environment, item) or
    (environment, item, status): an LTL formula, a Model, or a Component
    whose implementation is the environment, drawn on the left of the item
    and driving its inputs; the status is that of ``is_environment``. The
    ``context`` of the checks (a formula or a list of formulas) is written
    above the diagram."""
    scene = _Scene()
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
            replace(node, scene.wrap(node, item.contract, item.status))
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
    for r in ordered:
        low, high = r[0], r[1]
        status = _status(r[2]) if len(r) > 2 else None
        if isinstance(high, Contract) and high.origin and high.origin[0] == "quotient":
            if id(high) not in scene.ids:
                add(high)
            hole = _find(nodes, scene.ids[id(high)])
            inner = scene.node(low)
            hole["children"].append(inner)
            hole["edges"] += _through(inner, hole["inputs"], hole["outputs"])
            hole["badge"] = {"text": "⪯", "status": status}
            continue
        drawn = _find(nodes, scene.ids[id(high)]) if id(high) in scene.ids else None
        if drawn is not None and not drawn.get("children") and id(low) not in top:
            # the contract is drawn inside another box already: its box takes what refines it
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
        inner = top.pop(id(low), None) or next(   # in a chain, the box drawn already
            (n for n in nodes if n["id"] == scene.ids.get(id(low))), None) or scene.node(low)
        wrapped = scene.wrap(inner, high, status)
        replace(inner, wrapped)
        top[id(low)] = wrapped

    def box_of(obj) -> dict:              # an item drawn on its own first, then inside others
        if id(obj) in own:
            return own[id(obj)]
        if id(obj) not in scene.ids:
            return add(obj)
        return _find(nodes, scene.ids[id(obj)])

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
    if context is not None:
        from .logic import _formula
        context = [context] if isinstance(context, (str, ltl.Node)) else list(context)
        context = [ltl.to_str(_formula(f)) for f in context]
    return {"title": title or "CHASE", "nodes": nodes, "edges": wires, "context": context or []}


def html(item, **kwargs) -> str:
    """The page that draws a diagram (or a contract, component, or system)."""
    d = item if isinstance(item, dict) and "nodes" in item else diagram(item, **kwargs)
    return (_PAGE.replace("__TITLE__", _html.escape(d["title"]))
                 .replace("__RELOAD__", _RELOAD)
                 .replace("__DATA__", json.dumps(d).replace("</", "<\\/")))


def save(item, path: str, **kwargs) -> str:
    """Writes the page to path and returns the path."""
    with open(path, "w", encoding="utf-8") as f:
        f.write(html(item, **kwargs))
    return path


def show(item, **kwargs) -> str:
    """Writes the page to a temporary file and opens it in the browser."""
    fd, path = tempfile.mkstemp(prefix="chase-", suffix=".html")
    os.close(fd)
    save(item, path, **kwargs)
    webbrowser.open("file://" + path)
    return path


def notebook(item, **kwargs) -> str:
    """The page in an iframe, for Jupyter."""
    page = _html.escape(html(item, **kwargs), quote=True)
    return f'<iframe srcdoc="{page}" style="width:100%;height:520px;border:0"></iframe>'


def serve(page, path: str, port: int = 8765, host: str = "127.0.0.1") -> int:
    """Serves page() on http://host:port, and draws it again whenever the
    file at path changes; the page in the browser reloads by itself. Stops
    with Ctrl-C."""
    import http.server
    import threading
    import time

    state = {"mtime": None, "version": 0, "page": ""}
    lock = threading.Lock()

    def refresh():
        mtime = os.path.getmtime(path) if os.path.exists(path) else None
        if mtime == state["mtime"]:
            return
        try:
            text = page()
        except Exception as e:          # an error in the file is shown in the page
            text = ("<!doctype html><meta charset=utf-8><pre style='color:#b03a2e;padding:16px'>"
                    + _html.escape(f"{type(e).__name__}: {e}") + "</pre>" + _RELOAD)
        with lock:
            state.update(mtime=mtime, page=text, version=state["version"] + 1)

    class Handler(http.server.BaseHTTPRequestHandler):
        def do_GET(self):
            refresh()
            with lock:
                body = (str(state["version"]) if self.path == "/version" else state["page"]).encode()
            self.send_response(200)
            self.send_header("Content-Type", "text/plain" if self.path == "/version"
                             else "text/html; charset=utf-8")
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(body)

        def log_message(self, *args):
            pass

    refresh()
    server = http.server.ThreadingHTTPServer((host, port), Handler)
    print(f"Drawing {path} on http://{host}:{port} (Ctrl-C to stop)")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
    return 0


# Served pages ask for /version every second, and reload when it changes.
_RELOAD = """<script>
(function () {
  if (!location.protocol.startsWith("http")) return;
  let seen = null;
  setInterval(async () => {
    try { const v = await (await fetch("/version", { cache: "no-store" })).text();
          if (seen === null) seen = v; else if (v !== seen) location.reload(); } catch (e) {}
  }, 1000);
})();
</script>"""


_PAGE = r"""<!doctype html>
<html><head><meta charset="utf-8"><title>__TITLE__</title>
<style>
  :root { --ink:#1d2526; --soft:#667374; --faint:#97a3a4; --teal:#17393b; --line:#b6c1c1; --fill:#f5f7f7;
          --wire:#7d8c8d; --ok:#2f7d4f; --bad:#b03a2e; --warn:#c07a14; }
  body { margin:0; font:13px/1.4 -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; color:var(--ink); background:#fff; }
  #top { position:sticky; top:0; z-index:5; display:flex; justify-content:space-between; align-items:flex-start;
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
  #help { position:absolute; right:12px; top:46px; background:#fff; border:1px solid #d5dddd; border-radius:8px;
          box-shadow:0 6px 24px rgba(0,0,0,.12); padding:12px 14px; font:12px 'Arial Unicode MS', Helvetica, Arial, sans-serif;
          color:var(--soft); display:none; z-index:11; }
  #help div { display:flex; gap:10px; align-items:center; margin:6px 0; white-space:nowrap; }
  #help svg { flex:none; }
  #context { display:none; gap:8px; flex-wrap:wrap; padding-top:4px; }
  #context span { display:inline-flex; align-items:center; gap:7px; border:1px solid #d5dddd; border-radius:6px;
                  padding:2px 9px 2px 2px; font:11.5px Menlo, Monaco, monospace; color:var(--ink);
                  white-space:nowrap; background:#fff; }
  #context i { font:600 9.5px -apple-system, Helvetica, Arial, sans-serif; font-style:normal; color:#fff;
               background:#6f7f80; border-radius:4px; padding:2px 5px; }
  #view { overflow:auto; padding:6px 18px 18px; }
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
    <button class="on" title="Ports and their names" onclick="flip(this, 'ports')"><svg viewBox="0 0 16 16"><rect x="4.5" y="3" width="7" height="10" rx="1.5"/><path d="M1.5 6h3M1.5 10h3M11.5 8h3"/></svg></button>
    <button class="on" title="Types of the ports (bit vectors, ranges, reals)" onclick="flip(this, 'types')"><svg viewBox="0 0 16 16"><path d="M1.5 8h13" stroke-width="2.4"/><path d="M6.5 11.5l3-7"/></svg></button>
    <button class="on" title="Conditions of the moves of the models" onclick="flip(this, 'conditions')"><svg viewBox="0 0 16 16"><circle cx="3" cy="11" r="1.8"/><circle cx="13" cy="11" r="1.8"/><path d="M4.8 11h6.4"/><rect x="5" y="3" width="6" height="4" rx="1"/></svg></button>
    <button class="on" title="Results of the checks" onclick="flip(this, 'checks')"><svg viewBox="0 0 16 16"><rect x="1.5" y="4" width="13" height="8" rx="3"/><path d="M5.5 8l1.8 1.8L10.5 6.3"/></svg></button>
    <span class="sep"></span>
    <button title="What the shapes mean" onclick="help()"><svg viewBox="0 0 16 16"><circle cx="8" cy="8" r="6.2"/><path d="M6.3 6.3a1.8 1.8 0 1 1 2.4 1.7c-.5.2-.7.6-.7 1.1v.4"/><circle cx="8" cy="11.6" r=".4"/></svg></button>
  </span><div id="help"></div></div>
<div id="view"></div>
<div id="pop"><header><b id="poptitle"></b><button onclick="closePop()">close</button></header><div id="popview"></div></div>
<script src="https://cdn.jsdelivr.net/npm/elkjs@0.9.3/lib/elk.bundled.js"></script>
<script>
const DATA = __DATA__;
const LINE = 17, PAD = 10, PORT = 6, HEAD = 28, ICON = 16, INLINE_STATES = 8;
const ctx = document.createElement("canvas").getContext("2d");
function textWidth(s, font) { ctx.font = font; return ctx.measureText(s).width; }
const FONT_F = "11.5px Menlo, Monaco, monospace", FONT_H = "600 12.5px Helvetica, Arial, sans-serif",
      FONT_S = "11px 'Arial Unicode MS', Helvetica, Arial, sans-serif", FONT_W = "10px Menlo, Monaco, monospace";
const show = { ports: true, types: true, conditions: true, checks: true };

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
const boxes = Object.values(byId);
const states = n => n.model && n.model.fsm && n.model.fsm.states;
const inlineModel = n => states(n) && n.model.fsm.count <= INLINE_STATES;
let allOpen = false;
function openAll(on) {
  boxes.forEach(n => ["c:", "k:", "m:"].forEach(p => {
    const key = p + n.id, there = p === "c:" ? n.contract : p === "k:" ? n.composed : inlineModel(n);
    if (on && there) open.add(key); else open.delete(key); }));
  if (on) folded.clear();
  allOpen = on;
  const b = document.getElementById("all");
  b.title = on ? "Close all" : "Open all";
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
const focused = n => n.id in focus ? n.checks[focus[n.id]].core : null;

// The lines under the name. A long contract (a computed one, often) shows
// its first lines; the rest opens in a panel.
const LONG = 14, SHORT = 8;
function lines(n) {
  const out = [], core = focused(n);
  const part = (c, which) => {
    const ts = [];
    terms(c, ts, core && core.part === which ? core : null);
    if (ts.length <= LONG) { out.push(...ts); return; }
    out.push(...ts.slice(0, SHORT), ["more", "", "⋯ " + (ts.length - SHORT) + " more lines", c]);
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
const WHAT = { "A": "A is satisfiable: the contract is compatible",
               "G": "A → G is satisfiable: the contract is consistent",
               "A∧G": "feasible: A ∧ G is satisfiable",
               "R": "the contract is realizable (GR(1))",
               "WS": "well-separated: the component cannot win by making the environment break A",
               "¬V": "no vacuous output: each liveness guarantee asks something of the component",
               "M ⊨ C": "the model implements the contract",
               "E ⊨ A": "the environment satisfies the assumptions",
               "⪯": "what is inside refines this contract" };
const checks = n => show.checks && n.checks ? n.checks : [];

const MODELS = {};       // the layout of each open model drawn in its box
function size(n) {
  const ls = lines(n);
  const top = textWidth(n.title, FONT_H) + icons(n).length * (ICON + 4) + 14
            + checks(n).reduce((w, c) => w + tagWidth(c.name) + 4, checks(n).length ? 6 : 0)
            + (n.badge ? tagWidth(n.badge.text) + 18 : 0) + (inside(n) ? ICON + 6 : 0);
  let w = Math.max(top, ...ls.map(lineWidth)) + 2 * PAD;
  let h = HEAD + ls.length * LINE + (ls.length ? 8 : 0);
  if (folded.has(n.id)) h = Math.max(h, HEAD + 10);
  const m = open.has("m:" + n.id) && MODELS[n.id];
  if (m) { w = Math.max(w, m.w + 2 * PAD); h += m.h + 10; }
  return { ls, w: Math.max(w, 110), h, m };
}

const portText = (n, v) => { const t = show.types && n.types && n.types[v]; return t ? v + " " + t.label : v; };
function ports(n) {
  if (n.bare) return [];
  const ps = [], lab = v => show.ports ? [{ text: portText(n, v), width: textWidth(portText(n, v), FONT_W), height: 11 }] : [];
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
function toElk(n, column, row) {
  const sz = size(n);
  const base = { id: n.id, ports: ports(n), x: (column || 0) * 600, y: (row || 0) * 40,
    layoutOptions: { "elk.portConstraints": "FIXED_ORDER", "elk.nodeSize.constraints": "MINIMUM_SIZE",
      "elk.portLabels.placement": "OUTSIDE", "elk.spacing.portPort": "13",
      "elk.nodeSize.minimum": "(" + sz.w + "," + Math.max(sz.h, (Math.max((n.inputs || []).length, (n.outputs || []).length) + 1) * 17) + ")" } };
  if (!inside(n) || folded.has(n.id)) { base.width = sz.w; base.height = sz.h; return base; }
  const col = columns(n);
  base.children = n.children.map((c, i) => toElk(c, col[c.id], i));
  base.edges = (n.edges || []).filter(w => !w.open).map((w, i) => {
    const id = n.id + ":e" + i; KIND[id] = typeOf(n, w.var);
    return { id, sources: [(w.from[0] || n.id) + ":" + w.from[1] + ":" + w.from[2]],
             targets: [(w.to[0] || n.id) + ":" + w.to[1] + ":" + w.to[2]] }; });
  Object.assign(base.layoutOptions, FLOW, {
    "elk.padding": "[top=" + (sz.h + 10) + ",left=28,bottom=18,right=" + (28 + (widen[n.id] || 0)) + "]" });
  return base;
}

const NS = "http://www.w3.org/2000/svg";
function el(tag, attrs, parent) { const e = document.createElementNS(NS, tag);
  for (const k in attrs) e.setAttribute(k, attrs[k]); if (parent) parent.appendChild(e); return e; }
function text(parent, x, y, cls, s) { const t = el("text", { x, y, class: cls }, parent); t.textContent = s; return t; }
function tip(parent, s) { el("title", {}, parent).textContent = s; }

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
  const n = byId[e.id], x = ox + e.x, y = oy + e.y, sz = size(n);
  const box = el("g", { "data-box": n.id }, g);
  const st = n.kind === "group" && n.badge ? Object.assign({}, STYLE.group, { stroke: "var(--teal)", width: 1.2 })
                                           : STYLE[n.kind] || STYLE.contract;
  el("rect", { x, y, width: e.width, height: e.height, rx: st.rx, fill: st.fill, stroke: st.stroke,
               "stroke-width": st.width, "stroke-dasharray": st.dash }, box);
  if (n.kind === "component" || n.kind === "environment") el("line", { x1: x, y1: y + HEAD - 2, x2: x + e.width, y2: y + HEAD - 2,
                                           stroke: "#e3e8e8", "stroke-width": 1 }, box);
  const name = text(box, x + PAD, y + 18, "h", n.title);
  if (n.contract) name.onclick = toggle("c:" + n.id);
  let ix = x + PAD + textWidth(n.title, FONT_H) + 10;
  icons(n).forEach(([kind, key, title]) => {
    const on = key && open.has(key), c = icon(box, kind, ix, y + 6, on, title, !key);
    if (key) c.onclick = (kind === "model" || kind === "synth") && !on && !inlineModel(n) ? () => showModel(n) : toggle(key);
    ix += ICON + 4; });
  if (checks(n).length) ix += 6;
  checks(n).forEach((c, i) => {      // a click on a check that has a core shows the core
    const t = el("g", {}, box), on = focus[n.id] === i;
    ix += tag(t, ix, y + 6, c.name, c.status) + 4;
    tip(t, (WHAT[c.name] || c.name) + ": " + (SAYS[c.status] || "not checked") + (c.reason ? " (" + c.reason + ")" : "")
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
  if (n.badge) { const b = el("g", {}, box), w = tagWidth(n.badge.text);
    tag(b, rx - w, y + 6, n.badge.text, n.badge.status);
    tip(b, (WHAT[n.badge.text.split(" of ")[0]] || n.badge.text) + ": " + (SAYS[n.badge.status] || "not checked")
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
    const c = (e.children || []).find(c => c.id === w.open[0]); if (!c) return;
    const p = (c.ports || []).find(p => p.id === w.open[0] + ":in:" + w.var); if (!p) return;
    const px = x + c.x + p.x, py = y + c.y + p.y + PORT / 2;
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
      if (!show.types || !show.ports) text(g, mx + 4, my - 6, "w", String(k.bus));
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

function svgIn(container, width, height) {
  container.innerHTML = "";
  const svg = el("svg", { width: width + 40, height: height + 40 }, container);
  const defs = el("defs", {}, svg);
  const m = el("marker", { id: "arrow", viewBox: "0 0 10 10", refX: 9, refY: 5, markerWidth: 7,
                           markerHeight: 7, markerUnits: "userSpaceOnUse", orient: "auto" }, defs);
  el("path", { d: "M0,0 L10,5 L0,10 z", fill: "var(--wire)" }, m);
  const h = el("pattern", { id: "hatch", width: 7, height: 7, patternUnits: "userSpaceOnUse",
                            patternTransform: "rotate(45)" }, defs);
  el("rect", { width: 7, height: 7, fill: "#fff" }, h);
  el("line", { x1: 0, y1: 0, x2: 0, y2: 7, stroke: "#e6ecec", "stroke-width": 2 }, h);
  return el("g", { transform: "translate(20,20)" }, svg);
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
function layout() {
  return new ELK().layout({ id: "root", children: DATA.nodes.map((n, i) => toElk(n, i, 0)), edges: rootEdges(),
    // with INCLUDE_CHILDREN, every level takes the direction of the root
    layoutOptions: Object.assign({ "elk.algorithm": "layered", "elk.direction": "RIGHT",
      "elk.hierarchyHandling": "INCLUDE_CHILDREN", "elk.edgeRouting": "ORTHOGONAL" }, FLOW,
      { "elk.spacing.nodeNode": "40" }) });
}
async function render() {
  for (const n of boxes)
    if (open.has("m:" + n.id) && inlineModel(n) && !MODELS[n.id]) MODELS[n.id] = await layoutModel(n);
  widen = {};
  let out = await layout();
  for (let pass = 0; pass < 3; pass++) {
    const todo = []; (out.children || []).forEach(c => narrow(c, todo));
    if (!todo.length) break;
    out = await layout();
  }
  // the items with no wire between them start at the top
  const wired = new Set((DATA.edges || []).flatMap(w => [w.from[0], w.to[0]]));
  const free = (out.children || []).filter(c => !wired.has(c.id));
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
  h.innerHTML = "";
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

if (DATA.context && DATA.context.length) {
  const c = document.getElementById("context"), esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;");
  c.innerHTML = DATA.context.map(f => "<span title='context: holds in every check'><i>context</i>" + esc(f) + "</span>").join("");
  c.style.display = "flex";
}
render().catch(e => { document.getElementById("view").textContent = "Cannot draw the diagram: " + e; });
</script>
__RELOAD__
</body></html>
"""
