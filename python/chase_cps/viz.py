"""Diagrams of contracts, components, and systems.

The diagram follows two rules. Inside a level, components and contracts sit
side by side, connected by the names of their variables (composition).
Between levels, the abstract contract sits above the one that refines it,
with an arrow marked with the status of the refinement. The other
operations are drawn inside a box: a quotient is a box with a dashed hole,
filled by the computed contract; merging and conjunction stack their
operands in one box. A contract is a rounded box with its assumptions and
guarantees; a component is a square box, with its contract, its
implementation, and whether the implementation implements the contract.

The structure comes from the contracts themselves: each result of the
algebra remembers how it was computed (``Contract.origin``). The page is
drawn in the browser, laid out by ELK (the Eclipse Layout Kernel, loaded
from a CDN).

    from chase_cps import viz
    viz.save(viz.diagram(plan, refines=[(plan, parent, "valid")]), "plan.html")
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

# Operations drawn as a box with its operands inside, and the sign used.
SIGNS = {"composition": "∥", "quotient": "/", "merging": "•", "conjunction": "∧",
         "disjunction": "∨", "separation": "÷"}
STACKED = {"merging", "conjunction", "disjunction", "separation"}
# The signs in the page, in characters that every font has.
TEXT_SIGNS = {"composition": "||", "quotient": "/", "merging": "merged with",
              "conjunction": "and", "disjunction": "or", "separation": "separated by"}


class _Scene:
    def __init__(self):
        self.ids = {}

    def fresh(self, prefix="g") -> str:
        self.count = getattr(self, "count", 0) + 1
        return f"{prefix}_{self.count}"

    def id(self, obj, prefix="n") -> str:
        """The id of the first box of obj; a second box of the same object
        (a contract in two compositions) gets an id of its own."""
        key = id(obj)
        if key in self.ids:
            return self.fresh(prefix)
        self.ids[key] = f"{prefix}{len(self.ids)}"
        return self.ids[key]

    # Nodes --------------------------------------------------------------------

    def leaf(self, c: Contract, kind="contract", **extra) -> dict:
        return {"id": self.id(c), "kind": kind, "title": c.name,
                "inputs": sorted(c.inputs), "outputs": sorted(c.outputs),
                "A": [ltl.to_str(a) for a in c.assumptions],
                "G": [ltl.to_str(g) for g in c.guarantees], **extra}

    def contract(self, c: Contract) -> dict:
        origin = c.origin
        if not origin or origin[0] not in SIGNS:
            note = None
            if origin:
                note = f"{origin[0]} of " + ", ".join(o.name for o in origin[1])
            return self.leaf(c, note=note)
        operation, operands = origin
        if operation == "composition":
            parts = []
            for o in operands:            # a chain of compositions is one level
                if o.origin and o.origin[0] == "composition":
                    parts += self.contract(o)["children"]
                else:
                    parts.append(self.contract(o))
            return self.group(c, operation, parts)
        if operation == "quotient":
            dividend, divisor = operands
            known = self.contract(divisor)
            known = known["children"] if known.get("op") == "composition" else [known]
            hole = self.leaf(c, kind="hole", note=f"{dividend.name} / {divisor.name}")
            node = self.group(dividend, operation, known + [hole])
            node["id"] = self.fresh("q")
            node["note"] = " || ".join([k["title"] for k in known] + ["? (" + c.name + ")"])
            return node
        return self.group(c, operation, [self.contract(o) for o in operands])

    def group(self, c: Contract, operation: str, children: list) -> dict:
        names = [ch["title"] for ch in children]
        return {"id": self.id(c), "kind": "group", "op": operation, "title": c.name,
                "sign": SIGNS[operation],
                "note": f" {TEXT_SIGNS[operation]} ".join(names),
                "layout": "stack" if operation in STACKED else "row",
                "inputs": sorted(c.inputs), "outputs": [],
                "children": children, "edges": _wires(children, c.inputs, c.outputs)}

    def component(self, k: Component) -> dict:
        impl = None
        if k.implementation is not None:
            impl = getattr(k.implementation, "name", None) or type(k.implementation).__name__
        node = self.leaf(k.contract, kind="component", impl=impl, status=k.status)
        node["id"] = self.id(k)
        node["title"] = k.name
        node["contract"] = k.contract.name
        return node

    def system(self, s: System) -> dict:
        children = [self.component(k) for k in s.components]
        if s.contract is not None:
            inputs, outputs = s.contract.inputs, s.contract.outputs
        else:
            driven = {v for k in s.components for v in k.contract.outputs}
            inputs = {v for k in s.components for v in k.contract.inputs if v not in driven}
            outputs = driven
        return {"id": self.id(s), "kind": "group", "op": "system", "title": s.name,
                "sign": "∥", "note": " || ".join(k.name for k in s.components),
                "layout": "row", "inputs": sorted(inputs), "outputs": [],
                "children": children, "edges": _wires(children, inputs, outputs)}

    def node(self, obj) -> dict:
        if isinstance(obj, System):
            return self.system(obj)
        if isinstance(obj, Component):
            return self.component(obj)
        if isinstance(obj, Contract):
            return self.contract(obj)
        raise TypeError(f"cannot draw {type(obj).__name__}")


def _wires(children: list, inputs, outputs) -> list:
    """The connections inside a box: an output of a child to the inputs of
    the others with the same name; an input of the box to the children that
    read it; an output of a child to the output of the box. An input read by
    a child that nobody drives is marked open."""
    wires = []
    drivers = {v: ch["id"] for ch in children for v in ch["outputs"]}
    for ch in children:
        for v in ch["inputs"]:
            if v in drivers and drivers[v] != ch["id"]:
                wires.append({"from": [drivers[v], "out", v], "to": [ch["id"], "in", v], "var": v})
            elif v in inputs:
                wires.append({"from": [None, "in", v], "to": [ch["id"], "in", v], "var": v})
            else:
                wires.append({"open": [ch["id"], "in", v], "var": v})
    return wires


def diagram(*items, refines: Iterable = (), title: Optional[str] = None) -> dict:
    """The diagram of contracts, components, and systems, as data for the
    page. ``refines`` lists (refined, abstract) or (refined, abstract,
    status), where the status is a string or the result of ``refines``.

    A decomposition that refines a contract (a system with its contract, or
    a composition in ``refines``) is drawn inside the contract: the box of
    the contract, with its assumptions and guarantees at the top, holds the
    components. Any other refinement, a contract that can replace another,
    is an arrow from the refined contract to the one above it."""
    scene = _Scene()
    nodes, arrows, top = [], [], {}

    def add(obj):
        if id(obj) not in top:
            node = scene.node(obj)
            top[id(obj)] = node["id"]
            nodes.append(node)
        return top[id(obj)]

    def inside(node, contract, status):
        node["spec"] = {"title": contract.name, "status": status,
                        "A": [ltl.to_str(a) for a in contract.assumptions],
                        "G": [ltl.to_str(g) for g in contract.guarantees]}
        node["inputs"] = sorted(v for v in contract.inputs
                                if any(w.get("from", [0])[0] is None and w["var"] == v
                                       for w in node["edges"]))

    for item in items:
        add(item)
        if isinstance(item, System) and item.contract is not None:
            inside(next(n for n in nodes if n["id"] == top[id(item)]), item.contract, item.status)
    for r in refines:
        low, high = r[0], r[1]
        status = r[2] if len(r) > 2 else None
        status = getattr(status, "status", status)
        decomposed = isinstance(low, System) or (
            isinstance(low, Contract) and low.origin and low.origin[0] == "composition")
        if decomposed and id(low) not in top:
            add(low)
            inside(next(n for n in nodes if n["id"] == top[id(low)]), high, status)
        else:
            to = add(high)
            if isinstance(high, Contract) and high.origin and high.origin[0] == "quotient":
                to = scene.ids[id(high)]        # the hole that it fills
            arrows.append({"from": add(low), "to": to, "status": status})
    return {"title": title or "CHASE", "nodes": nodes, "refines": arrows}


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
  :root { --ink:#1f2728; --soft:#586566; --teal:#17393b; --line:#8a9999; --fill:#f4f6f6;
          --ok:#2f7d4f; --bad:#b03a2e; --hole:#fdf3d8; }
  body { margin:0; font:13px/1.4 -apple-system, Helvetica, Arial, sans-serif; color:var(--ink); background:#fff; }
  header { padding:10px 16px; border-bottom:1px solid #dde3e3; display:flex; gap:16px; align-items:baseline; }
  header b { font-size:15px; } header span { color:var(--soft); font-size:12px; }
  #view { overflow:auto; padding:16px; }
  svg text { font-family:-apple-system, Helvetica, Arial, sans-serif; }
  .f { font-family:Menlo, Monaco, monospace; font-size:11.5px; fill:var(--ink); }
  .h { font-weight:700; font-size:13px; fill:var(--ink); cursor:pointer; }
  .s { font-size:11px; fill:var(--soft); }
  .k { font-size:9.5px; font-weight:700; letter-spacing:.06em; fill:var(--soft); }
  .w { font-family:Menlo, Monaco, monospace; font-size:10.5px; fill:var(--soft); }
</style></head><body>
<header><b>__TITLE__</b><span>click a title to fold or unfold it</span></header>
<div id="view"></div>
<script src="https://cdn.jsdelivr.net/npm/elkjs@0.9.3/lib/elk.bundled.js"></script>
<script>
const DATA = __DATA__;
const folded = new Set();
const LINE = 16, PAD = 10, PORT = 8;
const ctx = document.createElement("canvas").getContext("2d");
function textWidth(s, font) { ctx.font = font; return ctx.measureText(s).width; }
const FONT_F = "11.5px Menlo, Monaco, monospace", FONT_H = "700 13px Helvetica, Arial, sans-serif",
      FONT_S = "11px Helvetica, Arial, sans-serif", FONT_W = "10.5px Menlo, Monaco, monospace";

// The lines drawn in a leaf box: [class, text]
function lines(n) {
  const out = [["h", n.title]];
  if (n.kind === "component") {
    out.push(["s", "contract " + n.contract + (n.impl ? " · implementation " + n.impl : "")]);
  } else if (n.note) out.push(["s", n.note]);
  if (folded.has(n.id)) { out.push(["s", n.A.length + " assumptions · " + n.G.length + " guarantees"]); return out; }
  if (n.A.length) { out.push(["k", "ASSUMES"]); n.A.forEach(a => wrap(a).forEach(l => out.push(["f", l]))); }
  out.push(["k", "GUARANTEES"]);
  if (n.G.length) n.G.forEach(g => wrap(g).forEach(l => out.push(["f", l]))); else out.push(["f", "true"]);
  return out;
}

// A long formula on several lines, cut before an operator; the next lines
// are indented.
const WIDTH = 64;
function wrap(f) {
  const lines = [];
  let rest = f;
  while (rest.length > WIDTH) {
    let cut = -1;
    for (const op of [" & ", " | ", " -> ", " <-> ", " "]) {
      const i = rest.lastIndexOf(op, WIDTH);
      if (i > WIDTH / 3) { cut = i; break; }
    }
    if (cut < 0) cut = WIDTH;
    lines.push(rest.slice(0, cut));
    rest = "    " + rest.slice(cut).trimStart();
  }
  lines.push(rest);
  return lines;
}
const fontOf = c => c === "f" ? FONT_F : c === "h" ? FONT_H : FONT_S;

// A box that holds a decomposition: the contract it refines, at the top.
function heading(n) { return n.spec ? n.spec.title + "  =  " + n.note : n.title + "  =  " + n.note; }
function specLines(n) {
  if (!n.spec) return [];
  const out = [];
  if (n.spec.A.length) { out.push(["k", "ASSUMES"]); n.spec.A.forEach(a => wrap(a).forEach(l => out.push(["f", l]))); }
  out.push(["k", "GUARANTEES"]);
  n.spec.G.forEach(g => wrap(g).forEach(l => out.push(["f", l])));
  return out;
}

function ports(n) {
  const ps = [];
  const lab = v => [{ text: v, width: textWidth(v, FONT_W), height: 12 }];
  n.inputs.forEach((v, i) => ps.push({ id: n.id + ":in:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "WEST", "elk.port.index": String(n.inputs.length - i) } }));
  n.outputs.forEach((v, i) => ps.push({ id: n.id + ":out:" + v, width: PORT, height: PORT, labels: lab(v),
    layoutOptions: { "elk.port.side": "EAST", "elk.port.index": String(i) } }));
  return ps;
}

function toElk(n) {
  const base = { id: n.id, ports: ports(n),
    layoutOptions: { "elk.portConstraints": "FIXED_ORDER", "elk.nodeSize.constraints": "MINIMUM_SIZE",
                     "elk.portLabels.placement": "OUTSIDE", "elk.spacing.portPort": "14" } };
  if (n.kind === "group" && !folded.has(n.id)) {
    const sl = specLines(n);
    const titleW = Math.max(textWidth(heading(n), FONT_H),
                            ...sl.map(([c, t]) => textWidth(t, fontOf(c)) + 2 * PAD)) + 2 * PAD;
    base.children = n.children.map(toElk);
    base.edges = n.edges.filter(w => !w.open).map((w, i) => ({
      id: n.id + ":e" + i,
      sources: [(w.from[0] || n.id) + ":" + w.from[1] + ":" + w.from[2]],
      targets: [(w.to[0] || n.id) + ":" + w.to[1] + ":" + w.to[2]],
      labels: [] }));
    Object.assign(base.layoutOptions, {
      "elk.direction": n.layout === "stack" ? "DOWN" : "RIGHT",
      "elk.padding": "[top=" + (46 + (sl.length ? sl.length * LINE + 2 * PAD + 10 : 0)) + ",left=26,bottom=18,right=26]",
      "elk.spacing.nodeNode": "36", "elk.layered.spacing.nodeNodeBetweenLayers": "90",
      "elk.layered.cycleBreaking.strategy": "MODEL_ORDER",
      "elk.nodeSize.minimum": "(" + titleW + ",60)" });
    return base;
  }
  const ls = lines(n);
  const w = Math.max(160, ...ls.map(([c, t]) => textWidth(t, fontOf(c)))) + 2 * PAD + 8;
  const h = ls.length * LINE + 2 * PAD + (n.kind === "component" ? 6 : 0);
  base.layoutOptions["elk.nodeSize.minimum"] = "(" + w + "," + Math.max(h, (Math.max(n.inputs.length, n.outputs.length) + 1) * 18) + ")";
  base.width = w; base.height = h;
  return base;
}

const byId = {};
(function index(ns) { ns.forEach(n => { byId[n.id] = n; if (n.children) index(n.children); }); })(DATA.nodes);

const NS = "http://www.w3.org/2000/svg";
function el(tag, attrs, parent) { const e = document.createElementNS(NS, tag);
  for (const k in attrs) e.setAttribute(k, attrs[k]); if (parent) parent.appendChild(e); return e; }
function text(parent, x, y, cls, s) { const t = el("text", { x, y, class: cls }, parent); t.textContent = s; return t; }

function mark(status) { return status === "valid" ? ["✓", "var(--ok)"] : status === "invalid" ? ["✗", "var(--bad)"]
                         : status ? ["?", "var(--soft)"] : ["○", "var(--soft)"]; }

function drawNode(g, e, ox, oy) {
  const n = byId[e.id], x = ox + e.x, y = oy + e.y;
  const fold = () => { folded.has(n.id) ? folded.delete(n.id) : folded.add(n.id); render(); };
  if (n.kind === "group" && !folded.has(n.id)) {
    const spec = n.spec;
    el("rect", { x, y, width: e.width, height: e.height, rx: spec ? 12 : 8, fill: "var(--fill)",
                 stroke: spec ? "var(--teal)" : "var(--line)", "stroke-width": spec ? 1.6 : 1 }, g);
    const name = spec ? spec.title : n.title;
    const t = text(g, x + PAD, y + 20, "h", name); t.onclick = fold;
    text(g, x + PAD + textWidth(name, FONT_H) + 8, y + 20, "s", "= " + n.note);
    if (spec) {
      const [m, col] = mark(spec.status);
      const b = text(g, x + e.width - PAD, y + 20, "s", "the decomposition refines it " + m);
      b.setAttribute("text-anchor", "end"); b.setAttribute("fill", col);
      const sl = specLines(n);
      el("rect", { x: x + PAD, y: y + 32, width: e.width - 2 * PAD, height: sl.length * LINE + 2 * PAD,
                   rx: 8, fill: "#fff", stroke: "var(--teal)", "stroke-width": 1 }, g);
      sl.forEach(([c, s2], i) => text(g, x + 2 * PAD, y + 32 + PAD + 12 + i * LINE, c, s2));
    } else if (n.op === "system") text(g, x + PAD, y + 36, "s", "system");
  } else {
    const hole = n.kind === "hole", comp = n.kind === "component";
    el("rect", { x, y, width: e.width, height: e.height, rx: comp ? 0 : 10,
                 fill: hole ? "var(--hole)" : "#fff", stroke: comp ? "var(--ink)" : "var(--teal)",
                 "stroke-width": comp ? 2 : 1.4, "stroke-dasharray": hole ? "6 4" : "none" }, g);
    if (comp) el("rect", { x, y, width: e.width, height: LINE + PAD, fill: "var(--fill)", stroke: "none" }, g);
    lines(n).forEach(([c, s], i) => { const t = text(g, x + PAD, y + PAD + 12 + i * LINE, c, s);
      if (c === "h") t.onclick = fold; });
    if (comp) { const [m, col] = mark(n.status);
      const b = text(g, x + e.width - PAD - 52, y + PAD + 12, "s", "M ⊨ C " + m); b.setAttribute("fill", col); }
  }
  (e.ports || []).forEach(p => {
    el("rect", { x: x + p.x, y: y + p.y, width: p.width, height: p.height, fill: "var(--teal)" }, g);
    (p.labels || []).forEach(l => text(g, x + p.x + l.x, y + p.y + l.y + 10, "w", l.text)); });
  (e.children || []).forEach(c => drawNode(g, c, x, y));
  (e.edges || []).forEach(ed => drawEdge(g, ed, x, y));
  if (n.edges && !folded.has(n.id)) n.edges.filter(w => w.open).forEach(w => {
    const c = (e.children || []).find(c => c.id === w.open[0]); if (!c) return;
    const p = (c.ports || []).find(p => p.id === w.open[0] + ":in:" + w.var); if (!p) return;
    const px = x + c.x + p.x, py = y + c.y + p.y + PORT / 2;
    el("line", { x1: px - 22, y1: py, x2: px, y2: py, stroke: "var(--bad)", "stroke-width": 2 }, g);
    const t = text(g, px - 24, py - 4, "w", "open"); t.setAttribute("text-anchor", "end"); t.setAttribute("fill", "var(--bad)"); });
}

function drawEdge(g, ed, ox, oy) {
  (ed.sections || []).forEach(s => {
    const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint].map(p => (ox + p.x) + "," + (oy + p.y));
    el("polyline", { points: pts.join(" "), fill: "none", stroke: "var(--soft)", "stroke-width": 1.3,
                     "marker-end": "url(#arrow)" }, g); });
  (ed.labels || []).forEach(l => text(g, ox + l.x, oy + l.y + 10, "w", l.text));
}

async function render() {
  const elk = new ELK();
  const root = { id: "root", children: DATA.nodes.map(toElk),
    edges: DATA.refines.map((r, i) => { const [m] = mark(r.status);
      return { id: "r" + i, sources: [r.from], targets: [r.to],
               labels: [{ text: "refines " + m, width: 70, height: 16 }] }; }),
    layoutOptions: { "elk.algorithm": "layered", "elk.direction": "UP",
      "elk.hierarchyHandling": "INCLUDE_CHILDREN", "elk.spacing.nodeNode": "40",
      "elk.layered.spacing.nodeNodeBetweenLayers": "70", "elk.edgeLabels.inline": "true" } };
  const out = await elk.layout(root);
  const view = document.getElementById("view"); view.innerHTML = "";
  const svg = el("svg", { width: out.width + 40, height: out.height + 40 }, view);
  const defs = el("defs", {}, svg);
  const m = el("marker", { id: "arrow", viewBox: "0 0 10 10", refX: 9, refY: 5, markerWidth: 7, markerHeight: 7, orient: "auto" }, defs);
  el("path", { d: "M0,0 L10,5 L0,10 z", fill: "var(--soft)" }, m);
  const g = el("g", { transform: "translate(20,20)" }, svg);
  (out.edges || []).forEach((ed, i) => {
    const r = DATA.refines[i], [mk, col] = mark(r.status);
    (ed.sections || []).forEach(s => {
      const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint].map(p => p.x + "," + p.y);
      el("polyline", { points: pts.join(" "), fill: "none", stroke: col, "stroke-width": 1.8,
                       "stroke-dasharray": "7 4", "marker-end": "url(#arrow)" }, g); });
    (ed.labels || []).forEach(l => { const t = text(g, l.x, l.y + 12, "h", "refines " + mk); t.setAttribute("fill", col); });
  });
  (out.children || []).forEach(c => drawNode(g, c, 0, 0));
}
render().catch(e => { document.getElementById("view").textContent = "Cannot draw the diagram: " + e; });
</script>
__RELOAD__
</body></html>
"""
