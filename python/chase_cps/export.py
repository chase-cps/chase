"""The checks of a contract as the input files of the tools, for the tools
that one runs oneself (a tool under license, or another solver):

    from chase_cps import to_smv, to_smt
    open("gen.smv", "w").write(to_smv(gen))                  # nuXmv, NuSMV
    open("gen.smv", "w").write(to_smv(gen, refines=spec))
    open("gen.smt2", "w").write(to_smt(gen))              # z3, cvc5

The functions are named after the language of the file; ``tool`` names the
tool when the files of two tools differ (``to_smv``, ``to_gr1``). Each file
holds the questions of one check, each with its name:

- a contract alone: ``compatible`` (the assumptions can hold),
  ``consistent`` (the saturated guarantees can hold), and ``feasible`` (the
  assumptions and the guarantees can hold together), as ``verify``;
- with ``refines=abstract``: ``assumptions`` (the assumptions of
  ``abstract`` imply those of the contract) and ``guarantees`` (its
  saturated guarantees imply those of ``abstract``), as ``refines``.

A comment above each question says how to read the answer of the tool.
``to_gr1`` writes the realizability question, ``to_sva`` the contract as a
SystemVerilog checker, ``to_spec`` contracts as a specification file.
"""

from __future__ import annotations

from typing import Optional

from . import ltl
from .contract import Contract
from .queries import Implies, Satisfiable

SMV_TOOLS = ("nuxmv", "nusmv")


def questions(contract: Contract, refines: Optional[Contract] = None) -> list:
    """(name, query) for each question of the check: verify, or refines."""
    if refines is None:
        v = contract.variables
        return [("compatible", Satisfiable(contract.assumption, v)),
                ("consistent", Satisfiable(contract.saturated_guarantee, v)),
                ("feasible", Satisfiable(ltl.Binary("&", contract.assumption,
                                                    contract.guarantee), v))]
    from .checks import _merge_vars
    v = _merge_vars(contract, refines)
    return [("assumptions", Implies(refines.assumption, contract.assumption, v)),
            ("guarantees", Implies(contract.saturated_guarantee, refines.saturated_guarantee, v))]


def _title(contract: Contract, refines: Optional[Contract]) -> str:
    if refines is None:
        return f"verify {contract.name}"
    return f"{contract.name} refines {refines.name}"


def to_smv(contract: Contract, refines: Optional[Contract] = None, tool: str = "nuxmv") -> str:
    """The check as an SMV model with one named LTLSPEC per question. A
    satisfiability question is written as the negation: the specification
    is false, with a counterexample that is a witness, when the formula can
    hold. NuSMV takes no real variables and no numbers with a fraction."""
    from .backends.nusmv import _has_reals, _history, _rename, _smv_type
    from . import words
    if tool not in SMV_TOOLS:
        raise ValueError(f"tool {tool!r}: one of {', '.join(SMV_TOOLS)}")
    asked = questions(contract, refines)
    reals = any(_has_reals(q) for _, q in asked)
    if reals and tool == "nusmv":
        raise ValueError("NuSMV takes no real variables or numbers with a fraction: tool='nuxmv'")
    specs, variables, renamed = [], {}, {}
    for name, q in asked:
        q, back = _rename(q)
        variables.update(q.variables)
        renamed.update(back)
        if isinstance(q, Implies):
            specs.append((name, ltl.Binary("->", q.phi, q.psi),
                          "true: valid; false: the counterexample is a behavior that breaks it"))
        else:
            specs.append((name, ltl.Unary("!", q.phi),
                          "false: the formula can hold, the counterexample is a behavior that "
                          "satisfies it; true: it cannot hold"))
    used = set().union(*(ltl.variables(s) for _, s, _ in specs))
    run = "nuXmv -int FILE, then go_msat and check_ltlspec_ic3 (real numbers)" if reals \
        else ("nuXmv FILE" if tool == "nuxmv" else "NuSMV FILE")
    lines = [f"-- {_title(contract, refines)}, written by CHASE (chase_cps.to_smv)",
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


def to_smt(contract: Contract, refines: Optional[Contract] = None) -> str:
    """The check in SMT-LIB 2, for a contract whose formulas read a bounded
    number of steps (no G, F, U, R; x at step k is x@k): one block per
    question, each ended by (check-sat) after an (echo) of its name. The
    file is standard: z3 and cvc5 read it. Needs z3 (``pip install
    chase-cps[z3]``) to be written."""
    from .backends import z3 as backend_z3
    asked = questions(contract, refines)
    for _, q in asked:
        reason = backend_z3.why_not(q)
        if reason:
            raise ValueError(reason)
    out = [f"; {_title(contract, refines)}, written by CHASE (chase_cps.to_smt)"]
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
