"""Repair by the parameter synthesis of nuXmv (``synth_param``; Cimatti,
Griggio, Mover, Tonetta, "Parameter synthesis with IC3", FMCAD 2013), for
a state machine checked against a contract of invariants.

nuXmv computes, with IC3, the region of values of frozen variables for
which an invariant holds on every behavior of the model: the whole set,
written as a formula over the parameters, shown by IC3. The repair is then
one optimization over the region (z3), and a run of the check itself.

nuXmv takes only invariants here (G of one step: "PARSYNTH name := {p, q |
VALID G (...)}"), real parameters, and a model without INVAR. The check of
an implementation is A -> G: with A and G made of invariants G a and G g,
it is written as the invariant "if a held at every step so far, g holds
now", with a monitor of a. That invariant is stronger: it also fails when a
held so far, g fails, and the environment could not keep a afterwards. Its
region is then inside the values that pass, so its least cost is a repair
but may not be the least; with no assumptions the two are the same.
"""

from __future__ import annotations

import os
import re
import subprocess
import tempfile

from .. import ltl
from ..queries import Holds

MONITOR = "chase_assumed"


def _terms(node):
    """The one-step conditions s of the invariants G s of a conjunction, or
    None."""
    from .copies import _one_step
    out = []
    for t in ltl.conjuncts(node):
        if isinstance(t, ltl.Const) and t.value:
            continue
        if isinstance(t, ltl.Unary) and t.op == "G" and _one_step(t.arg):
            out.append(t.arg)
        else:
            return None
    return out


def invariant(phi):
    """(the assumed conditions, the guaranteed ones) of A -> G or of G, both
    made of invariants; None otherwise."""
    if isinstance(phi, ltl.Binary) and phi.op == "->":
        a, g = _terms(phi.left), _terms(phi.right)
    else:
        a, g = [], _terms(phi)
    return None if a is None or g is None else (a, g)


def applicable(asked: list, params: dict) -> str:
    if len(asked) != 1 or not isinstance(asked[0][1], Holds):
        return "the parameter synthesis of nuXmv is for a state machine and its contract"
    if any(p.integer for p in params.values()):
        return "the parameter synthesis of nuXmv takes real parameters only"
    q = asked[0][1]
    if invariant(q.phi) is None:
        return ("the parameter synthesis of nuXmv takes invariants: every term G of a "
                "condition on one step")
    if re.search(r"(?m)^\s*INVAR\b", q.model):
        return "the parameter synthesis of nuXmv does not take a model with INVAR"
    return ""


def model(q, params) -> str:
    """The model with the monitor of the assumptions and the problem."""
    from .. import words
    a, g = invariant(q.phi)
    smv = lambda f: words.to_smv(f, q.variables)
    assumed = ltl.conjunction(a) if a else ltl.Const(True)
    guaranteed = ltl.conjunction(g) if g else ltl.Const(True)
    body = q.model.rstrip() + "\n"
    if a:
        body += (f"VAR\n    {MONITOR} : boolean;\nASSIGN\n    init({MONITOR}) := TRUE;\n"
                 f"    next({MONITOR}) := {MONITOR} & ({smv(assumed)});\n")
        spec = f"G (({MONITOR} & ({smv(assumed)})) -> ({smv(guaranteed)}))"
    else:
        spec = f"G ({smv(guaranteed)})"
    names = ", ".join(params)
    return body + f"PARSYNTH chase_repair := {{{names} | VALID {spec}}};\n"


def region(text: str, timeout: float):
    """(the region as a formula over the parameters, or None, and the reason)."""
    from ..backends.nusmv import NuSMVAdapter
    found = NuSMVAdapter(tool="nuxmv").binary()
    if found is None:
        return None, "the parameter synthesis needs nuXmv, which is not installed"
    binary = found[0]
    with tempfile.TemporaryDirectory() as tmp:
        path = os.path.join(tmp, "repair.smv")
        with open(path, "w") as f:
            f.write(text)
        commands = (f"read_model -i {path}\ngo_msat\nsynth_param -n 0 -a ic3\nquit\n")
        try:
            p = subprocess.run([binary, "-int"], input=commands, capture_output=True, text=True,
                               timeout=timeout)
        except subprocess.TimeoutExpired:
            return None, f"nuXmv: timeout after {timeout:g} s"
        except OSError as e:
            return None, f"nuXmv: {e}"
    m = re.search(r"Region for problem chase_repair is (.*)", p.stdout)
    if not m:
        why = [l.strip() for l in (p.stdout + p.stderr).splitlines()
               if "Parameter" in l or "rror" in l]
        return None, "nuXmv: " + ("; ".join(why[-3:]) or "no region")
    try:
        return _read(m.group(1).strip()), ""
    except ltl.LTLSyntaxError as e:
        return None, f"the region of nuXmv: {e}"


def _read(text: str):
    """The region printed by nuXmv (SMV syntax over the parameters)."""
    text = re.sub(r"f'(-?\d+)/(\d+)", r"(\1 / \2)", text)
    text = text.replace("TRUE", "true").replace("FALSE", "false")
    return _fold(ltl.parse(text))


def _fold(node):
    """Constant arithmetic as numbers (3 / 5 is a fraction, not a division
    of integers)."""
    from .bisection import _constant
    if isinstance(node, ltl.Unary):
        arg = _fold(node.arg)
        if node.op == "neg" and isinstance(arg, ltl.Num):
            return ltl.Num(ltl.number(-arg.value))
        return ltl.Unary(node.op, arg)
    if isinstance(node, ltl.Binary):
        folded = ltl.Binary(node.op, _fold(node.left), _fold(node.right))
        if node.op in ("+", "-", "*", "/"):
            c = _constant(folded)
            if c is not None:
                return ltl.Num(ltl.number(c))
        return folded
    return node


def by_synthesis(check, args, asked, params, cost, aux, backend, opts, timeout):
    from . import IMPOSSIBLE, REPAIRED, UNKNOWN, Repair, _best, _cost_of
    from .problem import instantiated, run
    why = applicable(asked, params)
    if why:
        return Repair(UNKNOWN, method="synthesis", reason=why)
    q = asked[0][1]
    exact = not invariant(q.phi)[0]
    found, why = region(model(q, params), timeout)
    if found is None:
        return Repair(UNKNOWN, method="synthesis", reason=why)
    o, values = _best(params, cost, aux, [found], [], timeout)
    if o.status == "infeasible":
        if exact:
            return Repair(IMPOSSIBLE, method="synthesis", rounds=1,
                          reason="the region of nuXmv has no values in the ranges")
        return Repair(UNKNOWN, method="synthesis", rounds=1,
                      reason="the region of nuXmv (of the stronger invariant) has no values in "
                             "the ranges")
    if o.status not in ("optimal", "not_attained"):
        return Repair(UNKNOWN, method="synthesis", reason=f"the optimization: {o.status} "
                      f"{o.reason}".strip())
    out = run(check, instantiated(args, values), backend, opts)
    if out.passed:
        return Repair(REPAIRED, values, _cost_of(cost, aux, values, params),
                      exact and o.status == "optimal", "synthesis", out.result, 1,
                      reason="" if exact else "the least cost in the region of the stronger "
                                              "invariant: the least may be lower")
    return Repair(UNKNOWN, values, method="synthesis", check=out.result, rounds=1,
                  reason="the check does not pass with the values of the region "
                         f"({out.reason or 'it failed'})")
