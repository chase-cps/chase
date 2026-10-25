"""LTL formulas: a parser for the standard syntax and printers for CHASE and NuSMV.

The logics language of CHASE gives the same precedence to all the binary
Boolean operators (``a -> b & c`` is read as ``(a -> b) & c``), and a lower
one to G, F and X (``G a -> b`` is read as ``G (a -> b)``). Formulas are
therefore parsed here with the standard LTL precedence and printed fully
parenthesized before being handed to CHASE.

Precedence, from the loosest to the tightest binding::

    <->    ->    |    &    U R W    ! G F X    relations    + -    * /

``->`` is right-associative, the other binary operators are left-associative.
Primed variables (``x'``) denote the value of ``x`` in the next step.
``[]`` and ``<>`` are accepted as aliases of ``G`` and ``F``.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from typing import Union


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
    value: int


@dataclass(frozen=True)
class Unary:
    op: str            # '!', 'G', 'F', 'X', 'neg' (arithmetic minus)
    arg: "Node"


@dataclass(frozen=True)
class Binary:
    op: str            # '&', '|', '->', '<->', 'U', 'R', relations, arithmetic
    left: "Node"
    right: "Node"


Node = Union[Const, Var, Num, Unary, Binary]

RELATIONS = ("=", "!=", "<", "<=", ">", ">=")
ARITHMETIC = ("+", "-", "*", "/")
BOOLEAN_BINARY = ("&", "|", "->", "<->")
TEMPORAL_BINARY = ("U", "R")


# Tokenizer ------------------------------------------------------------------

_TOKEN_RE = re.compile(r"""
    \s*(?:
      (?P<num>\d+)
    | (?P<id>[A-Za-z_][A-Za-z0-9_]*)(?P<prime>')?
    | (?P<op><->|<=>|->|=>|==|!=|<=|>=|&&|\|\||/\\|\\/|\[\]|<>|[!~&|()<>=+\-*/])
    )""", re.VERBOSE)

_OP_ALIASES = {"<=>": "<->", "=>": "->", "==": "=", "&&": "&", "||": "|",
               "/\\": "&", "\\/": "|", "~": "!", "[]": "G", "<>": "F"}
_KEYWORDS = {"G", "F", "X", "U", "R", "W"}


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
    "<->": (1, False), "->": (2, True), "|": (3, False), "&": (4, False),
    "U": (5, True), "R": (5, True), "W": (5, True),
    "=": (7, False), "!=": (7, False), "<": (7, False), "<=": (7, False),
    ">": (7, False), ">=": (7, False),
    "+": (8, False), "-": (8, False), "*": (9, False), "/": (9, False),
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
            return Num(int(val))
        if kind == "const":
            return Const(val == "true")
        if kind == "id":
            return Var(val)
        if kind == "primed":
            return Var(val, primed=True)
        if kind == "op" and val == "(":
            node = self.expression(0)
            self.expect(")")
            return node
        if kind == "op" and val in ("!", "G", "F", "X"):
            return Unary(val, self.expression(_UNARY_PRECEDENCE))
        if kind == "op" and val == "-":
            arg = self.expression(_BINARY["*"][0] + 1)
            return Num(-arg.value) if isinstance(arg, Num) else Unary("neg", arg)
        raise LTLSyntaxError(f"unexpected {val or 'end of formula'!r} in {self.text!r}")


def parse(text: str) -> Node:
    """Parse a formula written in the standard LTL syntax."""
    if not isinstance(text, str) or not text.strip():
        raise LTLSyntaxError("empty formula")
    return _Parser(text).parse()


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
        return str(node.value)
    if isinstance(node, Unary):
        if node.op == "neg":
            return f"(0 - {to_chase(node.arg)})"
        # In the CHASE grammar G, F and X bind more loosely than the binary
        # Boolean operators (G a -> b is G (a -> b)): always parenthesize.
        return f"({node.op} ({to_chase(node.arg)}))"
    op = node.op
    return f"({to_chase(node.left)} {op} {to_chase(node.right)})"


def to_smv(node: Node) -> str:
    """Formula in the LTL syntax of NuSMV and nuXmv."""
    if isinstance(node, Const):
        return "TRUE" if node.value else "FALSE"
    if isinstance(node, Var):
        return f"next({node.name})" if node.primed else node.name
    if isinstance(node, Num):
        return str(node.value)
    if isinstance(node, Unary):
        if node.op == "neg":
            return f"(-{to_smv(node.arg)})"
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
            return f"-{_pretty(node.arg, 10)}"
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
    if name in ("IntegerValue", "RealValue"):
        return Num(int(obj.getValue()))
    if name == "BooleanValue":
        return Const(bool(obj.getValue()))
    if name == "UnaryBooleanFormula":
        return Unary("!", from_pychase(obj.getOp1()))
    if name == "UnaryTemporalFormula":
        op = {R.TemporalOperator.op_globally: "G", R.TemporalOperator.op_future: "F",
              R.TemporalOperator.op_next: "X"}[obj.getOp()]
        return Unary(op, from_pychase(obj.getFormula()))
    if name == "BinaryTemporalFormula":
        op = {R.TemporalOperator.op_until: "U", R.TemporalOperator.op_release: "R"}[obj.getOp()]
        return Binary(op, from_pychase(obj.getFormula1()), from_pychase(obj.getFormula2()))
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
    raise TypeError(f"unsupported CHASE formula node: {name}")


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
