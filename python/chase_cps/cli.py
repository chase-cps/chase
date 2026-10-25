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
import sys
import textwrap

from . import __version__, checks, tools
from .backends import ADAPTERS
from .contract import Contract, load_spec
from .queries import REALIZABLE, SAT, VALID

# What each back end is needed for.
NEEDED_FOR = {
    "nuxmv": "refines, consistent, compatible, implies, satisfiable, simplify, "
             "contains_environment, contains_implementation",
    "nusmv": "the same checks as nuXmv (nuXmv is used first when both are installed)",
    "slugs": "realizable (GR(1) realizability, controllers, counter-strategies)",
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
    if model_checker:   # one model checker is enough
        missing = [n for n in missing if n not in ("nuxmv", "nusmv")]
    if missing:
        print("\nTo install the missing back ends:")
        for name in missing:
            print(f"  {name}: {_how_to_install(name)}")
    if not model_checker:
        print("\nThe checks on contracts need nuXmv or NuSMV: install one of them.")
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
    """The contracts of a specification (logics language) or of a JSON file
    (one contract, a list of contracts, or {"contracts": [...]})."""
    if not os.path.isfile(path):
        raise ValueError(f"no such file: {path}")
    if path.endswith(".json"):
        with open(path) as f:
            data = json.load(f)
        if isinstance(data, dict):
            data = data.get("contracts", [data])
        contracts = [Contract.from_dict(d) for d in data]
        return {c.name: c for c in contracts}
    return load_spec(path)


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
    if not (args.compatible or args.consistent or args.refines or args.realizable):
        args.compatible = args.consistent = True
    options = {"timeout": args.timeout}
    for name in every if args.compatible or args.consistent else []:
        c = _contract(name, contracts)
        if args.compatible:
            out.append(("compatible", name,
                        lambda c=c: checks.compatible(c, args.backend, **options)))
        if args.consistent:
            out.append(("consistent", name,
                        lambda c=c: checks.consistent(c, args.backend, **options)))
    for refined, abstract in args.refines or []:
        r, a = _contract(refined, contracts), _contract(abstract, contracts)
        out.append(("refines", f"{refined} <= {abstract}",
                    lambda r=r, a=a: checks.refines(r, a, args.backend, **options)))
    for name in args.realizable or []:
        c = _contract(name, contracts)
        out.append(("realizable", name, lambda c=c: checks.realizable(c, **options)))
    return out


def _passed(kind: str, result) -> bool:
    return result.status == {"refines": VALID, "realizable": REALIZABLE}.get(kind, SAT)


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
    else:
        if result.status == "unrealizable" and result.counter_strategy:
            print("  counter-strategy of the environment:")
            print(_indent(result.counter_strategy))
        if result.reason:
            print(f"  {result.reason}")


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
        "CHASE, or of a JSON file. Without options, checks the compatibility and the "
        "consistency of every contract. A contract argument can be NAME or NAME+NAME+... "
        "for the composition of the contracts. The exit status is 0 when every check "
        "passes, 1 when a check fails or does not finish, and 2 on an error in the input.")
    p.add_argument("file")
    p.add_argument("--contracts", nargs="+", metavar="CONTRACT",
                   help="the contracts for --compatible and --consistent (default: all)")
    p.add_argument("--compatible", action="store_true", help="some environment satisfies A")
    p.add_argument("--consistent", action="store_true", help="some behavior satisfies A -> G")
    p.add_argument("--refines", nargs=2, action="append", metavar=("REFINED", "ABSTRACT"),
                   help="REFINED refines ABSTRACT (may be repeated)")
    p.add_argument("--realizable", action="append", metavar="CONTRACT",
                   help="GR(1) realizability with slugs (may be repeated)")
    p.add_argument("--backend", default="auto", choices=["auto", "nuxmv", "nusmv"])
    p.add_argument("--timeout", type=float, default=300, help="seconds per check")
    p.add_argument("--json", action="store_true", help="print the results as JSON")
    p.set_defaults(run=check)

    args = parser.parse_args(argv)
    return args.run(args)


if __name__ == "__main__":
    sys.exit(main())
