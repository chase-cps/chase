"""ESBMC and CBMC: bounded model checkers of C programs.

They answer ``ImplementsC``: is a C function an implementation of a
contract? The program of ``chase_cps.cprograms.to_c`` calls the function
with any inputs allowed and asserts the guarantees; the checker proves the
assertion, or gives the values of a call that breaks it, which are the
trace (one state, repeated). A failure of another property (an overflow, a
division by zero, a loop unwound too few times) is in the reason.
"""

from __future__ import annotations

import json
import os
import re
import subprocess
import tempfile
import time

from ..queries import INVALID, UNKNOWN, VALID, ImplementsC, Result, Trace

GUARANTEES = "the guarantees"
from .base import Adapter


class BMCAdapter(Adapter):
    """ESBMC (tool="esbmc") or CBMC (tool="cbmc")."""

    answers = (ImplementsC,)

    def __init__(self, tool: str, timeout: float = 300.0, unwind: int = 20):
        self.name = tool
        self.timeout = timeout
        self.unwind = unwind

    def binary(self):
        from ..tools import find
        return find(os.environ.get(f"CHASE_{self.name.upper()}", self.name))

    def available(self) -> bool:
        return self.binary() is not None

    def emit(self, query: ImplementsC) -> str:
        from ..cprograms import to_c
        return to_c(query.contract, query.function)

    def run(self, query: ImplementsC) -> Result:
        from ..cprograms import CProgramError
        try:
            program = self.emit(query)
        except (CProgramError, OSError) as e:
            return Result(UNKNOWN, self.name, reason=str(e))
        start = time.monotonic()
        with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
            check = os.path.join(tmp, "chase_check.c")
            source = os.path.join(tmp, os.path.basename(query.function.path) or "function.c")
            with open(check, "w") as f:
                f.write(program)
            with open(source, "w") as f:
                f.write(query.function.text())
            args = [self.binary(), check, source, "--unwind", str(self.unwind)]
            if self.name == "esbmc":      # unwinding assertions by default
                args += ["--overflow-check"]
            else:
                args += ["--unwinding-assertions", "--json-ui"]
            try:
                proc = subprocess.run(args, capture_output=True, text=True, cwd=tmp,
                                      stdin=subprocess.DEVNULL, timeout=self.timeout)
            except subprocess.TimeoutExpired:
                return Result(UNKNOWN, self.name, reason=f"timeout after {self.timeout:g} s",
                              seconds=time.monotonic() - start)
            out, err = proc.stdout, proc.stderr
            for d in (os.path.realpath(tmp), tmp):        # the files by their names
                out, err = out.replace(d + os.sep, ""), err.replace(d + os.sep, "")
        seconds = time.monotonic() - start
        if self.name == "esbmc":
            found = _esbmc(query, out + err)
        else:
            found = _cbmc(query, out, err)
        if isinstance(found, str):
            return Result(UNKNOWN, self.name, reason=found, seconds=seconds)
        return self._verdict(query, *found, seconds)

    def _verdict(self, query, failed: list, state: dict, seconds: float) -> Result:
        """failed: the properties that fail, GUARANTEES for the assertion of
        the contract; state: the values of the call."""
        if not failed:
            return Result(VALID, self.name, seconds=seconds)
        if GUARANTEES not in failed and all("unwinding" in f for f in failed):
            return Result(UNKNOWN, self.name, reason=f"a loop runs more than unwind="
                          f"{self.unwind} times: {failed[0]}", seconds=seconds)
        trace = Trace(prefix=[], loop=[{v: state[v] for v in _variables(query) if v in state}])
        others = [f for f in failed if f != GUARANTEES]
        reason = f"{query.function.name}: " + "; ".join(others) if others else ""
        return Result(INVALID, self.name, trace=trace, reason=reason, seconds=seconds,
                      details={"failed": failed})


def _variables(query: ImplementsC) -> dict:
    return query.contract.variables


def _number(text: str, kind, bits: str = ""):
    """A value of the trace; a real from its bits when they are given (the
    checkers print doubles rounded)."""
    text = text.strip()
    if kind == "bool":
        return text not in ("0", "false", "FALSE")
    bits = bits.replace(" ", "")
    if kind == "real" and len(bits) in (32, 64):
        import struct
        from fractions import Fraction
        from .. import ltl
        raw = int(bits, 2).to_bytes(len(bits) // 8, "big")
        value = struct.unpack(">d" if len(bits) == 64 else ">f", raw)[0]
        if value == value and abs(value) != float("inf"):
            return ltl.number(Fraction(value))
    try:
        return int(text)
    except ValueError:
        pass
    try:
        from fractions import Fraction
        from .. import ltl
        return ltl.number(Fraction(text))
    except (ValueError, ZeroDivisionError):
        return text


_STATE = re.compile(r"^\s+(\*?[A-Za-z_]\w*) = (.*?)(?: \(([01 ]+)\))?$")


def _name(variable: str) -> str:
    """The variable of the contract that a variable of the program shows."""
    return variable[len("chase_out_"):] if variable.startswith("chase_out_") else variable


def _esbmc(query, out: str):
    """(failed, state), or the reason of no answer."""
    if "VERIFICATION SUCCESSFUL" in out:
        return [], {}
    if "VERIFICATION FAILED" not in out:
        return "esbmc: " + "\n".join(out.strip().splitlines()[-5:])
    variables = _variables(query)
    state, in_main = {}, False
    for line in out.splitlines():
        if line.startswith("State "):
            in_main = "function main" in line
            continue
        m = _STATE.match(line)
        if not m:
            continue
        name = _name(m.group(1).lstrip("*"))
        # main sets the inputs and the return value, the function the
        # outputs passed by pointer (*heat = 1)
        if name in variables and (in_main or m.group(1).startswith("*")):
            state[name] = _number(m.group(2), variables[name], m.group(3) or "")
    failed = []
    for block in re.findall(r"Violated property:\n(.*?)(?:\n\n|\Z)", out, re.S):
        lines = [l.strip() for l in block.splitlines() if l.strip()]
        where, what = lines[0], (lines[1] if len(lines) > 1 else "")
        if "function main" in where and what.startswith("assertion"):
            failed.append(GUARANTEES)
        else:
            failed.append(f"{what} ({where})")
    return failed, state


def _cbmc(query, out: str, err: str):
    """(failed, state), or the reason of no answer."""
    try:
        messages = json.loads(out)
    except ValueError:
        return f"cbmc: {(err or out).strip()[-300:]}"
    variables = _variables(query)
    failed, state = [], {}
    for m in messages:
        for r in m.get("result", []):
            if r.get("status") != "FAILURE":
                continue
            prop = r.get("property", "")
            failed.append(GUARANTEES if prop.startswith("main.assertion")
                          else f"{r.get('description', prop)} ({prop})")
            for s in r.get("trace", []):
                name = _name(s.get("lhs", ""))
                if s.get("stepType") == "assignment" and name in variables \
                        and not s.get("hidden") and s.get("function", {}).get("identifier",
                                                                               "main") == "main":
                    value = s["value"]
                    state[name] = _number(str(value.get("data", "")), variables[name],
                                          value.get("binary", ""))
        if m.get("messageType") == "ERROR":
            return f"cbmc: {m.get('messageText')}"
    return failed, state
