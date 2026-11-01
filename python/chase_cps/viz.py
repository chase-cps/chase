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
import functools
import json
import os
import re
import tempfile
import time
import warnings
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


# the ranges of the C types, drawn with their names
_C_RANGES = {(-2 ** 31, 2 ** 31 - 1): "int", (0, 2 ** 32 - 1): "unsigned",
             (-2 ** 63, 2 ** 63 - 1): "long", (-32768, 32767): "short"}


def _type(t) -> Optional[dict]:
    """How a port of a type is drawn: None for a Boolean; a label, and the
    width of a bus for a bit vector."""
    from . import words
    w = words.word_type(t)
    if w is not None:
        width, signed = w
        return {"label": ("s" if signed else "") + f"[{width - 1}:0]", "bus": width}
    if isinstance(t, tuple):
        return {"label": _C_RANGES.get(tuple(t)) or f"{t[0]}..{t[1]}"}
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


def _c_source(f) -> Optional[dict]:
    """The text of a C function and of the functions of its file it calls,
    with the line of each in the file: {"file", "parts": [{"name", "line",
    "text"}]}, or None when the file cannot be read."""
    try:                                # as written, with its clauses (the code checked has none)
        with open(f.path, encoding="utf-8", errors="replace") as file:
            text = file.read()
    except OSError:
        try:
            text = f.text()
        except OSError:
            return None
    lines = text.split("\n")

    def body(name):                     # (first line, text) of the definition of name
        head = re.compile(r"^[A-Za-z_][\w \t\*]*\b" + re.escape(name) + r"\s*\(")
        for i, line in enumerate(lines):
            if head.match(line) and not line.rstrip().endswith(";"):     # not a declaration
                depth, seen = 0, False
                for j in range(i, len(lines)):
                    depth += lines[j].count("{") - lines[j].count("}")
                    seen = seen or "{" in lines[j]
                    if seen and depth <= 0:
                        return i + 1, "\n".join(lines[i:j + 1])
        return None
    parts, todo, done = [], [f.name], set()
    while todo:
        name = todo.pop(0)
        if name in done:
            continue
        done.add(name)
        b = body(name)
        if b is None:
            continue
        parts.append({"name": name, "line": b[0], "text": b[1]})
        for called in re.findall(r"\b([A-Za-z_]\w*)\s*\(", b[1].split("{", 1)[-1]):
            if called not in done and called not in ("if", "while", "for", "switch", "return", "sizeof"):
                todo.append(called)
    return {"file": os.path.basename(f.path), "parts": parts} if parts else None


# the steps and variables of a counterexample kept in the page: the table shows the first steps, the
# waveform all of them
MAX_STEPS, MAX_VARIABLES = 200, 40


def _why(result, contract=None) -> Optional[dict]:
    """Why a check fails, for its panel: which side fails (a refinement),
    the counterexample (a few steps), the reason, and the terms of the
    contract checked against (its guarantees, or the assumptions for an
    environment) that the counterexample breaks, by index."""
    if result is None or isinstance(result, str) or _status(result) != "invalid":
        return None
    failed = getattr(result, "failed", None)
    part = getattr(result, failed, None) if failed in ("assumptions", "guarantees") else result
    out = {}
    if failed:
        out["failed"] = failed
    trace = getattr(part, "trace", None)
    if trace is not None:
        d = trace.to_dict()
        steps = d["prefix"] + d["loop"]
        names = list(dict.fromkeys(k for st in steps for k in st if not k.startswith("chase_")))
        out["trace"] = {"vars": names[:MAX_VARIABLES], "more": max(0, len(names) - MAX_VARIABLES),
                        "steps": [[st.get(k) for k in names[:MAX_VARIABLES]] for st in steps[:MAX_STEPS]],
                        "loop": len(d["prefix"]) if d["loop"] else None, "cut": len(steps) > MAX_STEPS}
    reason = getattr(part, "reason", "") or getattr(result, "reason", "")
    if reason:
        out["reason"] = reason
    if trace is not None and isinstance(contract, Contract) and failed != "assumptions":
        from .robustness import holds
        terms = contract.assumptions if out.get("side") == "assumptions" else contract.guarantees
        try:
            out["violated"] = [i for i, t in enumerate(terms) if not holds(t, trace, contract.variables)]
            steps = [_first_failure(terms[i], trace, contract.variables, len(out["trace"]["steps"]))
                     for i in out["violated"]]
            steps = [k for k in steps if k is not None]
            if steps:                    # the step the panel opens at: where a term fails first
                out["at"] = out["trace"]["at"] = min(steps)
        except Exception:                # a term it cannot read on the trace: none marked
            pass
    return out or None


def _broken_assumptions(node: dict, res, scene) -> dict:
    """When a refinement fails on the assumptions (the requirement allows an
    environment that a part does not accept): {box of a part: (its title, [(index, term)])}
    of the assumptions of the parts inside node that its trace breaks."""
    if getattr(res, "failed", None) != "assumptions":
        return {}
    trace = getattr(res.assumptions, "trace", None)
    if trace is None:
        return {}
    from .robustness import holds
    out = {}
    for ch in node.get("children", []):
        obj = scene.things.get(ch["id"])
        c = obj.contract if isinstance(obj, Component) else obj
        if not isinstance(c, Contract):
            continue
        try:
            bad = [(i, ltl.to_str(t)) for i, t in enumerate(c.assumptions)
                   if not holds(t, trace, c.variables)]
        except Exception:                # a term it cannot read on the trace
            continue
        if bad:
            out[ch["id"]] = (ch["title"], bad)
    return out


def _environment_breaks(why: dict, node: dict, target, result) -> None:
    """An environment that breaks the assumptions of what it drives: those
    assumptions (red on the box of target, listed in the panel) and the
    step where one fails first; with no such step, the whole run breaks
    them (an F p that never holds)."""
    c = target.contract if isinstance(target, Component) else target
    trace = getattr(result, "trace", None)
    if trace is None or not isinstance(c, Contract) or "trace" not in why:
        return
    from .robustness import holds
    try:
        bad = [i for i, t in enumerate(c.assumptions) if not holds(t, trace, c.variables)]
        steps = [_first_failure(c.assumptions[i], trace, c.variables, len(why["trace"]["steps"])) for i in bad]
    except Exception:                    # a term it cannot read on the trace: none marked
        return
    if not bad:
        return
    why["parts"] = {node["id"]: bad}
    why["broken"] = [f"{node['title']}: {ltl.to_str(c.assumptions[i])}" for i in bad]
    steps = [k for k in steps if k is not None]
    if steps:
        why["at"] = why["trace"]["at"] = min(steps)
    else:
        why["whole"] = True


def _name_holes(nodes: list) -> None:
    """A quotient whose hole holds a candidate says which one: "ModeLogic ⊗ ?"
    becomes "ModeLogic ⊗ ? · candidate: Pilot", so that boxes of the same requirement with
    other candidates are told apart."""
    for n in nodes:
        if n.get("op") == "quotient" and str(n.get("note", "")).endswith("?"):
            inside = [g["title"] for ch in n.get("children", []) if ch.get("kind") == "hole"
                      for g in ch.get("children", [])]
            if inside:
                n["note"] = n["note"] + " · candidate: " + ", ".join(inside)
        _name_holes(n.get("children", []))


def _first_failure(term, trace, variables, length: int) -> Optional[int]:
    """The first step of the trace at which the term is seen to fail, when it
    says one: X^n p at n, G p (and G[a,b] p, a conjunction of X^k p) at the
    first step where p fails, F[a,b] p (a disjunction of X^k p) at b; and a
    step whose X looks ahead (G (p -> X X q)) at the step that settles it,
    as a waveform viewer marks it. None for a term that fails as a whole
    (F p never holds)."""
    from .robustness import _Lasso, holds
    lasso = _Lasso(trace, variables) if not ltl.pasts(term) else None
    last = len(lasso.states) if lasso else 0
    n = 0
    while isinstance(term, ltl.Unary) and term.op == "X":
        n, term = n + 1, term.arg
    if isinstance(term, ltl.Binary) and term.op == "&":
        found = [k for k in (_first_failure(t, trace, variables, length) for t in (term.left, term.right))
                 if k is not None]
        return n + min(found) if found else None

    def settled(f, k: int) -> int:     # where f, false at k, is seen false (k when it looks past the trace)
        r = _seen(f, k, k, lasso) if lasso else None
        return r[1] if r and not r[0] and r[1] < last else k
    if isinstance(term, ltl.Unary) and term.op == "G":
        for k in range(n, max(length, n + 1)):
            if not holds(term.arg, trace, variables, at=k):
                return settled(term.arg, k)
        return None
    if isinstance(term, ltl.Unary) and term.op == "F":
        return None
    if holds(term, trace, variables, at=n):
        return None
    r = _seen(term, n, n, lasso) if lasso else None
    if r is None:                    # it needs the whole run (a G, F or U inside)
        return None if _unbounded(term) else n
    return r[1] if r[1] < last else n


def _unbounded(f) -> bool:
    if isinstance(f, ltl.Unary):
        return f.op in ("G", "F") or _unbounded(f.arg)
    if isinstance(f, ltl.Binary):
        return f.op in ltl.TEMPORAL_BINARY or _unbounded(f.left) or _unbounded(f.right)
    return False


def _seen(f, i: int, col: int, lasso) -> Optional[tuple]:
    """(the value of f at the state i, the column from which it is known) for
    a formula whose X look a few steps ahead; col counts the steps without
    going back into the loop. None when f needs the whole run."""
    if not _unbounded(f) and not _nexts(f):
        return (lasso.holds(f, i), col)
    if isinstance(f, ltl.Unary):
        if f.op == "X":
            return _seen(f.arg, lasso.next(i), col + 1, lasso)
        if f.op == "!":
            r = _seen(f.arg, i, col, lasso)
            return (not r[0], r[1]) if r else None
        return None
    if not isinstance(f, ltl.Binary) or f.op not in ("&", "|", "->", "<->"):
        return None
    a, b = _seen(f.left, i, col, lasso), _seen(f.right, i, col, lasso)
    if f.op == "->":
        a = (not a[0], a[1]) if a else None
    if f.op == "<->":
        return (a[0] == b[0], max(a[1], b[1])) if a and b else None
    want = f.op == "&"                # & is decided by a false side, | and -> by a true one
    early = [r[1] for r in (a, b) if r and r[0] != want]
    if early:
        return (not want, min(early))
    return (want, max(a[1], b[1])) if a and b else None


def _nexts(f) -> bool:
    if isinstance(f, ltl.Unary):
        return f.op == "X" or _nexts(f.arg)
    if isinstance(f, ltl.Binary):
        return _nexts(f.left) or _nexts(f.right)
    return False


class _Scene:
    def __init__(self):
        self.count = 0
        self.ids = {}           # the first box of an object
        self.objs = {}          # the object of each box (its id)
        self.things = {}        # the object of each box
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
        self.things[node["id"]] = obj
        node.setdefault("children", [])
        node.setdefault("edges", [])
        node.setdefault("layout", "row")
        return node

    @staticmethod
    def terms(c: Contract) -> dict:
        """The terms as shown: with the names of the constants, and the
        intervals of steps folded again (the terms of G[0,9] p are p, X p,
        ..., shown as G[0,9] p); "_A" and "_G" are the terms themselves (with
        the values), "_gA" and "_gG" the terms of each shown one."""
        a, g = c.terms
        (sa, ga), (sg, gg) = _fold(a), _fold(g)
        out = {"name": c.name, "A": sa, "G": sg}
        raw_a, raw_g = [ltl.to_str(t) for t in c.assumptions], [ltl.to_str(t) for t in c.guarantees]
        if sa != raw_a or sg != raw_g:     # shown otherwise: the cores are found on the terms
            out.update({"_A": raw_a, "_G": raw_g, "_gA": ga, "_gG": gg})
        if c.constants:
            out["K"] = [f"{k} = {ltl.to_str(ltl.Num(v))}" for k, v in c.constants.items()]
        return out

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
        from .cprograms import CFunction
        name = getattr(k.implementation, "name", None) or type(k.implementation).__name__
        model = {"name": name, "fsm": fsm}
        if isinstance(k.implementation, CFunction):
            model.update(source="c", code=_c_source(k.implementation))
        else:
            model["source"] = (getattr(k.implementation, "source", None) or {}).get("kind")
        # a C function with no contract: the check of the code alone
        safe = isinstance(k.implementation, CFunction) and not k.contract.assumptions \
            and not k.contract.guarantees
        node = self.box(k, types=_types(k.contract), kind="component", title=k.name,
                        contract=self.terms(k.contract),
                        model=model,
                        badge={"text": "safe" if safe else "M ⊨ C", "status": _status(k.status),
                               **({"reason": why_not} if (why_not := getattr(k, "reason", "") or
                                  (getattr(k.status, "reason", "") if _status(k.status) == "unknown" else "")) else {}),
                               **({"why": _why(k.status, k.contract)} if _why(k.status) else {})},
                        inputs=sorted(k.contract.inputs), outputs=sorted(k.contract.outputs))
        self.ids.setdefault(id(k.contract), self.ids[id(k)])
        self.drawn.setdefault(id(k.contract), k.contract)
        return node

    def missing(self, s: System) -> Optional[dict]:
        """The part of s not written: when its components leave an output of
        its contract, or an input of a component that the contract does not
        give, driven by none (the logic of a module besides its instances),
        the quotient of the contract by the components, as from_spec draws
        it: a dashed hole, in closed form (no check). When the algebra fails
        (two parts drive a variable, a variable with two types), the hole
        has the ports of the quotient and no terms."""
        if s.contract is None or not s.components:
            return None
        driven = set().union(*(k.contract.outputs for k in s.components))
        read = set().union(*(k.contract.inputs for k in s.components))
        if not (set(s.contract.outputs) - driven) | (read - set(s.contract.inputs) - driven):
            return None
        note = f"{s.contract.name} / " + " ⊗ ".join(k.name for k in s.components)
        try:
            parts = functools.reduce(lambda a, b: a.compose(b), [k.contract for k in s.components])
            q = s.contract.divide(parts, name="?")
        except Exception as e:                # the ports only, as the quotient has them
            outputs = (set(s.contract.outputs) - driven) | (read - driven - set(s.contract.inputs))
            inputs = (set(s.contract.inputs) | driven) - outputs
            warnings.warn(f"the hole of {s.name} is drawn with its ports only: {e}", stacklevel=2)
            return self.box(object(), types=_types(*(k.contract for k in s.components), s.contract),
                            kind="hole", title="?", note=note + " (ports only)", contract=None,
                            inputs=sorted(inputs), outputs=sorted(outputs))
        return self.box(q, types=_types(q), kind="hole", title=q.name, note=note,
                        contract=self.terms(q), inputs=sorted(q.inputs), outputs=sorted(q.outputs))

    def system(self, s: System) -> dict:
        children = [self.component(k) for k in s.components]
        hole = self.missing(s)
        if hole is not None:
            children.append(hole)
        if s.contract is not None:
            inputs = s.contract.inputs
        else:
            driven = {v for k in s.components for v in k.contract.outputs}
            inputs = {v for k in s.components for v in k.contract.inputs if v not in driven}
        outputs = sorted(s.contract.outputs) if s.contract else _outside(children)
        edges = _wires(children, inputs) + _out(children, outputs)
        # what the parts do not agree on: its wires red, and said
        clash = _clashes(s, children)
        _mark_clash(edges, clash)
        for said in clash.values():
            warnings.warn(f"{s.name}: {said}", stacklevel=3)
        return self.box(s, types=_types(*(k.contract for k in s.components),
                                         *([s.contract] if s.contract else [])),
                        kind="group", op="system", title=s.name,
                        note=" ⊗ ".join([k.name for k in s.components] + (["?"] if hole else [])), contract=None,
                        inputs=sorted(inputs), outputs=outputs, children=children, edges=edges,
                        **({"clash": clash} if clash else {}))

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
            # all the ports of the contract it refines, as for the inputs: the wires of the box
            # around name them, and an output that no part drives stays open
            outputs = sorted(high.outputs)
            # and its inputs that no part drives: an input of a part that the contract
            # does not give, and no part drives, is open (not a port of the box)
            edges = []
            for w in inner["edges"]:
                if w.get("to", [0])[0] is None:
                    continue
                if w.get("from", [0])[0] is None and w["var"] not in high.inputs:
                    edges.append({"open": w["to"], "var": w["var"]})
                    continue
                edges.append(w)
            inner.update(kind="group", title=high.name, contract=self.terms(high),
                         badge={"text": "⪯", "status": status},
                         inputs=sorted(set(high.inputs) - {v for ch in inner["children"]
                                                           for v in ch.get("outputs", [])}),
                         outputs=outputs, edges=_mark_clash(edges + _out(inner["children"], outputs), inner.get("clash", {})))
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


def _clashes(s: System, children: list) -> dict:
    """The signals the parts of a system do not agree on, as a model would not
    compile: one driven by two parts, one of two types (in two parts, or in a
    part and the contract of the system). What to say of each."""
    out, drive, types = {}, {}, {}
    for ch in children:
        for v in ch.get("outputs", []):
            drive.setdefault(v, []).append(ch["title"])
    for v, who in drive.items():
        if len(who) > 1:
            out[v] = f"{v} is driven by {' and by '.join(who)}"
    named = [(k.name, k.contract) for k in s.components] + ([(s.contract.name, s.contract)] if s.contract else [])
    for who, c in named:
        for v, t in c.variables.items():
            types.setdefault(v, {}).setdefault(_type_text(t), []).append(who)
    for v, by in types.items():
        if len(by) > 1:
            said = "; ".join(f"{t} in {', '.join(who)}" for t, who in by.items())
            out[v] = (out[v] + "; " if v in out else "") + f"{v} has two types: {said}"
    return out


def _mark_clash(edges: list, clash: dict) -> list:
    """The wires of the signals in clash, with what to say of them."""
    for w in edges:
        if w["var"] in clash:
            w["error"] = clash[w["var"]]
    return edges


def _type_text(t) -> str:
    if isinstance(t, tuple):
        return f"{t[0]}..{t[1]}"
    return str(t)


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
         "compatible": "A", "consistent": "G", "feasible": "A∧G"}


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
        if isinstance(c, tuple):        # kept in a results file: the texts of its terms
            return c
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


def _unX(node):
    """(phi, k) for X^k phi."""
    k = 0
    while isinstance(node, ltl.Unary) and node.op == "X":
        node, k = node.arg, k + 1
    return node, k


def _interval(op: str, a: int, b: int, phi) -> str:
    return f"{op}[{a},{b}] " + ltl.to_str(ltl.Unary(op, phi))[len(op):].lstrip()


def _items(n, op):
    if isinstance(n, ltl.Binary) and n.op == op:
        return _items(n.left, op) + _items(n.right, op)
    return [n]


def _chain(node, op):
    """(a, b, phi) when node is X^a phi op X^(a+1) phi op ... op X^b phi
    (op & for G[a,b], | for F[a,b]), else None. phi may itself be made of
    op: its terms come first, as the unrolling writes them."""
    items = _items(node, op)
    if len(items) < 2:
        return None
    first = next((i for i, t in enumerate(items) if isinstance(t, ltl.Unary) and t.op == "X"), None)
    if first is None:
        return None
    phi, k = _unX(items[first])
    a = k - 1 if first else k
    if first:                          # phi at step 0: its own terms before X phi
        if items[:first] != _items(phi, op) or k != 1:
            return None
    rest = items[first:]
    if [_unX(t) for t in rest] != [(phi, k + i) for i in range(len(rest))]:
        return None
    return a, k + len(rest) - 1, phi


def _show(node) -> str:
    """The formula, with the steps of an interval folded again, also inside
    (F[0,10] G[0,5] p, not F[0,10] (p & X p & ...))."""
    if not _has_chain(node):
        return ltl.to_str(node)
    for op, name in (("|", "F"), ("&", "G")):
        c = _chain(node, op)
        if c:
            a, b, phi = c
            inner = _show(phi)
            bare = _atomic(phi) or _chain(phi, "&") or _chain(phi, "|")     # F[0,10] G[0,5] p
            return f"{name}[{a},{b}] " + (inner if bare else f"({inner})")
    if isinstance(node, ltl.Binary):
        side = lambda n: _show(n) if _atomic(n) else f"({_show(n)})"
        return f"{side(node.left)} {node.op} {side(node.right)}"
    if isinstance(node, ltl.Unary):
        return f"{node.op} ({_show(node.arg)})"
    return ltl.to_str(node)


def _atomic(n) -> bool:
    return not isinstance(n, ltl.Binary) or n.op in ("=", "!=", "<", "<=", ">", ">=", "+", "-", "*", "/")


def _has_chain(node) -> bool:
    if isinstance(node, ltl.Binary):
        if node.op in ("&", "|") and _chain(node, node.op):
            return True
        return _has_chain(node.left) or _has_chain(node.right)
    if isinstance(node, ltl.Unary):
        return _has_chain(node.arg)
    return False


def _fold(nodes: list) -> tuple:
    """(the terms as shown, the indices of the terms of each): consecutive
    terms X^a phi, X^(a+1) phi, ..., X^b phi are shown as G[a,b] phi."""
    def conjuncts(n):
        if isinstance(n, ltl.Binary) and n.op == "&":
            return conjuncts(n.left) + conjuncts(n.right)
        return [n]

    shown, groups, i = [], [], 0
    while i < len(nodes):
        phi, k = _unX(nodes[i])
        first = i          # the conjuncts of X^k phi may be terms of their own
        for m in range(2, len(nodes) - i):
            nxt, kn = _unX(nodes[i + m])
            if kn == k + 1 and conjuncts(nxt) == [_unX(t)[0] for t in nodes[i:i + m]] \
                    and all(_unX(t)[1] == k for t in nodes[i:i + m]):
                phi, i = nxt, i + m - 1
                break
        j = i
        while j + 1 < len(nodes) and _unX(nodes[j + 1]) == (phi, k + j + 1 - i):
            j += 1
        if j > i:
            shown.append(_interval("G", k, k + j - i, phi))
        else:
            j = i = first
            shown.append(_show(nodes[i]))
        groups.append(list(range(first, j + 1)))
        i = j + 1
    return shown, groups


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
        mine = {p: terms.get("_" + p, terms[p]) for p in "AG"}     # the terms themselves
        hit = {p: [i for i, t in enumerate(mine[p]) if t in want[p]] for p in "AG"}
        found = {mine[p][i] for p in "AG" for i in hit[p]}
        if found and found >= want["A"] | want["G"]:
            groups = {p: terms.get("_g" + p) or [[i] for i in range(len(mine[p]))] for p in "AG"}
            shown = {p: [j for j, grp in enumerate(groups[p]) if set(grp) & set(hit[p])]
                     for p in "AG"}
            return dict(shown, part=key)
    return None


def diagram(*items, refines: Iterable = (), checks: Iterable = (), environments: Iterable = (),
            context=None, open: Iterable = (), title: Optional[str] = None,
            results: bool = True, connect: Iterable = (), repairs: Iterable = (),
            why: Iterable = (), period=None) -> dict:
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
    it. The items in ``connect`` are wired side by side: an output of one to
    the inputs of the others with the same name. ``repairs`` lists (item,
    tag, about): what the page needs to repair the check of the tag on the
    box of the item (see ``from_spec``). ``why`` lists (item, tag, result):
    the results of the checks drawn on the tags of the boxes, whose
    counterexamples the panel of a tag shows (refinements and environments
    given here with their results need none)."""
    scene = _Scene()
    why = list(why)
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
            wrapped.update(title=item.name, note=f"Contract {item.contract.name} · {node['note']}")
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

    def shows(node, res, high):           # the counterexample of this refinement, on the box of its tag
        if res is not None and _why(res) and node.get("badge"):
            node["badge"]["why"] = _why(res, high)
            parts = _broken_assumptions(node, res, scene)
            if parts:                     # failed on the assumptions: those of the parts it breaks
                node["badge"]["why"]["parts"] = {k: [i for i, _ in v] for k, (_, v) in parts.items()}
                node["badge"]["why"]["broken"] = [f"{title}: {t}" for title, v in parts.values() for _, t in v]
        return node
    for r in ordered:
        low, high = r[0], r[1]
        status = _status(r[2]) if len(r) > 2 else None
        res = r[2] if len(r) > 2 else None
        if status is None and results and isinstance(low, Contract) and isinstance(high, Contract):
            kept = [res for kind, other, res in low.checked(context) if kind == "refines" and other is high]
            status = _status(kept[-1]) if kept else None
            res = kept[-1] if kept else None
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
            shows(hole, res, high)
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
            shows(drawn, res, high)
            continue
        if id(low) in refined:              # it refines another contract too: drawn again, apart
            after(top[id(low)], shows(scene.wrap(scene.node(low), high, status), res, high))
            continue
        inner = top.pop(id(low), None) or next(   # in a chain, the box drawn already
            (n for n in nodes if n["id"] == scene.ids.get(id(low))), None) or scene.node(low)
        if _holds(inner, id(high), scene.objs):  # a loop (each refines the other): drawn apart
            top.setdefault(id(low), inner)
            after(inner, shows(scene.wrap(scene.node(low), high, status), res, high))
            continue
        wrapped = shows(scene.wrap(inner, high, status), res, high)
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

    wires, breaks = [], []
    for e in environments:
        env, target = e[0], e[1]
        status = _status(e[2]) if len(e) > 2 else None
        node = box_of(target)
        if len(e) > 2 and _why(e[2]):
            why.append((env, "E ⊨ A", e[2], None))
            breaks.append((env, node, target, e[2]))
        env_node = scene.environment(env, node, status)
        outer = next(n for n in nodes if n is node or _find([n], node["id"]) is not None)
        nodes.insert(nodes.index(outer), env_node)
        if outer is node:                   # wires between boxes at the top only
            wires += [{"from": [env_node["id"], "out", v], "to": [node["id"], "in", v], "var": v}
                      for v in node.get("inputs", [])]
        else:
            env_node["badge"]["text"] = f"E ⊨ A of {node['title']}"
    tops = [box_of(c) for c in connect]  # boxes side by side: an output to the inputs of the same name
    tops = [n for n in tops if n in nodes]
    for a in tops:
        for b in tops:
            if a is not b:
                wires += [{"from": [a["id"], "out", v], "to": [b["id"], "in", v], "var": v}
                          for v in a.get("outputs", []) if v in b.get("inputs", [])]
    for obj in open:                    # its first box, and its other boxes but the parts of a composition
        first = box_of(obj)
        boxes = [first] + [n for n in _boxes(nodes) if scene.objs.get(n["id"]) == id(obj) and n is not first
                           and (_parent(nodes, n["id"]) or {}).get("kind") != "group"]
        holes = [n for n in _boxes(nodes) if n["kind"] == "hole" and scene.objs.get(n["id"]) == id(obj)]
        for n in holes or boxes:        # a quotient: its holes, not the box of the dividend
            n["open"] = True
    for obj, name, result, *against in why:     # the counterexample, on the tag of the check
        # the boxes of obj (it may be drawn more than once): the one whose tag shows this check
        first = _find(nodes, scene.ids[id(obj)]) if id(obj) in scene.ids else None
        boxes = ([first] if first is not None else []) + [n for n in _boxes(nodes) if n is not first
                                                          and scene.objs.get(n["id"]) == id(obj)]
        for n in boxes:
            tag = n.get("badge", {}).get("text", "")
            if tag.startswith(name) or (name, tag) == ("M ⊨ C", "safe"):
                n["badge"]["why"] = _why(result, against[0] if against else None)
                break
    for env, node, target, result in breaks:     # the assumptions an environment breaks
        for n in _boxes(nodes):
            w = n.get("badge", {}).get("why")
            if w and scene.objs.get(n["id"]) == id(env) and n["badge"].get("text", "").startswith("E ⊨ A"):
                _environment_breaks(w, node, target, result)
                break
    for obj, name, about in repairs:     # on the box that shows the result of the check
        n = _find(nodes, scene.ids[id(obj)]) if id(obj) in scene.ids else None
        if n is not None:
            n.setdefault("repair", {})[name] = about
    if context is not None:
        from .logic import _formula
        context = [context] if isinstance(context, (str, ltl.Node)) else list(context)
        context = [ltl.to_str(_formula(f)) for f in context]
    _name_holes(nodes)
    out = {"title": title or "CHASE", "nodes": nodes, "edges": wires, "context": context or []}
    if period is not None:            # the time between two steps: the steps drawn as times
        if ltl.number(period) <= 0:
            raise ValueError(f"the period must be positive, not {period}")
        out["period"] = ltl.number_text(ltl.number(period))
    return out


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


def _structure(spec: dict) -> tuple[list, list, list]:
    """How the contracts of a file go together, from their ports alone, as
    one draws them without being told: ([(parts, system)], [names],
    [(parts, system)] with a part missing). The contracts are in the order
    of the file.

    Contracts with the same inputs and outputs are versions of one part
    (fixes, or candidates): the first one in the file stands for them. A
    contract is the contract of a composition of parts when the parts drive
    its outputs (and maybe signals between them), and read from outside
    only its inputs: the parts are then drawn inside it ("A+B", "C"). The
    names are those drawn side by side, wired by name: the first versions,
    and the systems. When two parts or more are found but some outputs of
    the contract are driven by none (a part not written yet), the missing
    part is the quotient of the contract by the parts."""
    ports = {n: (frozenset(c.inputs), frozenset(c.outputs)) for n, c in spec.items()}
    first = {}
    for n in spec:
        first.setdefault(ports[n], n)
    reps = [n for n in spec if first[ports[n]] == n]
    def gather(s, used, taken):       # the parts of s, as long as some of its signals lack a driver
        ins, outs = ports[s]
        parts, driven = [], set()
        while True:                   # what the system and the parts need, and nobody drives yet
            need = (outs | set().union(*(ports[p][0] for p in parts))) - driven - ins
            if not need:
                break
            p = next((p for p in reps if p != s and p not in used and p not in parts and p not in taken
                      and ports[p][1] & need and not ports[p][1] & driven), None)
            if p is None:
                break
            parts.append(p)
            driven |= ports[p][1]
        return parts, driven, need

    systems, missing, used = [], [], set()
    for s in reps:                    # first the systems whose parts are all written
        if s in used:
            continue
        parts, _, need = gather(s, used, {x for x, _ in systems})
        if len(parts) >= 2 and not need:
            systems.append((s, parts))
            used |= set(parts) | {s}
    for s in reps:                    # then those with a part missing
        if s in used:
            continue
        ins, outs = ports[s]
        parts, driven, need = gather(s, used, {x for x, _ in systems})
        read = set().union(*(ports[p][0] for p in parts)) if parts else set()
        if len(parts) >= 2 and need and read <= ins | outs | driven:
            missing.append((s, parts))
            used |= set(parts) | {s}
    inside = {p for _, parts in systems + missing for p in parts}
    return ([("+".join(parts), s) for s, parts in systems], [n for n in reps if n not in inside],
            [("+".join(parts), s) for s, parts in missing])


def _in_file_order(spec: dict, path: str) -> dict:
    """The contracts of spec in the order the file declares them."""
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            text = f.read()
    except OSError:
        return spec
    at = {}
    for m in re.finditer(r"^\s*(?:CONTRACT|module)\s+([A-Za-z_]\w*)", text, re.M):
        at.setdefault(m.group(1), m.start())
    return dict(sorted(spec.items(), key=lambda kv: at.get(kv[0], len(text))))


def from_spec(path: str, contracts: Iterable = (), refines: Iterable = (),
              environments: Iterable = (), check: bool = False, verify: bool = False,
              realizable: bool = False, timeout: float = 60, title: Optional[str] = None,
              open: Iterable = (), implementations: Iterable = (), results: Optional[str] = None,
              budget: Optional[float] = None, progress=None, period=None) -> dict:
    """The diagram of a specification file (the logics language of CHASE, or
    JSON). ``contracts`` are the contracts to draw (default: all, but those
    drawn inside another box by ``refines``), each a
    name or names joined by + for their composition; ``refines`` lists
    (refined, abstract) names, drawn one inside the other; ``environments``
    lists (model file, contract): an SMV or JSON state machine, or a
    controller of slugs. ``implementations`` lists (model file, contract):
    the contract is drawn as a component with its model (SMV, JSON, or RTL
    in .v or .sv, read with Yosys). ``check`` checks that the assumptions
    and the guarantees of each contract drawn can hold together (A∧G, with
    the core when they cannot), and the refinements, the environments, and
    the implementations; ``verify`` (A, G) and ``realizable`` (R) check each
    contract drawn too. ``results`` is the file where the results of
    ``check`` are kept (see ``chase_cps.record``): a check whose contracts
    and models have not changed is not run again. ``period`` is the time
    between two steps (the sampling period): the steps of a counterexample
    are drawn as times. With ``budget`` (seconds),
    no check starts after that time; those left are not marked. ``progress``
    is called with each result as soon as it is known ({"check", "result",
    and what it is about}), for an editor that shows them one by one. ``open`` names the contracts drawn open. Each box of a
    contract of the file says where it is written (its line), for an editor.
    A Verilog or SystemVerilog file with no assertion is drawn as its module
    with its state machine."""
    if path.endswith((".v", ".sv")) and not _has_assertions(path):
        return _rtl_diagram(path, check=check, timeout=timeout, title=title)
    from . import checks as _checks_
    from . import gr1
    from .cli import _contract, _model, _read
    spec = _read(path, period)
    if path.endswith(".c") and not implementations:   # each function with its code
        implementations = [(f"{path}:{name}", name) for name in spec]
    if path.endswith(".smv") and not implementations:  # the module with its specifications
        implementations = [(path, name) for name in spec]
    made = {}

    def get(expression):              # one box per expression
        if expression not in made:
            made[expression] = _contract(expression, spec)
        return made[expression]
    def written(c, out):              # the contracts of the file in c, in order
        if c.origin:
            for o in c.origin[1]:
                written(o, out)
        elif all(x is not c for x in out):
            out.append(c)
        return out
    if isinstance(spec, dict):
        spec = _in_file_order(spec, path)
    found, connect, missing = _structure(spec) if isinstance(spec, dict) else ([], [], [])
    holes = {}                            # system -> the system with the missing part as a hole
    if not refines and not contracts:     # nothing said: as the ports of the contracts say
        refines = found
        for parts, system in missing:     # closed form: no check, a few milliseconds
            holes[system] = get(system).divide(get(parts), name="?")
    from . import record as _record
    # the results kept: shown when what they read has not changed, also with no check run
    rec = _record.Record(results, path) if results and (check or os.path.exists(results)) else None
    deadline = time.monotonic() + budget if budget is not None else None

    def kept(kind, k, run, **about):    # a result kept in the file, or the check run and kept
        e = rec.get(k) if rec is not None else None
        if e is not None:
            r = _record.as_result(e)
        elif not check or (deadline is not None and time.monotonic() > deadline):
            return None
        else:
            r = run()
            if rec is not None:
                rec.put(k, kind, r, **about)
        if progress is not None:
            progress(dict(check=kind, result=r.status, **about))
        return r

    def feasible(c):                  # A ∧ G, and the core when they cannot hold together
        from .logic import is_satisfiable, unsat_core
        r = is_satisfiable(ltl.Binary("&", c.assumption, c.guarantee), c.variables, timeout=timeout)
        if r.status == "unsat":
            r.core = unsat_core(c, timeout=timeout)
        return r
    pairs = [(get(a), get(b), None) for a, b in refines]
    if contracts:
        items = [get(e) for e in contracts]
    else:                             # all, but those drawn inside another box
        inside = [x for r, _, _ in pairs for x in written(r, [])]
        inside += [x for parts, _ in missing if holes for x in written(get(parts), [])]
        items = [holes.get(e) or get(e) for e in spec if all(get(e) is not x for x in inside)]
    atoms = []
    for c in items + [x for a, b, _ in pairs for x in (a, b)]:
        written(c, atoms)
    marks = []
    kept_or_checked = check or rec is not None
    for c in atoms if kept_or_checked and not verify else ():     # verify checks A ∧ G too
        r = kept("feasible", _record.key("feasible", c), lambda c=c: feasible(c), of=c.name)
        if r is not None:
            marks.append((c, "feasible", r))
    status = lambda r: r.status if r is not None else None
    pairs = [(a, b, kept("refines", _record.key("refines", a, b),
                         lambda a=a, b=b: _checks_.refines(a, b, timeout=timeout),
                         refined=x, abstract=y) if kept_or_checked else None)
             for (a, b, _), (x, y) in zip(pairs, refines)]
    for c in atoms:                   # a composition is not GR(1): its parts are checked
        if verify:
            marks.append((c, _checks_.verify(c, core=True, timeout=timeout)))
        if realizable:
            marks.append((c, "realizable", gr1.is_realizable(c, core=True, timeout=timeout)))
    envs = []
    for model, name in environments:
        c = get(name)
        m = _model(model, c)
        envs.append((m, c, kept("environment", _record.key("environment", _record.model_bytes(model), c),
                                lambda m=m, c=c: _checks_.is_environment(m, c, timeout=timeout),
                                model=_near(model, path), contract=name) if kept_or_checked else None))
    shown, impls, why = list(items), {}, []
    for model, name in implementations:   # a contract drawn with its model, as a component
        c = get(name)
        m = _model(model, c)
        k = impls[id(c)] = Component(c.name, c, m)
        if kept_or_checked:
            r = kept("implementation", _record.key("implementation", _record.model_bytes(model), c),
                     lambda m=m, c=c: _checks_.is_implementation(m, c, timeout=timeout),
                     model=_near(model, path), contract=name)
            k.status = status(r)
            k.reason = getattr(r, "reason", "") if status(r) == "unknown" else ""
            if _why(r):
                why.append((k, "M ⊨ C", r, c))
    # a part with a model, in a decomposition: the decomposition is drawn as
    # its components, each with its model when it has one
    inner = set()
    for i, (a, b, st) in enumerate(pairs):
        parts = written(a, [])
        if a.origin and a.origin[0] == "composition" and any(id(x) in impls for x in parts):
            pairs[i] = (System(a.name, [impls.get(id(x)) or Component(x.name, x) for x in parts]), b, st)
            inner |= {id(x) for x in parts}
    for model, name in implementations:
        c = get(name)
        if id(c) not in inner:
            k = impls[id(c)]
            shown = [k if x is c else x for x in shown] if any(x is c for x in shown) else shown + [k]
    drawn = {id(x) for x in items}
    repairs = _repairs(path, refines, pairs, implementations, environments, envs, impls, atoms,
                       written, get)
    d = diagram(*shown, refines=pairs, checks=marks, environments=envs,
                open=[get(e) for e in open], title=title or os.path.basename(path),
                connect=[get(n) for n in connect if id(get(n)) in drawn], repairs=repairs, why=why,
                period=period)
    _place_in_source(d, path)
    if rec is not None and check:     # the whole file checked: the results of old versions go
        rec.save(prune=not contracts and (deadline is None or time.monotonic() <= deadline))
    return d


_CONSTANT = re.compile(r"^\s*(integer|real)\s+constant\s+([A-Za-z_]\w*)\s+([^;\s]+)\s*;")


def _near(model: str, path: str) -> str:
    """A model as written next to the file: relative to its directory."""
    try:
        return os.path.relpath(model, os.path.dirname(os.path.abspath(path)))
    except ValueError:
        return model


def _constant_lines(path: str) -> dict:
    """{(contract, constant): (line, "int" or "real")} of the declarations
    of the named constants in a file of the logics language (lines from 1)."""
    out, name = {}, None
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            text = f.read()
    except OSError:
        return out
    for number, raw in enumerate(text.splitlines(), 1):
        line = re.sub(r"(//|#).*", "", raw)
        m = re.match(r"\s*CONTRACT\s+([A-Za-z_]\w*)", line)
        if m:
            name = m.group(1)
            continue
        m = _CONSTANT.match(line)
        if m and name:
            out[(name, m.group(2))] = (number, "int" if m.group(1) == "integer" else "real")
    return out


def _repairs(path, refines, pairs, implementations, environments, envs, impls, atoms,
             written, get) -> list:
    """(item, tag, about) for each check drawn whose contracts have named
    constants: the check as chase-cps repair takes it (its arguments as
    written in the file or on the command line), and the constants, each
    with its value, its type, and where it is declared, in which contract,
    as a part of what is checked or as its requirement."""
    lines = _constant_lines(path)

    def constants(roles):              # [(contract of the file, role)] -> [constant]
        found = {}
        for c, role in roles:
            for k, v in c.constants.items():
                line, kind = lines.get((c.name, k), (None, "int" if isinstance(v, int) else "real"))
                e = found.setdefault(k, {"name": k, "value": _number_text(v), "type": kind,
                                         "owners": []})
                e["owners"].append({"contract": c.name, "role": role, "line": line})
        for e in found.values():       # the requirement's when it declares it too (r of DroneAltitude)
            roles_of = {o["role"] for o in e["owners"]}
            e["role"] = "requirement" if "requirement" in roles_of else roles_of.pop()
        return list(found.values())

    out = []

    def add(obj, tag, check, args, roles):
        cs = constants(roles)
        if cs:
            out.append((obj, tag, {"check": check, "args": list(args), "constants": cs}))
    for (a, b, _), (x, y) in zip(pairs, refines):
        a = get(x)                      # the contracts, also when drawn as components
        add(b, "⪯", "refines", (x, y), [(c, "part") for c in written(a, [])]
            + [(c, "requirement") for c in written(b, [])])
    for model, name in implementations:
        c = get(name)
        add(impls[id(c)], "M ⊨ C", "impl", (model, name), [(x, "contract") for x in written(c, [])])
    for (model, name), (m, c, _) in zip(environments, envs):
        add(m, "E ⊨ A", "env", (model, name), [(x, "contract") for x in written(c, [])])
    for c in atoms:
        add(c, "A∧G", "verify", (c.name,), [(c, "contract")])
    return out


def _number_text(v) -> str:
    return ltl.to_str(ltl.Num(v)) if ltl._decimal(v) else str(v)    # 1/3: no finite decimal


def _has_assertions(path: str) -> bool:
    with open(path, encoding="utf-8", errors="replace") as f:
        text = re.sub(r"//[^\n]*|/\*.*?\*/", "", f.read(), flags=re.S)
    return re.search(r"\b(assert|assume)\s+property\b", text) is not None


def _rtl_diagram(path: str, check: bool = False, timeout: float = 60,
                 title: Optional[str] = None) -> dict:
    """The diagram of the RTL of a file: its module, with its ports and its
    state machine (drawn open when it is small)."""
    from .cli import _rtl
    m = _rtl(path)
    outputs = (m.source or {}).get("outputs", [])
    c = Contract(m.name, {v: m.variables[v] for v in sorted(m.ivars)},
                 {v: m.variables[v] for v in outputs if v in m.variables}, [], [])
    k = Component(m.name, c, m)
    d = diagram(k, open=[k], title=title or os.path.basename(path))
    _place_in_source(d, path)
    return d


def _term_lines(text: str) -> dict:
    """{contract: {"A": [[first, last], ...], "G": [...]}}: the lines of each
    term of the contracts of a file of the logics language. A statement (up
    to its ";") gives a term for each of its top-level conjuncts, and none
    when it is true, as the contract has them."""
    out, name, part, start, body = {}, None, None, None, []
    def end(last):
        try:
            n = len([t for t in ltl.conjuncts(ltl.parse(" ".join(body))) if ltl.to_str(t) != "true"])
        except Exception:
            n = 0 if " ".join(body).strip() == "true" else 1
        out[name][part].extend([[start, last]] * n)
    for number, raw in enumerate(text.splitlines(), 1):
        line = re.sub(r"(//|#).*", "", raw)
        m = re.match(r"\s*CONTRACT\s+([A-Za-z_]\w*)", line)
        if m:
            name, part, start = m.group(1), None, None
            out[name] = {"A": [], "G": []}
            continue
        m = re.match(r"\s*(Assumptions|Guarantees)\s*:", line)
        if m and name:
            part, start = m.group(1)[0], None
            line = line[m.end():]
        if not part:
            continue
        for piece in re.split(r"(;)", line):
            if piece == ";":
                if start is not None:
                    end(number)
                start = None
            elif piece.strip():
                if start is None:
                    start, body = number, []
                body.append(piece)
    return out


def _sva_term_lines(text: str) -> dict:
    """{module: {"A": [[first, last], ...], "G": [...]}}: the lines of each
    assume and assert property of a SystemVerilog file, in order."""
    text = re.sub(r"//[^\n]*|/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)
    line_of = lambda i: text.count("\n", 0, i) + 1
    out, name = {}, None
    for m in re.finditer(r"\bmodule\s+(?:automatic\s+)?([A-Za-z_]\w*)|\bendmodule\b|"
                         r"\b(assume|assert)\s+property\b", text):
        if m.group(1):
            name = m.group(1)
            out[name] = {"A": [], "G": []}
        elif m.group(0) == "endmodule":
            name = None
        elif name:
            depth, i = 0, m.end()
            while i < len(text) and not (text[i] == ";" and depth == 0):
                depth += {"(": 1, ")": -1}.get(text[i], 0)
                i += 1
            start = text.rfind("\n", 0, m.start()) + 1          # a label or initial before it
            first = line_of(start + len(text[start:m.start()]) - len(text[start:m.start()].lstrip()))
            out[name]["A" if m.group(2) == "assume" else "G"].append([first, line_of(i)])
    return out


def _place_in_source(d: dict, path: str) -> None:
    """Writes in each box of a contract (or a module) of the file the line
    where it starts, and the lines of its terms, for an editor to go from the
    box to the text."""
    lines = {}
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            text = f.read()
    except OSError:
        return
    for number, line in enumerate(text.splitlines(), 1):
        m = re.match(r"\s*(?:CONTRACT|module)\s+([A-Za-z_]\w*)", line)
        if m and m.group(1) not in lines:
            lines[m.group(1)] = number
    terms = (_sva_term_lines(text) if path.endswith((".v", ".sv", ".sva"))
             else {} if path.endswith(".json") else _term_lines(text))
    if path.endswith((".c", ".smv")):     # the functions and their clauses, the module and its specifications
        from .cspec import places
        from .smvspec import places as smv_places
        try:
            lines, terms = (smv_places if path.endswith(".smv") else places)(path)
        except ValueError:
            return

    def walk(nodes):
        for n in nodes:
            c = n.get("contract") or {}
            name = c.get("name") or n.get("title")
            if name in lines and n.get("kind") in ("contract", "component", "group"):
                n["source"] = {"line": lines[name]}
                for p in "AG":   # the terms of the file, when they are the terms of the box
                    groups = c.get("_g" + p) or [[i] for i in range(len(c.get(p) or []))]
                    count = sum(len(grp) for grp in groups)
                    if name in terms and c.get(p) is not None and count == len(terms[name][p]):
                        at = terms[name][p]
                        n["source"][p] = [[at[grp[0]][0], at[grp[-1]][1]] for grp in groups]
            walk(n.get("children", []))
    walk(d["nodes"])


def notebook(item, **kwargs) -> str:
    """The page in an iframe, for Jupyter."""
    page = _html.escape(html(item, **kwargs), quote=True)
    return f'<iframe srcdoc="{page}" style="width:100%;height:520px;border:0"></iframe>'


def repair_request(path: str, request: dict, timeout: float = 300) -> dict:
    """The repair a page asks for, on the specification at path: request is
    {"check", "args" (as chase-cps repair takes them), "params": [{"name",
    "low", "high", "type": "int" or "real"}]}. The result of
    ``Repair.to_dict``."""
    from .cli import _contract, _model, _read
    from .repair import repair
    spec = _read(path, request.get("period"))
    check, args = request["check"], list(request["args"])
    if check in ("impl", "env"):
        c = _contract(args[1], spec)
        arguments = (_model(args[0], c), c)
    elif check == "refines":
        arguments = tuple(_contract(a, spec) for a in args)
    else:
        arguments = (_contract(args[0], spec),)
    params = {}
    for q in request["params"]:
        low, high = ltl.number(str(q["low"])), ltl.number(str(q["high"]))
        params[q["name"]] = (low, high, "int") if q.get("type") == "int" else (low, high)
    return repair(check, *arguments, parameters=params,
                  timeout=float(request.get("timeout", timeout)),
                  time_limit=float(request["time_limit"]) if request.get("time_limit") else None).to_dict()


def _repair_in_process(path, request, out):
    try:
        out.put({"result": repair_request(path, request)})
    except Exception as e:               # the reason goes to the page
        out.put({"error": str(e) if isinstance(e, (ValueError, RuntimeError, KeyError))
                 else f"{type(e).__name__}: {e}"})


def set_constants(text: str, edits) -> str:
    """The text of a specification with the values of named constants
    changed: each edit is {"line" (from 1), "name", "value"}, the line of
    the declaration of the constant."""
    lines = text.split("\n")
    for e in edits:
        i = int(e["line"]) - 1
        value = str(e["value"])
        if not re.fullmatch(r"-?\d+(\.\d+)?", value):
            raise ValueError(f"{e['name']}: {value} is not a number a file can write")
        pattern = re.compile(r"(\bconstant\s+" + re.escape(e["name"]) + r"\s+)([^;\s]+)")
        if not 0 <= i < len(lines) or not pattern.search(lines[i]):
            raise ValueError(f"line {i + 1} does not declare the constant {e['name']}")
        lines[i] = pattern.sub(lambda m: m.group(1) + value, lines[i], count=1)
    return "\n".join(lines)


def serve(make, path: str, port: int = 8765, host: str = "127.0.0.1",
          elk: Optional[str] = None) -> int:
    """Serves the diagram make() on http://host:port, and draws it again
    whenever the file at path changes: the page in the browser takes the new
    drawing in place, and keeps the view. When the file cannot be drawn, the
    page keeps the last diagram and says why. The page asks for repairs
    (POST /repair) and writes the values of constants in the file (POST
    /apply), with the header X-CHASE (a page of another site cannot send
    it). Stops with Ctrl-C."""
    import http.server
    import threading

    state = {"mtime": None, "version": 0, "data": None, "error": None, "page": None}
    jobs = {}                           # the repairs running, by the id the page gave them
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
                state["page"] = html(d, elk=elk)      # a reload shows the last drawing

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

        def do_POST(self):
            if self.headers.get("X-CHASE") != "1" or self.path not in ("/repair", "/apply", "/repair-stop"):
                self.send_response(403)
                self.end_headers()
                return
            try:
                asked = json.loads(self.rfile.read(int(self.headers.get("Content-Length") or 0)) or b"{}")
                if self.path == "/repair":     # in a process of its own: a stop ends it
                    import multiprocessing as mp
                    queue = mp.Queue()
                    job = mp.Process(target=_repair_in_process, args=(path, asked, queue), daemon=True)
                    with lock:
                        jobs[str(asked.get("id"))] = job
                    job.start()
                    job.join()
                    with lock:
                        jobs.pop(str(asked.get("id")), None)
                    reply = queue.get() if not queue.empty() else {"error": "stopped"}
                elif self.path == "/repair-stop":
                    with lock:
                        job = jobs.get(str(asked.get("id")))
                    if job is not None and job.is_alive():
                        job.kill()
                    reply = {"ok": True}
                else:
                    with open(path, encoding="utf-8") as f:
                        text = f.read()
                    text = set_constants(text, asked.get("edits", []))
                    with open(path, "w", encoding="utf-8") as f:
                        f.write(text)
                    reply = {"ok": True}
            except Exception as e:      # the reason goes to the page
                reply = {"error": str(e) if isinstance(e, (ValueError, RuntimeError, KeyError))
                         else f"{type(e).__name__}: {e}"}
            data = json.dumps(reply).encode()
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_header("Cache-Control", "no-store")
            self.end_headers()
            self.wfile.write(data)

        def log_message(self, *args):
            pass

    refresh()
    try:
        server = http.server.ThreadingHTTPServer((host, port), Handler)
    except OSError as e:              # the port is taken: say so, and how to choose another
        raise ValueError(f"cannot serve on {host}:{port} ({e.strerror}): give another port, "
                         f"--port {port + 1}") from None
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
  #top { position:fixed; top:0; left:0; right:0; z-index:10; display:flex; justify-content:space-between; align-items:flex-start;
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
  [data-box].sel > rect:first-of-type { stroke:var(--teal); stroke-width:3px; }
  .trow > .hl { display:none; } .trow.sel > .hl { display:inline; }
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
  #run, #runsep { display:none; } #run .turn { display:none; } #run.busy .idle { display:none; }
  #run.busy .turn { display:inline; transform-origin:8px 8px; animation:turn 0.9s linear infinite; }
  @keyframes turn { to { transform:rotate(360deg); } }
  /* a term picked in the panel, on the diagram */
  @keyframes flash { 0%, 50%, 100% { opacity:0; } 25%, 75% { opacity:1; } }
  rect.flash { fill:none; stroke:var(--bad); stroke-width:2; animation:flash 1.6s ease-in-out; pointer-events:none; }
  .pending circle { fill:var(--soft); animation:dot 1.2s ease-in-out infinite; }
  .pending circle:nth-of-type(2) { animation-delay:0.2s; } .pending circle:nth-of-type(3) { animation-delay:0.4s; }
  @keyframes dot { 0%, 80%, 100% { opacity:0.2; } 40% { opacity:1; } }
  #busy { display:none; position:fixed; right:14px; bottom:12px; z-index:6; padding:5px 12px;
          border-radius:12px; background:var(--teal); color:#fff; font:600 12px Helvetica, Arial, sans-serif;
          box-shadow:0 1px 4px rgba(0,0,0,.2); }
  #context span { display:inline-flex; align-items:center; gap:7px; border:1px solid #d5dddd; border-radius:6px;
                  padding:2px 9px 2px 2px; font:11.5px Menlo, Monaco, monospace; color:var(--ink);
                  white-space:nowrap; background:#fff; }
  #context i { font:600 9.5px -apple-system, Helvetica, Arial, sans-serif; font-style:normal; color:#fff;
               background:#6f7f80; border-radius:4px; padding:2px 5px; }
  #view { position:fixed; inset:0; overflow:hidden; cursor:grab; touch-action:none; user-select:none; -webkit-user-select:none; }
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
  #pop header { padding:8px 8px 8px 16px; border-bottom:1px solid #e3e8e8; display:flex; gap:20px; align-items:center; }
  #pop header b { font-size:13px; font-weight:600; flex:1; }
  #popview { overflow:auto; padding:6px 18px 16px; }
  /* the waveform: a drawer under the diagram, beside the details; the names and values on the left, the steps scrolled and zoomed */
  #wave { position:fixed; left:12px; right:12px; bottom:12px; box-sizing:border-box;
          background:#fff; border:1px solid #d5dddd; border-radius:10px; box-shadow:0 6px 28px rgba(0,0,0,.14);
          display:none; flex-direction:column; z-index:11; outline:none; overflow:hidden; }
  body.insp #wave { right:calc(12px + min(402px, 100vw - 22px) + 12px); }   /* the details: 400 pixels and their border; the bottoms on one line */
  .wv-grip { position:absolute; top:0; left:0; right:0; height:12px; cursor:ns-resize; z-index:4; }
  .wv-grip::after { content:""; position:absolute; left:50%; top:3px; width:36px; height:3px; margin-left:-18px; border-radius:2px; background:#dfe5e5; }
  .wv-grip:hover::after, .wv-grip.drag::after { background:var(--line); }
  #wave header { padding:9px 8px 7px 16px; border-bottom:1px solid #e3e8e8; display:flex; gap:14px; align-items:center; user-select:none; }
  #wave .wv-back { display:none; align-items:center; gap:8px; border:0; background:none; cursor:pointer; padding:0; margin-left:-6px;
                   color:var(--teal); font:600 12px -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; flex:none; }
  #wave .wv-back .ib { color:var(--teal); }
  #wave header b { font-size:13px; font-weight:400; flex:1; min-width:0; overflow:hidden; text-overflow:ellipsis; white-space:nowrap; }
  #wave header b b { font-weight:600; }
  .wv-where { font:12px Helvetica, Arial, sans-serif; color:var(--soft); white-space:nowrap; }
  .wv-where .wv-k { color:#2f6fb5; font-weight:700; }
  .wv-failat { font:600 12px Helvetica, Arial, sans-serif; color:#b03a2e; background:none; border:0; padding:3px 7px; margin:0 -7px;
               border-radius:5px; cursor:pointer; white-space:nowrap; }
  .wv-failat:hover { background:#fbeceb; }
  .wv-tools { display:flex; gap:2px; align-items:center; }
  .wv-keys { display:none; position:fixed; z-index:21; background:#fff; border:1px solid #d5dddd; border-radius:8px;
             box-shadow:0 6px 20px rgba(0,0,0,.12); padding:9px 14px; font-size:12px; color:var(--ink);
             grid-template-columns:auto auto; column-gap:18px; row-gap:4px; white-space:nowrap; }
  .wv-keys.on { display:grid; }
  .wv-keys-t { grid-column:1 / -1; font:600 12px -apple-system, Helvetica, Arial, sans-serif; color:var(--ink); margin-bottom:4px; }
  .wv-caps { display:flex; gap:3px; align-items:center; color:var(--faint); font-size:11px; }
  .wv-keys kbd { display:inline-block; min-width:12px; padding:0 5px; line-height:17px; text-align:center; border:1px solid #d5dddd;
                 border-bottom-width:2px; border-radius:4px; background:#f7f9f9; font:11px -apple-system, Helvetica, Arial, sans-serif; color:var(--ink); }
  .wv-scroll { flex:0 1 auto; overflow:auto; position:relative; cursor:default; scrollbar-width:none; }
  .wv-scroll::-webkit-scrollbar { display:none; }
  .wv-scroll.pan { cursor:grabbing; }
  .wv-grid { position:relative; min-width:100%; }
  .wv-hov { display:none; position:absolute; top:0; bottom:0; background:rgba(23,57,59,.05); pointer-events:none; }
  .wv-fade { display:none; position:absolute; left:1px; right:1px; height:12px; z-index:3; pointer-events:none;
             background:linear-gradient(rgba(255,255,255,0), rgba(255,255,255,.95)); box-shadow:inset 0 -1px #e3e8e8; }
  .wv-row { display:flex; align-items:stretch; border-bottom:1px solid #f1f4f4; }
  .wv-row.nb { border-bottom-color:transparent; }
  .wv-row.odd .wv-left { background:#fafbfb; }
  .wv-left { position:sticky; left:0; z-index:1; flex:none; display:flex; align-items:center; background:#fff;
             border-right:1px solid #e3e8e8; box-sizing:border-box; padding-left:12px; cursor:pointer; }
  .wv-left.sel { box-shadow:inset 3px 0 #17393b; }
  .wv-row.sel, .wv-row.sel .wv-left { background:#f0f5f4; }   /* the selected row: a faint tint across it */
  .wv-name { flex:none; font:11px Menlo, Monaco, monospace; color:var(--soft); overflow:hidden; text-overflow:ellipsis; white-space:nowrap; }
  .wv-name.port { color:var(--ink); font-weight:600; }
  .wv-val { flex:none; box-sizing:border-box; padding-right:8px; font:11px Menlo, Monaco, monospace; color:#2f6fb5;
            overflow:hidden; text-overflow:ellipsis; white-space:nowrap; }
  .wv-val small { display:block; color:var(--faint); font-size:9.5px; }
  .wv-plot { flex:none; width:20px; height:20px; margin:0 2px; padding:0; border:0; background:none; color:var(--faint); border-radius:4px;
             cursor:pointer; display:flex; align-items:center; justify-content:center; }
  .wv-plot svg { width:14px; height:14px; fill:none; stroke:currentColor; stroke-width:1.4; stroke-linejoin:round; }
  .wv-plot:hover { color:var(--teal); background:var(--fill); }
  .wv-plot.on { color:var(--teal); background:#e1eded; }
  span.wv-plot { cursor:inherit; }
  .wv-group .wv-left { cursor:default; }
  .wv-gname { font:10.5px Helvetica, Arial, sans-serif; color:var(--faint); min-width:0; white-space:nowrap; overflow:hidden; text-overflow:ellipsis; }
  .wv-gname b { font-weight:600; color:var(--ink); }
  .wv-axis { position:sticky; top:0; z-index:2; background:#fff; border-bottom:1px solid #d5dddd; }
  .wv-axis .wv-left { z-index:3; cursor:default; align-items:flex-end; padding-bottom:10px; }
  .wv-axis .wv-name { font:600 10.5px Helvetica, Arial, sans-serif; letter-spacing:.06em; text-transform:uppercase; color:var(--faint); }
  .wv-corner { font:10.5px Helvetica, Arial, sans-serif; color:var(--soft); white-space:nowrap; }
  .wv-at { color:#2f6fb5; font-weight:600; }
  .wv-svg { display:block; flex:none; }
  .wv-tick { font:10.5px Menlo, Monaco, monospace; fill:var(--soft); }
  .wv-tick.cur { fill:#2f6fb5; font-weight:700; } .wv-tick.fail { fill:#b03a2e; font-weight:700; }
  .wv-tick.ghost { fill:var(--faint); font-style:italic; }
  .wv-loop { font:600 10px Helvetica, Arial, sans-serif; fill:var(--teal); }
  .wv-fails { font:bold 10px Helvetica, Arial, sans-serif; fill:#b03a2e; }
  .wv-v { font:11px Menlo, Monaco, monospace; fill:var(--ink); pointer-events:none; paint-order:stroke; stroke:#f7f9f9; stroke-width:3px; }
  /* each in the lane of its mark: the loop on the first line, the step that fails on the second */
  .wv-edge { position:absolute; z-index:4; display:flex; flex-direction:column; align-items:flex-start; gap:2px; }
  .wv-edge.r { right:22px; align-items:flex-end; }
  .wv-edge span { height:12px; }
  .wv-edge button { border:0; background:#fff; padding:0 6px; height:12px; line-height:12px; border-radius:4px; cursor:pointer;
                    box-shadow:0 0 0 1px #e3e8e8; }
  .wv-edge button.wv-fails { color:#b03a2e; } .wv-edge button.wv-loop { color:var(--teal); }
  .wv-edge button:hover { background:var(--fill); }
  #wave .wv-bar { display:block; visibility:hidden; flex:none; margin:4px 16px 0 0; }   /* its room kept: the drawer does not jump */
  #wave .wv-bar.on { visibility:visible; }
  #wave footer { display:flex; gap:16px; padding:6px 16px 8px; font-size:11.5px; color:var(--soft); border-top:1px solid #f1f4f4; }
  #wave footer .wv-note { flex:1; } #wave footer .wv-hint { color:var(--faint); white-space:nowrap; }
  table.fsm { border-collapse:collapse; font:11.5px Menlo, Monaco, monospace; }
  table.fsm th { font:600 11.5px -apple-system, Helvetica, Arial, sans-serif; text-align:left; color:var(--soft);
                 padding:6px 16px 6px 0; border-bottom:1px solid #e3e8e8; position:sticky; top:0; background:#fff; }
  table.fsm td { padding:3px 16px 3px 0; border-bottom:1px solid #f1f4f4; vertical-align:top; }
  table.fsm td.st { color:var(--teal); font-weight:600; border-bottom:1px solid #e3e8e8; }
  table.fsm td.when { color:var(--soft); }
  /* the inspector: docked on the right of the diagram (below it in a narrow window); it does not move with the view */
  #insp { position:fixed; top:46px; right:12px; bottom:12px; width:400px; max-width:calc(100vw - 24px); display:none;
          flex-direction:column; background:#fff; border:1px solid #d5dddd; border-radius:10px;
          box-shadow:0 4px 18px rgba(0,0,0,.08); z-index:9; color:var(--ink);
          font:12px/1.45 -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; }
  body.insp #insp { display:flex; }
  body.insp #view { right:424px; }
  #insp-bar { display:flex; align-items:center; gap:6px; min-height:40px; padding:0 10px 0 14px; border-bottom:1px solid #e3e8e8; flex:none;
              font:600 12px -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; color:var(--teal); }
  #insp-bar .count { padding:0 5px; border-radius:7px; background:var(--bad); color:#fff; font-size:10.5px; }
  /* the back arrow as far from the left, the top, the bottom, and its word */
  #insp-bar button.back { display:flex; align-items:center; gap:8px; border:0; background:none; cursor:pointer; color:var(--teal);
                          padding:0; margin-left:-6px; font:inherit; }
  #insp-bar button.back .ib { color:var(--teal); }
  #insp:focus { outline:none; }
  #repair { flex:1; overflow:auto; }
  @media (max-width: 820px) {
    #insp { top:auto; left:12px; right:12px; width:auto; height:45vh; max-width:none; }
    body.insp #view { right:0; bottom:calc(45vh + 12px); }
    body.insp.wave #insp { display:none; }        /* the drawer in the place of the details; its arrow goes back */
    body.insp #wave { right:12px; }
    #wave .wv-back { display:flex; }
  }
  [data-box].pick > rect:first-of-type { stroke:#2f6fb5; stroke-width:3px; }
  .pv-val { fill:#2f6fb5 !important; font-weight:600; paint-order:stroke; stroke:#fff; stroke-width:3px; stroke-linejoin:round; }   /* read over a wire */
  .rp-step { display:flex; gap:8px; align-items:center; margin-top:6px; color:var(--soft); }
  .rp-step input { flex:1; accent-color:var(--teal); }
  ul.rp-why { margin:4px 0 0; padding-left:16px; } ul.rp-why li { margin:2px 0; }
  /* the terms of a core: a click shows one on the diagram */
  .rp-terms { display:flex; flex-direction:column; gap:3px; margin-top:6px; }
  .rp-term { display:flex; gap:6px; align-items:flex-start; text-align:left; border:0; border-radius:4px; padding:3px 5px;
             background:#fbeceb; color:var(--ink); cursor:pointer; font:inherit; }
  .rp-term:hover { background:#f6dcd9; }
  .rp-term .fs { display:flex; flex-direction:column; min-width:0; }
  .rp-term .f.cut { -webkit-line-clamp:2; }
  .rp-term .d { font:11px/1.4 Menlo, Monaco, monospace; color:inherit; white-space:nowrap; overflow:hidden; text-overflow:ellipsis; }
  .rp-term .ln { flex:none; margin-left:auto; padding-left:6px; color:#7a8a8a; font-size:10.5px; white-space:nowrap; }
  .rp-term .st { flex:none; width:13px; height:13px; margin-top:1px; border-radius:3px; background:var(--teal); color:#fff;
                 font:700 9.5px/13px Helvetica, Arial, sans-serif; text-align:center; }
  .rp-term .f { font:11px/1.4 Menlo, Monaco, monospace; min-width:0; overflow-wrap:anywhere;
                display:-webkit-box; -webkit-line-clamp:3; -webkit-box-orient:vertical; overflow:hidden; }
  .rp-head { display:grid; grid-template-columns:52px minmax(0,1fr); column-gap:6px; align-items:center;
             padding:9px 10px 8px 8px; border-bottom:1px solid #e3e8e8; position:sticky; top:0; background:#fff; z-index:1;
             min-height:54px; box-sizing:border-box; }   /* room for a title of two lines: what is under it does not move */
  .rp-head .t { flex:1; font-size:12.5px; min-width:0; overflow-wrap:anywhere; }
  .rp-head .t b, .q-item .t b { font-weight:600; }
  /* the small icons of the page: one size, one stroke, as the icons of the tools */
  .ib { display:inline-flex; align-items:center; justify-content:center; flex:none; width:24px; height:24px; padding:0;
        border:0; border-radius:5px; background:none; color:var(--soft); cursor:pointer; }
  .ib svg, .chev svg { width:16px; height:16px; fill:none; stroke:currentColor; stroke-width:1.4;
                       stroke-linecap:round; stroke-linejoin:round; }
  button.ib:hover, button.back:hover .ib { background:var(--fill); color:var(--teal); }
  button.ib.main { background:var(--teal); color:#fff; } button.ib.main:hover { background:var(--teal); color:#fff; opacity:.85; }
  button.ib.on { background:#dfe9e9; color:var(--teal); }
  button.ib.warn, button.ib.warn:hover { background:var(--warn); color:#fff; opacity:1; }
  button.ib.off, button.ib.off:hover { opacity:.4; cursor:default; background:none; color:var(--soft); }
  button.ib.main.off, button.ib.main.off:hover { background:var(--teal); color:#fff; }
  .rp-tag { flex:none; display:block; }
  .rp-chip { flex:none; font:600 10.5px 'Arial Unicode MS', 'Segoe UI', Helvetica, Arial, sans-serif; color:#fff; border-radius:4px;
             padding:1px 6px; background:var(--faint); }
  .rp-chip.valid { background:var(--ok); } .rp-chip.invalid { background:var(--bad); }
  .rp-sec { padding:8px 14px 10px; border-bottom:1px solid #f1f4f4; }
  .rp-sec:last-child { border-bottom:0; }
  .rp-says { color:var(--soft); }
  .rp-h { font:600 10.5px Helvetica, Arial, sans-serif; letter-spacing:.06em; text-transform:uppercase; color:var(--soft); margin:2px 0 6px;
          height:24px; display:flex; align-items:center; }   /* as high with the page buttons as without */
  .rp-row { display:grid; grid-template-columns:16px minmax(0,1fr) auto; gap:2px 6px; align-items:center; padding:4px 0; }
  .rp-row + .rp-row { border-top:1px dashed #edf1f1; }
  .rp-row input[type=checkbox] { margin:0; accent-color:var(--teal); }
  .rp-name { font:11.5px Menlo, Monaco, monospace; }
  .rp-who { grid-column:2 / span 2; color:var(--soft); font-size:11px; display:flex; gap:6px; align-items:center; flex-wrap:wrap; }
  .rp-role { font-size:10.5px; border:1px solid var(--line); border-radius:4px; padding:0 5px; color:var(--soft); }
  .rp-role.requirement { color:var(--warn); border-color:var(--warn); }
  .rp-range { display:flex; gap:3px; align-items:center; color:var(--soft); font:11px Menlo, Monaco, monospace; }
  .rp-range input { width:54px; font:11px Menlo, Monaco, monospace; border:1px solid #d5dddd; border-radius:4px; padding:1px 4px; color:var(--ink); }
  .rp-btn { font:600 11.5px -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; border:1px solid #d5dddd; background:#fff;
            border-radius:6px; padding:3px 10px; cursor:pointer; color:var(--teal); white-space:nowrap; }
  .rp-btn:hover { background:var(--fill); }
  .rp-btn.main { background:var(--teal); border-color:var(--teal); color:#fff; }
  .rp-btn.on { background:#e6eeee; }
  .rp-btn.warn { border-color:var(--warn); color:var(--warn); }
  .rp-btn:disabled { opacity:.45; cursor:default; background:#fff; }
  .rp-btn.main:disabled { background:var(--teal); }
  .rp-run { display:flex; gap:8px; align-items:center; flex-wrap:wrap; }
  .rp-opt { border:1px solid #e3e8e8; border-radius:8px; padding:7px 10px; margin:6px 0 0; display:grid;
            grid-template-columns:minmax(0,1fr) auto; gap:2px 8px; align-items:center; }
  .rp-opt.repaired { border-color:#b9d6c4; } .rp-opt.repaired:hover, .rp-opt.shown { border-color:var(--ok); background:#f6faf7; }
  .rp-opt.old, .rp-opt.faint { opacity:.6; } .rp-opt.faint:hover { opacity:1; }
  .rp-l { grid-column:1; font-size:11px; color:var(--soft); } .rp-l b { color:var(--ink); font-weight:600; }
  .rp-t { grid-column:1; font-size:11.5px; }
  .rp-h.rp-hb { justify-content:space-between; }
  .rp-h.rp-hb .ib { text-transform:none; letter-spacing:0; }
  #insp-bar .ib.wf-open { margin-left:auto; }
  .ib.wf-open { width:auto; padding:0 7px 0 5px; gap:4px; border:1px solid var(--line); border-radius:5px; color:var(--teal); }
  .ib.wf-open .wf-word { font:600 11px Helvetica, Arial, sans-serif; text-transform:none; letter-spacing:0; }
  .rp-v { font:11.5px Menlo, Monaco, monospace; overflow-wrap:anywhere; }
  .rp-v s { color:var(--bad); } .rp-v em { font-style:normal; color:var(--ok); font-weight:600; }
  .rp-m { grid-column:1; color:var(--soft); font-size:11px; }
  .rp-b { grid-row:1 / span 3; grid-column:2; display:flex; gap:4px; align-items:center; }
  .rp-note { color:var(--soft); font-size:11px; margin-top:6px; }
  .rp-err { color:var(--bad); font-size:11px; margin-top:6px; white-space:pre-wrap; }
  .rp-cmd { font:10.5px/1.5 Menlo, Monaco, monospace; background:var(--fill); border:1px solid #e3e8e8; border-radius:6px;
            padding:5px 7px; margin-top:6px; white-space:pre-wrap; overflow-x:auto; user-select:text; -webkit-user-select:text; }
  /* a command breaks between its words only, never inside a flag (--serve) */
  .rp-cmd > span, code.cmd { white-space:nowrap; }
  code.cmd { font:10.5px Menlo, Monaco, monospace; background:var(--fill); border-radius:3px; padding:0 3px; }
  .rp-spin { width:10px; height:10px; border:1.6px solid #d5dddd; border-top-color:var(--teal); border-radius:50%;
             animation:turn 0.8s linear infinite; }
  /* the steps in view, and a page back or on: only when the table is wider than the panel */
  .rp-trace-top { display:none; align-items:center; justify-content:flex-end; gap:2px; margin-top:2px; color:var(--soft); font-size:11px; }
  .rp-h .rp-trace-top { margin:0; }
  .rp-trace-top.on { display:flex; }
  /* a scroll bar of the page, always drawn: the one of the system shows only while scrolling (macOS) */
  .rp-tracebox .rp-tracewrap { scrollbar-width:none; } .rp-tracebox .rp-tracewrap::-webkit-scrollbar { display:none; }
  .rp-bar { display:none; position:relative; height:8px; margin:5px 0 2px; border-radius:4px; background:#edf1f1; cursor:pointer; }
  .rp-bar.on { display:block; }
  .rp-bar > div { position:absolute; top:0; bottom:0; border-radius:4px; background:#a9b6b6; cursor:grab; }
  .rp-bar > div:hover, .rp-bar > div.drag { background:#7c8e8e; } .rp-bar > div.drag { cursor:grabbing; }
  .rp-tracewrap { overflow:auto; max-height:260px; margin-top:6px;
    /* a shadow at a side where more columns are: the table scrolls */
    background: linear-gradient(to right, var(--paper, #fff) 30%, transparent) left / 24px 100% no-repeat local,
                linear-gradient(to left, var(--paper, #fff) 30%, transparent) right / 24px 100% no-repeat local,
                radial-gradient(farthest-side at 0 50%, rgba(0,0,0,.16), transparent) left / 10px 100% no-repeat scroll,
                radial-gradient(farthest-side at 100% 50%, rgba(0,0,0,.16), transparent) right / 10px 100% no-repeat scroll; }
  .rp-values { display:flex; flex-wrap:wrap; gap:2px 18px;
               font:11px Menlo, Monaco, monospace; color:var(--soft); }
  .rp-values span { white-space:nowrap; }
  .rp-h .rp-file { margin-left:8px; font:400 10.5px Menlo, Monaco, monospace; text-transform:none; letter-spacing:0; color:var(--ink); }
  .rp-io { display:grid; grid-template-columns:auto minmax(0,1fr); gap:3px 10px; align-items:baseline; }
  .rp-io .w { font:600 10.5px Helvetica, Arial, sans-serif; color:var(--soft); }
  #view.previewing rect.core-row { fill:#e6efe6; }
  .rp-values .port { color:var(--ink); font-weight:600; }
  table.rp-trace { border-collapse:collapse; font:11px Menlo, Monaco, monospace; width:100%; }   /* the width of the panel, as every table */
  table.rp-trace th { font:600 10.5px Helvetica, Arial, sans-serif; color:var(--soft); text-align:center; padding:2px 8px; white-space:nowrap;
                      border-bottom:1px solid #e3e8e8; position:sticky; top:0; background:#fff; }
  table.rp-trace td { padding:2px 8px; text-align:center; border-bottom:1px solid #f1f4f4; white-space:nowrap; }
  table.rp-trace td.v { text-align:left; color:var(--soft); width:1%; }
  table.rp-trace th:first-child { text-align:left; width:1%; }
  table.rp-trace td[data-k] { min-width:var(--cw, 0); }
  table.rp-trace td.unk { color:var(--soft); }
  /* the first column, the names, stays when the table scrolls */
  table.rp-trace th:first-child, table.rp-trace td.v { position:sticky; left:0; background:#fff; box-shadow:inset -1px 0 #e3e8e8; }
  table.rp-trace td.v { z-index:1; } table.rp-trace th:first-child { z-index:3; } table.rp-trace th { z-index:2; }
  table.rp-trace th.fail { color:var(--bad); background:#fbeceb; }
  .rp-lead { margin-top:6px; }
  .rp-broken { margin:6px 0 2px; } .rp-broken .code { font:11px Menlo, Monaco, monospace; color:var(--bad); margin:2px 0 0 8px; }
  table.rp-trace td.fail { background:#fbeceb; }
  table.rp-trace .k { cursor:pointer; } table.rp-trace th.hov { color:#2f6fb5; } table.rp-trace td.hov { background:#eef4fb; }
  table.rp-trace td.fail.hov { background:#f6dedc; }
  /* the step on the diagram, when another than the one that fails: the column shaded, as the red one */
  table.rp-trace th.cur { color:#2f6fb5; background:#dce9f7; }
  table.rp-trace td.cur, table.rp-trace td.cur.hov { background:#dce9f7; }
  .rp-at { color:var(--soft); margin-top:4px; } .rp-at .b { color:#2f6fb5; font-weight:600; } .rp-at .r { color:var(--bad); font-weight:600; }
  table.rp-trace tr.port td.v { color:var(--ink); font-weight:600; }
  .rp-link { border:0; background:none; color:var(--teal); text-decoration:underline; cursor:pointer;
             font:11px -apple-system, "Segoe UI", Helvetica, Arial, sans-serif; padding:4px 0 0; }
  pre.code { margin:0 0 12px; font:11.5px/1.55 Menlo, Monaco, monospace; color:var(--ink); overflow-x:auto; }
  details.rp-code > summary { cursor:pointer; margin:2px 0 2px -4px; list-style:none; display:flex; align-items:center; gap:2px; }
  details.rp-code > summary::-webkit-details-marker { display:none; }
  .chev { display:inline-flex; color:var(--soft); transition:transform .12s; }
  details[open] > summary .chev { transform:rotate(90deg); }
  details.rp-code[open] > summary { margin-bottom:6px; }
  details.rp-code pre.code { margin-bottom:4px; }
  pre.code span.l { display:block; } pre.code span.l.hit { background:#fbeceb; }
  pre.code span.n { display:inline-block; width:2.4em; color:var(--faint); text-align:right; margin-right:12px;
                    user-select:none; -webkit-user-select:none; }
  #popview h4 { font:600 12px -apple-system, Helvetica, Arial, sans-serif; color:var(--soft); margin:12px 0 4px; }
  .q-item { padding:9px 10px 8px 8px; border-bottom:1px solid #f1f4f4; cursor:pointer; display:grid;
            grid-template-columns:52px minmax(0,1fr); column-gap:6px; align-items:center;
            min-height:58px; box-sizing:border-box; }   /* a row of one line as high as one of two */
  .q-tag { display:flex; justify-content:center; align-items:center; }
  .q-main { display:grid; gap:3px; min-width:0; }
  .q-item.on { background:#eef3f9; }
  .q-item:last-child { border-bottom:0; } .q-item:hover { background:var(--fill); }
  /* the title of an item as the title of its details: no jump from one to the other */
  .q-item .t, .q-item .s, .q-item .v { overflow:hidden; text-overflow:ellipsis; white-space:nowrap; }   /* one line each: rows of one height */
  .q-item .t { font-size:12.5px; }
  .q-item .s { color:var(--soft); font-size:11px; } .q-item .s.ok { color:var(--ok); } .q-item .s.old { color:var(--warn); }
  .q-item .v { font:11px Menlo, Monaco, monospace; color:var(--ink); }
  .tools #repairs { width:auto; padding:0 7px; gap:4px; display:none; color:var(--bad); font:600 11.5px Helvetica, Arial, sans-serif; }
  .tools #repairs svg { stroke:currentColor; }
</style></head><body>
<div id="top"><div id="context"></div>
  <span class="tools">
    <button id="all" title="Open all" onclick="openAll(!allOpen)"><svg viewBox="0 0 16 16"><path d="M4.5 6.5 8 3l3.5 3.5M4.5 9.5 8 13l3.5-3.5"/></svg></button>
    <button title="Fit the diagram to the window (0); the wheel zooms where it points, a drag moves" onclick="fit()"><svg viewBox="0 0 16 16"><path d="M2 5.5V2h3.5M10.5 2H14v3.5M14 10.5V14h-3.5M5.5 14H2v-3.5"/></svg></button>
    <span class="sep"></span>
    <button class="on" title="Names of the ports" onclick="flip(this, 'names')"><svg viewBox="0 0 16 16"><rect x="4.5" y="3" width="7" height="10" rx="1.5"/><path d="M1.5 6h3M1.5 10h3M11.5 8h3"/></svg></button>
    <button class="on" title="Types of the ports (ranges, reals, bit vectors)" onclick="flip(this, 'types')"><svg viewBox="0 0 16 16"><path d="M1.5 8h13" stroke-width="2.4"/><path d="M6.5 11.5l3-7"/></svg></button>
    <button id="conds" class="on" title="Transition labels of the models: the inputs that take each transition" onclick="flip(this, 'conditions')"><svg viewBox="0 0 16 16"><circle cx="3" cy="11" r="1.8"/><circle cx="13" cy="11" r="1.8"/><path d="M4.8 11h6.4"/><rect x="5" y="3" width="6" height="4" rx="1"/></svg></button>
    <button class="on" title="Results of the checks (A∧G, A, G, R, WS, ¬V, ⪯, M ⊨ C, E ⊨ A) and their cores" onclick="flip(this, 'checks')"><svg viewBox="0 0 16 16"><rect x="1.5" y="4" width="13" height="8" rx="3"/><path d="M5.5 8l1.8 1.8L10.5 6.3"/></svg></button>
    <span class="sep" id="runsep"></span>
    <button id="run" title="Check: A ∧ G of each contract, and the refinements, implementations, and environments drawn" onclick="runChecks()"><svg viewBox="0 0 16 16"><path class="idle" d="M5 3.2v9.6L13 8z"/><path class="turn" d="M8 2.2a5.8 5.8 0 1 1-5.8 5.8"/></svg></button>
    <button id="repairs" title="The panel of the checks: open or close (Esc)" onclick="toggleQueue()"><svg viewBox="0 0 16 16"><path d="M10.2 2.4a3.2 3.2 0 0 0-3.9 4.1L2.6 10.2a1.3 1.3 0 0 0 1.8 1.8l3.7-3.7a3.2 3.2 0 0 0 4.1-3.9l-1.9 1.9-1.6-.4-.4-1.6z"/></svg><span id="repcount"></span></button>
    <span class="sep"></span>
    <button title="Export the whole diagram (SVG, PNG, PDF)" onclick="exportMenu()"><svg viewBox="0 0 16 16"><path d="M8 2v8M4.8 6.8 8 10l3.2-3.2M2.5 11.5v2h11v-2"/></svg></button>
    <button title="What the shapes mean" onclick="help()"><svg viewBox="0 0 16 16"><circle cx="8" cy="8" r="6.2"/><path d="M6.3 6.3a1.8 1.8 0 1 1 2.4 1.7c-.5.2-.7.6-.7 1.1v.4"/><circle cx="8" cy="11.6" r=".4"/></svg></button>
  </span><div id="help"></div><div id="export"><button onclick="exportAs('svg')">SVG</button><button onclick="exportAs('png')">PNG</button><button onclick="exportAs('pdf')">PDF</button></div></div>
<div id="view"></div>
<div id="stale"></div>
<div id="busy">checking…</div>
<div id="tip"></div>
<div id="pop"><header><b id="poptitle"></b><button class="ib" title="Close" aria-label="Close" onclick="closePop()"><svg viewBox="0 0 16 16"><path d="M4 4l8 8M12 4l-8 8"/></svg></button></header><div id="popview"></div></div>
<div id="wave" tabindex="-1" role="dialog" aria-label="Waveform"></div>
<div id="insp" role="complementary" aria-label="The checks" tabindex="-1"><div id="insp-bar"></div><div id="repair"></div></div>
__ELK__
<script>
let DATA = __DATA__;
let busy = false;               // checks running in the editor (setBusy)
let progressed = {};            // the results known while they run: contract name -> {tag: status}
const LINE = 17, PAD = 10, PORT = 6, HEAD = 28, ICON = 16, INLINE_STATES = 8;
const ctx = document.createElement("canvas").getContext("2d");
function textWidth(s, font) { ctx.font = font; return ctx.measureText(s).width; }
// a number in a few digits, never rounder than it is: 0.8095238095238095 is 0.8095, but 17.996 is
// 17.996, not 18 (the value a term compares it with), and 12345.6 keeps its units
function digits(x) {
  if (typeof x !== "number" || Number.isInteger(x) || !isFinite(x) || String(x).length <= 7) return String(x);   // 17.875 whole
  for (let p = Math.max(4, Math.floor(Math.log10(Math.abs(x))) + 2); p < 17; p++) {
    const t = x.toPrecision(p);
    if (+t === x || !/0$/.test(t.replace(/e.*$/, ""))) return String(+t);
  }
  return String(x);
}
// a Boolean value of a counterexample: ⊤ or ⊥ (the formulas keep the words true and false)
const truth = b => b ? "⊤" : "⊥";
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
// the button of the transition labels: only on a page with a model drawn with its transitions
function condsButton() {
  document.getElementById("conds").style.display =
    Object.values(byId).some(n => n.model && n.model.fsm && (n.model.fsm.edges || []).some(e => e.label)) ? "" : "none";
}
condsButton();
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
// Each part opens with a tag: θ (parameters: named constants), A (assumes), G (guarantees), E (an environment).
// With a core in focus, its terms are on a red ground: [kind, tag, text, in the core].
// src: the lines of the terms in the file, [first, last], when known.
function terms(c, out, core, src, which) {
  const part = (tag, fs, hit, at) => fs.forEach((f, j) => wrap(f).forEach((l, i) =>
    out.push(["f", i || j ? "" : tag, l, hit.includes(j), at && at[j], which ? which + ":" + tag + j : null])));
  if (c.K) part("θ", c.K, []);           // the parameters (named constants) and their values
  if (c.E) { part("A", c.A, core ? core.A : [], src && src.A); part("E", c.E, []); return; }
  // no assumption is A = ⊤ (true), as no guarantee is G = ⊤: both written
  part("A", c.A.length ? c.A : ["⊤"], core ? core.A : [], c.A.length && src && src.A);
  part("G", c.G.length ? c.G : ["⊤"], core ? core.G : [], c.G.length && src && src.G);
}
function focused(n) {
  if (show.checks && n.id in focus) return n.checks[focus[n.id]].core;
  if (panel && panel.id !== n.id && n.contract) {     // a part whose assumptions the open counterexample breaks
    const pn = byId[panel.id], pw = pn && pn.badge && tagOf(pn.badge.text) === panel.tag && pn.badge.why;
    const b = pw && pw.parts && pw.parts[n.id];
    if (b && b.length) {
      const ga = n.contract._gA;
      return { part: "contract", A: ga ? ga.map((g, j) => g.some(i => b.includes(i)) ? j : -1).filter(j => j >= 0) : b, G: [] };
    }
  }
  const w = panel && panel.id === n.id && n.badge && tagOf(n.badge.text) === panel.tag && n.badge.why;
  if (!w || !w.violated || !w.violated.length || !n.contract) return null;    // the terms the counterexample breaks
  const gg = n.contract._gG;
  const shown = gg ? gg.map((g, j) => g.some(i => w.violated.includes(i)) ? j : -1).filter(j => j >= 0) : w.violated;
  return { part: "contract", A: [], G: shown };
}

// The lines under the name. A long contract (a computed one, often) shows
// its first lines; the rest opens in a panel.
const LONG = 14, SHORT = 8;
function lines(n) {
  const out = [], core = focused(n);
  const part = (c, which) => {
    const ts = [];
    terms(c, ts, core && core.part === which ? core : null, which === "contract" ? n.source : null, which);
    if (ts.length <= LONG) { out.push(...ts); return; }
    const parts = [];                  // A and G cut apart: the first lines of each
    ts.forEach(t => (t[1] || !parts.length) ? parts.push([t]) : parts[parts.length - 1].push(t));
    let left = SHORT;
    parts.forEach((p, i) => {
      const rest = parts.slice(i + 1).reduce((s, q) => s + q.length, 0);
      const keep = Math.max(1, i === parts.length - 1 ? left : Math.max(Math.floor(SHORT / parts.length), SHORT - rest));
      let room = keep - p.filter(t => t[3]).length;   // the lines of a core in focus are kept first
      const shown = p.filter(t => t[3] || room-- > 0);
      if (shown[0] !== p[0]) shown[0] = [shown[0][0], p[0][1], shown[0][2], shown[0][3], shown[0][4], shown[0][5]];
      out.push(...shown);
      if (p.length > shown.length)   // with the lines of the terms it hides, for the cursor
        out.push(["more", "", "⋯ " + (p.length - shown.length) + " more lines", c,
                  p.filter(t => !shown.includes(t) && t[4]).map(t => t[4])]);
      left -= Math.min(keep, shown.length);
    });
  };
  if (n.note) out.push(["s", "", n.note]);
  if (n.contract && open.has("c:" + n.id)) {
    if (n.composed && open.has("k:" + n.id)) out.push(["s", "", "Contract " + n.contract.name]);
    part(n.contract, "contract");
  }
  if (n.composed && open.has("k:" + n.id)) {
    out.push(["s", "", "Composition " + n.composed.name]);
    part(n.composed, "composed");
  }
  return out;
}
const lineWidth = ([k, tag, t]) => (k === "f" ? 20 : 0) + textWidth(t, k === "f" ? FONT_F : FONT_S);
function showTerms(n, c) {     // the whole contract, in the panel, with the core in focus on red
  const esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;"), ts = [];
  const core = focused(n), which = c === n.contract ? "contract" : "composed";
  terms(c, ts, core && core.part === which ? core : null, c === n.contract ? n.source : null);
  document.getElementById("poptitle").textContent = n.title + " · Contract " + c.name
    + (ts.some(t => t[3]) ? " · the terms of the core in red" : "");
  document.getElementById("popview").innerHTML = "<table class='fsm'>" + ts.map(([k, tag, t, hit, at]) =>
    "<tr" + (at ? " data-line='" + at[0] + "'" + (inEditor() ? " title='double click: line " + at[0] + "'" : "") : "")
    + (hit ? " style='background:#f6dcd9'" : "")
    + "><td class='st'>" + (tag || "") + "</td><td>" + esc(t) + "</td></tr>").join("") + "</table>";
  document.getElementById("pop").style.display = "flex";
}

function icons(n) {
  const out = [];
  if (n.contract) out.push(["contract", "c:" + n.id, "Contract " + n.contract.name]);
  if (n.composed) out.push(["composed", "k:" + n.id, "Composition " + n.composed.name]);
  if (n.model) {             // a model with nothing to draw has its icon, grey
    const f = n.model.fsm;
    const from = { controller: " (synthesized by slugs)", rtl: " (RTL)", json: " (JSON)", smv: " (SMV)", c: " (C)" }[n.model.source] || "";
    const kind = n.model.source === "controller" ? "synth" : "model";
    if (states(n)) out.push([kind, "m:" + n.id, "model " + n.model.name + from + ", " + f.count + " states"]);
    else if (n.model.source === "c") out.push([kind, n.model.code ? "m:" + n.id : null, "function " + n.model.name + from
      + ": its outputs come from its inputs, one call at a time (ESBMC, CBMC)" + (n.model.code ? ". Click: its code" : "")]);
    else out.push([kind, null, "model " + n.model.name + from + (f && f.too_many
      ? ": " + (f.more ? "more than " : "") + f.count + " states, too many to show" : ": no registers, its outputs are functions of its inputs")]);
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
// A check running: a tag as wide as its result will be, with three dots.
function pending(g, x, y, s) {
  const w = tagWidth(s), p = el("g", { class: "pending" }, g);
  el("rect", { x, y, width: w, height: 16, rx: 4, fill: "#fff", stroke: "var(--line)", "stroke-width": 1 }, p);
  [-5, 0, 5].forEach(d => el("circle", { cx: x + w / 2 + d, cy: y + 8, r: 1.6 }, p));
  tip(p, (WHAT[s] || s) + ": being checked");
  return w;
}
const SAYS = { valid: "holds", invalid: "does not hold", unknown: "not decided" };
const WHAT = { "A": "compatible: the assumptions A are satisfiable",
               "G": "consistent: A → G is satisfiable",
               "A∧G": "feasible: A ∧ G is satisfiable",
               "R": "realizable (GR(1))",
               "WS": "well-separated: the component cannot win by making the environment break A",
               "¬V": "not vacuous: each liveness guarantee asks something of the component",
               "M ⊨ C": "the model implements the contract",
               "safe": "safe code: no overflow, division by zero, access out of bounds, or failed assert",
               "E ⊨ A": "the environment satisfies the assumptions",
               "⪯": "what is inside refines this contract" };
const checks = n => show.checks && n.checks ? n.checks : [];

const MODELS = {};       // the layout of each open model drawn in its box
// the tag of A ∧ G while the checks run, on a box that has none yet
const pendingTag = n => busy && show.checks && n.contract && !checks(n).some(c => c.name === "A∧G" || c.name === "A");
function size(n) {
  const ls = lines(n);
  const top = textWidth(n.title, FONT_H) + icons(n).length * (ICON + 4) + 14
            + checks(n).reduce((w, c) => w + tagWidth(c.name) + 4, checks(n).length ? 6 : 0)
            + (n.badge && show.checks ? tagWidth(n.badge.text) + 18 : 0) + (inside(n) ? ICON + 6 : 0)
            + (pendingTag(n) ? tagWidth("A∧G") + 4 : 0);      // the A ∧ G being checked, for a while
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
const ERR = {};          // the wires of a signal the parts do not agree on: what to say
// The column of each box inside n, by the flow of its wires: the longest
// path from the inputs of n, a wire back to an earlier box (a loop) left out.
// Viewpoints stacked in one box share a column.
function columns(n) {
  const col = {}, ids = n.children.map(c => c.id);
  if (n.layout === "stack") { ids.forEach(id => { col[id] = 0; }); return col; }
  // the wires between the boxes, counted: a loop goes back where it sends the fewest
  // wires back (Eades, Lin and Smyth: the sinks last, the sources first, else the box
  // that sends the most more than it receives), in the order of the list on a tie
  const weight = {};
  ids.forEach(id => { weight[id] = {}; });
  (n.edges || []).forEach(w => { if (w.from && w.from[0] && w.to[0] && weight[w.from[0]] && weight[w.to[0]]
                                     && w.from[0] !== w.to[0])
    weight[w.from[0]][w.to[0]] = (weight[w.from[0]][w.to[0]] || 0) + 1; });
  const left = new Set(ids), first = [], last = [];
  const sum = (id, out) => ids.reduce((s, o) => s + (left.has(o) ? (out ? weight[id][o] : weight[o][id]) || 0 : 0), 0);
  while (left.size) {
    let moved = true;
    while (moved) {
      moved = false;
      for (const id of ids) if (left.has(id) && !sum(id, true)) { last.unshift(id); left.delete(id); moved = true; }
      for (const id of ids) if (left.has(id) && !sum(id, false)) { first.push(id); left.delete(id); moved = true; }
    }
    if (!left.size) break;
    let best = null;
    for (const id of ids) if (left.has(id) && (best === null || sum(id, true) - sum(id, false) > sum(best, true) - sum(best, false)))
      best = id;
    first.push(best); left.delete(best);
  }
  const order = first.concat(last), place = {};
  order.forEach((id, i) => { place[id] = i; });
  order.forEach(id => { col[id] = col[id] || 0;
    Object.keys(weight[id]).forEach(t => { if (place[t] > place[id]) col[t] = Math.max(col[t] || 0, col[id] + 1); }); });
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
    const id = n.id + ":e" + i; KIND[id] = typeOf(n, w.var); ERR[id] = w.error;
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
  // drawn at the width it was measured at: small on the screen, the glyphs of a
  // browser round up and the name would run into the icons after it
  name.setAttribute("textLength", textWidth(n.title, FONT_H)); name.setAttribute("lengthAdjust", "spacingAndGlyphs");
  if (n.contract) {
    const flip = toggle("c:" + n.id);
    name.onclick = e => { flip(); if (e.detail === 2) reveal(n.source && n.source.line); };   // a double click opens and closes: as it was
    tip(name, "Click: open or close the contract" + (n.source && typeof acquireVsCodeApi === "function"
                                                     ? " · double click: its text in the editor" : ""));
  }
  let ix = x + PAD + textWidth(n.title, FONT_H) + 10;
  icons(n).forEach(([kind, key, title]) => {
    const on = key && open.has(key), c = icon(box, kind, ix, y + 6, on, title, !key);
    if (key) c.onclick = (kind === "model" || kind === "synth") && !on && !inlineModel(n) ? () => showModel(n) : toggle(key);
    ix += ICON + 4; });
  if (checks(n).length) ix += 6;
  checks(n).forEach((c, i) => {      // a click on a check that has a core shows the core; on one that fails, its repairs
    const t = el("g", { "data-check": n.id + "|" + c.name, "data-text": c.name }, box), on = focus[n.id] === i;
    const fix = c.status === "invalid";
    ix += tag(t, ix, y + 6, c.name, c.status) + 4;
    tip(t, (WHAT[c.name] || c.name) + ": " + (SAYS[c.status] || "not checked") + (c.reason ? " (" + c.reason + ")" : "")
           + (fix ? ". Click: why it fails" : c.core ? (on ? ". Click: hide the core" : ". Click: show the core") : ""));
    if (on) el("rect", { x: ix - 4 - tagWidth(c.name) - 2, y: y + 4, width: tagWidth(c.name) + 4, height: 20, rx: 5,
                         fill: "none", stroke: COLOR[c.status], "stroke-width": 1 }, t);
    if (c.core || fix) { t.setAttribute("class", "icon");
      // one that fails: its details, with its core shown; one that holds: its core on and off
      t.onclick = () => { if (fix) { if (c.core) { focus[n.id] = i; open.add((c.core.part === "composed" ? "k:" : "c:") + n.id); }
                                     return openRepair(n, c.name); }
        if (on) delete focus[n.id];
        else { focus[n.id] = i; open.add((c.core.part === "composed" ? "k:" : "c:") + n.id); } render(); }; } });
  if (pendingTag(n)) {
    const known = (progressed[n.contract.name] || {})["A∧G"];   // A ∧ G: known already, or being checked
    ix += (known ? tag(el("g", {}, box), ix, y + 6, "A∧G", known) : pending(el("g", {}, box), ix, y + 6, "A∧G")) + 4;
  }
  let rx = x + e.width - PAD;
  if (inside(n)) {             // fold: hide or show the boxes inside
    const f = el("g", { class: "icon" }, box), shut = folded.has(n.id), cx = rx - ICON / 2, cy = y + 14;
    el("rect", { x: rx - ICON, y: y + 6, width: ICON, height: ICON, rx: 4, fill: shut ? "var(--teal)" : "#fff",
                 stroke: "var(--teal)", "stroke-width": 1.1 }, f);
    el("path", { d: shut ? `M${cx - 3.5} ${cy - 1.5} L${cx} ${cy + 2} L${cx + 3.5} ${cy - 1.5}`
                         : `M${cx - 3.5} ${cy + 1.5} L${cx} ${cy - 2} L${cx + 3.5} ${cy + 1.5}`,
                 fill: "none", stroke: shut ? "#fff" : "var(--teal)", "stroke-width": 1.4,
                 "stroke-linecap": "round", "stroke-linejoin": "round" }, f);
    tip(f, (shut ? "Show the " : "Hide the ") + (n.children.length === 1 ? "box" : n.children.length + " boxes") + " inside");
    f.onclick = fold(n.id);
    rx -= ICON + 6;
  }
  if (n.badge && show.checks) { const b = el("g", { "data-check": n.id + "|" + tagOf(n.badge.text), "data-text": n.badge.text,
                                                   class: "icon" }, box), w = tagWidth(n.badge.text);
    const known = busy && !n.badge.status && n.contract && (progressed[n.contract.name] || {})[n.badge.text];
    if (busy && !n.badge.status && !known) pending(b, rx - w, y + 6, n.badge.text);
    else tag(b, rx - w, y + 6, n.badge.text, n.badge.status || known);
    tip(b, (WHAT[n.badge.text.split(" of ")[0]] || n.badge.text) + ": " + (SAYS[n.badge.status] || "not checked")
           + ". Click: the check" + ((n.repair || {})[tagOf(n.badge.text)] ? ", and its repairs" : ""));
    b.onclick = () => openRepair(n, tagOf(n.badge.text)); }
  sz.ls.forEach(([k, label, s, core, at, term], i) => {
    const frame = r => el("rect", { class: "hl", x: x + 4, y: ty - 12, width: e.width - 8, height: LINE, rx: 3,
                                     fill: "none", stroke: "var(--teal)", "stroke-width": 1.5 }, r);
    const ty = y + HEAD + 12 + i * LINE, row = el("g", term ? { "data-term": term } : {}, box);
    if (k === "f") {
      if (core) el("rect", { x: x + 4, y: ty - 12, width: e.width - 8, height: LINE, rx: 3, fill: "#f6dcd9", class: "core-row" }, row);
      if (at) {             // its lines in the file: the editor goes there, and marks it from there
        row.setAttribute("class", "trow"); row.setAttribute("data-line", at[0]); row.setAttribute("data-last", at[1]);
        el("rect", { x: x + 4, y: ty - 12, width: e.width - 8, height: LINE, fill: "transparent" }, row);   // the whole line
        frame(row);
        tip(row, (at[0] === at[1] ? "line " + at[0] : "lines " + at[0] + " to " + at[1])
                 + (inEditor() ? " · double click: there in the editor" : "")); }
      if (label) { el("rect", { x: x + PAD, y: ty - 10, width: 13, height: 13, rx: 3, fill: "var(--teal)" }, row);
                   const t = text(row, x + PAD + 6.5, ty, "s", label); t.setAttribute("text-anchor", "middle");
                   t.setAttribute("style", "font-size:9.5px;font-weight:700;fill:#fff"); }
      const ft = text(row, x + PAD + 20, ty, "f", s);
      if (n.contract && (n.contract.K || []).includes(s)) {      // a named constant: a repair shown changes it
        ft.setAttribute("data-const", s.split(" = ")[0]); ft.setAttribute("data-cbox", n.id); } }
    else if (k === "more") {
      if (at && at.length) { row.setAttribute("class", "trow"); row.setAttribute("data-hidden", JSON.stringify(at)); frame(row); }
      const t = text(row, x + PAD + 20, ty, "s", s);
      t.setAttribute("style", "fill:var(--teal);cursor:pointer;text-decoration:underline");
      tip(t, "the whole contract");
      t.onclick = () => showTerms(n, core); }
    else text(row, x + PAD, ty, "s", s); });
  if (sz.m) drawModel(box, sz.m, x + PAD, y + HEAD + sz.ls.length * LINE + (sz.ls.length ? 8 : 0) + 4);
  (e.ports || []).forEach(p => {
    el("rect", { x: x + p.x, y: y + p.y, width: p.width, height: p.height, fill: "var(--teal)" }, g);
    (p.labels || []).forEach(l => { const t = text(g, x + p.x + l.x, y + p.y + l.y + 9, "w", l.text);
      t.setAttribute("data-var", String(p.id).split(":").pop()); t.setAttribute("data-pbox", n.id);
      if (l.x < 0) t.setAttribute("data-side", "left"); }); });
  (e.children || []).forEach(c => drawNode(g, c, x, y));
  (e.edges || []).forEach(ed => drawEdge(g, ed, x, y));
  // a signal the parts do not agree on (two drivers, two types): its names on the ports red, and said
  Object.entries(n.clash || {}).forEach(([v, says]) => {
    const here = new Set([n.id, ...(n.children || []).map(c => c.id)]);
    g.querySelectorAll('text[data-var="' + v + '"]').forEach(t => {
      if (!here.has(t.getAttribute("data-pbox"))) return;
      t.setAttribute("style", "fill:var(--bad)"); tip(t, says); }); });
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
    // an input that nothing drives: its name in the color of the warning, a short
    // dashed stub before it (the words "not driven" in its tip, and in the help)
    const name = [...g.querySelectorAll("text[data-var]")].find(t => t.getAttribute("data-pbox") === c.id
      && t.getAttribute("data-var") === w.var && t.getAttribute("data-side") === "left");
    const says = w.var + ": not driven, no part or input drives it";
    if (name) {
      name.setAttribute("style", "fill:var(--warn)"); tip(name, says);
      const end = +name.getAttribute("x") - 3, sy = +name.getAttribute("y") - 4;
      tip(el("line", { x1: end - 16, y1: sy, x2: end, y2: sy, stroke: "var(--warn)", "stroke-width": 1.4,
                       "stroke-dasharray": "4 3" }, g), says);
      return;
    }
    tip(el("line", { x1: px - 26, y1: py, x2: px, y2: py, stroke: "var(--warn)", "stroke-width": 1.4,
                     "stroke-dasharray": "4 3" }, g), says);
    const t = text(g, px - 28, py - 4, "w", "not driven"); t.setAttribute("text-anchor", "end");
    t.setAttribute("style", "fill:var(--warn)"); });
}

function drawEdge(g, ed, ox, oy) {
  const k = KIND[ed.id], bad = ERR[ed.id];
  (ed.sections || []).forEach(s => {
    const pts = [s.startPoint, ...(s.bendPoints || []), s.endPoint];
    const line = el("polyline", { points: pts.map(p => (ox + p.x) + "," + (oy + p.y)).join(" "), fill: "none",
                     stroke: bad ? "var(--bad)" : "var(--wire)", "stroke-width": k && k.bus ? 2.6 : k || bad ? 1.8 : 1.1,
                     "stroke-linejoin": "round", "marker-end": bad ? "url(#arrow-bad)" : "url(#arrow)" }, g);
    if (bad) tip(line, bad);
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
// the scale of the whole diagram in the view: a small diagram is made larger too, up to 1.5 times
function fitScale() {
  const v = document.getElementById("view"), W = v.clientWidth, H = v.clientHeight - TOP;
  return Math.min(1.5, (W - 40) / Math.max(size0.w, 1), (H - 30) / Math.max(size0.h, 1));
}
function fit() {
  const v = document.getElementById("view"), W = v.clientWidth, H = v.clientHeight - TOP;
  const k = fitScale();
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
  // the pointer is held by the view once it moves, not before: a click or a
  // double click on a box still reaches the box
  v.addEventListener("pointerdown", e => {
    if (!cam || e.button !== 0 || live(e.target)) return;
    drag = { x: e.clientX, y: e.clientY, cx: cam.x, cy: cam.y, id: e.pointerId, held: false };
  });
  v.addEventListener("pointermove", e => {
    if (!drag) return;
    if (!drag.held) {
      if (Math.abs(e.clientX - drag.x) + Math.abs(e.clientY - drag.y) < 4) return;
      drag.held = true; v.setPointerCapture(drag.id); v.classList.add("moving");
    }
    cam.x = drag.cx + e.clientX - drag.x; cam.y = drag.cy + e.clientY - drag.y; place();
  });
  const stop = () => { drag = null; v.classList.remove("moving"); };
  // a double click on a box (not on its name, which opens it): its text
  v.addEventListener("dblclick", e => {
    for (let t = e.target; t && t !== v; t = t.parentNode) {
      if (t.onclick) return;
      const line = t.getAttribute && t.getAttribute("data-line");   // a term: its line
      if (line) { reveal(+line); return; }
      const id = t.getAttribute && t.getAttribute("data-box");
      if (id && byId[id] && byId[id].source) { reveal(byId[id].source.line); return; }
    }
  });
  document.getElementById("popview").addEventListener("dblclick", e => {
    const tr = e.target.closest && e.target.closest("tr[data-line]");
    if (tr) reveal(+tr.getAttribute("data-line"));
  });
  v.addEventListener("pointerup", stop); v.addEventListener("pointercancel", stop);
  v.addEventListener("wheel", e => {
    if (!cam) return;
    e.preventDefault();
    const dy = e.deltaMode === 1 ? e.deltaY * 33 : e.deltaMode === 2 ? e.deltaY * 400 : e.deltaY;  // lines, pages
    zoomAt(e.clientX, e.clientY, Math.exp(-dy * (e.ctrlKey ? 0.01 : 0.002)));          // a pinch comes with ctrl
  }, { passive: false });
  document.addEventListener("keydown", e => {
    if (e.key === "Escape") {      // the help first, the model window, the details (back to the list), the panel
      const help = document.getElementById("help"), pop = document.getElementById("pop");
      if (help.style.display === "block") help.style.display = "none";
      else if (WAVE.open) closeWave();
      else if (pop.style.display === "flex") closePop();
      else if (inspector.open && inspector.tab === "details" && panel) closeRepair();
      else hideInspector();
      return; }
    if (e.target.closest && e.target.closest("input, textarea")) return;
    if (WAVE.open && WAVE.key && !(e.target.closest && e.target.closest("#wave, button, a, select, summary")) && WAVE.key(e)) return;
    if ((e.key === "ArrowLeft" || e.key === "ArrowRight") && inspector.open && panel && panel.goStep
        && document.getElementById("insp").contains(document.activeElement)) {   // the step of the counterexample
      panel.goStep(e.key === "ArrowLeft" ? -1 : 1); e.preventDefault(); return; }
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
  const mb = el("marker", { id: "arrow-bad", viewBox: "0 0 10 10", refX: 9, refY: 5, markerWidth: 7,
                            markerHeight: 7, markerUnits: "userSpaceOnUse", orient: "auto" }, defs);
  el("path", { d: "M0,0 L10,5 L0,10 z", fill: "var(--bad)" }, mb);
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
  return (DATA.edges || []).map((w, i) => { const id = "root:e" + i; KIND[id] = typeOf(ROOT, w.var); ERR[id] = w.error;
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
// Boxes wired to each other at the top are laid out by ELK in columns: the
// column of each box, and its place in it, stay those of the first drawing.
let colPlan = null;              // key -> [column, place]
function colsOf(out) {
  if (!(DATA.edges || []).length) return null;
  const keys = keysOf(DATA.nodes), cs = out.children || [], xs = [];
  cs.forEach(c => { if (!xs.some(x => Math.abs(x - c.x) < 20)) xs.push(c.x); });
  xs.sort((a, b) => a - b);
  const col = c => xs.findIndex(x => Math.abs(x - c.x) < 20), plan = {};
  cs.forEach(c => { const same = cs.filter(d => col(d) === col(c)).sort((a, b) => a.y - b.y);
                    plan[keys[c.id]] = [col(c), same.indexOf(c)]; });
  return plan;
}
function layout() {
  const keys = colPlan ? keysOf(DATA.nodes) : {};
  const items = rootRows
    ? rootRows.map((ids, r) => ({ id: "root:row" + r, x: 0, y: r * 40, edges: [],
        children: ids.map((id, i) => toElk(byId[id], i, 0)),
        layoutOptions: Object.assign({}, FLOW, { "elk.padding": "[top=0,left=0,bottom=0,right=0]", "elk.alignment": "LEFT" }) }))
    : DATA.nodes.map((n, i) => { const e = toElk(n, i, 0), at = colPlan && colPlan[keys[n.id]];
        if (at) Object.assign(e.layoutOptions, { "elk.layered.layering.layerChoiceConstraint": String(at[0]),
                                                 "elk.layered.crossingMinimization.positionChoiceConstraint": String(at[1]) });
        return e; });
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
  if (rowPlan === undefined) { rootRows = null; colPlan = null; const first = await layout();
                                rowPlan = planOf(rowsOf(first)); colPlan = rowPlan ? null : colsOf(first); }
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
  select(selectedLine, false, selectedAt);
  afterRender();
}

// From the diagram to the text, and back: a double click on a box asks the
// editor to show where it is written; the editor tells the line of the
// cursor, and the box written there is marked (and brought into view).
let selectedLine = null, selectedAt = null;
const inEditor = () => typeof acquireVsCodeApi === "function";
function reveal(line) {
  if (!inEditor() || !line) return;
  vscodeApi = vscodeApi || acquireVsCodeApi();
  vscodeApi.postMessage({ type: "chase-reveal", line });
}
// line: where the contract of the cursor starts; at: the line of the cursor
function select(line, bring, at) {
  selectedLine = line; selectedAt = at || null;
  let hit = null;
  document.querySelectorAll("[data-box]").forEach(e => {
    const n = byId[e.getAttribute("data-box")], on = !!(line && n && n.source && n.source.line === line);
    e.classList.toggle("sel", on);
    if (on && !hit) hit = e;
  });
  document.querySelectorAll(".trow").forEach(r => {
    const box = r.closest("[data-box]"), hidden = r.getAttribute("data-hidden");
    const ranges = hidden ? JSON.parse(hidden) : [[+r.getAttribute("data-line"), +r.getAttribute("data-last")]];
    const on = !!(selectedAt && box && box.classList.contains("sel") && ranges.some(([a, z]) => a <= selectedAt && selectedAt <= z));
    r.classList.toggle("sel", on);
  });
  if (!hit || !bring || !cam) return;
  const r = hit.getBoundingClientRect(), v = document.getElementById("view").getBoundingClientRect();
  if (r.left >= v.left && r.right <= v.right && r.top >= v.top + TOP && r.bottom <= v.bottom) return;
  cam.x += (v.left + v.width / 2) - (r.left + r.width / 2);
  cam.y += (v.top + TOP + (v.height - TOP) / 2) - (r.top + r.height / 2);
  place();
}

// A model with more states is shown in a panel, as a table of moves.
function showModel(n) {
  if (n.model.code) { showCode(n); return; }
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
// The code of a C function, and of the functions of its file it calls, with the lines of the file.
function showCode(n) {
  const c = n.model.code, esc = s => s.replace(/&/g, "&amp;").replace(/</g, "&lt;").replace(/>/g, "&gt;");
  document.getElementById("poptitle").textContent = n.title + " · " + c.file + " · function " + n.model.name;
  document.getElementById("popview").innerHTML = c.parts.map(q => "<h4>" + esc(q.name) + ", line " + q.line + "</h4>"
    + "<pre class='code'>" + q.text.split("\n").map((l, i) => "<span class='n'>" + (q.line + i) + "</span>" + esc(l)).join("\n")
    + "</pre>").join("");
  document.getElementById("pop").style.display = "flex";
}

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
  [["contract", "parameters θ, assumptions A, guarantees G"], ["composed", "composed contract (⊗)"],
   ["model", "model given (RTL, JSON, SMV)"], ["synth", "model synthesized (slugs)"]].forEach(([k, t]) =>
    sample(svg => icon(svg, k, 16, 2, false, t), t));
  sample(s => { el("rect", { x: 16, y: 2, width: 16, height: 16, rx: 4, fill: "#fff", stroke: "#17393b", "stroke-width": 1.1 }, s);
                el("path", { d: "M20.5 11.5 L24 8 L27.5 11.5", fill: "none", stroke: "#17393b", "stroke-width": 1.4 }, s); },
         "hide the boxes inside");
  // a check: green when it holds, red when not, grey when undecided
  const pair = (t, d) => { const d2 = document.createElement("div");
    const svg = el("svg", { width: 88, height: 20 }, d2), w = tagWidth(t);
    tag(svg, 44 - w - 2, 2, t, "valid"); tag(svg, 46, 2, t, "invalid");
    d2.appendChild(document.createTextNode(d)); h.appendChild(d2); };
  [["A", "compatible"], ["G", "consistent"], ["A∧G", "feasible"], ["R", "realizable"],
   ["WS", "well-separated"], ["¬V", "no vacuous output"], ["M ⊨ C", "implements"], ["safe", "safe code"],
   ["E ⊨ A", "environment"], ["⪯", "refines"]].forEach(([t, d]) => pair(t, d));
  sample(s => el("rect", { x: 4, y: 4, width: 40, height: 12, rx: 3, fill: "#f6dcd9" }, s), "core (click a red tag)");
  sample(s => { tag(s, 4, 2, "⪯", "invalid"); }, "click a result: the check, and its repairs");
  const wire = (w, d) => sample(s => el("line", { x1: 4, y1: 10, x2: 44, y2: 10, stroke: "#7d8c8d", "stroke-width": w }, s), d);
  wire(1.1, "Boolean");
  wire(1.8, "integer or real");
  sample(s => { el("line", { x1: 4, y1: 10, x2: 44, y2: 10, stroke: "#7d8c8d", "stroke-width": 2.6 }, s);
                el("line", { x1: 20, y1: 15, x2: 28, y2: 5, stroke: "#7d8c8d", "stroke-width": 1.3 }, s);
                text(s, 29, 7, "w", "8"); }, "bus, with its width");
  sample(s => { el("line", { x1: 4, y1: 10, x2: 20, y2: 10, stroke: "#c07a14", "stroke-width": 1.4, "stroke-dasharray": "4 3" }, s);
                const t = text(s, 23, 14, "w", "x"); t.setAttribute("style", "fill:#c07a14"); },
         "input that nobody drives (its name in orange)");
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
  condsButton();
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
  rebindPanel();
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
  else if (m.type === "chase-select") select(m.line, true, m.at);
  else if (m.type === "chase-busy") setBusy(m.on);
  else if (m.type === "chase-progress") progress(m.event);
  else if (m.type === "chase-repair-result") {
    const a = asked[m.id];
    if (a) { delete asked[m.id]; if (m.error) a.no(new Error(m.error)); else a.ok(m.result); }
  }
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
  svg.querySelectorAll(".sel").forEach(e => e.classList.remove("sel"));            // the box of the cursor
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
// Repair. A click on the result of a check (⪯, M ⊨ C, E ⊨ A, or a red A∧G)
// opens its panel by the tag: the named constants of the contracts of the
// check, which of them may change and within which range, and the repairs,
// found by chase-cps repair (run by the editor, or by chase-cps view
// --serve); each has passed the check itself. A repair pointed at shows on
// the diagram what it changes: the value struck out, the new one, and the
// tag as it would be. With two or more checks that fail, the tools list
// them all. A repair applied changes the declaration of the constant in the
// file; the checks that read it run again, and the repairs found before for
// other checks are marked out of date, found again when asked.
const REPAIRS = {};       // the check (by its key) -> { picks, ranges, options, running, error }
let panel = null;         // the panel open: { id, tag, nkey }
let previewing = null;    // a repair drawn on the diagram: { id, tag, values, from, owners, pinned }
const repairKey = r => r.check + " " + r.args.join(" ");
const repairSig = r => JSON.stringify(r.constants.map(c => [c.name, c.value]));
const backend = () => inEditor() ? "editor" : (location.protocol.startsWith("http") ? "serve" : null);
const tagOf = t => t.startsWith("⪯") ? "⪯" : t.startsWith("E ⊨ A") ? "E ⊨ A" : t;
function statusOf(n, t) {
  if (n.badge && tagOf(n.badge.text) === t) return n.badge.status;
  return ((n.checks || []).find(c => c.name === t) || {}).status;
}
function failing() {                 // every check that fails: its box, its tag
  const out = [];
  boxes.forEach(n => {
    const b = n.badge && tagOf(n.badge.text);
    if (b && n.badge.status === "invalid") out.push([n, b]);
    (n.checks || []).forEach(c => { if (c.status === "invalid" && c.name !== b) out.push([n, c.name]); });
  });
  return out;
}
function defaultRange(c) {
  const v = Number(c.value), int = c.type === "int";
  if (v > 0) return [0, int ? Math.max(2 * v, v + 10) : 2 * v];
  if (v < 0) return [int ? Math.min(2 * v, v - 10) : 2 * v, 0];
  return [-10, 10];
}
function stateOf(r) {
  const k = repairKey(r);
  const s = REPAIRS[k] = REPAIRS[k] || { picks: {}, ranges: {}, options: [], running: null, error: null };
  r.constants.forEach(c => {
    if (!(c.name in s.picks)) s.picks[c.name] = c.role !== "requirement";   // a requirement: only when asked
    if (!(c.name in s.ranges)) s.ranges[c.name] = defaultRange(c);
  });
  return s;
}
// one element: h("div", { class: "x", onclick: f }, children...)
function h(tag, attrs, ...kids) {
  const e = document.createElement(tag);
  for (const k in attrs || {}) {
    const v = attrs[k];
    if (v === undefined || v === null || v === false) continue;
    if (k.startsWith("on")) e[k] = v;
    else if (k === "class") e.className = v;
    else if (k === "checked" || k === "disabled" || k === "value") e[k] = v;
    else e.setAttribute(k, v);
  }
  kids.flat().forEach(c => { if (c !== null && c !== undefined && c !== false)
    e.appendChild(typeof c === "string" || typeof c === "number" ? document.createTextNode(String(c)) : c); });
  return e;
}
// The title of a check, as parts: a name (of a part, a model, a contract: {b: name}) or plain text.
// richOf draws it with the names in bold; plainOf is its text (for a tooltip).
const B = name => ({ b: name });
const andOf = names => names.length < 2 ? [B(names[0])]
  : names.slice(0, -1).flatMap((x, i) => i ? [", ", B(x)] : [B(x)]).concat([" and ", B(names[names.length - 1])]);
const plainOf = parts => parts.map(x => typeof x === "string" ? x : x.b).join("");
const richOf = parts => parts.map(x => typeof x === "string" ? x : h("b", {}, x.b));
function checkTitle(n, t) {
  const st = statusOf(n, t);         // a check not run is not said as if it held
  return (st === "unknown" ? ["Not decided: "] : st === "valid" || st === "invalid" ? [] : ["Not checked: "]).concat(claimOf(n, t));
}
// the model of a check of an implementation, said as the reader knows it: the C function f, the state
// machine of X, the RTL of X; from the argument of a repair ("heater.c:thermostat", "path/arb.smv") or the box
function modelOf(n, arg) {
  const src = n.model && n.model.source, file = arg ? arg.split("/").pop() : "";
  if (src === "c" || /\.(c|h):/.test(file)) return ["The C function ", B(file ? file.split(":").pop() : n.model.name)];
  if (src === "rtl" || /\.s?v$/.test(file)) return ["The RTL of ", B(n.title)];
  return ["The state machine of ", B(n.title)];
}
// the claim of a check, said as it is: "Room and Thermostat do not refine Comfort" when it fails
function claimOf(n, t) {
  const no = statusOf(n, t) === "invalid", r = (n.repair || {})[t];
  const v = (yes, not) => no ? not : yes;
  const req = (n.contract && n.contract.name) || n.title;           // from the box: what is inside, its model
  const refines = (names, what) => andOf(names).concat([names.length > 1 ? v(" refine ", " do not refine ")
                                                                         : v(" refines ", " does not refine "), B(what)]);
  const meets = (model, what) => model.concat(what === null ? [v(" meets its contract", " does not meet its contract")]
                                                           : [v(" implements ", " does not implement "), B(what)]);
  const conflict = what => ["The terms of ", B(what), v(" can hold together", " conflict")];
  if (r && r.check === "refines") return refines(r.args[0].split("+"), r.args[1]);
  if (r && r.check === "impl") {
    const model = modelOf(n, r.args[0]);
    return meets(model, model[1].b === r.args[1] ? null : r.args[1]);
  }
  if (r && r.check === "env") return [B(r.args[0].split("/").pop()), v(" is an environment of ", " is not an environment of "), B(r.args[1])];
  if (r) return conflict(r.args[0]);
  if (t === "⪯") {
    // what is inside the box (the candidate in a hole, the parts of a decomposition), else its note
    const inner = (n.children || []).filter(c => c.kind !== "hole").map(c => c.title);
    const names = inner.length ? inner : (n.note ? n.note.split(" ⊗ ") : []);
    return names.length ? refines(names, req) : [B(n.title)];
  }
  if (t === "M ⊨ C" && n.model) {
    const model = modelOf(n, null);
    return n.model.name === req && n.model.source !== "c" ? [B(req), v(" meets its contract", " does not meet its contract")]
                                                          : meets(model, model[1].b === req ? null : req);
  }
  if (t === "safe" && n.model) return [B(n.model.name), v(" is safe code", " is not safe code")];
  if (t === "E ⊨ A") {                // whose environment: the contract its wires go to
    const e = (DATA.edges || []).find(x => x.from[0] === n.id), to = e && byId[e.to[0]];
    const of = to && ((to.contract && to.contract.name) || to.title);
    return [B(n.title), v(" is an environment", " is not an environment")].concat(of ? [" of ", B(of)] : []);
  }
  if (t === "A∧G") return conflict(req);
  if (t === "¬V") return [B(req), v(" is not vacuous", " is vacuous")];
  return [B(req), v(" is ", " is not ") + (WHAT[t] || t).split(":")[0].replace(/\s*\(.*\)$/, "")];
}
// The second line of a check in the list: where its counterexample fails first and with which values,
// how many terms are in conflict, the constants that can change; nothing when there is nothing to say.
function factsOf(n, t, r, w) {
  const out = [];
  const c = (n.checks || []).find(x => x.name === t);
  if (c && c.core && statusOf(n, t) === "invalid") {
    const k = (c.core.A || []).length + (c.core.G || []).length, many = k > 1;
    if (k) out.push(t === "R" ? k + (many ? " guarantees" : " guarantee") + " in conflict"
                  : ["A", "G", "A∧G"].includes(t) ? k + (many ? " terms" : " term") + " in conflict"
                  : k + (many ? " terms" : " term") + " involved");
  }
  const tr = w && w.trace;
  if (tr && tr.steps && tr.steps.length && statusOf(n, t) === "invalid") {
    // the values of the variables of the terms it breaks (else of the terms of the contract), two at most
    const terms = (w.violated || []).map(i => ((n.contract || {}).G || [])[i]).filter(Boolean);
    const text = (terms.length ? terms : [].concat((n.contract || {}).A || [], (n.contract || {}).G || [])).join(" ");
    const named = new Set((text.match(/[A-Za-z_]\w*/g) || []));
    const fmt = x => typeof x === "boolean" ? truth(x) : String(x);
    const vals = k => tr.vars.map((x, i) => [x, i]).filter(([x]) => named.has(x)).slice(0, 2)
                        .map(([x, i]) => x + " = " + fmt(tr.steps[k][i])).join(", ");
    const call = n.model && n.model.source === "c" && t === "M ⊨ C";       // one call of a C function
    const still = tr.steps.length === 1 && tr.loop === 0;                    // one state, forever
    const ever = wholeRun(n, w);
    const k = w.at;
    if (call) out.push(vals(0) ? "Fails for " + vals(0) : "");
    else if (still) out.push(vals(0) ? vals(0) + " at every step" : "");
    else if (!ever && k !== undefined && k !== null && tr.steps[k])
      out.push("Fails at " + (DATA.period ? "t = " + timeOf(k) + " s" : "step " + k) + (vals(k) ? " (" + vals(k) + ")" : ""));
  }
  if (r && r.constants.length) out.push("Repairable: " + r.constants.map(x => x.name).join(", "));
  if (!out.filter(Boolean).length && w && w.reason) out.push(reasonOf(w.reason).text);
  return out.filter(Boolean).join(". ");
}
// A term that asks for something to happen (F, U) fails on the whole run, its loop included, not at a step.
function wholeRun(n, w) {
  if (w && w.whole) return true;
  if (w && w.at !== undefined && w.at !== null) return false;     // a step settles it (F[0,6] p at 6)
  const terms = ((w && w.violated) || []).map(i => ((n.contract || {}).G || [])[i]).filter(Boolean);
  return terms.length > 0 && terms.every(x => /^\s*(G\s*)?F\b|^\s*F\[|\bU\b/.test(x));
}
// the tag of a check as the diagram draws it, for a panel
function tagSvg(t, status) {
  const svg = el("svg", { width: tagWidth(t) + 2, height: 18, class: "rp-tag" });
  tag(svg, 1, 1, t, status || null);
  return svg;
}
// The panel of the checks: docked on the right of the diagram. It shows the
// list of the checks that fail, or the details of the one picked (why it
// fails, the code of a C function, how to fix it), with a way back to the
// list. A click on the tag of a check picks it.
let inspector = { open: false, tab: "problems" };
// the view keeps its middle when the panel takes the right of the window, or gives it back
// the panel opened or closed: the diagram keeps its middle in the middle of the room left; a diagram
// seen whole that the panel would cut is fitted to the room left, and fitted again when it closes
function shiftView(sign) {
  const panelWidth = innerWidth > 820 ? 424 : 0;
  if (!cam || !panelWidth) return;
  const W = document.getElementById("view").clientWidth;
  const whole = w => cam.x >= -1 && cam.x + size0.w * cam.k <= w + 1;
  if (sign < 0 && whole(W) && !whole(W - panelWidth)) { inspector.fitted = true; return "fit"; }
  if (sign > 0 && inspector.fitted) { inspector.fitted = false; return "fit"; }
  cam.x += sign * panelWidth / 2; place();
}
function showInspector(tab) {
  const how = !inspector.open ? shiftView(-1) : null;
  inspector.open = true;
  if (tab) inspector.tab = tab;
  document.body.classList.add("insp");
  if (how === "fit") fit();
  document.getElementById("repairs").classList.add("on");     // pressed while the panel is open
  renderInspector();
  renderQueue();
}
function hideInspector() {
  closeWave();
  inspector.waved = false;
  const how = inspector.open ? shiftView(1) : null;
  inspector.open = false;
  document.body.classList.remove("insp");
  if (how === "fit") fit();
  document.getElementById("repairs").classList.remove("on");
  renderQueue();
  closeRepair();
}
async function openRepair(n, t) {
  // the waveform is of one check: that of the new one, if it has one (also when it was open before Problems)
  const waved = WAVE.open || inspector.waved;
  inspector.waved = false;
  closeWave(waved);
  const keys = keysOf(DATA.nodes);
  // the counterexample opens at the step where a term fails first, when it has one
  const w0 = n.badge && tagOf(n.badge.text) === t ? n.badge.why : ((n.checks || []).find(c => c.name === t) || {}).why;
  panel = { id: n.id, tag: t, nkey: keys[n.id], step: (w0 && w0.at) || 0 };
  if (w0 && w0.parts) Object.keys(w0.parts).forEach(id => open.add("c:" + id));   // the parts whose assumptions break
  const ci = (n.checks || []).findIndex(c => c.name === t && c.core);               // a core: shown, its lines unfolded
  if (ci >= 0) { focus[n.id] = ci; open.add((n.checks[ci].core.part === "composed" ? "k:" : "c:") + n.id); }
  inspector.tab = "details";
  showInspector();
  const r0 = (n.repair || {})[t];      // the boxes that declare the constants, and the box of the check, open
  const owners = new Set(r0 ? r0.constants.flatMap(c => c.owners.map(o => o.contract)) : []);
  if (n.contract) open.add("c:" + n.id);
  (function walk(b) { if (b.contract && owners.has(b.contract.name)) open.add("c:" + b.id);
                      (b.children || []).forEach(walk); })(n);
  await render();
  renderInspector();
  document.getElementById("insp").focus({ preventScroll: true });    // the arrow keys: the steps of its counterexample
  if (waved && panel && panel.wave) panel.wave(); else if (waved) viewAboveWave(false);
  bringInto(n);
  if (r0 && backend() && statusOf(n, t) === "invalid") {   // found at once: the constants that may change already
    const s = stateOf(r0);
    if (!s.running && !s.options.some(o => o.sig === repairSig(r0)) && r0.constants.some(c => s.picks[c.name]))
      findRepairs(n, t);
  }
}
function closeRepair() {
  inspector.waved = inspector.open && WAVE.open;   // back to Problems: the next problem opens with its waveform
  closeWave();
  panel = null;
  clearPreview(true);
  clearReplay();
  markPick();
  if (inspector.open) { inspector.tab = "problems"; renderInspector(); }
  render();
}
// after a new drawing: the panel stays on the box that says the same
function rebindPanel() {
  previewing = null;
  if (!panel) return;
  const keys = keysOf(DATA.nodes), id = Object.keys(keys).find(i => keys[i] === panel.nkey);
  if (!id || !byId[id]) { panel = null; if (inspector.open) inspector.tab = "problems"; return; }
  panel.id = id;
}
function renderInspector() {
  if (!inspector.open) return;
  const list = failing(), n = panel && byId[panel.id];
  if (inspector.tab === "details" && !n) inspector.tab = "problems";
  const bar = document.getElementById("insp-bar");
  bar.innerHTML = "";
  if (inspector.tab === "details")
    bar.append(h("button", { class: "back", title: "Back to the checks that fail (Esc)", onclick: closeRepair },
      svgSpan("ib", '<path d="M13 8H3M7 4L3 8l4 4"/>'), "Problems"));
  else bar.append("Problems", list.length ? h("span", { class: "count" }, String(list.length)) : "");
  if (inspector.tab === "problems") fillProblems(list);
  else fillPanel();
  if (inspector.tab === "details" && panel && panel.wave) {   // the waveform: at the right of the line of the panel
    const b = iconButton(GLYPH.wave, "Open the waveform of every step", panel.wave, "wf-open");
    b.appendChild(h("span", { class: "wf-word" }, "Waveform"));
    bar.append(b);
  }
}
const ROLE = { part: "part", requirement: "requirement", contract: "contract" };
// the panel drawn again keeps in the window: its height changes as repairs come
function renderPanel() { if (inspector.open && inspector.tab === "details") renderInspector(); }
function fillPanel() {
  const box = document.getElementById("repair");
  const n = panel && byId[panel.id];
  box.innerHTML = "";
  panel.wave = null;
  if (!n) return;
  const t = panel.tag, r = (n.repair || {})[t], st = statusOf(n, t), how = backend();
  box.appendChild(h("div", { class: "rp-head" },
    h("span", { class: "q-tag" }, tagSvg(t, st)), h("span", { class: "t" }, richOf(checkTitle(n, t)))));
  const unsure = st === "unknown" ? ((n.badge && tagOf(n.badge.text) === t ? n.badge.reason
                                     : ((n.checks || []).find(c => c.name === t) || {}).reason) || "") : "";
  const says = st === "valid" ? "Holds." : st === "invalid" ? null
             : st === "unknown" ? "Not decided" + (unsure ? ": " + reasonOf(unsure).text + (reasonOf(unsure).line ? ", line " + reasonOf(unsure).line : "") : ".")
             : "Not checked yet." + (n.clash ? " The parts do not agree: " + Object.values(n.clash).join("; ")
                                                + ". Their composition is not defined until this is fixed." : "");
  if (says) box.appendChild(h("div", { class: "rp-sec" }, h("div", { class: "rp-says" }, says)));
  const why = whySection(n, t, st);
  if (why) box.appendChild(why);
  const code = codeSection(n);
  if (code) box.appendChild(code);
  if (st !== "invalid") return;          // nothing to fix
  if (!r) { box.appendChild(noRepairSection(n, t)); return; }
  const s = stateOf(r), sig = repairSig(r);
  // the constants
  const picked = r.constants.filter(c => s.picks[c.name]);
  const act = s.running
    ? iconButton(GLYPH.stop, "Stop the search", () => stopRepairs(r))
    : iconButton(s.options.length ? GLYPH.again : GLYPH.find,
                 !how ? "Repairs run in VS Code, or on a page of chase-cps view FILE --serve"
                 : !picked.length ? "Pick a constant first"
                 : s.options.length ? "Find again: the least change of the picked constants"
                 : "Find the least change of the picked constants",
                 () => findRepairs(n, t), "main", !how || !picked.length);
  const sec = h("div", { class: "rp-sec" }, h("div", { class: "rp-h rp-hb" },
    h("span", { title: "Pick the constants that may change, and their ranges" }, "Repair ", h("span", { style: "text-transform:none" }, "θ")),
    act));
  r.constants.forEach(c => {
    const [lo, hi] = s.ranges[c.name];
    const num = (v, i) => h("input", { type: "number", value: v, step: c.type === "int" ? 1 : "any", "aria-label": (i ? "high" : "low") + " " + c.name,
      onchange: e => { const x = Number(e.target.value); if (Number.isFinite(x)) s.ranges[c.name][i] = x; renderQueue(); } });
    sec.appendChild(h("div", { class: "rp-row" },
      h("input", { type: "checkbox", checked: !!s.picks[c.name], "aria-label": "may change " + c.name,
                   onchange: e => { s.picks[c.name] = e.target.checked; renderPanel(); } }),
      h("span", { class: "rp-name" }, c.name + " = " + c.value),
      h("span", { class: "rp-range" }, num(lo, 0), "..", num(hi, 1)),
      h("span", {}),
      h("span", { class: "rp-who" }, (c.type === "int" ? "int" : "real") + " · " + c.owners.map(o => o.contract).join(", "),
        c.role === "requirement" ? h("span", { class: "rp-role " + c.role, title: "Changed only with your consent" }, ROLE[c.role]) : null)));
  });
  if (s.running) sec.appendChild(h("div", { class: "rp-run", style: "margin-top:8px" }, h("span", { class: "rp-spin" }),
    h("span", { class: "rp-says" }, "Searching… " + s.running.done + " / " + s.running.of)));
  else if (!picked.length) sec.appendChild(h("div", { class: "rp-says", style: "margin-top:8px" }, "Pick a constant."));
  box.appendChild(sec);
  // the repairs
  if (s.options.length || s.error) {
    const out = h("div", { class: "rp-sec" }, h("div", { class: "rp-h",
      title: "Each value has passed the check itself. Point at one to see it on the diagram." }, "Repairs"));
    const seen = new Set();               // all together may give what one alone gave: shown once
    s.options.forEach(o => { const k = changedKey(o); if (k && seen.has(k)) return; if (k) seen.add(k);
                             out.appendChild(optionCard(n, t, r, s, o, sig)); });
    if (s.error) out.appendChild(h("div", { class: "rp-err" }, s.error));
    box.appendChild(out);
  }
  if (!how) box.appendChild(h("div", { class: "rp-sec" },
    h("div", { class: "rp-note", style: "margin-top:0", title: "This page cannot run repairs: open the file in VS Code, or draw it with chase-cps view FILE --serve" }, "Repairs run in VS Code, or:"),
    h("div", { class: "rp-cmd" }, words(commandOf(r, s)))));
}
// Why the check fails: in one line, then the counterexample (and its steps on the diagram).
function whySection(n, t, st) {
  if (st !== "invalid") return null;
  const sec = h("div", { class: "rp-sec" }, h("div", { class: "rp-h" }, "Counterexample"));
  if (t !== "⪯" && t !== "M ⊨ C" && t !== "safe" && t !== "E ⊨ A") {
    const c = (n.checks || []).find(x => x.name === t);
    if (!(c && c.core)) return null;
    sec.firstChild.textContent = "Why it fails";
    const src = n[c.core.part] || {};
    const list = [].concat((c.core.A || []).map(i => ["A", i, (src.A || [])[i]]), (c.core.G || []).map(i => ["G", i, (src.G || [])[i]]))
                   .filter(x => x[2]);
    // the line of each term in the file: terms cut short alike still tell apart
    const lineAt = (p, i) => { const at = c.core.part === "contract" && n.source && (n.source[p] || [])[i];
                               return at ? at[0] : null; };
    // terms that begin alike (a long computed contract): each also shows where it differs from the others
    const apart = f => {
      const same = Math.max(0, ...list.map(x => x[2]).filter(g => g !== f).map(g => { let k = 0;
                     while (k < f.length && f[k] === g[k]) k++; return k; }));
      if (same < 60) return null;
      const at = f.lastIndexOf(" ", Math.max(0, same - 12));
      return f.slice(at < 0 ? same : at + 1);
    };
    const nA = list.filter(x => x[0] === "A").length, nG = list.length - nA;
    const these = (k, one, many) => k === 1 ? "This " + one : "These " + k + " " + many;
    const all = these(list.length, nA && nG ? "term" : nA ? "assumption" : "guarantee", nA && nG ? "terms" : nA ? "assumptions" : "guarantees");
    // what the core says, for the kind of check: no strategy (R), terms false together (consistency),
    // assumptions the component's outputs can make false (WS)
    const together = list.length === 1 ? "" : list.length === 2 ? " both" : " all";
    const say = t === "R" ? "No component can meet " + all.toLowerCase().replace(/^these \d+ /, list.length === 2 ? "both " : "these " + list.length + " ")
                            + " for every input that the assumptions allow: the environment can always make one fail."
      : t === "¬V" ? (nA ? these(nA, "assumption makes ", "assumptions make ") + (nG === 1 ? "a guarantee" : nG + " guarantees") + " vacuous."
                         : these(nG, "guarantee is", "guarantees are") + " vacuous.")
      : t === "WS" ? "With " + all.toLowerCase() + ", the component can make an assumption false by its outputs, and so meet the contract without its guarantees."
      : all + (list.length === 1 ? " cannot hold." : " cannot" + together + " hold together.");
    sec.appendChild(h("div", {}, say));
    sec.appendChild(h("div", { class: "rp-terms" }, list.map(([p, i, f]) =>
      h("button", { class: "rp-term", title: "Show it on the diagram", onclick: () => showTerm(n, c.core.part + ":" + p + i) },
        h("span", { class: "st" }, p),
        h("span", { class: "fs" }, h("span", { class: "f" + (apart(f) ? " cut" : "") }, f),
          apart(f) ? h("span", { class: "d" }, "… " + apart(f)) : null),
        lineAt(p, i) ? h("span", { class: "ln" }, "line " + lineAt(p, i)) : null))));
    return sec;
  }
  const w = n.badge && n.badge.why, req = (n.contract && n.contract.name) || n.title;
  const code = n.model && n.model.source === "c";
  // said only when the table does not: which side fails
  const whole = w && w.trace && wholeRun(n, w);
  const looped = w && w.trace && w.trace.loop !== null && w.trace.loop !== undefined;
  const lead = w && w.failed === "assumptions"
    ? req + " allows this input, and the parts do not accept it."
    : whole ? "No single step breaks " + req + ": the whole run does" + (looped ? ", its loop repeated forever." : ".") : null;
  if (w && w.trace) {
    const trace = whole ? { ...w.trace, at: null, whole: true } : w.trace;
    const terms = n.contract ? [].concat(n.contract.A || [], n.contract.G || [], n.contract.E || []).join(" ") : "";
    const named = new Set((terms.match(/[A-Za-z_]\w*/g) || []).filter(x => !/^(G|F|X|U|R|W|true|false)$/.test(x)));
    const last = trace.steps.length - 1;
    // the table picks the step on the diagram: a click on a column, or the arrow keys
    const go = k => { k = Math.max(0, Math.min(last, k)); panel.step = k; replay(n, trace, k); table.setStep(k);
                      if (WAVE.open) WAVE.setStep(k); };
    const first = trace.vars.filter(v => named.has(v));
    // one call of a C function: its inputs and outputs, not a state repeated forever
    const shown = code && last === 0 ? { ...trace, loop: null } : trace;
    const table = traceTable(shown, first, req, last > 0 ? panel.step || 0 : null, go, n);
    panel.goStep = last > 0 ? d => go((panel.step || 0) + d) : null;
    // the waveform: every step kept, in a drawer under the diagram; its button on the line of the panel
    if (last > 0) panel.wave = () => openWave(trace, first, checkTitle(n, t), go, req);
    if (table.nav) {                    // on the line of the heading: the page back and on
      const head = sec.firstChild;
      head.classList.add("rp-hb");
      head.textContent = "";
      head.append(h("span", {}, "Counterexample"), table.nav);
    }
    sec.appendChild(table);
    // what the table does not say, under it: the table stays right under its heading in every problem
    if (lead) sec.appendChild(h("div", { class: "rp-lead" }, lead));
    if (w.broken && w.broken.length)  // the core of a refinement that fails on the assumptions
      sec.appendChild(h("div", { class: "rp-broken" }, h("div", {}, t === "E ⊨ A" ? "The assumptions it breaks (red):"
                                                                              : "The assumptions of the parts it breaks (red):"),
        w.broken.map(b => h("div", { class: "code" }, b))));
  } else {
    sec.appendChild(h("div", { class: "rp-note", title: inEditor() ? "" : "chase-cps view FILE --check --no-results" },
                      "Not kept: " + (inEditor() ? "press ▶ to check again." : "run the check again.")));
  }
  const toCode = () => {                 // the code, unfolded, at the line
    const d = document.querySelector("#repair details.rp-code"), at = d && d.querySelector('[data-line="' + why.line + '"]');
    if (!d) return;
    d.open = true; panel.code = true;
    d.querySelectorAll(".hit").forEach(x => x.classList.remove("hit"));
    if (at) at.classList.add("hit");
    (at || d).scrollIntoView({ block: "center" });
  };
  const why = w && w.reason ? reasonOf(w.reason) : null;
  if (why) sec.appendChild(h("div", { class: "rp-note", title: w.reason }, why.text,
    why.line && code && n.model.code ? [", ", h("button", { class: "rp-link", onclick: toCode }, "line " + why.line)]
    : why.line ? ", line " + why.line : ""));
  return sec;
}
// The reason of a checker, short: "division by zero" and its line, without the file and the function.
function reasonOf(r) {
  const m = /^(?:[\w$]+: )?(.*?)\s*\((?:file \S+ )?line (\d+)[^)]*\)\s*$/.exec(r);
  return m ? { text: m[1].charAt(0).toUpperCase() + m[1].slice(1), line: +m[2] } : { text: r, line: null };
}
// A step as the page shows it: its number, or its time i·Δτ when the diagram has a period.
const timeOf = i => DATA.period ? String(+(i * Number(DATA.period)).toFixed(9)) : String(i);
// A counterexample: one column per step (one only for a check of one call), the ports of the box first.
// With a step (the one on the diagram): its column marked blue, the column of the step that fails
// first red; a click on a column picks its step, and the arrow keys the next one.
function traceTable(tr, first, req, step, onPick, io) {
  const fmt = v => v === null || v === undefined ? "?" : typeof v === "boolean" ? truth(v) : String(v);
  const order = tr.vars.map((_, i) => i).sort((a, b) => (first.includes(tr.vars[b]) - first.includes(tr.vars[a])) || a - b);
  // one call of a function: name = value; one state repeated forever is a table, with its ↺
  const one = tr.steps.length === 1 && (tr.loop === null || tr.loop === undefined);
  if (one) {                            // one call: name = value, in rows that wrap, its inputs and its outputs apart
    const val = i => h("span", { class: first.includes(tr.vars[i]) ? "port" : "" }, tr.vars[i] + " = " + fmt(tr.steps[0][i]));
    const ins = new Set((io && io.inputs) || []), outs = new Set((io && io.outputs) || []);
    const groups = [["In", order.filter(i => ins.has(tr.vars[i]))], ["Out", order.filter(i => outs.has(tr.vars[i]))],
                    ["", order.filter(i => !ins.has(tr.vars[i]) && !outs.has(tr.vars[i]))]].filter(g => g[1].length);
    const grid = groups.length > 1 || (groups[0] && groups[0][0])
      ? h("div", { class: "rp-io" }, groups.flatMap(([w, is]) => [h("span", { class: "w" }, w), h("div", { class: "rp-values" }, is.map(val))]))
      : h("div", { class: "rp-values" }, order.map(val));
    const notes = [tr.more ? tr.more + " more variables not kept." : null].filter(Boolean);
    return h("div", { class: "rp-tracewrap" }, grid, notes.length ? h("div", { class: "rp-note" }, notes.join(" ")) : null);
  }
  // every step kept, as the waveform: the columns scroll under the names, a page at a time
  const kept = tr.steps.length, keptCut = tr.cut;
  const t = h("table", { class: "rp-trace", title: tr.vars.length ? "⊤ true, ⊥ false" : "" });
  const cell = v => typeof v === "number" ? digits(v) : fmt(v);       // a column per step: short, the whole value in its tip
  const picks = step !== null && step !== undefined && onPick;
  const show = k => picks ? "Show " + (DATA.period ? "t = " + timeOf(k) : "step " + k) + " on the diagram" : "";
  if (!one) t.appendChild(h("tr", {}, h("th", { title: DATA.period ? "time: a step every " + DATA.period + " s" : "" }, DATA.period ? "t" : "step"),
    tr.steps.map((_, i) => h("th", { class: (tr.at === i ? "fail" : "") + (picks ? " k" : ""), "data-k": String(i),
                                     title: (tr.at === i ? "A term fails first at this step. " : "")
                                            + (tr.loop === i ? "The loop starts here. " : "") + show(i) },
                             (tr.loop === i ? "↺ " : "") + timeOf(i)))));
  const rows = order.map(i => h("tr", { class: first.includes(tr.vars[i]) ? "port" : "" },
    h("td", { class: "v" }, tr.vars[i]), tr.steps.map((st, k) => h("td", { class: (tr.at === k ? "fail" : "") + (picks ? " k" : "")
                                                                                    + (st[i] === null || st[i] === undefined ? " unk" : ""),
                                                                           "data-k": String(k),
                                                                           title: [cell(st[i]) !== fmt(st[i]) ? tr.vars[i] + " = " + fmt(st[i]) : "", show(k)].filter(Boolean).join(" · ") },
                                                                     cell(st[i])))));
  const LIMIT = one ? 24 : 12;
  // the columns of the steps all as wide as the widest value or step of the rows shown
  const widest = Math.max(1, ...tr.steps.map((st, k) => Math.max(String(timeOf(k)).length,
                                                                 ...order.slice(0, LIMIT).map(i => String(cell(st[i])).length))));
  t.style.setProperty("--cw", widest + "ch");
  rows.slice(0, LIMIT).forEach(r => t.appendChild(r));
  const wrap = h("div", { class: "rp-tracewrap" }, t);
  // the table scrolls under its first column; above it, a page back or on
  // a column under the names is never cut in two: the scroll stops at the edges of the columns
  const snap = x => { const ks = t.querySelectorAll("th[data-k]"), n = ks.length - 1;    // the width of a column, exact
                      const w = n > 0 ? (ks[n].getBoundingClientRect().left - ks[0].getBoundingClientRect().left) / n : 0;
                      return w > 0 ? Math.round(x / w) * w : x; };
  const turn = d => { const v = t.querySelector("th, td.v"), c = t.querySelector('th[data-k="0"]');
                      const w = c ? c.offsetWidth : 40, room = wrap.clientWidth - (v ? v.offsetWidth : 0);
                      wrap.scrollLeft = snap(wrap.scrollLeft + d * Math.max(1, Math.floor(room / w)) * w); };
  const prev = iconButton(GLYPH.prev, "Earlier steps", () => turn(-1)), next = iconButton(GLYPH.next, "Later steps", () => turn(1));
  const top = h("div", { class: "rp-trace-top" }, prev, next);
  // the bar under the table: its thumb is the part in view; drag it, or click the bar to page
  const thumb = h("div", {}), bar = h("div", { class: "rp-bar" }, thumb);
  const out = h("div", { class: "rp-tracebox" }, top, wrap, bar);
  out.nav = top;                        // the caller may put it on the line of its heading
  bar.addEventListener("mousedown", e => {
    if (e.target === thumb) return;
    turn(e.offsetX < thumb.offsetLeft ? -1 : 1);
  });
  thumb.addEventListener("mousedown", e => {
    e.preventDefault(); e.stopPropagation();
    const x0 = e.clientX, s0 = wrap.scrollLeft, k = wrap.scrollWidth / bar.clientWidth;
    thumb.classList.add("drag");
    const move = m => { wrap.scrollLeft = s0 + (m.clientX - x0) * k; };
    const up = () => { thumb.classList.remove("drag"); removeEventListener("mousemove", move); removeEventListener("mouseup", up); };
    addEventListener("mousemove", move); addEventListener("mouseup", up);
  });
  const last = tr.steps.length - 1;
  const where = () => {
    const over = wrap.scrollWidth > wrap.clientWidth + 1;
    top.classList.toggle("on", over);
    bar.classList.toggle("on", over);
    if (!over) return;
    const w = bar.clientWidth;
    thumb.style.width = Math.max(24, w * wrap.clientWidth / wrap.scrollWidth) + "px";
    thumb.style.left = (w - thumb.offsetWidth) * wrap.scrollLeft / Math.max(1, wrap.scrollWidth - wrap.clientWidth) + "px";
    prev.style.visibility = wrap.scrollLeft > 1 ? "visible" : "hidden";
    next.style.visibility = wrap.scrollLeft + wrap.clientWidth < wrap.scrollWidth - 1 ? "visible" : "hidden";
  };
  wrap.addEventListener("scroll", where);
  if (typeof ResizeObserver === "function") new ResizeObserver(where).observe(wrap);
  requestAnimationFrame(where);
  if (rows.length > LIMIT) {
    const more = h("button", { class: "rp-link", onclick: () => { rows.slice(LIMIT).forEach(r => t.appendChild(r)); more.remove(); where(); } },
                   (rows.length - LIMIT) + " more variables");
    out.appendChild(more);
  }
  const notes = [keptCut ? "The first " + kept + " steps are kept." : null,
                 tr.loop !== null && tr.loop !== undefined
                   ? "↺ " + timeOf(tr.loop) + ": from this step on, the steps repeat forever." : null,
                 tr.more ? tr.more + " more variables not kept." : null].filter(Boolean);
  if (notes.length) out.appendChild(h("div", { class: "rp-note" }, notes.join(" ")));
  if (!picks) { out.setStep = () => {}; return out; }
  const column = k => t.querySelectorAll('[data-k="' + k + '"]');
  t.addEventListener("click", e => { const c = e.target.closest("[data-k]"); if (c) onPick(+c.dataset.k); });
  t.addEventListener("mouseover", e => { const c = e.target.closest("[data-k]");
    t.querySelectorAll(".hov").forEach(x => x.classList.remove("hov")); if (c) column(c.dataset.k).forEach(x => x.classList.add("hov")); });
  t.addEventListener("mouseleave", () => t.querySelectorAll(".hov").forEach(x => x.classList.remove("hov")));
  // the step on the diagram is marked only when it is not the one shown at first (the step that fails)
  const base = tr.at !== null && tr.at !== undefined ? tr.at : step;
  out.setStep = k => {
    t.querySelectorAll(".cur").forEach(x => x.classList.remove("cur"));
    if (k !== base) column(k).forEach(x => x.classList.add("cur"));
    const th = t.querySelector('th[data-k="' + k + '"]'), v = t.querySelector("th");   // the column in view
    if (th) {                           // the next column: just enough; one further away: in the middle
      const names = v ? v.offsetWidth : 0, left = th.offsetLeft - names, right = th.offsetLeft + th.offsetWidth;
      const far = left < wrap.scrollLeft - th.offsetWidth || right > wrap.scrollLeft + wrap.clientWidth + th.offsetWidth;
      if (far) wrap.scrollLeft = snap(left - (wrap.clientWidth - names - th.offsetWidth) / 2);
      else if (left < wrap.scrollLeft) wrap.scrollLeft = left;
      else if (right > wrap.scrollLeft + wrap.clientWidth) wrap.scrollLeft = right - wrap.clientWidth;
    }
    where();
  };
  requestAnimationFrame(() => out.setStep(step));       // once in the page: the scroll needs its width
  out.setStep(step);
  return out;
}
// A check that fails with no constant to repair, or a page that cannot run repairs: in a few points.
function noRepairSection(n, t) {
  const sec = h("div", { class: "rp-sec" }, h("div", { class: "rp-h" }, "How to fix"));
  const clauses = n.model && n.model.source === "c" && n.model.name === ((n.contract || {}).name);
  const code = x => h("code", { class: "cmd" }, x);
  // a number of the terms (not a step of an interval, F[0,6]) can be repaired once it is named
  const terms = [].concat((n.contract || {}).A || [], (n.contract || {}).G || []).join(" ").replace(/\[[^\]]*\]/g, "");
  const named = /(^|[^\w'])\d+(\.\d+)?\b/.test(terms)
    ? ["To repair a number of the terms, give it a name in the file, as ", code("integer constant k 2;"), ", and write k in the term."] : null;
  // the constants that can change, and whose they are: "theta (Thermostat), tmax (Comfort)"
  const r0 = (n.repair || {})[t];
  const owned = r0 && r0.constants && r0.constants.length
    ? r0.constants.map(c => c.name + " (" + [...new Set(c.owners.map(o => o.contract))].join(", ") + ")").join(", ")
    : ((n.contract || {}).K || []).map(x => x.split(" = ")[0]).join(", ");
  const holed = (n.children || []).some(c => c.kind === "hole") || n.kind === "hole";
  const of = t === "E ⊨ A" ? (() => { const e = (DATA.edges || []).find(x => x.from[0] === n.id), to = e && byId[e.to[0]];
                                      return to ? (to.contract && to.contract.name) || to.title : null; })() : null;
  const points = t === "safe"
    ? ["Fix the code, or exclude such inputs with a requires clause."]
    : t === "M ⊨ C" && clauses && !(n.contract || {}).K
    ? ["Fix the code, or change its requires / ensures clauses."]
    : n.contract && n.contract.K
    ? [["Repair " + owned + " in VS Code, or with ", code("chase-cps view FILE --serve"), "."]]
    : [{ "⪯": "Change a term of a part or of the requirement.",
         "E ⊨ A": "Change the environment, or weaken the assumptions" + (of ? " of " + of : "") + ".",
         "M ⊨ C": "Change the model, or its contract.",
         "A": "Weaken or drop one of the red assumptions.",
         "G": "Weaken or drop one of the red terms.",
         "A∧G": "Weaken or drop one of the red terms.",
         "R": "Weaken a red guarantee, or assume more of the environment.",
         "WS": "Change the red terms so that the outputs of the component cannot make an assumption false.",
         "¬V": "Ask something of the component in the red guarantees." }[t] || "Change one of the red terms.", named]
       .concat(t === "⪯" && holed ? ["Or put in the hole a part that refines the quotient: what the missing part must guarantee."] : [])
       .filter(Boolean);
  sec.appendChild(h("ul", { class: "rp-why" }, points.map(x => h("li", {}, x))));
  return sec;
}
// which constants a repair changes, as the first line of its card: "Change r and vmax together"
function changeLabel(names) {
  const out = h("div", { class: "rp-l" }, "Change ");
  names.forEach((k, i) => { if (i) out.append(i === names.length - 1 ? " and " : ", "); out.append(h("b", {}, k)); });
  if (names.length > 1) out.append(" together");
  return out;
}
// the time a repair was asked to take: its own, or the time limit of the page (in the editor, the
// time it took when it stopped at the limit)
const limitOf = o => o.limit || (backend() === "serve" ? TIME_LIMIT : Math.max(1, Math.round(o.res.seconds || 0)));
function optionCard(n, t, r, s, o, sig) {
  const res = o.res, old = o.sig !== sig && !o.applied;
  if (o.running) return h("div", { class: "rp-opt" }, changeLabel(o.names),
    h("div", { class: "rp-run" }, h("span", { class: "rp-spin" }), h("span", { class: "rp-says" }, "Searching for up to " + o.running + " s…")));
  if (res.status !== "repaired") {
    const range = k => k + " in [" + o.ranges[k].join(", ") + "]";
    const rs = o.names.map(range), list = rs.length > 1 ? rs.slice(0, -1).join(", ") + " and " + rs[rs.length - 1] : rs[0];
    const timed = !res.reason || /time limit/.test(res.reason);
    const text = res.status === "holds" ? "Holds already."
      : res.status === "impossible" ? (o.names.length === 1 ? "No value of " + list + " works." : "No values of " + list + " work together.")
          + " Edit the range" + (o.names.length > 1 ? "s" : "") + " above to search further."
      : res.bound !== undefined && res.bound !== null ? "Stopped after " + limitOf(o) + " s: the least change is at least " + short(res.bound) + "."
      : timed ? "No answer within " + limitOf(o) + " s."
      : /optimization/.test(res.reason) ? "No answer: z3 did not finish the optimization."
      : "No answer: " + res.reason.split(";")[0].replace(/\.$/, "") + ".";
    // all together, undecided, when one alone has its least change already: shown faint
    const faint = o.names.length > 1 && res.status === "unknown"
      && s.options.some(p => p.names.length === 1 && p.res.status === "repaired" && p.res.optimal);
    const card = h("div", { class: "rp-opt " + res.status + (old ? " old" : "") + (faint ? " faint" : ""), title: res.reason || "" },
      changeLabel(o.names), h("div", { class: "rp-t" }, text), old ? h("div", { class: "rp-m" }, "Out of date") : null);
    if (res.status === "unknown" && timed) {
      const longer = 2 * limitOf(o);
      card.appendChild(h("div", { class: "rp-b" }, iconButton(GLYPH.longer,
        !backend() ? "Repairs run in VS Code, or on a page of chase-cps view FILE --serve"
        : old ? "Out of date: find the repairs again" : "Try longer: search again for up to " + longer + " s",
        () => retryOption(n, t, r, o, longer), "", !backend() || old || !!s.running)));
    }
    return card;
  }
  const v = h("div", { class: "rp-v" });
  const moved = o.names.filter(k => !same(o, k));
  (moved.length ? moved : o.names).forEach((k, i) => { if (i) v.appendChild(document.createTextNode("   "));
    v.append(k + " ", h("s", {}, o.from[k]), " → ", h("em", {}, res.text[k] === null ? "≈ " + short(res.values[k]) : res.text[k])); });
  const meta = [o.applied ? "Applied" : old ? "Out of date" : null, res.optimal ? "optimal" : "least found",
                "change " + short(res.cost), o.confirm ? "click ✓ again to change the requirement" : null].filter(Boolean).join(" · ");
  const how = res.method + (res.rounds ? ", " + res.rounds + (res.rounds === 1 ? " round" : " rounds") : "") + ", " + res.seconds + " s";
  const req = o.names.some(k => (r.constants.find(c => c.name === k) || {}).role === "requirement");
  const lines = o.names.every(k => (r.constants.find(c => c.name === k) || { owners: [] }).owners.every(w => w.line));
  const exact = o.names.every(k => res.text[k] !== null);
  const can = backend() && !old && lines && exact && !o.applied;
  const shown = previewing && previewing.pinned && previewing.option === o;
  const apply = iconButton(GLYPH.apply,
    o.applied ? "Applied: the value is in the file"
    : !backend() ? "Repairs are written by VS Code, or on a page of chase-cps view FILE --serve"
    : old ? "Out of date: find the repairs again"
    : !exact ? "This value has no finite decimal: write a nearby one by hand"
    : !lines ? "No declaration of the constant in the file"
    : o.confirm ? "Click again to change the requirement"
    : "Write the new value into the file and check again" + (req ? " (it changes a requirement)" : ""),
    () => applyOption(n, t, r, o), (can ? "main" : "") + (o.confirm ? " warn" : ""), !can);
  const preview = iconButton(GLYPH.eye, shown ? "Stop showing this change on the diagram"
                                             : "Show this change on the diagram (the file is not changed)",
    () => { if (shown) clearPreview(true); else { const p = previewOf(n, t, r, o); p.pinned = true; showPreview(p); }
            renderPanel(); }, shown ? "on" : "", old);
  const card = h("div", { class: "rp-opt repaired" + (old ? " old" : "") + (shown ? " shown" : ""),
                          onmouseenter: () => { if (!old) hoverPreview(previewOf(n, t, r, o)); },
                          onmouseleave: () => clearPreview(false) },
    changeLabel(o.names), v, h("div", { class: "rp-m", title: how }, meta),
    h("div", { class: "rp-b" }, preview, apply));
  return card;
}
const short = digits;
// the value of a repair is the one it had
const same = (o, k) => o.res.text[k] !== null && Number(o.res.text[k]) === Number(o.from[k]);
// what a repair changes, as a key: the same changes from two runs are one repair
function changedKey(o) {
  if (o.res.status !== "repaired") return null;
  return o.names.filter(k => !same(o, k)).map(k => k + "=" + o.res.text[k]).join(",");
}
function previewOf(n, t, r, o) {
  const owners = [];
  o.names.forEach(k => (r.constants.find(c => c.name === k) || { owners: [] }).owners.forEach(w => owners.push(w.contract)));
  const values = {};
  o.names.forEach(k => { values[k] = o.res.text[k] === null ? String(o.res.values[k]) : o.res.text[k]; });
  return { id: n.id, tag: t, values, from: o.from, owners, option: o, pinned: false };
}
// a command as its words, each kept whole on a line (with the spaces between them to copy)
const words = c => c.split(" ").flatMap((w, i) => i ? [" ", h("span", {}, w)] : [h("span", {}, w)]);
function commandOf(r, s) {
  const flag = { refines: "--refines", impl: "--impl", env: "--env", verify: "--verify", compat: "--compat", consist: "--consist" }[r.check];
  const q = a => { a = a.replace(/^.*\//, ""); return /^[\w.+:-]+$/.test(a) ? a : JSON.stringify(a); };   // run next to the file
  const ps = r.constants.filter(c => s.picks[c.name]).map(c => "--param " + c.name + "=" + s.ranges[c.name][0] + ".."
                                                       + s.ranges[c.name][1] + (c.type === "int" ? ":int" : ""));
  return ["chase-cps repair", q(DATA.title.split("/").pop()), flag, ...r.args.map(q), ...ps].join(" ");
}
// The repairs of the constants picked: each alone, then all together.
async function findRepairs(n, t) {
  const r = n.repair[t], s = stateOf(r);
  const names = r.constants.filter(c => s.picks[c.name]).map(c => c.name);
  if (!names.length || s.running) return;
  const runs = names.map(k => [k]);
  if (names.length > 1) runs.push(names);
  const sig = repairSig(r), from = {};
  r.constants.forEach(c => { from[c.name] = c.value; });
  const token = {};
  s.options = []; s.error = null; s.running = { done: 0, of: runs.length, token };
  clearPreview(true);
  renderPanel(); renderQueue();
  for (const ks of runs) {
    const params = ks.map(k => ({ name: k, low: s.ranges[k][0], high: s.ranges[k][1],
                                  type: r.constants.find(c => c.name === k).type }));
    const ranges = {}; ks.forEach(k => { ranges[k] = s.ranges[k].slice(); });
    try {
      const res = await askRepair({ check: r.check, args: r.args, params }, s);
      if (!s.running || s.running.token !== token) return;
      s.options.push({ names: ks, res, sig, from, ranges });
    } catch (e) {
      if (!s.running || s.running.token !== token) return;
      s.error = String(e && e.message || e);
      break;
    }
    s.running.done++;
    renderPanel(); renderQueue();
  }
  s.running = null;
  renderPanel(); renderQueue();
}
function stopRepairs(r) {
  const s = stateOf(r);
  if (s.cancel) s.cancel();
  s.running = null;
  s.options.forEach(o => { o.running = null; });
  renderPanel(); renderQueue();
}
// One repair asked again with a longer time: its card shows the new answer.
async function retryOption(n, t, r, o, limit) {
  const s = stateOf(r);
  if (s.running) return;
  const params = o.names.map(k => ({ name: k, low: o.ranges[k][0], high: o.ranges[k][1],
                                     type: r.constants.find(c => c.name === k).type }));
  const token = {};
  s.error = null; s.running = { done: 0, of: 1, token }; o.running = limit;
  renderPanel(); renderQueue();
  try {
    const res = await askRepair({ check: r.check, args: r.args, params, time_limit: limit }, s);
    if (!s.running || s.running.token !== token) return;
    o.res = res; o.limit = limit;
  } catch (e) {
    if (!s.running || s.running.token !== token) return;
    s.error = String(e && e.message || e);
  }
  o.running = null; s.running = null;
  renderPanel(); renderQueue();
}
let repairSeq = 0;
const TIME_LIMIT = 60;                   // seconds for a repair asked by the page (the editor has its setting)
const asked = {};
function askRepair(req, s) {
  const how = backend();
  if (how === "editor") {
    vscodeApi = vscodeApi || acquireVsCodeApi();
    const id = ++repairSeq;
    return new Promise((ok, no) => {
      asked[id] = { ok, no };
      s.cancel = () => { vscodeApi.postMessage({ type: "chase-repair-stop", id }); delete asked[id]; };
      vscodeApi.postMessage({ type: "chase-repair", id, request: Object.assign({ period: DATA.period }, req) });
    });
  }
  if (how === "serve") {
    const ctl = new AbortController();
    const id = ++repairSeq;
    s.cancel = () => { ctl.abort(); fetch("/repair-stop", { method: "POST", body: JSON.stringify({ id }),
      headers: { "Content-Type": "application/json", "X-CHASE": "1" } }).catch(() => {}); };
    return fetch("/repair", { method: "POST", headers: { "Content-Type": "application/json", "X-CHASE": "1" },
                              body: JSON.stringify(Object.assign({ id, time_limit: TIME_LIMIT, period: DATA.period }, req)), signal: ctl.signal })
      .then(x => x.json()).then(x => { if (x.error) throw new Error(x.error); return x.result; });
  }
  return Promise.reject(new Error("This page cannot run repairs."));
}
function applyOption(n, t, r, o) {
  const req = o.names.some(k => (r.constants.find(c => c.name === k) || {}).role === "requirement");
  if (req && !o.confirm) { o.confirm = true; renderPanel(); return; }    // a requirement: asked twice
  const edits = [];
  o.names.forEach(k => (r.constants.find(c => c.name === k) || { owners: [] }).owners
    .forEach(w => edits.push({ line: w.line, name: k, value: o.res.text[k] })));
  o.applied = true;
  clearPreview(true);
  const how = backend();
  if (how === "editor") {
    vscodeApi = vscodeApi || acquireVsCodeApi();
    vscodeApi.postMessage({ type: "chase-apply", edits });
  } else if (how === "serve") {
    fetch("/apply", { method: "POST", headers: { "Content-Type": "application/json", "X-CHASE": "1" },
                      body: JSON.stringify({ edits }) })
      .then(x => x.json()).then(x => { if (x.error) { o.applied = false; stateOf(r).error = x.error; renderPanel(); } });
  }
  renderPanel();
}
// What a repair changes, drawn on the diagram without laying it out again.
function showPreview(p) {
  clearPreview(true);
  previewing = p;
  document.getElementById("view").classList.add("previewing");   // what fails now, faded: the repair would fix it
  document.querySelectorAll("#view text[data-const]").forEach(x => {
    const k = x.getAttribute("data-const"), b = byId[x.getAttribute("data-cbox")];
    if (!(k in p.values) || !b || !b.contract || !p.owners.includes(b.contract.name)) return;
    x.setAttribute("data-was", x.textContent);
    x.textContent = k + " = ";
    el("tspan", { fill: "var(--bad)", "text-decoration": "line-through" }, x).textContent = p.from[k];
    el("tspan", { fill: "var(--ok)", "font-weight": "600" }, x).textContent = " → " + p.values[k];
  });
  const g = document.querySelector('#view [data-check="' + p.id + "|" + p.tag + '"]');
  if (g) {
    const r0 = g.querySelector("rect"), x = +r0.getAttribute("x"), y = +r0.getAttribute("y");
    const w = el("g", { class: "pv" }, g);
    tag(w, x, y, g.getAttribute("data-text"), null);
    w.querySelector("rect").setAttribute("stroke", "var(--ok)");
    w.querySelector("rect").setAttribute("stroke-dasharray", "3 2");
    w.querySelectorAll("path").forEach(q => q.setAttribute("stroke", "var(--ok)"));
    w.querySelectorAll("text").forEach(q => q.style.fill = "var(--ok)");
    tip(w, "would hold with this repair");
  }
}
function clearPreview(all) {
  // back to the one kept, unless all go
  const keep = !all && previewing && (previewing.pinned ? previewing : previewing.keep);
  document.querySelectorAll("#view text[data-was]").forEach(x => { x.textContent = x.getAttribute("data-was"); x.removeAttribute("data-was"); });
  document.querySelectorAll("#view .pv").forEach(x => x.remove());
  document.getElementById("view").classList.remove("previewing");
  previewing = null;
  if (keep) showPreview(keep);
}
// pointing at a repair shows it for a while; the one kept comes back after
function hoverPreview(p) {
  const kept = previewing && (previewing.pinned ? previewing : previewing.keep);
  if (kept && kept.option === p.option) return;
  p.keep = kept || null;
  showPreview(p);
}
function placePanel() {}
// the list of the checks that fail (all of them: with or without constants)
function fillProblems(list) {
  const box = document.getElementById("repair");
  box.innerHTML = "";
  if (!list.length) { box.appendChild(h("div", { class: "rp-sec rp-says" }, "No check fails.")); return; }
  list.forEach(([n, t]) => {
    const r = (n.repair || {})[t], w = n.badge && tagOf(n.badge.text) === t ? n.badge.why : null;
    const s = r && stateOf(r), sig = r && repairSig(r);
    const best = s && s.options.find(o => o.res.status === "repaired" && o.sig === sig);
    const what = s && s.running ? "Finding repairs…"
      : best ? "Repair: " + best.names.map(k => k + " " + best.from[k] + " → " + best.res.text[k]).join(", ")
      : factsOf(n, t, r, w);
    const title = checkTitle(n, t);
    box.appendChild(h("div", { class: "q-item" + (panel && panel.id === n.id && panel.tag === t ? " on" : ""),
                               title: plainOf(title) + (what ? "\n" + what : ""), onclick: () => openRepair(n, t) },
      h("div", { class: "q-tag" }, tagSvg(t, "invalid")),
      h("div", { class: "q-main" }, h("div", { class: "t" }, richOf(title)),
        what ? h("div", { class: best ? "v" : "s" }, what) : null)));
  });
}
// The waveform of a counterexample: a drawer under the diagram, beside the details, a row per variable
// and the steps as columns, as the waveform viewers draw a simulation. The names and the values at the
// cursor stay on the left; the steps scroll and zoom. The step that fails first is a red column, the
// loop a shaded span with one more pass drawn faded, the current step a blue cursor that the table and
// the diagram follow (and the other way round).
const WAVE = { open: false, height: null };
const WV = { axis: 54, band: 33, row: 24, bool: 24, plot: 56, group: 18, min: 6, max: 240 };
function openWave(tr, first, title, pick, reqName) {
  closeWave();
  const box = document.getElementById("wave"), n = tr.steps.length;
  const loop = tr.loop === null || tr.loop === undefined ? null : tr.loop;
  const ghost = loop === null ? 0 : n - loop;           // one more pass of the loop, faded
  const cols = n + ghost;
  const fails = tr.at !== undefined && tr.at !== null;
  const req = i => first.includes(tr.vars[i]);
  const order = tr.vars.map((_, i) => i).sort((a, b) => (req(b) - req(a)) || a - b);
  const groups = order.some(req) && !order.every(req);  // the variables of the requirement, then the others
  const kind = i => tr.steps.every(st => st[i] === null || st[i] === undefined || typeof st[i] === "boolean") ? "bool"
    : tr.steps.every(st => st[i] === null || st[i] === undefined || typeof st[i] === "number") ? "num" : "text";
  const S = { cur: panel && panel.step !== undefined && panel.step !== null ? panel.step : (tr.at || 0), px: 32,
              plots: new Set(), sel: order.length ? order[0] : null, fit: false, center: false };
  const mac = /Mac|iPhone|iPad/.test(navigator.platform || navigator.userAgent);
  const real = c => c < n ? c : loop + (c - n);         // the step of a column (the faded pass: the loop again)
  const valueAt = (c, i) => tr.steps[real(c)][i];
  const fmt = v => v === null || v === undefined ? "?" : typeof v === "boolean" ? truth(v) : String(v);
  const short = v => typeof v === "number" ? digits(v) : fmt(v);
  const label = k => DATA.period ? timeOf(k) : String(k);
  const at = k => DATA.period ? "t = " + label(k) + " s" : "step " + label(k);
  const NS = "http://www.w3.org/2000/svg";
  const sv = (tag, attrs, parent) => { const e = document.createElementNS(NS, tag);
    Object.entries(attrs || {}).forEach(([a, v]) => e.setAttribute(a, v)); if (parent) parent.appendChild(e); return e; };

  // the widths on the left: the longest name (and room for the plot button), the widest value
  const MONO = "11px Menlo, Monaco, monospace";
  const nameW = Math.max(72, Math.min(220, Math.ceil(Math.max(0, ...order.map(i => textWidth(tr.vars[i], (req(i) ? "600 " : "") + MONO)))) + 24));
  const seen = new Set([].concat(...tr.steps.map(st => order.map(i => fmt(st[i])))));
  const corner = k => DATA.period ? label(k) + " s" : label(k);
  const valW = Math.max(48, Math.min(120, Math.ceil(Math.max(textWidth("value at " + corner(n - 1), "10.5px Helvetica, Arial, sans-serif"),
                                                              ...[...seen].map(v => textWidth(v, MONO)))) + 16));
  const L = 12 + nameW + valW;

  // the bar: the check, the step at the cursor, the step that fails, the zoom, the keys, the cross
  const where = h("span", { class: "wv-where" });
  const failAt = fails ? h("button", { class: "wv-failat", "data-tip": "Go to the step that fails (F)", onclick: () => jump(tr.at) },
                           "Fails at " + (DATA.period ? "t = " + label(tr.at) + " s" : label(tr.at))) : null;
  const keys = h("div", { class: "wv-keys" });
  const kb = iconButton(GLYPH.keys, "Keys (?)", () => toggleKeys());
  const zout = iconButton(GLYPH.zout, "Zoom out (-)", () => zoom(1 / 1.5)), zin = iconButton(GLYPH.zin, "Zoom in (+)", () => zoom(1.5));
  // in a narrow window the drawer takes the place of the details: its arrow goes back to the problems, as theirs
  const back = h("button", { class: "back wv-back", title: "Back to the checks that fail", onclick: () => closeRepair() },
                 svgSpan("ib", '<path d="M13 8H3M7 4L3 8l4 4"/>'), "Problems");
  const bar = h("header", {}, back, h("b", {}, richOf(title)), where, failAt,
    h("span", { class: "wv-tools" },
      zout, zin,
      iconButton(GLYPH.fit, "Show all steps (0)", fitAll),
      kb,
      iconButton('<path d="M4 4l8 8M12 4l-8 8"/>', "Close (Esc)", closeWave)));
  bar.addEventListener("click", () => { if (WAVE.open) box.focus({ preventScroll: true }); });   // the keys stay with the drawer
  const grip = h("div", { class: "wv-grip" });
  const scroll = h("div", { class: "wv-scroll" });
  const grid = h("div", { class: "wv-grid" });
  scroll.appendChild(grid);
  const thumb = h("div", {}), sbar = h("div", { class: "rp-bar wv-bar", style: "margin-left:" + L + "px" }, thumb);
  const step1 = k => DATA.period ? "The step at t = " + label(k) + " s" : "Step " + label(k);
  const steps2 = (a, b) => DATA.period ? "The steps at t = " + label(a) + " to " + label(b) + " s" : "Steps " + label(a) + " to " + label(b);
  const again = loop === null ? "" : "↺ " + (loop === 0 ? (n === 1 ? "The step repeats" : "All " + n + " steps repeat")
    : loop === n - 1 ? step1(loop) + " repeats" : steps2(loop, n - 1) + " repeat") + " forever; the faded "
    + (ghost === 1 ? "column is its" : "columns are their") + " next pass. ";
  const foot = h("footer", {}, h("span", { class: "wv-span" }),
    h("span", { class: "wv-note" }, (tr.whole ? "No single step breaks " + reqName + ": the whole run does. " : "")
                                    + again + (tr.cut ? "The first " + n + " steps are kept. " : "")),
    h("span", { class: "wv-hint" }));
  const fade = h("div", { class: "wv-fade" });     // over the last row in view, when there are more rows under it
  // the keys, as keycaps ("~" a word between them), those of a Mac on a Mac
  keys.append(h("div", { class: "wv-keys-t" }, "Keys of the waveform"));
  [[["←", "→"], "The step before or after"],
   [[mac ? "⌥" : "Alt", "~+", "←", "→", "~or", "[", "]"], "The previous or next change of the selected row"],
   [["Home", "End"], "The first or the last step"], fails ? [["F"], "The step that fails"] : null,
   loop !== null ? [["L"], "The first step of the loop"] : null, [["↑", "↓"], "The row above or below"],
   [["Enter"], "A row of numbers as a line, or as values again"], [["+", "−", "0"], "Zoom in, zoom out, show all steps"],
   [[mac ? "⌘" : "Ctrl", "~+ wheel"], "Zoom where the pointer is"], [["~Drag, or", "Shift", "~+ wheel"], "Pan along the steps"],
   [["Esc"], "Close"]]
    .filter(Boolean).forEach(([ks, d]) => keys.append(
      h("span", { class: "wv-caps" }, ks.map(k => k[0] === "~" ? h("span", {}, k.slice(1)) : h("kbd", {}, k))), h("span", {}, d)));
  const edgeL = h("div", { class: "wv-edge" }), edgeR = h("div", { class: "wv-edge r" });   // the markers out of view
  box.innerHTML = "";
  box.append(grip, bar, keys, scroll, sbar, foot, fade, edgeL, edgeR);
  box.style.display = "flex";
  document.body.classList.add("wave");
  WAVE.open = true;
  WAVE.setStep = k => { S.cur = k; draw(); show(k, S.center); };
  // a jump (the step that fails, the loop, a marker at the edge, the view at first): the step brought
  // from out of view to the middle; a step to the next one stays as near as it can
  function jump(k) { S.center = true; try { pick(k); } finally { S.center = false; } }
  WAVE.px = () => S.px;                  // for the tests: the width of a step, the left part, the step at a point
  WAVE.left = () => L;
  WAVE.stepAt = x => { const c = colAt(x); return c === null ? null : real(c); };

  // the keys: under their button, or above it when there is no room under
  function toggleKeys(on) {
    on = on === undefined ? !keys.classList.contains("on") : on;
    keys.classList.toggle("on", on);
    if (!on) return;
    const r = kb.getBoundingClientRect(), w = keys.offsetWidth, hk = keys.offsetHeight;
    keys.style.left = Math.max(8, Math.min(r.right - w, innerWidth - w - 8)) + "px";
    keys.style.top = (r.bottom + 6 + hk > innerHeight - 8 ? Math.max(8, r.top - 6 - hk) : r.bottom + 6) + "px";
  }
  // the height: at most 45% of the window (or what the top edge was dragged to, kept for the next
  // drawer), never more than the rows, and cut between two rows, not through one
  function fitHeight() {
    const chrome = box.offsetHeight - scroll.offsetHeight;
    const room = Math.min(WAVE.height || 0.45 * innerHeight, innerHeight - 70) - chrome;
    const rows = grid.querySelectorAll(".wv-row");
    if (grid.offsetHeight <= room) { scroll.style.maxHeight = ""; return; }
    let fit = 0;
    rows.forEach((r, j) => { const b = r.offsetTop + r.offsetHeight; if (b <= room || j < 2) fit = Math.max(fit, b); });
    scroll.style.maxHeight = fit + "px";
  }
  // the top edge: drag it for a taller or shorter drawer, up to all its rows; a double-click for the height it had
  grip.addEventListener("pointerdown", e => {
    e.preventDefault();
    const y0 = e.clientY, h0 = box.offsetHeight, most = box.offsetHeight - scroll.offsetHeight + grid.offsetHeight;
    grip.classList.add("drag");
    const move = m => { WAVE.height = Math.max(Math.min(140, most), Math.min(innerHeight - 70, most, h0 + y0 - m.clientY)); fitHeight(); place(); };
    const up = () => { grip.classList.remove("drag"); removeEventListener("pointermove", move); removeEventListener("pointerup", up); };
    addEventListener("pointermove", move); addEventListener("pointerup", up);
  });
  grip.addEventListener("dblclick", () => { WAVE.height = null; fitHeight(); place(); });

  const avail = () => Math.max(120, scroll.clientWidth - L - 16);
  // the least width of a step: that of all the steps in the drawer (down to a pixel), or WV.min if more
  const least = () => Math.max(Math.min(1, WV.min), Math.min(WV.min, avail() / cols));
  // all the steps in the width; the drawer stays so ("fitted") as it gets wider or narrower, until a zoom or a pan
  function fitAll() { S.px = Math.max(least(), Math.min(WV.max, avail() / cols)); S.fit = true; draw(); scroll.scrollLeft = 0; }
  function zoom(f, atX) {                // around a point of the view (the pointer), or around the cursor
    const x = atX === undefined ? (S.cur + .5) * S.px - scroll.scrollLeft : atX;
    const a = (scroll.scrollLeft + x) / S.px;
    const px = Math.max(least(), Math.min(WV.max, S.px * f));
    if (px === S.px) return;
    S.px = px; S.fit = false;
    draw();
    scroll.scrollLeft = a * S.px - x;
    if (atX === undefined) show(S.cur);
  }
  function show(k, center) {             // the step k in view: in the middle when it comes from out of view, if asked
    const x0 = k * S.px, x1 = x0 + S.px, a = scroll.scrollLeft, b = a + avail();
    if (x0 >= a && x1 <= b) return;
    if (center) scroll.scrollLeft = x0 + S.px / 2 - avail() / 2;
    else if (x0 < a) scroll.scrollLeft = x0 - S.px; else scroll.scrollLeft = x1 - avail() + S.px;
  }
  function selRow() {                    // the selected row in view, under the axis, above the bottom
    const rw = grid.querySelector('.wv-row[data-i="' + S.sel + '"]');
    if (!rw) return;
    const top = scroll.scrollTop + WV.axis, bot = scroll.scrollTop + scroll.clientHeight - 12;
    if (rw.offsetTop < top) scroll.scrollTop = rw.offsetTop - WV.axis;
    else if (rw.offsetTop + rw.offsetHeight > bot) scroll.scrollTop = rw.offsetTop + rw.offsetHeight - scroll.clientHeight + 12;
  }
  const colAt = clientX => {
    const r = scroll.getBoundingClientRect();
    const c = Math.floor((scroll.scrollLeft + clientX - r.left - L) / S.px);
    return c < 0 || c >= cols ? null : c;
  };

  let words = [], hov = null, tags = [];  // the words of the boxes: {t, a, b, txt}; the column under the pointer; the marker words
  function draw() {
    const px = S.px, W = cols * px, every = [1, 2, 5, 10, 20, 50, 100, 200, 500, 1000].find(e => e * px >= 30) || 2000;
    tags = [];
    grid.style.width = (L + W + 16) + "px";
    grid.innerHTML = "";
    words = [];
    const left = (cls, ...kids) => h("div", { class: "wv-left" + (cls ? " " + cls : ""), style: "width:" + L + "px" }, ...kids);
    // behind the values: the loop, the cursor, the step that fails, the start of the loop, the faded pass
    const marks = (svg, H, y0, dash) => {
      if (loop !== null) sv("rect", { x: loop * px, y: y0, width: (n - loop) * px, height: H - y0, fill: "#edf3f2" }, svg);
      if (S.cur !== tr.at) sv("rect", { x: S.cur * px, y: y0, width: px, height: H - y0, fill: "#e6effa" }, svg);
      if (fails) sv("rect", { x: tr.at * px, y: y0, width: px, height: H - y0, fill: "#fbeceb" }, svg);
      if (loop !== null) sv("line", { x1: loop * px + .5, y1: dash === undefined ? y0 : dash, x2: loop * px + .5, y2: H,
                             stroke: "#8a9b9c", "stroke-dasharray": "3 3" }, svg);
      if (ghost) sv("line", { x1: n * px + .5, y1: y0, x2: n * px + .5, y2: H, stroke: "#c3cdcd" }, svg);
    };
    // the cursor: under the waves, just right of the edge of its step, so a change there stays in view
    const cursor = (svg, y0, H) => sv("line", { x1: S.cur * px + 2, y1: y0, x2: S.cur * px + 2, y2: H, stroke: "#2f6fb5", "stroke-width": 2 }, svg);
    const backed = (svg, t, font) => {   // a word with white behind it, over the lines it crosses
      const w = textWidth(t.textContent, font), x = +t.getAttribute("x"), a = t.getAttribute("text-anchor");
      const x0 = a === "middle" ? x - w / 2 : a === "end" ? x - w : x;
      const r = sv("rect", { x: x0 - 3, y: +t.getAttribute("y") - 1, width: w + 6, height: 13, fill: "#fff" });
      svg.insertBefore(r, t);
      return { t, r, w };
    };
    // the axis: the lane of the loop, the lane of the step that fails, then the steps (or the times)
    const ax = h("div", { class: "wv-row wv-axis", style: "height:" + WV.axis + "px" },
      left("", h("span", { class: "wv-name", style: "width:" + nameW + "px" }, DATA.period ? "t (s)" : "step"),
        h("span", { class: "wv-corner" }, "value at ", h("span", { class: "wv-at" }, corner(S.cur)))));
    const asvg = sv("svg", { width: W + 16, height: WV.axis, class: "wv-svg" }, ax);
    const B = WV.band;
    marks(asvg, WV.axis, B, 13);
    cursor(asvg, B, WV.axis);
    if (loop !== null) {                  // the loop: a bracket over its steps
      const x0 = loop * px, x1 = n * px;
      sv("path", { d: `M${x0 + .5} 13V7H${x1 - .5}V13`, fill: "none", stroke: "#17393b", "stroke-width": 1.2 }, asvg);
      const t = sv("text", { x: x0 + 6, y: 2, "dominant-baseline": "hanging", class: "wv-loop" }, asvg);
      t.textContent = "↺ loop";
      tags.push({ ...backed(asvg, t, "600 10px Helvetica, Arial, sans-serif"), mid: false, x: x0 + 6, a: x0, b: x1 });
    }
    if (fails) {                          // the step that fails: "fails ▾" over a red column
      const m = tr.at * px + px / 2;
      const t = sv("text", { x: m, y: 15, "text-anchor": "middle", "dominant-baseline": "hanging", class: "wv-fails" }, asvg);
      t.textContent = "fails";
      tags.push({ ...backed(asvg, t, "bold 10px Helvetica, Arial, sans-serif"), mid: true, x: m, a: tr.at * px, b: tr.at * px + px });
      sv("path", { d: `M${m - 4} 26h8l-4 5z`, fill: "#b03a2e" }, asvg);
    }
    for (let c = 0; c < cols; c++) {
      const k = real(c), x = c * px, g = c >= n;
      sv("line", { x1: x, y1: (g ? c - n : c) % every ? WV.axis - 4 : WV.axis - 8, x2: x, y2: WV.axis, stroke: "#b6c1c1" }, asvg);
      if ((g ? c - n : c) % every === 0 || (!g && (k === tr.at || k === S.cur))) {
        const t = sv("text", { x: Math.max(x + px / 2, textWidth(label(k), "10.5px Menlo, Monaco, monospace") / 2 + 2), y: WV.axis - 11, "text-anchor": "middle",
          class: "wv-tick" + (g ? " ghost" : k === tr.at ? " fail" : k === S.cur ? " cur" : "") }, asvg);
        t.textContent = label(k);
      }
    }
    if (ghost) sv("text", { x: cols * px + 4, y: WV.axis - 11, class: "wv-tick ghost" }, asvg).textContent = "…";
    grid.appendChild(ax);
    // the rows, under the name of their group
    let r = 0;
    order.forEach((i, j) => {
      if (groups && (j === 0 || req(order[j - 1]) !== req(i))) {
        const gr = h("div", { class: "wv-row wv-group", style: "height:" + WV.group + "px" },
          left("", h("span", { class: "wv-gname", title: req(i) ? "The variables in the terms of " + reqName : "" },
            req(i) ? ["Variables of ", h("b", {}, reqName)] : "Other variables")));
        const gs = sv("svg", { width: W + 16, height: WV.group, class: "wv-svg" }, gr);
        marks(gs, WV.group, 0); cursor(gs, 0, WV.group);
        grid.appendChild(gr);
      }
      const kd = kind(i), plot = kd === "num" && S.plots.has(i);
      const H = plot ? WV.plot : kd === "bool" ? WV.bool : WV.row;
      const nums = tr.steps.map(st => st[i]).filter(v => typeof v === "number");
      const lo = Math.min(...nums), hi = Math.max(...nums);
      const nm = h("span", { class: "wv-name" + (req(i) ? " port" : ""), title: tr.vars[i], style: "width:" + (nameW - 24) + "px" }, tr.vars[i]);
      const valv = h("span", { class: "wv-val", style: "width:" + valW + "px" }, fmt(tr.steps[S.cur][i]));
      if (plot) valv.append(h("small", {}, "max " + short(hi)), h("small", {}, "min " + short(lo)));
      let tg = h("span", { class: "wv-plot" });
      if (kd === "num") {
        const tip = plot ? "Show as values" : "Draw as a line";
        tg = h("button", { class: "wv-plot" + (plot ? " on" : ""), "data-tip": tip, "aria-label": tip,
                           onclick: e => { e.stopPropagation(); S.plots.has(i) ? S.plots.delete(i) : S.plots.add(i); draw(); } });
        tg.innerHTML = '<svg viewBox="0 0 16 16"><path d="M2 13.5h12"/><path d="M2.5 10.5l3-4 3 2.5 5-6"/></svg>';
      }
      const lf = left(S.sel === i ? "sel" : "", nm, tg, valv);
      lf.addEventListener("click", () => { S.sel = i; draw(); selRow(); });
      const next = order[j + 1];
      const flat = kd === "bool" && next !== undefined && kind(next) === "bool" && req(next) === req(i);  // no line between two Booleans
      const row = h("div", { class: "wv-row" + (r++ % 2 ? " odd" : "") + (flat ? " nb" : "") + (S.sel === i ? " sel" : ""), "data-i": i, style: "height:" + H + "px" }, lf);
      const svg = sv("svg", { width: W + 16, height: H, class: "wv-svg" }, row);
      marks(svg, H, 0);
      cursor(svg, 0, H);
      if (plot) [6, H - 6].forEach(y => sv("line", { x1: 0, y1: y, x2: W, y2: y, stroke: "#dfe5e5", "stroke-dasharray": "2 3" }, svg));
      const draw1 = (from, to, g, wg) => {
        if (kd === "bool") {
          const hiY = 6, loY = H - 6;
          let d = "";
          for (let c = from; c < to; c++) {
            const v = valueAt(c, i), x = c * px;
            if (v === null || v === undefined) { sv("rect", { x, y: hiY, width: px, height: loY - hiY, fill: "url(#wvhatch)" }, g); continue; }
            const y = v ? hiY : loY;
            if (v) sv("rect", { x, y: hiY, width: px, height: loY - hiY, fill: "#c4d9d5" }, g);
            d += (d && c > from ? "L" : "M") + x + " " + y + "H" + (x + px);
          }
          sv("path", { d, fill: "none", stroke: "#17393b", "stroke-width": 1.5 }, g);
        } else if (plot) {                // a step line, filled faintly under it, between the least and the greatest value
          const y = v => lo === hi ? H / 2 : H - 6 - (v - lo) / (hi - lo) * (H - 12);
          let d = "", f = "", x0 = null;
          for (let c = from; c <= to; c++) {
            const v = c < to ? valueAt(c, i) : null;
            if (typeof v !== "number") { if (x0 !== null) f += "V" + (H - 6) + "H" + x0 + "Z"; x0 = null; continue; }
            const yv = y(v);
            if (x0 === null) { x0 = c * px; d += "M" + x0 + " " + yv; f += "M" + x0 + " " + (H - 6) + "V" + yv; }
            else { d += "V" + yv; f += "V" + yv; }
            d += "H" + (c + 1) * px; f += "H" + (c + 1) * px;
          }
          sv("path", { d: f, fill: "rgba(23,57,59,.08)" }, g);
          sv("path", { d, fill: "none", stroke: "#17393b", "stroke-width": 1.5 }, g);
        } else if (px < 8) {              // steps too narrow for boxes: a band, a tick where the value changes
          const top = 4, bot = H - 4;
          sv("rect", { x: from * px, y: top, width: (to - from) * px, height: bot - top, fill: "#f7f9f9" }, g);
          for (let c = from; c < to; c++) {
            const v = valueAt(c, i);
            if (v === null || v === undefined) sv("rect", { x: c * px, y: top, width: px, height: bot - top, fill: "url(#wvhatch)" }, g);
          }
          let d = `M${from * px} ${top + .5}H${to * px}M${from * px} ${bot - .5}H${to * px}`;
          for (let c = from + 1; c < to; c++) if (c === loop || fmt(valueAt(c, i)) !== fmt(valueAt(c - 1, i))) d += `M${c * px + .5} ${top}V${bot}`;
          sv("path", { d, fill: "none", stroke: "#7d8c8d", "stroke-width": 1 }, g);
        } else {                          // a box for each run of one value
          let c = from;
          while (c < to) {
            const v = valueAt(c, i); let e = c + 1;
            while (e < to && e !== loop && fmt(valueAt(e, i)) === fmt(v)) e++;   // a box ends where the loop starts
            const x0 = c * px + .5, x1 = e * px - .5, s = Math.min(4, (x1 - x0) / 4), top = 4, bot = H - 4, mid = H / 2;
            const sl = c === 0 ? 0 : s, sr = e === cols ? 0 : s;   // flat at the start and the end: nothing changes there
            sv("path", { d: `M${x0} ${mid}L${x0 + sl} ${top}H${x1 - sr}L${x1} ${mid}L${x1 - sr} ${bot}H${x0 + sl}Z`,
              fill: v === null || v === undefined ? "url(#wvhatch)" : "#f7f9f9", stroke: "#7d8c8d", "stroke-width": 1 }, g);
            if (x1 - x0 - 2 * s >= 17)
              words.push({ t: sv("text", { y: mid, "text-anchor": "middle", "dominant-baseline": "central", class: "wv-v" }, wg),
                           a: x0 + sl, b: x1 - sr, txt: fmt(v), alt: short(v), shown: null });
            c = e;
          }
        }
      };
      const wg = sv("g", {});             // the words over the cursor, their halo hiding it
      draw1(0, n, svg, wg);
      if (ghost) { const gg = sv("g", { opacity: ".35" }, svg); draw1(n, cols, gg, gg); }
      svg.appendChild(wg);
      grid.appendChild(row);
    });
    const defs = sv("svg", { width: 0, height: 0, style: "position:absolute" });
    defs.innerHTML = '<defs><pattern id="wvhatch" width="6" height="6" patternUnits="userSpaceOnUse" patternTransform="rotate(45)">'
      + '<rect width="6" height="6" fill="#f2f4f4"/><line x1="0" y1="0" x2="0" y2="6" stroke="#d3dbdb" stroke-width="2"/></pattern></defs>';
    grid.appendChild(defs);
    hov = h("div", { class: "wv-hov" });
    grid.appendChild(hov);
    where.innerHTML = "";
    if (DATA.period) where.append("t = ", h("span", { class: "wv-k" }, label(S.cur)), " s");
    else where.append("Step ", h("span", { class: "wv-k" }, label(S.cur)));
    zin.classList.toggle("off", S.px >= WV.max); zout.classList.toggle("off", S.px <= least() + 1e-9);   // at the limits
    fitHeight();
    place();
  }
  function place() {                     // the words of the boxes, the bar under the steps and the steps in view
    const v0 = scroll.scrollLeft, v1 = v0 + scroll.clientWidth - L;
    const cx = S.cur * S.px + 2;           // the cursor line
    const roomOf = (a, b) => Math.floor((b - a - 4) / 6.6);
    words.forEach(w => {                 // in the middle of the part in view, cut to fit it; beside the cursor, not under it
      let a = Math.max(w.a, v0), b = Math.min(w.b, v1);
      if (cx > a + 2 && cx < b - 2) {
        const [l, r] = [[a, cx - 3], [cx + 4, b]];
        const side = roomOf(...l) >= roomOf(...r) ? l : r;
        if (roomOf(...side) >= Math.min(2, w.txt.length)) [a, b] = side;
      }
      const room = roomOf(a, b);
      const txt = w.txt.length <= room ? w.txt : w.alt.length <= room ? w.alt : room >= 3 ? w.alt.slice(0, room - 1) + "…" : "";
      if (txt !== w.shown) { w.t.textContent = txt; w.shown = txt; }
      if (txt) w.t.setAttribute("x", (a + b) / 2);
    });
    tags.forEach(m => {                  // "fails" and "↺ loop" inside the part in view, while their steps are in it
      let x = m.x;
      if (m.b > v0 && m.a < v1) x = m.mid ? Math.max(v0 + m.w / 2 + 4, Math.min(v1 - m.w / 2 - 4, x))
                                          : Math.max(v0 + 4, Math.min(v1 - m.w - 4, x));
      m.t.setAttribute("x", x); m.r.setAttribute("x", (m.mid ? x - m.w / 2 : x) - 3);
    });
    const over = scroll.scrollWidth > scroll.clientWidth + 1;
    sbar.classList.toggle("on", over);
    const a = Math.max(0, Math.floor(scroll.scrollLeft / S.px)), b = Math.min(cols, Math.ceil((scroll.scrollLeft + avail()) / S.px)) - 1;
    const end = scroll.scrollTop + scroll.clientHeight;
    const below = [...grid.querySelectorAll(".wv-row[data-i]")].filter(r => r.offsetTop + r.offsetHeight / 2 > end).length;
    foot.firstChild.textContent = (over ? (DATA.period ? "t " + label(a) + " to " + label(Math.min(b, n - 1)) : "steps " + a + " to " + Math.min(b, n - 1))
      + " of " + n + " in view" : n + " steps") + (below ? " · " + below + (below === 1 ? " more row" : " more rows") + " below" : "");
    fade.style.display = below ? "block" : "none";
    edgeL.innerHTML = edgeR.innerHTML = "";
    if (over) {                             // the step that fails or the loop out of view: a way to it at the edge
      const go = (k, cls, word, key) => (k < a ? edgeL : edgeR).append(h("button", { class: cls, "data-tip": "Go there (" + key + ")",
        onclick: () => { jump(k); box.focus({ preventScroll: true }); } }, k < a ? "◂ " + word : word + " ▸"));   // the keys stay
      if (loop !== null && (loop > b || loop < a)) go(loop, "wv-loop", "↺ loop from " + label(loop), "L");   // its word out of view
      if (fails && (tr.at < a || tr.at > b)) {
        const side = tr.at < a ? edgeL : edgeR;
        if (!side.childNodes.length) side.append(h("span"));       // the first line, that of the loop, kept
        go(tr.at, "wv-fails", "fails at " + label(tr.at), "F");
      }
      edgeL.style.top = edgeR.style.top = scroll.offsetTop + "px";
      edgeL.style.left = (L + 6) + "px";
    }
    if (below) fade.style.top = (scroll.offsetTop + scroll.clientHeight - 12) + "px";
    const hint = foot.lastChild;            // the ways to move, when the drawer has room for them
    hint.textContent = "Click a step · " + (over ? "drag to pan · " : "") + (mac ? "⌘" : "Ctrl") + " + wheel to zoom · ? for the keys";
    hint.hidden = box.clientWidth < 800;
    if (!hint.hidden && foot.offsetHeight > 34) hint.hidden = true;   // the note on one line first
    if (!over) return;
    const w = sbar.clientWidth, vis = avail() / (scroll.scrollWidth - L);
    thumb.style.width = Math.max(24, w * Math.min(1, vis)) + "px";
    thumb.style.left = (w - thumb.offsetWidth) * scroll.scrollLeft / Math.max(1, scroll.scrollWidth - scroll.clientWidth) + "px";
  }
  scroll.addEventListener("scroll", place);
  // the bar: drag its thumb, or click it to page
  sbar.addEventListener("mousedown", e => { if (e.target === thumb) return;
    S.fit = false; scroll.scrollLeft += (e.offsetX < thumb.offsetLeft ? -1 : 1) * avail(); });
  thumb.addEventListener("mousedown", e => {
    e.preventDefault(); e.stopPropagation();
    const x0 = e.clientX, s0 = scroll.scrollLeft, k = scroll.scrollWidth / sbar.clientWidth;
    thumb.classList.add("drag");
    const move = m => { S.fit = false; scroll.scrollLeft = s0 + (m.clientX - x0) * k; };
    const up = () => { thumb.classList.remove("drag"); removeEventListener("mousemove", move); removeEventListener("mouseup", up); };
    addEventListener("mousemove", move); addEventListener("mouseup", up);
  });
  // the pointer on the steps: its column tinted, the value under it in full in the tip
  const tip = document.getElementById("tip");
  let tipped = false;
  const untip = () => { if (tipped) { tip.style.display = "none"; tipped = false; } };
  const unhover = () => { if (hov) hov.style.display = "none"; untip(); };
  scroll.addEventListener("mousemove", e => {
    const c = colAt(e.clientX), rw = e.target.closest(".wv-row");
    if ((down && down.moved) || c === null || !rw || e.target.closest(".wv-left")) return unhover();
    hov.style.display = "block"; hov.style.left = (L + c * S.px) + "px"; hov.style.width = S.px + "px";
    if (rw.dataset.i === undefined) return untip();
    const i = +rw.dataset.i, v = valueAt(c, i);
    tip.textContent = tr.vars[i] + (v === null || v === undefined ? ": no value" : " = " + fmt(v)) + " · " + at(real(c)) + (c >= n ? ", next pass" : "");
    tip.style.display = "block"; tipped = true;
    const w = tip.offsetWidth, ht = tip.offsetHeight;
    tip.style.left = Math.max(4, Math.min(e.clientX + 12, innerWidth - w - 8)) + "px";
    tip.style.top = (e.clientY + 18 + ht > innerHeight ? e.clientY - ht - 10 : e.clientY + 18) + "px";
  });
  scroll.addEventListener("mouseleave", unhover);
  // a click on the steps picks one; a drag pans along them
  let down = null;
  scroll.addEventListener("pointerdown", e => {
    if (e.button !== 0 || e.target.closest(".wv-left")) return;
    const rw = e.target.closest(".wv-row[data-i]");
    down = { x: e.clientX, s: scroll.scrollLeft, moved: false, row: rw ? +rw.dataset.i : null };
  });
  WAVE.move = e => {
    if (!down) return;
    if (Math.abs(e.clientX - down.x) > 4) { down.moved = true; scroll.classList.add("pan"); S.fit = false; }
    if (down.moved) scroll.scrollLeft = down.s - (e.clientX - down.x);
  };
  addEventListener("pointermove", WAVE.move);
  WAVE.up = e => {
    if (!down) return;
    const was = down; down = null; scroll.classList.remove("pan");
    if (was.moved) return;
    const k = WAVE.stepAt(e.clientX);      // a click on a row: that row selected, for [ ] and Enter
    if (was.row !== null && was.row !== S.sel) { S.sel = was.row; if (k === null || k === S.cur) draw(); }
    if (k !== null) pick(k);
    if (was.row !== null) selRow();
  };
  addEventListener("pointerup", WAVE.up);
  // Ctrl or Cmd and the wheel (a pinch too) zoom where they point; Shift and the wheel pan along the
  // steps, and so does the wheel alone when all the rows are in view
  scroll.addEventListener("wheel", e => {
    if (!(e.ctrlKey || e.metaKey)) {
      const tall = scroll.scrollHeight > scroll.clientHeight + 1;
      if (e.deltaX || !e.deltaY || (tall && !e.shiftKey)) return;
      e.preventDefault();
      if (scroll.scrollWidth > scroll.clientWidth + 1) S.fit = false;
      scroll.scrollLeft += e.deltaY;
      return;
    }
    e.preventDefault();
    const r = scroll.getBoundingClientRect();
    zoom(Math.exp(-e.deltaY * 0.01), Math.max(0, e.clientX - r.left - L));
  }, { passive: false });
  box.onkeydown = e => {
    const k = e.key;
    const go = s => { e.preventDefault(); pick(Math.max(0, Math.min(n - 1, s))); };
    const changed = s => fmt(tr.steps[s][S.sel]) !== fmt(tr.steps[s - 1][S.sel]);
    if (k === "Escape") { if (keys.classList.contains("on")) toggleKeys(false); else closeWave(); e.preventDefault(); }
    else if (S.sel !== null && (k === "]" || (k === "ArrowRight" && e.altKey))) {   // the next change of the row
      let s = S.cur + 1;
      while (s < n && !changed(s)) s++;
      if (s < n) go(s); else e.preventDefault();
    } else if (S.sel !== null && (k === "[" || (k === "ArrowLeft" && e.altKey))) {  // the change before
      let s = S.cur - 1;
      while (s > 0 && !changed(s)) s--;
      go(s);
    }
    else if (k === "ArrowLeft" || k === "ArrowRight") go(S.cur + (k === "ArrowLeft" ? -1 : 1));
    else if (k === "Home") go(0);
    else if (k === "End") go(n - 1);
    else if ((k === "f" || k === "F") && fails) { e.preventDefault(); jump(tr.at); }
    else if ((k === "l" || k === "L") && loop !== null) { e.preventDefault(); jump(loop); }
    else if (k === "+" || k === "=") zoom(1.5);
    else if (k === "-") zoom(1 / 1.5);
    else if (k === "0") fitAll();
    else if (k === "?") toggleKeys();
    else if (k === "ArrowUp" || k === "ArrowDown") {
      const a = order.indexOf(S.sel), to = order[Math.max(0, Math.min(order.length - 1, a + (k === "ArrowUp" ? -1 : 1)))];
      S.sel = to; draw(); selRow(); e.preventDefault();
    } else if (k === "Enter" && S.sel !== null && kind(S.sel) === "num") {
      S.plots.has(S.sel) ? S.plots.delete(S.sel) : S.plots.add(S.sel); draw();
    } else return false;
    e.stopPropagation();
    return true;
  };
  // the keys are the drawer's while it is open, wherever the focus went (a button of the drawer drawn
  // again, a click on the diagram): those of a field or a button of the page stay theirs
  WAVE.key = e => box.onkeydown(e);
  // at first: every step in view, if each has room; else a width where most values are written whole
  // (four in five of the runs one step long), the step that fails in the middle
  S.px = Math.min(WV.max, avail() / cols);
  S.fit = S.px >= 24;
  if (!S.fit) {
    const lens = [].concat(...order.filter(i => kind(i) !== "bool").map(i => tr.steps.map(st => short(st[i]).length))).sort((a, b) => a - b);
    const p80 = lens.length ? lens[Math.floor(lens.length * .8)] : 1;
    S.px = Math.max(32, Math.min(90, Math.ceil(13 + 6.6 * p80)));
  }
  draw();
  show(fails ? tr.at : S.cur, true);
  box.focus();
  // a wider or narrower drawer (the window, the details opened or closed): all the steps stay in the
  // width if they all were; else the zoom stays. A taller or shorter window: the height again
  let width = scroll.clientWidth;
  WAVE.ro = new ResizeObserver(() => {
    if (!WAVE.open || scroll.clientWidth === width) return;
    width = scroll.clientWidth;
    if (S.fit) fitAll(); else place();
  });
  WAVE.ro.observe(scroll);
  WAVE.bro = new ResizeObserver(() => { if (WAVE.open) viewAboveWave(true); });   // the drawer taller or shorter
  WAVE.bro.observe(box);
  WAVE.resize = () => { fitHeight(); place(); viewAboveWave(true); };
  addEventListener("resize", WAVE.resize);
}
// The diagram ends above the drawer, as it ends left of the details: what was in the middle stays in the
// middle; a diagram seen whole that the drawer would cut is fitted to the room left (or, tall, its box of
// the check brought into view), and at the close the view of before comes back.
function viewAboveWave(on) {
  const v = document.getElementById("view"), box = document.getElementById("wave");
  const h0 = v.clientHeight;
  const whole = H => cam && cam.y >= TOP - 1 && cam.y + size0.h * cam.k <= H + 1;
  const was = whole(h0);
  v.style.bottom = on ? Math.max(0, innerHeight - box.getBoundingClientRect().top + 8) + "px" : "";
  const d = h0 - v.clientHeight;
  if (!cam || !d) return;
  if (!on && WAVE.cam0) { cam = WAVE.cam0; WAVE.cam0 = null; place(); return; }   // the view of before the drawer
  const cam0 = { ...cam };
  cam.y -= d / 2;
  // a diagram seen whole that the drawer would cut: fitted above it, unless that makes it hard to read
  // (a tall diagram); then its scale kept and the box of the check brought into view
  if (on && was && !whole(v.clientHeight)) {
    WAVE.cam0 = WAVE.cam0 || cam0;
    const n = panel && byId[panel.id];
    if (fitScale() >= 0.7 * cam.k || !n) fit(); else { place(); bringInto(n); }
  }
  else place();
}
function closeWave(keepView) {
  const box = document.getElementById("wave"), tip = document.getElementById("tip");
  if (!box || !WAVE.open) return;
  box.style.display = "none"; box.innerHTML = "";
  document.body.classList.remove("wave");
  if (tip) tip.style.display = "none";
  WAVE.open = false;
  if (WAVE.up) removeEventListener("pointerup", WAVE.up);
  if (WAVE.move) removeEventListener("pointermove", WAVE.move);
  if (WAVE.resize) removeEventListener("resize", WAVE.resize);
  if (WAVE.ro) WAVE.ro.disconnect();
  if (WAVE.bro) WAVE.bro.disconnect();
  if (!keepView) viewAboveWave(false);
}
// The code of a C function in the details, folded: its title says whose it is.
function codeSection(n) {
  const c = n.model && n.model.code;
  if (!c) return null;
  const d = h("details", { class: "rp-sec rp-code" },
    h("summary", { class: "rp-h" }, svgSpan("chev", '<path d="M6 4l4 4-4 4"/>'), "Code",
      h("span", { class: "rp-file" }, c.file + " · " + n.model.name)));
  d.open = !!panel.code;
  d.addEventListener("toggle", () => { panel.code = d.open; });
  c.parts.forEach((q, i) => {
    if (c.parts.length > 1) d.appendChild(h("div", { class: "rp-note", style: "margin:" + (i ? 8 : 4) + "px 0 2px" }, q.name));
    const pre = h("pre", { class: "code" });
    q.text.split("\n").forEach((l, i) => pre.appendChild(h("span", { class: "l", "data-line": String(q.line + i) },
      h("span", { class: "n" }, String(q.line + i)), l + "\n")));
    d.appendChild(pre);
  });
  return d;
}
// The checks that fail, on the tools: the button opens the list.
function renderQueue() {
  const list = failing(), b = document.getElementById("repairs");
  b.style.display = list.length || inspector.open ? "inline-flex" : "none";   // also to close the panel
  document.getElementById("repcount").textContent = list.length || "";
  if (inspector.open && inspector.tab === "problems") fillProblems(list);
}
// the wrench opens and closes the panel
function toggleQueue() {
  const list = failing();
  if (inspector.open) hideInspector();
  else if (list.length === 1) openRepair(list[0][0], list[0][1]);    // one check that fails: its details at once
  else showInspector("problems");
}
// an icon of the page, as a span: one size and one stroke for all (class ib or chev)
function svgSpan(cls, paths) {
  const sp = h("span", { class: cls });
  sp.innerHTML = '<svg viewBox="0 0 16 16" aria-hidden="true">' + paths + "</svg>";
  return sp;
}
// the buttons of the panel: icons, as the cross and the back arrow; what each does is in its tip
const GLYPH = {
  find: '<circle cx="7" cy="7" r="4.3"/><path d="M10.2 10.2l3.6 3.6"/>',
  again: '<path d="M13 8a5 5 0 1 1-1.6-3.7"/><path d="M13 2.6v3.2H9.8"/>',
  stop: '<rect x="4.5" y="4.5" width="7" height="7" rx="1"/>',
  eye: '<path d="M1.6 8S4 3.8 8 3.8 14.4 8 14.4 8 12 12.2 8 12.2 1.6 8 1.6 8z"/><circle cx="8" cy="8" r="1.9"/>',
  apply: '<path d="M3.2 8.4l3 3 6.6-7"/>',
  longer: '<circle cx="8" cy="8" r="5.6"/><path d="M8 4.8V8l2.2 1.6"/>',
  prev: '<path d="M10 3.5L5.5 8l4.5 4.5"/>',
  wave: '<path d="M1.5 11h2.5V5h3v6h2.5V5h3v6h2"/>',
  zin: '<circle cx="7" cy="7" r="4.3"/><path d="M10.2 10.2l3.6 3.6M5 7h4M7 5v4"/>',
  zout: '<circle cx="7" cy="7" r="4.3"/><path d="M10.2 10.2l3.6 3.6M5 7h4"/>',
  fit: '<path d="M1.5 8h13M4.5 5l-3 3 3 3M11.5 5l3 3-3 3"/>',
  keys: '<rect x="1.5" y="4" width="13" height="8" rx="1.5"/><path d="M4 6.6h.5M6.5 6.6h.5M9 6.6h.5M11.5 6.6h.5M5 9.4h6"/>',
  next: '<path d="M6 3.5L10.5 8 6 12.5"/>',
};
// an icon button; one that cannot act now stays in view, with its tip saying why (not disabled: a
// disabled button has no tip)
function iconButton(paths, tip, onclick, cls, off) {
  const b = h("button", { class: "ib" + (cls ? " " + cls : "") + (off ? " off" : ""), "data-tip": tip, "aria-label": tip,
                          "aria-disabled": off ? "true" : null, onclick: e => { if (!off) onclick(e); } });
  b.innerHTML = '<svg viewBox="0 0 16 16" aria-hidden="true">' + paths + "</svg>";
  return b;
}
// the box of the check picked, marked; brought into the view when it is out of it (once, when picked)
function markPick() {
  document.querySelectorAll("#view .pick").forEach(e => e.classList.remove("pick"));
  if (panel) { const b = document.querySelector('#view [data-box="' + panel.id + '"]'); if (b) b.classList.add("pick"); }
}
const READ = 0.75;     // below this scale the words of a box are hard to read
function bringInto(n) {
  const b = document.querySelector('#view [data-box="' + n.id + '"] > rect'), v = document.getElementById("view");
  if (!b || !cam) return;
  const r = b.getBoundingClientRect(), all = v.getBoundingClientRect(), P = document.getElementById("insp");
  const right = inspector.open && P && P.offsetParent ? Math.min(all.right, P.getBoundingClientRect().left - 8) : all.right;
  const V = { left: all.left, top: all.top, bottom: all.bottom, right, width: right - all.left, height: all.height };
  const seen = r.left >= V.left && r.right <= V.right && r.top >= V.top + 40 && r.bottom <= V.bottom;
  if (seen && cam.k >= READ) return;
  // the scale that shows it whole; one too small to read (a large diagram fitted to the window) made
  // larger, up to the size of its words, and then, if it is still too large, its top left (its title
  // and its tag) in view
  const whole = Math.min(0.9 * V.width / r.width * cam.k, 0.85 * (V.height - 40) / r.height * cam.k);
  const k = cam.k >= READ ? Math.min(cam.k, whole) : Math.max(cam.k, Math.min(1, Math.max(whole, READ)));
  if (k !== cam.k) zoomAt(r.left + r.width / 2, r.top + r.height / 2, k / cam.k);
  const r2 = b.getBoundingClientRect();
  cam.x += r2.width <= 0.95 * V.width ? V.left + V.width / 2 - (r2.left + r2.width / 2) : V.left + 16 - r2.left;
  cam.y += r2.height <= 0.9 * (V.height - 40) ? V.top + 40 + (V.height - 40) / 2 - (r2.top + r2.height / 2) : V.top + 52 - r2.top;
  place();
}
// A term listed in the panel, on the diagram: its box open, the term in view, its lines flashed.
async function showTerm(n, id) {
  const key = (id.startsWith("composed") ? "k:" : "c:") + n.id;
  if (!open.has(key)) { open.add(key); await render(); }
  const rows = [...document.querySelectorAll('#view [data-box="' + n.id + '"] > g[data-term="' + id + '"]')];
  if (!rows.length || !cam) return;
  bringInto(n);
  const v = document.getElementById("view").getBoundingClientRect();
  const r = rows.map(x => x.getBoundingClientRect()), top = Math.min(...r.map(q => q.top)), bottom = Math.max(...r.map(q => q.bottom));
  const W = document.getElementById("wave"), low = W && W.offsetParent ? W.getBoundingClientRect().top - 8 : v.bottom;
  if (top < v.top + 48 || bottom > low) { cam.y += (v.top + 48 + low) / 2 - (top + bottom) / 2; place(); }
  rows.forEach(x => { const b = x.getBBox();
    const f = el("rect", { class: "flash", x: b.x - 2, y: b.y - 1, width: b.width + 4, height: b.height + 2, rx: 3 }, x);
    setTimeout(() => f.remove(), 1700); });
}
// A counterexample on the diagram: the values of a step on the ports of the box of the check and inside it.
function replay(n, tr, k) {
  clearReplay();
  const inside = new Set();             // the box of the check and the boxes inside it
  (function walk(b) { inside.add(String(b.id)); (b.children || []).forEach(walk); })(n);
  const at = {}; tr.vars.forEach((v, i) => { at[v] = tr.steps[k][i]; });
  const fmt = v => typeof v === "boolean" ? truth(v) : typeof v === "number" ? digits(v) : String(v);
  document.querySelectorAll("#view text[data-var]").forEach(x => {
    const v = x.getAttribute("data-var");
    if (!(v in at) || !inside.has(x.getAttribute("data-pbox"))) return;
    // a label left of its port keeps its right end at the port (the value may be longer than the type)
    const w0 = x.getComputedTextLength();
    x.setAttribute("data-orig", x.textContent);
    x.textContent = v + " = " + fmt(at[v]);
    x.classList.add("pv-val");
    if (typeof at[v] === "number" && fmt(at[v]) !== String(at[v])) { x.setAttribute("data-otip", x.getAttribute("data-tip") || "");
      x.setAttribute("data-tip", v + " = " + at[v]); }     // the whole value in its tip
    if (x.getAttribute("data-side") === "left") { x.setAttribute("data-ox", x.getAttribute("x"));
      x.setAttribute("x", +x.getAttribute("x") + w0 - x.getComputedTextLength()); }
  });
  // the two ends of a short wire say the same value: one says it
  const shown = [...document.querySelectorAll("#view text.pv-val")].map(x => [x, x.getBoundingClientRect()]);
  shown.forEach(([x, r], i) => {
    if (shown.slice(0, i).some(([y, q]) => !y.hasAttribute("data-hid") && y.textContent === x.textContent
        && r.left < q.right && q.left < r.right && r.top < q.bottom && q.top < r.bottom)) {
      x.setAttribute("data-hid", ""); x.style.visibility = "hidden"; }
  });
  // a variable on a box and on the box around it, with the same value, the two near: the box around
  // says it (a wire across the box, between two parts, keeps its own)
  const rectOf = id => { const b = document.querySelector('#view [data-box="' + id + '"]'); return b && b.getBoundingClientRect(); };
  const around = (q, r) => q.left <= r.left && q.right >= r.right && q.top <= r.top && q.bottom >= r.bottom
    && (q.width > r.width || q.height > r.height);
  shown.forEach(([x]) => {
    const r = rectOf(x.getAttribute("data-pbox"));
    if (!r || x.hasAttribute("data-hid")) return;
    const near = (a, b) => Math.abs(a.left - b.left) < 140 && Math.abs(a.top - b.top) < 40;
    if (shown.some(([y, q]) => y !== x && !y.hasAttribute("data-hid") && y.textContent === x.textContent
        && near(q, x.getBoundingClientRect()) && (o => o && around(o, r))(rectOf(y.getAttribute("data-pbox"))))) {
      x.setAttribute("data-hid", ""); x.style.visibility = "hidden"; }
  });
}
function clearReplay() {
  document.querySelectorAll("#view text[data-orig]").forEach(x => { x.textContent = x.getAttribute("data-orig");
    x.removeAttribute("data-orig"); x.classList.remove("pv-val");
    if (x.hasAttribute("data-hid")) { x.removeAttribute("data-hid"); x.style.visibility = ""; }
    if (x.hasAttribute("data-ox")) { x.setAttribute("x", x.getAttribute("data-ox")); x.removeAttribute("data-ox"); }
    if (x.hasAttribute("data-otip")) { const o = x.getAttribute("data-otip"); o ? x.setAttribute("data-tip", o) : x.removeAttribute("data-tip");
                                       x.removeAttribute("data-otip"); } });
}
function afterRender() {
  if (previewing) { const p = previewing; previewing = null; showPreview(p); }
  markPick();
  const n = panel && byId[panel.id], w = n && n.badge && tagOf(n.badge.text) === panel.tag && n.badge.why;
  if (w && w.trace) replay(n, w.trace, Math.min(panel.step || 0, w.trace.steps.length - 1));
  renderQueue();
  if (inspector.open && inspector.tab !== "problems") renderInspector();   // new results: the details too
}
// The checks run by the editor: the button starts them (or stops them), and
// the places where their results will be show three dots meanwhile.
function setBusy(on) {
  busy = !!on;
  progressed = {};
  document.getElementById("run").classList.toggle("busy", busy);
  document.getElementById("run").title = busy ? "Checking… Click to stop"
    : "Check: A ∧ G of each contract, and the refinements, implementations, and environments drawn";
  if (DATA) render();
}
// A result known while the checks run: on its box at once (a few at a time).
const TAGS = { feasible: ["of", "A∧G"], refines: ["abstract", "⪯"] };
let redraw = null;
function progress(e) {
  const where = TAGS[e.check];
  if (!where || !busy) return;
  const status = { sat: "valid", valid: "valid", unsat: "invalid", invalid: "invalid" }[e.result] || "unknown";
  (progressed[e[where[0]]] = progressed[e[where[0]]] || {})[where[1]] = status;
  clearTimeout(redraw);
  redraw = setTimeout(() => { if (DATA) render(); }, 60);
}
function runChecks() {
  if (!inEditor()) return;
  vscodeApi = vscodeApi || acquireVsCodeApi();
  vscodeApi.postMessage({ type: busy ? "chase-stop" : "chase-check" });
}
if (inEditor()) {                  // the Check button, in a group of its own
  document.getElementById("run").style.display = "inline-flex";
  document.getElementById("runsep").style.display = "inline-block";
}
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
