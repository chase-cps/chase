"""LTL formulas: a parser for the standard syntax and printers for CHASE and NuSMV.

The logics language of CHASE gives the same precedence to all the binary
Boolean operators (``a -> b & c`` is read as ``(a -> b) & c``), and a lower
one to G, F and X (``G a -> b`` is read as ``G (a -> b)``). Formulas are
therefore parsed here with the standard LTL precedence and printed fully
parenthesized before being handed to CHASE.

Precedence, from the loosest to the tightest binding::

    <->    ->    | xor    &    U R W    ! G F X    relations    << >>    + -
    * / mod    ::    x[h:l]

``->`` is right-associative, the other binary operators are left-associative.
Primed variables (``x'``) denote the value of ``x`` in the next step.
``[]`` and ``<>`` are accepted as aliases of ``G`` and ``F``.

The operators of words (``chase_cps.words``) are those of nuXmv: ``x[7:0]``
and ``x[3]`` select bits, ``::`` concatenates, ``<<`` and ``>>`` shift,
``mod`` is the remainder, and ``&``, ``|``, ``xor``, ``!`` are bitwise on
words. A bitwise operation inside a relation needs parentheses:
``(x & y) = 0``. ``extend(x, n)`` adds n bits (copies of the sign bit for
a signed word, zeros otherwise), ``signed(x)`` and ``unsigned(x)`` read the
same bits as a signed or an unsigned word.

``past(x, n)`` is the value of the variable x n steps before (x' is the
value one step after); in the first n steps it is any value of the type
of x. It compares values of different steps: ``y = past(x, 2)``.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from fractions import Fraction
from typing import Optional, Union


class LTLSyntaxError(ValueError):
    """Raised when a formula cannot be parsed."""


# AST ------------------------------------------------------------------------

@dataclass(frozen=True)
class Const:
    value: bool


@dataclass(frozen=True)
class Var:
    name: str
    primed: bool = False


@dataclass(frozen=True)
class Num:
    value: Union[int, Fraction]    # an int, or a Fraction for a number with a fraction


def number(value) -> Union[int, Fraction]:
    """An exact number: an int when the value is an integer, else a Fraction.
    Accepts ints, Fractions, floats, and decimal strings such as "20.5"."""
    if isinstance(value, bool):
        raise TypeError("not a number: a Boolean")
    if isinstance(value, float):
        value = str(value)     # the decimal written, not the binary approximation
    f = Fraction(value)
    return int(f) if f.denominator == 1 else f


def number_text(value, fraction: str = "decimal") -> str:
    """A number as text: an integer, a decimal (``fraction="decimal"``), or
    for nuXmv ``f'p/q'`` when it has no finite decimal expansion
    (``fraction="smv"``)."""
    value = number(value)
    if not isinstance(value, Fraction):
        return str(int(value))
    q, twos, fives = value.denominator, 0, 0
    while q % 2 == 0:
        q //= 2
        twos += 1
    while q % 5 == 0:
        q //= 5
        fives += 1
    if q != 1:
        if fraction == "smv":
            return f"f'{value.numerator}/{value.denominator}"
        raise ValueError(f"{value} has no finite decimal expansion")
    digits = max(twos, fives)
    sign = "-" if value < 0 else ""
    scaled = abs(value) * 10 ** digits
    whole, frac = divmod(int(scaled), 10 ** digits)
    return f"{sign}{whole}.{frac:0{digits}d}"


@dataclass(frozen=True)
class Unary:
    op: str            # '!', 'G', 'F', 'X', 'neg' (arithmetic minus), '[h:l]' (bits of a word),
                       # 'signed', 'unsigned', 'extend n' (functions on words),
                       # 'past n' (the value n steps before)
    arg: "Node"


@dataclass(frozen=True)
class Binary:
    op: str            # '&', '|', '->', '<->', 'U', 'R', relations, arithmetic
    left: "Node"
    right: "Node"


Node = Union[Const, Var, Num, Unary, Binary]

RELATIONS = ("=", "!=", "<", "<=", ">", ">=")
ARITHMETIC = ("+", "-", "*", "/")
WORD_OPS = ("mod", "<<", ">>", "::")
FUNCTIONS = ("signed", "unsigned", "extend", "past")
BOOLEAN_BINARY = ("&", "|", "->", "<->")
TEMPORAL_UNARY = ("G", "F", "X")
TEMPORAL_BINARY = ("U", "R")


# Tokenizer ------------------------------------------------------------------

_TOKEN_RE = re.compile(r"""
    \s*(?:
      (?P<num>\d+(?:\.\d+)?)
    | (?P<id>[A-Za-z_][A-Za-z0-9_]*)(?P<prime>')?
    | (?P<op><->|<=>|->|=>|==|!=|<<|>>|<=|>=|::|&&|\|\||/\\|\\/|\[\]|<>|[!~&|()<>=+\-*/\[\]:,])
    )""", re.VERBOSE)

_OP_ALIASES = {"<=>": "<->", "=>": "->", "==": "=", "&&": "&", "||": "|",
               "/\\": "&", "\\/": "|", "~": "!", "[]": "G", "<>": "F"}
_KEYWORDS = {"G", "F", "X", "U", "R", "W", "mod", "xor"}


def _tokenize(text: str) -> list[tuple[str, str]]:
    tokens, pos = [], 0
    text = text.strip().rstrip(";")
    while pos < len(text):
        if text[pos:].strip() == "":
            break
        m = _TOKEN_RE.match(text, pos)
        if not m or m.end() == pos:
            raise LTLSyntaxError(f"unexpected character {text[pos:].strip()[0]!r} "
                                 f"at position {pos} in {text!r}")
        pos = m.end()
        if m.group("num"):
            tokens.append(("num", m.group("num")))
        elif m.group("id"):
            name = m.group("id")
            if name in _KEYWORDS and not m.group("prime"):
                tokens.append(("op", name))
            elif name in ("true", "TRUE", "false", "FALSE"):
                tokens.append(("const", name.lower()))
            else:
                tokens.append(("primed" if m.group("prime") else "id", name))
        else:
            op = m.group("op")
            tokens.append(("op", _OP_ALIASES.get(op, op)))
    tokens.append(("end", ""))
    return tokens


# Parser (precedence climbing) -----------------------------------------------

# Binary operators: (precedence, right associative)
_BINARY = {
    "<->": (1, False), "->": (2, True), "|": (3, False), "xor": (3, False), "&": (4, False),
    "U": (5, True), "R": (5, True), "W": (5, True),
    "=": (7, False), "!=": (7, False), "<": (7, False), "<=": (7, False),
    ">": (7, False), ">=": (7, False),
    "<<": (8, False), ">>": (8, False),
    "+": (9, False), "-": (9, False), "*": (10, False), "/": (10, False), "mod": (10, False),
    "::": (11, False),
}
_UNARY_PRECEDENCE = 6


class _Parser:
    def __init__(self, text: str):
        self.text = text
        self.tokens = _tokenize(text)
        self.i = 0

    def peek(self):
        return self.tokens[self.i]

    def take(self):
        tok = self.tokens[self.i]
        self.i += 1
        return tok

    def expect(self, value):
        kind, val = self.take()
        if val != value:
            raise LTLSyntaxError(f"expected {value!r} but found {val or 'end of formula'!r} "
                                 f"in {self.text!r}")

    def parse(self) -> Node:
        node = self.expression(0)
        if self.peek()[0] != "end":
            raise LTLSyntaxError(f"unexpected {self.peek()[1]!r} in {self.text!r}")
        return node

    def expression(self, min_prec: int) -> Node:
        left = self.prefix()
        while True:
            kind, op = self.peek()
            if kind != "op" or op not in _BINARY:
                return left
            prec, right_assoc = _BINARY[op]
            if prec < min_prec:
                return left
            self.take()
            right = self.expression(prec if right_assoc else prec + 1)
            if op == "W":   # a W b == (a U b) | G a
                left = Binary("|", Binary("U", left, right), Unary("G", left))
            else:
                left = Binary(op, left, right)

    def prefix(self) -> Node:
        kind, val = self.take()
        if kind == "num":
            return Num(number(val))
        if kind == "const":
            return Const(val == "true")
        if kind == "id" and val in FUNCTIONS and self.peek() == ("op", "("):
            self.take()
            arg = self.expression(0)
            if val in ("extend", "past"):
                self.expect(",")
                kind, n = self.take()
                if kind != "num" or not n.isdigit() or (val == "past" and int(n) < 1):
                    what = "a number of bits n" if val == "extend" else "a number of steps n > 0"
                    raise LTLSyntaxError(f"{val}(x, n) takes {what}, not "
                                         f"{n or 'end of formula'!r}, in {self.text!r}")
                if val == "past" and not (isinstance(arg, Var) and not arg.primed):
                    raise LTLSyntaxError(f"past(x, n) takes a variable, in {self.text!r}")
                val = f"{val} {int(n)}"
            self.expect(")")
            return self.bits(Unary(val, arg))
        if kind == "id":
            return self.bits(Var(val))
        if kind == "primed":
            return self.bits(Var(val, primed=True))
        if kind == "op" and val == "(":
            node = self.expression(0)
            self.expect(")")
            return self.bits(node)
        if kind == "op" and val in ("!", "G", "F", "X"):
            return Unary(val, self.expression(_UNARY_PRECEDENCE))
        if kind == "op" and val == "-":
            arg = self.expression(_BINARY["*"][0] + 1)
            return Num(-arg.value) if isinstance(arg, Num) else Unary("neg", arg)
        raise LTLSyntaxError(f"unexpected {val or 'end of formula'!r} in {self.text!r}")

    def bits(self, node: Node) -> Node:
        """Bit selections after an operand: x[7:0], x[3]."""
        while self.peek() == ("op", "["):
            self.take()
            high = self.index()
            low = high
            if self.peek() == ("op", ":"):
                self.take()
                low = self.index()
            self.expect("]")
            if low > high:
                raise LTLSyntaxError(f"bits [{high}:{low}]: the high bit comes first, "
                                     f"in {self.text!r}")
            node = Unary(f"[{high}:{low}]", node)
        return node

    def index(self) -> int:
        kind, val = self.take()
        if kind != "num" or not val.isdigit():
            raise LTLSyntaxError(f"a bit index is a number, not {val or 'end of formula'!r}, "
                                 f"in {self.text!r}")
        return int(val)


def function(node: Node) -> Optional[str]:
    """The function of words applied by the node ("signed", "unsigned",
    "extend"), or None."""
    if isinstance(node, Unary) and node.op.split(" ")[0] in FUNCTIONS:
        return node.op.split(" ")[0]
    return None


def extension(node: Node) -> int:
    """The number of bits that ``extend`` adds, or the steps of ``past``."""
    return int(node.op.split(" ")[1])


def past_name(node: Node) -> str:
    """The history variable that holds past(x, n) in a model."""
    return f"chase_past{extension(node)}_{node.arg.name}"


def has_past(node: Node) -> bool:
    if isinstance(node, Unary):
        return function(node) == "past" or has_past(node.arg)
    if isinstance(node, Binary):
        return has_past(node.left) or has_past(node.right)
    return False


def pasts(node: Node) -> set:
    """The (variable, steps) of the past(x, n) of a formula."""
    if isinstance(node, Unary):
        if function(node) == "past":
            return {(node.arg.name, extension(node))}
        return pasts(node.arg)
    if isinstance(node, Binary):
        return pasts(node.left) | pasts(node.right)
    return set()


def bits(node: Node) -> Optional[tuple[int, int]]:
    """(high, low) when the node selects bits of a word, else None."""
    if isinstance(node, Unary) and node.op.startswith("["):
        high, low = node.op[1:-1].split(":")
        return int(high), int(low)
    return None


def parse(text: str) -> Node:
    """Parse a formula written in the standard LTL syntax."""
    if not isinstance(text, str) or not text.strip():
        raise LTLSyntaxError("empty formula")
    node = _Parser(text).parse()
    _check_terms(node, None)
    return node


def _check_terms(node: Node, relation: Optional[str]) -> None:
    """Temporal operators apply to formulas, not to the operands of a
    relation or of arithmetic: the value of x in the next step is x'."""
    if isinstance(node, Unary):
        if relation and node.op in TEMPORAL_UNARY:
            raise LTLSyntaxError(
                f"{node.op} applied to an operand of '{relation}': temporal operators apply "
                f"to formulas; write x' for the value of x in the next step")
        _check_terms(node.arg, relation)
    elif isinstance(node, Binary):
        if relation and node.op in TEMPORAL_BINARY:
            raise LTLSyntaxError(f"{node.op} inside an operand of '{relation}'")
        inner = node.op if node.op in RELATIONS + ARITHMETIC + WORD_OPS else relation
        _check_terms(node.left, inner)
        _check_terms(node.right, inner)


# Queries on formulas ---------------------------------------------------------

def variables(node: Node) -> set[str]:
    """Names of the variables occurring in a formula (primed or not)."""
    if isinstance(node, Var):
        return {node.name}
    if isinstance(node, Unary):
        return variables(node.arg)
    if isinstance(node, Binary):
        return variables(node.left) | variables(node.right)
    return set()


def has_fraction(node: Node) -> bool:
    """Whether a number with a fraction (such as 2.5) occurs in a formula."""
    if isinstance(node, Num):
        return isinstance(node.value, Fraction)
    if isinstance(node, Unary):
        return has_fraction(node.arg)
    if isinstance(node, Binary):
        return has_fraction(node.left) or has_fraction(node.right)
    return False


def conjunction(nodes: list[Node]) -> Node:
    if not nodes:
        return Const(True)
    result = nodes[0]
    for n in nodes[1:]:
        result = Binary("&", result, n)
    return result


def conjuncts(node: Node) -> list[Node]:
    """Split a formula on its top-level conjunctions."""
    if isinstance(node, Binary) and node.op == "&":
        return conjuncts(node.left) + conjuncts(node.right)
    if isinstance(node, Const) and node.value:
        return []
    return [node]


# Printers --------------------------------------------------------------------

def to_chase(node: Node) -> str:
    """Fully parenthesized formula in the logics language of CHASE."""
    if isinstance(node, Const):
        return "true" if node.value else "false"
    if isinstance(node, Var):
        return node.name + ("'" if node.primed else "")
    if isinstance(node, Num):
        # The logics language reads -3 as one token, which it rejects.
        if node.value < 0:
            return f"(0 - {number_text(-node.value)})"
        return number_text(node.value)
    if isinstance(node, Unary):
        if node.op == "neg":
            return f"(0 - {to_chase(node.arg)})"
        if bits(node) or function(node):
            raise ValueError(f"{'bits' if bits(node) else function(node)} of a word "
                             f"({to_str(node)}) are not in the logics language of CHASE")
        # Always parenthesized: the logics language before 0.5.1 read
        # G a -> b as G (a -> b).
        return f"({node.op} ({to_chase(node.arg)}))"
    op = node.op
    if op in WORD_OPS:
        raise ValueError(f"the operator {op} of words ({to_str(node)}) is not in the logics "
                         "language of CHASE")
    if op == "xor":
        return f"(! ({to_chase(node.left)} <-> {to_chase(node.right)}))"
    return f"({to_chase(node.left)} {op} {to_chase(node.right)})"


def to_smv(node: Node) -> str:
    """Formula in the LTL syntax of NuSMV and nuXmv."""
    if isinstance(node, Const):
        return "TRUE" if node.value else "FALSE"
    if isinstance(node, Var):
        return f"next({node.name})" if node.primed else node.name
    if isinstance(node, Num):
        return number_text(node.value, "smv")
    if isinstance(node, Unary):
        if node.op == "neg":
            return f"(-{to_smv(node.arg)})"
        if bits(node):
            return f"{to_smv(node.arg)}{node.op}"
        if function(node) == "extend":
            return f"extend({to_smv(node.arg)}, {extension(node)})"
        if function(node) == "past":
            return past_name(node)
        if function(node):
            return f"{node.op}({to_smv(node.arg)})"
        return f"{node.op} ({to_smv(node.arg)})"
    op = "V" if node.op == "R" else node.op
    return f"({to_smv(node.left)} {op} {to_smv(node.right)})"


def to_str(node: Node) -> str:
    """Readable formula in the standard syntax (parenthesized where needed)."""
    return _pretty(node, 0)


def _pretty(node: Node, outer: int) -> str:
    if isinstance(node, (Const, Var, Num)):
        return to_chase(node)
    if isinstance(node, Unary):
        if node.op == "neg":
            return f"-{_pretty(node.arg, _BINARY['*'][0] + 1)}"
        if bits(node):
            return f"{_pretty(node.arg, 99)}{node.op}"
        if function(node) in ("extend", "past"):
            return f"{function(node)}({_pretty(node.arg, 0)}, {extension(node)})"
        if function(node):
            return f"{node.op}({_pretty(node.arg, 0)})"
        sep = "" if node.op == "!" else " "
        return f"{node.op}{sep}{_pretty(node.arg, _UNARY_PRECEDENCE)}"
    prec, right_assoc = _BINARY[node.op]
    left = _pretty(node.left, prec + (1 if right_assoc else 0))
    right = _pretty(node.right, prec + (0 if right_assoc else 1))
    text = f"{left} {node.op} {right}"
    return f"({text})" if prec < outer else text


# Conversion from the CHASE AST (pychase) --------------------------------------

def from_pychase(obj) -> Node:
    """Convert a pychase formula (LogicFormula, Proposition, Expression...)."""
    from ._native import representation as R

    name = type(obj).__name__
    if name == "BooleanConstant":
        return Const(bool(obj.getValue()))
    if name == "Proposition":
        value = obj.getValue()
        if value is not None:
            return from_pychase(value)
        return Var(obj.getName().getString())
    if name == "Identifier":
        text = obj.getString()
        return Var(text.rstrip("'"), primed=text.endswith("'"))
    if name == "IntegerValue":
        return Num(int(obj.getValue()))
    if name == "RealValue":
        return Num(number(obj.getValue()))
    if name == "BooleanValue":
        return Const(bool(obj.getValue()))
    if name == "UnaryBooleanFormula":
        return Unary("!", from_pychase(obj.getOp1()))
    if name == "UnaryTemporalFormula":
        op = {R.TemporalOperator.op_globally: "G", R.TemporalOperator.op_future: "F",
              R.TemporalOperator.op_next: "X"}[obj.getOp()]
        arg = from_pychase(obj.getFormula())
        steps = _interval(obj.getString().lstrip()[1:], op)
        return Unary(op, arg) if steps is None else _bounded(op, steps, arg)
    if name == "BinaryTemporalFormula":
        op = {R.TemporalOperator.op_until: "U", R.TemporalOperator.op_release: "R"}[obj.getOp()]
        left, right = obj.getFormula1(), obj.getFormula2()
        rest = obj.getString()[len(left.getString()):].lstrip()[1:]   # after the U or the R
        steps = _interval(rest, op)
        f1, f2 = from_pychase(left), from_pychase(right)
        return Binary(op, f1, f2) if steps is None else _bounded(op, steps, f1, f2)
    if name == "BinaryBooleanFormula":
        return _boolean(obj.getOp(), [from_pychase(obj.getOp1()), from_pychase(obj.getOp2())])
    if name == "LargeBooleanFormula":
        return _boolean(obj.getOp(), [from_pychase(o) for o in obj.operands])
    if name == "Expression":
        op = _EXPRESSION_OPS[obj.getOperator().name]
        left, right = obj.getOp1(), obj.getOp2()
        if right is None:   # unary minus
            return Unary("neg", from_pychase(left))
        return Binary(op, from_pychase(left), from_pychase(right))
    if name == "ProbabilityFunction":
        raise ValueError("probabilities, P(...), are not supported: chase-cps checks LTL only")
    raise ValueError(f"unsupported CHASE formula node: {name}")


# Bounded temporal operators: G[a,b] p, F[a,b] p, p U[a,b] q, p R[a,b] q,
# with ] or [ for open ends. In discrete time the bounds count steps, and the
# operators unfold into X: G[a,b] p is X^a p & ... & X^b p.

_INTERVAL = re.compile(r"^\s*([\[\]])\s*([^,\[\]]+?)\s*,\s*([^,\[\]]+?)\s*([\[\]])")
MAX_STEPS = 1000


def _interval(text: str, op: str):
    """The steps (first, last) of the interval at the start of text, or None
    when there is no interval."""
    m = _INTERVAL.match(text)
    if not m:
        return None
    if op == "X":
        raise ValueError(f"X with an interval ({m.group(0).strip()}) is not supported: write X X p")
    bounds = []
    for b in (m.group(2), m.group(3)):
        try:
            v = float(b)
        except ValueError:
            raise ValueError(f"the interval {m.group(0).strip()} of {op} has a bound that is not "
                             "a number of steps") from None
        if v != int(v) or v < 0:
            raise ValueError(f"the interval {m.group(0).strip()} of {op}: the bounds count steps, "
                             "whole numbers from 0")
        bounds.append(int(v))
    first = bounds[0] + (1 if m.group(1) == "]" else 0)      # ]a: a left out
    last = bounds[1] - (1 if m.group(4) == "[" else 0)       # b[: b left out
    if last - first > MAX_STEPS:
        raise ValueError(f"the interval {m.group(0).strip()} of {op} spans more than "
                         f"{MAX_STEPS} steps")
    return first, last


def _next(f: Node, k: int) -> Node:
    for _ in range(k):
        f = Unary("X", f)
    return f


def _bounded(op: str, steps, f: Node, g: Optional[Node] = None) -> Node:
    first, last = steps
    if op in ("G", "F"):
        if first > last:              # no step: G holds, F does not
            return Const(op == "G")
        terms = [_next(f, k) for k in range(first, last + 1)]
        return conjunction(terms) if op == "G" else _disjunction(terms)
    if op == "R":                     # p R q = !(!p U !q)
        return Unary("!", _bounded("U", steps, Unary("!", f), Unary("!", g)))
    if first > last:
        return Const(False)
    # p U[0,n] q: q now, or p now and p U[0,n-1] q next
    u = g
    for _ in range(last - first):
        u = Binary("|", g, Binary("&", f, Unary("X", u)))
    # p U[a,b] q: p for the first a steps, then p U[0,b-a] q
    return conjunction([_next(f, k) for k in range(first)] + [_next(u, first)])


def _disjunction(terms: list) -> Node:
    out = terms[0]
    for t in terms[1:]:
        out = Binary("|", out, t)
    return out


_EXPRESSION_OPS = {"op_eq": "=", "op_neq": "!=", "op_lt": "<", "op_le": "<=",
                   "op_gt": ">", "op_ge": ">=", "op_plus": "+", "op_minus": "-",
                   "op_multiply": "*", "op_divide": "/"}


def _boolean(op, args: list[Node]) -> Node:
    from ._native import representation as R
    B = R.BooleanOperator
    if op == B.op_and:
        return conjunction(args)
    if op == B.op_or:
        result = args[0]
        for a in args[1:]:
            result = Binary("|", result, a)
        return result
    if op == B.op_implies:
        return Binary("->", args[0], args[1])
    if op == B.op_iff:
        return Binary("<->", args[0], args[1])
    if op == B.op_xor:
        return Unary("!", Binary("<->", args[0], args[1]))
    if op == B.op_nand:
        return Unary("!", conjunction(args))
    if op == B.op_nor:
        return Unary("!", _boolean(B.op_or, args))
    if op == B.op_xnor:
        return Binary("<->", args[0], args[1])
    raise TypeError(f"unsupported Boolean operator: {op}")
