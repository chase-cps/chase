"""The state graph of a small implementation, for the diagrams.

An implementation read from RTL is written by Yosys as an and-inverter
graph (AIGER), whose latches are the registers of the design; the states
reachable from the initial values are enumerated over every value of the
inputs. A controller synthesized by slugs is a state graph already. The
transitions between two states are labeled by the inputs that take them,
as a short sum of products. The graph is for reading: the checks do not use
it.
"""

from __future__ import annotations

import itertools
import os
import shutil
import subprocess
import tempfile
from typing import Optional

CLOCKS = {"clk", "clk_i", "clock"}


def state_graph(model, inputs=(), max_states: int = 24, max_inputs: int = 10) -> Optional[dict]:
    """{"states": [{"id", "label", "init"}], "edges": [{"from", "to",
    "label"}], "count"}, or {"count", "too_many": True} when more than
    max_states states are reachable, or None when the model cannot be
    enumerated. ``inputs`` are the inputs of the contract of the model: in
    SMV, the variables that are not states but the labels of the
    transitions."""
    source = getattr(model, "source", None) or {}
    if source.get("kind") == "json":
        return _json(source["dict"], max_states)
    if source.get("kind") == "smv" or (not source and getattr(model, "smv", None)):
        return _smv(model, set(inputs), max_states, max_inputs)
    if source.get("kind") == "rtl":
        aig = _aiger(source)
        if aig is None or not aig["latches"]:     # no register: no state machine
            return None
        return _enumerate(aig, max_states, max_inputs)
    if source.get("kind") == "controller":
        return _controller(source["controller"], max_states)
    return None


# AIGER ---------------------------------------------------------------------------

def _aiger(source: dict) -> Optional[dict]:
    from .rtl import yosys
    command = yosys()
    if command is None:
        return None
    files, top = source["files"], source["top"]
    defs = " ".join(f"-D{k}={v}" for k, v in (source.get("defines") or {}).items())
    chparam = " ".join(f"-chparam {k} {v}" for k, v in (source.get("parameters") or {}).items())
    with tempfile.TemporaryDirectory(prefix="chase_aig_") as tmp:
        for f in files:
            shutil.copy(f, os.path.join(tmp, os.path.basename(f)))
        script = "\n".join([
            f"read_verilog -sv -I. {defs} " + " ".join(f'"{os.path.basename(f)}"' for f in files),
            f"hierarchy -check -top {top} {chparam}", f"prep -top {top}", "flatten",
            "memory -nomap", "memory_map", "opt -fast", "async2sync", "dffunmap",
            "formalff -clk2ff", "simplemap", "setundef -zero", "aigmap",
            "write_aiger -ascii -map map.txt design.aag"]) + "\n"
        with open(os.path.join(tmp, "script.ys"), "w") as f:
            f.write(script)
        p = subprocess.run(command + ["-q", "-s", "script.ys"], cwd=tmp, capture_output=True,
                           text=True, timeout=120)
        if p.returncode or not os.path.exists(os.path.join(tmp, "design.aag")):
            return None
        with open(os.path.join(tmp, "design.aag")) as f:
            lines = f.read().split("\n")
        with open(os.path.join(tmp, "map.txt")) as f:
            mapping = [l.split() for l in f if l.strip()]
    _, _m, n_in, n_latch, n_out, n_and = lines[0].split()[:6]
    n_in, n_latch, n_out, n_and = int(n_in), int(n_latch), int(n_out), int(n_and)
    pos = 1
    inputs = [int(lines[pos + i]) for i in range(n_in)]
    pos += n_in
    latches = []
    for i in range(n_latch):
        f = lines[pos + i].split()
        init = 0 if len(f) < 3 else (int(f[2]) if f[2] in ("0", "1") else 0)
        latches.append((int(f[0]), int(f[1]), init))
    pos += n_latch + n_out
    ands = [tuple(int(x) for x in lines[pos + i].split()) for i in range(n_and)]
    names = {("input", int(m[1])): m[3] for m in mapping if m[0] == "input"}
    names.update({("latch", int(m[1])): m[3] for m in mapping if m[0] == "latch"})
    bits = {}
    for (kind, i), name in names.items():
        bits[(kind, i)] = name
    return {"inputs": inputs, "latches": latches, "ands": ands,
            "input_names": [names.get(("input", i), f"i{i}") for i in range(n_in)],
            "latch_names": [names.get(("latch", i), f"r{i}") for i in range(n_latch)]}


def _enumerate(aig: dict, max_states: int, max_inputs: int) -> Optional[dict]:
    free = [i for i, n in enumerate(aig["input_names"]) if n not in CLOCKS]
    if len(free) > max_inputs:
        return None
    names = [aig["input_names"][i] for i in free]

    def step(state, values):
        v = {0: 0}
        for lit, s in zip(aig["inputs"], values):
            v[lit // 2] = s
        for (lit, _, _), s in zip(aig["latches"], state):
            v[lit // 2] = s
        val = lambda l: v[l // 2] ^ (l & 1)
        for lhs, a, b in aig["ands"]:
            v[lhs // 2] = val(a) & val(b)
        return tuple(val(nxt) for _, nxt, _ in aig["latches"])

    init = tuple(i for _, _, i in aig["latches"])
    seen, order, moves = {init: 0}, [init], {}
    while len(order) > len(set(moves)) and len(seen) <= max_states:
        state = next(s for s in order if s not in moves)
        moves[state] = {}
        for bits_in in itertools.product((0, 1), repeat=len(free)):
            values = [0] * len(aig["inputs"])
            for i, b in zip(free, bits_in):
                values[i] = b
            nxt = step(state, values)
            if nxt not in seen:
                seen[nxt] = len(order)
                order.append(nxt)
            moves[state].setdefault(nxt, []).append(bits_in)
    if len(seen) > max_states:
        return {"count": len(seen), "too_many": True}

    def label(state):
        on = [n for n, b in zip(aig["latch_names"], state) if b]
        return " ".join(on) if on else "(all low)"
    states = [{"id": f"s{seen[s]}", "label": label(s), "init": s == init} for s in order]
    edges = [{"from": f"s{seen[s]}", "to": f"s{seen[t]}", "label": _cover(cubes, names)}
             for s in order for t, cubes in moves[s].items()]
    return {"states": states, "edges": edges, "count": len(order),
            "registers": aig["latch_names"]}


def _cover(minterms, names) -> str:
    """A short sum of products of the input values (Quine-McCluskey)."""
    n = len(names)
    if len(minterms) == 2 ** n:
        return ""
    terms = {tuple(m) for m in minterms}
    primes = set()
    while terms:
        merged, used = set(), set()
        for a, b in itertools.combinations(sorted(terms), 2):
            diff = [i for i in range(n) if a[i] != b[i]]
            if len(diff) == 1:
                c = list(a)
                c[diff[0]] = None
                merged.add(tuple(c))
                used |= {a, b}
        primes |= terms - used
        terms = merged
    # a cover: the primes in order of size, while they add minterms
    left, cover = {tuple(m) for m in minterms}, []
    for p in sorted(primes, key=lambda p: sum(x is not None for x in p)):
        hit = {m for m in left if all(x is None or x == y for x, y in zip(p, m))}
        if hit:
            cover.append(p)
            left -= hit
    words = []
    for p in cover:
        lits = [("" if x else "!") + nm for x, nm in zip(p, names) if x is not None]
        words.append(" & ".join(lits) or "true")
    return " | ".join(words)


# State machines in JSON -----------------------------------------------------------------

def _json(d: dict, max_states: int) -> dict:
    states = list(d.get("states", []))
    if len(states) > max_states:
        return {"count": len(states), "too_many": True}
    initial = d.get("initial", states[0] if states else None)
    initial = {initial} if isinstance(initial, str) else set(initial or [])
    labels = {}
    for t in d.get("transitions", []):
        when = t.get("when") or ""
        outs = ", ".join(f"{o}={int(v) if isinstance(v, bool) else v}"
                         for o, v in t.get("outputs", {}).items())
        label = when + (" / " + outs if outs else "")
        labels.setdefault((t["from"], t["to"]), []).append(label.strip())
    return {"states": [{"id": s, "label": s, "init": s in initial} for s in states],
            "edges": [{"from": a, "to": b, "label": "; ".join(l for l in ls if l)}
                      for (a, b), ls in labels.items()],
            "count": len(states)}


# SMV ------------------------------------------------------------------------------------

def _smv(model, inputs: set, max_states: int, max_inputs: int) -> Optional[dict]:
    """The reachable states of an SMV model, and its transitions: nuXmv (or
    NuSMV) lists the reachable states of the model with a copy of every
    variable one step late, which are the reachable transitions."""
    from .models import _smvtype
    from .tools import find
    binary = find("nuXmv") or find("NuSMV")
    if binary is None:
        return None
    variables = {v: t for v, t in model.variables.items()
                 if t != "define" and v not in model.ivars}
    if any(t == "real" for t in variables.values()):
        return None
    ins = sorted(v for v in variables if v in inputs)
    if len(ins) > max_inputs:
        return None
    lines = ["MODULE main", "VAR", "    chase_started : boolean;"]
    lines += [f"    chase_prev_{v} : {_smvtype(t)};" for v, t in variables.items()]
    lines += ["ASSIGN", "    init(chase_started) := FALSE;", "    next(chase_started) := TRUE;"]
    lines += [f"    next(chase_prev_{v}) := {v};" for v in variables]
    # the copies go right after MODULE main, before any other module
    text = model.smv.replace("MODULE main", "\n".join(lines), 1)
    with tempfile.TemporaryDirectory(prefix="chase_fsm_") as tmp:
        path = os.path.join(tmp, "m.smv")
        with open(path, "w") as f:
            f.write(text)
        p = subprocess.run([binary, "-int"], input=f"read_model -i {path}\ngo\n"
                           "print_reachable_states -v\nquit\n", capture_output=True,
                           text=True, timeout=120)
    states, current = [], None
    for line in p.stdout.splitlines():
        line = line.strip()
        if line.startswith("------- State"):
            current = {}
            states.append(current)
        elif current is not None and " = " in line:
            k, v = line.split(" = ", 1)
            current[k.strip()] = v.strip()
    if not states:
        return None
    keep = [v for v in variables if v not in inputs]

    def part(st, prefix=""):
        return tuple(st.get(prefix + v) for v in keep)

    def label(key):
        on = [f"{v}" if val == "TRUE" else f"{v}={val}" for v, val in zip(keep, key)
              if val not in ("FALSE", "0")]
        return " ".join(on) if on else "(all low)"
    found, order, init, moves = {}, [], set(), {}
    for st in states:
        cur = part(st)
        if cur not in found:
            found[cur] = len(order)
            order.append(cur)
        if st.get("chase_started") == "FALSE":
            init.add(cur)
        else:
            prev = part(st, "chase_prev_")
            if prev not in found:
                found[prev] = len(order)
                order.append(prev)
            values = tuple(1 if st.get("chase_prev_" + v) == "TRUE" else 0 for v in ins)
            moves.setdefault((prev, cur), set()).add(values)
    if len(order) > max_states:
        return {"count": len(order), "too_many": True}
    return {"states": [{"id": f"s{found[s]}", "label": label(s), "init": s in init} for s in order],
            "edges": [{"from": f"s{found[a]}", "to": f"s{found[b]}", "label": _cover(sorted(ms), ins)}
                      for (a, b), ms in moves.items()],
            "count": len(order)}


# Controllers -------------------------------------------------------------------------

def _controller(controller: dict, max_states: int) -> dict:
    nodes = controller["nodes"]
    if len(nodes) > max_states:
        return {"count": len(nodes), "too_many": True}
    names = controller["variables"]

    def label(node):
        on = [nm for nm, b in zip(names, node["state"]) if b]
        return " ".join(on) if on else "(all low)"
    states = [{"id": f"s{k}", "label": label(v), "init": False} for k, v in nodes.items()]
    edges = [{"from": f"s{k}", "to": f"s{t}", "label": ""}
             for k, v in nodes.items() for t in v["trans"]]
    return {"states": states, "edges": edges, "count": len(nodes)}
