"""The checks of contracts as the input files of the tools, for the tools
that one runs oneself (a tool under license, or another solver):

    from chase_cps import to_smv, to_smt
    open("c1.smv", "w").write(to_smv("verify", c1))          # nuXmv, NuSMV
    open("ref.smv", "w").write(to_smv("refines", c1, c2))    # c1 refines c2
    open("impl.smv", "w").write(to_smv("impl", model, c1))   # the model implements c1
    open("c1.smt2", "w").write(to_smt("verify", c1))         # z3, cvc5

A function is named after the language of the file. Its first argument is
the check, then come the arguments of the check, in the order of the
function of the check: ``verify`` (``verify(c)``), ``compat``
(``is_compatible(c)``), ``consist`` (``is_consistent(c)``), ``refines``
(``refines(c1, c2)``), ``impl`` (``is_implementation(m, c)``), ``env``
(``is_environment(e, c)``). ``tool`` names the tool when the files of two
tools differ. Each file holds the questions of the check, each with its
name:

- ``verify``: ``compat`` (the assumptions can hold), ``consist`` (the
  saturated guarantees can hold), and ``feasible`` (the assumptions and the
  guarantees can hold together);
- ``refines``: ``assumptions`` (those of c2 imply those of c1) and
  ``guarantees`` (the saturated guarantees of c1 imply those of c2);
- ``impl`` and ``env``: one question, of the same name.

A comment above each question says how to read the answer of the tool.
``to_c`` writes the check ``impl`` of a C function; ``to_gr1`` writes the
realizability question of a contract, ``to_sva`` a contract as a
SystemVerilog checker, ``to_spec`` contracts as a specification file.
"""

from __future__ import annotations

from . import ltl
from .contract import Contract
from .queries import Holds, Implies, Satisfiable

CHECKS = {"verify": 1, "compat": 1, "consist": 1, "refines": 2, "impl": 2, "env": 2}
SMV_TOOLS = ("nuxmv", "nusmv")
VALID = "true: valid; false: the counterexample is a behavior that breaks it"


def questions(check: str, *args) -> tuple:
    """(title, [(name, query)]): the questions of a check on its arguments."""
    if check not in CHECKS:
        raise ValueError(f"check {check!r}: one of {', '.join(CHECKS)}")
    if len(args) != CHECKS[check]:
        raise ValueError(f"{check} takes {CHECKS[check]} argument(s), got {len(args)}")
    if check in ("verify", "compat", "consist"):
        c = args[0]
        v = c.variables
        asked = {"compat": Satisfiable(c.assumption, v),
                 "consist": Satisfiable(c.saturated_guarantee, v),
                 "feasible": Satisfiable(ltl.Binary("&", c.assumption, c.guarantee), v)}
        names = ["compat", "consist", "feasible"] if check == "verify" else [check]
        return f"{check} {c.name}", [(n, asked[n]) for n in names]
    if check == "refines":
        from .checks import _merge_vars
        c1, c2 = args
        v = _merge_vars(c1, c2)
        return f"{c1.name} refines {c2.name}", [
            ("assumptions", Implies(c2.assumption, c1.assumption, v)),
            ("guarantees", Implies(c1.saturated_guarantee, c2.saturated_guarantee, v))]
    from .checks import _formula, holds_query
    from .models import as_model
    machine, c = args
    m = as_model(machine, c)
    if check == "impl":
        title = f"{getattr(m, 'name', 'the formula')} implements {c.name}"
        if m is None:
            return title, [("impl", Implies(ltl.Binary("&", _formula(machine), c.assumption),
                                            c.guarantee, c.variables))]
        return title, [("impl", holds_query(m, c.saturated_guarantee, c,
                                            f"the contract {c.name}"))]
    title = f"{getattr(m, 'name', 'the formula')} is an environment of {c.name}"
    if m is None:
        return title, [("env", Implies(_formula(machine), c.assumption, c.variables))]
    return title, [("env", holds_query(m, c.assumption, c, f"the assumptions of {c.name}"))]


def to_smv(check: str, *args, tool: str = "nuxmv") -> str:
    """The check as an SMV model with one named LTLSPEC per question; for
    ``impl`` and ``env`` of a state machine, its model with the LTLSPEC. A
    satisfiability question is written as the negation: the specification
    is false, with a counterexample that is a witness, when the formula can
    hold. NuSMV takes no real variables and no numbers with a fraction."""
    from .backends.nusmv import _has_reals, _history, _rename, _smv_type
    from . import words
    if tool not in SMV_TOOLS:
        raise ValueError(f"tool {tool!r}: one of {', '.join(SMV_TOOLS)}")
    title, asked = questions(check, *args)
    if isinstance(asked[0][1], Holds):
        return _model_check(title, *asked[0], tool)
    reals = any(_has_reals(q) for _, q in asked)
    if reals and tool == "nusmv":
        raise ValueError("NuSMV takes no real variables or numbers with a fraction: tool='nuxmv'")
    specs, variables, renamed = [], {}, {}
    for name, q in asked:
        q, back = _rename(q)
        variables.update(q.variables)
        renamed.update(back)
        if isinstance(q, Implies):
            specs.append((name, ltl.Binary("->", q.phi, q.psi), VALID))
        else:
            specs.append((name, ltl.Unary("!", q.phi),
                          "false: the formula can hold, the counterexample is a behavior that "
                          "satisfies it; true: it cannot hold"))
    used = set().union(*(ltl.variables(s) for _, s, _ in specs))
    run = "nuXmv -int FILE, then go_msat and check_ltlspec_ic3 (real numbers)" if reals \
        else ("nuXmv FILE" if tool == "nuxmv" else "NuSMV FILE")
    lines = [f"-- {title}, written by CHASE (chase_cps.to_smv)",
             f"-- Run: {run}"]
    if renamed:
        lines.append("-- Renamed (keywords of SMV): " +
                     ", ".join(f"{old} is {new}" for new, old in sorted(renamed.items())))
    lines += ["MODULE main", "VAR"]
    header = len(lines)
    for v, t in variables.items():
        if v in used:
            lines.append(f"    {v} : {_smv_type(t)};")
    if len(lines) == header:
        lines.append("    chase_dummy : boolean;")
    if reals and "real" not in {variables[v] for v in used}:
        lines.append("    chase_real : real;")      # the SMT engine needs an infinite state
    lines += _history(ltl.conjunction([s for _, s, _ in specs]), variables)
    for name, spec, meaning in specs:
        lines += [f"-- {name}: {meaning}",
                  f"LTLSPEC NAME {name} := {words.to_smv(spec, variables)};"]
    return "\n".join(lines) + "\n"


def to_smt(check: str, *args) -> str:
    """The check in SMT-LIB 2, for formulas that read a bounded number of
    steps (no G, F, U, R; x at step k is x@k): one block per question, each
    ended by (check-sat) after an (echo) of its name. The file is standard:
    z3 and cvc5 read it. Needs z3 (``pip install chase-cps[z3]``) to be
    written. A state machine (impl, env) is written by ``to_smv``."""
    from .backends import z3 as backend_z3
    title, asked = questions(check, *args)
    for _, q in asked:
        if isinstance(q, Holds):
            raise ValueError("a state machine is written in SMV: to_smv")
        reason = backend_z3.why_not(q)
        if reason:
            raise ValueError(reason)
    out = [f"; {title}, written by CHASE (chase_cps.to_smt)"]
    for i, (name, q) in enumerate(asked):
        e, f = backend_z3.Z3Adapter().problem(q)
        s = backend_z3._z3.Solver()
        s.add(f, *e.bounds())
        meaning = "unsat: valid; sat: the model is a counterexample" \
            if isinstance(q, Implies) else "sat: the formula can hold; unsat: it cannot"
        body = [l for l in s.to_smt2().splitlines()
                if l and not l.startswith(";") and l != "(check-sat)"]
        out += ([] if i == 0 else ["(reset)"]) + \
            [f"; {name}: {meaning}", "(set-logic ALL)"] + body + \
            [f'(echo "{name}")', "(check-sat)"]
    return "\n".join(out) + "\n"


def _model_check(title: str, name: str, query: Holds, tool: str) -> str:
    """The model of the state machine with the LTLSPEC of the question."""
    from .backends.nusmv import NuSMVAdapter, _has_reals
    if tool == "nusmv" and _has_reals(query):
        raise ValueError("NuSMV takes no real variables or numbers with a fraction: tool='nuxmv'")
    text = NuSMVAdapter().emit(query)
    head, _, spec = text.rpartition("LTLSPEC ")
    run = "nuXmv FILE" if tool == "nuxmv" else "NuSMV FILE"
    return (f"-- {title}, written by CHASE (chase_cps.to_smv)\n-- Run: {run}\n" + head +
            f"-- {name}: {VALID}\nLTLSPEC NAME {name} := {spec}")
