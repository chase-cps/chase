"""Assume-guarantee contracts over LTL, backed by the CHASE core library."""

from __future__ import annotations

import os
import tempfile
from dataclasses import dataclass, field
from typing import Iterable, Mapping, Union

from . import ltl

# Words that cannot be used as contract or variable names.
RESERVED = frozenset({
    "G", "F", "X", "U", "R", "W", "P", "true", "false", "TRUE", "FALSE", "NAME",
    "CONTRACT", "Assumptions", "Guarantees", "integer", "boolean", "real",
    "variable", "input", "output", "constant", "proposition", "is"})

# A variable type: "bool", or an integer range (low, high), bounds included.
VarType = Union[str, tuple]


def _normalize_type(t) -> VarType:
    if t in ("bool", "boolean", bool, None):
        return "bool"
    if isinstance(t, range):
        return (t.start, t.stop - 1)
    if isinstance(t, (tuple, list)) and len(t) == 2:
        lo, hi = int(t[0]), int(t[1])
        if lo > hi:
            raise ValueError(f"empty integer range {t!r}")
        return (lo, hi)
    raise ValueError(f"unsupported variable type {t!r}: use 'bool' or (low, high)")


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
    conjunctions. Variables are Boolean ("bool") or bounded integers
    (low, high).
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
    def from_pychase(cls, pc) -> "Contract":
        """Convert a contract of the CHASE core library."""
        from ._native import representation as R
        inputs, outputs = {}, {}
        for d in pc.declarations:
            t = d.getType()
            vtype = (t.getMin(), t.getMax()) if type(t).__name__ == "Integer" else "bool"
            target = inputs if d.getCausality() == R.causality_t.input else outputs
            target[d.getName().getString()] = vtype

        def terms(domain_map) -> list[ltl.Node]:
            out = []
            for spec in domain_map.values():
                out.extend(ltl.conjuncts(ltl.from_pychase(spec)))
            return out

        return cls(name=pc.getName().getString(), inputs=inputs, outputs=outputs,
                   assumptions=terms(pc.assumptions), guarantees=terms(pc.guarantees))

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
            return "bool" if v == "bool" else list(v)
        return {"name": self.name,
                "inputs": {k: t(v) for k, v in self.inputs.items()},
                "outputs": {k: t(v) for k, v in self.outputs.items()},
                "assumptions": [ltl.to_str(f) for f in self.assumptions],
                "guarantees": [ltl.to_str(f) for f in self.guarantees]}

    @classmethod
    def from_dict(cls, d: Mapping) -> "Contract":
        def t(v):
            return "bool" if v in ("bool", "boolean") else tuple(v)
        return cls.from_strings(
            inputs={k: t(v) for k, v in (d.get("inputs") or {}).items()},
            outputs={k: t(v) for k, v in (d.get("outputs") or {}).items()},
            assumptions=d.get("assumptions") or [],
            guarantees=d.get("guarantees") or [], name=d.get("name", "C"))

    def __str__(self) -> str:
        lines = [f"CONTRACT {self.name}"]
        for k, v in self.variables.items():
            kind = "input" if k in self.inputs else "output"
            lines.append(f"  {kind} {k} : {'bool' if v == 'bool' else f'{v[0]}..{v[1]}'}")
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
            typ = "boolean" if v == "bool" else f"integer ({v[0]}:{v[1]})"
            lines.append(f"    {kind} {typ} variable {k};")
        lines.append("    Assumptions:")
        lines += [f"        {ltl.to_chase(f)};" for f in self.assumptions] or ["        true;"]
        lines.append("    Guarantees:")
        lines += [f"        {ltl.to_chase(f)};" for f in self.guarantees] or ["        true;"]
        return "\n".join(lines) + "\n"

    def to_pychase(self):
        """Parse the contract with the CHASE core library.

        Returns the pychase ``Contract``. The parsed ``System`` that owns it is
        kept alive as an attribute of the returned object's holder.
        """
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

    def compose(self, other: "Contract", name: str | None = None) -> "Contract":
        """Parallel composition; variables with the same name are connected."""
        both = set(self.outputs) & set(other.outputs)
        if both:
            raise ValueError(f"cannot compose {self.name} and {other.name}: "
                             f"{sorted(both)} are outputs of both")
        return _algebra("composition", self, other, name or f"{self.name}_{other.name}")

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

    def quotient(self, other: "Contract", name: str | None = None) -> "Contract":
        """Largest C such that other || C refines self:
        (A1 & G2, (G1 & A2) | !(A1 & G2)) (Incer et al., MEMOCODE 2018)."""
        return _algebra("quotient", self, other, name or f"{self.name}_div_{other.name}")

    def separation(self, other: "Contract", name: str | None = None) -> "Contract":
        """Smallest C such that self refines other . C (merging):
        ((A1 & G2) | !(G1 & A2), G1 & A2) (Passerone et al., TECS 2019)."""
        return _algebra("separation", self, other, name or f"{self.name}_sep_{other.name}")


class _Holder:
    """Keeps the pychase System alive while its contracts are in use."""

    def __init__(self, system, contracts):
        self.system = system
        self.contracts = contracts

    def __getitem__(self, name):
        return self.contracts[name]


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


def load_spec(path: str | os.PathLike) -> dict[str, Contract]:
    """The contracts of a specification file in the logics language of CHASE,
    by name."""
    from ._native import pychase_logicsLang
    if not os.path.isfile(path):
        raise FileNotFoundError(f"no such file: {os.fspath(path)}")
    system = pychase_logicsLang.LogicsSpecsBuilder().parseSpecificationFile(os.fspath(path))
    return {c.getName().getString(): Contract.from_pychase(c)
            for c in system.getContractsSet()}


def parse_spec(text: str) -> dict[str, Contract]:
    """Like ``load_spec``, from the text of the specification."""
    fd, path = tempfile.mkstemp(suffix=".ltl", prefix="chase_cps_")
    try:
        with os.fdopen(fd, "w") as f:
            f.write(text)
        return load_spec(path)
    finally:
        os.unlink(path)


def to_spec(contracts: Iterable[Contract], name: str = "chase_cps") -> str:
    """A specification in the logics language of CHASE with the contracts."""
    return f"NAME: {name};\n\n" + "\n".join(c.to_logics() for c in contracts)


def _saturated(c: Contract, name: str | None = None) -> Contract:
    """The same contract with guarantee A -> G (the algebra expects it)."""
    guarantees = [c.saturated_guarantee] if c.assumptions else list(c.guarantees)
    return Contract(name or c.name, dict(c.inputs), dict(c.outputs),
                    list(c.assumptions), guarantees)


def _algebra(operation: str, c1: Contract, c2: Contract, name: str) -> Contract:
    from ._native import representation as R
    # The operations of the core library are defined on saturated contracts.
    c1 = _saturated(c1)
    c2 = _saturated(c2, c2.name + "_2" if c1.name == c2.name else None)
    # Variables with the same name are the same variable (connected ports).
    shared = {}
    for v in set(c1.variables) & set(c2.variables):
        if c1.variables[v] != c2.variables[v]:
            raise ValueError(f"variable {v} has type {c1.variables[v]} in {c1.name} "
                             f"and {c2.variables[v]} in {c2.name}")
        shared[v] = v
    holder = _parse_contracts([c1, c2])
    p1, p2 = holder[c1.name], holder[c2.name]
    fn = getattr(R.Contract, operation, None)
    if fn is None:
        raise NotImplementedError(f"the CHASE core library does not provide {operation}")
    if operation == "quotient":
        result = fn(p1, p2, shared, name, False)
    else:
        result = fn(p1, p2, shared, name)
    out = Contract.from_pychase(result)
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
