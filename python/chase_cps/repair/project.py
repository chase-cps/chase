"""Repair by projection, for checks whose formulas read a bounded number of
steps (static contracts, signal temporal logic in discrete time) over the
reals: the values of the parameters for which the check fails are computed
at once, by eliminating the other variables (quantifier elimination of z3).

For a question phi -> psi, the parameters p break it when some behavior x
has phi(x, p) and not psi(x, p): the set of such p is the projection of
phi & !psi on the parameters. For a question of satisfiability (verify),
the parameters keep it when some x has phi(x, p). The repair is then one
optimization of the cost over the parameters outside the first sets and
inside the second ones, checked by the solver (chase_cps.optimize).

When z3 cannot eliminate a variable (an integer, a nonlinear term), the
projection is not used: the repair goes by counterexamples.
"""

from __future__ import annotations

from .. import ltl
from ..queries import Implies, Satisfiable


def applicable(asked: list) -> str:
    """Why the projection cannot be used for the questions, or ""."""
    from ..backends.z3 import _z3, why_not
    if _z3 is None:
        return "z3 is not installed"
    for name, q in asked:
        if not isinstance(q, (Implies, Satisfiable)):
            return f"{name} is a question on a state machine"
        reason = why_not(q)
        if reason:
            return reason
    return ""


def regions(asked: list, params: dict, timeout: float):
    """The constraints on the parameters (z3 formulas) under which every
    question has the answer that the check needs, or the reason why they
    cannot be computed."""
    from ..backends import z3 as backend_z3
    z3 = backend_z3._z3
    out = []
    for name, q in asked:
        e = backend_z3.Encoding(q.variables, rigid=params)
        if isinstance(q, Implies):
            body = z3.And(e.formula(q.phi), z3.Not(e.formula(q.psi)), *e.bounds())
        else:
            body = z3.And(e.formula(q.phi), *e.bounds())
        others = [v for (n, _), v in e.vars.items() if n not in params]
        projected = _eliminate(z3, others, body, timeout)
        if projected is None:
            return None, f"z3 could not eliminate the variables of {name}"
        out.append(z3.Not(projected) if isinstance(q, Implies) else projected)
    return out, ""


def _eliminate(z3, xs, body, timeout):
    if not xs:
        return body
    goal = z3.Goal()
    goal.add(z3.Exists(xs, body))
    tactic = z3.TryFor(z3.Then("simplify", "qe", "simplify"), int(timeout * 1000))
    try:
        result = tactic(goal)
    except z3.Z3Exception:
        return None
    parts = [z3.And(*g) if len(g) else z3.BoolVal(True) for g in result]
    f = z3.Or(*parts) if len(parts) != 1 else parts[0]
    return None if _has_quantifier(z3, f) else f


def _has_quantifier(z3, f) -> bool:
    todo, seen = [f], set()
    while todo:
        x = todo.pop()
        if x.get_id() in seen:
            continue
        seen.add(x.get_id())
        if z3.is_quantifier(x):
            return True
        if z3.is_app(x):
            todo.extend(x.children())
    return False


def generalized(question, trace, params: dict, timeout: float):
    """The values of the parameters for which the behavior of the trace,
    with its integers and Booleans as they are and its reals free, still
    breaks the question: a z3 formula over the parameters (it holds at the
    values of the trace), or None when it cannot be computed. A
    counterexample then removes a set of values, not one."""
    from ..backends import z3 as backend_z3
    z3 = backend_z3._z3
    if z3 is None or applicable([("q", question)]):
        return None
    from ..queries import Implies
    e = backend_z3.Encoding(question.variables, rigid=params)
    if isinstance(question, Implies):
        body = [e.formula(question.phi), z3.Not(e.formula(question.psi))]
    else:
        return None
    states = trace.prefix + trace.loop
    start = len(trace.prefix)

    def at(step):
        if step < len(states):
            return states[step]
        return states[start + (step - start) % len(trace.loop)] if trace.loop else states[-1]
    fixed, reals = [], []
    for (name, step), v in e.vars.items():
        if name in params:
            continue
        if z3.is_real(v):
            reals.append(v)
            continue
        if step < 0:            # before step 0: past(x, n), from the history of the trace
            value = at(0).get(f"chase_past{-step}_{name}")
        else:
            value = at(step).get(name)
        if value is None:
            return None
        fixed.append(v == (z3.BoolVal(value) if isinstance(value, bool) else value))
    projected = _eliminate(z3, reals, z3.And(*body, *fixed, *e.bounds()), timeout)
    return projected
