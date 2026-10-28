"""SystemVerilog Assertions (SVA, IEEE 1800): contracts to SVA and back.

``to_sva(contract)`` writes a checker module: the assumptions as ``assume
property``, the guarantees as ``assert property``, and, with ``bind``, a
bind statement that puts the checker in a design. Any tool that reads SVA
can check it (formal tools and simulators); nothing in it is specific to a
tool.

``parse_sva(text)`` reads the ``assume property`` and ``assert property``
statements of a text (a module, a bind file, or the statements alone) as a
contract: the assumptions and the guarantees. The properties that cannot be
written in LTL are listed in ``skipped``, with the reason.

Meaning: that of IEEE 1800-2017 (chapter 16, and Annex F for ``disable
iff``), not that of a tool; tools may differ in corner cases (a reset that
comes while a sequence is still running, after the failure is decided). One
clock: one step of LTL is one cycle of the clock.

- A concurrent assertion is checked at every cycle: ``assert property (p)``
  is the guarantee ``G p``; ``initial assert property (p)`` is ``p``.
- Expressions keep the widths of Verilog: the operands of an operation are
  extended to the width of the context first (``x + 1`` has 32 bits, as the
  constant 1), and an expression is unsigned unless all its operands are
  signed. Vectors become words (``logic [7:0] x`` is ``word[8]``), 1-bit
  signals Booleans; the translation keeps the width of an operation only
  where it can overflow.
- ``$past(e, n)`` is the value of e n cycles before. A property that looks
  before the first cycle is checked from the cycle where the past exists
  (cycle 1 for ``x == $past(x)``), as with the usual ``f_past_valid`` flag.
  ``$rose``, ``$fell``, ``$stable``, ``$changed``, ``$onehot``,
  ``$onehot0``, ``$countones`` are read from it; ``$isunknown`` is false
  (two values, as in formal tools).
- Sequences of a bounded length (``##n``, ``##[m:n]``, ``[*m:n]``, ``and``,
  ``or``, ``intersect``, ``throughout``, ``within``, ``first_match``) are
  exact. ``##[m:$]`` is read in an antecedent and in a strong sequence
  (``strong(...)``, ``#-#``); in a consequent it is weak, so it never fails:
  write ``s_eventually`` for a liveness property.
- ``disable iff (r)`` is ``accept_on (r)``: the attempt passes when r holds
  before it fails. ``default clocking`` and ``default disable iff`` apply.
- Properties: ``not and or implies iff |-> |=> #-# #=# if/else nexttime
  always s_always eventually s_eventually until s_until until_with
  s_until_with strong weak``, named properties and sequences (with
  arguments), ``assign`` and ``wire x = e`` definitions (put in place).

Values one cycle apart in a comparison are written with x', values further
apart with ``past(x, n)`` (``y == $past(x, 2)``).

Not read: local variables, ``[->n]`` and ``[=n]``, unbounded repetition,
``$past`` with a gating clock, ``x`` and ``z`` digits, unpacked arrays. A
contract has one clock: the properties of the other clocks are skipped.
"""

from __future__ import annotations

import re
from dataclasses import dataclass, field
from typing import Optional

from . import ltl, words
from .contract import Contract


class SVAError(ValueError):
    """A property outside what CHASE reads or writes (the message says why)."""


# Lexer -----------------------------------------------------------------------

_TOKEN = re.compile(r"""
  (?P<ws>\s+|//[^\n]*|/\*.*?\*/)
| (?P<attr>\(\*.*?\*\))
| (?P<num>\d[\d_]*\s*'[sS]?[bBoOdDhH]\s*[0-9a-fA-FxXzZ?_]+
         |'[sS]?[bBoOdDhH]\s*[0-9a-fA-FxXzZ?_]+
         |'[01xXzZ](?![\w'])
         |\d[\d_]*\.\d+
         |\d[\d_]*)
| (?P<sys>\$[A-Za-z_]\w*)
| (?P<macro>`\w+)
| (?P<id>[A-Za-z_][\w$]*)
| (?P<str>"(?:[^"\\]|\\.)*")
| (?P<op>\|->|\|=>|\#-\#|\#=\#|\#\#|<<<|>>>|===|!==|==\?|!=\?|<->|->|\[\*|\[=|\[->|\[\+\]
        |\+:|-:|==|!=|<=|>=|&&|\|\||<<|>>|~&|~\||~\^|\^~|\*\*|::
        |[()\[\]{},;:?@\#!~&|^+\-*/%<>=.'$])
""", re.S | re.X)


@dataclass(frozen=True)
class _Num:
    value: int
    width: Optional[int]       # None: unsized (32 bits), or a fill ('0, '1)
    signed: bool
    fill: bool = False         # '0 and '1 take the width of the context
    unknown: bool = False      # x or z digits


def _number(text: str) -> _Num:
    text = text.replace(" ", "").replace("\t", "").replace("_", "")
    if re.fullmatch(r"\d+\.\d+", text):
        raise SVAError(f"the real number {text}")
    if re.fullmatch(r"\d+", text):
        return _Num(int(text), None, True)
    if re.fullmatch(r"'[01xXzZ]", text):
        if text[1] in "xXzZ":
            return _Num(0, None, False, fill=True, unknown=True)
        return _Num(int(text[1]), None, False, fill=True)
    m = re.fullmatch(r"(\d*)'([sS]?)([bBoOdDhH])(.+)", text)
    width = int(m.group(1)) if m.group(1) else None
    digits = m.group(4)
    unknown = bool(re.search(r"[xXzZ?]", digits))
    base = {"b": 2, "o": 8, "d": 10, "h": 16}[m.group(3).lower()]
    value = 0 if unknown else int(digits, base)
    if width is not None:
        if width == 0:
            raise SVAError(f"a number of width 0: {text}")
        value &= (1 << width) - 1
    return _Num(value, width, bool(m.group(2)), unknown=unknown)


_DIRECTIVE = re.compile(r"^[ \t]*`(define|undef|ifdef|ifndef|elsif|else|endif|include|timescale|"
                        r"default_nettype|resetall|celldefine|endcelldefine|pragma|line)\b"
                        r"(?:[^\n]*\\\n)*[^\n]*", re.M)


def _tokens(text: str) -> list[tuple[str, str]]:
    # preprocessor lines are dropped (both branches of an `ifdef are read);
    # a macro used in a statement makes that statement skipped
    text = _DIRECTIVE.sub("", text)
    text = re.sub(r"`(\w+)", r"`\1", text)
    out, pos = [], 0
    while pos < len(text):
        m = _TOKEN.match(text, pos)
        if not m:
            raise SVAError(f"unexpected character {text[pos]!r} near {text[pos:pos + 30]!r}")
        pos = m.end()
        kind = m.lastgroup
        if kind == "ws":
            continue
        out.append((kind, m.group(kind)))
    out.append(("end", ""))
    return out


# Syntax tree ------------------------------------------------------------------

class N:
    """A node: kind, operands, and a value (name, operator, number, range)."""
    __slots__ = ("k", "a", "v")

    def __init__(self, k: str, a=(), v=None):
        self.k, self.a, self.v = k, tuple(a), v

    def __repr__(self):
        return f"N({self.k!r}, {list(self.a)!r}, {self.v!r})"


_PROPERTY_PREFIX = {"always", "s_always", "eventually", "s_eventually", "nexttime",
                    "s_nexttime", "not", "if", "strong", "weak", "first_match",
                    "accept_on", "reject_on", "sync_accept_on", "sync_reject_on", "case"}
# operator: (left binding power, right binding power)
_INFIX = {
    "|->": (1, 1), "|=>": (1, 1), "#-#": (1, 1), "#=#": (1, 1),
    "until": (2, 2), "s_until": (2, 2), "until_with": (2, 2), "s_until_with": (2, 2),
    "implies": (2, 2), "iff": (3, 3), "or": (4, 5), "and": (5, 6),
    "intersect": (7, 8), "within": (8, 9), "throughout": (9, 9), "##": (10, 11),
    "->": (12, 12), "<->": (12, 12), "?": (13, 13),
    "||": (14, 15), "&&": (15, 16), "|": (16, 17), "^": (17, 18), "~^": (17, 18),
    "^~": (17, 18), "&": (18, 19),
    "==": (19, 20), "!=": (19, 20), "===": (19, 20), "!==": (19, 20), "==?": (19, 20),
    "!=?": (19, 20),
    "<": (20, 21), "<=": (20, 21), ">": (20, 21), ">=": (20, 21), "inside": (20, 21),
    "<<": (21, 22), ">>": (21, 22), "<<<": (21, 22), ">>>": (21, 22),
    "+": (22, 23), "-": (22, 23), "*": (23, 24), "/": (23, 24), "%": (23, 24),
    "**": (24, 24),
}
_WORD_INFIX = {"until", "s_until", "until_with", "s_until_with", "implies", "iff", "or",
               "and", "intersect", "within", "throughout", "inside"}
_UNARY = {"!", "~", "&", "|", "^", "~&", "~|", "~^", "^~", "+", "-"}
_STOP = {"endproperty", "endsequence", "else", "endmodule"}


class _Parser:
    def __init__(self, tokens, text=""):
        self.t, self.i, self.text = tokens, 0, text
        self.names: dict = {}          # a__b -> a.b (hierarchical names)

    def peek(self, k=0):
        return self.t[min(self.i + k, len(self.t) - 1)]

    def at(self, *values):
        return self.peek()[1] in values and self.peek()[0] in ("op", "id")

    def take(self):
        tok = self.t[self.i]
        self.i += 1
        return tok

    def expect(self, value):
        tok = self.take()
        if tok[1] != value:
            raise SVAError(f"expected {value!r}, found {tok[1] or 'the end'!r}")
        return tok

    def infix(self):
        kind, val = self.peek()
        if kind == "op" and val in _INFIX and val not in _WORD_INFIX:
            return val
        if kind == "id" and val in _WORD_INFIX:
            return val
        return None

    # property, sequence, expression: one precedence climbing parser
    def parse(self, min_bp: int = 0) -> N:
        left = self.prefix()
        while True:
            if min_bp <= 11 and self.at("[*", "[=", "[->", "[+]"):
                left = self.repetition(left)    # binds an expression or a sequence
                continue
            op = self.infix()
            if op is None:
                return left
            lbp, rbp = _INFIX[op]
            if lbp < min_bp:
                return left
            self.take()
            if op == "##":
                d = self.delay()
                left = N("delay", [left, self.parse(rbp)], d)
            elif op == "?":
                then = self.parse(13)
                self.expect(":")
                left = N("cond", [left, then, self.parse(rbp)])
            elif op == "inside":
                left = N("inside", [left] + self.set_items())
            else:
                left = N("bin", [left, self.parse(rbp)], op)

    def delay(self):
        """The delay after ##: n, (e), [m:n], [m:$], [*], [+]."""
        if self.at("[*"):
            self.take()
            self.expect("]")
            return (N("num", v=_Num(0, None, True)), None)
        if self.at("[+]"):
            self.take()
            return (N("num", v=_Num(1, None, True)), None)
        if self.at("["):
            self.take()
            lo = self.parse(13)
            self.expect(":")
            if self.at("$"):
                self.take()
                hi = None
            else:
                hi = self.parse(13)
            self.expect("]")
            return (lo, hi)
        if self.at("("):
            self.take()
            n = self.parse(0)
            self.expect(")")
            return (n, n)
        kind, val = self.take()
        if kind == "num":
            n = N("num", v=_number(val))
        elif kind == "id":
            n = N("id", v=val)
        else:
            raise SVAError(f"a delay ## needs a number, not {val!r}")
        return (n, n)

    def set_items(self):
        self.expect("{")
        items = []
        while True:
            if self.at("["):
                self.take()
                lo = self.parse(13)
                self.expect(":")
                hi = self.parse(13)
                self.expect("]")
                items.append(N("range", [lo, hi]))
            else:
                items.append(self.parse(13))
            if self.at(","):
                self.take()
                continue
            self.expect("}")
            return items

    def range_after_keyword(self):
        """[m:n] or [m:$] or [n] after always, eventually, nexttime."""
        if not self.at("["):
            return None
        self.take()
        lo = self.parse(13)
        hi = lo
        if self.at(":"):
            self.take()
            if self.at("$"):
                self.take()
                hi = None
            else:
                hi = self.parse(13)
        self.expect("]")
        return (lo, hi)

    def prefix(self) -> N:
        kind, val = self.peek()
        if kind == "op" and val == "##":
            self.take()
            d = self.delay()
            return N("delay", [None, self.parse(11)], d)
        if kind == "op" and val == "(":
            self.take()
            node = self.parse(0)
            self.expect(")")
            return self.postfix(self.cast(N("paren", [node])))
        if kind == "op" and val == "{":
            return self.postfix(self.concat())
        if kind == "op" and val in _UNARY:
            self.take()
            return N("un", [self.parse(25)], val)
        if kind == "num":
            self.take()
            num = N("num", v=_number(val))
            return self.postfix(self.cast(num))
        if kind == "macro":
            raise SVAError(f"the macro {val}")
        if kind == "sys":
            self.take()
            args = self.arguments() if self.at("(") else []
            return self.postfix(N("call", args, val))
        if kind == "id" and val in _PROPERTY_PREFIX:
            self.take()
            return self.property_prefix(val)
        if kind == "id" and val in ("signed", "unsigned") and self.peek(1)[1] == "'":
            self.take()
            self.take()
            self.expect("(")
            arg = self.parse(0)
            self.expect(")")
            return self.postfix(N("cast", [arg], val))
        if kind == "id" and val not in _STOP:
            self.take()
            node = N("id", v=val)
            if self.at("("):
                node = N("call", self.arguments(), val)
            return self.postfix(self.cast(node))
        raise SVAError(f"unexpected {val or 'end of text'!r}")

    def cast(self, node: N) -> N:
        """width'(e) after a number or a parameter."""
        if self.at("'") and self.peek(1)[1] == "(":
            self.take()
            self.take()
            arg = self.parse(0)
            self.expect(")")
            return N("cast", [arg], node)
        return node

    def arguments(self):
        self.expect("(")
        args = []
        if self.at(")"):
            self.take()
            return args
        while True:
            if self.at(","):           # an empty argument ($past(e, , en))
                args.append(None)
            else:
                args.append(self.parse(0))
            if self.at(","):
                self.take()
                continue
            self.expect(")")
            return args

    def concat(self) -> N:
        self.expect("{")
        first = self.parse(0)
        if self.at("{"):                  # {n{...}}: replication
            inner = self.concat()
            self.expect("}")
            return N("repl", [first, inner])
        items = [first]
        while self.at(","):
            self.take()
            items.append(self.parse(0))
        self.expect("}")
        return N("concat", items)

    def postfix(self, node: N) -> N:
        while True:
            if self.at("["):
                self.take()
                first = self.parse(13)
                if self.at(":"):
                    self.take()
                    node = N("sel", [node, first, self.parse(13)])
                elif self.at("+:", "-:"):
                    op = self.take()[1]
                    node = N("ipsel", [node, first, self.parse(13)], op)
                else:
                    node = N("idx", [node, first])
                self.expect("]")
            elif self.at(".") and node.k == "id" and self.peek(1)[0] == "id":
                self.take()
                part = self.take()[1]
                joined = f"{node.v}__{part}"
                self.names[joined] = f"{self.names.get(node.v, node.v)}.{part}"
                node = N("id", v=joined)
            elif self.at("."):
                raise SVAError("a hierarchical name after an expression")
            else:
                return node

    def repetition(self, node: N) -> N:
        """[*n], [*m:n], [*m:$], [*], [+], [=n], [->n] after a sequence."""
        op = self.take()[1]
        if op == "[+]":
            return N("rep", [node], ("[*", N("num", v=_Num(1, None, True)), None))
        if op == "[*" and self.at("]"):
            self.take()
            return N("rep", [node], ("[*", N("num", v=_Num(0, None, True)), None))
        lo = self.parse(13)
        hi = lo
        if self.at(":"):
            self.take()
            if self.at("$"):
                self.take()
                hi = None
            else:
                hi = self.parse(13)
        self.expect("]")
        return N("rep", [node], (op, lo, hi))

    def property_prefix(self, kw: str) -> N:
        if kw in ("always", "s_always", "eventually", "s_eventually"):
            r = self.range_after_keyword()
            strong = kw.startswith("s_")
            return N("always" if kw.endswith("always") else "eventually",
                     [self.parse(0)], (r, strong))
        if kw in ("nexttime", "s_nexttime"):
            r = self.range_after_keyword()
            return N("nexttime", [self.parse(7)], (r, kw == "s_nexttime"))
        if kw == "not":
            return N("not", [self.parse(7)])
        if kw == "if":
            self.expect("(")
            c = self.parse(0)
            self.expect(")")
            then = self.parse(0)
            other = None
            if self.at("else"):
                self.take()
                other = self.parse(0)
            return N("if", [c, then] + ([other] if other else []))
        if kw in ("strong", "weak", "first_match"):
            self.expect("(")
            s = self.parse(0)
            self.expect(")")
            return N(kw, [s])
        raise SVAError(f"{kw} is not read")

    def clocking_event(self) -> str:
        """@(posedge clk) or @clk: the event as text."""
        self.expect("@")
        if not self.at("("):
            return self.take()[1]
        start = self.i
        depth = 0
        while True:
            tok = self.take()
            if tok[0] == "end":
                raise SVAError("unbalanced parentheses in a clocking event")
            if tok[1] == "(":
                depth += 1
            elif tok[1] == ")":
                depth -= 1
                if depth == 0:
                    return " ".join(t[1] for t in self.t[start + 1:self.i - 1])

    def spec(self):
        """property_spec: [clocking event] [disable iff (e)] property."""
        clock = self.clocking_event() if self.at("@") else None
        disable = None
        if self.at("disable"):
            self.take()
            self.expect("iff")
            self.expect("(")
            disable = self.parse(0)
            self.expect(")")
        return clock, disable, self.parse(0)


# Reading: statements -------------------------------------------------------------

@dataclass
class Skipped:
    """A statement that was not read: its label (or None), text, and reason."""
    label: Optional[str]
    text: str
    reason: str


@dataclass
class Property:
    """An assertion read: its label, kind (assume or assert), SVA text, the
    LTL formula, and the cycle from which it is checked (0 unless it looks
    before the first cycle)."""
    label: Optional[str]
    kind: str
    text: str
    formula: ltl.Node
    from_cycle: int = 0


@dataclass
class SVAReading:
    """The result of ``parse_sva``: the contract, the properties read, the
    statements skipped, and the cover statements (not part of a contract)."""
    contract: Contract
    properties: list = field(default_factory=list)
    skipped: list = field(default_factory=list)
    covers: list = field(default_factory=list)
    clock: Optional[str] = None
    undeclared: list = field(default_factory=list)
    names: dict = field(default_factory=dict)       # a__b -> a.b (hierarchical names)


@dataclass
class _Decl:
    msb: int
    lsb: int
    signed: bool
    direction: Optional[str] = None     # "input" or "output" of the contract
    dims: tuple = ()                    # packed dimensions [msb:lsb], the first outermost

    @property
    def width(self):
        if self.dims:
            w = 1
            for m, l in self.dims:
                w *= abs(m - l) + 1
            return w
        return abs(self.msb - self.lsb) + 1


_TYPES = {"logic", "wire", "reg", "bit", "var", "tri", "uwire", "wand", "wor"}
_ATOMS = {"int": (32, True), "integer": (32, True), "byte": (8, True),
          "shortint": (16, True), "longint": (64, True)}


class _Scanner:
    """Finds the statements of a text: declarations, parameters, named
    properties and sequences, definitions, and assertions."""

    def __init__(self, text: str):
        self.text = text
        self.p = _Parser(_tokens(text), text)
        self.decls: dict[str, _Decl] = {}
        self.params: dict[str, N] = {}
        self.defs: dict[str, tuple[N, Optional[_Decl]]] = {}
        self.named: dict[str, tuple[list, tuple]] = {}     # name -> (formals, spec)
        self.asserts: list = []   # (label, kind, initial, spec or exception, text)
        self.default_clock = None
        self.default_disable = None
        self.module = None
        self.notes: list[Skipped] = []

    def run(self):
        p = self.p
        attr = None
        while p.peek()[0] != "end":
            kind, val = p.peek()
            start = p.i
            try:
                if kind == "attr":
                    attr = p.take()[1]
                    continue
                if kind == "id" and val == "module":
                    self.module_header()
                elif kind == "id" and val in ("input", "output", "inout") \
                        or kind == "id" and (val in _TYPES or val in _ATOMS) \
                        and p.peek(1)[0] in ("id", "op"):
                    self.declaration(attr)
                elif kind == "id" and val in ("parameter", "localparam"):
                    self.parameter()
                elif kind == "id" and val in ("property", "sequence"):
                    self.named_property()
                elif kind == "id" and val == "assign":
                    p.take()
                    self.definitions(None)
                elif kind == "id" and val == "default":
                    self.default()
                elif kind == "id" and val in ("assert", "assume", "cover", "restrict") \
                        or kind == "id" and p.peek(1)[1] == ":" and \
                        p.peek(2)[1] in ("assert", "assume", "cover", "restrict"):
                    self.assertion(False)
                elif kind == "id" and val == "initial" and \
                        (p.peek(1)[1] in ("assert", "assume", "cover", "restrict")
                         or p.peek(2)[1] == ":"):
                    p.take()
                    self.assertion(True)
                else:
                    self.skip()
            except SVAError as e:
                self.notes.append(Skipped(None, self.source(start, p.i), str(e)))
                self.recover()
            attr = None

    def source(self, start, end):
        return " ".join(t[1] for t in self.p.t[start:end] if t[0] != "end").strip()

    def recover(self):
        p = self.p
        while p.peek()[0] != "end" and not p.at(";", "endproperty", "endsequence"):
            p.take()
        if p.peek()[0] != "end":
            tok = p.take()[1]
            if tok in ("endproperty", "endsequence") and p.at(":"):
                p.take()
                p.take()

    def skip(self):
        """A statement that is not read: up to its ';', not into an
        assertion that follows."""
        p = self.p
        depth = 0
        first = True
        while p.peek()[0] != "end":
            kind, val = p.peek()
            if not first and depth == 0 and kind == "id" and val in (
                    "assert", "assume", "cover", "restrict", "property", "sequence",
                    "module", "endmodule", "default", "assign", "input", "output",
                    "parameter", "localparam", "initial"):
                return
            first = False
            p.take()
            if val in ("(", "[", "{"):
                depth += 1
            elif val in (")", "]", "}"):
                depth = max(depth - 1, 0)
            elif val == ";" and depth == 0:
                return
            elif val in ("begin", "end", "endmodule", "endgenerate", "generate"):
                return

    def module_header(self):
        p = self.p
        p.expect("module")
        self.module = p.take()[1]
        while p.at("import"):
            while not p.at(";") and p.peek()[0] != "end":
                p.take()
            p.take()
        if p.at("#"):
            p.take()
            p.expect("(")
            while not p.at(")"):
                if p.at("parameter", "localparam"):
                    p.take()
                self.one_parameter()
                if p.at(","):
                    p.take()
            p.expect(")")
        if p.at("("):
            p.take()
            direction, decl, attr = None, None, None
            while not p.at(")"):
                if p.peek()[0] == "attr":
                    attr = p.take()[1]
                    continue
                if p.at("input", "output", "inout"):
                    direction = p.take()[1]
                    decl = self.data_type()
                elif p.peek()[0] == "id" and (p.peek()[1] in _TYPES or p.peek()[1] in _ATOMS):
                    decl = self.data_type()
                name = p.take()[1]
                if decl is not None:
                    self.declare(name, decl, direction, attr)
                attr = None
                if p.at(","):
                    p.take()
            p.expect(")")
        p.expect(";")

    def data_type(self) -> _Decl:
        """[logic|wire|...] [signed|unsigned] [[msb:lsb]]...: the vector."""
        p = self.p
        signed, width = False, None
        while p.peek()[0] == "id" and (p.peek()[1] in _TYPES or p.peek()[1] in _ATOMS):
            t = p.take()[1]
            if t in _ATOMS:
                width, signed = _ATOMS[t]
        if p.at("signed", "unsigned"):
            signed = p.take()[1] == "signed"
        ranges = []
        while p.at("["):
            p.take()
            msb = p.parse(13)
            p.expect(":")
            lsb = p.parse(13)
            p.expect("]")
            ranges.append((msb, lsb))
        if len(ranges) > 1:
            dims = tuple((self.constant(m), self.constant(l)) for m, l in ranges)
            d = _Decl(0, 0, signed, dims=dims)
            return _Decl(d.width - 1, 0, signed, dims=dims)
        if ranges:
            msb, lsb = (self.constant(x) for x in ranges[0])
            return _Decl(msb, lsb, signed)
        if width:
            return _Decl(width - 1, 0, signed)
        return _Decl(0, 0, signed)

    def declare(self, name, decl, direction, attr):
        if attr and "chase" in attr:
            m = re.search(r'chase\s*=\s*"(input|output)"', attr)
            if m:
                direction = m.group(1)
        if direction == "inout":
            direction = None
        decl.direction = direction
        self.decls[name] = decl

    def declaration(self, attr):
        p = self.p
        direction = p.take()[1] if p.at("input", "output", "inout") else None
        decl = self.data_type()
        while True:
            name = p.take()[1]
            if p.at("["):
                raise SVAError(f"{name}: unpacked arrays are not read")
            self.declare(name, _Decl(decl.msb, decl.lsb, decl.signed, dims=decl.dims),
                         direction, attr)
            if p.at("="):
                p.take()
                self.defs[name] = (p.parse(0), self.decls[name])
            if p.at(","):
                p.take()
                continue
            p.expect(";")
            return

    def definitions(self, decl):
        p = self.p
        while True:
            name = p.take()[1]
            if p.at("["):
                raise SVAError(f"assign to bits of {name} is not read")
            p.expect("=")
            self.defs[name] = (p.parse(0), self.decls.get(name))
            if p.at(","):
                p.take()
                continue
            p.expect(";")
            return

    def one_parameter(self):
        p = self.p
        while p.peek()[0] == "id" and p.peek(1)[1] not in ("=", ",", ")", ";"):
            p.take()                 # type, signing
        while p.at("["):
            while not p.at("]"):
                p.take()
            p.take()
        name = p.take()[1]
        if p.at("="):
            p.take()
            self.params.setdefault(name, p.parse(13))

    def parameter(self):
        p = self.p
        p.take()
        while True:
            self.one_parameter()
            if p.at(","):
                p.take()
                continue
            p.expect(";")
            return

    def named_property(self):
        p = self.p
        kw = p.take()[1]
        name = p.take()[1]
        formals = []
        if p.at("("):
            p.take()
            while not p.at(")"):
                while p.peek()[0] == "id" and p.peek(1)[0] == "id":
                    p.take()                 # untyped, sequence, property, logic...
                formals.append(p.take()[1])
                if p.at("="):
                    raise SVAError(f"{name}: default arguments are not read")
                if p.at(","):
                    p.take()
            p.expect(")")
        p.expect(";")
        start = p.i
        end = "endproperty" if kw == "property" else "endsequence"
        try:
            if p.peek()[0] == "id" and (p.peek()[1] in _TYPES or p.peek()[1] in _ATOMS):
                raise SVAError("local variables of a property or sequence are not read")
            spec = p.spec()
            if p.at(";"):
                p.take()
            p.expect(end)
        except SVAError as e:
            while not p.at(end) and p.peek()[0] != "end":
                p.take()
            p.take()
            spec = e
        if p.at(":"):
            p.take()
            p.take()
        self.named[name] = (formals, spec, self.source(start, p.i))

    def default(self):
        p = self.p
        p.take()
        if p.at("clocking"):
            p.take()
            if p.peek()[0] == "id" and not p.at("@"):
                p.take()
            self.default_clock = p.clocking_event()
            p.expect(";")
            while not p.at("endclocking") and p.peek()[0] != "end":
                p.take()
            p.take()
            if p.at(":"):
                p.take()
                p.take()
        elif p.at("disable"):
            p.take()
            p.expect("iff")
            p.expect("(")
            self.default_disable = p.parse(0)
            p.expect(")")
            p.expect(";")
        else:
            self.skip()

    def assertion(self, initial):
        p = self.p
        start = p.i
        label = None
        if p.peek(1)[1] == ":":
            label = p.take()[1]
            p.take()
        kind = p.take()[1]
        if not p.at("property"):
            self.skip()
            self.notes.append(Skipped(label, self.source(start, p.i),
                                      "an immediate assertion (not a property)"))
            return
        p.take()
        p.expect("(")
        try:
            spec = p.spec()
            p.expect(")")
        except SVAError as e:
            spec = e
            depth = 1
            while depth and p.peek()[0] != "end":
                v = p.take()[1]
                depth += v == "("
                depth -= v == ")"
        text = self.source(start, p.i)
        self.action()
        self.asserts.append((label, kind, initial, spec, text))

    def action(self):
        """The action block after an assertion: ; or [stmt] [else stmt]."""
        p = self.p
        if p.at(";"):
            p.take()
            return
        depth = 0
        while p.peek()[0] != "end":
            v = p.take()[1]
            if v in ("begin", "(", "{"):
                depth += 1
            elif v in ("end", ")", "}"):
                depth -= 1
                if depth == 0 and v == "end" and not p.at("else"):
                    return
            elif v == ";" and depth == 0 and not p.at("else"):
                return

    def constant(self, node: N) -> int:
        return _constant(node, self.params)


def _constant(node: N, params: dict, seen=()) -> int:
    """The value of a constant expression (numbers and parameters)."""
    if node is None:
        raise SVAError("a missing constant")
    if node.k == "num":
        if node.v.unknown:
            raise SVAError("x or z digits")
        n = node.v
        if n.signed and n.width and n.value >= 1 << (n.width - 1):
            return n.value - (1 << n.width)
        return n.value
    if node.k == "id":
        if node.v not in params:
            raise SVAError(f"{node.v} is not a constant (give its value in parameters)")
        if node.v in seen:
            raise SVAError(f"the parameter {node.v} is defined by itself")
        value = params[node.v]
        return value if isinstance(value, int) else _constant(value, params, seen + (node.v,))
    if node.k == "paren":
        return _constant(node.a[0], params, seen)
    if node.k == "un":
        x = _constant(node.a[0], params, seen)
        if node.v not in ("-", "+", "!", "~"):
            _bad_constant(node)
        return {"-": -x, "+": x, "!": int(not x), "~": ~x}[node.v]
    if node.k == "bin":
        a, b = _constant(node.a[0], params, seen), _constant(node.a[1], params, seen)
        ops = {"+": lambda: a + b, "-": lambda: a - b, "*": lambda: a * b,
               "/": lambda: int(a / b), "%": lambda: a - int(a / b) * b,
               "**": lambda: a ** b, "<<": lambda: a << b, ">>": lambda: a >> b,
               "==": lambda: int(a == b), "!=": lambda: int(a != b),
               "<": lambda: int(a < b), "<=": lambda: int(a <= b),
               ">": lambda: int(a > b), ">=": lambda: int(a >= b),
               "&&": lambda: int(bool(a and b)), "||": lambda: int(bool(a or b)),
               "&": lambda: a & b, "|": lambda: a | b, "^": lambda: a ^ b}
        if node.v not in ops:
            _bad_constant(node)
        return ops[node.v]()
    if node.k == "cond":
        return _constant(node.a[1] if _constant(node.a[0], params, seen) else node.a[2],
                         params, seen)
    if node.k in ("idx", "sel"):
        v = _constant(node.a[0], params, seen)
        hi = _constant(node.a[1], params, seen)
        lo = hi if node.k == "idx" else _constant(node.a[2], params, seen)
        hi, lo = max(hi, lo), min(hi, lo)
        return (v >> lo) & ((1 << (hi - lo + 1)) - 1)
    if node.k == "concat":
        v = 0
        for x in node.a:
            if x.k == "num" and x.v.width:
                w = x.v.width
            elif x.k in ("sel", "idx"):
                hi = _constant(x.a[1], params, seen)
                lo = hi if x.k == "idx" else _constant(x.a[2], params, seen)
                w = abs(hi - lo) + 1
            else:
                w = 32
            v = (v << w) | (_constant(x, params, seen) & ((1 << w) - 1))
        return v
    if node.k == "cast" and not isinstance(node.v, str):
        n = _constant(node.v, params, seen)
        return _constant(node.a[0], params, seen) & ((1 << n) - 1)
    if node.k == "call" and node.v == "$clog2":
        x = _constant(node.a[0], params, seen)
        return max(x - 1, 0).bit_length()
    _bad_constant(node)


def _bad_constant(node):
    raise SVAError(f"not a constant expression: {_show(node)}")


def _show(node: N) -> str:
    """A short text of a node, for the messages."""
    if node is None:
        return ""
    if node.k == "id":
        return node.v
    if node.k == "num":
        return str(node.v.value)
    if node.k == "call":
        return f"{node.v}(...)"
    if node.k == "bin":
        return f"{_show(node.a[0])} {node.v} {_show(node.a[1])}"
    if node.k == "paren":
        return f"({_show(node.a[0])})"
    return node.k


# Reading: meaning --------------------------------------------------------------

class _NeedsWord(Exception):
    """A 1-bit signal used as a number: it becomes a word[1]."""

    def __init__(self, name):
        self.name = name


def _wrap(value: int, width: int, signed: bool) -> int:
    value &= (1 << width) - 1
    if signed and value >= 1 << (width - 1):
        value -= 1 << width
    return value


def _need(lo: int, hi: int, signed: bool) -> int:
    """The width that holds every value of [lo, hi]."""
    if signed:
        k = 1
        while not (-(1 << (k - 1)) <= lo and hi < 1 << (k - 1)):
            k += 1
        return k
    return max(hi.bit_length(), 1)


def _fits(lo, hi, width, signed) -> bool:
    if signed:
        return -(1 << (width - 1)) <= lo and hi < 1 << (width - 1)
    return 0 <= lo and hi < 1 << width


def _x(node, k: int):
    for _ in range(k):
        node = ltl.Unary("X", node)
    return node


def _and(*fs):
    out = [f for f in fs if not (isinstance(f, ltl.Const) and f.value)]
    if any(isinstance(f, ltl.Const) and not f.value for f in out):
        return ltl.Const(False)
    if not out:
        return ltl.Const(True)
    result = out[0]
    for f in out[1:]:
        result = ltl.Binary("&", result, f)
    return result


def _or(*fs):
    out = [f for f in fs if not (isinstance(f, ltl.Const) and not f.value)]
    if any(isinstance(f, ltl.Const) and f.value for f in out):
        return ltl.Const(True)
    if not out:
        return ltl.Const(False)
    result = out[0]
    for f in out[1:]:
        result = ltl.Binary("|", result, f)
    return result


_NEGATED = {"=": "!=", "!=": "=", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}


def _not(f):
    if isinstance(f, ltl.Const):
        return ltl.Const(not f.value)
    if isinstance(f, ltl.Unary) and f.op == "!":
        return f.arg
    if isinstance(f, ltl.Binary) and f.op in _NEGATED:
        return ltl.Binary(_NEGATED[f.op], f.left, f.right)
    return ltl.Unary("!", f)


def _implies(a, b):
    if isinstance(a, ltl.Const):
        return b if a.value else ltl.Const(True)
    if isinstance(b, ltl.Const):
        return ltl.Const(True) if b.value else _not(a)
    return ltl.Binary("->", a, b)


def _iff(a, b):
    if isinstance(b, ltl.Const):
        return a if b.value else _not(a)
    if isinstance(a, ltl.Const):
        return b if a.value else _not(b)
    return ltl.Binary("<->", a, b)


class _Meaning:
    """The LTL of properties, sequences, and expressions."""

    MAX_ALTERNATIVES = 4096
    MAX_STEPS = 256
    MAX_CASES = 16       # Booleans used as numbers, else they become words

    def __init__(self, scan: _Scanner, params: dict, types: dict, vector_bits: set):
        self.scan = scan
        self.params = params
        self.types = types              # name -> _Decl
        self.vector_bits = vector_bits  # 1-bit signals used as numbers
        self.used: set = set()
        self.declared = set(types)

    # constants ---
    def const(self, node) -> int:
        return _constant(node, self.params)

    def try_const(self, node) -> Optional[int]:
        try:
            return self.const(node)
        except SVAError:
            return None

    # names ---
    def decl(self, name) -> _Decl:
        if name in self.params:
            raise SVAError(f"the parameter {name} where a signal is expected")
        d = self.types.get(name)
        if d is None:
            d = _Decl(0, 0, False)
            self.types[name] = d
        return d

    def var(self, name, d):
        self.used.add(name)
        return ltl.Var(f"{name}@{d}")

    # self-determined width and signedness (IEEE 1800 11.6, 11.8) ---
    def self_type(self, e) -> tuple[int, bool]:
        k = e.k
        if k == "paren":
            return self.self_type(e.a[0])
        if k == "num":
            if e.v.fill:
                return (1, False)
            return (e.v.width or 32, e.v.signed)
        if k == "id":
            if e.v in self.params:
                return (32, True)
            if e.v in self.scan.defs and e.v not in self.types:
                return self.self_type(self.scan.defs[e.v][0])
            d = self.decl(e.v)
            return (d.width, d.signed)
        if k == "un":
            if e.v in ("+", "-", "~"):
                return self.self_type(e.a[0])
            return (1, False)
        if k == "bin":
            op = e.v
            if op in ("&&", "||", "->", "<->") or op in ("==", "!=", "===", "!==", "<", "<=",
                                                         ">", ">=", "==?", "!=?"):
                return (1, False)
            if op in ("<<", ">>", "<<<", ">>>", "**"):
                return self.self_type(e.a[0])
            (wa, sa), (wb, sb) = self.self_type(e.a[0]), self.self_type(e.a[1])
            return (max(wa, wb), sa and sb)
        if k == "cond":
            (wa, sa), (wb, sb) = self.self_type(e.a[1]), self.self_type(e.a[2])
            return (max(wa, wb), sa and sb)
        if k == "concat":
            return (sum(self.self_type(x)[0] for x in e.a), False)
        if k == "repl":
            return (self.const(e.a[0]) * self.self_type(e.a[1])[0], False)
        if k in ("idx", "sel", "ipsel") and self.packed(e) is not None:
            return (self.packed(e)[2], False)
        if k == "idx":
            base = self.self_type(e.a[0])
            return (1, False) if base[0] > 1 or e.a[0].k == "id" else base
        if k == "sel":
            return (abs(self.const(e.a[1]) - self.const(e.a[2])) + 1, False)
        if k == "ipsel":
            return (self.const(e.a[2]), False)
        if k == "cast":
            inner = self.self_type(e.a[0])
            if e.v in ("signed", "unsigned"):
                return (inner[0], e.v == "signed")
            return (self.const(e.v), inner[1])
        if k == "inside":
            return (1, False)
        if k == "call":
            name = e.v
            if name in ("$past", "$sampled"):
                return self.self_type(e.a[0])
            if name in ("$signed", "$unsigned"):
                return (self.self_type(e.a[0])[0], name == "$signed")
            if name == "$countones":
                return (32, True)
            if name in ("$rose", "$fell", "$stable", "$changed", "$onehot", "$onehot0",
                        "$isunknown"):
                return (1, False)
            raise SVAError(f"the function {name} is not read")
        raise SVAError(f"a {k} where an expression is expected")

    # Boolean meaning of an expression ---
    def formula(self, e, d: int = 0):
        k = e.k
        if k == "paren":
            return self.formula(e.a[0], d)
        if k == "num":
            if e.v.unknown:
                raise SVAError("x or z digits")
            return ltl.Const(e.v.value != 0)
        if k == "id":
            if e.v in self.params:
                return ltl.Const(self.const(e) != 0)
            if e.v in self.scan.defs:
                return self.defined_formula(e.v, d)
            dec = self.decl(e.v)
            if dec.width == 1:
                return self.var(e.v, d)
            return self.nonzero(e, d)
        if k == "un":
            op = e.v
            if op == "!":
                return _not(self.formula(e.a[0], d))
            w, _ = self.self_type(e.a[0])
            if op in ("~", "&", "|", "^", "~&", "~|", "~^", "^~") and w == 1:
                f = self.formula(e.a[0], d)
                return _not(f) if op in ("~", "~&", "~|", "~^", "^~") else f
            if op in ("|", "~|"):
                f = self.relation("!=", e.a[0], _ZERO, d)
                return f if op == "|" else _not(f)
            if op in ("&", "~&"):
                ones = N("num", v=_Num((1 << w) - 1, w, False))
                f = self.relation("==", e.a[0], ones, d)
                return f if op == "&" else _not(f)
            if op in ("^", "~^", "^~"):
                f = self.reduce_xor(e.a[0], d)
                return f if op == "^" else _not(f)
            return self.nonzero(e, d)
        if k == "bin":
            op = e.v
            if op in ("&&", "||", "->", "<->"):
                a, b = self.formula(e.a[0], d), self.formula(e.a[1], d)
                return {"&&": _and, "||": _or, "->": _implies, "<->": _iff}[op](a, b)
            if op in ("==", "!=", "===", "!==", "<", "<=", ">", ">="):
                return self.relation(op, e.a[0], e.a[1], d)
            if op in ("==?", "!=?"):
                raise SVAError(f"the wildcard equality {op}")
            if op in ("&", "|", "^", "~^", "^~") and self.self_type(e)[0] == 1:
                a, b = self.formula(e.a[0], d), self.formula(e.a[1], d)
                if op == "&":
                    return _and(a, b)
                if op == "|":
                    return _or(a, b)
                x = ltl.Binary("xor", a, b)
                return x if op == "^" else _not(x)
            return self.nonzero(e, d)
        if k == "cond":
            c = self.formula(e.a[0], d)
            return _or(_and(c, self.formula(e.a[1], d)), _and(_not(c), self.formula(e.a[2], d)))
        if k == "inside":
            out = []
            for item in e.a[1:]:
                if item.k == "range":
                    out.append(_and(self.relation(">=", e.a[0], item.a[0], d),
                                    self.relation("<=", e.a[0], item.a[1], d)))
                else:
                    out.append(self.relation("==", e.a[0], item, d))
            return _or(*out)
        if k == "call":
            return self.call_formula(e, d)
        if k in ("idx", "sel", "ipsel", "concat", "repl", "cast"):
            return self.nonzero(e, d)
        raise SVAError(f"a {k} where an expression is expected")

    def nonzero(self, e, d):
        """e != 0; for a value of one bit, from its terms (no relation)."""
        if self.self_type(e)[0] != 1:
            return self.relation("!=", e, _ZERO, d)
        out = []
        for g, t in self.term(e, 1, False, d):
            out.append(_and(g, ltl.Const(t.value != 0) if isinstance(t, ltl.Num) else t))
        return _or(*out)

    def decl_or_def_width(self, name):
        if name in self.scan.defs and name not in self.types:
            return self.self_type(self.scan.defs[name][0])[0]
        return self.decl(name).width

    def defined_formula(self, name, d):
        expr, dec = self.scan.defs[name]
        if dec is None or (dec.width == 1 and self.self_type(expr)[0] == 1):
            return self.formula(expr, d)
        return self.relation("!=", N("id", v=name), _ZERO, d)

    def reduce_xor(self, e, d):
        w, s = self.self_type(e)
        out = []
        for g, t in self.term(e, w, s, d):
            if isinstance(t, ltl.Num):
                out.append(_and(g, ltl.Const(bin(t.value & ((1 << w) - 1)).count("1") % 2 == 1)))
                continue
            t = _cast(t, False, s)
            x = None
            for i in range(w):
                bit = _bit(t, i)
                x = bit if x is None else ltl.Binary("xor", x, bit)
            out.append(_and(g, x))
        return _or(*out)

    def call_formula(self, e, d):
        name, args = e.v, e.a
        if name == "$past":
            return self.formula(args[0], d - self.past_steps(e))
        if name == "$sampled":
            return self.formula(args[0], d)
        if name in ("$rose", "$fell"):
            self.no_clock(e)
            now, before = self.lsb(args[0], d), self.lsb(args[0], d - 1)
            return _and(now, _not(before)) if name == "$rose" else _and(_not(now), before)
        if name in ("$stable", "$changed"):
            self.no_clock(e)
            f = self.relation("==", args[0], N("call", [args[0]], "$past"), d)
            return f if name == "$stable" else _not(f)
        if name in ("$onehot", "$onehot0"):
            w, s = self.self_type(args[0])
            if w == 1:
                return self.formula(args[0], d) if name == "$onehot" else ltl.Const(True)
            out = []
            for g, t in self.term(args[0], w, s, d):
                t = _cast(t, False, s)
                low = self.fold(ltl.Binary("&", t, self.fold(ltl.Binary("-", t, ltl.Num(1)),
                                                             w, False)), w, False)
                one = _rel("=", low, ltl.Num(0))
                if name == "$onehot":
                    one = _and(_rel("!=", t, ltl.Num(0)), one)
                out.append(_and(g, one))
            return _or(*out)
        if name == "$isunknown":
            return ltl.Const(False)
        if name in ("$countones", "$signed", "$unsigned"):
            return self.nonzero(e, d)
        raise SVAError(f"the function {name} is not read")

    def no_clock(self, e):
        if len(e.a) > 1:
            raise SVAError(f"{e.v} with a clocking event")

    def past_steps(self, e) -> int:
        args = e.a
        if len(args) > 2 or (len(args) == 2 and args[1] is None):
            raise SVAError("$past with a gating expression or a clock")
        n = self.const(args[1]) if len(args) == 2 else 1
        if n < 1:
            raise SVAError(f"$past of {n} cycles")
        return n

    def lsb(self, e, d):
        w, s = self.self_type(e)
        if w == 1:
            return self.formula(e, d)
        out = []
        for g, t in self.term(e, w, s, d):
            b = _bitword(_cast(t, False, s), 0)
            out.append(_and(g, ltl.Const(b.value == 1) if isinstance(b, ltl.Num) else b))
        return _or(*out)

    # relations, with the widths of Verilog ---
    def relation(self, op, a, b, d):
        (wa, sa), (wb, sb) = self.self_type(a), self.self_type(b)
        W, S = max(wa, wb), sa and sb
        ra, rb = self.rng(a, W, S), self.rng(b, W, S)
        if ra is not None and rb is not None:
            W = min(W, max(ra[2], rb[2], 1))
            if S and W == 1:
                W = min(2, max(wa, wb))
        if W == 1 and not S:
            fa, fb = self.formula_of_bit(a, d), self.formula_of_bit(b, d)
            return {"==": lambda: _iff(fa, fb), "===": lambda: _iff(fa, fb),
                    "!=": lambda: _not(_iff(fa, fb)), "!==": lambda: _not(_iff(fa, fb)),
                    "<": lambda: _and(_not(fa), fb), "<=": lambda: _implies(fa, fb),
                    ">": lambda: _and(fa, _not(fb)), ">=": lambda: _implies(fb, fa)}[op]()
        rel = {"==": "=", "===": "=", "!=": "!=", "!==": "!="}.get(op, op)
        out = []
        tas, tbs = self.term(a, W, S, d), self.term(b, W, S, d)
        if len(tas) * len(tbs) > self.MAX_CASES:
            raise _NeedsWord(self.boolean_in(a, b))
        for ga, ta in tas:
            for gb, tb in tbs:
                if isinstance(ta, ltl.Num) and isinstance(tb, ltl.Num):
                    holds = {"=": ta.value == tb.value, "!=": ta.value != tb.value,
                             "<": ta.value < tb.value, "<=": ta.value <= tb.value,
                             ">": ta.value > tb.value, ">=": ta.value >= tb.value}[rel]
                    out.append(_and(ga, gb, ltl.Const(holds)))
                else:
                    out.append(_and(ga, gb, ltl.Binary(rel, ta, tb)))
        return _or(*out)

    def boolean_in(self, *es):
        """A 1-bit signal of the expressions that is not a word yet."""
        stack = list(es)
        while stack:
            e = stack.pop()
            if not isinstance(e, N):
                continue
            if e.k == "id" and e.v not in self.params and e.v not in self.scan.defs \
                    and self.decl(e.v).width == 1 and e.v not in self.vector_bits:
                return e.v
            stack += list(e.a)
        raise SVAError("an expression with too many cases")

    def formula_of_bit(self, e, d):
        """A value of one bit (0 or 1) as a formula."""
        w, s = self.self_type(e)
        if w == 1:
            return self.formula(e, d)
        c = self.try_const(e)
        if c is not None:
            return ltl.Const(c != 0)
        return self.nonzero(e, d)

    def rng(self, e, W, S):
        """(low, high, width needed) of the exact values of e in a context of
        W bits, when no operation of e overflows W bits; else None."""
        k = e.k
        if k == "paren":
            return self.rng(e.a[0], W, S)
        c = self.try_const(e) if k in ("num", "id", "bin", "un") else None
        if c is not None and not (k == "num" and (e.v.fill or e.v.unknown)):
            w, s = self.self_type(e)
            v = self.const_in(c, w, s, W, S)
            return (v, v, _need(v, v, S))
        if k == "num" and e.v.fill:
            v = _wrap(-1 if e.v.value else 0, W, S) if e.v.value else 0
            if e.v.value and not S:
                v = (1 << W) - 1
            return (v, v, _need(v, v, S))
        if k in ("id", "idx", "sel", "ipsel", "concat", "repl", "cast", "call", "inside"):
            w, s = self.self_type(e)
            if k == "id" and e.v in self.scan.defs and e.v not in self.types:
                return self.rng(self.scan.defs[e.v][0], W, S)
            if s and S:
                lo, hi = -(1 << (w - 1)), (1 << (w - 1)) - 1
            else:
                lo, hi = 0, (1 << w) - 1
            if k == "call" and e.v == "$countones":
                hi = self.self_type(e.a[0])[0]
                lo = 0
            return (lo, hi, _need(lo, hi, S))
        if k == "un":
            if e.v in ("!", "&", "|", "^", "~&", "~|", "~^", "^~"):
                return (0, 1, 1)
            r = self.rng(e.a[0], W, S)
            if r is None:
                return None
            if e.v == "+":
                return r
            if e.v == "-":
                lo, hi = -r[1], -r[0]
            else:   # ~
                lo, hi = (-r[1] - 1, -r[0] - 1) if S else ((1 << W) - 1 - r[1], (1 << W) - 1 - r[0])
            if not _fits(lo, hi, W, S):
                return None
            return (lo, hi, max(r[2], _need(lo, hi, S)))
        if k == "cond":
            a, b = self.rng(e.a[1], W, S), self.rng(e.a[2], W, S)
            if a is None or b is None:
                return None
            lo, hi = min(a[0], b[0]), max(a[1], b[1])
            return (lo, hi, max(a[2], b[2]))
        if k == "bin":
            op = e.v
            if op in ("&&", "||", "->", "<->", "==", "!=", "===", "!==", "<", "<=", ">",
                      ">="):
                return (0, 1, 1)
            if op in ("<<", ">>", "<<<", ">>>"):
                a = self.rng(e.a[0], W, S)
                n = self.try_const(e.a[1])
                if n is None and a is not None and a[0] >= 0:
                    n = (1 << min(self.self_type(e.a[1])[0], 8)) - 1   # the largest amount
                    if op in (">>", ">>>"):
                        return a
                if a is None or n is None or n < 0:
                    return None
                if op in ("<<", "<<<"):
                    lo, hi = a[0] << n, a[1] << n
                elif a[0] >= 0:
                    lo, hi = a[0] >> n, a[1] >> n
                else:
                    return None
                if not _fits(lo, hi, W, S):
                    return None
                return (lo, hi, max(a[2], _need(lo, hi, S)))
            if op == "**":
                return None
            a, b = self.rng(e.a[0], W, S), self.rng(e.a[1], W, S)
            if a is None or b is None:
                return None
            if op in ("+", "-", "*"):
                f = {"+": lambda x, y: x + y, "-": lambda x, y: x - y,
                     "*": lambda x, y: x * y}[op]
                vals = [f(x, y) for x in a[:2] for y in b[:2]]
                lo, hi = min(vals), max(vals)
            elif op in ("/", "%"):
                m = max(abs(a[0]), abs(a[1]))
                lo, hi = (-m, m) if S else (0, a[1])
            elif op in ("&", "|", "^", "~^", "^~"):
                if a[0] < 0 or b[0] < 0:
                    n = max(_need(a[0], a[1], True), _need(b[0], b[1], True))
                    lo, hi = -(1 << (n - 1)), (1 << (n - 1)) - 1
                else:
                    n = max(a[1].bit_length(), b[1].bit_length(), 1)
                    lo, hi = 0, (1 << n) - 1
                if op in ("~^", "^~"):
                    if not S:
                        return None
            else:
                return None
            if not _fits(lo, hi, W, S):
                return None
            return (lo, hi, max(a[2], b[2], _need(lo, hi, S)))
        return None

    def const_in(self, c, w, s, W, S) -> int:
        """A constant of w bits (signed s) in a context of W bits (signed S)."""
        bits_ = c & ((1 << w) - 1)
        if S and s and bits_ >= 1 << (w - 1):
            bits_ -= 1 << w                 # sign extension
        return _wrap(bits_, W, S)

    # a term of W bits: [(guard, term)] ---
    def term(self, e, W, S, d):
        k = e.k
        if k == "paren":
            return self.term(e.a[0], W, S, d)
        if k == "num":
            n = e.v
            if n.unknown:
                raise SVAError("x or z digits")
            if n.fill:
                return [(ltl.Const(True), ltl.Num(_wrap(-1, W, S) if n.value else 0))]
            w, s = self.self_type(e)
            return [(ltl.Const(True), ltl.Num(self.const_in(n.value, w, s, W, S)))]
        if k == "id":
            if e.v in self.params:
                return [(ltl.Const(True), ltl.Num(self.const_in(self.const(e), 32, True, W, S)))]
            if e.v in self.scan.defs:
                return self.defined_term(e.v, W, S, d)
            dec = self.decl(e.v)
            if dec.width == 1 and e.v not in self.vector_bits:
                v = self.var(e.v, d)         # a Boolean as a number: 1 or 0
                return [(v, self.extend(ltl.Num(1), 1, False, W, S)),
                        (_not(v), ltl.Num(0))]
            return [(ltl.Const(True), self.extend(self.var(e.v, d), dec.width, dec.signed, W, S))]
        if k == "un":
            op = e.v
            if op in ("!", "&", "|", "^", "~&", "~|", "~^", "^~"):
                return self.bit_term(self.formula(e, d), W, S)
            inner = self.term(e.a[0], W, S, d)
            if op == "+":
                return inner
            if op == "-":
                return [(g, self.fold(ltl.Unary("neg", t), W, S)) for g, t in inner]
            return [(g, self.fold(_bitnot(t, W), W, S)) for g, t in inner]
        if k == "bin":
            op = e.v
            if op in ("&&", "||", "->", "<->", "==", "!=", "===", "!==", "<", "<=", ">", ">=",
                      "==?", "!=?"):
                return self.bit_term(self.formula(e, d), W, S)
            if op in ("<<", ">>", "<<<", ">>>"):
                left = self.term(e.a[0], W, S, d)
                n = self.try_const(e.a[1])
                if n is not None:
                    if n < 0:
                        raise SVAError("a shift by a negative number")
                    amounts = [(ltl.Const(True), ltl.Num(n))]
                else:
                    wb, sb = self.self_type(e.a[1])
                    amounts = [(g, ltl.Unary("unsigned", t) if sb else t)
                               for g, t in self.term(e.a[1], wb, sb, d)]
                out = []
                for g, t in left:
                    for h, m in amounts:
                        if isinstance(t, ltl.Num) and not isinstance(m, ltl.Num):
                            out += self.shift_cases(op, t, m, _and(g, h), e.a[1], W, S)
                            continue
                        out.append((_and(g, h), self.fold(self.shift(op, t, m, W, S), W, S)))
                return out
            if op == "**":
                raise SVAError("the power operator **")
            ops = {"+": "+", "-": "-", "*": "*", "/": "/", "%": "mod", "&": "&", "|": "|",
                   "^": "xor", "~^": "xor", "^~": "xor"}
            if op not in ops:
                raise SVAError(f"the operator {op}")
            out = []
            for g, a in self.term(e.a[0], W, S, d):
                for h, b in self.term(e.a[1], W, S, d):
                    t = ltl.Binary(ops[op], a, b)
                    if op in ("~^", "^~"):
                        t = _bitnot(t, W)
                    out.append((_and(g, h), self.fold(t, W, S)))
            return out
        if k == "cond":
            c = self.formula(e.a[0], d)
            out = [(_and(c, g), t) for g, t in self.term(e.a[1], W, S, d)]
            out += [(_and(_not(c), g), t) for g, t in self.term(e.a[2], W, S, d)]
            return [(g, t) for g, t in out if not (isinstance(g, ltl.Const) and not g.value)]
        # self-determined operands: computed in their own width, then extended
        w, s = self.self_type(e)
        return [(g, self.extend(t, w, s, W, S)) for g, t in self.own(e, d)]

    def own(self, e, d):
        """[(guard, term)] of a self-determined expression, in its own width."""
        k = e.k
        w, s = self.self_type(e)
        if k == "concat":
            out = [(ltl.Const(True), [])]
            for x in e.a:
                wx, sx = self.self_type(x)
                ts = [(g, (_as_unsigned(t, sx), wx)) for g, t in self.term(x, wx, sx, d)]
                out = [(_and(g, h), parts + [t]) for g, parts in out for h, t in ts]
            return [(g, _concat(parts)) for g, parts in out]
        if k == "repl":
            n = self.const(e.a[0])
            if n < 1:
                raise SVAError("a replication of 0 times")
            wx = self.self_type(e.a[1])[0]
            return [(g, _concat([(t, wx)] * n)) for g, t in self.own(e.a[1], d)]
        if k in ("idx", "sel", "ipsel"):
            return self.select(e, d)
        if k == "cast":
            inner_w, inner_s = self.self_type(e.a[0])
            if e.v in ("signed", "unsigned"):
                ts = self.term(e.a[0], inner_w, inner_s, d)
                return [(g, _cast(t, e.v == "signed", inner_s)) for g, t in ts]
            n = self.const(e.v)
            ctx = max(n, inner_w)
            ts = self.term(e.a[0], ctx, inner_s, d)
            if n < ctx:
                out = []
                for g, t in ts:
                    t = ltl.Unary(f"[{n - 1}:0]", _as_unsigned(t, inner_s)) \
                        if not isinstance(t, ltl.Num) else ltl.Num(_wrap(t.value, n, False))
                    out.append((g, _cast(t, inner_s, False) if inner_s else t))
                return out
            return ts
        if k == "call":
            name = e.v
            if name == "$past":
                return self.term(e.a[0], w, s, d - self.past_steps(e))
            if name == "$sampled":
                return self.term(e.a[0], w, s, d)
            if name in ("$signed", "$unsigned"):
                inner_w, inner_s = self.self_type(e.a[0])
                return [(g, _cast(t, name == "$signed", inner_s))
                        for g, t in self.term(e.a[0], inner_w, inner_s, d)]
            if name == "$countones":
                wx, sx = self.self_type(e.a[0])
                out = []
                for g, t in self.term(e.a[0], wx, sx, d):
                    t = ltl.Unary("unsigned", t) if sx else t
                    total = None
                    for i in range(wx):
                        bit = self.extend(_bitword(t, i), 1, False, 32, False)
                        total = bit if total is None else self.fold(ltl.Binary("+", total, bit),
                                                                    32, False)
                    out.append((g, _cast(total, True, False)))
                return out
            return self.bit_term(self.call_formula(e, d), w, s)
        if k == "inside":
            return self.bit_term(self.formula(e, d), w, s)
        raise SVAError(f"a {k} where an expression is expected")

    def packed(self, e):
        """(name, low bit, width) of a select into a packed array of several
        dimensions (x[i], x[i][j], x[i][3:0], ...), with constant indices;
        None for other selects."""
        chain, node = [], e
        while node.k in ("idx", "sel", "ipsel"):
            chain.insert(0, node)
            node = node.a[0]
        if node.k != "id" or node.v in self.scan.defs:
            return None
        dec = self.types.get(node.v)
        if dec is None or len(dec.dims) < 2:
            if len(chain) > 1:
                raise SVAError(f"{node.v}: a select of a select")
            return None
        dims, low = list(dec.dims), 0
        for sel in chain:
            if not dims:
                raise SVAError(f"{node.v}: more selects than dimensions")
            (m, l), rest = dims[0], dims[1:]
            element = 1
            for mm, ll in rest:
                element *= abs(mm - ll) + 1

            def phys(i):
                if not min(m, l) <= i <= max(m, l):
                    raise SVAError(f"index {i} of {node.v}, declared [{m}:{l}]")
                return i - l if m >= l else l - i
            if sel.k == "idx":
                i = self.try_const(sel.a[1])
                if i is None:
                    raise SVAError(f"a variable index into {node.v}, a packed array of "
                                   "several dimensions")
                low += phys(i) * element
                dims = rest
            else:
                if sel.k == "sel":
                    a, b = self.const(sel.a[1]), self.const(sel.a[2])
                else:
                    start, w = self.const(sel.a[1]), self.const(sel.a[2])
                    a, b = (start + w - 1, start) if sel.v == "+:" else (start, start - w + 1)
                pa, pb = phys(a), phys(b)
                low += min(pa, pb) * element
                dims = [(abs(pa - pb), 0)] + rest
        width = 1
        for m, l in dims:
            width *= abs(m - l) + 1
        return node.v, low, width

    def select(self, e, d):
        shape = self.packed(e)
        if shape is not None:
            name, low, width = shape
            dec = self.types[name]
            ts = self.term(N("id", v=name), dec.width, dec.signed, d)
            return [(g, ltl.Unary(f"[{low + width - 1}:{low}]", _as_unsigned(t, dec.signed)))
                    for g, t in ts]
        base = e.a[0]
        if base.k != "id":
            raise SVAError("a bit select of an expression")
        name = base.v
        if name in self.scan.defs:
            expr, dec = self.scan.defs[name]
            if dec is None:
                raise SVAError(f"bits of {name}, whose width is not declared")
        else:
            dec = self.decl(name)
        if name not in self.declared and name not in self.scan.defs:
            raise SVAError(f"bits of {name}, whose width is not declared (declare it, or give "
                           "its type in inputs or outputs)")
        if dec.width == 1 and e.k == "idx" and self.try_const(e.a[1]) in (0, dec.lsb):
            return self.term(base, 1, False, d)
        ts = [(g, ltl.Unary("unsigned", t) if dec.signed else t)
              for g, t in self.term(base, dec.width, dec.signed, d)]

        def phys(i):
            if dec.msb >= dec.lsb:
                if not dec.lsb <= i <= dec.msb:
                    raise SVAError(f"bit {i} of {name}, declared [{dec.msb}:{dec.lsb}]")
                return i - dec.lsb
            if not dec.msb <= i <= dec.lsb:
                raise SVAError(f"bit {i} of {name}, declared [{dec.msb}:{dec.lsb}]")
            return dec.lsb - i

        if e.k == "idx":
            i = self.try_const(e.a[1])
            if i is not None:
                p = phys(i)
                return [(g, _bitword(t, p)) for g, t in ts]
            if dec.msb < dec.lsb or dec.lsb != 0:
                raise SVAError(f"a variable index into {name}, declared [{dec.msb}:{dec.lsb}]")
            wi, si = self.self_type(e.a[1])
            out = []
            for g, t in ts:
                for h, i in self.term(e.a[1], wi, si, d):
                    i = ltl.Unary("unsigned", i) if si else i
                    out.append((_and(g, h), _bitword(self.fold(ltl.Binary(">>", t, i),
                                                              dec.width, False), 0)))
            return out
        if e.k == "sel":
            hi, lo = phys(self.const(e.a[1])), phys(self.const(e.a[2]))
            hi, lo = max(hi, lo), min(hi, lo)
            return [(g, ltl.Unary(f"[{hi}:{lo}]", t)) for g, t in ts]
        width = self.const(e.a[2])
        start = self.try_const(e.a[1])
        if start is None:
            raise SVAError(f"an indexed part select of {name} at a variable position")
        if e.v == "+:":
            first, last = start, start + width - 1
        else:
            first, last = start - width + 1, start
        hi, lo = phys(last), phys(first)
        hi, lo = max(hi, lo), min(hi, lo)
        return [(g, ltl.Unary(f"[{hi}:{lo}]", t)) for g, t in ts]

    def defined_term(self, name, W, S, d):
        expr, dec = self.scan.defs[name]
        if dec is None:          # assign without a declaration: the expression
            return self.term(expr, W, S, d)
        w, s = dec.width, dec.signed
        we, se = self.self_type(expr)
        ctx = max(w, we)
        ts = self.term(expr, ctx, se, d)
        out = []
        for g, t in ts:
            if ctx > w:
                t = ltl.Num(_wrap(t.value, w, False)) if isinstance(t, ltl.Num) else \
                    ltl.Unary(f"[{w - 1}:0]", _as_unsigned(t, se))
                se_now = False
            else:
                se_now = se
            if isinstance(t, ltl.Num):
                t = ltl.Num(self.const_in(t.value, w, s, w, s))
            elif s != se_now:
                t = _cast(t, s, se_now)
            out.append((g, self.extend(t, w, s, W, S)))
        return out

    def extend(self, t, w, s, W, S):
        """A term of w bits (signed s) in a context of W bits (signed S)."""
        if isinstance(t, ltl.Num):
            return ltl.Num(self.const_in(t.value, w, s, W, S))
        if s and not S:
            t = ltl.Unary("unsigned", t)
        elif S and not s:
            t = ltl.Unary("signed", t)        # only for a 1-bit value of a formula
        if W > w:
            t = ltl.Unary(f"extend {W - w}", t)
        return t

    def bit_term(self, f, W, S):
        """A formula as a number (0 or 1) of W bits."""
        if isinstance(f, ltl.Const):
            return [(ltl.Const(True), ltl.Num(int(f.value)))]
        return [(f, ltl.Num(1)), (_not(f), ltl.Num(0))]

    def shift_cases(self, op, t, m, guard, amount, W, S):
        """A constant shifted by a word m: one case per value of m."""
        wm = self.self_type(amount)[0]
        if wm > 6:
            raise SVAError("a constant shifted by a signal of more than 6 bits")
        out = []
        for k in range(1 << wm):
            v = self.fold(self.shift(op, t, ltl.Num(k), W, S), W, S)
            out.append((_and(guard, ltl.Binary("=", m, ltl.Num(k))), v))
        return out

    def shift(self, op, t, m, W, S):
        if op in ("<<", "<<<"):
            return ltl.Binary("<<", t, m)
        if S and op == ">>":       # logical shift of a signed value
            return ltl.Unary("signed", ltl.Binary(">>", ltl.Unary("unsigned", t), m))
        return ltl.Binary(">>", t, m)

    def fold(self, t, W, S):
        """Operations on numbers only: their value (the arithmetic of W bits)."""
        if isinstance(t, ltl.Unary) and isinstance(t.arg, ltl.Num):
            if t.op == "neg":
                return ltl.Num(_wrap(-t.arg.value, W, S))
            if t.op == "!":
                return ltl.Num(_wrap(~t.arg.value, W, S))
        if isinstance(t, ltl.Binary) and isinstance(t.left, ltl.Num) \
                and isinstance(t.right, ltl.Num):
            a, b = t.left.value, t.right.value
            if t.op in ("/", "mod") and b == 0:
                raise SVAError("a division by 0")
            q = abs(a) // abs(b) * (1 if (a >= 0) == (b >= 0) else -1) if t.op in ("/", "mod") \
                else 0
            v = {"+": lambda: a + b, "-": lambda: a - b, "*": lambda: a * b, "/": lambda: q,
                 "mod": lambda: a - q * b, "&": lambda: a & b, "|": lambda: a | b,
                 "xor": lambda: a ^ b, "<<": lambda: a << b,
                 ">>": lambda: (a & ((1 << W) - 1)) >> b if not S else a >> b}[t.op]()
            return ltl.Num(_wrap(v, W, S))
        return t

    # sequences: [(length, condition)] with the condition at the start ---
    def sequence(self, s, d=0):
        k = s.k
        if k == "alts":
            return s.v
        if k == "paren":
            return self.sequence(s.a[0], d)
        if k == "delay":
            left, right = s.a
            lo, hi = s.v
            lo = self.const(lo)
            if hi is None:
                raise _Unbounded()
            hi = self.const(hi)
            if lo < 0 or hi < lo:
                raise SVAError(f"the delay ##[{lo}:{hi}]")
            first = [(1, ltl.Const(True))] if left is None else self.sequence(left, d)
            second = self.sequence(right, d)
            out = []
            for n in range(lo, hi + 1):
                for l1, c1 in first:
                    for l2, c2 in second:
                        if n == 0:
                            if l1 == 0 or l2 == 0:
                                continue
                            start = l1 - 1
                        else:
                            start = l1 + n - 1
                        if start < 0:
                            continue
                        out.append((start + l2, _and(c1, _x(c2, start))))
            return self.limit(out)
        if k == "rep":
            op, lo, hi = s.v
            if op != "[*":
                raise SVAError(f"the repetition {op}...]")
            lo = self.const(lo)
            if hi is None:
                raise _Unbounded()
            hi = self.const(hi)
            inner = self.sequence(s.a[0], d)
            out = []
            for n in range(lo, hi + 1):
                acc = [(0, ltl.Const(True))]
                for _ in range(n):
                    acc = [(l1 + l2, _and(c1, _x(c2, l1))) for l1, c1 in acc for l2, c2 in inner]
                    acc = self.limit(acc)
                out += acc
            return self.limit(out)
        if k == "bin" and s.v in ("or", "and", "intersect", "within", "throughout"):
            if s.v == "throughout":
                b = self.formula(s.a[0], d)
                return [(n, _and(c, *[_x(b, i) for i in range(n)]))
                        for n, c in self.sequence(s.a[1], d)]
            a, b = self.sequence(s.a[0], d), self.sequence(s.a[1], d)
            if s.v == "or":
                return self.limit(a + b)
            if s.v == "and":
                return self.limit([(max(l1, l2), _and(c1, c2)) for l1, c1 in a for l2, c2 in b])
            if s.v == "intersect":
                return self.limit([(l1, _and(c1, c2)) for l1, c1 in a for l2, c2 in b
                                   if l1 == l2])
            out = []        # a within b
            for l2, c2 in b:
                for l1, c1 in a:
                    for j in range(0, l2 - l1 + 1):
                        out.append((l2, _and(c2, _x(c1, j))))
            return self.limit(out)
        if k == "first_match":
            alts = sorted(self.sequence(s.a[0], d), key=lambda a: a[0])
            out = []
            for i, (n, c) in enumerate(alts):
                earlier = [c2 for n2, c2 in alts if n2 < n]
                out.append((n, _and(c, _not(_or(*earlier))) if earlier else c))
            return out
        if k in ("not", "always", "eventually", "nexttime", "if", "strong", "weak") or \
                (k == "bin" and s.v in ("|->", "|=>", "#-#", "#=#", "implies", "iff", "until",
                                        "s_until", "until_with", "s_until_with")):
            raise SVAError("a property where a sequence is expected")
        if k == "call" and s.v in self.scan.named:
            return self.sequence(self.instance(s), d)
        if k == "id" and s.v in self.scan.named:
            return self.sequence(self.instance(s), d)
        return [(1, self.formula(s, d))]

    def limit(self, alts):
        seen, out = set(), []
        for n, c in alts:
            if n > self.MAX_STEPS:
                raise SVAError(f"a sequence of more than {self.MAX_STEPS} cycles")
            key = (n, c)
            if key not in seen:
                seen.add(key)
                out.append((n, c))
        if len(out) > self.MAX_ALTERNATIVES:
            raise SVAError(f"a sequence with more than {self.MAX_ALTERNATIVES} ways to match")
        return out

    def chain(self, s):
        """A sequence with ##[m:$] gaps: [alternatives, gap, alternatives, ...]."""
        try:
            return [self.sequence(s)]
        except _Unbounded:
            pass
        k = s.k
        if k == "paren":
            return self.chain(s.a[0])
        if k == "delay":
            left, right = s.a
            lo, hi = s.v
            first = [[(1, ltl.Const(True))]] if left is None else self.chain(left)
            second = self.chain(right)
            if hi is None:
                return first + [self.const(lo)] + second
            # a bounded delay between two parts: joined as one part
            joined = self.sequence(N("delay", [_Alts(first[-1]), _Alts(second[0])], (lo, hi)))
            return first[:-1] + [joined] + second[1:]
        raise SVAError("an unbounded sequence other than ##[m:$] (such as [*m:$], [+], "
                       "or ##[m:$] inside and, or, repetition)")

    # properties ---
    def prop(self, p):
        k = p.k
        if k == "paren":
            return self.prop(p.a[0])
        if k == "not":
            return _not(self.prop(p.a[0]))
        if k == "bin":
            op = p.v
            if op in ("and", "or") and not self.is_property(p):
                return self.seq_property(p, strong=False)
            if op == "and":
                return _and(self.prop(p.a[0]), self.prop(p.a[1]))
            if op == "or":
                return _or(self.prop(p.a[0]), self.prop(p.a[1]))
            if op == "implies":
                return _implies(self.prop(p.a[0]), self.prop(p.a[1]))
            if op == "iff":
                return _iff(self.prop(p.a[0]), self.prop(p.a[1]))
            if op in ("|->", "|=>"):
                return self.implication(p.a[0], self.prop(p.a[1]), op == "|=>")
            if op in ("#-#", "#=#"):
                return self.followed(p.a[0], self.prop(p.a[1]), op == "#=#")
            if op in ("until", "s_until", "until_with", "s_until_with"):
                a, b = self.prop(p.a[0]), self.prop(p.a[1])
                if op.endswith("_with"):
                    b = _and(a, b)
                strong = ltl.Binary("U", a, b)
                return strong if op.startswith("s_") else _or(strong, ltl.Unary("G", a))
        if k == "if":
            c = self.formula(p.a[0])
            then = _implies(c, self.prop(p.a[1]))
            if len(p.a) == 2:
                return then
            return _and(then, _implies(_not(c), self.prop(p.a[2])))
        if k == "nexttime":
            r, _ = p.v
            n = 1 if r is None else self.const(r[0])
            if r is not None and r[1] is not r[0]:
                raise SVAError("nexttime with a range")
            return _x(self.prop(p.a[0]), n)
        if k == "always":
            r, _ = p.v
            inner = self.prop(p.a[0])
            if r is None:
                return ltl.Unary("G", inner)
            lo = self.const(r[0])
            if r[1] is None:
                return _x(ltl.Unary("G", inner), lo)
            hi = self.const(r[1])
            self.steps(hi)
            return _and(*[_x(inner, i) for i in range(lo, hi + 1)])
        if k == "eventually":
            r, strong = p.v
            inner = self.prop(p.a[0])
            if r is None:
                return ltl.Unary("F", inner)
            lo = self.const(r[0])
            if r[1] is None:
                if not strong:
                    raise SVAError("eventually [m:$] (weak and unbounded): write s_eventually")
                return _x(ltl.Unary("F", inner), lo)
            hi = self.const(r[1])
            self.steps(hi)
            return _or(*[_x(inner, i) for i in range(lo, hi + 1)])
        if k in ("strong", "weak"):
            return self.seq_property(p.a[0], strong=k == "strong")
        if k in ("call", "id") and p.v in self.scan.named:
            return self.prop(self.instance(p))
        return self.seq_property(p, strong=False)

    def steps(self, n):
        if n > self.MAX_STEPS:
            raise SVAError(f"a range of more than {self.MAX_STEPS} cycles")

    def is_property(self, p) -> bool:
        k = p.k
        if k == "paren":
            return self.is_property(p.a[0])
        if k in ("not", "if", "nexttime", "always", "eventually", "strong", "weak"):
            return True
        if k == "bin" and p.v in ("|->", "|=>", "#-#", "#=#", "implies", "iff", "until",
                                  "s_until", "until_with", "s_until_with"):
            return True
        if k == "bin" and p.v in ("and", "or"):
            return self.is_property(p.a[0]) or self.is_property(p.a[1])
        if k in ("call", "id") and p.v in self.scan.named:
            return self.is_property(self.instance(p))
        return False

    def seq_property(self, s, strong):
        chain = self.chain(s)
        if len(chain) > 1 and not strong:
            raise SVAError("an unbounded sequence (##[m:$]) as a property is weak, so it "
                           "never fails: write s_eventually, or strong(...)")
        return self.exists(chain, ltl.Const(True), 0)

    def exists(self, chain, p, extra):
        """Some match of the chain, then p at its end (plus extra cycles)."""
        alts = chain[0]
        out = []
        for n, c in alts:
            if n == 0:
                raise SVAError("a sequence that matches the empty word")
            if len(chain) == 1:
                out.append(_and(c, _x(p, n - 1 + extra)))
            else:
                gap = chain[1]
                rest = ltl.Unary("F", self.exists(chain[2:], p, extra))
                out.append(_and(c, _x(rest, n - 1 + gap)))
        return _or(*out)

    def implication(self, s, p, next_cycle):
        chain = self.chain(s)
        return self.every(chain, p, 1 if next_cycle else 0)

    def every(self, chain, p, extra):
        """Every match of the chain is followed by p at its end."""
        out = []
        for n, c in chain[0]:
            if n == 0:
                raise SVAError("an antecedent that matches the empty word")
            if len(chain) == 1:
                out.append(_implies(c, _x(p, n - 1 + extra)))
            else:
                gap = chain[1]
                rest = ltl.Unary("G", self.every(chain[2:], p, extra))
                out.append(_implies(c, _x(rest, n - 1 + gap)))
        return _and(*out)

    def followed(self, s, p, next_cycle):
        return self.exists(self.chain(s), p, 1 if next_cycle else 0)

    def instance(self, node, top=False):
        """A named property or sequence with its arguments put in place; at
        the top of an assertion, (clock, disable, body)."""
        name = node.v
        formals, spec, _ = self.scan.named[name]
        if isinstance(spec, SVAError):
            raise SVAError(f"{name}: {spec}")
        clock, disable, body = spec
        actuals = list(node.a) if node.k == "call" else []
        if len(actuals) != len(formals):
            raise SVAError(f"{name} takes {len(formals)} arguments, {len(actuals)} given")
        env = dict(zip(formals, actuals))
        if top:
            return clock, _substitute(disable, env), _substitute(body, env)
        if disable is not None:
            raise SVAError(f"{name}: disable iff inside a named property used in another")
        return _substitute(body, env)


class _Unbounded(Exception):
    pass


class _Alts(N):
    """Alternatives already computed, as a node (for chains)."""

    def __init__(self, alts):
        super().__init__("alts", (), alts)


_ZERO = N("num", v=_Num(0, 1, False))


def _substitute(node, env):
    if node is None or not isinstance(node, N):
        return node
    if node.k == "id" and node.v in env:
        return N("paren", [env[node.v]])
    if node.k in ("delay",):
        lo, hi = node.v
        return N(node.k, [_substitute(x, env) for x in node.a],
                 (_substitute(lo, env), _substitute(hi, env)))
    v = node.v
    if node.k == "cast" and isinstance(v, N):
        v = _substitute(v, env)
    if node.k == "rep":
        v = (v[0], _substitute(v[1], env), _substitute(v[2], env))
    return N(node.k, [_substitute(x, env) for x in node.a], v)


def _bit(t, i):
    return ltl.Unary(f"[{i}:{i}]", t)


def _rel(op, a, b):
    if isinstance(a, ltl.Num) and isinstance(b, ltl.Num):
        return ltl.Const({"=": a.value == b.value, "!=": a.value != b.value}[op])
    return ltl.Binary(op, a, b)


def _bitword(t, i):
    if isinstance(t, ltl.Num):
        return ltl.Num((t.value >> i) & 1)
    return ltl.Unary(f"[{i}:{i}]", t)


def _bitnot(t, W):
    if W == 1:
        return ltl.Binary("xor", t, ltl.Num(1))
    return ltl.Unary("!", t)


def _concat(parts):
    """The concatenation of [(term, width)], the first on the high bits; the
    constants (numbers without a type) go in with shifts and |."""
    total = sum(w for _, w in parts)
    if all(isinstance(t, ltl.Num) for t, _ in parts):
        v = 0
        for t, w in parts:
            v = (v << w) | (t.value & ((1 << w) - 1))
        return ltl.Num(v)
    # adjacent words joined with ::
    segments = []
    for t, w in parts:
        if segments and not isinstance(t, ltl.Num) and not isinstance(segments[-1][0], ltl.Num):
            segments[-1] = (ltl.Binary("::", segments[-1][0], t), segments[-1][1] + w)
        else:
            segments.append((t, w))
    if len(segments) == 1:
        return segments[0][0]
    if len(segments) == 2 and isinstance(segments[0][0], ltl.Num) \
            and segments[0][0].value & ((1 << segments[0][1]) - 1) == 0:
        return ltl.Unary(f"extend {segments[0][1]}", segments[1][0])   # {n'b0, x}
    out, offset = None, total
    for t, w in segments:
        offset -= w
        if isinstance(t, ltl.Num):
            v = (t.value & ((1 << w) - 1)) << offset
            if v == 0:
                continue
            part = ltl.Num(v)
        else:
            part = ltl.Unary(f"extend {total - w}", t) if total > w else t
            if offset:
                part = ltl.Binary("<<", part, ltl.Num(offset))
        out = part if out is None else ltl.Binary("|", out, part)
    return out


def _as_unsigned(t, signed):
    return ltl.Unary("unsigned", t) if signed else t


def _cast(t, to_signed, signed):
    if isinstance(t, ltl.Num) or to_signed == signed:
        return t
    return ltl.Unary("signed" if to_signed else "unsigned", t)


# disable iff: accept_on ---------------------------------------------------------

def _nnf(f, positive=True):
    """Negations pushed down to the atoms (non-temporal subformulas)."""
    if not _temporal(f):
        return f if positive else _not(f)
    if isinstance(f, ltl.Unary):
        if f.op == "!":
            return _nnf(f.arg, not positive)
        if f.op == "X":
            return ltl.Unary("X", _nnf(f.arg, positive))
        if f.op == "G":
            return ltl.Unary("G" if positive else "F", _nnf(f.arg, positive))
        if f.op == "F":
            return ltl.Unary("F" if positive else "G", _nnf(f.arg, positive))
    op = f.op
    if op == "->":
        return _nnf(ltl.Binary("|", ltl.Unary("!", f.left), f.right), positive)
    if op == "<->":
        both = ltl.Binary("&", f.left, f.right)
        neither = ltl.Binary("&", ltl.Unary("!", f.left), ltl.Unary("!", f.right))
        return _nnf(ltl.Binary("|", both, neither), positive)
    if op in ("&", "|"):
        if not positive:
            op = "|" if op == "&" else "&"
        return ltl.Binary(op, _nnf(f.left, positive), _nnf(f.right, positive))
    if op == "U":
        if positive:
            return ltl.Binary("U", _nnf(f.left), _nnf(f.right))
        return ltl.Binary("R", _nnf(f.left, False), _nnf(f.right, False))
    if op == "R":
        if positive:
            return ltl.Binary("R", _nnf(f.left), _nnf(f.right))
        return ltl.Binary("U", _nnf(f.left, False), _nnf(f.right, False))
    raise SVAError(f"the operator {op} under disable iff")


def _temporal(f) -> bool:
    if isinstance(f, ltl.Unary):
        return f.op in ltl.TEMPORAL_UNARY or _temporal(f.arg)
    if isinstance(f, ltl.Binary):
        return f.op in ltl.TEMPORAL_BINARY or _temporal(f.left) or _temporal(f.right)
    return False


def _accept_on(f, r):
    """accept_on (r) f, for f in negation normal form: f holds on the trace
    where everything holds from the first cycle where r holds."""
    if not _temporal(f):
        return _or(f, r)
    if isinstance(f, ltl.Unary):
        if f.op == "X":
            return _or(r, ltl.Unary("X", _accept_on(f.arg, r)))
        if f.op == "G":       # f W r
            g = _accept_on(f.arg, r)
            return _or(ltl.Binary("U", g, r), ltl.Unary("G", g))
        if f.op == "F":
            return ltl.Unary("F", _accept_on(f.arg, r))
    a, b = _accept_on(f.left, r), _accept_on(f.right, r)
    if f.op in ("&", "|"):
        return (_and if f.op == "&" else _or)(a, b)
    if f.op == "U":
        return ltl.Binary("U", a, b)
    if f.op == "R":           # b W (a & b)
        return _or(ltl.Binary("U", b, _and(a, b)), ltl.Unary("G", b))
    raise SVAError(f"the operator {f.op} under disable iff")


# Delays: X pushed into the atoms, the past turned into the future --------------

_PLACE = re.compile(r"^(.*)@(-?\d+)$")


def _shift(f, k):
    """The atom with every value k cycles later."""
    if isinstance(f, ltl.Var):
        m = _PLACE.match(f.name)
        if m:
            return ltl.Var(f"{m.group(1)}@{int(m.group(2)) + k}")
        return f
    if isinstance(f, ltl.Unary):
        return ltl.Unary(f.op, _shift(f.arg, k))
    if isinstance(f, ltl.Binary):
        return ltl.Binary(f.op, _shift(f.left, k), _shift(f.right, k))
    return f


def _push(f, k=0):
    """X moved into the atoms (X commutes with every LTL operator)."""
    if not _temporal(f):
        return _shift(f, k)
    if isinstance(f, ltl.Unary):
        if f.op == "X":
            return _push(f.arg, k + 1)
        return ltl.Unary(f.op, _push(f.arg, k))
    return ltl.Binary(f.op, _push(f.left, k), _push(f.right, k))


def _delays(f) -> list[int]:
    if isinstance(f, ltl.Var):
        m = _PLACE.match(f.name)
        return [int(m.group(2))] if m else []
    if isinstance(f, ltl.Unary):
        return _delays(f.arg)
    if isinstance(f, ltl.Binary):
        return _delays(f.left) + _delays(f.right)
    return []


def _render(f):
    """Atoms with delays of 0 and more written with X and primes."""
    if _temporal(f):
        if isinstance(f, ltl.Unary):
            return ltl.Unary(f.op, _render(f.arg))
        return ltl.Binary(f.op, _render(f.left), _render(f.right))
    ds = _delays(f)
    if not ds:
        return f
    lo, hi = min(ds), max(ds)
    if lo == hi:
        return _x(_name(_shift(f, -lo), False), lo)
    boolean = (isinstance(f, ltl.Unary) and f.op == "!") or \
        (isinstance(f, ltl.Binary) and f.op in ("&", "|", "->", "<->")) or \
        (isinstance(f, ltl.Binary) and f.op == "xor" and _is_formula_xor(f))
    if boolean:
        if isinstance(f, ltl.Unary):
            return ltl.Unary("!", _render(f.arg))
        return ltl.Binary(f.op, _render(f.left), _render(f.right))
    if hi - lo > 1:
        # values two cycles apart or more: at the latest cycle, with past(x, k)
        return _x(_with_past(f, hi), hi)
    return _x(_name(_shift(f, -lo), True), lo)


def _with_past(f, at):
    """Placeholders x@d read at cycle `at`: x, or past(x, at - d)."""
    if isinstance(f, ltl.Var):
        m = _PLACE.match(f.name)
        if m:
            k = at - int(m.group(2))
            v = ltl.Var(m.group(1))
            return v if k == 0 else ltl.Unary(f"past {k}", v)
        return f
    if isinstance(f, ltl.Unary):
        return ltl.Unary(f.op, _with_past(f.arg, at))
    if isinstance(f, ltl.Binary):
        return ltl.Binary(f.op, _with_past(f.left, at), _with_past(f.right, at))
    return f


def _is_formula_xor(f):
    return isinstance(f.left, (ltl.Binary, ltl.Unary, ltl.Const)) and \
        not (isinstance(f.left, ltl.Unary) and (ltl.bits(f.left) or ltl.function(f.left)))


def _name(f, primes):
    """Placeholders x@0 and x@1 to x and x'."""
    if isinstance(f, ltl.Var):
        m = _PLACE.match(f.name)
        if m:
            d = int(m.group(2))
            return ltl.Var(m.group(1), primed=d == 1)
        return f
    if isinstance(f, ltl.Unary):
        return ltl.Unary(f.op, _name(f.arg, primes))
    if isinstance(f, ltl.Binary):
        return ltl.Binary(f.op, _name(f.left, primes), _name(f.right, primes))
    return f


def _tidy(f):
    """(!a | b) as (a -> b), true and false folded; terms are kept."""
    if isinstance(f, ltl.Binary) and (f.op in ltl.RELATIONS or f.op in ltl.ARITHMETIC
                                      or f.op in ltl.WORD_OPS):
        return f
    if isinstance(f, ltl.Unary) and (ltl.bits(f) or ltl.function(f) or f.op == "neg"):
        return f
    if isinstance(f, ltl.Unary):
        arg = _tidy(f.arg)
        if f.op == "!":
            return _not(arg)
        if f.op in ("G", "F", "X") and isinstance(arg, ltl.Const):
            return arg
        return ltl.Unary(f.op, arg)
    if isinstance(f, ltl.Binary):
        if f.op in ("|", "->"):
            terms = []
            for t in _disjuncts(f):
                t = _tidy(t)
                for u in _disjuncts(t) if not (isinstance(t, ltl.Binary) and t.op == "->") \
                        else [t]:
                    if u not in terms:
                        terms.append(u)
            if any(isinstance(t, ltl.Const) and t.value for t in terms):
                return ltl.Const(True)
            terms = [t for t in terms if not isinstance(t, ltl.Const)]
            negative = [t.arg for t in terms if isinstance(t, ltl.Unary) and t.op == "!"]
            positive = [t for t in terms if not (isinstance(t, ltl.Unary) and t.op == "!")]
            if negative and positive:
                return _implies(_and(*negative), _or(*positive))
            return _or(*terms)
        a, b = _tidy(f.left), _tidy(f.right)
        if f.op == "&":
            return _and(a, b)
        return ltl.Binary(f.op, a, b)
    return f


def _disjuncts(f):
    """The terms of a disjunction; a -> b is !a | b."""
    if isinstance(f, ltl.Binary) and f.op == "|":
        return _disjuncts(f.left) + _disjuncts(f.right)
    if isinstance(f, ltl.Binary) and f.op == "->":
        return [_not(f.left)] + _disjuncts(f.right)
    return [f]


# Reading -------------------------------------------------------------------------

def parse_sva(text: str, inputs=None, outputs=None, name: Optional[str] = None,
              parameters: Optional[dict] = None, clock: Optional[str] = None) -> SVAReading:
    """Read the assumptions and assertions of SVA text as a contract.

    ``inputs`` and ``outputs`` (lists of names, or dicts of types) say which
    signals the environment controls; for the other signals the directions
    of the module ports are used (the attribute ``(* chase = "output" *)``,
    written by ``to_sva``, overrides them), and the signals that are not
    ports are of the other kind than the list given. Hierarchical names
    (``u1.state``) are read as ``u1__state``; ``names`` of the result maps
    them back. Types: the declarations of the text, then
    the values of the dicts ("bool", "word[8]", "signed word[8]"), else
    Booleans. ``parameters`` gives the values of parameters (they override
    those of the text). ``assume property`` and ``restrict property`` give
    assumptions, ``assert property`` guarantees; ``cover property`` is kept
    apart (``covers``). A contract has one clock: ``clock`` ("posedge clk")
    chooses it when the text has several, else the clock of most properties
    is taken, and the properties of the other clocks are skipped. Signals
    without a declaration or a type are read as 1-bit signals and listed in
    ``undeclared``.
    """
    scan = _Scanner(text)
    scan.run()
    params = dict(scan.params)
    for k, v in (parameters or {}).items():
        params[k] = int(v)
    clocks = {c for c in [scan.default_clock] if c}
    main_clock = _main_clock(scan, clock)
    reading = SVAReading(Contract(name or scan.module or "C"), skipped=list(scan.notes))
    types = {k: v for k, v in scan.decls.items()}
    given = {}
    flat = lambda v: v.replace(".", "__")      # noqa: E731  hierarchical names
    inputs = {flat(v): t for v, t in inputs.items()} if isinstance(inputs, dict) else \
        [flat(v) for v in inputs] if inputs is not None else None
    outputs = {flat(v): t for v, t in outputs.items()} if isinstance(outputs, dict) else \
        [flat(v) for v in outputs] if outputs is not None else None
    for group, direction in ((inputs, "input"), (outputs, "output")):
        if isinstance(group, dict):
            for v, t in group.items():
                given[v] = direction
                if v not in types and t is not None:
                    types[v] = _decl_of(t)
        elif group:
            for v in group:
                given[v] = direction

    vector_bits: set = set()
    while True:
        meaning = _Meaning(scan, params, dict(types), vector_bits)
        results, retry = _read_all(scan, meaning, clocks, main_clock)
        if retry is None:
            break
        vector_bits.add(retry)
    for kind, item in results:
        if kind == "skip":
            reading.skipped.append(item)
        elif kind == "cover":
            reading.covers.append(item)
        else:
            reading.properties.append(item)
    reading.clock = main_clock
    reading.names = {v: scan.p.names[v] for v in sorted(meaning.used) if v in scan.p.names}
    reading.undeclared = sorted(v for v in meaning.used if v not in meaning.declared)

    used = sorted(meaning.used)
    variables = {}
    for v in used:
        dec = meaning.types[v]
        if dec.width == 1 and v not in vector_bits:
            variables[v] = "bool"
        else:
            variables[v] = f"{'signed ' if dec.signed else ''}word[{dec.width}]"
    # what is given, then the ports of the module, then the other kind of signal
    other = "output" if inputs is not None and outputs is None else \
        "input" if outputs is not None and inputs is None else None
    directions = {v: given.get(v) or meaning.types[v].direction or other for v in used}
    unknown = [v for v, dirn in directions.items() if dirn not in ("input", "output")]
    if unknown:
        raise SVAError(f"say whether these signals are inputs or outputs: {unknown} "
                       "(inputs=[...]; the other signals are outputs)")
    c = reading.contract
    c.inputs = {v: variables[v] for v in used if directions[v] == "input"}
    c.outputs = {v: variables[v] for v in used if directions[v] == "output"}
    c.assumptions = [p.formula for p in reading.properties if p.kind == "assume"]
    c.guarantees = [p.formula for p in reading.properties if p.kind == "assert"]
    c.validate()
    return reading


def _decl_of(t) -> _Decl:
    if t in ("bool", "boolean", bool):
        return _Decl(0, 0, False)
    w = words.word_type(t)
    if w:
        return _Decl(w[0] - 1, 0, w[1])
    m = re.fullmatch(r"\s*(?:logic|bit|wire|reg)?\s*(signed)?\s*\[\s*(\d+)\s*:\s*(\d+)\s*\]\s*",
                     str(t))
    if m:
        return _Decl(int(m.group(2)), int(m.group(3)), bool(m.group(1)))
    raise SVAError(f"unsupported type {t!r} for SVA: use 'bool', 'word[n]', "
                   "'signed word[n]', or 'logic [7:0]'")


def _clock_of(scan, spec):
    clock, _, body = spec
    while body.k == "paren":
        body = body.a[0]
    if clock is None and body.k in ("id", "call") and body.v in scan.named:
        named = scan.named[body.v][1]
        if isinstance(named, tuple):
            clock = named[0]
    return " ".join((clock or scan.default_clock or "").split()) or None


def _main_clock(scan, chosen):
    if chosen:
        return " ".join(chosen.split())
    counts: dict = {}
    for label, kind, initial, spec, text in scan.asserts:
        if not isinstance(spec, SVAError) and kind != "cover":
            c = _clock_of(scan, spec)
            if c:
                counts[c] = counts.get(c, 0) + 1
    return max(counts, key=lambda c: (counts[c], c)) if counts else scan.default_clock


def _bad_name(n: str) -> bool:
    from .contract import RESERVED
    return n in RESERVED or not n.replace("_", "a").isalnum() or not n[0].isalpha()


def _read_all(scan, meaning, clocks, main_clock):
    out = []
    for label, kind, initial, spec, text in scan.asserts:
        if isinstance(spec, SVAError):
            out.append(("skip", Skipped(label, text, str(spec))))
            continue
        if kind == "cover":
            out.append(("cover", text))
            continue
        clock, disable, body = spec
        own = _clock_of(scan, spec)
        if own and main_clock and own != main_clock:
            out.append(("skip", Skipped(label, text, f"the clock {own}, not {main_clock} (one "
                                        "clock per contract: choose it with clock=)")))
            continue
        used = set(meaning.used)
        try:
            while body.k == "paren":
                body = body.a[0]
            if body.k in ("id", "call") and body.v in scan.named:
                clock2, disable2, body = meaning.instance(body, top=True)
                clock = clock or clock2
                disable = disable if disable is not None else disable2
            clock = clock or scan.default_clock
            if clock:
                clocks.add(clock)
            if disable is None:
                disable = scan.default_disable
            f = meaning.prop(body)
            if disable is not None:
                f = _accept_on(_nnf(f), meaning.formula(disable))
            if not initial:
                f = ltl.Unary("G", f)
            f = _push(f)
            ds = _delays(f)
            start = max(0, -min(ds)) if ds else 0
            if start:
                f = _shift_all(f, start)
            f = _tidy(_render(f))
            bad = sorted(v for v in ltl.variables(f) if _bad_name(v))
            if bad:
                raise SVAError(f"signals whose names CHASE does not take (a keyword, or not "
                               f"starting with a letter): {bad}")
            words.check(f, {v: _ctype(meaning, v) for v in ltl.variables(f)})
            out.append(("prop", Property(label, "assume" if kind in ("assume", "restrict")
                                         else "assert", text, f, start)))
        except _NeedsWord as e:
            return out, e.name
        except (SVAError, words.WordTypeError, ltl.LTLSyntaxError, RecursionError) as e:
            meaning.used = used           # the signals of a skipped property are not kept
            out.append(("skip", Skipped(label, text, str(e) or type(e).__name__)))
    return out, None


def _shift_all(f, k):
    if _temporal(f):
        if isinstance(f, ltl.Unary):
            return ltl.Unary(f.op, _shift_all(f.arg, k))
        return ltl.Binary(f.op, _shift_all(f.left, k), _shift_all(f.right, k))
    return _shift(f, k)


def _ctype(meaning, v):
    dec = meaning.types.get(v)
    if dec is None:
        return "bool"
    if dec.width == 1 and v not in meaning.vector_bits:
        return "bool"
    return f"{'signed ' if dec.signed else ''}word[{dec.width}]"


# Writing -------------------------------------------------------------------------

SV_KEYWORDS = frozenset("""
accept_on alias always always_comb always_ff always_latch and assert assign assume
automatic before begin bind bins binsof bit break buf bufif0 bufif1 byte case casex casez
cell chandle checker class clocking cmos config const constraint context continue cover
covergroup coverpoint cross deassign default defparam design disable dist do edge else end
endcase endchecker endclass endclocking endconfig endfunction endgenerate endgroup
endinterface endmodule endpackage endprimitive endprogram endproperty endspecify
endsequence endtable endtask enum event eventually expect export extends extern final
first_match for force foreach forever fork forkjoin function generate genvar global
highz0 highz1 if iff ifnone ignore_bins illegal_bins implements implies import incdir
include initial inout input inside instance int integer interconnect interface intersect
join join_any join_none large let liblist library local localparam logic longint
macromodule matches medium modport module nand negedge nettype new nexttime nmos nor
noshowcancelled not notif0 notif1 null or output package packed parameter pmos posedge
primitive priority program property protected pull0 pull1 pulldown pullup
pulsestyle_ondetect pulsestyle_onevent pure rand randc randcase randsequence rcmos real
realtime ref reg reject_on release repeat restrict return rnmos rpmos rtran rtranif0
rtranif1 s_always s_eventually s_nexttime s_until s_until_with scalared sequence shortint
shortreal showcancelled signed small soft solve specify specparam static string strong
strong0 strong1 struct super supply0 supply1 sync_accept_on sync_reject_on table tagged
task this throughout time timeprecision timeunit tran tranif0 tranif1 tri tri0 tri1
triand trior trireg type typedef union unique unique0 unsigned until until_with untyped
use uwire var vectored virtual void wait wait_order wand weak weak0 weak1 while wildcard
wire with within wor xnor xor""".split())


def to_sva(contract: Contract, clock: str = "clk", edge: str = "posedge",
           reset: Optional[str] = None, module: Optional[str] = None,
           bind: Optional[str] = None, names: Optional[dict] = None) -> str:
    """The contract as a SystemVerilog checker module.

    Assumptions become ``assume property``, guarantees ``assert property``,
    all clocked by ``@(edge clock)``; a formula G p is checked at every
    cycle, any other formula once, at the first cycle (``initial``).
    ``reset``, an expression such as "rst" or "!rst_n", disables the
    properties while it holds. Words are vectors (``logic [7:0]``), integer
    ranges ``int`` with their range assumed (inputs) or asserted (outputs).
    With ``bind``, the name of a design module, a bind statement puts the
    checker in every instance of it; its ports connect by name (``.*``).
    ``names`` gives the names of the signals in the design where they
    differ, such as the hierarchical names of ``parse_sva`` (its
    ``names``): the bind statement connects them. Real variables cannot be
    written.
    """
    module = module or f"{contract.name}_sva"
    variables = contract.variables
    bad = sorted(v for v in list(variables) + [clock, module] if v in SV_KEYWORDS)
    if bad:
        raise SVAError(f"names that are SystemVerilog keywords: {bad}")
    reals = sorted(v for v, t in variables.items() if t == "real")
    if reals:
        raise SVAError(f"real variables cannot be written in SVA: {reals}")
    w = _Writer(variables)
    lines = [f"// Contract {contract.name}, written by CHASE (chase_cps.to_sva).",
             f"// Assumptions: assume property; guarantees: assert property.",
             f"module {module} ("]
    ports = [f"  input logic {clock}"]
    if reset:
        for v in dict.fromkeys(re.findall(r"[A-Za-z_]\w*", reset)):
            if v not in variables and v != clock:
                ports.append(f"  input logic {v}")
    for group, role in ((contract.inputs, "input"), (contract.outputs, "output")):
        for v, t in group.items():
            ports.append(f'  (* chase = "{role}" *) input {_sv_type(t)} {v}')
    lines.append(",\n".join(ports))
    lines.append(");")
    event = f"@({edge} {clock})"
    disable = f" disable iff ({reset})" if reset else ""

    def statement(kind, label, f):
        if isinstance(f, ltl.Unary) and f.op == "G":
            return f"  {label}: {kind} property ({event}{disable}\n    {w.prop(f.arg)});"
        return f"  initial {label}: {kind} property ({event}{disable}\n    {w.prop(f)});"

    ranges = [(v, t, "assume" if v in contract.inputs else "assert")
              for v, t in variables.items() if isinstance(t, tuple)]
    if ranges:
        lines.append("  // integer ranges")
        for v, (lo, hi), kind in ranges:
            lines.append(f"  range_{v}: {kind} property ({event}{disable} "
                         f"{lo} <= {v} && {v} <= {hi});")
    if contract.assumptions:
        lines.append("  // assumptions")
        for i, f in enumerate(contract.assumptions):
            lines.append(statement("assume", f"A{i}", f))
    if contract.guarantees:
        lines.append("  // guarantees")
        for i, f in enumerate(contract.guarantees):
            lines.append(statement("assert", f"G{i}", f))
    lines.append("endmodule")
    if bind:
        extra = "".join(f", .{v}({n})" for v, n in sorted((names or {}).items())
                        if v in variables)
        lines += ["", f"bind {bind} {module} {module}_i (.*{extra});"]
    return "\n".join(lines) + "\n"


def _sv_type(t) -> str:
    if t == "bool":
        return "logic"
    if isinstance(t, tuple):
        return "int"
    width, signed = words.word_type(t)
    return f"logic {'signed ' if signed else ''}[{width - 1}:0]"


class _Writer:
    def __init__(self, variables):
        self.variables = variables

    def type(self, node):
        return words.type_of(node, self.variables)

    def prop(self, f) -> str:
        """A formula as an SVA property."""
        if isinstance(f, ltl.Binary) and f.op == "->" and not _temporal(f) \
                and not _primed(f.left) and _primed(f.right):
            # a -> b' is a |=> b, b with $past for the values of now
            return f"{self.boolean(f.left)} |=> ({self.expr(f.right, past=True)})"
        if not _temporal(f):
            return self.boolean(f)
        if isinstance(f, ltl.Unary):
            if f.op == "X":
                return f"nexttime ({self.prop(f.arg)})"
            if f.op == "G":
                return f"always ({self.prop(f.arg)})"
            if f.op == "F":
                return f"s_eventually ({self.prop(f.arg)})"
            return f"not ({self.prop(f.arg)})"
        op = f.op
        if op == "->" and not _temporal(f.left) and not _primed(f.left):
            right = f.right
            if isinstance(right, ltl.Unary) and right.op == "X":
                return f"{self.boolean(f.left)} |=> ({self.prop(right.arg)})"
            return f"{self.boolean(f.left)} |-> ({self.prop(right)})"
        a, b = self.prop(f.left), self.prop(f.right)
        if op == "R":
            return f"(({b}) until_with ({a}))"
        name = {"&": "and", "|": "or", "->": "implies", "<->": "iff", "U": "s_until",
                "xor": None}[op]
        if name is None:
            return f"(({a}) iff (not ({b})))"
        return f"(({a}) {name} ({b}))"

    def boolean(self, f) -> str:
        """A formula without temporal operators; with primes, one cycle later."""
        if _primed(f):
            return f"nexttime ({self.expr(f, past=True)})"
        return self.expr(f, past=False)

    def expr(self, f, past) -> str:
        """A formula as a 1-bit expression."""
        if isinstance(f, ltl.Const):
            return "1'b1" if f.value else "1'b0"
        if isinstance(f, ltl.Var) and self.variables.get(f.name, "bool") == "bool":
            return self.name(f, past)
        if ltl.function(f) == "past" and self.variables.get(f.arg.name, "bool") == "bool":
            return self.term(f, past)
        t = self.type(f)
        if isinstance(t, words._W) and not (isinstance(f, ltl.Unary) and f.op == "!"):
            return self.term(f, past)                       # a word of one bit
        if isinstance(f, ltl.Unary) and f.op == "!":
            return f"!({self.expr(f.arg, past)})"
        op = f.op
        if op in ("&", "|", "->", "<->", "xor"):
            a, b = self.expr(f.left, past), self.expr(f.right, past)
            return {"&": f"({a} && {b})", "|": f"({a} || {b})", "->": f"(!({a}) || {b})",
                    "<->": f"({a} == {b})", "xor": f"({a} != {b})"}[op]
        if op in ltl.RELATIONS:
            a, b = self.term(f.left, past, f.right), self.term(f.right, past, f.left)
            return f"({a} {'==' if op == '=' else op} {b})"
        raise SVAError(f"cannot write {ltl.to_str(f)} in SVA")

    def name(self, v, past):
        if past and not v.primed:
            return f"$past({v.name})"
        return v.name

    def term(self, t, past, other=None) -> str:
        if isinstance(t, ltl.Num):
            ty = self.type(other) if other is not None else "num"
            if isinstance(ty, words._W):
                if ty.signed and t.value < 0:
                    return f"-{ty.width}'sd{-t.value}"
                return f"{ty.width}'{'s' if ty.signed else ''}d{t.value}"
            if isinstance(t.value, int):
                return str(t.value)
            raise SVAError(f"the number {t.value} with a fraction")
        if isinstance(t, ltl.Var):
            return self.name(t, past)
        ty = self.type(t)
        if isinstance(t, ltl.Unary):
            b = ltl.bits(t)
            if b:
                inner_t = self.type(t.arg)
                if isinstance(t.arg, ltl.Var):
                    sel = f"[{b[0]}:{b[1]}]" if b[0] != b[1] else f"[{b[0]}]"
                    if past and not t.arg.primed:
                        return f"$past({t.arg.name}{sel})"
                    return f"{t.arg.name}{sel}"
                inner = f"$unsigned({inner_t.width}'({self.term(t.arg, past)}))"
                if b[1]:
                    inner = f"({inner} >> {b[1]})"
                return f"{b[0] - b[1] + 1}'({inner})"
            f = ltl.function(t)
            if f == "past":
                n = ltl.extension(t) + (1 if past else 0)
                return f"$past({t.arg.name}, {n})"
            if f == "extend":
                inner_t = self.type(t.arg)
                inner = self.term(t.arg, past)
                if not isinstance(t.arg, ltl.Var):
                    inner = f"{inner_t.width}'({inner})"
                return f"{ty.width}'({inner})"
            if f:
                return f"${f}({self.term(t.arg, past)})"
            if t.op == "neg":
                return f"(-{self.term(t.arg, past)})"
            if t.op == "!":
                if isinstance(ty, words._W):
                    return f"(~{self.term(t.arg, past)})"
                return f"!({self.expr(t.arg, past)})"
            return self.expr(t, past)
        op = t.op
        if op in ("<<", ">>"):
            left = self.term(t.left, past)
            right = self.term(t.right, past)
            sv = ">>>" if op == ">>" and isinstance(ty, words._W) and ty.signed else op
            return f"({left} {sv} {right})"
        if op == "::":
            return f"{{{self.term(t.left, past)}, {self.term(t.right, past)}}}"
        if op in ltl.ARITHMETIC or op in ("mod", "&", "|", "xor"):
            if not isinstance(ty, words._W) and op in ("&", "|", "xor"):
                return self.expr(t, past)
            sv = {"mod": "%", "xor": "^"}.get(op, op)
            return f"({self.term(t.left, past, t.right)} {sv} {self.term(t.right, past, t.left)})"
        return self.expr(t, past)


def _primed(f) -> bool:
    if isinstance(f, ltl.Var):
        return f.primed
    if isinstance(f, ltl.Unary):
        return _primed(f.arg)
    if isinstance(f, ltl.Binary):
        return _primed(f.left) or _primed(f.right)
    return False


__all__ = ["SVAError", "SVAReading", "Property", "Skipped", "parse_sva", "to_sva"]
