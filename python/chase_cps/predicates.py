"""Predicate abstraction of the comparisons of real variables.

Used by ``chase_cps.reals`` when the SMT engine of nuXmv gives no answer
(nonlinear arithmetic, or no nuXmv). Each comparison with a real variable,
or with a number with a fraction, becomes a Boolean variable. Comparisons
are first written in a normal form, so that ``x <= 10`` and ``x > 10`` are
the same predicate ``p`` and ``!p``, and ``x' <= 10`` is ``p'``.

The abstraction allows more behaviors than the real variables do (``x > 10``
and ``x < 5`` both true), so a valid implication and an unsatisfiable
formula hold for the real variables, and the other answers may come from
the abstraction. With z3 (``pip install chase-cps[z3]``), two steps
remove most of these:

1. Before the check, the combinations of truth values of one or two
   predicates on a common variable that no real values satisfy are
   excluded: ``G !(p & q)`` for ``x > 10`` and ``x < 5``.
2. After the check, z3 looks for real values for every state of the trace.
   If it finds them, the trace is one of the real variables and the answer
   is exact. If a state (or two consecutive states) has none, the
   predicates of an unsat core are excluded and the check is repeated.

The answers that may come from the abstraction are returned as unknown,
with the trace of the abstraction.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from fractions import Fraction
from typing import Optional

from . import ltl
from .queries import UNKNOWN, UNSAT, VALID, Implies, Result, Satisfiable, Trace

try:
    import z3
except ImportError:      # optional: pip install chase-cps[z3]
    z3 = None

PREDICATES = "predicates"
ROUNDS = 10              # refinements of the abstraction after a spurious trace
Z3_TIMEOUT_MS = 2000     # for each query to z3


# Normal form of comparisons -------------------------------------------------

class _Opaque(Exception):
    pass


def _poly(node: ltl.Node) -> dict:
    """The expression as a polynomial {monomial: coefficient}; a monomial is
    a sorted tuple of factors (name, primed); a division by an expression
    with variables is a factor of its own, ("/", text)."""
    if isinstance(node, ltl.Num):
        return {(): Fraction(node.value)}
    if isinstance(node, ltl.Var):
        return {((node.name, node.primed),): Fraction(1)}
    if isinstance(node, ltl.Unary) and node.op == "neg":
        return {m: -c for m, c in _poly(node.arg).items()}
    if isinstance(node, ltl.Binary) and node.op in ("+", "-"):
        out = dict(_poly(node.left))
        sign = 1 if node.op == "+" else -1
        for m, c in _poly(node.right).items():
            out[m] = out.get(m, 0) + sign * c
        return {m: c for m, c in out.items() if c != 0}
    if isinstance(node, ltl.Binary) and node.op == "*":
        out: dict = {}
        for m1, c1 in _poly(node.left).items():
            for m2, c2 in _poly(node.right).items():
                m = tuple(sorted(m1 + m2))
                out[m] = out.get(m, 0) + c1 * c2
        return {m: c for m, c in out.items() if c != 0}
    if isinstance(node, ltl.Binary) and node.op == "/":
        divisor = _poly(node.right)
        if set(divisor) == {()}:
            d = divisor[()]
            if d == 0:
                raise _Opaque
            return {m: c / d for m, c in _poly(node.left).items()}
        return {(("/", ltl.to_str(node)),): Fraction(1)}
    raise _Opaque


@dataclass(frozen=True)
class Atom:
    """A comparison in normal form: ``poly op 0`` with op in <, <=, =."""
    op: str
    poly: tuple            # sorted ((monomial, coefficient), ...)


def _normalize(rel: ltl.Binary) -> Optional[tuple]:
    """(atom, negated, shifted) for a relation: the relation is ``atom`` (or
    its negation), taken at the next step when ``shifted`` (all its
    variables are primed). None when it is not a polynomial or has no
    variable."""
    try:
        e = _poly(ltl.Binary("-", rel.left, rel.right))
    except _Opaque:
        return None
    op, negated = rel.op, False
    if op == ">":
        op, negated = "<=", True
    elif op == ">=":
        op, negated = "<", True
    elif op == "!=":
        op, negated = "=", True
    monomials = sorted(m for m in e if m)
    if not monomials:
        return None
    factors = {f for m in monomials for f in m}
    shifted = all(f[0] != "/" and f[1] for f in factors)
    if shifted:
        e = {tuple(sorted((n, False) for n, _ in m)): c for m, c in e.items()}
        monomials = sorted(m for m in e if m)
    lead = e[monomials[0]]
    if lead < 0:
        e = {m: -c for m, c in e.items()}
        if op != "=":         # e < 0 is !(-e <= 0), e <= 0 is !(-e < 0)
            op, negated = ("<=" if op == "<" else "<"), not negated
    scale = abs(lead)
    poly = tuple(sorted((m, c / scale) for m, c in e.items()))
    return Atom(op, poly), negated, shifted


# The abstraction ---------------------------------------------------------------

_COMPLEMENT = {">": "<=", ">=": "<", "<": ">=", "<=": ">", "=": "!=", "!=": "="}


def _primed(node: ltl.Node) -> bool:
    if isinstance(node, ltl.Var):
        return node.primed
    if isinstance(node, ltl.Unary):
        return _primed(node.arg)
    if isinstance(node, ltl.Binary):
        return _primed(node.left) or _primed(node.right)
    return False


def _unshift(node: ltl.Node) -> ltl.Node:
    if isinstance(node, ltl.Var):
        return ltl.Var(node.name)
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, _unshift(node.arg))
    if isinstance(node, ltl.Binary):
        return ltl.Binary(node.op, _unshift(node.left), _unshift(node.right))
    return node


@dataclass
class Abstraction:
    variables: dict                                 # of the query
    atoms: dict = field(default_factory=dict)       # Atom -> predicate name
    nodes: dict = field(default_factory=dict)       # predicate name -> its meaning
    problem: Optional[str] = None

    def abstracted(self, rel: ltl.Binary) -> bool:
        reals = {v for v, t in self.variables.items() if t == "real"}
        return bool(ltl.variables(rel) & reals) or ltl.has_fraction(rel)

    def rewrite(self, node: ltl.Node) -> ltl.Node:
        from .reals import _map_relations

        def fn(rel):
            if not self.abstracted(rel):
                return rel
            found = _normalize(rel)
            if found is None:
                self.problem = f"{ltl.to_str(rel)} is not a polynomial comparison"
                return rel
            atom, negated, shifted = found
            if atom not in self.atoms:
                name = f"chase_p{len(self.atoms) + 1}"
                self.atoms[atom] = name
                # the predicate is the atom: the relation, or its complement
                meaning = _unshift(rel) if shifted else rel
                if negated:
                    meaning = ltl.Binary(_COMPLEMENT[meaning.op], meaning.left, meaning.right)
                self.nodes[name] = meaning
            p = ltl.Var(self.atoms[atom], primed=shifted)
            return ltl.Unary("!", p) if negated else p
        return _map_relations(node, fn)

    def predicate_variables(self) -> dict:
        return {name: "bool" for name in self.atoms.values()}

    def step_predicates(self) -> list[str]:
        """The predicates on one step (no primed variable in them)."""
        return [n for n, rel in self.nodes.items() if not _primed(rel)]


# z3 ------------------------------------------------------------------------------

class _Z3:
    """The relations of the predicates as z3 formulas, over the values of the
    variables at given steps."""

    def __init__(self, abstraction: Abstraction):
        self.a = abstraction
        self.vars: dict = {}

    def var(self, name: str, step):
        key = (name, step)
        if key not in self.vars:
            t = self.a.variables.get(name, "real")
            self.vars[key] = z3.Real(f"{name}@{step}") if t == "real" \
                else z3.Int(f"{name}@{step}")
        return self.vars[key]

    def bounds(self) -> list:
        out = []
        for (name, _), v in self.vars.items():
            t = self.a.variables.get(name)
            if isinstance(t, tuple):
                out += [v >= t[0], v <= t[1]]
        return out

    def term(self, node: ltl.Node, step, nxt):
        if isinstance(node, ltl.Num):
            f = Fraction(node.value)
            return z3.Q(f.numerator, f.denominator)
        if isinstance(node, ltl.Var):
            return self.var(node.name, nxt if node.primed else step)
        if isinstance(node, ltl.Unary) and node.op == "neg":
            return -self.term(node.arg, step, nxt)
        if isinstance(node, ltl.Unary) and node.op == "!":
            return z3.Not(self.term(node.arg, step, nxt))
        if isinstance(node, ltl.Binary):
            a, b = self.term(node.left, step, nxt), self.term(node.right, step, nxt)
            ops = {"+": lambda: a + b, "-": lambda: a - b, "*": lambda: a * b,
                   "/": lambda: a / b, "=": lambda: a == b, "!=": lambda: a != b,
                   "<": lambda: a < b, "<=": lambda: a <= b, ">": lambda: a > b,
                   ">=": lambda: a >= b}
            return ops[node.op]()
        raise TypeError(f"not an arithmetic term: {ltl.to_str(node)}")

    def predicate(self, name: str, step, nxt):
        return self.term(self.a.nodes[name], step, nxt)

    def solver(self):
        s = z3.Solver()
        s.set("timeout", Z3_TIMEOUT_MS)
        return s


def _literal(name: str, value: bool, primed: bool = False) -> ltl.Node:
    v = ltl.Var(name, primed=primed)
    return v if value else ltl.Unary("!", v)


def _exclude(literals: list[ltl.Node]) -> ltl.Node:
    return ltl.Unary("G", ltl.Unary("!", ltl.conjunction(literals)))


def relations(a: Abstraction) -> list[ltl.Node]:
    """Step 1: G !(...) for the combinations of one or two predicates on a
    common variable that no values satisfy at one step."""
    if z3 is None:
        return []
    zz = _Z3(a)
    names = a.step_predicates()
    out, constant = [], set()

    def unsat(constraints) -> bool:
        s = zz.solver()
        s.add(*constraints, *zz.bounds())
        return s.check() == z3.unsat
    for n in names:
        f = zz.predicate(n, 0, 1)
        for value in (True, False):
            if unsat([f if value else z3.Not(f)]):
                out.append(_exclude([_literal(n, value)]))
                constant.add(n)
    for i, n in enumerate(names):
        for m in names[i + 1:]:
            if n in constant or m in constant:
                continue
            if not ltl.variables(a.nodes[n]) & ltl.variables(a.nodes[m]):
                continue
            fn, fm = zz.predicate(n, 0, 1), zz.predicate(m, 0, 1)
            for vn in (True, False):
                for vm in (True, False):
                    if unsat([fn if vn else z3.Not(fn), fm if vm else z3.Not(fm)]):
                        out.append(_exclude([_literal(n, vn), _literal(m, vm)]))
    return out


def _value(v):
    if z3.is_int_value(v):
        return v.as_long()
    if z3.is_rational_value(v):
        return ltl.number(Fraction(v.numerator_as_long(), v.denominator_as_long()))
    if z3.is_algebraic_value(v):        # an irrational root: a close rational
        return ltl.number(v.approx(20).as_fraction())
    return str(v)


def _fixed_integers(a: Abstraction, zz: _Z3, state: Optional[dict], step) -> list:
    """The integers of a trace state keep their value."""
    if state is None:
        return []
    return [zz.var(v, step) == state[v] for v, t in a.variables.items()
            if isinstance(t, tuple) and v in state and isinstance(state[v], int)
            and not isinstance(state[v], bool)]


def concretize(a: Abstraction, trace: Trace):
    """Step 2: (trace of the real variables, None) when z3 finds values for
    every state; else (None, constraint excluding the predicates of a core),
    or (None, None) when it cannot tell."""
    states = trace.prefix + trace.loop
    n = len(states)
    if n == 0:
        return None, None

    def nxt(i):
        if i + 1 < n:
            return i + 1
        return len(trace.prefix) if trace.loop else n      # n: a free last state
    zz = _Z3(a)
    constraints = []
    for i, s in enumerate(states):
        for name in a.nodes:
            if name in s:
                f = zz.predicate(name, i, nxt(i))
                constraints.append(f if s[name] else z3.Not(f))
        constraints += _fixed_integers(a, zz, s, i)
    solver = zz.solver()
    solver.add(*constraints, *zz.bounds())
    answer = solver.check()
    if answer == z3.sat:
        model = solver.model()
        out = []
        for i, s in enumerate(states):
            state = {k: v for k, v in s.items() if k not in a.nodes}
            for v, t in a.variables.items():
                if t == "real" and (v, i) in zz.vars:
                    state[v] = _value(model.eval(zz.vars[(v, i)], model_completion=True))
            out.append(state)
        p = len(trace.prefix)
        return Trace(out[:p], out[p:]), None
    if answer != z3.unsat:
        return None, None
    # A state, then two consecutive states, without values: exclude a core.
    for s in states:
        core = _core(a, s, None)
        if core:
            return None, _exclude(core)
    for i, s in enumerate(states):
        j = nxt(i)
        if j < n:
            core = _core(a, s, states[j])
            if core:
                return None, _exclude(core)
    return None, None


def _core(a: Abstraction, s: dict, t: Optional[dict]) -> list[ltl.Node]:
    """The literals of an unsat core of the predicates in state s (and in
    the next state t), as LTL literals (those of t primed)."""
    zz = _Z3(a)
    solver = zz.solver()
    marks = {}
    for name, rel in a.nodes.items():
        two_steps = _primed(rel)
        if two_steps and t is None:
            continue
        if name in s:
            f = zz.predicate(name, 0, 1)
            m = z3.Bool(f"now_{name}")
            solver.add(z3.Implies(m, f if s[name] else z3.Not(f)))
            marks[m] = _literal(name, s[name])
        if t is not None and not two_steps and name in t:
            f = zz.predicate(name, 1, 2)
            m = z3.Bool(f"next_{name}")
            solver.add(z3.Implies(m, f if t[name] else z3.Not(f)))
            marks[m] = _literal(name, t[name], primed=True)
    solver.add(*_fixed_integers(a, zz, s, 0), *_fixed_integers(a, zz, t, 1), *zz.bounds())
    if not marks or solver.check(*marks) != z3.unsat:
        return []
    core = solver.unsat_core()
    return [lit for m, lit in marks.items() if any(m.eq(c) for c in core)]


def nonlinear(query) -> bool:
    """Whether a comparison of the query multiplies or divides variables."""
    from .reals import _formulas, _relations
    for f in _formulas(query):
        for rel in _relations(f):
            try:
                e = _poly(ltl.Binary("-", rel.left, rel.right))
            except _Opaque:
                return True
            if any(len(m) > 1 or any(x[0] == "/" for x in m) for m in e):
                return True
    return False


def _readable(a: Abstraction, trace: Optional[Trace]) -> Optional[Trace]:
    """The trace of the abstraction with the predicates written out."""
    if trace is None:
        return None

    def state(s):
        return {ltl.to_str(a.nodes[k]) if k in a.nodes else k: v for k, v in s.items()}
    return Trace([state(s) for s in trace.prefix], [state(s) for s in trace.loop])


# The check -----------------------------------------------------------------------

def check(query, backend: str = "auto", **options) -> Optional[Result]:
    """The answer of the abstraction (see the module), or None when the
    query cannot be abstracted."""
    from .backends import dispatch
    a = Abstraction(dict(query.variables))
    phi = a.rewrite(query.phi)
    psi = a.rewrite(query.psi) if isinstance(query, Implies) else None
    if a.problem:
        return None
    variables = {v: t for v, t in query.variables.items() if t != "real"}
    variables.update(a.predicate_variables())
    excluded = relations(a)
    hint = "" if z3 is not None else "; install z3 (pip install chase-cps[z3]) to check it"
    r = None
    for _ in range(ROUNDS):
        premise = ltl.conjunction(excluded + [phi])
        q = Implies(premise, psi, variables) if psi is not None \
            else Satisfiable(premise, variables)
        r = dispatch(q, backend, **options)
        r.reals = PREDICATES
        if r.status in (VALID, UNSAT, UNKNOWN):
            return r
        # invalid or sat: the trace may come from the abstraction
        if r.trace is None or z3 is None:
            return Result(UNKNOWN, r.backend, trace=_readable(a, r.trace), reals=PREDICATES,
                          seconds=r.seconds, reason="the trace of the predicate abstraction "
                          f"may have no real values{hint}")
        real, constraint = concretize(a, r.trace)
        if real is not None:
            r.trace = real
            return r
        if constraint is None:
            return Result(UNKNOWN, r.backend, trace=_readable(a, r.trace), reals=PREDICATES,
                          seconds=r.seconds, reason="z3 found no real values for the trace "
                          "of the predicate abstraction, and no core to exclude")
        excluded.append(constraint)
    return Result(UNKNOWN, r.backend, trace=_readable(a, r.trace), reals=PREDICATES,
                  seconds=r.seconds,
                  reason=f"no answer after {ROUNDS} refinements of the predicate abstraction")
