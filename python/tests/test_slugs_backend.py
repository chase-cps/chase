"""The slugs back end writes each term faithfully.

Each contract is written for slugs, the file is compiled with the slugs
compiler, read back as LTL (INIT terms as they are, TRANS terms under G,
LIVENESS terms under G F), and nuXmv checks that the assumptions and the
guarantees read back are equivalent to those of the contract. Set
CHASE_SLUGS_FUZZ to the number of random contracts to check (default 40).
"""

import os
import random
import subprocess
import sys
import tempfile

import pytest

from chase_cps import implies, ltl
from chase_cps.backends import ADAPTERS
from chase_cps.contract import Contract
from chase_cps.queries import Realizable

slugs = ADAPTERS["slugs"]
pytestmark = pytest.mark.skipif(
    not (slugs.available() and (ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available())),
    reason="slugs and nuXmv/NuSMV are needed")

INPUTS = {"a": "bool", "n": (0, 2)}
OUTPUTS = {"b": "bool", "m": (0, 2)}


def read_back(text: str) -> dict[str, list[ltl.Node]]:
    sections, current = {}, None
    for line in text.splitlines():
        line = line.strip()
        if not line:
            continue
        if line.startswith("[") and line.endswith("]"):
            current = line[1:-1]
            sections[current] = []
        elif current not in ("INPUT", "OUTPUT"):
            sections[current].append(ltl.parse(line))
    parts = {"ENV": [], "SYS": []}
    for side in parts:
        parts[side] += sections.get(f"{side}_INIT", [])
        parts[side] += [ltl.Unary("G", f) for f in sections.get(f"{side}_TRANS", [])]
        parts[side] += [ltl.Unary("G", ltl.Unary("F", f))
                        for f in sections.get(f"{side}_LIVENESS", [])]
    return parts


def equivalent(phi, psi, variables) -> bool:
    return (implies(phi, psi, variables).status == "valid"
            and implies(psi, phi, variables).status == "valid")


def check_faithful(contract: Contract) -> None:
    text = slugs.emit(Realizable(contract))
    with tempfile.TemporaryDirectory() as tmp:
        path = os.path.join(tmp, "spec.structuredSlugs")
        with open(path, "w") as f:
            f.write(text)
        compiled = subprocess.run([sys.executable, slugs.compiler(), path],
                                  capture_output=True, text=True)
    assert compiled.returncode == 0 and compiled.stdout.strip(), (text, compiled.stderr)
    parts = read_back(text)
    v = contract.variables
    assert equivalent(ltl.conjunction(parts["ENV"]), contract.assumption, v), text
    assert equivalent(ltl.conjunction(parts["SYS"]), contract.guarantee, v), text


CASES = [
    (["a", "G F a"], ["!b", "G (a -> X b)", "G F b"]),
    (["true"], ["G (b | false)", "G F true"]),
    (["!a & n = 0"], ["m = 2 | b", "G (n' = n -> m' != m)", "G F (m = 1 & b)"]),
    (["G (n < 2 -> n' >= n)"], ["G (n + 1 = m' | !b')", "G (a <-> X !b)"]),
    (["G F (a -> X !a)"], ["G ((a & b) | (!a & !b) | m > n)", "G F (b <-> a)"]),
    (["G (true -> a)"], ["G (false -> b)", "true", "!false"]),
]


@pytest.mark.parametrize("assumptions, guarantees", CASES)
def test_terms_are_written_faithfully(assumptions, guarantees):
    check_faithful(Contract.from_strings(inputs=INPUTS, outputs=OUTPUTS,
                                         assumptions=assumptions, guarantees=guarantees,
                                         name="T"))


def _atom(rnd: random.Random, primed: bool) -> str:
    def var(v):
        return f"{v}'" if primed and rnd.random() < 0.4 else v
    k = rnd.randint(0, 2)
    return rnd.choice([var("a"), var("b"), "true", "false", f"{var('n')} = {k}",
                       f"{var('m')} != {k}", f"{var('n')} < {var('m')}",
                       f"{var('n')} + 1 = {var('m')}", f"{var('m')} >= {k}"])


def _formula(rnd: random.Random, depth: int, primed: bool) -> str:
    if depth == 0 or rnd.random() < 0.3:
        return _atom(rnd, primed)
    if rnd.random() < 0.2:
        return f"!({_formula(rnd, depth - 1, primed)})"
    op = rnd.choice(["&", "|", "->", "<->"])
    return f"({_formula(rnd, depth - 1, primed)}) {op} ({_formula(rnd, depth - 1, primed)})"


def _term(rnd: random.Random) -> str:
    kind = rnd.choice(["init", "safety", "justice"])
    if kind == "init":
        return _formula(rnd, 2, False)
    if kind == "safety":
        return f"G ({_formula(rnd, 2, True)})"
    return f"G F ({_formula(rnd, 2, rnd.random() < 0.5)})"


def test_random_contracts_are_written_faithfully():
    rnd = random.Random(2026)
    for _ in range(int(os.environ.get("CHASE_SLUGS_FUZZ", "40"))):
        c = Contract.from_strings(inputs=INPUTS, outputs=OUTPUTS,
                                  assumptions=[_term(rnd) for _ in range(rnd.randint(1, 2))],
                                  guarantees=[_term(rnd) for _ in range(rnd.randint(1, 3))],
                                  name="T")
        check_faithful(c)
