"""Command line of the chase_cps package.

    chase-cps doctor              the CHASE library and the back ends found, and
                                  how to install the missing ones
    chase-cps install TOOL        install nuxmv, nusmv, or slugs in ~/.chase
    chase-cps check FILE [...]    check the contracts of a specification

``python -m chase_cps`` runs the same commands.
"""

from __future__ import annotations

import argparse
import json
import os
import platform
import re
import sys
from typing import Optional
import textwrap

from . import __version__, checks, gr1, ltl, models, tools
from .backends import ADAPTERS
from .contract import Contract, load_spec
from .queries import REALIZABLE, SAT, VALID

# What each back end is needed for.
NEEDED_FOR = {
    "nuxmv": "refines, verify, is_consistent, is_compatible, implies, is_satisfiable, "
             "is_implementation, is_environment, is_receptive, remove_redundant",
    "nusmv": "the same checks, where nuXmv cannot be used (optional)",
    "slugs": "is_realizable and synthesize (GR(1) realizability and synthesis), "
             "is_well_separated, is_vacuous",
}


def _location(name: str) -> str | None:
    adapter = ADAPTERS[name]
    if not adapter.available():
        return None
    found = adapter.binary()
    return found[0] if isinstance(found, tuple) else found


def _how_to_install(name: str) -> str:
    if tools.installable(name):
        flag = " --accept-license" if name == "nuxmv" else ""
        note = " (academic and non-commercial use only)" if name == "nuxmv" else ""
        return f"chase-cps install {name}{flag}{note}"
    return tools.guideline(name)


# doctor ----------------------------------------------------------------------

def doctor(args=None) -> int:
    print(f"chase-cps {__version__}, Python {platform.python_version()}, "
          f"{platform.system()} {platform.machine()}")
    ok = True
    try:
        from . import _native
        print(f"CHASE library: found ({_native.pychase.__file__})")
    except ImportError as e:
        ok = False
        print(f"CHASE library: not found ({e})")

    print("\nBack ends:")
    missing = []
    for name in ADAPTERS:
        where = _location(name)
        print(f"  {name:6} {'found ' + where if where else 'not found'}")
        print(f"         needed for {NEEDED_FOR.get(name, 'its queries')}")
        if not where:
            missing.append(name)

    model_checker = any(_location(n) for n in ("nuxmv", "nusmv"))
    missing = [n for n in missing if n != "nusmv"]      # NuSMV is optional
    if model_checker:   # one model checker is enough
        missing = [n for n in missing if n != "nuxmv"]
    if missing:
        print("\nTo install the missing back ends:")
        for name in missing:
            print(f"  {name}: {_how_to_install(name)}")
    if not model_checker:
        print("\nThe checks on contracts need nuXmv (or NuSMV where nuXmv cannot be used).")
    return 0 if ok and model_checker else 1


# install ---------------------------------------------------------------------

def install(args) -> int:
    try:
        tools.install(args.tool, accept_license=args.accept_license)
    except tools.InstallError as e:
        print(f"chase-cps install {args.tool}: {e}", file=sys.stderr)
        return 1
    return 0


# check -----------------------------------------------------------------------

def _read(path: str) -> dict[str, Contract]:
    """The contracts of a specification (logics language), of a JSON file
    (one contract, a list of contracts, or {"contracts": [...]}), or of a
    SystemVerilog file (.sv, .sva: its assumptions and assertions)."""
    if not os.path.isfile(path):
        raise ValueError(f"no such file: {path}")
    ext = os.path.splitext(path)[1].lower()
    if ext == ".json":
        with open(path) as f:
            data = json.load(f)
        if isinstance(data, dict):
            data = data.get("contracts", [data])
        contracts = [Contract.from_dict(d) for d in data]
        return {c.name: c for c in contracts}
    if ext in (".sv", ".sva", ".svh"):
        return _read_sva(path)
    if ext in (".smv", ".v", ".vhd", ".py"):
        raise ValueError(f"{os.path.basename(path)} is not a specification: CHASE reads contracts "
                         "from .ltl (the logics language), .json, and .sv files; a model (.smv, "
                         "RTL) is drawn as the implementation of a contract")
    return load_spec(path)


def _place(text: str, word: str) -> Optional[tuple[int, int]]:
    """The line (from 1) and column (from 0) where word first appears."""
    pattern = re.compile(r"(?<![\w$])" + re.escape(word) + r"(?!\w)")
    for number, line in enumerate(text.splitlines(), 1):
        m = pattern.search(line.split("//", 1)[0])
        if m:
            return number, m.start()
    return None


_TOKEN = re.compile(r"[A-Za-z_$][\w$]*|\d+'?\w*|&&|\|\||[^\s\w]")


def _statement_line(text: str, statement: str) -> Optional[int]:
    """The line (from 1) where the tokens of statement appear in text; of
    several places, one not read as a statement of its own (whose first word
    is not assert, assume, or cover)."""
    tokens = [(m.group(0), n) for n, line in enumerate(text.splitlines(), 1)
              for m in _TOKEN.finditer(line.split("//", 1)[0])]
    want = _TOKEN.findall(statement)
    if not want:
        return None
    found = [i for i in range(len(tokens) - len(want) + 1)
             if [t for t, _ in tokens[i:i + len(want)]] == want]
    if not found:
        return None
    alone = [i for i in found if i == 0 or tokens[i - 1][0] not in ("assert", "assume", "cover")]
    return tokens[(alone or found)[0]][1]


def _read_sva(path: str) -> dict[str, Contract]:
    """The contract of a SystemVerilog file. A signal whose direction is not
    known is placed in the file; a statement that is not read is said on the
    standard error, with its line, as a warning."""
    from .sva import SVAError, parse_sva
    with open(path, encoding="utf-8") as f:
        text = f.read()
    try:
        reading = parse_sva(text)
    except SVAError as e:
        m = re.search(r"\['([\w.$]+)'", str(e))
        where = _place(text, m.group(1)) if m else None
        raise ValueError(f"line {where[0]}:{where[1]}: {e}" if where else str(e)) from None
    for k in reading.skipped:
        where = _statement_line(text, k.text)
        line = f"line {where}: " if where else ""
        print(f"warning: {line}not read ({k.reason}): {k.text}", file=sys.stderr)
    c = reading.contract
    return {c.name: c}


def _contract(expression: str, contracts: dict[str, Contract]) -> Contract:
    """A contract name, or names joined by + for their composition."""
    names = expression.split("+")
    for n in names:
        if n not in contracts:
            raise ValueError(f"no contract {n!r}; the contracts are {', '.join(contracts)}")
    result = contracts[names[0]]
    for n in names[1:]:
        result = result.compose(contracts[n])
    if len(names) > 1:
        result.name = expression.replace("+", "_")
    return result


def _checks(args, contracts):
    """(kind, label, function) for each check asked, in order."""
    out = []
    every = args.contracts or list(contracts)
    if not (args.verify or args.compatible or args.consistent or args.refines
            or args.implementation or args.environment or args.receptive or args.realizable or args.synthesize
            or args.well_separated or args.vacuous):
        args.verify = True
    options = {"timeout": args.timeout}
    game = dict(options)   # slugs has no engine to choose
    if args.engine != "auto":
        options["engine"] = args.engine
    for name in every if args.verify or args.compatible or args.consistent else []:
        c = _contract(name, contracts)
        if args.verify:
            out.append(("verify", name,
                        lambda c=c: checks.verify(c, args.backend, core=args.core, **options)))
        if args.compatible:
            out.append(("compatible", name,
                        lambda c=c: checks.is_compatible(c, args.backend, core=args.core,
                                                         **options)))
        if args.consistent:
            out.append(("consistent", name,
                        lambda c=c: checks.is_consistent(c, args.backend, core=args.core,
                                                         **options)))
    for refined, abstract in args.refines or []:
        r, a = _contract(refined, contracts), _contract(abstract, contracts)
        out.append(("refines", f"{refined} <= {abstract}",
                    lambda r=r, a=a: checks.refines(r, a, args.backend, **options)))
    for path, name in args.implementation or []:
        c = _contract(name, contracts)
        m = _model(path, c)
        out.append(("implementation", f"{m.name} |= {name}",
                    lambda m=m, c=c: checks.is_implementation(m, c, args.backend, **options)))
    for path, name in args.environment or []:
        c = _contract(name, contracts)
        m = _model(path, c)
        out.append(("environment", f"{m.name} |=E {name}",
                    lambda m=m, c=c: checks.is_environment(m, c, args.backend, **options)))
    for path, name in args.receptive or []:
        c = _contract(name, contracts)
        m = _model(path, c)
        out.append(("receptive", f"{m.name} for {name}",
                    lambda m=m, c=c: models.is_receptive(m, c, args.backend, **options)))
    for name in args.realizable or []:
        c = _contract(name, contracts)
        out.append(("realizable", name,
                    lambda c=c: gr1.is_realizable(c, core=args.core, **game)))
    for name in args.well_separated or []:
        c = _contract(name, contracts)
        out.append(("separated", name,
                    lambda c=c: gr1.is_well_separated(c, core=args.core, **game)))
    for name in args.vacuous or []:
        c = _contract(name, contracts)
        out.append(("vacuous", name,
                    lambda c=c: gr1.is_vacuous(c, core=args.core, **game)))
    for name in args.synthesize or []:
        c = _contract(name, contracts)
        out.append(("synthesize", name, lambda c=c: gr1.synthesize(c, **game)))
    return out


def _model(path: str, contract):
    """The model in an SMV file, or in a JSON file: a state machine, or a
    controller of slugs (synthesized from the contract)."""
    if not os.path.exists(path):
        raise ValueError(f"no file {path}")
    if path.endswith(".json"):
        with open(path) as f:
            data = json.load(f)
        if "nodes" in data:
            return models.Model.from_controller(data, contract,
                                                name=os.path.splitext(os.path.basename(path))[0])
        return models.Model.from_dict(data)
    return models.Model.from_smv(path)


def _passed(kind: str, result) -> bool:
    return result.status == {"refines": VALID, "implementation": VALID, "environment": VALID, "realizable": REALIZABLE,
                             "synthesize": REALIZABLE, "separated": "separated",
                             "receptive": "receptive", "vacuous": "not_vacuous"}.get(kind, SAT)


def _indent(text) -> str:
    return textwrap.indent(textwrap.dedent(str(text)).strip("\n"), "    ")


def _report(kind: str, label: str, result) -> None:
    seconds = (result.assumptions.seconds + result.guarantees.seconds
               if kind == "refines" else result.seconds)
    extra = f"  ({result.failed} fail)" if kind == "refines" and result.failed else ""
    print(f"{kind:11} {label:40} {result.status:12} {seconds:7.2f} s{extra}")
    if kind == "refines":
        part = result.assumptions if result.failed == "assumptions" else result.guarantees
        if result.failed and part.trace is not None:
            print(f"  counterexample ({result.failed}):")
            print(_indent(part.trace))
        for p in (result.assumptions, result.guarantees):
            if p.reason:
                print(f"  {p.reason}")
    elif kind == "vacuous":
        for v in result.vacuous:
            print(f"  {v.output}: a winning strategy "
                  + ("never sets it" if v.mode == "never" else "stops setting it"))
            if v.core is not None and v.core.status == "found":
                for t in v.core.core:
                    print(f"    assumption {ltl.to_str(t)}")
        if result.reason:
            print(f"  {result.reason}")
    elif kind == "receptive" and result.status == "not_receptive":
        print(f"  refuses the input {result.details.get('input')} after the state "
              f"{result.details.get('state')}")
    elif kind == "verify":
        if result.status != SAT and result.feasible is not None:
            for part, r in (("compatible", result.compatible), ("consistent", result.consistent)):
                print(f"  {part:10} {r.status}")
            if result.feasible.core is not None:
                _report_core(result.feasible.core, guarantees_only=False)
        for r in (result.feasible, result.compatible, result.consistent):
            if r is not None and r.reason:
                print(f"  {r.reason}")
                break
    else:
        if result.controller is not None:
            nodes = result.controller.get("nodes", {}) if isinstance(result.controller, dict) else {}
            what = ("strategy forcing the environment to violate its assumptions"
                    if kind == "separated" else "controller")
            print(f"  {what}: {len(nodes)} states (use --json to print it)")
        if result.status == "unrealizable" and result.counter_strategy:
            print("  counter-strategy of the environment:")
            print(_indent(result.counter_strategy))
        if result.core is not None:
            # the unrealizable core keeps every assumption: show its guarantees
            _report_core(result.core, guarantees_only=kind == "realizable")
        if result.reason:
            print(f"  {result.reason}")


def _report_core(core, guarantees_only: bool) -> None:
    if core.status != "found":
        print(f"  core: {core.status}  {core.reason}".rstrip())
        return
    c = core.core
    print(f"  core ({core.checks} checks):")
    parts = [("assumption", c.assumptions)] if not guarantees_only else []
    for part, terms in parts + [("guarantee", c.guarantees)]:
        for t in terms:
            print(f"    {part:10} {ltl.to_str(t)}")


def check(args) -> int:
    try:
        contracts = _read(args.file)
        todo = _checks(args, contracts)
    except (ValueError, RuntimeError) as e:
        print(f"chase-cps check: {e}", file=sys.stderr)
        return 2
    results, passed = [], True
    for kind, label, run in todo:
        result = run()
        passed = passed and _passed(kind, result)
        if args.json:
            results.append({"check": kind, "contracts": label, **result.to_dict()})
        else:
            _report(kind, label, result)
    if args.json:
        print(json.dumps({"passed": passed, "checks": results}, indent=2))
    return 0 if passed else 1


def _quiet(run):
    """run(), with what the CHASE libraries print on the standard output sent
    to the standard error."""
    sys.stdout.flush()
    saved = os.dup(1)
    os.dup2(2, 1)
    try:
        return run()
    finally:
        sys.stdout.flush()
        os.dup2(saved, 1)
        os.close(saved)


def view(args) -> int:
    """Draws the contracts of a specification; with --serve, in a page that
    is drawn again whenever the file changes."""
    from . import viz
    checks = not args.no_checks

    def make() -> dict:
        if args.format == "json" and args.output in (None, "-"):    # the JSON alone on the standard output
            return _quiet(lambda: viz.from_spec(args.file, args.contracts or (), args.refines or (),
                                                args.environment or (), check=args.check and checks,
                                                verify=args.verify and checks,
                                                realizable=args.realizable and checks,
                                                timeout=args.timeout, open=args.open or ()))
        return viz.from_spec(args.file, args.contracts or (), args.refines or (),
                             args.environment or (), check=args.check and checks,
                             verify=args.verify and checks, realizable=args.realizable and checks,
                             timeout=args.timeout, open=args.open or ())

    if args.serve:
        return viz.serve(make, args.file, args.port, args.host, elk=args.elk)
    try:
        d = make()
    except (ValueError, RuntimeError) as e:
        print(f"chase-cps view: {e}", file=sys.stderr)
        return 2
    if args.format == "json":
        text = json.dumps(d)
        if args.output in (None, "-"):
            print(text)
            return 0
    else:
        text = viz.html(d, elk=args.elk)
    out = args.output or os.path.splitext(args.file)[0] + ".html"
    with open(out, "w", encoding="utf-8") as f:
        f.write(text)
    print(out)
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="chase-cps", description="CHASE command line")
    parser.add_argument("--version", action="version", version=f"chase-cps {__version__}")
    sub = parser.add_subparsers(dest="command", required=True)

    p = sub.add_parser("doctor", help="show the CHASE library and the back ends found")
    p.set_defaults(run=doctor)

    p = sub.add_parser("install", help="install a back end in ~/.chase (or $CHASE_HOME)")
    p.add_argument("tool", choices=tools.TOOLS)
    p.add_argument("--accept-license", action="store_true",
                   help="accept the license of nuXmv (academic and non-commercial use only)")
    p.set_defaults(run=install)

    p = sub.add_parser(
        "check", help="check the contracts of a specification",
        description="Check the contracts of a specification in the logics language of "
        "CHASE, or of a JSON file, as the commands verify, refines, and synthesize of "
        "logics_tool. Without options, verifies every contract: its assumptions and "
        "guarantees can hold together (A & G satisfiable). A contract argument can be NAME or NAME+NAME+... "
        "for the composition of the contracts. The exit status is 0 when every check "
        "passes, 1 when a check fails or does not finish, and 2 on an error in the input.")
    p.add_argument("file")
    p.add_argument("--contracts", nargs="+", metavar="CONTRACT",
                   help="the contracts for --verify, --compatible, and --consistent (default: all)")
    g = p.add_argument_group("checks on contracts (nuXmv)")
    g.add_argument("--verify", action="store_true",
                   help="the assumptions and the guarantees can hold together, A & G "
                        "satisfiable (the default)")
    g.add_argument("--compatible", action="store_true", help="some environment satisfies A")
    g.add_argument("--consistent", action="store_true", help="some behavior satisfies A -> G")
    g.add_argument("--refines", nargs=2, action="append", metavar=("REFINED", "ABSTRACT"),
                   help="REFINED refines ABSTRACT (may be repeated)")
    g.add_argument("--implementation", nargs=2, action="append", metavar=("MODEL", "CONTRACT"),
                   help="the state machine in MODEL (an SMV file, or a JSON file: a state "
                        "machine or a controller of slugs) is an implementation of CONTRACT")
    g.add_argument("--implements", nargs=2, action="append", dest="implementation",
                   metavar=("MODEL", "CONTRACT"), help=argparse.SUPPRESS)
    g.add_argument("--environment", nargs=2, action="append", metavar=("MODEL", "CONTRACT"),
                   help="the state machine in MODEL is an environment of CONTRACT: its "
                        "behaviors satisfy the assumptions")
    g.add_argument("--receptive", nargs=2, action="append", metavar=("MODEL", "CONTRACT"),
                   help="the state machine in MODEL accepts every input that the "
                        "assumptions of CONTRACT allow")
    g = p.add_argument_group("GR(1) synthesis (slugs)")
    g.add_argument("--realizable", action="append", metavar="CONTRACT",
                   help="GR(1) realizability (may be repeated)")
    g.add_argument("--synthesize", action="append", metavar="CONTRACT",
                   help="GR(1) synthesis of a controller (may be repeated)")
    g.add_argument("--well-separated", action="append", metavar="CONTRACT",
                   help="the component cannot win by forcing the environment to violate "
                        "its assumptions")
    g.add_argument("--vacuous", action="append", metavar="CONTRACT",
                   help="the component can win without an output that a liveness guarantee "
                        "asks for (vacuity in synthesis)")
    p.add_argument("--core", action="store_true",
                   help="when a check fails, give a minimal set of terms that explains it")
    p.add_argument("--backend", default="auto", choices=["auto", "nuxmv", "nusmv"])
    p.add_argument("--engine", default="auto",
                   choices=["auto", "ic3", "bdd", "bmc", "portfolio"],
                   help="engine of nuXmv or NuSMV: auto runs IC3 and BDDs at the same time "
                        "with nuXmv (portfolio); bdd is fast when a counterexample needs a "
                        "long loop")
    p.add_argument("--timeout", type=float, default=300, help="seconds per check")
    p.add_argument("--json", action="store_true", help="print the results as JSON")
    p.set_defaults(run=check)

    p = sub.add_parser(
        "view", help="draw the contracts of a specification",
        description="Draw the contracts of a specification in the logics language of CHASE, "
        "of a JSON file, or of SystemVerilog Assertions (.sv), as a web page. A contract argument can be NAME+NAME+... for the "
        "composition of the contracts, drawn as a box with the contracts inside. With "
        "--serve, the page is served on this computer and drawn again whenever the file "
        "changes.")
    p.add_argument("file")
    p.add_argument("--contracts", nargs="+", metavar="CONTRACT",
                   help="the contracts to draw (default: all)")
    p.add_argument("--refines", nargs=2, action="append", metavar=("REFINED", "ABSTRACT"),
                   help="draw ABSTRACT above REFINED (may be repeated)")
    p.add_argument("--environment", nargs=2, action="append", metavar=("MODEL", "CONTRACT"),
                   help="draw the state machine in MODEL as an environment of CONTRACT, on "
                        "its left (may be repeated)")
    p.add_argument("--check", action="store_true",
                   help="check the refinements and the environments, and mark them (nuXmv "
                        "or NuSMV)")
    p.add_argument("--verify", action="store_true",
                   help="check that each contract drawn is compatible and consistent; when "
                        "its terms cannot hold together, mark them")
    p.add_argument("--realizable", action="store_true",
                   help="check that each contract drawn is realizable (GR(1), slugs); when "
                        "it is not, mark the guarantees of the core")
    p.add_argument("-o", "--output", help="the page to write (default: FILE with .html)")
    p.add_argument("--open", nargs="+", metavar="CONTRACT",
                   help="draw these contracts open: their terms, and the core of a check "
                        "that fails")
    p.add_argument("--format", choices=("html", "json"), default="html",
                   help="the page (html), or the data of the diagram (json; on the standard "
                        "output without -o), for a page that draws it again in place")
    p.add_argument("--no-checks", action="store_true",
                   help="draw the structure only: leave out --check, --verify, and --realizable "
                        "(an editor draws as one types, and checks on save)")
    p.add_argument("--elk", metavar="PATH",
                   help="put elk.bundled.js (elkjs 0.9) from PATH in the page, for a page that "
                        "works offline (default: load it from a CDN)")
    p.add_argument("--serve", action="store_true",
                   help="serve the page, and draw it again when the file changes")
    p.add_argument("--port", type=int, default=8765)
    p.add_argument("--host", default="127.0.0.1",
                   help="the address to serve on (0.0.0.0 in a Docker container)")
    p.add_argument("--timeout", type=float, default=60, help="seconds per check")
    p.set_defaults(run=view)

    args = parser.parse_args(argv)
    return args.run(args)


if __name__ == "__main__":
    sys.exit(main())
