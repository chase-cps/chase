"""An SMV model with its specifications, as a component: the program of
nuXmv or NuSMV, and the properties it is meant to have.

    MODULE main
    IVAR req : boolean;
    VAR  grant : boolean;
    ASSIGN init(grant) := FALSE;
           next(grant) := req;
    LTLSPEC G (req -> X grant)
    INVARSPEC !(req & grant)

``load_smv(path)`` reads the module main of the file as a component: its
contract has no assumption, and a guarantee for each specification
(``LTLSPEC p`` is p, ``INVARSPEC p`` is ``G p``); the model, without the
specifications, is its implementation, checked with nuXmv or NuSMV. The
inputs are the IVAR variables, the outputs the VAR and FROZENVAR ones. A
specification in CTL (SPEC, CTLSPEC), or with past operators, is not read.

An enumerated type ({idle, busy}) is read as the integers 0, 1, ... in the
order written, in the contract and in the model alike (the traces show the
numbers); a DEFINE used in a specification is replaced by its definition
(one written with case ... esac is not read).
"""

from __future__ import annotations

import os
import re

from . import ltl
from .contract import Contract
from .models import _KEYWORD, _DECL, Model, _vartype

_LTL = ("LTLSPEC", "INVARSPEC")
_OTHER = ("SPEC", "CTLSPEC", "PSLSPEC", "COMPUTE")


def _blank(text: str) -> str:
    """The comments blanked out: the offsets stay those of the file."""
    return re.sub(r"--[^\n]*", lambda m: " " * len(m.group(0)), text)


def _main(text: str) -> tuple:
    """(start, end) of the module main."""
    m = re.search(r"\bMODULE\s+main\b", text)
    if not m:
        raise ValueError("the SMV model has no MODULE main")
    nxt = re.search(r"\bMODULE\b", text[m.end():])
    return m.end(), (m.end() + nxt.start() if nxt else len(text))


def _sections(text: str) -> list:
    """(keyword, start of the body, end of the body, start of the keyword)
    of the sections of the module main."""
    start, end = _main(text)
    marks = [(k.group(1), k.start(), k.end()) for k in _KEYWORD.finditer(text, start, end)]
    return [(kw, b, (marks[i + 1][1] if i + 1 < len(marks) else end), a)
            for i, (kw, a, b) in enumerate(marks)]


def has_specs(path: str) -> bool:
    try:
        with open(path, encoding="utf-8", errors="replace") as f:
            text = _blank(f.read())
        return any(kw in _LTL + _OTHER for kw, *_ in _sections(text))
    except (OSError, ValueError):
        return False


_NAME = r"[A-Za-z_][\w$#-]*"


def _words(text: str, values: dict) -> str:
    """text with each name of values replaced by its value."""
    if not values:
        return text
    pattern = re.compile(r"(?<![\w$#.-])(" + "|".join(map(re.escape, values)) + r")(?![\w$#-])")
    return pattern.sub(lambda m: str(values[m.group(1)]), text)


def _defines(body: str) -> dict:
    """{name: definition} of a DEFINE section."""
    marks = list(re.finditer(r"(" + _NAME + r")\s*:=", body))
    return {m.group(1): body[m.end():(marks[i + 1].start() if i + 1 < len(marks) else len(body))]
            .strip().rstrip(";").strip() for i, m in enumerate(marks)}


def _expand(e: str, defines: dict, keyword: str) -> str:
    """The DEFINEs of e replaced by their definitions, in parentheses."""
    for _ in range(50):
        used = [d for d in defines if re.search(r"(?<![\w$#.-])" + re.escape(d) + r"(?![\w$#-])", e)]
        if not used:
            return e
        for d in used:
            if re.search(r"\bcase\b", defines[d]):
                raise ValueError(f"{keyword} {e}: the DEFINE {d} is written with case ... esac, "
                                 "which a contract does not read: write the condition instead")
        e = _words(e, {d: f"({defines[d]})" for d in used})
    raise ValueError(f"{keyword}: the DEFINEs refer to each other without end")


def _formula(spec: str, keyword: str, defines: dict, symbols: dict = None) -> ltl.Node:
    e = re.sub(r"^\s*NAME\s+[\w$#-]+\s*:=", "", spec).strip().rstrip(";")
    if re.search(r"\b(Y|Z|H|O|S|T)\b", e):
        raise ValueError(f"{keyword} {e}: past operators are not read")
    e = _words(_expand(e, defines, keyword), symbols or {})
    e = re.sub(r"\bnext\s*\(\s*([A-Za-z_]\w*)\s*\)", r"\1'", e)
    e = re.sub(r"\bTRUE\b", "true", re.sub(r"\bFALSE\b", "false", e))
    e = re.sub(r"\bV\b", "R", e)
    try:
        node = ltl.parse(e)
    except ltl.LTLSyntaxError as err:
        raise ValueError(f"cannot read {keyword} {e}: {err}") from None
    return ltl.Unary("G", node) if keyword == "INVARSPEC" else node


def _read(path: str) -> tuple:
    """(the contract, the model, [(keyword, first line, last line)] of each
    specification), from an SMV file."""
    with open(path, encoding="utf-8", errors="replace") as f:
        original = f.read()
    text = _blank(original)
    name = os.path.splitext(os.path.basename(path))[0]
    inputs, outputs, defines, specs, symbols = {}, {}, {}, [], {}
    model = list(original)
    for kw, start, stop, at in _sections(text):
        body = text[start:stop]
        if kw in ("VAR", "IVAR", "FROZENVAR"):
            for m in _DECL.finditer(body):
                t = m.group(2).strip()
                if t.startswith("{") and t.endswith("}"):     # an enumeration: 0, 1, ...
                    names = [x.strip() for x in t[1:-1].split(",") if x.strip()]
                    for i, x in enumerate(names):
                        if not re.fullmatch(_NAME, x):
                            raise ValueError(f"{os.path.basename(path)}: the value {x} of {m.group(1)} "
                                             "is not a name: write the variable as a range a..b")
                        if symbols.get(x, i) != i:
                            raise ValueError(f"{os.path.basename(path)}: {x} is the value {symbols[x]} "
                                             f"of an enumeration and {i} of {m.group(1)}: write them "
                                             "in the same order")
                        symbols[x] = i
                    t = f"0..{len(names) - 1}"
                (inputs if kw == "IVAR" else outputs)[m.group(1)] = _vartype(t)
        elif kw == "DEFINE":
            defines.update(_defines(body))
        elif kw in _OTHER:
            raise ValueError(f"{os.path.basename(path)}: {kw} is not read (CTL); LTLSPEC and "
                             "INVARSPEC are")
        if kw in _LTL:
            specs.append((kw, body, original.count("\n", 0, at) + 1,
                          original.count("\n", 0, at + len(body.rstrip()) + len(kw)) + 1))
            for i in range(at, stop):        # the model without its specifications
                if model[i] != "\n":
                    model[i] = " "
    clash = set(symbols) & (set(inputs) | set(outputs) | set(defines))
    if clash:
        raise ValueError(f"{os.path.basename(path)}: {', '.join(sorted(clash))} is both a value of an "
                         "enumeration and a variable or a DEFINE")
    for v, t in list(outputs.items()) + list(inputs.items()):
        if not (t in ("bool", "real") or isinstance(t, tuple)):
            raise ValueError(f"{os.path.basename(path)}: the type of {v} ({t}) is not read; "
                             "boolean, a range a..b, and real are")
    guarantees = [_formula(body, kw, defines, symbols) for kw, body, _, _ in specs]
    contract = Contract.from_strings(name=name, inputs=inputs, outputs=outputs,
                                     guarantees=guarantees)
    program = "".join(model)
    if symbols:                       # the enumerations as numbers, in the model too
        program = re.sub(r"\{[^{}]*\}", lambda m: "0.." + str(len([x for x in m.group(0)[1:-1].split(",")
                                                                       if x.strip()]) - 1)
                         if all(x.strip() in symbols for x in m.group(0)[1:-1].split(",") if x.strip())
                         else m.group(0), program)
        program = _words(program, symbols)
    return contract, Model.from_smv(program, name=name), [(kw, a, b) for kw, _, a, b in specs]


def load_smv(path: str) -> dict:
    """{name: (contract, model)}: the module main of the SMV file, with its
    specifications as the guarantees of its contract."""
    contract, model, _ = _read(path)
    return {contract.name: (contract, model)}


def model(path: str) -> Model:
    """The model of the file, without its specifications."""
    return _read(path)[1]


def places(path: str) -> tuple:
    """({name: the line of MODULE main}, {name: {"A": [], "G": [(first,
    last)] of each term}}), for an editor. A specification that is a
    conjunction gives each of its terms its lines."""
    with open(path, encoding="utf-8", errors="replace") as f:
        original = f.read()
    contract, _, specs = _read(path)
    head = original.count("\n", 0, re.search(r"\bMODULE\s+main\b", _blank(original)).start()) + 1
    terms = [(a, b) for (kw, a, b), f in zip(specs, contract_formulas(path)) for _ in ltl.conjuncts(f)]
    return {contract.name: head}, {contract.name: {"A": [], "G": terms}}


def contract_formulas(path: str) -> list:
    """The formula of each specification of the file, in order."""
    with open(path, encoding="utf-8", errors="replace") as f:
        text = _blank(f.read())
    defines, symbols = {}, {}
    for kw, start, stop, _ in _sections(text):
        if kw == "DEFINE":
            defines.update(_defines(text[start:stop]))
        elif kw in ("VAR", "IVAR", "FROZENVAR"):
            for m in _DECL.finditer(text[start:stop]):
                t = m.group(2).strip()
                if t.startswith("{") and t.endswith("}"):
                    for i, x in enumerate(x.strip() for x in t[1:-1].split(",") if x.strip()):
                        symbols.setdefault(x, i)
    return [_formula(text[start:stop], kw, defines, symbols)
            for kw, start, stop, _ in _sections(text) if kw in _LTL]
