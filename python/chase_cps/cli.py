"""Command line of the chase_cps package.

    chase-cps doctor     the CHASE library and the back ends found, and how to
                         install the missing ones

``python -m chase_cps`` runs the same command.
"""

from __future__ import annotations

import argparse
import platform
import sys

from . import __version__
from .backends import ADAPTERS

# What each back end is needed for, and how to install it.
NEEDED_FOR = {
    "nuxmv": "refines, consistent, compatible, implies, satisfiable, simplify, "
             "contains_environment, contains_implementation",
    "nusmv": "the same checks as nuXmv (nuXmv is used first when both are installed)",
    "slugs": "realizable (GR(1) realizability, controllers, counter-strategies)",
}

INSTALL = {
    "nuxmv": """\
Download nuXmv from https://nuxmv.fbk.eu/download.html (Linux x86_64 and
macOS arm64; free for non-commercial and academic use), unpack it, and put
bin/nuXmv in the PATH or set CHASE_NUXMV to its path.""",
    "nusmv": """\
Download NuSMV from https://nusmv.fbk.eu (Linux x86_64 and macOS x86_64; on
Apple silicon it runs with Rosetta), unpack it, and put bin/NuSMV in the
PATH or set CHASE_NUSMV to its path.""",
    "slugs": """\
Build slugs from https://github.com/VerifiableRobotics/slugs (make in src/),
and put src/slugs in the PATH or set CHASE_SLUGS to its path. The compiler
tools/StructuredSlugsParser/compiler.py is found next to it, or set
SLUGSCOMPILER. The CHASE Docker image contains slugs.""",
}


def _location(name: str) -> str | None:
    adapter = ADAPTERS[name]
    if not adapter.available():
        return None
    found = adapter.binary()
    return found[0] if isinstance(found, tuple) else found


def doctor() -> int:
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
            print(f"\n{name}:")
            print("  " + INSTALL[name].replace("\n", "\n  "))
    if not model_checker:
        print("\nThe checks on contracts need nuXmv or NuSMV: install one of them.")
    return 0 if ok and model_checker else 1


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="chase-cps", description="CHASE command line")
    parser.add_argument("--version", action="version", version=f"chase-cps {__version__}")
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("doctor", help="show the CHASE library and the back ends found")
    args = parser.parse_args(argv)
    if args.command == "doctor":
        return doctor()
    return 2


if __name__ == "__main__":
    sys.exit(main())
