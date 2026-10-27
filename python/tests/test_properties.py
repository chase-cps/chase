"""Properties of the operations and analyses on random contracts.

Each test draws random contracts over a few Boolean variables and checks a
law of the contract algebra or a defining property of an analysis with
nuXmv (and slugs for the GR(1) ones). Set CHASE_PROPERTY_RUNS to the number
of random cases per test (default 12).
"""

import os
import random

import pytest

from chase_cps import (Contract, is_realizable, is_satisfiable, is_well_separated, ltl,
                       refines, remove_vacuous, unrealizable_core, unsat_core)
from chase_cps.backends import ADAPTERS
from chase_cps.cores import gr1_kind

HAS_MC = ADAPTERS["nuxmv"].available() or ADAPTERS["nusmv"].available()
HAS_SLUGS = ADAPTERS["slugs"].available()
try:
    from chase_cps import _native  # noqa: F401
    HAS_NATIVE = True
except ImportError:
    HAS_NATIVE = False

needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv/NuSMV not installed")
needs_native = pytest.mark.skipif(not HAS_NATIVE, reason="CHASE modules not installed")
needs_slugs = pytest.mark.skipif(not (HAS_SLUGS and HAS_MC), reason="slugs and nuXmv/NuSMV are needed")

RUNS = int(os.environ.get("CHASE_PROPERTY_RUNS", "12"))
INPUTS = {"a": "bool", "c": "bool"}
OUTPUTS = {"b": "bool", "d": "bool"}


def same(x: Contract, y: Contract) -> bool:
    return refines(x, y).status == "valid" and refines(y, x).status == "valid"


def holds(x: Contract, y: Contract) -> bool:
    return refines(x, y).status == "valid"


# Random LTL ------------------------------------------------------------------

def _prop(rnd, names):
    v = rnd.choice(names)
    return v if rnd.random() < 0.6 else f"!{v}"


def _ltl(rnd, names, depth):
    if depth == 0 or rnd.random() < 0.25:
        return _prop(rnd, names)
    k = rnd.random()
    if k < 0.15:
        return f"G ({_ltl(rnd, names, depth - 1)})"
    if k < 0.3:
        return f"F ({_ltl(rnd, names, depth - 1)})"
    if k < 0.4:
        return f"X ({_ltl(rnd, names, depth - 1)})"
    if k < 0.5:
        return f"({_ltl(rnd, names, depth - 1)}) U ({_ltl(rnd, names, depth - 1)})"
    op = rnd.choice(["&", "|", "->"])
    return f"({_ltl(rnd, names, depth - 1)}) {op} ({_ltl(rnd, names, depth - 1)})"


def contract(rnd, name, inputs=INPUTS, outputs=OUTPUTS, depth=2):
    names = list(inputs) + list(outputs)
    return Contract.from_strings(
        inputs=inputs, outputs=outputs, name=name,
        assumptions=[_ltl(rnd, list(inputs), depth) for _ in range(rnd.randint(1, 2))],
        guarantees=[_ltl(rnd, names, depth) for _ in range(rnd.randint(1, 2))])


# Random GR(1) ----------------------------------------------------------------

def _boolean(rnd, names, primed, depth=1):
    """primed: the names that may be read in the next step."""
    def atom():
        v = rnd.choice(names)
        v = f"{v}'" if v in primed and rnd.random() < 0.5 else v
        return v if rnd.random() < 0.6 else f"!{v}"
    if depth == 0 or rnd.random() < 0.4:
        return atom()
    op = rnd.choice(["&", "|", "->"])
    return f"({_boolean(rnd, names, primed, depth - 1)}) {op} ({_boolean(rnd, names, primed, depth - 1)})"


def _gr1_term(rnd, environment):
    """A GR(1) term with the variable roles of slugs: the initial assumptions
    read the inputs only, the safety assumptions read the next inputs only."""
    names, inputs = list(INPUTS) + list(OUTPUTS), list(INPUTS)
    k = rnd.choice(["init", "safety", "safety", "justice"])
    if k == "init":
        return _boolean(rnd, inputs if environment else names, ())
    if k == "safety":
        return f"G ({_boolean(rnd, names, inputs if environment else names)})"
    return f"G F ({_boolean(rnd, names, ())})"


def gr1_contract(rnd, name, n_assumptions=(1, 3), n_guarantees=(2, 5)):
    return Contract.from_strings(
        inputs=INPUTS, outputs=OUTPUTS, name=name,
        assumptions=[_gr1_term(rnd, True) for _ in range(rnd.randint(*n_assumptions))],
        guarantees=[_gr1_term(rnd, False) for _ in range(rnd.randint(*n_guarantees))])


# Algebra ---------------------------------------------------------------------

@needs_native
@needs_mc
def test_implication_and_coimplication_are_adjoints():
    rnd = random.Random(1)
    for _ in range(RUNS):
        c1, c2, e = (contract(rnd, n) for n in ("Cone", "Ctwo", "Env"))
        i, k = c1.imply(c2), c1.coimply(c2)
        # C1 -> C2 is the largest C with C & C1 <= C2
        assert holds(i.conjoin(c1), c2)
        assert holds(c2, i)                          # C2 & C1 <= C2
        if holds(e.conjoin(c1), c2):
            assert holds(e, i)
        # C1 ~> C2 is the smallest C with C2 <= C | C1
        assert holds(c2, k.disjoin(c1))
        assert holds(k, c2)                          # C2 <= C2 | C1
        if holds(c2, e.disjoin(c1)):
            assert holds(k, e)


@needs_native
@needs_mc
def test_reciprocal_identities():
    rnd = random.Random(2)
    for _ in range(RUNS):
        c1, c2 = contract(rnd, "Cone"), contract(rnd, "Ctwo")
        assert same(c1.invert().invert(), c1)
        assert same(c1.divide(c2), c1.merge(c2.invert()))
        assert same(c1.separate(c2), c1.compose(c2.invert()))


@needs_native
@needs_mc
def test_quotient_and_separation_are_adjoints():
    rnd = random.Random(3)
    for _ in range(RUNS):
        c1, c2 = contract(rnd, "Cone"), contract(rnd, "Ctwo")
        assert holds(c2.compose(c1.divide(c2)), c1)
        assert holds(c1, c2.merge(c1.separate(c2)))


@needs_native
@needs_mc
def test_assume_and_ensure():
    rnd = random.Random(4)
    names = list(INPUTS) + list(OUTPUTS)
    for _ in range(RUNS):
        c = contract(rnd, "Cone")
        b = _ltl(rnd, names, 2)
        assert holds(c, c.assume(b))
        assert holds(c.ensure(b), c)


@needs_native
@needs_mc
def test_composition_with_discharge_is_an_abstraction():
    rnd = random.Random(5)
    for _ in range(RUNS):
        # C1 drives C2 (on b, or on b with another input), unconnected, feedback
        c1 = contract(rnd, "Cone", {"a": "bool"}, {"b": "bool"})
        chain = contract(rnd, "Ctwo", {"b": "bool"}, {"d": "bool"})
        mixed = contract(rnd, "Cthree", {"b": "bool", "c": "bool"}, {"d": "bool"})
        free = contract(rnd, "Cfour", {"c": "bool"}, {"d": "bool"})
        for other in (chain, mixed, free):
            assert holds(c1.compose(other), c1.compose(other, discharge=True))
            assert holds(other.compose(c1), other.compose(c1, discharge=True))
        back = contract(rnd, "Cback", {"b": "bool"}, {"a": "bool"})
        front = contract(rnd, "Cfront", {"a": "bool"}, {"b": "bool"})
        assert holds(front.compose(back), front.compose(back, discharge=True))


@needs_mc
def test_the_checks_on_related_terms_give_the_same_answers(monkeypatch):
    # remove_redundant uses, for each term, only the terms that share
    # variables with it (logic._Implications); with every term it must drop
    # the same terms. Many variables and short terms, so that the terms fall
    # into several groups.
    from chase_cps import logic, remove_redundant
    rnd = random.Random(10)
    inputs = {v: "bool" for v in "acegik"}
    outputs = {v: "bool" for v in "bdfhjl"}

    def terms(names, n):
        return [_ltl(rnd, rnd.sample(names, 2), 1) for _ in range(n)]

    def removed(c):
        r = remove_redundant(c)
        return ([ltl.to_str(t) for t in r.removed_assumptions],
                [ltl.to_str(t) for t in r.removed_guarantees], r.unknown)

    whole = logic._Implications.__init__

    def without_slicing(self, *args, **kwargs):
        whole(self, *args, **kwargs)
        self.slice = False

    sliced = 0
    for i in range(RUNS):
        names = list(inputs) + list(outputs)
        c = Contract.from_strings(inputs=inputs, outputs=outputs, name=f"R{i}",
                                  assumptions=terms(list(inputs), rnd.randint(2, 4)),
                                  guarantees=terms(names, rnd.randint(3, 6)))
        first = removed(c)
        with monkeypatch.context() as m:
            m.setattr(logic._Implications, "__init__", without_slicing)
            assert removed(c) == first, c
        sliced += bool(first[0] or first[1])
    assert sliced > 0


# Cores -----------------------------------------------------------------------

@needs_mc
def test_unsat_core_is_minimal():
    rnd = random.Random(6)
    names = list(INPUTS) + list(OUTPUTS)
    v = {**INPUTS, **OUTPUTS}
    found = 0
    for _ in range(RUNS * 4):
        formulas = [_ltl(rnd, names, 2) for _ in range(rnd.randint(3, 6))]
        r = unsat_core(formulas, v)
        sat = is_satisfiable(ltl.conjunction([ltl.parse(f) for f in formulas]), v).status
        assert (r.status == "none") == (sat == "sat")
        if r.status != "found":
            continue
        found += 1
        assert is_satisfiable(ltl.conjunction(r.core), v).status == "unsat"
        for i in range(len(r.core)):
            rest = r.core[:i] + r.core[i + 1:]
            assert is_satisfiable(ltl.conjunction(rest), v).status == "sat"
    assert found > 0


@needs_native
@needs_slugs
def test_unrealizable_core_is_minimal():
    rnd = random.Random(7)
    found = 0
    for _ in range(RUNS * 3):
        c = gr1_contract(rnd, "Spec")
        r = unrealizable_core(c)
        status = is_realizable(c).status
        assert status in ("realizable", "unrealizable"), c
        assert (r.status == "none") == (status == "realizable"), (c, r)
        if r.status != "found":
            continue
        found += 1
        core = r.core.guarantees
        assert r.core.assumptions == c.assumptions

        def realizable(g):
            return is_realizable(Contract(c.name, c.inputs, c.outputs, c.assumptions, g)).status

        assert realizable(core) == "unrealizable"
        for i in range(len(core)):
            assert realizable(core[:i] + core[i + 1:]) == "realizable"
    assert found > 0


# Diagnostics -----------------------------------------------------------------

@needs_native
@needs_slugs
def test_remove_vacuous_keeps_realizability():
    rnd = random.Random(8)
    removed = 0
    for _ in range(RUNS * 2):
        c = gr1_contract(rnd, "Spec", n_assumptions=(2, 4), n_guarantees=(3, 6))
        v = remove_vacuous(c)
        assert not v.reason and not v.unknown
        removed += len(v.removed)
        before = is_realizable(c).status
        assert before in ("realizable", "unrealizable"), c
        assert is_realizable(v.contract).status == before, (c, v.removed)
        # removed terms come out in order, the kept ones are the rest
        kept = len(v.contract.assumptions) + len(v.contract.guarantees)
        assert kept + len(v.removed) == len(c.assumptions) + len(c.guarantees)
    assert removed > 0


@needs_native
@needs_slugs
def test_well_separation_core_is_minimal():
    rnd = random.Random(9)
    found = 0
    for _ in range(RUNS * 3):
        c = gr1_contract(rnd, "Spec", n_assumptions=(2, 4))
        r = is_well_separated(c, core=True)
        assert r.status in ("separated", "not_separated")
        if r.status != "not_separated":
            continue
        found += 1
        core = r.core.core.assumptions
        # the core is minimal with the guarantees kept (with_guarantees)
        sub = lambda a: Contract(c.name, c.inputs, c.outputs, a, list(c.guarantees))  # noqa: E731
        assert is_well_separated(sub(core)).status == "not_separated"
        for i in range(len(core)):
            assert is_well_separated(sub(core[:i] + core[i + 1:])).status == "separated"
    assert found > 0


def test_generators_produce_gr1_terms():
    rnd = random.Random(10)
    for _ in range(50):
        c = gr1_contract(rnd, "Spec")
        assert all(gr1_kind(t) is not None for t in c.assumptions + c.guarantees)
