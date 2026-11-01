"""The repair problem: the parameters (constants of the contracts that may
change), the contracts with the parameters left as names, and the check run
with values of the parameters."""

from __future__ import annotations

from dataclasses import dataclass
from fractions import Fraction
from typing import Optional

from .. import ltl
from ..contract import Contract

UNIVERSAL = ("refines", "impl", "env")       # every behavior: counterexamples
EXISTENTIAL = ("verify", "compat", "consist")   # some behavior: witnesses
CHECKS = UNIVERSAL + EXISTENTIAL


class RepairError(ValueError):
    """The repair problem is not well posed."""


@dataclass
class Parameter:
    """A constant that the repair may change, within [low, high]: a real,
    or an integer (``(low, high, "int")``)."""
    name: str
    low: object
    high: object
    value: object
    integer: bool

    @property
    def type(self):
        return (int(self.low), int(self.high)) if self.integer else "real"

    def range(self) -> ltl.Node:
        v = ltl.Var(self.name)
        return ltl.Binary("&", ltl.Binary(">=", v, ltl.Num(self.low)),
                          ltl.Binary("<=", v, ltl.Num(self.high)))


def contracts_in(check: str, args: tuple) -> list:
    """The contracts among the arguments of a check."""
    return [a for a in args if isinstance(a, Contract)]


def parameters_of(check: str, args: tuple, spec: dict) -> dict:
    """The parameters, from {name: (low, high)} and the constants of the
    contracts (their present values)."""
    if check not in CHECKS:
        raise RepairError(f"check {check!r}: one of {', '.join(CHECKS)}")
    values = {}
    for c in contracts_in(check, args):
        for k, v in c.constants.items():
            if k in values and values[k] != v:
                raise RepairError(f"constant {k} has two values: {values[k]} and {v}")
            values[k] = v
    if not spec:
        raise RepairError("no parameters: give parameters={name: (low, high)}")
    out = {}
    for name, bounds in spec.items():
        if name not in values:
            raise RepairError(f"{name} is not a constant of the contracts (their constants: "
                              f"{', '.join(sorted(values)) or 'none'})")
        kind = "real"
        if isinstance(bounds, (tuple, list)) and len(bounds) == 3:
            bounds, kind = bounds[:2], bounds[2]
        try:
            low, high = (ltl.number(b) for b in bounds)
        except (TypeError, ValueError):
            raise RepairError(f"parameters[{name!r}] = {bounds!r}: give (low, high), or "
                              "(low, high, 'int')") from None
        if kind not in ("real", "int"):
            raise RepairError(f"parameters[{name!r}]: the type is 'real' or 'int', not {kind!r}")
        if low > high:
            raise RepairError(f"{name}: empty range [{low}, {high}]")
        value = values[name]
        integer = kind == "int"
        if integer and not all(isinstance(x, int) for x in (low, high, value)):
            raise RepairError(f"{name} is an integer: its range and its value are integers")
        out[name] = Parameter(name, low, high, value, integer)
    return out


def symbolic(c: Contract, params: dict) -> Contract:
    """The contract with the parameters as variables (outputs, of their
    types), the other constants with their values."""
    mine = {p: params[p] for p in params if p in c.constants}
    if not mine:
        return c
    out = Contract(c.name, dict(c.inputs), {**c.outputs, **{p: q.type for p, q in mine.items()}})
    a, g = c.terms
    rest = {k: v for k, v in c.constants.items() if k not in mine}
    out._set_terms(a, g, rest)
    return out


def symbolic_args(check: str, args: tuple, params: dict) -> tuple:
    return tuple(symbolic(a, params) if isinstance(a, Contract) else a for a in args)


def instantiated(args: tuple, values: dict) -> tuple:
    """The arguments with the parameters at the values."""
    out = []
    for a in args:
        if isinstance(a, Contract) and set(a.constants) & set(values):
            a = a.instantiate({k: v for k, v in values.items() if k in a.constants})
        out.append(a)
    return tuple(out)


def questions(check: str, sym_args: tuple, params: Optional[dict] = None, context=None) -> list:
    """(name, query) of the check on the contracts with the parameters as
    variables (see chase_cps.export). A state machine gets the parameters
    as frozen variables (FROZENVAR), which its contract reads, and the
    context as the check reads it: after the steps of a reset."""
    from ..cprograms import CFunction
    from ..queries import Implies
    if check == "impl" and isinstance(sym_args[0], CFunction):
        c = sym_args[1]
        return [("impl", Implies(c.assumption, c.guarantee, c.variables))]
    m = None
    if check in ("impl", "env"):
        from ..models import as_model
        m = as_model(sym_args[0], sym_args[1])
        if m is not None and params:
            m = with_parameters(m, params)
            sym_args = (m,) + tuple(sym_args[1:])
    from ..export import questions as export_questions
    asked = export_questions(check, *sym_args)[1]
    if m is not None and context is not None:
        from ..checks import holds_query
        c = sym_args[1]
        phi = c.saturated_guarantee if check == "impl" else c.assumption
        what = f"the contract {c.name}" if check == "impl" else f"the assumptions of {c.name}"
        asked = [(name, holds_query(m, phi, c, what, context))
                 for name, _ in asked]
    return asked


def with_parameters(model, params: dict):
    """The model with the parameters it does not declare as frozen
    variables."""
    import dataclasses
    from ..backends.nusmv import _smv_type
    new = {p.name: p.type for p in params.values() if p.name not in model.variables}
    if not new:
        return model
    decls = "".join(f"    {n} : {_smv_type(t)};\n" for n, t in new.items())
    return dataclasses.replace(model, smv=model.smv.rstrip() + "\nFROZENVAR\n" + decls,
                               variables={**model.variables, **new})


@dataclass
class Outcome:
    """The check with values of the parameters: passed (True, False, or None
    for no answer), its result, and the counterexamples [(question, trace)]."""
    passed: Optional[bool]
    result: object
    counterexamples: list
    reason: str = ""


def run(check: str, args: tuple, backend: str, options: dict) -> Outcome:
    """The check itself (the functions of chase_cps.checks)."""
    from .. import checks
    if check == "refines":
        r = checks.refines(*args, backend=backend, **options)
        if r.status == "valid":
            return Outcome(True, r, [])
        if r.status == "invalid":
            part = r.assumptions if r.failed == "assumptions" else r.guarantees
            cex = [(r.failed, part.trace)] if part.trace is not None else []
            return Outcome(False, r, cex)
        return Outcome(None, r, [], _reason(r.assumptions) or _reason(r.guarantees))
    if check in ("impl", "env"):
        fn = checks.is_implementation if check == "impl" else checks.is_environment
        r = fn(*args, backend=backend, **options)
        if r.status == "valid":
            return Outcome(True, r, [])
        if r.status == "invalid":
            return Outcome(False, r, [(check, r.trace)] if r.trace is not None else [])
        return Outcome(None, r, [], r.reason)
    fn = {"verify": checks.verify, "compat": checks.is_compatible,
          "consist": checks.is_consistent}[check]
    r = fn(*args, backend=backend, **options)
    if r.status == "sat":
        return Outcome(True, r, [])
    if r.status == "unsat":
        return Outcome(False, r, [])
    return Outcome(None, r, [], getattr(r, "reason", ""))


def _reason(r) -> str:
    return getattr(r, "reason", "") if getattr(r, "status", "") == "unknown" else ""


def json_value(v):
    if isinstance(v, Fraction):
        return int(v) if v.denominator == 1 else float(v)
    return v
