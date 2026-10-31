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
projection is not used: the repair goes by counterexamples. The
elimination grows fast with the variables and the disjunctions (a contract
that starts at the later of two events has one): by default it is tried
only on questions with at most LIMIT variables to eliminate, and the larger
ones go by counterexamples, each generalized by a model-based projection
(``generalized``), which is cheap.
"""

from __future__ import annotations

from .. import ltl
from ..queries import Implies, Satisfiable


LIMIT = 12          # variables to eliminate, at most, for the projection by default


def eliminated(asked: list, params: dict) -> int:
    """The most variables a question has besides the parameters (each of its
    steps counted), what the projection eliminates."""
    from ..backends import z3 as backend_z3
    most = 0
    for _, q in asked:
        e = backend_z3.Encoding(q.variables, rigid=params)
        if isinstance(q, Implies):
            e.formula(q.phi), e.formula(q.psi)
        else:
            e.formula(q.phi)
        most = max(most, sum(1 for (n, _) in e.vars if n not in params))
    return most


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


def generalized(question, trace, params: dict, values: dict, timeout: float):
    """Values of the parameters for which the behavior of the trace, with its
    integers and Booleans as they are and its reals free, still breaks the
    question: a z3 formula over the parameters that holds at the values the
    trace was found with, or None when it cannot be computed. A
    counterexample then removes a set of values, not one.

    The set is a model-based projection (Komuravelli, Gurfinkel, Chaki, CAV
    2014; Bjoerner, Janota, LPAR 2015) of the behaviors that break the
    question, at the values: every value in it breaks the question (it is
    inside the projection), and it is computed in polynomial time on linear
    arithmetic, where the whole projection may take exponential time."""
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
    fixed, others = [], []
    for (name, step), v in e.vars.items():
        if name in params:
            continue
        others.append(v)
        if z3.is_real(v):
            continue
        if step < 0:            # before step 0: past(x, n), from the history of the trace
            value = at(0).get(f"chase_past{-step}_{name}")
        else:
            value = at(step).get(name)
        if value is None:
            return None
        fixed.append(v == (z3.BoolVal(value) if isinstance(value, bool) else value))
    point = []
    for name, p in params.items():
        if name not in values:
            return None
        v = e.var(name)
        point.append(v == (z3.RealVal(str(values[name])) if z3.is_real(v) else z3.IntVal(int(values[name]))))
    return _project_at(z3, others, z3.And(*body, *fixed, *e.bounds()), point, timeout)


def _project_at(z3, xs, body, point, timeout):
    """A model-based projection of body on the variables but xs, at a model
    of body with the point: a formula over those variables that holds at
    the point and implies that body holds for some xs; None when there is no
    such model, or when a variable of xs is left (nonlinear terms)."""
    from z3 import z3core, z3types
    s = z3.Solver()
    s.set("timeout", int(timeout * 1000))
    s.add(body, *point)
    if s.check() != z3.sat:
        return None
    if not xs:
        return body
    m = s.model()
    bound = (z3types.Ast * len(xs))(*[x.as_ast() for x in xs])
    try:
        r = z3core.Z3_qe_model_project(body.ctx.ref(), m.model, len(xs), bound, body.as_ast())
    except z3.Z3Exception:
        return None
    f = z3.simplify(z3.BoolRef(r, body.ctx))
    left = {x.get_id() for x in xs}
    todo, seen = [f], set()
    while todo:
        x = todo.pop()
        if x.get_id() in seen:
            continue
        seen.add(x.get_id())
        if x.get_id() in left or z3.is_quantifier(x):
            return None
        if z3.is_app(x):
            todo.extend(x.children())
    return f
