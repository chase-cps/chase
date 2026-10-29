"""Components and systems: contracts bound to the parts of a design.

A component is a contract with a name and, optionally, an implementation (a
``Model``: a state machine in SMV or JSON, RTL read with Yosys, or a
synthesized controller). A system is a set of components, connected by the
names of their variables, with the contract the system must meet. Checking
a system checks that each implementation implements its contract and that
the composition of the contracts refines the contract of the system; if
both hold, the system of implementations meets the contract of the system
(Benveniste et al., Foundations and Trends in EDA 2018, Section 4).
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Optional

from .contract import Contract


@dataclass
class Component:
    """A part of a design: its contract and, optionally, its implementation.
    ``status`` is the result of the last check of the implementation against
    the contract (valid, invalid, unknown), or None."""

    name: str
    contract: Contract
    implementation: Optional[object] = None
    status: Optional[str] = None


@dataclass
class System:
    """Components connected by the names of their variables, and the
    contract of the system. ``status`` is the result of the last check of
    the composition against the contract of the system, or None."""

    name: str
    components: list = field(default_factory=list)
    contract: Optional[Contract] = None
    status: Optional[str] = None

    def composition(self, discharge: bool = False, **options) -> Contract:
        """The composition of the contracts of the components."""
        if not self.components:
            raise ValueError(f"system {self.name} has no component")
        result = self.components[0].contract
        for c in self.components[1:]:
            result = result.compose(c.contract, name=self.name, discharge=discharge, **options)
        return result

    def connections(self) -> list[tuple[str, str, str]]:
        """(driver, reader, variable) for each output of a component read by
        another one."""
        out = []
        for d in self.components:
            for r in self.components:
                if d is not r:
                    out += [(d.name, r.name, v) for v in d.contract.outputs
                            if v in r.contract.inputs]
        return out

    def check(self, discharge: bool = False, backend: str = "auto", **options) -> dict:
        """Checks each implementation against its contract, and the
        composition against the contract of the system; sets the statuses
        and returns them by name (the system under its own name)."""
        from .checks import is_implementation, refines
        statuses = {}
        for c in self.components:
            if c.implementation is not None:
                c.status = is_implementation(c.implementation, c.contract, backend=backend,
                                             **options).status
                statuses[c.name] = c.status
        if self.contract is not None:
            self.status = refines(self.composition(discharge, backend=backend, **options),
                                  self.contract, backend=backend, **options).status
            statuses[self.name] = self.status
        return statuses

    def _repr_html_(self) -> str:
        from .viz import notebook
        return notebook(self)
