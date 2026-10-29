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

The structure comes from the contracts themselves: each result of the
algebra remembers how it was computed (``Contract.origin``). The page is
drawn in the browser, laid out by ELK (the Eclipse Layout Kernel, loaded
from a CDN).

    from chase_cps import viz
    viz.save(viz.diagram(system, refines=[(text, q3, "valid")]), "plan.html")
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
from .system import Component, System

# Operations drawn as a box with its operands inside, and the word used.
SIGNS = {"composition": "||", "quotient": "/", "merging": "merged with",
         "conjunction": "and", "disjunction": "or", "separation": "separated by"}
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
            node["note"] = " || ".join([k["title"] for k in known] + ["?"])
            return node
        return self.group(c, operation, [self.contract(o) for o in operands])

    def group(self, c: Contract, operation: str, children: list, box_of=None) -> dict:
        names = [ch["title"] for ch in children]
        if operation in STACKED:      # viewpoints of one component: the same ports, no wires
            return self.box(box_of if box_of is not None else c, types=_types(c), kind="group",
                            op=operation,
                            title=c.name, note=f" {SIGNS[operation]} ".join(names),
                            layout="stack", contract=None, inputs=[], outputs=[],
                            children=children, edges=[])
        return self.box(box_of if box_of is not None else c, types=_types(c), kind="group",
                        op=operation, title=c.name, note=f" {SIGNS[operation]} ".join(names),
                        layout="row", contract=None, inputs=sorted(c.inputs), outputs=[],
                        children=children, edges=_wires(children, c.inputs))

    # Components and systems ------------------------------------------------------------

    def component(self, k: Component) -> dict:
        if k.implementation is None:          # a contract with a name
            return self.contract(k.contract, title=k.name)
        fsm = None
        try:
            from .explicit import state_graph
            fsm = state_graph(k.implementation, inputs=k.contract.inputs)
        except Exception:                     # the diagram does without it
            fsm = None
        name = getattr(k.implementation, "name", None) or type(k.implementation).__name__
        return self.box(k, types=_types(k.contract), kind="component", title=k.name,
                        contract=self.terms(k.contract),
                        model={"name": name, "fsm": fsm},
                        badge={"text": "M implements C", "status": k.status},
                        inputs=sorted(k.contract.inputs), outputs=sorted(k.contract.outputs))

    def system(self, s: System) -> dict:
        children = [self.component(k) for k in s.components]
        if s.contract is not None:
            inputs = s.contract.inputs
        else:
            driven = {v for k in s.components for v in k.contract.outputs}
            inputs = {v for k in s.components for v in k.contract.inputs if v not in driven}
        return self.box(s, types=_types(*(k.contract for k in s.components),
                                         *([s.contract] if s.contract else [])),
                        kind="group", op="system", title=s.name,
                        note=" || ".join(k.name for k in s.components), contract=None,
                        inputs=sorted(inputs), outputs=[], children=children,
                        edges=_wires(children, inputs))

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
            inner.update(kind="group", title=high.name, contract=self.terms(high),
                         badge={"text": "refines it", "status": status})
            return inner
        return self.box(high, types=_types(high), kind="contract", title=high.name,
                        contract=self.terms(high), inputs=sorted(high.inputs), outputs=sorted(high.outputs),
                        children=[inner], edges=_through(inner, high.inputs, high.outputs),
                        badge={"text": f"{inner['title']} refines it", "status": status})


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


def diagram(*items, refines: Iterable = (), title: Optional[str] = None) -> dict:
    """The diagram of contracts, components, and systems, as data for the
    page. ``refines`` lists (refined, abstract) or (refined, abstract,
    status), where the status is a string or the result of ``refines``: the
    refined one is drawn inside the abstract one, or inside the hole when
    the abstract one is a quotient. A system with a contract is drawn inside
    its contract."""
    scene = _Scene()
    nodes, top = [], {}

    def add(obj):
        if id(obj) not in top:
            node = scene.node(obj)
            top[id(obj)] = node
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
    for r in refines:
        low, high = r[0], r[1]
        status = _status(r[2]) if len(r) > 2 else None
        if isinstance(high, Contract) and high.origin and high.origin[0] == "quotient":
            if id(high) not in scene.ids:
                add(high)
            hole = _find(nodes, scene.ids[id(high)])
            inner = scene.node(low)
            hole["children"].append(inner)
            hole["edges"] += _through(inner, hole["inputs"], hole["outputs"])
            hole["badge"] = {"text": f"{inner['title']} refines it", "status": status}
            continue
        inner = top.pop(id(low), None) or scene.node(low)
        wrapped = scene.wrap(inner, high, status)
        replace(inner, wrapped)
        top[id(low)] = wrapped
    return {"title": title or "CHASE", "nodes": nodes}


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
  :root { --ink:#1f2728; --soft:#5f6c6d; --teal:#17393b; --line:#a3afaf; --fill:#f3f5f5;
          --ok:#2f7d4f; --bad:#b03a2e; --warn:#c07a14; --hole:#fdf6e3; --gold:#b8962e; }
  body { margin:0; font:13px/1.4 -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; color:var(--ink); background:#fff; }
  header { padding:10px 16px; border-bottom:1px solid #dde3e3; display:flex; gap:20px; align-items:center; flex-wrap:wrap; }
  header b { font-size:15px; }
  .legend { display:flex; gap:16px; color:var(--soft); font-size:12px; align-items:center; flex-wrap:wrap; }
  .legend i { display:inline-block; width:18px; height:12px; margin-right:5px; vertical-align:-2px; border:1.5px solid var(--teal); }
  .legend .c { border-radius:5px; } .legend .m { border-color:var(--ink); border-width:2px; }
  .legend .h { border-style:dashed; border-color:var(--gold); border-radius:5px; background:var(--hole); }
  .legend .chip { display:inline-block; font-size:10px; font-weight:700; border:1px solid var(--teal); border-radius:3px;
                  padding:0 4px; color:var(--teal); margin-right:3px; }
  #view { overflow:auto; padding:16px; }
  svg text { font-family:-apple-system, "Segoe UI", Helvetica, Arial, sans-serif; }
  .f { font-family:Menlo, Monaco, monospace; font-size:11.5px; fill:var(--ink); }
  .h { font-weight:600; font-size:13px; fill:var(--ink); cursor:pointer; }
  .s { font-size:11px; fill:var(--soft); }
  .k { font-family:Menlo, Monaco, monospace; font-size:11px; font-weight:700; fill:var(--teal); }
  .w { font-family:Menlo, Monaco, monospace; font-size:10.5px; fill:var(--soft); }
  .chip { cursor:pointer; }
  #pop { position:fixed; inset:40px; background:#fff; border:1px solid #c9d2d2; border-radius:10px;
         box-shadow:0 8px 30px rgba(0,0,0,.18); display:none; flex-direction:column; z-index:10; }
  #pop header { justify-content:space-between; }
  #pop button { font:inherit; border:1px solid #c9d2d2; background:#fff; border-radius:6px; padding:2px 10px; cursor:pointer; }
  #popview { overflow:auto; padding:8px 20px 20px; flex:1; }
  table.fsm { border-collapse:collapse; font:12px Menlo, Monaco, monospace; }
  table.fsm th { font:600 12px -apple-system, Helvetica, Arial, sans-serif; text-align:left; color:var(--soft);
                 padding:6px 14px 6px 0; border-bottom:1px solid #dde3e3; position:sticky; top:0; background:#fff; }
  table.fsm td { padding:3px 14px 3px 0; border-bottom:1px solid #f0f3f3; vertical-align:top; }
  table.fsm td.st { color:var(--teal); font-weight:600; border-bottom:1px solid #dde3e3; }
  table.fsm td.when { color:var(--soft); }
</style></head><body>
<header><b>__TITLE__</b>
  <span class="legend"><span><i class="c"></i>contract</span><span><i class="m"></i>component with a model</span>
  <span><i class="h"></i>the missing part of a quotient</span><span>a box inside a box refines it</span>
  <span><span class="chip">C</span>contract <span class="chip">M</span>model: click to open</span></span></header>
<div id="view"></div>
<div id="pop"><header><b id="poptitle"></b><button onclick="closePop()">close</button></header><div id="popview"></div></div>
<script src="https://cdn.jsdelivr.net/npm/elkjs@0.9.3/lib/elk.bundled.js"></script>
<script>
const DATA = __DATA__;
const LINE = 17, PAD = 10, PORT = 8, HEAD = 28, CHIP = 18, INLINE_STATES = 8;
const ctx = document.createElement("canvas").getContext("2d");
function textWidth(s, font) { ctx.font = font; return ctx.measureText(s).width; }
const FONT_F = "11.5px Menlo, Monaco, monospace", FONT_H = "600 13px Helvetica, Arial, sans-serif",
      FONT_S = "11px Helvetica, Arial, sans-serif", FONT_W = "10.5px Menlo, Monaco, monospace";

// What is open: the contract ("c:" + id) and the model ("m:" + id) of a
// box. At first, only the names show.
const open = new Set();
const toggle = key => () => { open.has(key) ? open.delete(key) : open.add(key); render(); };

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

const byId = {};
(function index(ns) { ns.forEach(n => { byId[n.id] = n; index(n.children || []); }); })(DATA.nodes);
const hasModel = n => n.kind === "component";
const states = n => n.model && n.model.fsm && n.model.fsm.states;
const inlineModel = n => states(n) && n.model.fsm.count <= INLINE_STATES;

// The lines under the name: [kind, letter or class, text]
function lines(n) {
  const out = [];
  if (n.note) out.push(["s", "", n.note]);
  if (n.contract && open.has("c:" + n.id)) {
    // each part opens with a tag, A or G; a term that does not fit goes on with an indent
    n.contract.A.forEach((a, j) => wrap(a).forEach((l, i) => out.push(["f", i || j ? "" : "A", l])));
    const G = n.contract.G.length ? n.contract.G : ["true"];
    G.forEach((g, j) => wrap(g).forEach((l, i) => out.push(["f", i || j ? "" : "G", l])));
  }
  if (hasModel(n) && open.has("m:" + n.id)) {
    const f = n.model.fsm;
    if (!f) out.push(["s", "", "model " + n.model.name + ": no registers, the outputs are functions of the inputs"]);
    else if (f.too_many) out.push(["s", "", "model " + n.model.name + ": more than " + f.count + " states, too many to draw"]);
    else if (!inlineModel(n)) out.push(["s", "", "model " + n.model.name + ": " + f.count + " states, in a table above the page"]);
    else out.push(["s", "", "model " + n.model.name]);
  }
  return out;
}
const lineWidth = ([k, letter, t]) => (k === "f" ? 20 : 0) + textWidth(t, k === "f" ? FONT_F : FONT_S);

function chips(n) {
  const out = [];
  if (n.contract) out.push(["C", "c:" + n.id]);
  if (hasModel(n)) out.push(["M", "m:" + n.id]);
  return out;
}
function badgeText(n) { return n.badge ? n.badge.text + " " + mark(n.badge.status)[0] : ""; }

function size(n) {     // the width and the height of the name and of the lines under it
  const ls = lines(n);
  const top = textWidth(n.title, FONT_H) + chips(n).length * (CHIP + 4) + 12
            + (n.badge ? textWidth(badgeText(n), FONT_S) + 16 : 0);
  const w = Math.max(top, ...ls.map(lineWidth)) + 2 * PAD;
  return { ls, w: Math.max(w, 120), h: HEAD + ls.length * LINE + (ls.length ? 8 : 0) };
}

// The type of a variable, from the box or the boxes inside it.
function typeOf(n, v) {
  if (n.types && n.types[v]) return n.types[v];
  for (const c of n.children || []) { const t = typeOf(c, v); if (t) return t; }
  return null;
}
const portText = (n, v) => { const t = n.types && n.types[v]; return t ? v + " " + t.label : v; };
function ports(n) {
  const ps = [], lab = v => [{ text: portText(n, v), width: textWidth(portText(n, v), FONT_W), height: 12 }];
  (n.inputs || []).forEach((v, i) => ps.push({ id: n.id + ":in:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "WEST", "elk.port.index": String(n.inputs.length - i) } }));
  (n.outputs || []).forEach((v, i) => ps.push({ id: n.id + ":out:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "EAST", "elk.port.index": String(i) } }));
  return ps;
}

let widen = {};       // the room added to a box narrower than its text
function stateGraph(n, prefix) {
  const f = n.model.fsm;
  return { children: f.states.map(st => { const id = prefix + st.id;
      byId[id] = { kind: "state", id, label: st.label, init: st.init };
      return { id, width: textWidth(st.label, FONT_W) + 26, height: 26 }; }),
    edges: f.edges.map((t, i) => ({ id: prefix + "t" + i, sources: [prefix + t.from], targets: [prefix + t.to],
      labels: t.label ? [{ text: t.label, width: textWidth(t.label, FONT_W), height: 12 }] : [] })) };
}
const FLOW = { "elk.layered.spacing.nodeNodeBetweenLayers": "80", "elk.spacing.nodeNode": "30",
               "elk.spacing.edgeLabel": "4", "elk.layered.crossingMinimization.semiInteractive": "true",
               "elk.layered.cycleBreaking.strategy": "INTERACTIVE",
               "elk.layered.layering.strategy": "INTERACTIVE" };

function toElk(n, order) {
  const sz = size(n);
  const base = { id: n.id, ports: ports(n), x: (order || 0) * 600, y: 0,
    layoutOptions: { "elk.portConstraints": "FIXED_ORDER", "elk.nodeSize.constraints": "MINIMUM_SIZE",
      "elk.portLabels.placement": "OUTSIDE", "elk.spacing.portPort": "14",
      "elk.nodeSize.minimum": "(" + sz.w + "," + Math.max(sz.h, (Math.max((n.inputs || []).length, (n.outputs || []).length) + 1) * 18) + ")" } };
  let inner = null;
  if (n.children && n.children.length) {
    inner = { children: n.children.map((c, i) => toElk(c, i)),
      edges: (n.edges || []).filter(w => !w.open).map((w, i) => ({ id: n.id + ":e" + i,
        kind: typeOf(n, w.var),
        sources: [(w.from[0] || n.id) + ":" + w.from[1] + ":" + w.from[2]],
        targets: [(w.to[0] || n.id) + ":" + w.to[1] + ":" + w.to[2]] })),
      dir: n.layout === "stack" ? "DOWN" : "RIGHT" };
  } else if (hasModel(n) && open.has("m:" + n.id) && inlineModel(n)) {
    inner = Object.assign(stateGraph(n, n.id + ":"), { dir: "RIGHT" });
  }
  if (!inner) { base.width = sz.w; base.height = sz.h; return base; }
  inner.edges.forEach(e => { KIND[e.id] = e.kind; delete e.kind; });
  base.children = inner.children; base.edges = inner.edges;
  Object.assign(base.layoutOptions, FLOW, { "elk.direction": inner.dir,
    "elk.padding": "[top=" + (sz.h + 8) + ",left=26,bottom=16,right=" + (26 + (widen[n.id] || 0)) + "]" });
  return base;
}

const NS = "http://www.w3.org/2000/svg";
function el(tag, attrs, parent) { const e = document.createElementNS(NS, tag);
  for (const k in attrs) e.setAttribute(k, attrs[k]); if (parent) parent.appendChild(e); return e; }
function text(parent, x, y, cls, s) { const t = el("text", { x, y, class: cls }, parent); t.textContent = s; return t; }
function mark(status) { return status === "valid" ? ["✓", "var(--ok)"] : status === "invalid" ? ["✗", "var(--bad)"]
                         : status ? ["?", "var(--soft)"] : ["○", "var(--soft)"]; }
const STYLE = {
  contract:  { rx: 10, fill: "#fff", stroke: "var(--teal)", width: 1.4, dash: "none" },
  component: { rx: 0, fill: "#fff", stroke: "var(--ink)", width: 2, dash: "none" },
  group:     { rx: 8, fill: "var(--fill)", stroke: "var(--line)", width: 1, dash: "none" },
  hole:      { rx: 10, fill: "var(--hole)", stroke: "var(--gold)", width: 1.4, dash: "6 4" } };

function drawState(g, e, x, y, n) {
  el("rect", { x, y, width: e.width, height: e.height, rx: 13, fill: "#fff", stroke: "var(--teal)",
               "stroke-width": n.init ? 2.4 : 1.1 }, g);
  const t = text(g, x + e.width / 2, y + 17, "w", n.label); t.setAttribute("text-anchor", "middle");
  if (n.init) el("polyline", { points: (x - 16) + "," + (y + 13) + " " + (x - 2) + "," + (y + 13),
    stroke: "var(--teal)", "stroke-width": 1.6, fill: "none", "marker-end": "url(#arrow)" }, g);
}

function drawNode(g, e, ox, oy) {
  const n = byId[e.id], x = ox + e.x, y = oy + e.y;
  if (n.kind === "state") return drawState(g, e, x, y, n);
  const box = el("g", { "data-box": n.id }, g);
  const st = n.kind === "group" && n.contract ? Object.assign({}, STYLE.contract, { fill: "var(--fill)" })
                                              : STYLE[n.kind] || STYLE.contract;
  el("rect", { x, y, width: e.width, height: e.height, rx: st.rx, fill: st.fill, stroke: st.stroke,
               "stroke-width": st.width, "stroke-dasharray": st.dash }, box);
  // the name, the C and M chips, the status
  const name = text(box, x + PAD, y + 19, "h", n.title);
  if (n.contract) name.onclick = toggle("c:" + n.id);
  let cx = x + PAD + textWidth(n.title, FONT_H) + 10;
  chips(n).forEach(([letter, key]) => {
    const on = open.has(key), c = el("g", { class: "chip" }, box);
    el("rect", { x: cx, y: y + 8, width: CHIP, height: 15, rx: 3, fill: on ? "var(--teal)" : "#fff",
                 stroke: "var(--teal)", "stroke-width": 1 }, c);
    const t = text(c, cx + CHIP / 2, y + 19.5, "k", letter); t.setAttribute("text-anchor", "middle");
    t.setAttribute("style", "font-family:Helvetica,Arial;font-size:10.5px;font-weight:700;fill:" + (on ? "#fff" : "var(--teal)"));
    c.onclick = letter === "M" && on === false && states(n) && !inlineModel(n)
      ? () => { open.add(key); render(); showModel(n); } : toggle(key);
    cx += CHIP + 4; });
  if (n.badge) { const [m, col] = mark(n.badge.status);
    const b = text(box, x + e.width - PAD, y + 19, "s", badgeText(n));
    b.setAttribute("text-anchor", "end"); b.setAttribute("fill", col); }
  size(n).ls.forEach(([k, letter, s], i) => {
    const ty = y + HEAD + 12 + i * LINE;
    if (k === "f") {
      if (letter) { el("rect", { x: x + PAD, y: ty - 10.5, width: 13, height: 13, rx: 3,
                                 fill: letter === "A" ? "#7a8b8c" : "var(--teal)" }, box);
                    const t = text(box, x + PAD + 6.5, ty, "k", letter); t.setAttribute("text-anchor", "middle");
                    t.setAttribute("style", "font-family:Helvetica,Arial;font-size:9.5px;font-weight:700;fill:#fff"); }
      text(box, x + PAD + 20, ty, "f", s); }
    else text(box, x + PAD, ty, "s", s); });
  (e.ports || []).forEach(p => {
    el("rect", { x: x + p.x, y: y + p.y, width: p.width, height: p.height, fill: "var(--teal)" }, g);
    (p.labels || []).forEach(l => text(g, x + p.x + l.x, y + p.y + l.y + 10, "w", l.text)); });
  (e.children || []).forEach(c => drawNode(g, c, x, y));
  (e.edges || []).forEach(ed => drawEdge(g, ed, x, y));
  (n.edges || []).filter(w => w.open).forEach(w => {
    const c = (e.children || []).find(c => c.id === w.open[0]); if (!c) return;
    const p = (c.ports || []).find(p => p.id === w.open[0] + ":in:" + w.var); if (!p) return;
    const px = x + c.x + p.x, py = y + c.y + p.y + PORT / 2;
    el("line", { x1: px - 26, y1: py, x2: px, y2: py, stroke: "var(--warn)", "stroke-width": 1.6,
                 "stroke-dasharray": "4 3" }, g);
    const t = text(g, px - 28, py - 4, "w", "not driven"); t.setAttribute("text-anchor", "end");
    t.setAttribute("fill", "var(--warn)"); });
}

const KIND = {};       // the type of the variable of each wire, by the id of the wire
function drawEdge(g, ed, ox, oy) {
  const k = KIND[ed.id];
  (ed.sections || []).forEach(s => {
    const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint];
    el("polyline", { points: pts.map(p => (ox + p.x) + "," + (oy + p.y)).join(" "), fill: "none",
                     stroke: "var(--soft)", "stroke-width": k && k.bus ? 2.6 : k ? 2 : 1.2,
                     "marker-end": "url(#arrow)" }, g);
    if (k && k.bus) {         // a bus: a slash and its width, in the middle of its longest segment
      let a = pts[0], b = pts[1] || pts[0];
      for (let i = 0; i + 1 < pts.length; i++)
        if (Math.hypot(pts[i + 1].x - pts[i].x, pts[i + 1].y - pts[i].y) > Math.hypot(b.x - a.x, b.y - a.y)) { a = pts[i]; b = pts[i + 1]; }
      const mx = ox + (a.x + b.x) / 2, my = oy + (a.y + b.y) / 2;
      el("line", { x1: mx - 4, y1: my + 5, x2: mx + 4, y2: my - 5, stroke: "var(--soft)", "stroke-width": 1.4 }, g);
      text(g, mx + 3, my - 7, "w", String(k.bus));
    } });
  (ed.labels || []).forEach(l => text(g, ox + l.x, oy + l.y + 10, "w", l.text));
}

function svgIn(container, width, height) {
  container.innerHTML = "";
  const svg = el("svg", { width: width + 40, height: height + 40 }, container);
  const m = el("marker", { id: "arrow", viewBox: "0 0 10 10", refX: 9, refY: 5, markerWidth: 8,
                           markerHeight: 8, markerUnits: "userSpaceOnUse", orient: "auto" }, el("defs", {}, svg));
  el("path", { d: "M0,0 L10,5 L0,10 z", fill: "var(--soft)" }, m);
  return el("g", { transform: "translate(20,20)" }, svg);
}

// ELK does not keep the minimum width of a box with boxes inside: a second
// layout adds the room that its text needs.
function narrow(e, todo) {
  const n = byId[e.id];
  if (n && n.kind !== "state" && e.children && e.children.length) {
    const need = size(n).w - e.width;
    if (need > 1) { widen[n.id] = (widen[n.id] || 0) + need; todo.push(n.id); }
  }
  (e.children || []).forEach(c => narrow(c, todo));
}
function layout() {
  return new ELK().layout({ id: "root", children: DATA.nodes.map((n, i) => toElk(n, i)), edges: [],
    // with INCLUDE_CHILDREN, every level takes the direction of the root
    layoutOptions: Object.assign({ "elk.algorithm": "layered", "elk.direction": "RIGHT",
      "elk.hierarchyHandling": "INCLUDE_CHILDREN" }, FLOW, { "elk.spacing.nodeNode": "40" }) });
}
async function render() {
  widen = {};
  let out = await layout();
  for (let pass = 0; pass < 3; pass++) {
    const todo = []; (out.children || []).forEach(c => narrow(c, todo));
    if (!todo.length) break;
    out = await layout();
  }
  const g = svgIn(document.getElementById("view"), out.width, out.height);
  (out.children || []).forEach(c => drawNode(g, c, 0, 0));
}

// A model with many states is shown over the page as a table: each state,
// and the states it moves to, with the inputs that take each move. A graph
// of so many states, with free inputs, is a tangle.
function showModel(n) {
  const f = n.model.fsm, label = {};
  f.states.forEach(st => { label[st.id] = st.label; });
  const esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  let rows = "";
  f.states.forEach(st => {
    const moves = f.edges.filter(e => e.from === st.id);
    rows += moves.map((e, i) => "<tr>" + (i ? "" : "<td rowspan='" + moves.length + "' class='st'>"
        + (st.init ? "<b>→ </b>" : "") + esc(st.label) + "</td>")
      + "<td class='to'>" + esc(label[e.to]) + "</td><td class='when'>"
      + esc(e.label || (f.inputs_in_state ? "" : "any input")) + "</td></tr>").join("");
  });
  document.getElementById("poptitle").textContent = n.title + ": model " + n.model.name + ", "
    + f.count + (f.inputs_in_state ? " states (a state lists the variables that are 1, the inputs among them: a move needs no condition)"
                                   : " states (a state lists the registers that are 1)");
  document.getElementById("popview").innerHTML = "<table class='fsm'><tr><th>state</th><th>next state</th>"
    + "<th>when the inputs are</th></tr>" + rows + "</table>";
  document.getElementById("pop").style.display = "flex";
}
function closePop() {
  document.getElementById("pop").style.display = "none";
  Object.values(byId).forEach(n => open.delete("m:" + n.id)); render();
}
render().catch(e => { document.getElementById("view").textContent = "Cannot draw the diagram: " + e; });
</script>
__RELOAD__
</body></html>
"""
