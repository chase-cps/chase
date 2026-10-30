"""A contract as the input of gr1c, the GR(1) solver of S. Livingston
(https://github.com/tulip-control/gr1c), for users who run it themselves.

    from chase_cps import to_gr1c
    open("spec.spc", "w").write(to_gr1c(contract))
    # gr1c -r spec.spc     (realizability)
    # gr1c spec.spc        (a controller)

The inputs of the contract are the variables of the environment (ENV), its
outputs those of the system (SYS). Each term goes to the section of its
GR(1) class: an initial term to ENVINIT or SYSINIT, G f to ENVTRANS or
SYSTRANS, G F f to ENVGOAL or SYSGOAL, as for slugs. gr1c compares an
integer only with a constant, and its domains start at 0: an integer
variable of the range (low, high) is written over [0, high - low], and a
relation with arithmetic or between variables is written as the values
that satisfy it.
"""

from __future__ import annotations

import itertools
from fractions import Fraction

from . import ltl
from .contract import Contract

MAX_VALUES = 4096     # the assignments a relation may expand to


class Gr1cError(ValueError):
    """The contract cannot be written for gr1c."""


def to_gr1c(contract: Contract) -> str:
    """The specification of the contract in the input language of gr1c."""
    from .gr1 import gr1_kind, gr1_roles
    problem = gr1_roles(contract)
    if problem:
        raise Gr1cError(f"contract {contract.name}: {problem}")
    domains = {}
    for name, kind in {**contract.inputs, **contract.outputs}.items():
        if kind == "bool":
            domains[name] = None
        elif isinstance(kind, tuple) and len(kind) == 2:
            domains[name] = (int(kind[0]), int(kind[1]))
        else:
            raise Gr1cError(f"contract {contract.name}: {name} is {kind}; gr1c takes Booleans "
                            "and bounded integers")
    write = _Writer(domains)
    sections = {s: [] for s in ("ENVINIT", "SYSINIT", "ENVTRANS", "SYSTRANS", "ENVGOAL", "SYSGOAL")}
    for side, terms in (("ENV", contract.assumptions), ("SYS", contract.guarantees)):
        for t in terms:
            kind = gr1_kind(t)
            if kind == "init":
                sections[side + "INIT"].append(write(t))
            elif kind == "safety":
                sections[side + "TRANS"].append("[](" + write(t.arg) + ")")
            else:
                sections[side + "GOAL"].append("[]<>(" + write(t.arg.arg) + ")")

    def declare(names):
        return "".join(f"\n\t{n}" if domains[n] is None else
                       f"\n\t{n} [0, {domains[n][1] - domains[n][0]}]" for n in sorted(names))

    out = [f"# contract {contract.name}, written by chase-cps for gr1c"]
    shifted = [f"{n} = value - {lo}" for n, d in sorted(domains.items()) if d and (lo := d[0])]
    if shifted:
        out.append("# integers written from 0: " + ", ".join(shifted))
    out += ["", "ENV:" + declare(contract.inputs) + ";", "SYS:" + declare(contract.outputs) + ";", ""]
    for name, terms in sections.items():
        if terms:
            out.append(f"{name}:\n\t" + "\n\t& ".join(terms) + ";\n")
        elif name.endswith("INIT"):
            out.append(f"{name}: True;\n")
    return "\n".join(out)


class _Writer:
    def __init__(self, domains):
        self.domains = domains

    def __call__(self, node, nxt=False) -> str:
        if isinstance(node, ltl.Const):
            return "True" if node.value else "False"
        if isinstance(node, ltl.Var):
            if self.domains.get(node.name, 0) is not None:
                raise Gr1cError(f"{node.name} is not a Boolean variable")
            if node.primed and nxt:
                raise Gr1cError(f"{node.name}' inside X: gr1c reads one step ahead only")
            return node.name + ("'" if node.primed or nxt else "")
        if isinstance(node, ltl.Unary):
            if node.op == "!":
                return "!(" + self(node.arg, nxt) + ")"
            if node.op == "X":
                if nxt:
                    raise Gr1cError("X inside X: gr1c reads one step ahead only")
                return self(node.arg, True)
            raise Gr1cError(f"gr1c has no {node.op}")
        if isinstance(node, ltl.Binary):
            if node.op in ("&", "|", "->", "<->"):
                return "(" + self(node.left, nxt) + " " + node.op + " " + self(node.right, nxt) + ")"
            if node.op in ltl.RELATIONS:
                return self.relation(node, nxt)
        raise Gr1cError(f"cannot write {ltl.to_str(node)} for gr1c")

    def relation(self, node, nxt) -> str:
        """A relation over integers, as the values that satisfy it."""
        used = sorted(self.atoms(node, nxt))
        for name, primed in used:
            if self.domains.get(name) is None:
                raise Gr1cError(f"{name} is not an integer variable, in {ltl.to_str(node)}")
        ranges = [range(self.domains[n][0], self.domains[n][1] + 1) for n, _ in used]
        size = 1
        for r in ranges:
            size *= len(r)
        if size > MAX_VALUES:
            raise Gr1cError(f"{ltl.to_str(node)} has {size} assignments to write for gr1c")
        held = []
        for values in itertools.product(*ranges):
            env = dict(zip(used, values))
            if _value(node, env, nxt):
                held.append(" & ".join(f"{n}{chr(39) if p else ''}={v - self.domains[n][0]}"
                                       for (n, p), v in zip(used, values)))
        if not held:
            return "False"
        if len(held) == 1:
            return "(" + held[0] + ")"
        return "(" + " | ".join("(" + h + ")" for h in held) + ")"

    def atoms(self, node, nxt) -> set:
        if isinstance(node, ltl.Var):
            if node.primed and nxt:
                raise Gr1cError(f"{node.name}' inside X: gr1c reads one step ahead only")
            return {(node.name, node.primed or nxt)}
        if isinstance(node, ltl.Unary):
            return self.atoms(node.arg, nxt or node.op == "X")
        if isinstance(node, ltl.Binary):
            return self.atoms(node.left, nxt) | self.atoms(node.right, nxt)
        return set()


def _value(node, env, nxt):
    if isinstance(node, ltl.Num):
        return Fraction(node.value)
    if isinstance(node, ltl.Var):
        return env[(node.name, node.primed or nxt)]
    if isinstance(node, ltl.Unary):
        if node.op == "neg":
            return -_value(node.arg, env, nxt)
        if node.op == "X":
            return _value(node.arg, env, True)
        raise Gr1cError(f"gr1c has no {node.op} in a relation")
    a, b = _value(node.left, env, nxt), _value(node.right, env, nxt)
    op = node.op
    if op == "+":
        return a + b
    if op == "-":
        return a - b
    if op == "*":
        return a * b
    if op in ("/", "mod"):
        raise Gr1cError(f"{op} is not written for gr1c: divide by hand")
    return {"=": a == b, "!=": a != b, "<": a < b, "<=": a <= b, ">": a > b, ">=": a >= b}[op]
