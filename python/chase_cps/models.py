"""State machines as the implementations and the environments of contracts.

A ``Model`` is a program in SMV, the input language of nuXmv and NuSMV: a
state machine, possibly nondeterministic, given by its variables, initial
states, and transitions. It describes discrete behaviors, one step at a
time, as the traces of the contracts do. A model is built from

- SMV text or a file (``Model.from_smv``),
- a controller synthesized by slugs (``Model.from_controller``),
- a state machine in JSON (``Model.from_dict``).

The checks on contracts take models: ``is_implementation(m, c)`` (every behavior
of m satisfies A -> G), ``is_environment(e, c)`` (every behavior of the
environment e satisfies A), and ``is_receptive(m, c)`` (m never refuses an
input that the assumptions allow, so that ``is_implementation`` does not hold only
because m blocks the inputs).
"""

from __future__ import annotations

import os
import re
import subprocess
import tempfile
import time
from dataclasses import dataclass, field
from itertools import product
from typing import Any, Optional

from . import ltl, words
from .contract import Contract, VarType
from .queries import NOT_RECEPTIVE, RECEPTIVE, UNKNOWN, Result

_SECTIONS = ("MODULE", "VAR", "IVAR", "FROZENVAR", "DEFINE", "CONSTANTS", "ASSIGN", "INIT",
             "INVAR", "TRANS", "FAIRNESS", "JUSTICE", "COMPASSION", "ISA", "PRED", "MIRROR")
_SPECS = ("LTLSPEC", "SPEC", "CTLSPEC", "INVARSPEC", "PSLSPEC", "COMPUTE")
_KEYWORD = re.compile(r"\b(" + "|".join(_SECTIONS + _SPECS) + r")\b")
_DECL = re.compile(r"([A-Za-z_][\w$#-]*)\s*:\s*([^;]+);")
_NAME = re.compile(r"[A-Za-z_]\w*$")


def _strip_comments(text: str) -> str:
    return re.sub(r"--[^\n]*", "", text)


def _sections(text: str) -> list[tuple[str, int, int]]:
    """(keyword, start of the body, end of the body) of the sections of the
    module main."""
    body = _strip_comments(text)
    m = re.search(r"\bMODULE\s+main\b", body)
    if not m:
        raise ValueError("the SMV model has no MODULE main")
    end = len(body)
    nxt = re.search(r"\bMODULE\b", body[m.end():])
    if nxt:
        end = m.end() + nxt.start()
    marks = [(k.group(1), k.start(), k.end()) for k in _KEYWORD.finditer(body, m.end(), end)]
    out = []
    for i, (kw, _, stop) in enumerate(marks):
        out.append((kw, stop, marks[i + 1][1] if i + 1 < len(marks) else end))
    return out


def _vartype(smv_type: str) -> VarType:
    t = " ".join(smv_type.split())
    if t == "boolean":
        return "bool"
    if t == "real":
        return "real"
    m = re.fullmatch(r"(-?\d+)\s*\.\.\s*(-?\d+)", t)
    if m:
        return (int(m.group(1)), int(m.group(2)))
    if words.word_type(t):
        return words.normalize(t)
    return t     # integer, an enumeration...


def _smvtype(t: VarType) -> str:
    if t == "bool":
        return "boolean"
    if t == "real":
        return "real"
    if isinstance(t, tuple):
        return f"{t[0]}..{t[1]}"
    if words.word_type(t):
        return words.smv_type(t)
    return str(t)


def _values(t: VarType) -> Optional[list]:
    """The values of a finite type, or None."""
    if t == "bool":
        return [False, True]
    if isinstance(t, tuple) and t[1] - t[0] < 4096:
        return list(range(t[0], t[1] + 1))
    if words.word_type(t):
        return words.values(t)
    return None


def _smv_value(v, t: Optional[VarType] = None) -> str:
    if v is True:
        return "TRUE"
    if v is False:
        return "FALSE"
    if words.word_type(t):
        return words.literal(v, *words.word_type(t))
    return str(v)


@dataclass
class Model:
    """A state machine as an SMV program (MODULE main, no specification).
    ``variables`` are the variables it declares (VAR, IVAR, FROZENVAR) and
    defines (DEFINE), with their types; ``ivars`` the ones declared IVAR."""
    name: str
    smv: str
    variables: dict[str, VarType] = field(default_factory=dict)
    ivars: set[str] = field(default_factory=set)
    start: int = 0      # the steps of a reset: the contracts are checked after them

    @classmethod
    def from_smv(cls, source: str, name: Optional[str] = None) -> "Model":
        """A model from SMV text or the path of an SMV file. The module main
        holds the variables of the contracts it is checked against; it must
        not have specifications (LTLSPEC, SPEC, ...)."""
        text = source
        if "MODULE" not in source and os.path.exists(source):
            with open(source) as f:
                text = f.read()
            name = name or os.path.splitext(os.path.basename(source))[0]
        variables, ivars = {}, set()
        for kw, start, stop in _sections(text):
            if kw in _SPECS:
                raise ValueError(f"the SMV model has a specification ({kw}): CHASE adds the "
                                 "specification of each check")
            if kw in ("VAR", "IVAR", "FROZENVAR"):
                for m in _DECL.finditer(_strip_comments(text)[start:stop]):
                    variables[m.group(1)] = _vartype(m.group(2))
                    if kw == "IVAR":
                        ivars.add(m.group(1))
            elif kw == "DEFINE":
                for m in re.finditer(r"([A-Za-z_][\w$#-]*)\s*:=", _strip_comments(text)[start:stop]):
                    variables.setdefault(m.group(1), "define")
        return cls(name or "M", text, variables, ivars)

    @classmethod
    def from_rtl(cls, files, top: str, **options) -> "Model":
        """The module ``top`` of Verilog or SystemVerilog files, read with
        Yosys (see ``chase_cps.rtl.from_rtl`` for the options)."""
        from .rtl import from_rtl
        return from_rtl(files, top, **options)

    @classmethod
    def from_dict(cls, d: dict) -> "Model":
        """A model from a state machine in JSON::

            {"name": "Arbiter",
             "inputs": {"req": "bool"}, "outputs": {"grant": "bool"},
             "states": ["idle", "busy"], "initial": "idle",
             "transitions": [
               {"from": "idle", "when": "req", "outputs": {"grant": true}, "to": "busy"},
               {"from": "idle", "when": "!req", "outputs": {"grant": false}, "to": "idle"},
               {"from": "busy", "outputs": {"grant": false}, "to": "idle"}]}

        At each step the environment sets the inputs; in its state, the
        machine takes a transition whose ``when`` (a formula over the inputs
        and the outputs, true when absent) holds, sets the outputs of the
        same step as ``outputs`` says (an output not given is free), and
        moves to ``to`` for the next step. ``initial`` is a state or a list
        of states. The types are those of ``Contract.from_dict``: "bool",
        or [min, max] for an integer range. With no transition to take, the
        machine refuses the input (see ``is_receptive``).
        """
        name = d.get("name", "M")
        inputs = {k: tuple(v) if isinstance(v, list) else v for k, v in d.get("inputs", {}).items()}
        outputs = {k: tuple(v) if isinstance(v, list) else v
                   for k, v in d.get("outputs", {}).items()}
        states = list(d.get("states", []))
        for s in states:
            if not _NAME.match(s) or s in ("TRUE", "FALSE"):
                raise ValueError(f"state {s!r}: the states must be identifiers")
        if not states:
            raise ValueError("the state machine has no states")
        initial = d.get("initial", states[0])
        initial = [initial] if isinstance(initial, str) else list(initial)
        for s in initial:
            if s not in states:
                raise ValueError(f"the initial state {s!r} is not a state")
        variables = {**inputs, **outputs}
        cases = []
        for i, t in enumerate(d.get("transitions", [])):
            src, dst = t.get("from"), t.get("to")
            if src not in states or dst not in states:
                raise ValueError(f"transition {i}: from and to must be states")
            parts = [f"chase_state = {src}"]
            if t.get("when") not in (None, "", "true"):
                when = ltl.parse(t["when"]) if isinstance(t["when"], str) else t["when"]
                bad = ltl.variables(when) - set(variables)
                if bad:
                    raise ValueError(f"transition {i}: undeclared variables {sorted(bad)}")
                if _has_temporal(when):
                    raise ValueError(f"transition {i}: when must not have temporal operators")
                parts.append(words.to_smv(when, variables))
            for o, v in t.get("outputs", {}).items():
                if o not in outputs:
                    raise ValueError(f"transition {i}: {o} is not an output")
                parts.append(f"{o} = {_smv_value(v, variables.get(o))}")
            parts.append(f"next(chase_state) = {dst}")
            cases.append("(" + " & ".join(parts) + ")")
        lines = ["MODULE main", "VAR", f"    chase_state : {{{', '.join(states)}}};"]
        lines += [f"    {v} : {_smvtype(t)};" for v, t in variables.items()]
        lines.append("INIT " + " | ".join(f"chase_state = {s}" for s in initial) + ";")
        lines.append("TRANS " + (" | ".join(cases) if cases else "FALSE") + ";")
        model = cls(name, "\n".join(lines) + "\n", dict(variables), set())
        model.variables["chase_state"] = "{" + ", ".join(states) + "}"
        return model

    @classmethod
    def from_controller(cls, controller, contract: Optional[Contract] = None,
                        name: Optional[str] = None) -> "Model":
        """The model of a controller synthesized by slugs: the result of
        ``synthesize``, or its ``controller`` (the JSON of slugs) with the
        contract it was synthesized from. Each node of the controller is a
        state, with the values of the inputs and the outputs; the initial
        states are the nodes that satisfy the initial terms of that contract
        (slugs does not mark them)."""
        init = None
        if isinstance(controller, Result):
            init = controller.details.get("init")
            controller = controller.controller
        if contract is not None:
            from .gr1 import gr1_kind
            init = [ltl.to_str(t) for t in list(contract.assumptions) + list(contract.guarantees)
                    if gr1_kind(t) == "init"]
        if not isinstance(controller, dict) or "nodes" not in controller:
            raise ValueError("not a controller of slugs (the JSON of synthesize)")
        if init is None:
            raise ValueError("the initial states of a controller need the contract it was "
                             "synthesized from")
        names = controller["variables"]
        nodes = controller["nodes"]
        order = sorted(nodes, key=int)
        index = {n: i for i, n in enumerate(order)}
        # slugs writes an integer as bits x@0.min.max, x@1, ...
        ints: dict[str, tuple[int, int]] = {}
        bits: dict[str, list[tuple[int, int]]] = {}
        plain = []
        for pos, v in enumerate(names):
            m = re.fullmatch(r"(.+)@(\d+)(?:\.(-?\d+)\.(-?\d+))?", v)
            if m:
                base = m.group(1)
                if m.group(3) is not None:
                    ints[base] = (int(m.group(3)), int(m.group(4)))
                bits.setdefault(base, []).append((int(m.group(2)), pos))
            else:
                plain.append((v, pos))
        variables: dict[str, VarType] = {v: "bool" for v, _ in plain}
        variables.update(ints)
        if contract is not None:
            variables.update({v: t for v, t in contract.variables.items() if v in variables})

        def value(state, v):
            if v in ints:
                return ints[v][0] + sum(state[pos] << k for k, pos in bits[v])
            return _smv_value(bool(state[dict(plain)[v]]))

        lines = ["MODULE main", "VAR", f"    chase_node : 0..{max(len(order) - 1, 0)};", "DEFINE"]
        for v in variables:
            arms = [f"chase_node = {index[n]} : {value(nodes[n]['state'], v)};" for n in order]
            lines.append(f"    {v} := case {' '.join(arms)} esac;")
        trans = []
        for n in order:
            succ = sorted({index[str(s)] for s in nodes[n].get("trans", [])})
            target = "{" + ", ".join(map(str, succ)) + "}" if succ else None
            trans.append(f"chase_node = {index[n]} : "
                         + (f"next(chase_node) in {target};" if target else "FALSE;"))
        lines.append("TRANS case " + " ".join(trans) + " esac;")
        init_f = ltl.conjunction([ltl.parse(t) for t in init])
        lines.append(f"INIT {words.to_smv(init_f, variables)};")
        model = cls(name or "Controller", "\n".join(lines) + "\n", dict(variables), set())
        model.variables["chase_node"] = (0, max(len(order) - 1, 0))
        return model

    def check_variables(self, needed, what: str, types: Optional[dict] = None) -> None:
        """The model declares the variables needed, with the same kind of
        type as in ``types`` (Boolean, integer, real) when given."""
        missing = sorted(set(needed) - set(self.variables))
        if missing:
            raise ValueError(f"the model {self.name} does not declare {missing}, the variables "
                             f"of {what}")
        kind = lambda t: "integer" if isinstance(t, tuple) or t == "int" else t  # noqa: E731
        for v in needed:
            mine, theirs = self.variables[v], (types or {}).get(v)
            if theirs is None or mine == "define" or (
                    kind(mine) not in ("bool", "integer", "real")
                    and not words.word_type(mine)):
                continue
            if kind(mine) != kind(theirs):
                raise ValueError(f"{v} is {kind(theirs)} in {what} and {kind(mine)} in the "
                                 f"model {self.name}")

    def to_dict(self) -> dict:
        return {"name": self.name, "smv": self.smv,
                "variables": {k: list(v) if isinstance(v, tuple) else v
                              for k, v in self.variables.items()}}


def _has_temporal(node: ltl.Node) -> bool:
    if isinstance(node, ltl.Unary):
        return node.op in ("G", "F", "X") or _has_temporal(node.arg)
    if isinstance(node, ltl.Binary):
        return node.op in ("U", "R", "W") or _has_temporal(node.left) or _has_temporal(node.right)
    return isinstance(node, ltl.Var) and node.primed


def as_model(x, contract: Optional[Contract] = None) -> Optional[Model]:
    """x as a Model when it is one (a Model, a controller, SMV text, or a
    state machine in JSON), else None (an LTL formula)."""
    if isinstance(x, Model):
        return x
    if isinstance(x, Result):
        return Model.from_controller(x)
    if isinstance(x, dict):
        if "nodes" in x:
            return Model.from_controller(x, contract)
        return Model.from_dict(x)
    if isinstance(x, str) and re.search(r"\bMODULE\b", x):
        return Model.from_smv(x)
    return None


# Receptiveness ----------------------------------------------------------------

def _evaluate(node: ltl.Node, v: dict, variables=None):
    """The value of a formula without temporal operators."""
    if variables and words.has_words(variables):
        return words.evaluate(node, v, variables)
    if isinstance(node, ltl.Const):
        return node.value
    if isinstance(node, ltl.Num):
        return node.value
    if isinstance(node, ltl.Var):
        return v[node.name]
    if isinstance(node, ltl.Unary):
        a = _evaluate(node.arg, v)
        return (not a) if node.op == "!" else -a
    a, b = _evaluate(node.left, v), _evaluate(node.right, v)
    return {"&": lambda: a and b, "|": lambda: a or b, "->": lambda: (not a) or b,
            "<->": lambda: a == b, "=": lambda: a == b, "!=": lambda: a != b,
            "<": lambda: a < b, "<=": lambda: a <= b, ">": lambda: a > b,
            ">=": lambda: a >= b, "+": lambda: a + b, "-": lambda: a - b,
            "*": lambda: a * b, "/": lambda: a / b}[node.op]()


def _next_of(node: ltl.Node, inputs: set[str], probe, now: bool) -> Optional[ltl.Node]:
    """The body of a safety assumption as a condition on the current state
    and the next inputs, these read as their probes; None when it reads a
    next output. ``now`` is False under X."""
    if isinstance(node, ltl.Var):
        if node.primed or not now:
            return ltl.Var(probe(node.name)) if node.name in inputs else None
        return node
    if isinstance(node, ltl.Unary):
        if node.op == "X":
            return _next_of(node.arg, inputs, probe, False)
        a = _next_of(node.arg, inputs, probe, now)
        return None if a is None else ltl.Unary(node.op, a)
    if isinstance(node, ltl.Binary):
        a = _next_of(node.left, inputs, probe, now)
        b = _next_of(node.right, inputs, probe, now)
        return None if a is None or b is None else ltl.Binary(node.op, a, b)
    return node


def _allowed(contract: Contract, probe):
    """The initial assumptions on the inputs, and the condition, on the
    current state and the probes, that the safety assumptions put on the
    next inputs. The other assumptions are left out: the check then asks
    for more inputs, never fewer."""
    from .gr1 import _next_vars, gr1_kind
    inputs = set(contract.inputs)
    initial, step = [], []
    for t in contract.assumptions:
        kind = gr1_kind(t)
        if kind == "init" and ltl.variables(t) <= inputs:
            initial.append(t)
        elif kind == "safety":
            body = t.arg
            if _next_vars(body):
                cond = _next_of(body, inputs, probe, True)
            elif ltl.variables(body) <= inputs:    # an invariant of the inputs
                cond = _next_of(body, inputs, probe, False)
            else:
                cond = None
            if cond is not None:
                step.append(cond)
    return initial, step


def is_receptive(model, contract: Contract, backend: str = "auto",
                 timeout: float = 300, **options) -> Result:
    """Does the model accept every input that the assumptions of the
    contract allow? ``receptive``, or ``not_receptive`` with the trace to the
    state where it refuses an input (``details``: the ``state`` and the
    ``input``), or ``unknown``.

    A model that refuses an input has no behavior with it, so it may
    implement a contract only because it blocks the inputs (OCRA checks
    receptiveness with the implementation for this reason). The check: in
    every reachable state, for every next value of the inputs that the
    safety assumptions allow, there is a transition with that value from
    which the model can go on forever; and every initial value of the inputs
    that the initial assumptions allow starts a behavior. The other
    assumptions (liveness, or terms on the outputs) are not used, which asks
    for more inputs than OCRA does. The inputs must be Boolean or integer
    ranges. Uses the BDD engine of nuXmv or NuSMV (CTL).
    """
    from .backends import ADAPTERS
    m = as_model(model, contract)
    if m is None:
        raise TypeError("is_receptive takes a model (Model, SMV text, a controller, or a "
                        "state machine in JSON)")
    m.check_variables(contract.inputs, f"the inputs of {contract.name}", contract.variables)
    inputs = list(contract.inputs)
    domains = {x: _values(contract.inputs[x]) for x in inputs}
    infinite = [x for x, d in domains.items() if d is None]
    if infinite:
        return Result(UNKNOWN, "nusmv", reason=f"the inputs {infinite} are not finite: "
                      "receptiveness is checked with BDDs")
    tool = backend.partition("/")[0]   # CTL is checked with BDDs whatever the engine
    tools = [n for n in (["nuxmv", "nusmv"] if tool == "auto" else [tool])
             if n in ADAPTERS and hasattr(ADAPTERS[n], "binary")]
    found = next((ADAPTERS[n].binary() for n in tools if ADAPTERS[n].available()), None)
    if found is None:
        return Result(UNKNOWN, backend, reason="neither nuXmv nor NuSMV is installed")
    path, flavor = found

    probe = lambda x: f"chase_probe_{x}"   # noqa: E731
    initial, step = _allowed(contract, probe)
    text = _inputs_as_state(m, set(inputs))
    lines = [text.rstrip(), "FROZENVAR"]
    lines += [f"    {probe(x)} : {_smvtype(contract.inputs[x])};" for x in inputs]
    same = " & ".join(f"{x} = {probe(x)}" for x in inputs) or "TRUE"
    probes = {**contract.variables, **{probe(x): t for x, t in contract.inputs.items()}}
    allowed = words.to_smv(ltl.conjunction(step), probes) if step else "TRUE"
    lines.append(f"SPEC AG (({allowed}) -> EX ({same}));")
    starts = []
    count = 1
    for d in domains.values():
        count *= len(d)
    if count <= 1024:
        for combo in product(*(domains[x] for x in inputs)):
            v = dict(zip(inputs, combo))
            if all(_evaluate(t, v, contract.variables) for t in initial):
                starts.append(v)
                lines.append("SPEC !(" + (" & ".join(f"{x} = {_smv_value(v[x], contract.inputs[x])}"
                                                     for x in inputs)
                                          or "TRUE") + ");")
    smv = "\n".join(lines) + "\n"
    start = time.monotonic()
    with tempfile.TemporaryDirectory(prefix="chase_cps_") as tmp:
        with open(os.path.join(tmp, "model.smv"), "w") as f:
            f.write(smv)
        if flavor == "nuxmv":
            with open(os.path.join(tmp, "commands"), "w") as f:
                f.write("set on_failure_script_quits\nread_model -i model.smv\ngo\n"
                        "check_ctlspec\nquit\n")
            args = [path, "-source", "commands"]
        else:
            args = [path, "-dynamic", "model.smv"]
        try:
            proc = subprocess.run(args, cwd=tmp, capture_output=True, text=True,
                                  stdin=subprocess.DEVNULL, timeout=timeout)
        except subprocess.TimeoutExpired:
            return Result(UNKNOWN, f"{flavor}/bdd", reason=f"timeout after {timeout:g} s",
                          seconds=time.monotonic() - start)
    seconds = time.monotonic() - start
    backend_name = f"{flavor}/bdd"
    # The model checkers do not print the verdicts in the order of the file:
    # the step check is the one with AG, an initial check gives its values.
    step_chunk, step_verdict, refused = None, None, []
    for chunk in re.split(r"(?m)^-- specification ", proc.stdout)[1:]:
        v = re.match(r"(.*?)\s+is (true|false)", chunk, re.S)
        if not v:
            continue
        spec, verdict = " ".join(v.group(1).split()), v.group(2)
        if spec.startswith("AG"):
            step_chunk, step_verdict = chunk, verdict
        elif verdict == "true":     # no initial state with these inputs
            refused.append({x: _value(val) for x, val in
                            re.findall(r"(\w+) = (\w+)", spec)})
    n_specs = len(re.findall(r"(?m)^-- specification ", proc.stdout))
    if step_verdict is None or n_specs != 1 + len(starts):
        from .backends.nusmv import _error
        return Result(UNKNOWN, backend_name, seconds=seconds, details={"model": smv},
                      reason=_error(proc.stderr + "\n" + proc.stdout) or "no verdict")
    details: dict[str, Any] = {}
    if count > 1024:
        details["initial inputs"] = "not checked: more than 1024 values"
    from .backends.nusmv import parse_trace
    if step_verdict == "false":
        chunks = [step_chunk]
        names = {**m.variables, **{probe(x): t for x, t in contract.inputs.items()}}
        trace = parse_trace(chunks[0], names)
        last = trace.prefix[-1] if trace and trace.prefix else {}
        blocked = {x: last.get(probe(x)) for x in inputs}
        state = {k: v for k, v in last.items() if not k.startswith("chase_probe_")}
        if trace is not None:
            for s in trace.prefix:
                for x in inputs:
                    s.pop(probe(x), None)
        details.update({"state": state, "input": blocked})
        return Result(NOT_RECEPTIVE, backend_name, trace=trace, seconds=seconds,
                      details=details)
    if refused:
        details["input"] = refused[0]
        details["state"] = "initial"
        return Result(NOT_RECEPTIVE, backend_name, seconds=seconds, details=details)
    return Result(RECEPTIVE, backend_name, seconds=seconds, details=details)


def _value(text: str):
    w = words.value(text)
    if w is not None:
        return w
    return {"TRUE": True, "FALSE": False}.get(text, int(text) if re.fullmatch(r"-?\d+", text)
                                              else text)


def _inputs_as_state(m: Model, inputs: set[str]) -> str:
    """The SMV text with the inputs declared IVAR moved to VAR. An IVAR is
    the label of a transition; as a state variable, free, it has the same
    value at each step, and a refused input becomes a state without
    successor, which the CTL check sees."""
    move = inputs & m.ivars
    if not move:
        return m.smv
    text = _strip_comments(m.smv)
    decls = []
    out, pos = [], 0
    for kw, start, stop in _sections(text):
        if kw != "IVAR":
            continue
        body = text[start:stop]
        keep = body
        for d in _DECL.finditer(body):
            if d.group(1) in move:
                decls.append(f"    {d.group(1)} : {d.group(2).strip()};")
                keep = keep.replace(d.group(0), "", 1)
        out.append(text[pos:start])
        out.append(keep)
        pos = stop
    out.append(text[pos:])
    text = "".join(out)
    text = re.sub(r"\bIVAR\b(\s*)(?=\b(" + "|".join(_SECTIONS + _SPECS) + r")\b|\Z)", r"\1",
                  text)
    head = re.search(r"\bMODULE\s+main\b[^\n]*\n", text)
    return text[:head.end()] + "VAR\n" + "\n".join(decls) + "\n" + text[head.end():]
