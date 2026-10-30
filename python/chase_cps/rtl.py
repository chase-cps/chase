"""RTL as state machines: Verilog and SystemVerilog read with Yosys.

``from_rtl(files, top)`` gives the module ``top`` as a ``Model``, which
the checks take as an implementation (``is_implementation``) or an
environment (``is_environment``), and ``is_receptive``. Yosys (ISC license) reads the design,
flattens it, maps the memories to registers, and writes it in SMV; CHASE
names the ports as the variables of the contracts: a 1-bit port is a
Boolean, a wider one a word.

One step is one cycle of the clock: the clock inputs are left out. With
``reset`` (an input such as ``"reset_i"``, or ``"!rst_n"`` for an active-low
one), the reset holds in the first step and not after, and the contracts are
checked from the step after the reset (``start`` of the model is 1). Without
it the reset is an input like the others, and the registers start with any
value.

Yosys is found as ``yosys`` in the path, as ``yowasp-yosys`` (``pip install
chase-cps[rtl]``, Yosys compiled to WebAssembly), or with the environment
variable ``CHASE_YOSYS``.
"""

from __future__ import annotations

import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from typing import Optional

from .models import Model


class RTLError(ValueError):
    """The design could not be read (the message has the output of Yosys)."""


def yosys() -> Optional[list[str]]:
    """The command that runs Yosys, or None."""
    if os.environ.get("CHASE_YOSYS"):
        return [os.environ["CHASE_YOSYS"]]
    for name in ("yosys", "yowasp-yosys"):
        found = shutil.which(name)
        if found:
            return [found]
    here = os.path.join(os.path.dirname(sys.executable), "yowasp-yosys")
    if os.path.exists(here):
        return [here]
    return None


def from_rtl(files, top: str, parameters: Optional[dict] = None,
             reset: Optional[str] = None, clocks=("clk", "clk_i", "clock"),
             names: Optional[dict] = None, defines: Optional[dict] = None,
             timeout: float = 300) -> Model:
    """The module ``top`` of the Verilog or SystemVerilog ``files`` as a
    state machine. ``parameters`` sets parameters of ``top``; ``clocks``
    names the clock inputs; ``reset`` holds in the first step only (it is
    then not an input of the model); ``names`` renames ports as the
    variables of the contracts ({"ready_and_o": "ready_o"})."""
    command = yosys()
    if command is None:
        raise RTLError("Yosys is not installed: pip install chase-cps[rtl], or install yosys")
    if isinstance(files, (str, os.PathLike)):
        files = [files]
    files = [os.path.abspath(f) for f in files]
    # the headers next to the sources are copied too, for `include
    headers = [os.path.join(d, f) for d in dict.fromkeys(os.path.dirname(f) for f in files)
               for f in sorted(os.listdir(d)) if f.endswith((".svh", ".vh"))]
    for f in files:
        if not os.path.exists(f):
            raise RTLError(f"no file {f}")
    names_seen = {}
    for f in files:
        base = os.path.basename(f)
        if base in names_seen and names_seen[base] != f:
            raise RTLError(f"two files named {base}: {names_seen[base]} and {f}")
        names_seen[base] = f
    defs = " ".join(f"-D{k}={v}" for k, v in (defines or {}).items())
    chparam = " ".join(f"-chparam {k} {v}" for k, v in (parameters or {}).items())
    with tempfile.TemporaryDirectory(prefix="chase_rtl_") as tmp:
        # the sources are copied next to the script: Yosys compiled to
        # WebAssembly sees only its directory, and `include finds them there
        for f in headers + files:
            shutil.copy(f, os.path.join(tmp, os.path.basename(f)))
        script = "\n".join([
            f"read_verilog -sv -I. {defs} "
            + " ".join(f'"{os.path.basename(f)}"' for f in files),
            f"hierarchy -check -top {top} {chparam}",
            f"prep -top {top}",
            "flatten",
            "memory -nomap",
            "memory_map",
            "opt -fast",
            "dffunmap",
            "write_json ports.json",
            "write_smv design.smv",
        ]) + "\n"
        with open(os.path.join(tmp, "script.ys"), "w") as f:
            f.write(script)
        p = subprocess.run(command + ["-q", "-s", "script.ys"], cwd=tmp, capture_output=True,
                           text=True, timeout=timeout)
        if p.returncode or not os.path.exists(os.path.join(tmp, "design.smv")):
            output = (p.stdout + "\n" + p.stderr).splitlines()
            errors = [l for l in output if "ERROR" in l]
            if not errors:      # the WebAssembly build may stop without an ERROR line
                errors = [l for l in output if "Executing" in l][-1:] + \
                    ["Yosys stopped (an exception) while reading that file"]
            raise RTLError(f"Yosys could not read {top}: " + "\n".join(errors)[-2000:])
        with open(os.path.join(tmp, "design.smv")) as f:
            smv = f.read()
        with open(os.path.join(tmp, "ports.json")) as f:
            design = json.load(f)
    module = design["modules"][top]
    model = _model(top, smv, module, reset, set(clocks), names or {})
    model.source = {"kind": "rtl", "files": files, "top": top, "parameters": parameters,
                    "defines": defines,
                    "outputs": [(names or {}).get(p, p) for p, port in module["ports"].items()
                                if port["direction"] != "input"]}
    return model


def _model(top, smv, module, reset, clocks, names) -> Model:
    ports = {}
    for name, port in module["ports"].items():
        width = len(port["bits"])
        signed = bool(module["netnames"].get(name, {}).get("signed"))
        ports[name] = (port["direction"], width, signed)
    reset_name, reset_low = None, False
    if reset:
        reset_low = reset.strip().startswith("!")
        reset_name = reset.strip().lstrip("!").strip()
        if reset_name not in ports or ports[reset_name][0] != "input":
            raise RTLError(f"{reset_name} is not an input of {top}")
    lines = smv.splitlines()
    out, section = [], None
    header = re.compile(rf"^MODULE\s+_{re.escape(top)}\s*$")
    for line in lines:
        if header.match(line):
            out.append("MODULE main")
            continue
        s = line.strip()
        if s in ("IVAR", "VAR", "DEFINE", "ASSIGN", "INIT", "TRANS", "INVAR"):
            section = s
        m = re.match(r"^\s*_([A-Za-z]\w*)\s*:", line)
        if section == "IVAR" and m and m.group(1) in ports:
            continue                     # the inputs are declared again below
        if s.startswith("-- end of yosys output"):
            continue
        out.append(line)
    if not any(l.strip() == "MODULE main" for l in out):
        raise RTLError(f"no module {top} in the output of Yosys")
    # a section left empty (the IVAR of the inputs) is dropped
    keep = []
    for i, line in enumerate(out):
        if line.strip() in ("IVAR", "VAR", "DEFINE", "ASSIGN"):
            rest = [l for l in out[i + 1:] if l.strip() and not l.strip().startswith("--")]
            if not rest or rest[0].strip() in ("IVAR", "VAR", "DEFINE", "ASSIGN", "INIT",
                                               "TRANS", "INVAR") or rest[0].startswith("MODULE"):
                continue
        keep.append(line)
    out = keep

    def vtype(width, signed):
        if width == 1:
            return "bool", "boolean"
        t = f"{'signed ' if signed else ''}word[{width}]"
        return t, f"{'signed' if signed else 'unsigned'} word[{width}]"

    variables, ivars = {}, set()
    ivar, define, extra = [], [], []
    unknown = sorted(set(names) - set(ports))
    if unknown:
        raise RTLError(f"names: {unknown} are not ports of {top}")
    for name, (direction, width, signed) in ports.items():
        ctype, stype = vtype(width, signed)
        port, name = name, names.get(name, name)
        if direction == "input":
            if port in clocks:
                define.append(f"    _{port} := 0ub{width}_0;")
                continue
            if port == reset_name:
                extra += ["VAR", "    chase_started : boolean;", "ASSIGN",
                          "    init(chase_started) := FALSE;",
                          "    next(chase_started) := TRUE;"]
                held = "!chase_started"
                define.append(f"    {name} := {held if not reset_low else '!(' + held + ')'};")
                define.append(f"    _{port} := word1({name});")
                variables[name] = "bool"
                continue
            ivar.append(f"    {name} : {stype};")
            ivars.add(name)
            variables[name] = ctype
            if width == 1:
                define.append(f"    _{port} := word1({name});")
            elif signed:
                define.append(f"    _{port} := unsigned({name});")
            else:
                define.append(f"    _{port} := {name};")
        else:
            variables[name] = ctype
            if width == 1:
                define.append(f"    {name} := bool(_{port});")
            elif signed:
                define.append(f"    {name} := signed(_{port});")
            else:
                define.append(f"    {name} := _{port};")
    text = "\n".join(out + (["IVAR"] + ivar if ivar else []) + ["DEFINE"] + define + extra) + "\n"
    return Model(top, text, variables, ivars, start=1 if reset else 0)


__all__ = ["RTLError", "from_rtl", "yosys"]
