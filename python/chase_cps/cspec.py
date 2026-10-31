"""Contracts written in C, next to the code: the clauses of the function
contracts of ESBMC and of CBMC.

    int thermostat(int T)
    {
        __ESBMC_requires(T >= 0 && T <= 200);
        __ESBMC_ensures((T < 74 && __ESBMC_return_value == 3) ||
                        (T >= 74 && __ESBMC_return_value == 0));
        ...
    }

    int thermostat(int T)
        __CPROVER_requires(T >= 0 && T <= 200)
        __CPROVER_ensures(T >= 74 || __CPROVER_return_value == 3)
    { ... }

``load_c(path)`` reads each function of the file as a component: its
contract (the requires clauses are its assumptions, the ensures clauses its
guarantees), and the function as its implementation. The parameters are the
inputs; the return value is the output FUNCTION_out, and a
pointer parameter is the output of its name (``*p`` in a clause). The
ranges of the integer inputs come from their C types, narrowed by the
bounds the requires clauses give (``T >= 0 && T <= 200``). A function with
no clause has a contract with no assumption and no guarantee: its check is
that of the code itself (no overflow, division by zero, access out of
bounds, or failed assert).

The clauses are expressions over the inputs and the outputs, with && || !
== != < <= > >= + - * /; a conditional (a ? b : c) is not read. The
function is checked with the clauses taken out of its file (each checker
reads the clauses of the other as an error), against the contract read
from them. The clauses on pointers (is_fresh, r_ok, w_ok, rw_ok, and the
assigns clause) are not part of the contract: the harness passes a valid
pointer for each output.
"""

from __future__ import annotations

import os
import re

from . import ltl
from .contract import Contract
from .cprograms import CFunction, CProgramError, _signature

_CLAUSE = re.compile(r"\b__(ESBMC|CPROVER)_(requires|ensures|assigns)\s*\(")
_POINTER = re.compile(r"__(ESBMC|CPROVER)_(is_fresh|r_ok|w_ok|rw_ok)\b")
_RANGES = {"char": (-128, 127), "signed char": (-128, 127), "unsigned char": (0, 255),
           "short": (-32768, 32767), "unsigned short": (0, 65535),
           "int": (-2 ** 31, 2 ** 31 - 1), "unsigned": (0, 2 ** 32 - 1),
           "unsigned int": (0, 2 ** 32 - 1), "long": (-2 ** 63, 2 ** 63 - 1),
           "long long": (-2 ** 63, 2 ** 63 - 1)}


def _closing(text: str, start: int) -> int:
    """The index of the parenthesis that closes the one before start."""
    depth = 1
    for i in range(start, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return i
    raise CProgramError("a clause is not closed")


def _functions(text: str) -> list:
    """(name, start of its definition, end of its body) of each function
    defined in the text (comments blanked out)."""
    out = []
    head = re.compile(r"(?:^|[;}\n])\s*((?:[A-Za-z_]\w*[\s*]+)+?)([A-Za-z_]\w*)\s*\(([^()]*)\)")
    for m in head.finditer(text):
        name = m.group(2)
        if name in ("if", "while", "for", "switch", "return", "sizeof"):
            continue
        i = m.end()
        while True:                              # the clauses of CBMC, between ) and {
            rest = text[i:]
            c = re.match(r"\s*__CPROVER_(requires|ensures|assigns)\s*\(", rest)
            if not c:
                break
            i = _closing(text, i + c.end()) + 1
        b = re.match(r"\s*\{", text[i:])
        if not b:
            continue                             # a declaration
        depth, j = 0, i + b.end() - 1
        while j < len(text):
            depth += {"{": 1, "}": -1}.get(text[j], 0)
            if depth == 0:
                break
            j += 1
        out.append((name, m.start(1), j + 1))
    return out


def _blank_comments(text: str) -> str:
    return re.sub(r"/\*.*?\*/|//[^\n]*", lambda m: re.sub(r"[^\n]", " ", m.group(0)), text, flags=re.S)


def _terms(expr: str) -> list:
    """The operands of the && at the top of the expression."""
    out, depth, last = [], 0, 0
    for i, ch in enumerate(expr):
        depth += {"(": 1, ")": -1}.get(ch, 0)
        if depth == 0 and expr.startswith("&&", i):
            out.append(expr[last:i])
            last = i + 2
    return out + [expr[last:]]


def _formula(expr: str, result: str, pointers: set):
    """A clause in C as a formula of CHASE (None when it only says that the
    pointers are valid: the harness passes valid pointers)."""
    kept = [t for t in _terms(expr) if not _POINTER.search(t)]
    if not kept:
        return None
    e = " && ".join(kept)
    if result is not None:
        e = re.sub(r"__(ESBMC|CPROVER)_return_value", result, e)
    for p in pointers:                           # *p: the output p
        e = re.sub(r"(^|[^\w)\]\s])(\s*)\*\s*" + re.escape(p) + r"\b", r"\1\2" + p, e)
    e = e.replace("&&", " & ").replace("||", " | ").replace("==", " = ")
    try:
        return ltl.parse(e)
    except ltl.LTLSyntaxError as err:
        raise CProgramError(f"cannot read the clause {expr.strip()!r}: {err}") from None


def _bounds(node, names) -> dict:
    """{variable: [low, high]} from the conjuncts v >= c, v <= c (and c <= v, ...)."""
    out = {}
    for t in ltl.conjuncts(node):
        if not (isinstance(t, ltl.Binary) and t.op in ("<=", ">=", "<", ">")):
            continue
        a, b, op = t.left, t.right, t.op
        if isinstance(a, ltl.Num) and isinstance(b, ltl.Var):
            a, b, op = b, a, {"<=": ">=", ">=": "<=", "<": ">", ">": "<"}[op]
        if isinstance(a, ltl.Var) and a.name in names and isinstance(b, ltl.Num) and \
                float(b.value).is_integer():
            v = int(b.value)
            lo, hi = out.setdefault(a.name, [None, None])
            if op in (">=", ">"):
                out[a.name][0] = v + (1 if op == ">" else 0) if lo is None else max(lo, v)
            else:
                out[a.name][1] = v - (1 if op == "<" else 0) if hi is None else min(hi, v)
    return out


def _type(ctype: str, bounds) -> object:
    if ctype in ("bool", "_Bool"):
        return "bool"
    if ctype in ("float", "double", "long double"):
        return "real"
    if ctype not in _RANGES:
        raise CProgramError(f"the type {ctype} is not supported")
    lo, hi = _RANGES[ctype]
    if bounds:
        lo = bounds[0] if bounds[0] is not None else lo
        hi = bounds[1] if bounds[1] is not None else hi
    return (lo, hi)


def _read(path: str) -> tuple:
    """The functions of the file, each (name, its first line, its signature,
    the name of its return value, {"requires", "ensures": [(formula, first
    line, last line)]}), and the code without the clauses."""
    with open(path, encoding="utf-8", errors="replace") as f:
        original = f.read()
    text = _blank_comments(original)
    stripped = list(original)
    line = lambda i: text.count("\n", 0, i) + 1
    found = []
    for name, start, end in _functions(text):
        clauses = {"requires": [], "ensures": [], "assigns": []}
        for m in _CLAUSE.finditer(text, start, end):
            close = _closing(text, m.end())
            clauses[m.group(2)].append((text[m.end():close], line(m.start()), line(close)))
            stop = close + 1                     # the clause out of the code (with its ; when a statement)
            semi = re.match(r"\s*;", text[stop:])
            if semi:
                stop += semi.end()
            for i in range(m.start(), stop):
                if stripped[i] != "\n":
                    stripped[i] = " "
        sig = _signature(_blank_comments("".join(stripped)), name)
        pointers = {p.name for p in sig.parameters if p.pointer}
        result = f"{name}_out" if sig.returns != "void" else None
        read = {k: [(f, a, b) for f, a, b in ((_formula(e, result, pointers), a, b)
                                              for e, a, b in clauses[k]) if f]
                for k in ("requires", "ensures")}
        found.append((name, line(start), sig, result, read))
    if not found:
        raise CProgramError(f"{os.path.basename(path)} defines no function")
    return found, "".join(stripped)


def load_c(path: str) -> dict:
    """{name: (contract, CFunction)} for each function of the C file."""
    found, code = _read(path)
    out = {}
    for name, _, sig, result, read in found:
        assumptions = [f for f, _, _ in read["requires"]]
        guarantees = [f for f, _, _ in read["ensures"]]
        bounds = _bounds(ltl.conjunction(assumptions), {p.name for p in sig.parameters}) \
            if assumptions else {}
        inputs = {p.name: _type(p.type, bounds.get(p.name)) for p in sig.parameters if not p.pointer}
        outputs = {p.name: _type(p.type, None) for p in sig.parameters if p.pointer}
        if result is not None:
            outputs[result] = _type(sig.returns, None)
        c = Contract.from_strings(name=name, inputs=inputs, outputs=outputs,
                                  assumptions=assumptions, guarantees=guarantees)
        out[name] = (c, CFunction(path, name, result=result, source=code))
    return out


def places(path: str) -> tuple:
    """({function: its first line}, {function: {"A", "G": [(first line, last
    line)] of each term}}): where the boxes and their terms are written, for
    an editor. A clause of several conjuncts gives each of them its lines."""
    found, _ = _read(path)
    lines, terms = {}, {}
    for name, first, _, _, read in found:
        lines[name] = first
        terms[name] = {p: [(a, b) for f, a, b in read[k] for _ in ltl.conjuncts(f)]
                       for p, k in (("A", "requires"), ("G", "ensures"))}
    return lines, terms


def has_clauses(path: str) -> bool:
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            return bool(_CLAUSE.search(_blank_comments(f.read())))
    except OSError:
        return False


def function(path: str, name: str) -> CFunction:
    """The function of the file, with the clauses taken out of the code."""
    functions = load_c(path)
    if name not in functions:
        raise CProgramError(f"{os.path.basename(path)} defines no function {name}")
    return functions[name][1]
