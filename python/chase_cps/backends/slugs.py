"""Slugs adapter: GR(1) realizability, controller, counter-strategy."""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
import time

from .. import ltl
from ..queries import REALIZABLE, UNKNOWN, UNREALIZABLE, Realizable, Result
from ..tools import find
from .base import Adapter


class SlugsAdapter(Adapter):
    """``controller``: also extract the controller when realizable;
    ``counter_strategy``: also extract the counter-strategy of the environment
    when unrealizable."""

    name = "slugs"
    answers = (Realizable,)

    def __init__(self, controller: bool = False, counter_strategy: bool = True,
                 timeout: float = 300.0):
        self.controller = controller
        self.counter_strategy = counter_strategy
        self.timeout = timeout

    def binary(self) -> str | None:
        return find(os.environ.get("CHASE_SLUGS", "slugs"))

    def compiler(self) -> str | None:
        """The structured-slugs compiler shipped with slugs."""
        env = os.environ.get("SLUGSCOMPILER")
        if env and os.path.isfile(env):
            return env
        binary = self.binary()
        if binary:
            root = os.path.dirname(os.path.dirname(os.path.realpath(binary)))
            path = os.path.join(root, "tools", "StructuredSlugsParser", "compiler.py")
            if os.path.isfile(path):
                return path
        return None

    def available(self) -> bool:
        return self.binary() is not None and self.compiler() is not None

    def emit(self, query: Realizable) -> str:
        """The structured-slugs specification, written by the CHASE back end."""
        from .._native import pychase_backends
        from ..contract import _parse_contracts
        holder = _parse_contracts([_iff(query.contract)])
        try:
            with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
                path = os.path.join(tmp, "spec.structuredSlugs")
                pychase_backends.SlugsPrinter().print(holder[query.contract.name], path)
                with open(path) as f:
                    return f.read()
        finally:
            holder.free()

    def _slugs(self, args: list[str], cwd: str) -> subprocess.CompletedProcess:
        return subprocess.run([self.binary()] + args, cwd=cwd, capture_output=True,
                              text=True, timeout=self.timeout)

    def run(self, query: Realizable) -> Result:
        # what slugs does not take, whether it is installed or not
        from ..words import word_type
        found = sorted(v for v, t in query.contract.variables.items() if word_type(t))
        if found:
            return Result(UNKNOWN, self.name, reason=f"slugs does not take words ({', '.join(found)}): "
                          "use integer ranges")
        c = query.contract
        if any(ltl.has_past(f) for f in c.assumptions + c.guarantees):
            return Result(UNKNOWN, self.name, reason="slugs does not take past(x, n): GR(1) "
                          "formulas read the current and next values only")
        if not self.available():
            return Result(UNKNOWN, self.name, reason="slugs or its compiler.py is not installed; run "
                          "`chase-cps doctor` to see how to install it")
        start = time.monotonic()
        from ..gr1 import gr1_roles
        problem = gr1_roles(query.contract)
        if problem:
            return Result(UNKNOWN, self.name, reason=f"cannot encode for slugs: {problem}")
        try:
            structured = self.emit(query)
        except RuntimeError as e:   # the contract is not in a GR(1) form
            return Result(UNKNOWN, self.name, reason=f"cannot encode for slugs: {e}")
        with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
            src = os.path.join(tmp, "spec.structuredSlugs")
            with open(src, "w") as f:
                f.write(structured)
            compiled = subprocess.run([sys.executable, self.compiler(), src],
                                      capture_output=True, text=True)
            if compiled.returncode != 0 or not compiled.stdout.strip():
                return Result(UNKNOWN, self.name, reason="structured slugs compilation failed: "
                              + compiled.stderr.strip()[-300:])
            slugsin = os.path.join(tmp, "spec.slugsin")
            with open(slugsin, "w") as f:
                f.write(compiled.stdout)
            try:
                proc = self._slugs(["spec.slugsin"], tmp)   # realizability only
                out = proc.stdout + proc.stderr
                if "is realizable" in out:
                    status = REALIZABLE
                elif "is unrealizable" in out:
                    status = UNREALIZABLE
                else:
                    return Result(UNKNOWN, self.name, reason=out.strip()[-300:],
                                  seconds=time.monotonic() - start)
                result = Result(status, self.name, details={"structuredSlugs": structured})
                if self.controller and status == REALIZABLE:
                    s = self._slugs(["--explicitStrategy", "--jsonOutput", "spec.slugsin"], tmp)
                    try:
                        result.controller = json.loads(s.stdout)
                    except json.JSONDecodeError:
                        result.controller = s.stdout
                    # slugs does not mark the initial nodes: Model.from_controller
                    # takes the nodes that satisfy the initial terms.
                    from ..gr1 import gr1_kind
                    c = query.contract
                    result.details["init"] = [ltl.to_str(t) for t in
                                              list(c.assumptions) + list(c.guarantees)
                                              if gr1_kind(t) == "init"]
                elif self.counter_strategy and status == UNREALIZABLE:
                    s = self._slugs(["--counterStrategy", "spec.slugsin"], tmp)
                    result.counter_strategy = s.stdout
            except subprocess.TimeoutExpired:
                return Result(UNKNOWN, self.name, reason=f"timeout after {self.timeout:g} s",
                              seconds=time.monotonic() - start)
        result.seconds = time.monotonic() - start
        return result


def _iff(contract):
    """The contract with a = b written a <-> b, and a != b !(a <-> b), when
    a and b are Booleans: structured slugs compares only numbers."""
    from dataclasses import replace
    booleans = {v for v, t in contract.variables.items() if t == "bool"}

    def boolean(n) -> bool:          # a formula, not a number
        if isinstance(n, ltl.Num):
            return False
        if isinstance(n, ltl.Var):
            return n.name in booleans
        if isinstance(n, ltl.Unary):
            return n.op != "neg"
        if isinstance(n, ltl.Binary):
            return n.op not in ltl.ARITHMETIC + ltl.WORD_OPS
        return True

    def fix(n):
        if isinstance(n, ltl.Unary):
            return ltl.Unary(n.op, fix(n.arg))
        if isinstance(n, ltl.Binary):
            if n.op in ("=", "!=") and boolean(n.left) and boolean(n.right):
                same = ltl.Binary("<->", fix(n.left), fix(n.right))
                return same if n.op == "=" else ltl.Unary("!", same)
            return ltl.Binary(n.op, fix(n.left), fix(n.right))
        return n
    return replace(contract, assumptions=[fix(t) for t in contract.assumptions],
                   guarantees=[fix(t) for t in contract.guarantees])
