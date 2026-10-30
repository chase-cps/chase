"""z3: the formulas without temporal operators (static contracts).

A formula with no G, F, X, U, R, no x' and no past(x, n) says something about
one step only, so satisfiability and validity are questions of arithmetic,
which z3 answers exactly for linear arithmetic over the integers and the
reals (and often for nonlinear arithmetic). Booleans are z3 Booleans,
bounded integers z3 integers with their bounds, reals z3 reals. Words are
left to nuXmv.

The trace of a result has one state, repeated: a static formula reads the
first step only. ``to_smtlib`` writes the same question in SMT-LIB 2.
"""

from __future__ import annotations

import time
from fractions import Fraction

from .. import ltl
from ..queries import INVALID, SAT, UNKNOWN, UNSAT, VALID, Implies, Result, Satisfiable, Trace
from .base import Adapter

try:
    import z3 as _z3
except ImportError:                  # optional: pip install chase-cps[reals]
    _z3 = None

TEMPORAL = ("G", "F", "X", "U", "R")


def static(node: ltl.Node) -> bool:
    """Whether a formula reads one step only: no temporal operator, no x',
    no past(x, n)."""
    if isinstance(node, ltl.Var):
        return not node.primed
    if isinstance(node, ltl.Unary):
        return node.op not in TEMPORAL and ltl.function(node) != "past" and static(node.arg)
    if isinstance(node, ltl.Binary):
        return node.op not in TEMPORAL and static(node.left) and static(node.right)
    return True


def formulas(query) -> list:
    if isinstance(query, Implies):
        return [query.phi, query.psi]
    if isinstance(query, Satisfiable):
        return [query.phi]
    return []


def why_not(query) -> str:
    """Why z3 cannot answer the query, or "" when it can."""
    from ..words import word_type
    if _z3 is None:
        return "z3 is not installed: pip install chase-cps[reals]"
    if not all(static(f) for f in formulas(query)):
        return "z3 answers formulas without temporal operators, x' or past(x, n)"
    words = sorted(v for v, t in query.variables.items() if word_type(t))
    if words:
        return f"z3 does not take words here ({', '.join(words)}): use nuXmv"
    return ""


class Encoding:
    """The variables of a query as z3 terms, and formulas as z3 formulas."""

    def __init__(self, variables: dict):
        self.types = variables
        self.vars: dict = {}

    def var(self, name: str):
        if name not in self.vars:
            t = self.types.get(name, "bool")
            if t == "bool":
                self.vars[name] = _z3.Bool(name)
            elif t == "real":
                self.vars[name] = _z3.Real(name)
            else:
                self.vars[name] = _z3.Int(name)
        return self.vars[name]

    def bounds(self) -> list:
        return [c for name, v in self.vars.items() if isinstance(self.types.get(name), tuple)
                for c in (v >= self.types[name][0], v <= self.types[name][1])]

    def formula(self, node):
        z = _z3
        if isinstance(node, ltl.Const):
            return z.BoolVal(node.value)
        if isinstance(node, ltl.Num):
            f = Fraction(node.value)
            return z.IntVal(f.numerator) if f.denominator == 1 else z.Q(f.numerator, f.denominator)
        if isinstance(node, ltl.Var):
            return self.var(node.name)
        if isinstance(node, ltl.Unary):
            a = self.formula(node.arg)
            if node.op == "!":
                return z.Not(a)
            if node.op == "neg":
                return -a
            raise ValueError(f"z3: {ltl.to_str(node)} is not supported")
        a, b = self.formula(node.left), self.formula(node.right)
        op = node.op
        if op == "&":
            return z.And(a, b)
        if op == "|":
            return z.Or(a, b)
        if op == "->":
            return z.Implies(a, b)
        if op == "<->":
            return a == b
        if op == "xor":
            return z.Xor(a, b)
        if op == "/" and _integer(a) and _integer(b):
            # as nuXmv: integer division toward 0 (z3 divides with a remainder
            # >= 0); a division of numbers >= 0 is the same in both
            return z.If(a >= 0, z.If(b > 0, a / b, -(a / -b)),
                        z.If(b > 0, -((-a) / b), (-a) / (-b)))
        ops = {"+": lambda: a + b, "-": lambda: a - b, "*": lambda: a * b, "/": lambda: a / b,
               "=": lambda: a == b, "!=": lambda: a != b,
               "<": lambda: a < b, "<=": lambda: a <= b, ">": lambda: a > b, ">=": lambda: a >= b}
        if op not in ops:
            raise ValueError(f"z3: the operator {op} is not supported")
        return ops[op]()

    def state(self, model) -> dict:
        out = {}
        for name, v in sorted(self.vars.items()):
            value = model.eval(v, model_completion=True)
            out[name] = _value(value)
        return out


def _integer(term) -> bool:
    return _z3.is_int(term)


def _value(v):
    z = _z3
    if z.is_true(v):
        return True
    if z.is_false(v):
        return False
    if z.is_int_value(v):
        return v.as_long()
    if z.is_rational_value(v):
        return ltl.number(Fraction(v.numerator_as_long(), v.denominator_as_long()))
    if z.is_algebraic_value(v):                 # an irrational number: a close fraction
        return ltl.number(Fraction(v.approx(20).as_fraction()))
    return str(v)


class Z3Adapter(Adapter):
    """Satisfiability and validity of static formulas with z3."""

    name = "z3"
    answers = (Implies, Satisfiable)
    timeout = 300.0

    def available(self) -> bool:
        return _z3 is not None

    def problem(self, query):
        """The encoding and the formula to check for satisfiability (phi & !psi
        for an implication)."""
        e = Encoding(query.variables)
        if isinstance(query, Implies):
            f = _z3.And(e.formula(query.phi), _z3.Not(e.formula(query.psi)))
        else:
            f = e.formula(query.phi)
        return e, f

    def emit(self, query) -> str:
        """The question in SMT-LIB 2: (check-sat) answers sat for a witness of
        a satisfiability query, or a counterexample of an implication."""
        reason = why_not(query)
        if reason:
            raise ValueError(reason)
        e, f = self.problem(query)
        s = _z3.Solver()
        s.add(f, *e.bounds())
        what = "phi & !psi: unsat means that phi -> psi is valid" \
            if isinstance(query, Implies) else "phi: sat means that phi is satisfiable"
        return f"; chase-cps: {what}\n" + s.to_smt2()

    def run(self, query) -> Result:
        reason = why_not(query)
        if reason:
            return Result(UNKNOWN, self.name, reason=reason)
        start = time.time()
        try:
            e, f = self.problem(query)
        except ValueError as err:
            return Result(UNKNOWN, self.name, reason=str(err))
        s = _z3.Solver()
        s.set("timeout", int(self.timeout * 1000))
        s.add(f, *e.bounds())
        answer = s.check()
        seconds = time.time() - start
        implies = isinstance(query, Implies)
        if answer == _z3.unknown:
            return Result(UNKNOWN, self.name, reason=f"z3: {s.reason_unknown()}", seconds=seconds)
        if answer == _z3.unsat:
            return Result(VALID if implies else UNSAT, self.name, seconds=seconds)
        trace = Trace(prefix=[], loop=[e.state(s.model())])
        return Result(INVALID if implies else SAT, self.name, trace=trace, seconds=seconds)
