"""Slugs adapter: GR(1) realizability, controller, counter-strategy."""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
import time

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
        pc = query.contract.to_pychase()
        with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
            path = os.path.join(tmp, "spec.structuredSlugs")
            pychase_backends.SlugsPrinter().print(pc, path)
            with open(path) as f:
                return f.read()

    def _slugs(self, args: list[str], cwd: str) -> subprocess.CompletedProcess:
        return subprocess.run([self.binary()] + args, cwd=cwd, capture_output=True,
                              text=True, timeout=self.timeout)

    def run(self, query: Realizable) -> Result:
        if not self.available():
            return Result(UNKNOWN, self.name, reason="slugs or its compiler.py is not installed; run "
                          "`chase-cps doctor` to see how to install it")
        start = time.monotonic()
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
                elif self.counter_strategy and status == UNREALIZABLE:
                    s = self._slugs(["--counterStrategy", "spec.slugsin"], tmp)
                    result.counter_strategy = s.stdout
            except subprocess.TimeoutExpired:
                return Result(UNKNOWN, self.name, reason=f"timeout after {self.timeout:g} s",
                              seconds=time.monotonic() - start)
        result.seconds = time.monotonic() - start
        return result
