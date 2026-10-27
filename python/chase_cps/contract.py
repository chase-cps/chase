"""Assume-guarantee contracts over LTL, backed by the CHASE core library."""

from __future__ import annotations

import os
import sys
import tempfile
import warnings
from dataclasses import dataclass, field
from typing import Iterable, Mapping, Union

from . import ltl

# Words that cannot be used as contract or variable names.
RESERVED = frozenset({
    "G", "F", "X", "U", "R", "W", "P", "true", "false", "TRUE", "FALSE", "NAME",
    "CONTRACT", "Assumptions", "Guarantees", "integer", "boolean", "real",
    "variable", "input", "output", "constant", "proposition", "is"})

# A variable type: "bool", "real", or an integer range (low, high), bounds
# included.
VarType = Union[str, tuple]

_UNBOUNDED = float(2 ** 63 - 1)


def _normalize_type(t) -> VarType:
    if t in ("bool", "boolean", bool, None):
        return "bool"
    if t in ("real", float):
        return "real"
    if isinstance(t, range):
        return (t.start, t.stop - 1)
    if isinstance(t, (tuple, list)) and len(t) == 2:
        if not all(isinstance(b, (int, float)) and not isinstance(b, bool) and b == int(b)
                   for b in t):
            raise ValueError(f"integer range {t!r} with a bound that is not an integer")
        lo, hi = int(t[0]), int(t[1])
        if lo > hi:
            raise ValueError(f"empty integer range {t!r}")
        return (lo, hi)
    raise ValueError(f"unsupported variable type {t!r}: use 'bool', 'real', or (low, high)")


def _normalize_vars(vs) -> dict[str, VarType]:
    if vs is None:
        return {}
    if isinstance(vs, Mapping):
        return {str(k): _normalize_type(v) for k, v in vs.items()}
    return {str(k): "bool" for k in vs}


def _normalize_formulas(fs) -> list[ltl.Node]:
    if fs is None:
        return []
    if isinstance(fs, (str, ltl.Const, ltl.Var, ltl.Unary, ltl.Binary)):
        fs = [fs]
    nodes = []
    for f in fs:
        node = ltl.parse(f) if isinstance(f, str) else f
        nodes.extend(ltl.conjuncts(node))
    return nodes


@dataclass
class Contract:
    """An assume-guarantee contract.

    ``assumptions`` and ``guarantees`` are lists of LTL formulas, read as
    conjunctions. Variables are Boolean ("bool"), bounded integers
    (low, high), or reals ("real").
    """

    name: str
    inputs: dict[str, VarType] = field(default_factory=dict)
    outputs: dict[str, VarType] = field(default_factory=dict)
    assumptions: list[ltl.Node] = field(default_factory=list)
    guarantees: list[ltl.Node] = field(default_factory=list)

    # Construction ------------------------------------------------------------

    @classmethod
    def from_strings(cls, inputs=None, outputs=None, assumptions=None,
                     guarantees=None, name: str = "C") -> "Contract":
        """Build a contract from formulas in the standard LTL syntax.

        >>> c = Contract.from_strings(inputs=["req"], outputs=["grant"],
        ...                           assumptions=["G F !req"],
        ...                           guarantees=["G (req -> F grant)"])
        """
        c = cls(name=name, inputs=_normalize_vars(inputs),
                outputs=_normalize_vars(outputs),
                assumptions=_normalize_formulas(assumptions),
                guarantees=_normalize_formulas(guarantees))
        c.validate()
        return c

    @classmethod
    def from_pychase(cls, pc, shared=()) -> "Contract":
        """Convert a contract of the CHASE core library. ``shared`` are the
        declarations of the system (outside the contracts) that the contract
        may use."""
        from ._native import representation as R
        inputs, outputs, generic, constants = {}, {}, {}, {}
        for d in list(shared) + list(pc.declarations):
            name = d.getName().getString()
            t = d.getType()
            kind = type(t).__name__
            if kind not in ("Integer", "Boolean", "Real"):
                raise ValueError(f"{name!r} has the type {kind}: only Boolean, integer, "
                                 "and real variables are supported")
            if type(d).__name__ == "Constant":
                constants[name] = ltl.number(d.getValue().getValue())
                continue
            # A real without a range has the bounds -+infinity of the core library
            # (the largest int64).
            if kind == "Real" and (t.getMin() > -_UNBOUNDED or t.getMax() < _UNBOUNDED):
                raise ValueError(f"{name!r} is a real with a range: write the range as an "
                                 "assumption or a guarantee instead")
            if kind == "Integer":
                vtype = (t.getMin(), t.getMax())
            else:
                vtype = "real" if kind == "Real" else "bool"
            causality = d.getCausality()
            if causality == R.causality_t.input:
                inputs[name] = vtype
            elif causality == R.causality_t.generic:   # e.g. a proposition, inlined
                generic[name] = vtype
            else:
                outputs[name] = vtype

        def value(node):    # constants are replaced by their values
            if isinstance(node, ltl.Var) and node.name in constants:
                return ltl.Num(constants[node.name])
            if isinstance(node, ltl.Unary):
                return ltl.Unary(node.op, value(node.arg))
            if isinstance(node, ltl.Binary):
                return ltl.Binary(node.op, value(node.left), value(node.right))
            return node

        def terms(domain_map) -> list[ltl.Node]:
            out = []
            for spec in domain_map.values():
                out.extend(value(t) for t in ltl.conjuncts(ltl.from_pychase(spec)))
            return out

        assumptions, guarantees = terms(pc.assumptions), terms(pc.guarantees)
        used = ltl.variables(ltl.conjunction(assumptions + guarantees))
        outputs.update({k: v for k, v in generic.items() if k in used})
        return cls(name=pc.getName().getString(), inputs=inputs, outputs=outputs,
                   assumptions=assumptions, guarantees=guarantees)

    def validate(self) -> None:
        for n in [self.name] + list(self.variables):
            if n in RESERVED or not n.replace("_", "a").isalnum() or not n[0].isalpha():
                raise ValueError(f"{n!r} cannot be used as a name: names start with a "
                                 "letter, contain letters, digits and _, and are not "
                                 f"keywords of CHASE ({', '.join(sorted(RESERVED))})")
        both = set(self.inputs) & set(self.outputs)
        if both:
            raise ValueError(f"variables both input and output: {sorted(both)}")
        declared = self.variables
        for f in self.assumptions + self.guarantees:
            missing = ltl.variables(f) - set(declared)
            if missing:
                raise ValueError(f"undeclared variables {sorted(missing)} in "
                                 f"{ltl.to_str(f)!r} (contract {self.name})")

    # Views -------------------------------------------------------------------

    @property
    def variables(self) -> dict[str, VarType]:
        return {**self.inputs, **self.outputs}

    @property
    def assumption(self) -> ltl.Node:
        return ltl.conjunction(self.assumptions)

    @property
    def guarantee(self) -> ltl.Node:
        return ltl.conjunction(self.guarantees)

    @property
    def saturated_guarantee(self) -> ltl.Node:
        """A -> G."""
        if not self.assumptions:
            return self.guarantee
        return ltl.Binary("->", self.assumption, self.guarantee)

    def to_dict(self) -> dict:
        def t(v):
            return v if v in ("bool", "real") else list(v)
        return {"name": self.name,
                "inputs": {k: t(v) for k, v in self.inputs.items()},
                "outputs": {k: t(v) for k, v in self.outputs.items()},
                "assumptions": [ltl.to_str(f) for f in self.assumptions],
                "guarantees": [ltl.to_str(f) for f in self.guarantees]}

    @classmethod
    def from_dict(cls, d: Mapping) -> "Contract":
        def t(v):
            return tuple(v) if isinstance(v, list) else v
        return cls.from_strings(
            inputs={k: t(v) for k, v in (d.get("inputs") or {}).items()},
            outputs={k: t(v) for k, v in (d.get("outputs") or {}).items()},
            assumptions=d.get("assumptions") or [],
            guarantees=d.get("guarantees") or [], name=d.get("name", "C"))

    def __str__(self) -> str:
        lines = [f"CONTRACT {self.name}"]
        for k, v in self.variables.items():
            kind = "input" if k in self.inputs else "output"
            lines.append(f"  {kind} {k} : {v if v in ('bool', 'real') else f'{v[0]}..{v[1]}'}")
        lines.append("  Assumptions:")
        lines += [f"    {ltl.to_str(f)}" for f in self.assumptions] or ["    true"]
        lines.append("  Guarantees:")
        lines += [f"    {ltl.to_str(f)}" for f in self.guarantees] or ["    true"]
        return "\n".join(lines)

    # CHASE core library ------------------------------------------------------

    def to_logics(self) -> str:
        """The contract in the logics language of CHASE (as read by logics_tool)."""
        lines = [f"CONTRACT {self.name}:"]
        for k, v in self.variables.items():
            kind = "input" if k in self.inputs else "output"
            typ = {"bool": "boolean", "real": "real"}.get(v) or f"integer ({v[0]}:{v[1]})"
            lines.append(f"    {kind} {typ} variable {k};")
        lines.append("    Assumptions:")
        lines += [f"        {ltl.to_chase(f)};" for f in self.assumptions] or ["        true;"]
        lines.append("    Guarantees:")
        lines += [f"        {ltl.to_chase(f)};" for f in self.guarantees] or ["        true;"]
        return "\n".join(lines) + "\n"

    def to_pychase(self):
        """Parse the contract with the CHASE core library and return the
        pychase ``Contract`` (the CHASE objects are never freed)."""
        return _parse_contracts([self])[self.name]

    # Signature ---------------------------------------------------------------

    def rename(self, mapping: Mapping[str, str], name: str | None = None) -> "Contract":
        """The same contract with the variables renamed (old name -> new name)."""
        def ren(node):
            if isinstance(node, ltl.Var):
                return ltl.Var(mapping.get(node.name, node.name), node.primed)
            if isinstance(node, ltl.Unary):
                return ltl.Unary(node.op, ren(node.arg))
            if isinstance(node, ltl.Binary):
                return ltl.Binary(node.op, ren(node.left), ren(node.right))
            return node
        c = Contract(name or self.name,
                     {mapping.get(k, k): v for k, v in self.inputs.items()},
                     {mapping.get(k, k): v for k, v in self.outputs.items()},
                     [ren(f) for f in self.assumptions], [ren(f) for f in self.guarantees])
        c.validate()
        return c

    def shares_io_with(self, other: "Contract") -> bool:
        """Same inputs and outputs, with the same types."""
        return self.inputs == other.inputs and self.outputs == other.outputs

    def can_compose_with(self, other: "Contract") -> bool:
        """No variable is an output of both contracts, and the shared
        variables have the same type."""
        if set(self.outputs) & set(other.outputs):
            return False
        return all(self.variables[v] == other.variables[v]
                   for v in set(self.variables) & set(other.variables))

    # Contract algebra (computed by the CHASE core library) -------------------

    def simplify(self, name: str | None = None, nots: bool = True,
                 temporal_operators: bool = True) -> "Contract":
        """The contract with its formulas simplified by the core library (the
        simplify command of logics_tool): the negations are moved towards the
        atoms (nots), and the temporal operators are grouped
        (temporal_operators): G f & G g becomes G (f & g), F f | F g becomes
        F (f | g), X f & X g becomes X (f & g). The contract is equivalent."""
        from ._native import utilities
        holder = _parse_contracts([self])
        try:
            pc = holder[self.name]
            utilities.simplify(pc, utilities.simplify_options(nots, temporal_operators))
            out = Contract.from_pychase(pc)
        finally:
            holder.free()
        out.name = name or self.name
        out.inputs = {v: t for v, t in out.variables.items() if v in self.inputs}
        out.outputs = {v: t for v, t in out.variables.items() if v not in self.inputs}
        return out

    def saturate(self, name: str | None = None) -> "Contract":
        """Saturation: the equivalent contract (A, A -> G)."""
        guarantees = [self.saturated_guarantee] if self.assumptions else list(self.guarantees)
        return Contract(name or self.name, dict(self.inputs), dict(self.outputs),
                        list(self.assumptions), guarantees)

    def compose(self, other: "Contract", name: str | None = None, discharge: bool = False,
                backend: str = "auto", **options) -> "Contract":
        """Parallel composition; variables with the same name are connected.

        By default the closed form of the core library:
        ((A1 & A2) | !(G1 & G2), G1 & G2).

        With ``discharge=True``, the result keeps the terms of the two
        contracts (Incer, PhD thesis 2022, Algorithm 2, without variable
        elimination). When the outputs of one contract feed the inputs of the
        other, each assumption of the receiving contract implied by the
        assumptions and guarantees of the driving one is dropped; under
        feedback, this is done in one direction only, the one that drops more
        assumptions. Then the terms implied by the others are dropped as in
        ``remove_redundant``. Each test is one implication check (nuXmv or
        NuSMV; ``backend`` and ``options`` as for the checks). The result is
        an abstraction of the composition: the closed form refines it, so a
        refinement proved on it holds for the composition.
        """
        both = set(self.outputs) & set(other.outputs)
        if both:
            raise ValueError(f"cannot compose {self.name} and {other.name}: "
                             f"{sorted(both)} are outputs of both")
        name = name or f"{self.name}_{other.name}"
        if discharge:
            return _compose_terms(self, other, name, backend, **options)
        return _algebra("composition", self, other, name)

    def conjoin(self, other: "Contract", name: str | None = None) -> "Contract":
        """Greatest lower bound (both specifications at once):
        (A1 | A2, G1 & G2)."""
        return _algebra("conjunction", self, other, name or f"{self.name}_and_{other.name}")

    def disjoin(self, other: "Contract", name: str | None = None) -> "Contract":
        """Least upper bound: (A1 & A2, G1 | G2)."""
        return _algebra("disjunction", self, other, name or f"{self.name}_or_{other.name}")

    def merge(self, other: "Contract", name: str | None = None) -> "Contract":
        """Merging of two viewpoints of the same component:
        (A1 & A2, (G1 & G2) | !(A1 & A2))."""
        return _algebra("merging", self, other, name or f"{self.name}_merge_{other.name}")

    def divide(self, other: "Contract", name: str | None = None) -> "Contract":
        """Largest C such that other || C refines self:
        (A1 & G2, (G1 & A2) | !(A1 & G2)) (Incer et al., MEMOCODE 2018)."""
        return _algebra("quotient", self, other, name or f"{self.name}_div_{other.name}")

    def separate(self, other: "Contract", name: str | None = None) -> "Contract":
        """Smallest C such that self refines other . C (merging):
        ((A1 & G2) | !(G1 & A2), G1 & A2) (Passerone et al., TECS 2019)."""
        return _algebra("separation", self, other, name or f"{self.name}_sep_{other.name}")

    def imply(self, other: "Contract", name: str | None = None) -> "Contract":
        """Implication self -> other, the adjoint of conjunction: the largest C
        such that C conjoined with self refines other:
        ((A2 & !A1) | (G1 & !G2), G2 | !G1) (Incer et al., arXiv 2309.08875,
        Proposition 6)."""
        return _algebra("implication", self, other, name or f"{self.name}_imply_{other.name}")

    def coimply(self, other: "Contract", name: str | None = None) -> "Contract":
        """Coimplication, the adjoint of disjunction: the smallest C such that
        other refines C disjoined with self:
        (A2 | !A1, (G2 & !G1) | (A1 & !A2)) (Incer et al., arXiv 2309.08875,
        Proposition 7)."""
        return _algebra("coimplication", self, other,
                        name or f"{self.name}_coimply_{other.name}")

    def invert(self, name: str | None = None) -> "Contract":
        """Reciprocal (G, A): the contract seen from its environment; the
        inputs become outputs and the outputs inputs."""
        from ._native import representation as R
        c = self.saturate()
        holder = _parse_contracts([c])
        try:
            result = R.Contract.reciprocal(holder[c.name], name or f"{self.name}_inv")
            holder.results.append(result)
            out = Contract.from_pychase(result)
        finally:
            holder.free()
        variables = {**out.inputs, **out.outputs}
        out.inputs = {v: t for v, t in variables.items() if v in self.outputs}
        out.outputs = {v: t for v, t in variables.items() if v not in self.outputs}
        return out

    def assume(self, formula, name: str | None = None) -> "Contract":
        """Adds the assumption b: (b & A, b -> G), the left action of Incer et
        al., arXiv 2309.08875, Section 4, computed as the merging of (b, true)
        with the contract. The result is an abstraction of the contract."""
        b = _single_term(self, formula, assumption=True)
        return _algebra("merging", self, b, name or f"{self.name}_assume")

    def ensure(self, formula, name: str | None = None) -> "Contract":
        """Adds the guarantee b: (b -> A, b & G), the right action of Incer et
        al., arXiv 2309.08875, Section 4, computed as the composition of
        (true, b) with the contract. The result refines the contract."""
        b = _single_term(self, formula, assumption=False)
        return _algebra("composition", self, b, name or f"{self.name}_ensure")


def _single_term(c: Contract, formula, assumption: bool) -> Contract:
    """The contract (b, true) or (true, b) over the variables of c, as
    inputs."""
    f = ltl.parse(formula) if isinstance(formula, str) else formula
    missing = ltl.variables(f) - set(c.variables)
    if missing:
        raise ValueError(f"undeclared variables {sorted(missing)} in {ltl.to_str(f)!r} "
                         f"(contract {c.name})")
    inputs = {v: c.variables[v] for v in sorted(ltl.variables(f))}
    if assumption:
        return Contract(f"{c.name}_b", inputs, {}, [f], [])
    return Contract(f"{c.name}_b", inputs, {}, [], [f])


def _compose_terms(c1: Contract, c2: Contract, name: str, backend: str,
                   **options) -> Contract:
    """Composition keeping the terms (see Contract.compose)."""
    from .checks import VALID, _merge_vars, remove_redundant
    from .logic import _Implications
    variables = _merge_vars(c1, c2)

    def refine(terms, context):
        # The terms not implied by the context (RefineWithSupport). Each
        # check uses the terms of the context that share variables with the
        # term, when the context is satisfiable (see logic._Implications).
        check = _Implications(context, variables, backend, **options)
        return [t for t in terms if check(context, t).status != VALID]

    # The guarantees of one contract simplify the assumptions of the other
    # in one direction only: in both directions at once the result can be
    # wrong (Incer, thesis, Example 7.3.2). The direction follows the
    # connections; under feedback, either direction is correct (Prop. 7.3.1
    # does not depend on the connections), and the one that discharges more
    # assumptions is kept.
    feeds12 = bool(set(c1.outputs) & set(c2.inputs))
    feeds21 = bool(set(c2.outputs) & set(c1.inputs))
    a1, a2 = list(c1.assumptions), list(c2.assumptions)
    if feeds12 and not feeds21:
        assumptions = a1 + refine(a2, a1 + list(c1.guarantees))
    elif feeds21 and not feeds12:
        assumptions = refine(a1, a2 + list(c2.guarantees)) + a2
    elif feeds12 and feeds21:
        kept2 = refine(a2, a1 + list(c1.guarantees))
        kept1 = refine(a1, a2 + list(c2.guarantees))
        if len(a2) - len(kept2) >= len(a1) - len(kept1):
            assumptions = a1 + kept2
        else:
            assumptions = kept1 + a2
    else:
        assumptions = a1 + a2
    outputs = {**c1.outputs, **c2.outputs}
    inputs = {v: t for v, t in {**c1.inputs, **c2.inputs}.items() if v not in outputs}
    c = Contract(name, inputs, outputs, assumptions, list(c1.guarantees) + list(c2.guarantees))
    return remove_redundant(c, backend, **options).contract


class _Holder:
    """Keeps the pychase System alive while its contracts are in use.
    free() frees the system and the results of the operations (the
    contracts made from the contracts of the system)."""

    def __init__(self, system, contracts):
        self.system = system
        self.contracts = contracts
        self.results = []

    def __getitem__(self, name):
        return self.contracts[name]

    def free(self) -> None:
        from ._native import utilities
        objects = self.results + [self.system]
        self.contracts, self.results, self.system = {}, [], None
        for obj in objects:
            utilities.destroy(obj)


def _parse_contracts(contracts: Iterable[Contract]) -> _Holder:
    from ._native import pychase_logicsLang
    text = to_spec(contracts)
    fd, path = tempfile.mkstemp(suffix=".ltl", prefix="chase_cps_")
    try:
        with os.fdopen(fd, "w") as f:
            f.write(text)
        builder = pychase_logicsLang.LogicsSpecsBuilder()
        system = builder.parseSpecificationFile(path)
    finally:
        os.unlink(path)
    found = {c.getName().getString(): c for c in system.getContractsSet()}
    return _Holder(system, found)


def _parse_file(path: str, name: str):
    """The system of a specification file. The parser writes its syntax
    errors (line:column and the token) on the standard error: they are given
    in the message of the exception, with the file called name."""
    from ._native import pychase_logicsLang
    sys.stderr.flush()
    with tempfile.TemporaryFile(mode="w+") as err:
        saved = os.dup(2)
        os.dup2(err.fileno(), 2)
        failure = None
        try:
            system = pychase_logicsLang.LogicsSpecsBuilder().parseSpecificationFile(path)
        except RuntimeError as e:
            failure = e
        finally:
            os.dup2(saved, 2)
            os.close(saved)
        err.seek(0)
        lines = err.read().splitlines()
    syntax = [line for line in lines if line.startswith("line ")]
    other = [line.replace(path, name) for line in lines
             if line not in syntax or failure is None]
    if other:
        sys.stderr.write("\n".join(other) + "\n")
    if failure is not None:
        message = str(failure).replace(path, name)
        if syntax:
            message = message.rstrip(".") + ": " + "; ".join(syntax)
        raise RuntimeError(message) from None
    return system


def load_spec(path: str | os.PathLike, _name: str | None = None) -> dict[str, Contract]:
    """The contracts of a specification file in the logics language of CHASE,
    by name."""
    if not os.path.isfile(path):
        raise FileNotFoundError(f"no such file: {os.fspath(path)}")
    from ._native import utilities
    system = _parse_file(os.fspath(path), _name or os.fspath(path))
    try:
        contracts = {c.getName().getString(): c for c in system.getContractsSet()}
        shared = list(system.getDeclarationsSet())
        out = {}
        for name in sorted(contracts):
            c = Contract.from_pychase(contracts[name], shared)
            # keep the shared declarations the contract uses
            used = ltl.variables(ltl.conjunction(c.assumptions + c.guarantees))
            local = {d.getName().getString() for d in contracts[name].declarations}
            c.inputs = {v: t for v, t in c.inputs.items() if v in used or v in local}
            c.outputs = {v: t for v, t in c.outputs.items() if v in used or v in local}
            out[name] = c
    finally:
        utilities.destroy(system)
    for c in out.values():
        _warn_outside_ranges(c, _name or os.fspath(path))
    return out


def _warn_outside_ranges(c: Contract, source: str) -> None:
    """Warns when an integer variable is compared with a constant outside its
    range: the comparison then has the same value at every step, which is
    rarely meant (e.g. a range left out, 0..16 by default)."""
    found = {}

    def walk(node):
        if isinstance(node, ltl.Binary):
            if node.op in ltl.RELATIONS:
                for v, k in ((node.left, node.right), (node.right, node.left)):
                    if isinstance(v, ltl.Var) and isinstance(k, ltl.Num):
                        t = c.variables.get(v.name)
                        if isinstance(t, tuple) and not t[0] <= k.value <= t[1]:
                            found.setdefault((v.name, t), set()).add(k.value)
            walk(node.left)
            walk(node.right)
        elif isinstance(node, ltl.Unary):
            walk(node.arg)

    for f in c.assumptions + c.guarantees:
        walk(f)
    for (v, (lo, hi)), constants in sorted(found.items()):
        default = " (the range of an integer declared without one)" if (lo, hi) == (0, 16) \
            else ""
        warnings.warn(f"{source}, contract {c.name}: {v} is compared with "
                      f"{', '.join(str(k) for k in sorted(constants))}, outside its range "
                      f"{lo}..{hi}{default}", stacklevel=3)


def parse_spec(text: str) -> dict[str, Contract]:
    """Like ``load_spec``, from the text of the specification."""
    fd, path = tempfile.mkstemp(suffix=".ltl", prefix="chase_cps_")
    try:
        with os.fdopen(fd, "w") as f:
            f.write(text)
        return load_spec(path, "the specification")
    finally:
        os.unlink(path)


def to_spec(contracts: Iterable[Contract], name: str = "chase_cps") -> str:
    """A specification in the logics language of CHASE with the contracts."""
    return f"NAME: {name};\n\n" + "\n".join(c.to_logics() for c in contracts)


def _algebra(operation: str, c1: Contract, c2: Contract, name: str) -> Contract:
    from ._native import representation as R
    # The operations of the core library are defined on saturated contracts.
    c1 = c1.saturate()
    c2 = c2.saturate(c2.name + "_2" if c1.name == c2.name else None)
    # Variables with the same name are the same variable (connected ports).
    shared = {}
    for v in set(c1.variables) & set(c2.variables):
        if c1.variables[v] != c2.variables[v]:
            raise ValueError(f"variable {v} has type {c1.variables[v]} in {c1.name} "
                             f"and {c2.variables[v]} in {c2.name}")
        shared[v] = v
    fn = getattr(R.Contract, operation, None)
    if fn is None:
        raise NotImplementedError(f"the CHASE core library does not provide {operation}")
    holder = _parse_contracts([c1, c2])
    try:
        p1, p2 = holder[c1.name], holder[c2.name]
        if operation == "quotient":
            result = fn(p1, p2, shared, name, False)
        else:
            result = fn(p1, p2, shared, name)
        holder.results.append(result)
        out = Contract.from_pychase(result)
    finally:
        holder.free()
    if operation == "quotient":
        # The quotient controls the outputs of c1 not provided by c2 and the
        # inputs of c2 not provided by the environment of c1 (as in Pacti).
        outputs = (set(c1.outputs) - set(c2.outputs)) | (set(c2.inputs) - set(c1.inputs))
    else:
        # A variable that is an output of one operand stays an output.
        outputs = set(c1.outputs) | set(c2.outputs)
    variables = {**out.inputs, **out.outputs}
    out.inputs = {v: t for v, t in variables.items() if v not in outputs}
    out.outputs = {v: t for v, t in variables.items() if v in outputs}
    return out
