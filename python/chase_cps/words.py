"""Words: unsigned and signed bit vectors of a fixed width.

A word variable has the type ``"word[8]"`` (unsigned) or ``"signed word[8]"``,
as in nuXmv (``unsigned word[8]``) and SystemVerilog (``logic [7:0]``). Its
arithmetic wraps around: for a word[8] x equal to 255, x + 1 = 0. The
operators are those of nuXmv (see ``chase_cps.ltl``): + - * / mod, the
bitwise & | xor !, the shifts << >>, the concatenation ::, and the bits
x[7:0], x[3]. Both operands of an operation or a relation have the same
width and signedness; an integer constant takes the type of the other
operand. A single bit x[3] can be used as a Boolean.

The checks give words to nuXmv as they are. The contract algebra of the CHASE
core library reads the logics language, which has no words: each relation
over words is replaced by a fresh Boolean variable for the operation, and put
back after it (``Abstraction``). The operations only combine the formulas,
so the result is the same.

GR(1) synthesis (slugs) does not take words: use integer ranges.
"""

from __future__ import annotations

import re
from typing import Optional, Union

from . import ltl

_TYPE = re.compile(r"^(?:(unsigned|signed)\s+)?word\s*\[\s*(\d+)\s*\]$")


def word_type(t) -> Optional[tuple[int, bool]]:
    """(width, signed) of a word type, or None for another type."""
    if isinstance(t, str):
        m = _TYPE.match(t.strip())
        if m:
            return int(m.group(2)), m.group(1) == "signed"
    return None


def normalize(t: str) -> str:
    """``"word[8]"`` or ``"signed word[8]"``."""
    width, signed = word_type(t)
    if width < 1:
        raise ValueError(f"a word has at least one bit: {t!r}")
    return f"{'signed ' if signed else ''}word[{width}]"


def smv_type(t: str) -> str:
    width, signed = word_type(t)
    return f"{'signed' if signed else 'unsigned'} word[{width}]"


def has_words(variables) -> bool:
    return any(word_type(t) for t in variables.values())


def literal(value: int, width: int, signed: bool) -> str:
    """An integer as a word constant of nuXmv (0ud8_5, -0sd8_3)."""
    lo, hi = (-(1 << (width - 1)), (1 << (width - 1)) - 1) if signed else (0, (1 << width) - 1)
    if not lo <= value <= hi:
        raise ValueError(f"{value} is not a {'signed ' if signed else ''}word[{width}] "
                         f"({lo}..{hi})")
    if signed:
        return f"-0sd{width}_{-value}" if value < 0 else f"0sd{width}_{value}"
    return f"0ud{width}_{value}"


_VALUE = re.compile(r"^(-)?0([us])?([bodh])(\d+)_([0-9a-fA-F_]+)$")


def value(text: str) -> Optional[int]:
    """The integer of a word constant printed by nuXmv (0ud8_5, -0sd8_3,
    0ub4_0101), or None."""
    m = _VALUE.match(text.strip())
    if not m:
        return None
    base = {"b": 2, "o": 8, "d": 10, "h": 16}[m.group(3)]
    n = int(m.group(5).replace("_", ""), base)
    width = int(m.group(4))
    if m.group(2) == "s" and base != 10 and n >= 1 << (width - 1):
        n -= 1 << width       # two's complement
    return -n if m.group(1) else n


# Types of expressions ---------------------------------------------------------

class WordTypeError(ValueError):
    """An expression whose operands do not have compatible types."""


class _W:
    """The type of a word expression."""
    __slots__ = ("width", "signed")

    def __init__(self, width: int, signed: bool):
        self.width, self.signed = width, signed

    def __eq__(self, other):
        return isinstance(other, _W) and (self.width, self.signed) == (other.width, other.signed)

    def __str__(self):
        return f"{'signed ' if self.signed else ''}word[{self.width}]"


Type = Union[str, _W]       # "bool", "int", "real", "num" (an integer constant), or a word

_BOOLEAN = ("&", "|", "xor", "->", "<->")


def type_of(node: ltl.Node, variables) -> Type:
    """The type of an expression; raises WordTypeError when the operands do
    not fit."""
    if isinstance(node, ltl.Const):
        return "bool"
    if isinstance(node, ltl.Num):
        return "num" if isinstance(node.value, int) else "real"
    if isinstance(node, ltl.Var):
        t = variables.get(node.name, "bool")
        w = word_type(t)
        if w:
            return _W(*w)
        return t if t in ("bool", "real") else "int"
    if isinstance(node, ltl.Unary):
        arg = type_of(node.arg, variables)
        b = ltl.bits(node)
        if b:
            if not isinstance(arg, _W):
                raise WordTypeError(f"bits of {ltl.to_str(node.arg)}, which is not a word")
            high, low = b
            if high >= arg.width:
                raise WordTypeError(f"bit {high} of {ltl.to_str(node.arg)}, a {arg}")
            return _W(high - low + 1, False)
        f = ltl.function(node)
        if f == "past":
            return arg
        if f:
            if not isinstance(arg, _W):
                raise WordTypeError(f"{f} of {ltl.to_str(node.arg)}, which is not a word")
            if f == "extend":
                return _W(arg.width + ltl.extension(node), arg.signed)
            return _W(arg.width, f == "signed")
        if node.op == "neg":
            if arg == "bool":
                raise WordTypeError(f"minus applied to {ltl.to_str(node.arg)}, a Boolean")
            return arg
        if node.op == "!" and isinstance(arg, _W) and arg.width > 1:
            return arg                                  # bitwise not
        _boolean(arg, node.arg)
        return "bool"
    a, b = type_of(node.left, variables), type_of(node.right, variables)
    op = node.op
    if op in ("&", "|", "xor") and "bool" not in (a, b) \
            and (isinstance(a, _W) or isinstance(b, _W)):
        return _same(a, b, node)                        # bitwise
    if op in _BOOLEAN or op in ltl.TEMPORAL_BINARY:
        _boolean(a, node.left)
        _boolean(b, node.right)
        return "bool"
    if op == "::":
        if not (isinstance(a, _W) and isinstance(b, _W)):
            raise WordTypeError(f"{ltl.to_str(node)}: :: concatenates two words")
        return _W(a.width + b.width, False)
    if op in ("<<", ">>"):
        if not isinstance(a, _W):
            raise WordTypeError(f"{ltl.to_str(node)}: {op} shifts a word")
        if b not in ("num", "int") and not isinstance(b, _W):
            raise WordTypeError(f"{ltl.to_str(node)}: the shift is a number")
        return a
    if op in ltl.ARITHMETIC or op == "mod":
        if isinstance(a, _W) or isinstance(b, _W):
            return _same(a, b, node)
        if "bool" in (a, b):
            raise WordTypeError(f"{ltl.to_str(node)}: arithmetic on a Boolean")
        if op == "mod" and "real" in (a, b):
            raise WordTypeError(f"{ltl.to_str(node)}: mod of a real")
        return "real" if "real" in (a, b) else ("num" if a == b == "num" else "int")
    if op in ltl.RELATIONS:
        if isinstance(a, _W) or isinstance(b, _W):
            _same(a, b, node)
        elif ("bool" in (a, b)) and (a != b or op not in ("=", "!=")):
            raise WordTypeError(f"{ltl.to_str(node)}: a Boolean compared with a number")
        return "bool"
    raise WordTypeError(f"unknown operator {op}")


def _same(a: Type, b: Type, node) -> _W:
    if isinstance(a, _W) and isinstance(b, _W):
        if a != b:
            raise WordTypeError(f"{ltl.to_str(node)}: a {a} and a {b}; both operands must "
                                "have the same width and signedness (select or concatenate "
                                "bits to change the width)")
        return a
    w, other = (a, b) if isinstance(a, _W) else (b, a)
    if other != "num":
        raise WordTypeError(f"{ltl.to_str(node)}: a {w} and a{'n' if other == 'int' else ''} "
                            f"{'integer' if other == 'int' else other}; a word goes with words "
                            "and integer constants")
    return w


def _boolean(t: Type, node) -> None:
    if t == "bool" or (isinstance(t, _W) and t.width == 1):
        return
    raise WordTypeError(f"{ltl.to_str(node)} is a{'' if isinstance(t, _W) else ' number:'} "
                        f"{t}, not a formula")


def check(node: ltl.Node, variables) -> None:
    """Raises WordTypeError when a formula does not type, or a constant does
    not fit in its word."""
    _boolean(type_of(node, variables), node)
    try:
        _Printer(variables).formula(node)
    except WordTypeError:
        raise
    except ValueError as e:
        raise WordTypeError(f"{ltl.to_str(node)}: {e}") from None


# nuXmv ------------------------------------------------------------------------

def to_smv(node: ltl.Node, variables) -> str:
    """A formula for NuSMV and nuXmv; the constants that meet words are
    written as words. Without words, the same as ``ltl.to_smv``."""
    if not has_words(variables):
        return ltl.to_smv(node)
    return _Printer(variables).formula(node)


class _Printer:
    def __init__(self, variables):
        self.variables = variables

    def type(self, node):
        return type_of(node, self.variables)

    def formula(self, node) -> str:
        """node where a Boolean is expected."""
        t = self.type(node)
        text = self.term(node, None)
        if isinstance(t, _W):
            return f"bool({text})"
        return text

    def term(self, node, want: Optional[_W]) -> str:
        if isinstance(node, ltl.Num) and isinstance(want, _W) and isinstance(node.value, int):
            return literal(node.value, want.width, want.signed)
        if isinstance(node, (ltl.Const, ltl.Var, ltl.Num)):
            return ltl.to_smv(node)
        t = self.type(node)
        if isinstance(node, ltl.Unary):
            if ltl.bits(node):
                return f"{self.term(node.arg, None)}{node.op}"
            if ltl.function(node) == "extend":
                return f"extend({self.term(node.arg, None)}, {ltl.extension(node)})"
            if ltl.function(node) == "past":
                return ltl.past_name(node)
            if ltl.function(node):
                return f"{node.op}({self.term(node.arg, None)})"
            if node.op == "neg":
                return f"(-{self.term(node.arg, want if isinstance(t, _W) else None)})"
            if node.op == "!" and isinstance(t, _W):
                return f"(!{self.term(node.arg, t)})"
            return f"{node.op} ({self.formula(node.arg)})"
        op = "V" if node.op == "R" else node.op
        if t == "bool" and (node.op in _BOOLEAN or node.op in ltl.TEMPORAL_BINARY):
            return f"({self.formula(node.left)} {op} {self.formula(node.right)})"
        a, b = self.type(node.left), self.type(node.right)
        if node.op in ("<<", ">>"):
            return f"({self.term(node.left, a)} {op} {self.term(node.right, None)})"
        w = a if isinstance(a, _W) else (b if isinstance(b, _W) else None)
        if node.op == "::":
            w = None
        return f"({self.term(node.left, w)} {op} {self.term(node.right, w)})"


# The contract algebra ----------------------------------------------------------

class Abstraction:
    """Replaces the relations over words by fresh Boolean variables, and
    back. The same relation gets the same variable in every contract."""

    def __init__(self):
        self.atoms: dict = {}       # relation -> fresh name
        self.words: dict = {}       # word variable -> type
        self.roles: dict = {}       # word variable -> "input" or "output"

    def contract(self, c):
        from .contract import Contract
        words = {v: t for v, t in c.variables.items() if word_type(t)}
        if not words and not any(_wordy(f) for f in c.assumptions + c.guarantees):
            return c
        self.words.update(words)
        for v in words:
            if v in c.outputs or self.roles.get(v) == "output":
                self.roles[v] = "output"
            else:
                self.roles.setdefault(v, "input")
        fresh = {}

        def atom(node):
            if isinstance(node, ltl.Unary) and (node.op == "!" or node.op in ltl.TEMPORAL_UNARY):
                return ltl.Unary(node.op, atom(node.arg))
            if isinstance(node, ltl.Binary) and (node.op in ("&", "|", "->", "<->")
                                                 or node.op in ltl.TEMPORAL_BINARY
                                                 or node.op == "xor" and not _wordy(node)):
                return ltl.Binary(node.op, atom(node.left), atom(node.right))
            if not _wordy(node, words):
                return node
            name = self.atoms.get(node)
            if name is None:
                name = self._fresh(c)
                self.atoms[node] = name
            out = ltl.variables(node) & set(c.outputs)
            fresh[name] = "output" if out else "input"
            return ltl.Var(name)

        assumptions = [atom(f) for f in c.assumptions]
        guarantees = [atom(f) for f in c.guarantees]
        inputs = {v: t for v, t in c.inputs.items() if v not in words}
        outputs = {v: t for v, t in c.outputs.items() if v not in words}
        for name, role in fresh.items():
            (outputs if role == "output" else inputs)[name] = "bool"
        return Contract(c.name, inputs, outputs, assumptions, guarantees)

    def _fresh(self, c) -> str:
        k = len(self.atoms)
        while f"chase_w{k}" in c.variables:
            k += 1
        return f"chase_w{k}"

    def restore(self, c):
        """The contract with the relations put back and the word variables
        declared again (as outputs where they were outputs of an operand)."""
        if not self.atoms and not self.words:
            return c
        back = {name: node for node, name in self.atoms.items()}

        def put(node):
            if isinstance(node, ltl.Var) and node.name in back:
                return back[node.name]
            if isinstance(node, ltl.Unary):
                return ltl.Unary(node.op, put(node.arg))
            if isinstance(node, ltl.Binary):
                return ltl.Binary(node.op, put(node.left), put(node.right))
            return node

        c.assumptions = [put(f) for f in c.assumptions]
        c.guarantees = [put(f) for f in c.guarantees]
        c.inputs = {v: t for v, t in c.inputs.items() if v not in back}
        c.outputs = {v: t for v, t in c.outputs.items() if v not in back}
        for v, t in self.words.items():
            (c.outputs if self.roles[v] == "output" else c.inputs)[v] = t
        return c


def _wordy(node, words=None) -> bool:
    """Whether an expression has bits, a word operator, or (with words) a
    word variable."""
    if isinstance(node, ltl.Var):
        return bool(words) and node.name in words
    if isinstance(node, ltl.Unary):
        return bool(ltl.bits(node) or ltl.function(node)) or _wordy(node.arg, words)
    if isinstance(node, ltl.Binary):
        return node.op in ltl.WORD_OPS or _wordy(node.left, words) or _wordy(node.right, words)
    return False


# Values -------------------------------------------------------------------------

def values(t) -> Optional[list]:
    """The values of a word type of at most 12 bits, or None."""
    width, signed = word_type(t)
    if width > 12:
        return None
    return list(range(-(1 << (width - 1)), 1 << (width - 1))) if signed \
        else list(range(1 << width))


def evaluate(node: ltl.Node, state: dict, variables):
    """The value of a formula or a term without temporal operators in a state
    (integers for the words), with the arithmetic of words wrapping around."""

    def fit(x: int, t) -> int:
        if isinstance(t, _W):
            x &= (1 << t.width) - 1
            if t.signed and x >= 1 << (t.width - 1):
                x -= 1 << t.width
        return x

    def ev(n):
        if isinstance(n, (ltl.Const, ltl.Num)):
            return n.value
        if isinstance(n, ltl.Var):
            return state[n.name]
        t = type_of(n, variables)
        if isinstance(n, ltl.Unary):
            a = ev(n.arg)
            b = ltl.bits(n)
            if b:
                return (a >> b[1]) & ((1 << (b[0] - b[1] + 1)) - 1)
            if ltl.function(n) == "past":
                raise ValueError(f"{ltl.to_str(n)}: past values are not in one state")
            if ltl.function(n):
                return fit(a, t)        # extend keeps the value; the casts keep the bits
            if n.op == "neg":
                return fit(-a, t)
            return fit(~a, t) if isinstance(t, _W) else not a
        a, b = ev(n.left), ev(n.right)
        op = n.op
        if isinstance(t, _W):
            if op in ("/", "mod"):
                if b == 0:
                    raise ZeroDivisionError(ltl.to_str(n))
                q = abs(a) // abs(b) * (1 if (a >= 0) == (b >= 0) else -1)
                return fit(q if op == "/" else a - q * b, t)
            if op == "::":
                w = type_of(n.right, variables).width
                return fit((a << w) | (b & ((1 << w) - 1)), t)
            return fit({"&": lambda: a & b, "|": lambda: a | b, "xor": lambda: a ^ b,
                        "+": lambda: a + b, "-": lambda: a - b, "*": lambda: a * b,
                        "<<": lambda: a << b, ">>": lambda: a >> b}[op](), t)
        return {"&": lambda: bool(a) and bool(b), "|": lambda: bool(a) or bool(b),
                "xor": lambda: bool(a) != bool(b), "->": lambda: (not a) or bool(b),
                "<->": lambda: bool(a) == bool(b), "=": lambda: a == b, "!=": lambda: a != b,
                "<": lambda: a < b, "<=": lambda: a <= b, ">": lambda: a > b,
                ">=": lambda: a >= b, "+": lambda: a + b, "-": lambda: a - b,
                "*": lambda: a * b, "/": lambda: a / b, "mod": lambda: a % b}[op]()

    return ev(node)
