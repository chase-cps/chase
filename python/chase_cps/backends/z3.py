"""z3: the formulas over a bounded number of steps.

A formula with no G, F, U, R reads a bounded number of steps: X, x', and
past(x, n) move by one step or by n, and the intervals of steps (G[0,5] p,
F[1,3] p, p U[0,2] q) are written with X. A formula without them (a static
contract) reads one step. Satisfiability and validity of such a formula are
questions of arithmetic over the values of the variables at each step,
which z3 answers exactly for linear arithmetic over the integers and the
reals (and often for nonlinear arithmetic). Booleans are z3 Booleans,
bounded integers z3 integers with their bounds, reals z3 reals; x at step k
is the variable x@k (x at step 0). Words are left to nuXmv.

The trace of a result has the steps that the formulas read, the last one
repeated; past(x, n) before step 0 is in the history variables of nuXmv
(chase_past1_x, ...). ``to_smt`` writes the same question in SMT-LIB 2.
"""

from __future__ import annotations

import time
from fractions import Fraction

from .. import ltl
from ..queries import INVALID, SAT, UNKNOWN, UNSAT, VALID, Implies, Result, Satisfiable, Trace
from .base import Adapter

try:
    import z3 as _z3
except ImportError:                  # optional: pip install chase-cps[z3]
    _z3 = None

TEMPORAL = ("G", "F", "X", "U", "R")
UNBOUNDED = ("G", "F", "U", "R")


def bounded(node: ltl.Node) -> bool:
    """Whether a formula reads a bounded number of steps: no G, F, U, R."""
    if isinstance(node, ltl.Unary):
        return node.op not in UNBOUNDED and bounded(node.arg)
    if isinstance(node, ltl.Binary):
        return node.op not in UNBOUNDED and bounded(node.left) and bounded(node.right)
    return True


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
        return "z3 is not installed: pip install chase-cps[z3]"
    if not all(bounded(f) for f in formulas(query)):
        return "z3 answers formulas over a bounded number of steps: no G, F, U, R " \
            "(intervals such as G[0,5] p are fine)"
    words = sorted(v for v, t in query.variables.items() if word_type(t))
    if words:
        return f"z3 does not take words here ({', '.join(words)}): use nuXmv"
    return ""


class Encoding:
    """The variables of a query at each step as z3 terms, and formulas as z3
    formulas."""

    def __init__(self, variables: dict):
        self.types = variables
        self.vars: dict = {}           # (name, step) -> z3 variable
        self.pasts: dict = {}          # name -> the largest n of past(name, n)

    def var(self, name: str, step: int = 0):
        if (name, step) not in self.vars:
            t = self.types.get(name, "bool")
            label = name if step == 0 else f"{name}@{step}"
            if t == "bool":
                v = _z3.Bool(label)
            elif t == "real":
                v = _z3.Real(label)
            else:
                v = _z3.Int(label)
            self.vars[(name, step)] = v
        return self.vars[(name, step)]

    def bounds(self) -> list:
        return [c for (name, _), v in self.vars.items() if isinstance(self.types.get(name), tuple)
                for c in (v >= self.types[name][0], v <= self.types[name][1])]

    def formula(self, node, step: int = 0):
        z = _z3
        if isinstance(node, ltl.Const):
            return z.BoolVal(node.value)
        if isinstance(node, ltl.Num):
            f = Fraction(node.value)
            return z.IntVal(f.numerator) if f.denominator == 1 else z.Q(f.numerator, f.denominator)
        if isinstance(node, ltl.Var):
            return self.var(node.name, step + 1 if node.primed else step)
        if isinstance(node, ltl.Unary):
            if node.op == "X":
                return self.formula(node.arg, step + 1)
            if ltl.function(node) == "past":
                n = ltl.extension(node)
                self.pasts[node.arg.name] = max(self.pasts.get(node.arg.name, 0), n)
                return self.var(node.arg.name, step - n)
            a = self.formula(node.arg, step)
            if node.op == "!":
                return z.Not(a)
            if node.op == "neg":
                return -a
            raise ValueError(f"z3: {ltl.to_str(node)} is not supported")
        a, b = self.formula(node.left, step), self.formula(node.right, step)
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

    def robustness(self, node, step: int = 0):
        """The robustness of a formula at a step, as a z3 term (see
        ``chase_cps.robustness``), for a formula whose atoms compare numbers:
        a Boolean has the robustness inf or -inf, which z3 has not."""
        z = _z3
        if isinstance(node, ltl.Unary) and node.op == "!":
            return -self.robustness(node.arg, step)
        if isinstance(node, ltl.Unary) and node.op == "X":
            return self.robustness(node.arg, step + 1)
        if isinstance(node, ltl.Binary) and node.op in ltl.RELATIONS:
            a, b = self.formula(node.left, step), self.formula(node.right, step)
            if not (z.is_arith(a) and z.is_arith(b)):
                raise ValueError(f"{ltl.to_str(node)} compares Booleans: its robustness is "
                                 "inf or -inf")
            a, b = _real(a), _real(b)
            if node.op in ("<", "<="):
                return b - a
            if node.op in (">", ">="):
                return a - b
            d = z.If(a >= b, a - b, b - a)
            return -d if node.op == "=" else d
        if isinstance(node, ltl.Binary) and node.op in ("&", "|", "->", "<->"):
            a, b = self.robustness(node.left, step), self.robustness(node.right, step)
            if node.op == "&":
                return _min(a, b)
            if node.op == "|":
                return _max(a, b)
            if node.op == "->":
                return _max(-a, b)
            return _min(_max(-a, b), _max(a, -b))
        raise ValueError(f"the robustness of {ltl.to_str(node)} is inf or -inf (a Boolean), "
                         "or not supported: use formulas whose atoms compare numbers")

    def trace(self, model) -> Trace:
        """The values at the steps that the formulas read: the steps from 0
        to the last, which repeats; values before step 0 (past) in the
        history variables."""
        names = sorted({n for n, _ in self.vars})
        last = max([0] + [k for _, k in self.vars])
        states = []
        for i in range(last + 1):
            state = {n: self._value(model, n, i) for n in names}
            for n, d in sorted(self.pasts.items()):
                for k in range(1, d + 1):
                    state[f"chase_past{k}_{n}"] = self._value(model, n, i - k)
            states.append(state)
        return Trace(prefix=states[:-1], loop=states[-1:])

    def _value(self, model, name: str, step: int):
        """The value of the model, or a value of the type for a step that no
        formula reads."""
        if (name, step) in self.vars:
            return _value(model.eval(self.vars[(name, step)], model_completion=True))
        t = self.types.get(name, "bool")
        if t == "bool":
            return False
        if isinstance(t, tuple):
            return t[0] if t[0] > 0 or t[1] < 0 else 0
        return 0


def _real(term):
    return _z3.ToReal(term) if _z3.is_int(term) else term


def _min(a, b):
    return _z3.If(a <= b, a, b)


def _max(a, b):
    return _z3.If(a >= b, a, b)


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
        return Result(INVALID if implies else SAT, self.name, trace=e.trace(s.model()),
                      seconds=seconds)
