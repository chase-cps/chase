"""Repair: the values of some constants of the contracts that make a check
pass, at the least cost.

    from chase_cps import repair
    r = repair("refines", composed, system, parameters={"lam": (0, 10)})
    r.status, r.values, r.cost      # repaired, {"lam": 4.05}, 0.05

The check and its arguments are those of the checks (see chase_cps.export:
verify, compat, consist, refines, impl, env). The parameters are constants
of the contracts (``constants=`` of a contract, ``constant`` in a
specification file), each with a range; the repair changes nothing else.
The cost is ``minimize`` or ``maximize`` (a term over the parameters), by
default the change: the sum of |p - p0| over the parameters, in their own
units.

The result is

- ``holds``: the check passes with the present values, which are kept: the
  repair is a verification;
- ``repaired``: the values, their cost, and the check run with them (it
  passes): ``optimal`` when no values of lower cost pass;
- ``impossible``: no values in the ranges pass, with the evidence (the
  counterexamples that no values avoid, or the empty projection);
- ``unknown``: with the reason (a timeout, the rounds spent, a back end
  without an answer).

Three methods, chosen by the formulas:

- ``projection`` (bounded formulas over the reals, z3): the values for which
  the check fails are computed at once, by eliminating the other variables;
  one optimization (chase_cps.repair.project).
- ``counterexamples`` (any check that fails with a counterexample: LTL with
  nuXmv, C with ESBMC, and the rest): optimize the cost under the
  conditions that the counterexamples found so far are no longer
  counterexamples, run the check with the values, and add its
  counterexample, until it passes (chase_cps.repair.instantiate). The
  conditions only remove values that do not repair, so the cost of each
  round is a lower bound: values that pass at that cost are optimal.
- ``enumeration`` (a check of satisfiability on formulas that z3 does not
  take, with integer parameters): the values in the order of their cost,
  until one passes.

Every repaired value is checked by the check itself, with its usual back
end: the optimization proposes, the check decides.
"""

from __future__ import annotations

import itertools
import time
from dataclasses import dataclass, field
from typing import Optional

from .. import ltl
from .problem import (CHECKS, EXISTENTIAL, UNIVERSAL, Parameter, RepairError, instantiated,
                      json_value, parameters_of, questions, run, symbolic_args)

HOLDS, REPAIRED, IMPOSSIBLE, UNKNOWN = "holds", "repaired", "impossible", "unknown"
ROUNDS = 50
ENUMERATION = 10000      # values tried at most by enumeration


@dataclass
class Repair:
    """The result of ``repair``."""
    status: str
    values: dict = field(default_factory=dict)
    cost: object = None
    optimal: bool = False
    method: str = ""
    check: object = None             # the result of the check with the values
    rounds: int = 0
    counterexamples: list = field(default_factory=list)   # [(question, trace)]
    reason: str = ""
    seconds: float = 0.0

    def __bool__(self) -> bool:
        return self.status in (HOLDS, REPAIRED)

    def to_dict(self) -> dict:
        d = {"status": self.status, "method": self.method, "rounds": self.rounds,
             "seconds": round(self.seconds, 3)}
        if self.values:
            d["values"] = {k: json_value(v) for k, v in self.values.items()}
            d["text"] = {k: _text(v) for k, v in self.values.items()}   # as a file writes them
            d["cost"] = json_value(self.cost)
            d["optimal"] = self.optimal
        if self.check is not None and hasattr(self.check, "to_dict"):
            d["check"] = self.check.to_dict()
        if self.counterexamples:
            d["counterexamples"] = [{"question": q, "trace": t.to_dict()}
                                    for q, t in self.counterexamples]
        if self.reason:
            d["reason"] = self.reason
        return d


def _text(v):
    """The value as a specification file writes it, or None when it has no
    finite decimal expansion (1/3)."""
    return ltl.to_str(ltl.Num(v)) if ltl._decimal(v) else None


METHODS = ("auto", "projection", "counterexamples", "enumeration")


def repair(check: str, *args, parameters: dict, minimize=None, maximize=None,
           backend: str = "auto", method: str = "auto", rounds: int = ROUNDS,
           timeout: float = 300, **options) -> Repair:
    """The values of the parameters, within their ranges, at the least cost,
    that make the check on the arguments pass. ``method`` forces one of the
    methods (``auto`` chooses by the formulas)."""
    start = time.time()
    if minimize is not None and maximize is not None:
        raise RepairError("give minimize or maximize, not both")
    params = parameters_of(check, args, parameters)
    sym = symbolic_args(check, args, params)
    asked = questions(check, sym)
    cost, aux = _cost(params, minimize, maximize)
    opts = {"timeout": timeout, **options}

    def done(r: Repair) -> Repair:
        r.seconds = time.time() - start
        if r.status == REPAIRED and all(r.values.get(p.name) == p.value
                                        for p in params.values()):
            r.status = HOLDS          # the check passes with the present values
        if maximize is not None and r.cost is not None:
            r.cost = ltl.number(-r.cost)          # the value of the term maximized
        return r

    if method not in METHODS:
        raise RepairError(f"method {method!r}: one of {', '.join(METHODS)}")
    from ..backends.z3 import _z3
    if _z3 is None:
        return done(Repair(UNKNOWN, reason="repair optimizes with z3, which is not installed: "
                                           "pip install chase-cps[z3]"))
    from .project import LIMIT, applicable, eliminated, regions
    if method in ("auto", "projection"):
        why = applicable(asked) or ("an integer parameter" if any(
            p.integer for p in params.values()) else "")
        if not why and method == "auto" and eliminated(asked, params) > LIMIT:
            why = f"more than {LIMIT} variables to eliminate"     # by counterexamples, faster
        if not why and check in UNIVERSAL:
            hard, why = regions(asked, params, timeout)
            if hard is not None:
                return done(_by_projection(check, args, params, cost, aux, hard, backend,
                                           opts, timeout))
        if method == "projection":
            return done(Repair(UNKNOWN, method="projection", reason=why))
    if check in EXISTENTIAL and method in ("auto", "projection") and not applicable(asked):
        return done(_by_witnesses(check, args, asked, params, cost, aux, backend, opts, timeout))
    if method == "enumeration" or (method == "auto" and check in EXISTENTIAL):
        return done(_by_enumeration(check, args, params, cost, aux, backend, opts))
    if check in EXISTENTIAL:
        return done(Repair(UNKNOWN, method="counterexamples",
                           reason=f"{check} asks for a behavior: it has no counterexamples"))
    return done(_by_counterexamples(check, args, asked, params, cost, aux, backend, opts,
                                    rounds, timeout))


# the cost --------------------------------------------------------------------

def _cost(params: dict, minimize, maximize):
    """(the term to minimize, its auxiliary constraints): the change by
    default, with chase_dev_<p> >= |p - p0|."""
    if maximize is not None:
        term = ltl.parse(maximize) if isinstance(maximize, str) else maximize
        return ltl.Unary("neg", term), []
    if minimize is not None:
        return (ltl.parse(minimize) if isinstance(minimize, str) else minimize), []
    terms, aux = [], []
    for p in params.values():
        d = ltl.Var(f"chase_dev_{p.name}")
        diff = ltl.Binary("-", ltl.Var(p.name), ltl.Num(p.value))
        aux.append(ltl.Binary(">=", d, diff))
        aux.append(ltl.Binary(">=", d, ltl.Unary("neg", diff)))
        terms.append(d)
    term = terms[0]
    for t in terms[1:]:
        term = ltl.Binary("+", term, t)
    return term, aux


def _variables(params: dict, aux: list) -> dict:
    v = {p.name: p.type for p in params.values()}
    for f in aux:
        for name in ltl.variables(f):
            v.setdefault(name, "real")
    return v


def _best(params: dict, cost, aux: list, conditions: list, extra: list, timeout: float,
          types: dict | None = None):
    """The least cost under the ranges, the conditions (formulas over the
    parameters, and over the variables of ``types``), and extra z3
    constraints: (Optimum, values)."""
    from ..backends import z3 as backend_z3
    from ..optimize import certified
    z3 = backend_z3._z3
    variables = {**(types or {}), **_variables(params, aux)}
    e = backend_z3.Encoding(variables, rigid=params)
    hard = [e.formula(p.range()) for p in params.values()]
    hard += [e.formula(f) for f in aux + conditions] + list(extra)
    goal = e.formula(cost)
    if z3.is_int(goal):
        goal = z3.ToReal(goal)
    hard += e.bounds()
    o = certified(goal, hard, e.trace, False, timeout)
    values = {}
    if o.trace is not None:
        state = o.trace.loop[0]
        values = {p: state[p] for p in params if p in state}
    if o.status == "not_attained":
        # the least cost is a limit: values within 1e-6 of it
        from fractions import Fraction
        s = z3.Solver()
        s.set("timeout", int(timeout * 1000))
        s.add(*hard, goal <= z3.RealVal(str(Fraction(o.value) + Fraction(1, 10**6))))
        if s.check() == z3.sat:
            state = e.trace(s.model()).loop[0]
            values = {p: state[p] for p in params if p in state}
    for p in params.values():                     # parameters no formula reads
        values.setdefault(p.name, p.value)
    return o, values


def _cost_of(cost, aux, values: dict, params: dict):
    """The cost of the values (the auxiliary variables at their least)."""
    from fractions import Fraction
    from ..contract import substitute
    if aux:                                       # the change: sum of |p - p0|
        return ltl.number(sum(abs(Fraction(values[p.name]) - Fraction(p.value))
                              for p in params.values()))
    return _evaluate(substitute(cost, values))


def _evaluate(node):
    from fractions import Fraction
    if isinstance(node, ltl.Num):
        return node.value
    if isinstance(node, ltl.Unary) and node.op == "neg":
        return ltl.number(-Fraction(_evaluate(node.arg)))
    a, b = Fraction(_evaluate(node.left)), Fraction(_evaluate(node.right))
    return ltl.number({"+": a + b, "-": a - b, "*": a * b, "/": a / b}[node.op])


# the methods ------------------------------------------------------------------

def _by_projection(check, args, params, cost, aux, hard, backend, opts, timeout) -> Repair:
    o, values = _best(params, cost, aux, [], hard, timeout)
    if o.status == "infeasible":
        return Repair(IMPOSSIBLE, method="projection",
                      reason="no values in the ranges pass: the projection is empty")
    if o.status not in ("optimal", "not_attained"):
        return Repair(UNKNOWN, method="projection", reason=f"the optimization: {o.status} "
                      f"{o.reason}".strip())
    out = run(check, instantiated(args, values), backend, opts)
    if out.passed:
        return Repair(REPAIRED, values, _cost_of(cost, aux, values, params),
                      o.status == "optimal", "projection", out.result, 1)
    return Repair(UNKNOWN, values, method="projection", check=out.result, rounds=1,
                  reason="the check does not pass with the values of the projection "
                         f"({out.reason or 'it failed'}): the projection and the check disagree")


def _by_witnesses(check, args, asked, params, cost, aux, backend, opts, timeout) -> Repair:
    """A check of satisfiability on bounded formulas: the parameters and a
    behavior for each question, at once (the variables of the question i are
    renamed v__i, the parameters are shared)."""
    conditions, types = [], {}
    for i, (name, q) in enumerate(asked):
        mapping = {v: f"{v}__{i}" for v in q.variables if v not in params}
        conditions.append(_rename(q.phi, mapping))
        types.update({mapping[v]: t for v, t in q.variables.items() if v in mapping})
    o, values = _best(params, cost, aux, conditions, [], timeout, types)
    if o.status == "infeasible":
        return Repair(IMPOSSIBLE, method="witnesses",
                      reason="no values in the ranges give the behaviors that the check needs")
    if o.status not in ("optimal", "not_attained"):
        return Repair(UNKNOWN, method="witnesses", reason=f"the optimization: {o.status} "
                      f"{o.reason}".strip())
    out = run(check, instantiated(args, values), backend, opts)
    if out.passed:
        return Repair(REPAIRED, values, _cost_of(cost, aux, values, params),
                      o.status == "optimal", "witnesses", out.result, 1)
    return Repair(UNKNOWN, values, method="witnesses", check=out.result, rounds=1,
                  reason="the check does not pass with the values found "
                         f"({out.reason or 'it failed'})")


def _rename(node, mapping: dict):
    if isinstance(node, ltl.Var) and node.name in mapping:
        return ltl.Var(mapping[node.name], node.primed)
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, _rename(node.arg, mapping))
    if isinstance(node, ltl.Binary):
        return ltl.Binary(node.op, _rename(node.left, mapping), _rename(node.right, mapping))
    return node


def _by_counterexamples(check, args, asked, params, cost, aux, backend, opts, rounds,
                        timeout) -> Repair:
    from .instantiate import on_trace
    from ..queries import Holds, Implies
    from .project import generalized
    from ..cprograms import CFunction
    by_name = dict(asked)
    # A counterexample is generalized (its reals free) only when the premise of
    # the question describes every behavior: not for a C function, whose
    # behaviors are those of its code, not of the formulas.
    general = not (check == "impl" and isinstance(args[0], CFunction))
    conditions, extra, found = [], [], []
    for k in range(1, rounds + 1):
        o, values = _best(params, cost, aux, conditions, extra, timeout)
        if o.status == "infeasible":
            return Repair(IMPOSSIBLE, method="counterexamples", rounds=k - 1,
                          counterexamples=found,
                          reason="no values in the ranges avoid all the counterexamples")
        if o.status not in ("optimal", "not_attained"):
            return Repair(UNKNOWN, method="counterexamples", rounds=k - 1,
                          counterexamples=found,
                          reason=f"the optimization: {o.status} {o.reason}".strip())
        out = run(check, instantiated(args, values), backend, opts)
        if out.passed:
            return Repair(REPAIRED, values, _cost_of(cost, aux, values, params),
                          o.status == "optimal", "counterexamples", out.result, k, found)
        if out.passed is None or not out.counterexamples:
            return Repair(UNKNOWN, values, method="counterexamples", check=out.result,
                          rounds=k, counterexamples=found,
                          reason=out.reason or "the check failed without a counterexample")
        for name, trace in out.counterexamples:
            q = by_name[name]
            found.append((name, trace))
            # the values of the parameters under which this behavior, its reals
            # free, still breaks the question (bounded formulas), else under
            # which the trace itself still does
            piece = generalized(q, trace, params, values, timeout) \
                if general and isinstance(q, Implies) else None
            if piece is not None and _z3_holds_at(piece, values, params):
                extra.append(_z3_not(piece))
                continue
            spec = ltl.Binary("->", q.phi, q.psi) if isinstance(q, Implies) else q.phi
            condition = on_trace(spec, trace, params)
            if _holds_at(condition, values):
                raise RepairError(f"internal: the counterexample of {name} is not one with "
                                  f"the values {values}")
            conditions.append(condition)
    return Repair(UNKNOWN, method="counterexamples", rounds=rounds, counterexamples=found,
                  reason=f"no repair after {rounds} rounds")


def _z3_holds_at(f, values: dict, params: dict) -> bool:
    """Whether the z3 formula over the parameters holds at the values."""
    from ..backends import z3 as backend_z3
    z3 = backend_z3._z3
    e = backend_z3.Encoding({p.name: p.type for p in params.values()})
    subs = [(e.var(p), z3.RealVal(str(values[p])) if z3.is_real(e.var(p))
             else z3.IntVal(int(values[p]))) for p in params if p in values]
    r = z3.simplify(z3.substitute(f, *subs)) if subs else z3.simplify(f)
    return z3.is_true(r)


def _z3_not(f):
    from ..backends import z3 as backend_z3
    return backend_z3._z3.Not(f)


def _holds_at(condition, values) -> bool:
    from ..contract import substitute
    from ..queries import Trace
    from ..robustness import holds
    return holds(substitute(condition, values), Trace(prefix=[], loop=[{}]))


def _by_enumeration(check, args, params, cost, aux, backend, opts) -> Repair:
    ps = list(params.values())
    if not all(p.integer for p in ps):
        return Repair(UNKNOWN, method="enumeration",
                      reason="a check of satisfiability on formulas that z3 does not take "
                             "is repaired with integer parameters only")
    sizes = [p.high - p.low + 1 for p in ps]
    total = 1
    for n in sizes:
        total *= n
    if total > ENUMERATION:
        return Repair(UNKNOWN, method="enumeration",
                      reason=f"{total} values to try: more than {ENUMERATION}")
    candidates = [dict(zip([p.name for p in ps], vs))
                  for vs in itertools.product(*[range(p.low, p.high + 1) for p in ps])]
    candidates.sort(key=lambda v: _cost_of(cost, aux, v, params))
    for k, values in enumerate(candidates, 1):
        out = run(check, instantiated(args, values), backend, opts)
        if out.passed:
            return Repair(REPAIRED, values, _cost_of(cost, aux, values, params), True,
                          "enumeration", out.result, k)
        if out.passed is None:
            return Repair(UNKNOWN, method="enumeration", rounds=k, check=out.result,
                          reason=out.reason or "the check gave no answer")
    return Repair(IMPOSSIBLE, method="enumeration", rounds=total,
                  reason="no values in the ranges pass")


__all__ = ["repair", "Repair", "RepairError", "Parameter", "HOLDS", "REPAIRED", "IMPOSSIBLE",
           "UNKNOWN"]
