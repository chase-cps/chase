"""NuSMV and nuXmv adapter: implication and satisfiability of LTL formulas.

nuXmv is used with IC3 (``check_ltlspec_ic3``), which proves or refutes the
specification. NuSMV has no IC3: it first looks for a counterexample with
bounded model checking, then proves the specification with BDDs.

Real variables need nuXmv, which checks them with its SMT engine
(``go_msat``, then IC3 or bounded model checking); NuSMV has no real type.

The option ``engine`` picks another engine: "ic3", "bdd" (BDDs, fast when a
counterexample needs a long loop, slow on large state spaces), "bmc"
(bounded model checking: counterexamples up to ``bmc_bound`` steps, no
proofs), or "portfolio" (IC3 and BDDs at the same time; the first verdict
wins).
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


# auto: IC3 with nuXmv; with NuSMV, BMC for a counterexample, then BDDs.
# portfolio: two engines at the same time, the first verdict wins.
ENGINES = ("auto", "ic3", "bdd", "bmc", "portfolio")


class NuSMVAdapter(Adapter):
    """``tool`` is "nuxmv", "nusmv", or "auto" (nuXmv first, then NuSMV);
    ``engine`` is one of ENGINES."""

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
        """The answer; variables named as keywords of SMV are renamed in the
        model (and back in the trace)."""
        query, back = _rename(query)
        result = self._answer(query)
        if back and result.trace is not None:
            for states in (result.trace.prefix, result.trace.loop):
                for st in states:
                    for new, old in back.items():
                        if new in st:
                            st[old] = st.pop(new)
        return result

    def _answer(self, query) -> Result:
        if self.engine not in ENGINES:
            raise ValueError(f"unknown engine {self.engine!r}; known: {list(ENGINES)}")
        found = self.binary()
        if found is None:
            return Result(UNKNOWN, self.name, reason="neither nuXmv nor NuSMV is installed")
        path, flavor = found
        reals = _has_reals(query)
        if reals:
            if flavor != "nuxmv":
                return Result(UNKNOWN, self.name, reason="real variables need nuXmv: NuSMV "
                              "has no real type")
            if self.engine not in ("auto", "ic3", "bmc", "portfolio"):
                return Result(UNKNOWN, f"{flavor}/{self.engine}", reason=f"the "
                              f"{self.engine} engine of nuXmv does not check real "
                              "variables; use ic3 or bmc")
        engine = self.engine
        if flavor == "nuxmv" and not reals and _casts(query):
            # IC3 of nuXmv 2.2.0 gives wrong answers with signed() and unsigned():
            # G X (s < signed(x)) -> X G (s < signed(x)) is false for it. BDDs are right.
            if engine == "ic3":
                return Result(UNKNOWN, "nuxmv/ic3", reason="IC3 of nuXmv gives wrong answers "
                              "with signed() and unsigned(); use bdd")
            if engine in ("auto", "portfolio"):
                return self._run(query, path, flavor, "bdd", self.timeout)
        if flavor == "nusmv" and engine == "ic3":
            return Result(UNKNOWN, "nusmv/ic3", reason="NuSMV has no IC3; use bdd or bmc, "
                          "or install nuXmv")
        if self.engine == "portfolio":
            engines = ("ic3", "bmc") if reals else \
                ("ic3", "bdd") if flavor == "nuxmv" else ("bdd", "bmc")
            return self._portfolio(query, path, flavor, engines)
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

    def _command(self, query, path: str, flavor: str, engine: str, tmp: str) -> list[str]:
        """Write the model (and the commands for nuXmv) in tmp; the command line."""
        with open(os.path.join(tmp, "query.smv"), "w") as f:
            f.write(self.emit(query))
        if flavor == "nuxmv" and _has_reals(query):
            commands = ["set on_failure_script_quits", "read_model -i query.smv", "go_msat"]
            if engine == "bmc":
                commands.append(f"msat_check_ltlspec_bmc -k {self.bmc_bound}")
            else:
                commands.append("check_ltlspec_ic3")
        elif flavor == "nuxmv":
            commands = ["set on_failure_script_quits", "read_model -i query.smv",
                        "flatten_hierarchy", "encode_variables", "build_boolean_model"]
            if engine == "ic3":
                commands.append("check_ltlspec_ic3")
            elif engine == "bmc":
                commands += ["bmc_setup", f"check_ltlspec_bmc -k {self.bmc_bound}"]
            else:
                commands += ["build_model", "check_ltlspec"]
        elif engine == "bmc":
            return [path, "-bmc", "-bmc_length", str(self.bmc_bound), "query.smv"]
        else:
            return [path, "-dynamic", "-coi", "query.smv"]
        with open(os.path.join(tmp, "commands"), "w") as f:
            f.write("\n".join(commands + ["quit"]) + "\n")
        return [path, "-source", "commands"]

    def _run(self, query, path: str, flavor: str, engine: str, timeout: float) -> Result:
        start = time.monotonic()
        with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
            args = self._command(query, path, flavor, engine, tmp)
            try:
                # No input: after an error nuXmv would wait at its prompt.
                proc = subprocess.run(args, cwd=tmp, capture_output=True, text=True,
                                      stdin=subprocess.DEVNULL, timeout=timeout)
            except subprocess.TimeoutExpired:
                return Result(UNKNOWN, f"{flavor}/{engine}",
                              reason=f"timeout after {timeout:g} s",
                              seconds=time.monotonic() - start)
        return self._verdict(query, flavor, engine, proc.stdout, proc.stderr,
                             time.monotonic() - start)

    def _portfolio(self, query, path: str, flavor: str, engines) -> Result:
        """Run the engines at the same time; the first verdict wins and the
        other runs are stopped."""
        start = time.monotonic()
        with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
            running = {}
            for engine in engines:
                d = os.path.join(tmp, engine)
                os.mkdir(d)
                args = self._command(query, path, flavor, engine, d)
                out = open(os.path.join(d, "out"), "w")
                err = open(os.path.join(d, "err"), "w")
                proc = subprocess.Popen(args, cwd=d, stdout=out, stderr=err,
                                        stdin=subprocess.DEVNULL)
                running[engine] = (proc, d, out, err)
            unknown = []
            try:
                while running and time.monotonic() - start < self.timeout:
                    done = [e for e, (p, *_) in running.items() if p.poll() is not None]
                    if not done:
                        time.sleep(0.01)
                        continue
                    for engine in done:
                        proc, d, out, err = running.pop(engine)
                        out.close()
                        err.close()
                        with open(os.path.join(d, "out")) as f, \
                                open(os.path.join(d, "err")) as g:
                            result = self._verdict(query, flavor, engine, f.read(),
                                                   g.read(), time.monotonic() - start)
                        if result.status != UNKNOWN:
                            return result
                        unknown.append(f"{engine}: {result.reason}")
            finally:
                for proc, d, out, err in running.values():
                    proc.kill()
                    proc.wait()
                    out.close()
                    err.close()
        if running:
            unknown += [f"{e}: timeout after {self.timeout:g} s" for e in running]
        return Result(UNKNOWN, f"{flavor}/portfolio", reason="; ".join(unknown),
                      seconds=time.monotonic() - start)

    def _verdict(self, query, flavor: str, engine: str, stdout: str, stderr: str,
                 seconds: float) -> Result:
        backend = f"{flavor}/{engine}"
        verdict = re.search(r"specification .* is (true|false)", stdout, re.S)
        if not verdict:
            reason = _error(stderr + "\n" + stdout)
            if engine == "bmc" and "no counterexample found" in stdout:
                reason = f"no counterexample up to bound {self.bmc_bound}"
            return Result(UNKNOWN, backend, reason=reason or "no verdict in the output",
                          seconds=seconds, details={"model": self.emit(query)})
        holds = verdict.group(1) == "true"
        trace = None if holds else parse_trace(stdout, query.variables)
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


# Keywords and functions of NuSMV and nuXmv, which cannot name a variable.
SMV_KEYWORDS = frozenset("""
MODULE DEFINE MDEFINE CONSTANTS VAR IVAR FROZENVAR INIT TRANS INVAR SPEC CTLSPEC LTLSPEC
PSLSPEC COMPUTE NAME INVARSPEC FAIRNESS JUSTICE COMPASSION ISA ASSIGN CONSTRAINT SIMPWFF
CTLWFF LTLWFF PSLWFF COMPWFF IN MIN MAX MIRROR PRED PREDICATES process array of boolean
integer real word word1 bool signed unsigned extend resize sizeof uwconst swconst EX AX EF
AF EG AG E F O G H X Y Z A U S V T BU EBF ABF EBG ABG case esac mod next init union in xor
xnor self TRUE FALSE count toint floor abs max min READ WRITE CONSTARRAY typeof pi ln exp
sin cos tan asin acos atan pow sqrt clock time
""".split())


def _rename(query):
    """The query with the variables that are SMV keywords renamed, and the
    map back. Models (Holds) are written by the user and kept."""
    if isinstance(query, Holds):
        return query, {}
    clash = {v for v in query.variables if v in SMV_KEYWORDS}
    if not clash:
        return query, {}
    taken = set(query.variables)
    names = {}
    for v in sorted(clash):
        new = f"chase_{v}"
        while new in taken:
            new += "_"
        taken.add(new)
        names[v] = new

    def sub(node):
        if isinstance(node, ltl.Var) and node.name in names:
            return ltl.Var(names[node.name], node.primed)
        if isinstance(node, ltl.Unary):
            return ltl.Unary(node.op, sub(node.arg))
        if isinstance(node, ltl.Binary):
            return ltl.Binary(node.op, sub(node.left), sub(node.right))
        return node
    import dataclasses
    variables = {names.get(v, v): t for v, t in query.variables.items()}
    fields = {"phi": sub(query.phi), "variables": variables}
    if isinstance(query, Implies):
        fields["psi"] = sub(query.psi)
    return dataclasses.replace(query, **fields), {new: old for old, new in names.items()}


def _casts(query) -> bool:
    """Whether a formula of the query reads a word as signed or unsigned."""
    def walk(node):
        if isinstance(node, ltl.Unary):
            return ltl.function(node) in ("signed", "unsigned") or walk(node.arg)
        if isinstance(node, ltl.Binary):
            return walk(node.left) or walk(node.right)
        return False
    if isinstance(query, Holds):
        return walk(query.phi) or bool(re.search(r"\b(signed|unsigned)\s*\(", query.model))
    return walk(query.phi) or (isinstance(query, Implies) and walk(query.psi))


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
