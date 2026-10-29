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


def _status(s):
    return getattr(s, "status", s)


class _Scene:
    def __init__(self):
        self.count = 0
        self.ids = {}           # the first box of an object

    def fresh(self, prefix="n") -> str:
        self.count += 1
        return f"{prefix}{self.count}"

    def box(self, obj, **node) -> dict:
        node.setdefault("id", self.fresh())
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
            return self.box(c, kind="contract", title=title or c.name, note=note,
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
            hole = self.box(c, kind="hole", title=c.name, note=f"{dividend.name} / {divisor.name}",
                            contract=self.terms(c), inputs=sorted(c.inputs),
                            outputs=sorted(c.outputs))
            node = self.group(dividend, operation, known + [hole], box_of=c)
            node["note"] = " || ".join([k["title"] for k in known] + ["?"])
            return node
        return self.group(c, operation, [self.contract(o) for o in operands])

    def group(self, c: Contract, operation: str, children: list, box_of=None) -> dict:
        names = [ch["title"] for ch in children]
        if operation in STACKED:      # viewpoints of one component: the same ports, no wires
            return self.box(box_of if box_of is not None else c, kind="group", op=operation,
                            title=c.name, note=f" {SIGNS[operation]} ".join(names),
                            layout="stack", contract=None, inputs=[], outputs=[],
                            children=children, edges=[])
        return self.box(box_of if box_of is not None else c, kind="group", op=operation,
                        title=c.name, note=f" {SIGNS[operation]} ".join(names),
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
        return self.box(k, kind="component", title=k.name, contract=self.terms(k.contract),
                        model={"name": name, "fsm": fsm},
                        badge={"text": "model implements contract", "status": k.status},
                        inputs=sorted(k.contract.inputs), outputs=sorted(k.contract.outputs))

    def system(self, s: System) -> dict:
        children = [self.component(k) for k in s.components]
        if s.contract is not None:
            inputs = s.contract.inputs
        else:
            driven = {v for k in s.components for v in k.contract.outputs}
            inputs = {v for k in s.components for v in k.contract.inputs if v not in driven}
        return self.box(s, kind="group", op="system", title=s.name,
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
                         badge={"text": "the decomposition refines it", "status": status})
            return inner
        return self.box(high, kind="contract", title=high.name, contract=self.terms(high),
                        inputs=sorted(high.inputs), outputs=sorted(high.outputs),
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
  :root { --ink:#1f2728; --soft:#586566; --teal:#17393b; --line:#9aa7a7; --fill:#f3f5f5;
          --ok:#2f7d4f; --bad:#b03a2e; --hole:#fdf6e3; --link:#1c6a6f; }
  body { margin:0; font:13px/1.4 -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; color:var(--ink); background:#fff; }
  header { padding:10px 16px; border-bottom:1px solid #dde3e3; }
  header b { font-size:15px; margin-right:16px; }
  .legend { display:inline-flex; gap:18px; color:var(--soft); font-size:12px; align-items:center; flex-wrap:wrap; }
  .legend i { display:inline-block; width:18px; height:12px; margin-right:6px; vertical-align:-2px; border:1.5px solid var(--teal); }
  .legend .c { border-radius:5px; } .legend .m { border-color:var(--ink); border-width:2px; }
  .legend .h { border-style:dashed; border-radius:5px; background:var(--hole); }
  #view { overflow:auto; padding:16px; }
  svg text { font-family:-apple-system, "Segoe UI", Helvetica, Arial, sans-serif; }
  .f { font-family:Menlo, Monaco, monospace; font-size:11.5px; fill:var(--ink); }
  .h { font-weight:600; font-size:13px; fill:var(--ink); cursor:pointer; }
  .s { font-size:11px; fill:var(--soft); }
  .k { font-size:9.5px; font-weight:600; letter-spacing:.06em; fill:var(--soft); }
  .w { font-family:Menlo, Monaco, monospace; font-size:10.5px; fill:var(--soft); }
  .l { font-size:11.5px; fill:var(--link); cursor:pointer; }
</style></head><body>
<header><b>__TITLE__</b>
  <span class="legend"><span><i class="c"></i>contract</span><span><i class="m"></i>component: contract and model</span>
  <span><i class="h"></i>the missing part of a quotient</span><span>a box inside a box: it refines it</span>
  <span>click ▸ to open</span></span></header>
<div id="view"></div>
<script src="https://cdn.jsdelivr.net/npm/elkjs@0.9.3/lib/elk.bundled.js"></script>
<script>
const DATA = __DATA__;
const LINE = 17, PAD = 10, PORT = 8, HEAD = 26;
const ctx = document.createElement("canvas").getContext("2d");
function textWidth(s, font) { ctx.font = font; return ctx.measureText(s).width; }
const FONT_F = "11.5px Menlo, Monaco, monospace", FONT_H = "600 13px Helvetica, Arial, sans-serif",
      FONT_S = "11.5px Helvetica, Arial, sans-serif", FONT_W = "10.5px Menlo, Monaco, monospace";
const fontOf = c => c === "f" ? FONT_F : c === "h" ? FONT_H : FONT_S;
const count = (k, word) => k + " " + word + (k === 1 ? "" : "s");

// What is open: the contract of a box ("c:" + id) and the model of a
// component ("m:" + id). At first, only the boxes and their names show.
const open = new Set();
const widen = {};       // the room added to a box narrower than its text
const toggle = key => () => { open.has(key) ? open.delete(key) : open.add(key); render(); };

// A long formula on several lines, cut before an operator.
const WIDTH = 64;
function wrap(f) {
  const out = []; let rest = f;
  while (rest.length > WIDTH) {
    let cut = -1;
    for (const op of [" & ", " | ", " -> ", " <-> ", " "]) {
      const i = rest.lastIndexOf(op, WIDTH); if (i > WIDTH / 3) { cut = i; break; } }
    if (cut < 0) cut = WIDTH;
    out.push(rest.slice(0, cut)); rest = "    " + rest.slice(cut).trimStart();
  }
  out.push(rest); return out;
}

// The lines under the name of a box: [class, text, key to toggle]
function lines(n) {
  const out = [];
  if (n.note) out.push(["s", n.note]);
  if (n.contract) {
    const c = n.contract, key = "c:" + n.id;
    if (!open.has(key)) out.push(["l", "▸ contract " + c.name + ": " + count(c.A.length, "assumption")
                                  + ", " + count(c.G.length, "guarantee"), key]);
    else {
      out.push(["l", "▾ contract " + c.name, key]);
      if (c.A.length) { out.push(["k", "ASSUMES"]); c.A.forEach(a => wrap(a).forEach(l => out.push(["f", l]))); }
      out.push(["k", "GUARANTEES"]);
      if (c.G.length) c.G.forEach(g => wrap(g).forEach(l => out.push(["f", l]))); else out.push(["f", "true"]);
    }
  }
  if (n.model) {
    const m = n.model, key = "m:" + n.id, f = m.fsm;
    if (!f) out.push(["s", "model " + m.name]);
    else if (f.too_many) out.push(["s", "model " + m.name + ": more than 24 states"]);
    else out.push(["l", (open.has(key) ? "▾ " : "▸ ") + "model " + m.name + ": " + count(f.count, "state"), key]);
  }
  return out;
}

function ports(n) {
  const ps = [], lab = v => [{ text: v, width: textWidth(v, FONT_W), height: 12 }];
  (n.inputs || []).forEach((v, i) => ps.push({ id: n.id + ":in:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "WEST", "elk.port.index": String(n.inputs.length - i) } }));
  (n.outputs || []).forEach((v, i) => ps.push({ id: n.id + ":out:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "EAST", "elk.port.index": String(i) } }));
  return ps;
}

const byId = {};
(function index(ns) { ns.forEach(n => { byId[n.id] = n; index(n.children || []); }); })(DATA.nodes);

function header(n) {     // width and height of the name and the lines under it
  const ls = lines(n);
  let w = textWidth(n.title, FONT_H) + 2 * PAD + (n.badge ? textWidth(n.badge.text, FONT_S) + 40 : 0);
  ls.forEach(([c, t]) => { w = Math.max(w, textWidth(t, fontOf(c)) + 2 * PAD); });
  return { ls, w: Math.max(w, 150), h: HEAD + ls.length * LINE + (ls.length ? 6 : 0) };
}

function toElk(n) {
  const hd = header(n);
  const base = { id: n.id, ports: ports(n),
    layoutOptions: { "elk.portConstraints": "FIXED_ORDER", "elk.nodeSize.constraints": "MINIMUM_SIZE",
      "elk.portLabels.placement": "OUTSIDE", "elk.spacing.portPort": "14",
      "elk.nodeSize.minimum": "(" + hd.w + "," + Math.max(hd.h, ((Math.max((n.inputs || []).length, (n.outputs || []).length)) + 1) * 18) + ")" } };
  let inner = null;
  if (n.children && n.children.length) {
    inner = { children: n.children.map(toElk),
      edges: (n.edges || []).filter(w => !w.open).map((w, i) => ({ id: n.id + ":e" + i,
        sources: [(w.from[0] || n.id) + ":" + w.from[1] + ":" + w.from[2]],
        targets: [(w.to[0] || n.id) + ":" + w.to[1] + ":" + w.to[2]] })),
      dir: n.layout === "stack" ? "DOWN" : "RIGHT" };
  } else if (n.model && n.model.fsm && n.model.fsm.states && open.has("m:" + n.id)) {
    const f = n.model.fsm;
    inner = { children: f.states.map(st => { const id = n.id + ":" + st.id;
        byId[id] = { kind: "state", id, label: st.label, init: st.init };
        return { id, width: textWidth(st.label, FONT_W) + 26, height: 26 }; }),
      edges: f.edges.map((t, i) => ({ id: n.id + ":t" + i, sources: [n.id + ":" + t.from],
        targets: [n.id + ":" + t.to],
        labels: t.label ? [{ text: t.label, width: textWidth(t.label, FONT_W), height: 12 }] : [] })),
      dir: "RIGHT" };
  }
  if (!inner) { base.width = hd.w; base.height = hd.h; return base; }
  base.children = inner.children; base.edges = inner.edges;
  Object.assign(base.layoutOptions, { "elk.direction": inner.dir,
    "elk.padding": "[top=" + (hd.h + 8) + ",left=24,bottom=16,right=" + (24 + (widen[n.id] || 0)) + "]",
    "elk.spacing.nodeNode": "30", "elk.layered.spacing.nodeNodeBetweenLayers": "80",
    "elk.layered.cycleBreaking.strategy": "MODEL_ORDER", "elk.spacing.edgeLabel": "4" });
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
  hole:      { rx: 10, fill: "var(--hole)", stroke: "#b8962e", width: 1.4, dash: "6 4" } };

function drawNode(g, e, ox, oy) {
  const n = byId[e.id], x = ox + e.x, y = oy + e.y;
  if (n.kind === "state") {
    el("rect", { x, y, width: e.width, height: e.height, rx: 13, fill: "#fff", stroke: "var(--teal)",
                 "stroke-width": n.init ? 2.4 : 1.1 }, g);
    const t = text(g, x + e.width / 2, y + 17, "w", n.label); t.setAttribute("text-anchor", "middle");
    if (n.init) el("polyline", { points: (x - 16) + "," + (y + 13) + " " + (x - 2) + "," + (y + 13),
      stroke: "var(--teal)", "stroke-width": 1.6, fill: "none", "marker-end": "url(#arrow)" }, g);
    return;
  }
  const st = STYLE[n.kind] || STYLE.contract;
  // a group that holds a decomposition refining a contract looks like the contract
  const s2 = n.kind === "group" && n.contract ? Object.assign({}, STYLE.contract, { fill: "var(--fill)" }) : st;
  el("rect", { x, y, width: e.width, height: e.height, rx: s2.rx, fill: s2.fill, stroke: s2.stroke,
               "stroke-width": s2.width, "stroke-dasharray": s2.dash }, g);
  if (n.kind === "component") el("line", { x1: x, y1: y + HEAD, x2: x + e.width, y2: y + HEAD,
                                           stroke: "var(--ink)", "stroke-width": 1 }, g);
  text(g, x + PAD, y + 18, "h", n.title);
  if (n.badge) { const [m, col] = mark(n.badge.status);
    const b = text(g, x + e.width - PAD, y + 18, "s", n.badge.text + " " + m);
    b.setAttribute("text-anchor", "end"); b.setAttribute("fill", col); }
  header(n).ls.forEach(([c, s, key], i) => { const t = text(g, x + PAD, y + HEAD + 12 + i * LINE, c, s);
    if (key) t.onclick = toggle(key); });
  (e.ports || []).forEach(p => {
    el("rect", { x: x + p.x, y: y + p.y, width: p.width, height: p.height, fill: "var(--teal)" }, g);
    (p.labels || []).forEach(l => text(g, x + p.x + l.x, y + p.y + l.y + 10, "w", l.text)); });
  (e.children || []).forEach(c => drawNode(g, c, x, y));
  (e.edges || []).forEach(ed => drawEdge(g, ed, x, y));
  (n.edges || []).filter(w => w.open).forEach(w => {
    const c = (e.children || []).find(c => c.id === w.open[0]); if (!c) return;
    const p = (c.ports || []).find(p => p.id === w.open[0] + ":in:" + w.var); if (!p) return;
    const px = x + c.x + p.x, py = y + c.y + p.y + PORT / 2;
    el("line", { x1: px - 22, y1: py, x2: px, y2: py, stroke: "var(--bad)", "stroke-width": 2 }, g);
    const t = text(g, px - 24, py - 4, "w", "open"); t.setAttribute("text-anchor", "end"); t.setAttribute("fill", "var(--bad)"); });
}

function drawEdge(g, ed, ox, oy) {
  (ed.sections || []).forEach(s => {
    const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint].map(p => (ox + p.x) + "," + (oy + p.y));
    el("polyline", { points: pts.join(" "), fill: "none", stroke: "var(--soft)", "stroke-width": 1.2,
                     "marker-end": "url(#arrow)" }, g); });
  (ed.labels || []).forEach(l => text(g, ox + l.x, oy + l.y + 10, "w", l.text));
}

// ELK does not keep the minimum width of a box with boxes inside: a second
// layout adds the room that its text needs.
function narrow(e, todo) {
  const n = byId[e.id];
  if (n && n.kind !== "state" && e.children && e.children.length) {
    const need = header(n).w - e.width;
    if (need > 1) { widen[n.id] = (widen[n.id] || 0) + need; todo.push(n.id); }
  }
  (e.children || []).forEach(c => narrow(c, todo));
}
async function layout() {
  return new ELK().layout({ id: "root", children: DATA.nodes.map(toElk), edges: [],
    layoutOptions: { "elk.algorithm": "layered", "elk.direction": "DOWN",
      "elk.hierarchyHandling": "INCLUDE_CHILDREN", "elk.spacing.nodeNode": "40" } });
}
async function render() {
  let out = await layout();
  for (let pass = 0; pass < 3; pass++) {
    const todo = []; (out.children || []).forEach(c => narrow(c, todo));
    if (!todo.length) break;
    out = await layout();
  }
  const view = document.getElementById("view"); view.innerHTML = "";
  const svg = el("svg", { width: out.width + 40, height: out.height + 40 }, view);
  const m = el("marker", { id: "arrow", viewBox: "0 0 10 10", refX: 9, refY: 5, markerWidth: 7,
                           markerHeight: 7, orient: "auto" }, el("defs", {}, svg));
  el("path", { d: "M0,0 L10,5 L0,10 z", fill: "var(--soft)" }, m);
  const g = el("g", { transform: "translate(20,20)" }, svg);
  (out.children || []).forEach(c => drawNode(g, c, 0, 0));
}
render().catch(e => { document.getElementById("view").textContent = "Cannot draw the diagram: " + e; });
</script>
__RELOAD__
</body></html>
"""
