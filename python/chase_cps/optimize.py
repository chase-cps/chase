"""The best value of a term under constraints without temporal operators:
``minimize("cost", contract)``, ``maximize("p", "p <= 2.5 * n & n <= 4",
variables)``. The constraints are a formula, a list of formulas, or a
contract (its assumptions and its guarantees: the behaviors where both
hold). The answer gives the value and the values of the variables that
reach it, for design-space exploration.

z3 answers (``pip install chase-cps[reals]``): exact for linear arithmetic
over the integers and the reals, with Booleans and any Boolean structure.
"""

from __future__ import annotations

import time
from dataclasses import dataclass, field
from typing import Optional

from . import ltl
from .contract import Contract
from .queries import Trace

OPTIMAL, NOT_ATTAINED, UNBOUNDED, INFEASIBLE, UNKNOWN = (
    "optimal", "not_attained", "unbounded", "infeasible", "unknown")


@dataclass
class Optimum:
    """The answer of ``minimize`` or ``maximize``.

    status is optimal (value is reached by the values in trace),
    not_attained (value is the bound, which a strict inequality keeps out;
    trace has values that satisfy the constraints), unbounded, infeasible (no values
    satisfy the constraints), or unknown with the reason.
    """
    status: str
    value: object = None
    trace: Optional[Trace] = None
    backend: str = ""
    reason: str = ""
    seconds: float = 0.0
    details: dict = field(default_factory=dict)

    def __bool__(self) -> bool:
        return self.status == OPTIMAL

    def to_dict(self) -> dict:
        from .queries import _json_value
        d = {"status": self.status, "backend": self.backend, "seconds": round(self.seconds, 3)}
        if self.value is not None:
            d["value"] = _json_value(self.value)
        if self.trace is not None:
            d["values"] = self.trace.to_dict()["loop"][0]
        if self.reason:
            d["reason"] = self.reason
        return d


def minimize(objective, subject_to, variables: Optional[dict] = None, timeout: float = 300,
             backend: str = "auto") -> Optimum:
    """The smallest value of a term (text or tree) where the constraints hold."""
    return _optimize(objective, subject_to, variables, timeout, backend, maximize=False)


def maximize(objective, subject_to, variables: Optional[dict] = None, timeout: float = 300,
             backend: str = "auto") -> Optimum:
    """The largest value of a term (text or tree) where the constraints hold."""
    return _optimize(objective, subject_to, variables, timeout, backend, maximize=True)


def _constraints(subject_to, variables):
    if isinstance(subject_to, Contract):
        c = subject_to
        return ltl.Binary("&", c.assumption, c.guarantee), {**c.variables, **(variables or {})}
    if isinstance(subject_to, (list, tuple)):
        nodes = [ltl.parse(f) if isinstance(f, str) else f for f in subject_to]
        return ltl.conjunction(nodes), dict(variables or {})
    node = ltl.parse(subject_to) if isinstance(subject_to, str) else subject_to
    return node, dict(variables or {})


def _optimize(objective, subject_to, variables, timeout, backend, maximize) -> Optimum:
    from .backends import z3 as backend_z3
    from .queries import Satisfiable
    if backend not in ("auto", "z3"):
        return Optimum(UNKNOWN, reason=f"{backend} does not optimize; z3 does")
    term = ltl.parse(objective) if isinstance(objective, str) else objective
    phi, variables = _constraints(subject_to, variables)
    reason = backend_z3.why_not(Satisfiable(ltl.Binary("&", phi, ltl.Binary("=", term, term)),
                                            variables))
    if reason:
        return Optimum(UNKNOWN, reason=reason)
    z3 = backend_z3._z3
    start = time.time()
    e = backend_z3.Encoding(variables)
    try:
        goal, f = e.formula(term), e.formula(phi)
    except ValueError as err:
        return Optimum(UNKNOWN, "z3", reason=str(err))
    opt = z3.Optimize()
    opt.set("timeout", int(timeout * 1000))
    opt.add(f, *e.bounds())
    handle = opt.maximize(goal) if maximize else opt.minimize(goal)
    answer = opt.check()
    seconds = time.time() - start
    if answer == z3.unsat:
        return Optimum(INFEASIBLE, backend="z3", seconds=seconds)
    if answer == z3.unknown:
        return Optimum(UNKNOWN, backend="z3", reason=f"z3: {opt.reason_unknown()}", seconds=seconds)
    trace = Trace(prefix=[], loop=[e.state(opt.model())])
    inf, value, eps = (handle.upper_values() if maximize else handle.lower_values())
    inf, eps = backend_z3._value(z3.simplify(inf)), backend_z3._value(z3.simplify(eps))
    if inf != 0:
        return Optimum(UNBOUNDED, backend="z3", trace=trace, seconds=seconds)
    value = backend_z3._value(z3.simplify(value))
    status = OPTIMAL if eps == 0 else NOT_ATTAINED
    return Optimum(status, value, trace, "z3", seconds=seconds)
