"""Real variables in the checks.

A query with real variables is answered in this order:

1. regions: when every comparison of a real variable is with a constant
   (``kias > 40``, ``kias' <= 30.5``), the variable only matters through the
   interval of the constants it is in. With the constants c1 < ... < ck,
   the real line is split into the 2k + 1 regions
   ``(-inf, c1), c1, (c1, c2), ..., ck, (ck, inf)``, and the variable is
   replaced by the integer index of its region. The answer is exact, and
   the GR(1) back ends (realizability, synthesis) can be used.
2. smt: the SMT engine of nuXmv, exact for linear arithmetic; it may not
   finish with nonlinear arithmetic. Not for realizability.
3. predicates: each comparison is replaced by a Boolean variable (see
   ``_predicates``). Only the answers that hold for the real variables are
   returned: valid for an implication, unsat for a satisfiability query;
   the others are unknown, with the trace.

The result says which one answered in ``reals``.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from fractions import Fraction
from typing import Optional

from . import ltl
from .contract import Contract
from .queries import (INVALID, SAT, UNKNOWN, UNSAT, VALID, Implies, Realizable, Result,
                      Satisfiable, Trace)

REGIONS, SMT, PREDICATES = "regions", "smt", "predicates"
NONLINEAR_SMT_TIMEOUT = 30.0    # seconds for nuXmv on nonlinear arithmetic


def has_real_variables(query) -> bool:
    """Whether a query has real variables (only their types are looked at)."""
    if isinstance(query, Realizable):
        return "real" in query.contract.variables.values()
    variables = getattr(query, "variables", None)
    return bool(variables) and "real" in variables.values()


def check(query, backend: str = "auto", **options) -> Result:
    from .backends import dispatch
    regions = Regions.of(query)
    if regions.problem is None:
        result = dispatch(regions.finite(query), backend, **options)
        result.trace = regions.real_trace(result.trace)
        if result.controller is not None:
            result.controller = regions.real_controller(result.controller)
        result.reals = REGIONS
        return result
    if isinstance(query, Realizable):
        return Result(UNKNOWN, "slugs", reals=REGIONS,
                      reason=f"real variables: {regions.problem}. Realizability and synthesis "
                      "only handle real variables compared with constants")
    if not isinstance(query, (Implies, Satisfiable)):
        return Result(UNKNOWN, reason=f"no back end answers {type(query).__name__} "
                      "with real variables")
    from . import predicates
    smt_options = dict(options)
    if predicates.nonlinear(query):
        # nuXmv seldom proves nonlinear properties: leave time to the predicates
        smt_options["timeout"] = min(options.get("timeout", NONLINEAR_SMT_TIMEOUT),
                                     NONLINEAR_SMT_TIMEOUT)
    result = dispatch(query, backend, **smt_options)
    result.reals = SMT
    if result.status != UNKNOWN:
        return result
    abstract = predicates.check(query, backend, **options)
    if abstract is None:
        return result
    abstract.reason = (f"{result.reason or 'the SMT engine gave no answer'}; "
                       f"predicates: {abstract.reason}" if abstract.status == UNKNOWN
                       else abstract.reason)
    return abstract


# Regions --------------------------------------------------------------------

def _constant(node: ltl.Node) -> Optional[Fraction]:
    """The value of an expression without variables, or None."""
    if isinstance(node, ltl.Num):
        return Fraction(node.value)
    if isinstance(node, ltl.Unary) and node.op == "neg":
        v = _constant(node.arg)
        return None if v is None else -v
    if isinstance(node, ltl.Binary) and node.op in ltl.ARITHMETIC:
        a, b = _constant(node.left), _constant(node.right)
        if a is None or b is None or (node.op == "/" and b == 0):
            return None
        return {"+": a + b, "-": a - b, "*": a * b, "/": a / b if b else None}[node.op]
    return None


_FLIP = {"<": ">", "<=": ">=", ">": "<", ">=": "<=", "=": "=", "!=": "!="}


def _comparison(node: ltl.Binary, reals: set) -> Optional[tuple]:
    """(variable, operator, constant) when the relation compares one real
    variable (primed or not) with a constant, else None."""
    left, right = node.left, node.right
    if isinstance(left, ltl.Var) and left.name in reals:
        c = _constant(right)
        return None if c is None else (left, node.op, c)
    if isinstance(right, ltl.Var) and right.name in reals:
        c = _constant(left)
        return None if c is None else (right, _FLIP[node.op], c)
    return None


def _relations(node: ltl.Node):
    """The relations of a formula (the comparisons, outermost)."""
    if isinstance(node, ltl.Binary) and node.op in ltl.RELATIONS:
        yield node
    elif isinstance(node, ltl.Unary):
        yield from _relations(node.arg)
    elif isinstance(node, ltl.Binary):
        yield from _relations(node.left)
        yield from _relations(node.right)


def _map_relations(node: ltl.Node, fn) -> ltl.Node:
    """The formula with each relation replaced by fn(relation)."""
    if isinstance(node, ltl.Binary) and node.op in ltl.RELATIONS:
        return fn(node)
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, _map_relations(node.arg, fn))
    if isinstance(node, ltl.Binary):
        return ltl.Binary(node.op, _map_relations(node.left, fn), _map_relations(node.right, fn))
    return node


def _formulas(query) -> list[ltl.Node]:
    if isinstance(query, Implies):
        return [query.phi, query.psi]
    if isinstance(query, Realizable):
        return query.contract.assumptions + query.contract.guarantees
    return [query.phi]


def _variables(query) -> dict:
    return query.contract.variables if isinstance(query, Realizable) else query.variables


@dataclass
class Regions:
    """The regions of the real variables of a query; ``problem`` says why
    there are none (a comparison that is not with a constant)."""
    constants: dict = field(default_factory=dict)     # real variable -> sorted constants
    problem: Optional[str] = None

    @classmethod
    def of(cls, query) -> "Regions":
        reals = {v for v, t in _variables(query).items() if t == "real"}
        constants = {v: set() for v in reals}
        for f in _formulas(query):
            for rel in _relations(f):
                if not ltl.variables(rel) & reals:
                    continue
                found = _comparison(rel, reals)
                if found is None:
                    return cls(problem=f"{ltl.to_str(rel)} does not compare a real "
                               "variable with a constant")
                constants[found[0].name].add(found[2])
        return cls({v: sorted(cs) for v, cs in constants.items()})

    # The index of a region: 2i + 1 for the constant c_i (from 0), 2i for the
    # open interval below it, 2k above the last one.

    def _rewrite(self, rel: ltl.Binary) -> ltl.Node:
        found = _comparison(rel, set(self.constants))
        if found is None:
            return rel
        var, op, c = found
        index = ltl.Num(2 * self.constants[var.name].index(c) + 1)
        return ltl.Binary(op, var, index)

    def finite(self, query):
        """The query with each real variable replaced by its region index."""
        def types(vs):
            return {v: (0, 2 * len(self.constants[v])) if t == "real" else t
                    for v, t in vs.items()}

        def rw(f):
            return _map_relations(f, self._rewrite)
        if isinstance(query, Implies):
            return Implies(rw(query.phi), rw(query.psi), types(query.variables))
        if isinstance(query, Satisfiable):
            return Satisfiable(rw(query.phi), types(query.variables))
        c = query.contract
        return Realizable(Contract(c.name, types(c.inputs), types(c.outputs),
                                   [rw(f) for f in c.assumptions],
                                   [rw(f) for f in c.guarantees]))

    def value(self, var: str, index: int):
        """A real value in the region of the given index."""
        cs = self.constants[var]
        if not cs:
            return 0
        if index % 2:
            return ltl.number(cs[index // 2])
        i = index // 2
        if i == 0:
            return ltl.number(cs[0] - 1)
        if i == len(cs):
            return ltl.number(cs[-1] + 1)
        return ltl.number((cs[i - 1] + cs[i]) / 2)

    def describe(self, var: str, index: int) -> str:
        """The region of the given index as a condition on the variable."""
        cs = [ltl.number_text(c, "smv").replace("f'", "") for c in self.constants[var]]
        if not cs:
            return f"{var} any"
        if index % 2:
            return f"{var} = {cs[index // 2]}"
        i = index // 2
        if i == 0:
            return f"{var} < {cs[0]}"
        if i == len(cs):
            return f"{var} > {cs[-1]}"
        return f"{cs[i - 1]} < {var} < {cs[i]}"

    def real_trace(self, trace: Optional[Trace]) -> Optional[Trace]:
        if trace is None:
            return None

        def state(s):
            return {k: self.value(k, v) if k in self.constants and isinstance(v, int) else v
                    for k, v in s.items()}
        return Trace([state(s) for s in trace.prefix], [state(s) for s in trace.loop])

    def real_controller(self, controller):
        """The slugs controller with, for each node, the values of the
        variables, the regions of the real ones written as conditions."""
        if not isinstance(controller, dict) or "variables" not in controller:
            return controller
        names = controller["variables"]
        for node in controller.get("nodes", {}).values():
            values, bits = {}, node.get("state", [])
            for name, bit in zip(names, bits):
                if "@" not in name:
                    values[name] = bool(bit)
                    continue
                var, rest = name.split("@", 1)
                parts = rest.split(".")
                if len(parts) == 3:     # the first bit: var@0.low.high
                    values[var] = int(parts[1])
                values[var] += bit << int(parts[0])
            node["values"] = {v: self.describe(v, x) if v in self.constants else x
                              for v, x in values.items()}
        controller["regions"] = {v: [self.describe(v, i) for i in range(2 * len(cs) + 1)]
                                 for v, cs in self.constants.items()}
        return controller
