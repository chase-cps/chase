"""Queries answered by the back ends, and their results."""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Optional

from . import ltl
from .contract import Contract, VarType


# Queries ---------------------------------------------------------------------

@dataclass
class Implies:
    """Is phi -> psi valid (true on every trace)?"""
    phi: ltl.Node
    psi: ltl.Node
    variables: dict[str, VarType]


@dataclass
class Satisfiable:
    """Is there a trace satisfying phi?"""
    phi: ltl.Node
    variables: dict[str, VarType]


@dataclass
class Realizable:
    """Is there a strategy for the outputs such that A -> G holds for every
    behavior of the inputs?"""
    contract: Contract


@dataclass
class Simplify:
    """An equivalent, simpler formula. No back end answers it yet."""
    phi: ltl.Node
    variables: dict[str, VarType]


@dataclass
class Project:
    """A formula over ``keep`` only, implied by phi (existential projection).
    No back end answers it yet."""
    phi: ltl.Node
    variables: dict[str, VarType]
    keep: list[str]


QUERY_TYPES = (Implies, Satisfiable, Realizable, Simplify, Project)


# Results ---------------------------------------------------------------------

VALID, INVALID = "valid", "invalid"
SAT, UNSAT = "sat", "unsat"
REALIZABLE, UNREALIZABLE = "realizable", "unrealizable"
SIMPLIFIED = "simplified"
UNKNOWN = "unknown"


@dataclass
class Trace:
    """A lasso-shaped trace: the prefix, then the loop repeated forever.

    Each state maps every variable to its value.
    """
    prefix: list[dict[str, Any]]
    loop: list[dict[str, Any]]

    def to_dict(self) -> dict:
        return {"prefix": self.prefix, "loop": self.loop}

    def __str__(self) -> str:
        rows = []
        for i, s in enumerate(self.prefix + self.loop):
            mark = "  <- loop starts" if self.loop and i == len(self.prefix) else ""
            rows.append(f"  {i}: " + ", ".join(f"{k}={v}" for k, v in s.items()) + mark)
        return "\n".join(rows)


@dataclass
class Result:
    """The answer of a back end.

    status is valid or invalid (implication), sat or unsat (satisfiability),
    realizable or unrealizable, or unknown with the reason (e.g. a timeout,
    or no back end installed). A Result is true when the property holds.
    """
    status: str
    backend: str = ""
    trace: Optional[Trace] = None        # counterexample or witness
    controller: Optional[Any] = None     # synthesized strategy
    counter_strategy: Optional[Any] = None   # of the environment, when unrealizable
    reason: str = ""                     # why the result is unknown
    seconds: float = 0.0
    details: dict = field(default_factory=dict)

    def __bool__(self) -> bool:
        return self.status in (VALID, SAT, REALIZABLE)

    def to_dict(self) -> dict:
        d = {"status": self.status, "backend": self.backend, "seconds": round(self.seconds, 3)}
        if self.trace is not None:
            d["trace"] = self.trace.to_dict()
        if self.controller is not None:
            d["controller"] = self.controller
        if self.counter_strategy is not None:
            d["counter_strategy"] = self.counter_strategy
        if self.reason:
            d["reason"] = self.reason
        if self.details:
            d["details"] = self.details
        return d
