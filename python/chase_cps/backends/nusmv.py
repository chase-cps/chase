"""NuSMV and nuXmv adapter: implication and satisfiability of LTL formulas.

nuXmv is used with IC3 (``check_ltlspec_ic3``), which proves or refutes the
specification. NuSMV has no IC3: it first looks for a counterexample with
bounded model checking, then proves the specification with BDDs.

Real variables need nuXmv, which checks them with its SMT engine
(``go_msat``, then IC3 or bounded model checking); NuSMV has no real type.
"""

from __future__ import annotations

import os
import re
import subprocess
import tempfile
import time
from fractions import Fraction

from .. import ltl, words
from ..tools import find
from ..queries import (INVALID, SAT, UNKNOWN, UNSAT, VALID, Holds, Implies, Result,
                       Satisfiable, Trace)
from .base import Adapter


class NuSMVAdapter(Adapter):
    """``tool`` is "nuxmv", "nusmv", or "auto" (nuXmv first, then NuSMV)."""

    name = "nusmv"
    answers = (Implies, Satisfiable, Holds)

    def __init__(self, tool: str = "auto", engine: str = "auto",
                 timeout: float = 300.0, bmc_bound: int = 20):
        self.tool = tool
        self.engine = engine
        self.timeout = timeout
        self.bmc_bound = bmc_bound

    # Tool discovery ------------------------------------------------------------

    def binary(self) -> tuple[str, str] | None:
        """(path, flavor) of the model checker, flavor in {"nuxmv", "nusmv"}."""
        candidates = []
        if os.environ.get("CHASE_NUXMV"):
            candidates.append((os.environ["CHASE_NUXMV"], "nuxmv"))
        if os.environ.get("CHASE_NUSMV"):
            candidates.append((os.environ["CHASE_NUSMV"], "nusmv"))
        candidates += [("nuXmv", "nuxmv"), ("NuSMV", "nusmv")]
        for path, flavor in candidates:
            if self.tool not in ("auto", flavor):
                continue
            found = find(path)
            if found:
                return found, flavor
        return None

    def available(self) -> bool:
        return self.binary() is not None

    # emit / run ----------------------------------------------------------------

    def emit(self, query) -> str:
        """The SMV model whose LTLSPEC is valid iff the query holds."""
        if isinstance(query, Holds):
            return query.model.rstrip() + \
                f"\nLTLSPEC {words.to_smv(query.phi, query.variables)};\n"
        if isinstance(query, Implies):
            spec = ltl.Binary("->", query.phi, query.psi)
            variables = query.variables
        elif isinstance(query, Satisfiable):
            spec = ltl.Unary("!", query.phi)   # phi is satisfiable iff !phi is not valid
            variables = query.variables
        else:
            raise TypeError(f"{self.name} does not answer {type(query).__name__}")
        used = ltl.variables(spec)
        lines = ["MODULE main", "VAR"]
        for v, t in variables.items():
            if v in used:
                typ = {"bool": "boolean", "real": "real"}.get(t) or \
                    (words.smv_type(t) if words.word_type(t) else f"{t[0]}..{t[1]}")
                lines.append(f"    {v} : {typ};")
        if len(lines) == 2:   # no variable: NuSMV needs at least one
            lines.append("    chase_dummy : boolean;")
        if _has_reals(query) and not any(variables[v] == "real" for v in used):
            # The SMT engine of nuXmv needs an infinite-state model.
            lines.append("    chase_real : real;")
        lines.append(f"LTLSPEC {words.to_smv(spec, variables)};")
        return "\n".join(lines) + "\n"

    def run(self, query) -> Result:
        found = self.binary()
        if found is None:
            return Result(UNKNOWN, self.name, reason="neither nuXmv nor NuSMV is installed")
        path, flavor = found
        if _has_reals(query):
            if flavor != "nuxmv":
                return Result(UNKNOWN, self.name, reason="real variables need nuXmv: NuSMV "
                              "has no real type")
            if self.engine not in ("auto", "ic3", "bmc"):
                return Result(UNKNOWN, self.name, reason=f"the {self.engine} engine of "
                              "nuXmv does not check real variables; use ic3 or bmc")
        if self.engine != "auto":
            return self._run(query, path, flavor, self.engine, self.timeout)
        if flavor == "nuxmv":
            return self._run(query, path, flavor, "ic3", self.timeout)
        # NuSMV has no IC3: BMC finds counterexamples quickly, BDD proves validity.
        first = self._run(query, path, flavor, "bmc", min(self.timeout, 30.0))
        if first.status in (INVALID, SAT):
            return first
        result = self._run(query, path, flavor, "bdd", max(self.timeout - first.seconds, 1.0))
        result.seconds += first.seconds
        return result

    def _run(self, query, path: str, flavor: str, engine: str, timeout: float) -> Result:
        model = self.emit(query)
        start = time.monotonic()
        with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
            smv = os.path.join(tmp, "query.smv")
            with open(smv, "w") as f:
                f.write(model)
            if flavor == "nuxmv" and _has_reals(query):
                commands = ["set on_failure_script_quits", "read_model -i query.smv",
                            "go_msat"]
                if engine == "bmc":
                    commands.append(f"msat_check_ltlspec_bmc -k {self.bmc_bound}")
                else:
                    commands.append("check_ltlspec_ic3")
                commands.append("quit")
                with open(os.path.join(tmp, "commands"), "w") as f:
                    f.write("\n".join(commands) + "\n")
                args = [path, "-source", "commands"]
            elif flavor == "nuxmv":
                commands = ["set on_failure_script_quits", "read_model -i query.smv",
                            "flatten_hierarchy", "encode_variables", "build_boolean_model"]
                if engine == "ic3":
                    commands.append("check_ltlspec_ic3")
                elif engine == "bmc":
                    commands += ["bmc_setup", f"check_ltlspec_bmc -k {self.bmc_bound}"]
                else:
                    commands += ["build_model", "check_ltlspec"]
                commands.append("quit")
                with open(os.path.join(tmp, "commands"), "w") as f:
                    f.write("\n".join(commands) + "\n")
                args = [path, "-source", "commands"]
            elif engine == "bmc":
                args = [path, "-bmc", "-bmc_length", str(self.bmc_bound), "query.smv"]
            else:
                args = [path, "-dynamic", "-coi", "query.smv"]
            try:
                # No input: after an error nuXmv would wait at its prompt.
                proc = subprocess.run(args, cwd=tmp, capture_output=True, text=True,
                                      stdin=subprocess.DEVNULL, timeout=timeout)
            except subprocess.TimeoutExpired:
                return Result(UNKNOWN, f"{flavor}/{engine}",
                              reason=f"timeout after {timeout:g} s",
                              seconds=time.monotonic() - start)
        seconds = time.monotonic() - start
        backend = f"{flavor}/{engine}"
        verdict = re.search(r"specification .* is (true|false)", proc.stdout, re.S)
        if not verdict:
            reason = _error(proc.stderr + "\n" + proc.stdout)
            if engine == "bmc" and "no counterexample found" in proc.stdout:
                reason = f"no counterexample up to bound {self.bmc_bound}"
            return Result(UNKNOWN, backend, reason=reason or "no verdict in the output",
                          seconds=seconds, details={"model": model})
        holds = verdict.group(1) == "true"
        trace = None if holds else parse_trace(proc.stdout, query.variables)
        if isinstance(query, (Implies, Holds)):
            status = VALID if holds else INVALID
        else:   # Satisfiable: !phi valid means phi unsatisfiable
            status = UNSAT if holds else SAT
        return Result(status, backend, trace=trace, seconds=seconds)


def _error(output: str) -> str:
    """The most specific error message in the output of NuSMV or nuXmv: an
    error with a line number of the model, else any error, else the last line
    (the banners start with ***)."""
    lines = [l.strip() for l in output.splitlines() if l.strip() and not l.startswith("***")]
    errors = [l for l in lines if re.search(r"error", l, re.I)
              or re.match(r"file \S+: line \d+", l)]
    located = [l for l in errors if re.search(r"line \d+", l)]
    return (located or errors or lines or [""])[-1]


def _has_reals(query) -> bool:
    """Real variables, or a number with a fraction: only the SMT engine of
    nuXmv reads them (its finite-state engines reject 20.5 <= n)."""
    if "real" in query.variables.values():
        return True
    if isinstance(query, Implies):
        return ltl.has_fraction(query.phi) or ltl.has_fraction(query.psi)
    return ltl.has_fraction(query.phi)


_FRACTION = re.compile(r"^(-?)f'(-?\d+)/(\d+)$")


def _value(text: str):
    """A value printed by NuSMV or nuXmv: a Boolean, an int, or for a real
    an exact number (``f'1/3`` and ``2.5`` are read as fractions)."""
    text = text.strip()
    if text == "TRUE":
        return True
    if text == "FALSE":
        return False
    w = words.value(text)
    if w is not None:
        return w
    m = _FRACTION.match(text)
    if m:
        sign = -1 if m.group(1) else 1
        return ltl.number(sign * Fraction(int(m.group(2)), int(m.group(3))))
    try:
        return ltl.number(text)
    except ValueError:
        return text


def parse_trace(output: str, variables: dict) -> Trace | None:
    """Parse the first counterexample printed by NuSMV or nuXmv."""
    start = output.find("Trace Type: Counterexample")
    if start < 0:
        return None
    states: list[dict] = []
    loop_at = None
    current: dict = {}
    for line in output[start:].splitlines()[1:]:
        line = line.strip()
        if line.startswith("-- Loop starts here"):
            loop_at = len(states)
        elif line.startswith("-> State:"):
            current = dict(states[-1]) if states else {}
            states.append(current)
        elif line.startswith("-> Input:"):
            continue
        elif "=" in line and states and not line.startswith("--"):
            name, value = line.split("=", 1)
            name = name.strip()
            if name in variables or not variables:
                current[name] = _value(value)
        elif line.startswith("-- ") or line.startswith("nuXmv >") or line == "":
            if states and line.startswith("-- specification"):
                break
    if not states:
        return None
    if loop_at is None:
        return Trace(prefix=states, loop=[])
    # The last state closes the loop: it equals the state where the loop starts.
    loop = states[loop_at:]
    if len(loop) > 1 and loop[-1] == loop[0]:
        loop = loop[:-1]
    return Trace(prefix=states[:loop_at], loop=loop)
