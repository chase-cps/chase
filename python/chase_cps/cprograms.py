"""C functions as implementations of contracts, checked with ESBMC or CBMC.

    from chase_cps import CFunction, is_implementation, to_c
    f = CFunction("saturate.c", "saturate")
    r = is_implementation(f, contract)          # ESBMC, else CBMC
    open("check_saturate.c", "w").write(to_c(contract, f))

A function is a component that answers once: its parameters are the inputs
of the contract, by name, and its return value is the output (or the
output named by ``result``); a pointer parameter named as an output is
written by the function. The formulas of the contract say something about
one call: no temporal operator, no x', no past(x, n).

The check writes a C program (``to_c``) that calls the function with any
values of the inputs, within their types and the assumptions, and asserts
the guarantees on the outputs; then ESBMC or CBMC checks it. They also
check the function itself (an arithmetic overflow, a division by zero, an
access out of bounds): a failure there makes the result invalid, with the
property in the reason. Loops are unwound up to ``unwind`` times (20); a
loop that needs more makes the result unknown.

The arithmetic of the formulas is that of the integers (in long long) and
of the reals in double: a real variable is a double or a float of the
function, and its formulas are computed with rounding. Words are not
written yet.
"""

from __future__ import annotations

import os
import re
from dataclasses import dataclass
from fractions import Fraction
from typing import Optional

from . import ltl
from .contract import Contract


class CProgramError(ValueError):
    """The function or the contract cannot be checked as C."""


@dataclass
class CFunction:
    """A function of a C file: ``path`` and ``name``. ``source`` gives the
    text of the file instead of reading it; ``result`` names the output that
    is the return value (by default the only output not passed as a
    pointer)."""
    path: str
    name: str
    result: Optional[str] = None
    source: Optional[str] = None

    def text(self) -> str:
        if self.source is not None:
            return self.source
        with open(self.path) as f:
            return f.read()

    def signature(self) -> "Signature":
        return _signature(self.text(), self.name)


@dataclass
class Parameter:
    name: str
    type: str          # the C type, without the * of a pointer
    pointer: bool


@dataclass
class Signature:
    returns: str
    parameters: list
    declaration: str


_NONDET = {"int": "int", "unsigned int": "uint", "unsigned": "uint", "char": "char",
           "signed char": "char", "unsigned char": "uchar", "short": "short",
           "short int": "short", "unsigned short": "ushort", "long": "long",
           "long int": "long", "unsigned long": "ulong", "long long": "longlong",
           "unsigned long long": "ulonglong", "_Bool": "bool", "bool": "bool",
           "float": "float", "double": "double",
           "int8_t": "char", "uint8_t": "uchar", "int16_t": "short", "uint16_t": "ushort",
           "int32_t": "int", "uint32_t": "uint", "int64_t": "longlong", "uint64_t": "ulonglong"}
_FLOATS = ("float", "double")


def _signature(text: str, name: str) -> Signature:
    """The signature of the definition of the function in the text."""
    text = re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)
    m = re.search(r"(?:^|[;}\s])((?:[A-Za-z_]\w*[\s*]+)+?)" + re.escape(name) +
                  r"\s*\(([^()]*)\)\s*\{", text)
    if not m:
        raise CProgramError(f"no definition of the function {name}")
    returns = " ".join(m.group(1).replace("static", " ").replace("inline", " ").split())
    params = []
    body = m.group(2).strip()
    if body and body != "void":
        for p in body.split(","):
            p = " ".join(p.replace("const", " ").split())
            pm = re.match(r"^(.*?)\s*(\*?)\s*([A-Za-z_]\w*)$", p)
            if not pm or not pm.group(1):
                raise CProgramError(f"{name}: cannot read the parameter {p!r}")
            params.append(Parameter(pm.group(3), " ".join(pm.group(1).replace("*", " ").split()),
                                    bool(pm.group(2)) or "*" in pm.group(1)))
    return Signature(returns, params, f"{m.group(1).strip()} {name}({m.group(2).strip()})")


def _static(contract: Contract) -> None:
    from .backends.z3 import static
    for t in list(contract.assumptions) + list(contract.guarantees):
        if not static(t):
            raise CProgramError(f"{ltl.to_str(t)}: a C function answers once; its formulas "
                                "have no temporal operator, no x', no past(x, n)")
    from .words import word_type
    words = sorted(v for v, t in contract.variables.items() if word_type(t))
    if words:
        raise CProgramError(f"words are not written in C yet: {', '.join(words)}")


def roles(contract: Contract, function: CFunction) -> tuple:
    """(signature, the output that is the return value or None): each
    parameter must be an input of the contract, or a pointer to an output."""
    sig = function.signature()
    for p in sig.parameters:
        if p.pointer and p.name not in contract.outputs:
            raise CProgramError(f"{function.name}: the pointer parameter {p.name} is not an "
                                f"output of the contract {contract.name}")
        if not p.pointer and p.name not in contract.inputs:
            raise CProgramError(f"{function.name}: the parameter {p.name} is not an input of "
                                f"the contract {contract.name}")
        if p.type not in _NONDET:
            raise CProgramError(f"{function.name}: the type {p.type} of {p.name} is not "
                                "supported")
    missing = [v for v in contract.inputs if v not in {p.name for p in sig.parameters}]
    if missing:
        raise CProgramError(f"{function.name} has no parameter {', '.join(missing)}")
    left = [v for v in contract.outputs if v not in {p.name for p in sig.parameters}]
    result = function.result or (left[0] if len(left) == 1 else None)
    if len(left) > 1 and function.result is None:
        raise CProgramError(f"{function.name}: the outputs {', '.join(left)} are not "
                            "parameters; name the return value with result=")
    if result is not None:
        if sig.returns == "void":
            raise CProgramError(f"{function.name} returns nothing; {result} is not a parameter")
        if result not in contract.outputs:
            raise CProgramError(f"result={result} is not an output of {contract.name}")
        if sig.returns not in _NONDET:
            raise CProgramError(f"{function.name}: the type {sig.returns} it returns is not "
                                "supported")
    return sig, result


def to_c(contract: Contract, function: CFunction) -> str:
    """The C program that checks that the function is an implementation of
    the contract: give it with the file of the function to ESBMC or CBMC."""
    _static(contract)
    sig, result = roles(contract, function)
    types = {p.name: p.type for p in sig.parameters}
    if result is not None:
        types[result] = sig.returns
    w = _Writer(contract.variables, types)
    inputs_only = ltl.variables(contract.assumption) <= set(contract.inputs)
    nondets = sorted({_NONDET[t] for n, t in types.items() if n in contract.inputs or
                      any(p.name == n and p.pointer for p in sig.parameters)})
    file = os.path.basename(function.path)
    lines = [f"/* Contract {contract.name}, implemented by {function.name} ({file}): written",
             "   by CHASE (chase_cps.to_c). Run: esbmc THIS_FILE " + file +
             " --overflow-check --unwind 20",
             f"   or: cbmc THIS_FILE {file} --unwind 20 --unwinding-assertions */",
             "#include <assert.h>"]
    lines += [l.strip() for l in function.text().splitlines()      # the types of the function
              if re.match(r"\s*#\s*include\s*<", l) and "assert.h" not in l]
    lines += ["", sig.declaration + ";"]
    lines += [f"{_c_type(k)} __VERIFIER_nondet_{k}(void);" for k in nondets]
    lines += ["void __VERIFIER_assume(int);", "", "int main(void) {"]
    for p in sig.parameters:
        lines.append(f"    {p.type} {p.name} = __VERIFIER_nondet_{_NONDET[p.type]}();")
    domain = [w.domain(v) for v in contract.inputs]
    domain = [d for d in domain if d]
    if domain:
        lines.append(f"    __VERIFIER_assume({' && '.join(domain)});   /* the types of the inputs */")
    if inputs_only:
        lines.append(f"    __VERIFIER_assume({w.formula(contract.assumption)});   /* the assumptions */")
    args = ", ".join(("&" if p.pointer else "") + p.name for p in sig.parameters)
    call = f"{function.name}({args});"
    lines.append(f"    {sig.returns} {result} = {call}" if result is not None else f"    {call}")
    for p in sig.parameters:          # the outputs in the trace of CBMC
        if p.pointer:
            lines.append(f"    {p.type} chase_out_{p.name} = {p.name};")
    checks = [d for d in (w.domain(v) for v in contract.outputs) if d]
    checks.append(w.formula(contract.guarantee))
    g = " && ".join(f"({c})" for c in checks) if len(checks) > 1 else checks[0]
    if inputs_only:
        lines.append(f"    assert({g});   /* the guarantees */")
    else:
        lines.append(f"    assert(!({w.formula(contract.assumption)}) || {g});   "
                     "/* the assumptions imply the guarantees */")
    lines += ["    return 0;", "}"]
    return "\n".join(lines) + "\n"


def _c_type(kind: str) -> str:
    return {"int": "int", "uint": "unsigned int", "char": "char", "uchar": "unsigned char",
            "short": "short", "ushort": "unsigned short", "long": "long",
            "ulong": "unsigned long", "longlong": "long long",
            "ulonglong": "unsigned long long", "bool": "_Bool", "float": "float",
            "double": "double"}[kind]


class _Writer:
    """Formulas as C expressions: integers in long long, reals in double."""

    def __init__(self, variables: dict, ctypes: dict):
        self.variables = variables
        self.ctypes = ctypes

    def domain(self, v: str) -> str:
        t = self.variables[v]
        if isinstance(t, tuple):
            return f"{self._var(v)} >= {t[0]}LL && {self._var(v)} <= {t[1]}LL"
        if t == "bool" and self.ctypes.get(v) not in ("_Bool", "bool"):
            return f"({v} == 0 || {v} == 1)"
        return ""

    def _var(self, v: str) -> str:
        t = self.variables.get(v, "bool")
        if t == "bool":
            return f"({v} != 0)"
        if t == "real":
            return f"((double){v})"
        return f"((long long){v})"

    def formula(self, node) -> str:
        if isinstance(node, ltl.Const):
            return "1" if node.value else "0"
        if isinstance(node, ltl.Num):
            f = Fraction(node.value)
            if f.denominator == 1:
                return f"{f.numerator}LL"
            return f"({f.numerator}.0 / {f.denominator}.0)"
        if isinstance(node, ltl.Var):
            return self._var(node.name)
        if isinstance(node, ltl.Unary):
            a = self.formula(node.arg)
            if node.op == "!":
                return f"!{a}"
            if node.op == "neg":
                return f"(-{a})"
            raise CProgramError(f"{ltl.to_str(node)} cannot be written in C")
        a, b = self.formula(node.left), self.formula(node.right)
        op = node.op
        c = {"&": "&&", "|": "||", "=": "==", "!=": "!=", "<": "<", "<=": "<=", ">": ">",
             ">=": ">=", "+": "+", "-": "-", "*": "*", "/": "/", "mod": "%"}
        if op == "->":
            return f"(!{a} || {b})"
        if op == "<->":
            return f"(!{a} == !{b})"
        if op == "xor":
            return f"(!{a} != !{b})"
        if op not in c:
            raise CProgramError(f"the operator {op} cannot be written in C")
        return f"({a} {c[op]} {b})"
