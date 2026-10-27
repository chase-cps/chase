"""Tests of is_vacuous (vacuity in synthesis) and of well-separation on the
examples of the exact vacuity checks of ACIES (contrauto.feedback.exact_vacuity).
Skipped without slugs and the compiled CHASE modules."""

import pytest

from chase_cps import ltl
from chase_cps.contract import Contract

try:
    from chase_cps import _native  # noqa: F401
    from chase_cps.backends import SlugsAdapter
    HAS_SLUGS = SlugsAdapter().available()
except ImportError:
    HAS_SLUGS = False

needs_slugs = pytest.mark.skipif(not HAS_SLUGS, reason="slugs or the CHASE modules not installed")


def arbiter(*assumptions):
    return Contract.from_strings(
        inputs=["req"], outputs=["grant"], name="Arbiter",
        assumptions=["!req", *assumptions],
        guarantees=["!grant", "G (grant -> req)", "G F (!req | grant)"])


def test_the_outputs_asked_by_a_liveness_guarantee():
    from chase_cps.gr1 import _liveness_outputs, _polarity
    assert _polarity(ltl.parse("G F (!req | grant)")) == {"req": {False}, "grant": {True}}
    assert _polarity(ltl.parse("G (a <-> b)")) == {"a": {True, False}, "b": {True, False}}
    c = arbiter()
    assert {y: [ltl.to_str(t) for t in ts] for y, ts in _liveness_outputs(c).items()} == \
        {"grant": ["G F (!req | grant)"]}


@needs_slugs
def test_an_unconditional_release_makes_the_grant_vacuous():
    from chase_cps import is_vacuous
    # G F !req is assumed: G F (!req | grant) holds without any grant.
    r = is_vacuous(arbiter("G F !req"), core=True)
    assert r.status == "vacuous" and r
    assert r.checked == ["grant"]
    [v] = r.vacuous
    assert v.output == "grant" and v.mode == "never"
    assert [ltl.to_str(t) for t in v.guarantees] == ["G F (!req | grant)"]
    assert v.controller and v.controller.get("nodes")
    assert v.core.status == "found" and [ltl.to_str(t) for t in v.core.core] == ["G F !req"]
    d = r.to_dict()
    assert d["vacuous"][0]["mode"] == "never" and d["vacuous"][0]["core"]["status"] == "found"


@needs_slugs
def test_the_assumption_core_drops_the_assumptions_not_needed():
    from chase_cps import is_vacuous
    r = is_vacuous(arbiter("G F !req", "G (req -> X (req | !req))"), core=True)
    assert [ltl.to_str(t) for t in r.vacuous[0].core.core] == ["G F !req"]


@needs_slugs
def test_a_conditional_release_keeps_the_grant():
    from chase_cps import is_vacuous
    # the request is released only after a grant, and persists until then:
    # the contract is won only by granting, and not finitely often.
    c = arbiter("G ((req & grant) -> X !req)", "G ((req & !grant) -> X req)")
    r = is_vacuous(c)
    assert r.status == "not_vacuous" and not r and r.checked == ["grant"]


@needs_slugs
def test_an_output_needed_for_a_while_is_vacuous_eventually():
    from chase_cps import is_vacuous
    # requests must be granted until done latches; done eventually latches,
    # and then the requests are released on their own.
    c = Contract.from_strings(
        inputs=["req", "done"], outputs=["grant"], name="Arbiter",
        assumptions=["!req", "!done", "G (done -> X done)", "G F done", "G F (!req | !done)"],
        guarantees=["!grant", "G ((req & !done) -> X grant)", "G F (!req | grant)"])
    r = is_vacuous(c)
    assert r.status == "vacuous"
    assert [(v.output, v.mode) for v in r.vacuous] == [("grant", "eventually")]


@needs_slugs
def test_the_outputs_to_check_can_be_given():
    from chase_cps import is_vacuous
    c = arbiter("G F !req")
    assert is_vacuous(c, outputs="grant").status == "vacuous"
    assert is_vacuous(c, outputs=[]).status == "not_vacuous"
    with pytest.raises(ValueError):
        is_vacuous(c, outputs=["req"])


@needs_slugs
def test_vacuity_needs_a_realizable_gr1_contract():
    from chase_cps import is_vacuous
    bad = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Bad",
                                guarantees=["G (req -> grant)", "G (req -> !grant)"])
    assert is_vacuous(bad).status == "unrealizable"
    not_gr1 = Contract.from_strings(inputs=["req"], outputs=["grant"], name="N",
                                    guarantees=["G (req -> F grant)"])
    r = is_vacuous(not_gr1)
    assert r.status == "unknown" and "GR(1)" in r.reason
    with pytest.raises(TypeError):
        is_vacuous(arbiter(), context="G req")


@needs_slugs
def test_a_controller_that_can_force_a_deadlock_is_not_well_separated():
    from chase_cps import is_well_separated
    # setting act_a and act_b together forces a and b, which the
    # environment cannot do at once.
    c = Contract.from_strings(
        inputs=["a", "b"], outputs=["act_a", "act_b"], name="Ctrl",
        assumptions=["G (act_a -> X a)", "G (act_b -> X b)", "G !(a & b)"],
        guarantees=["!act_a", "!act_b", "G F act_a"])
    r = is_well_separated(c, core=True)
    assert r.status == "not_separated"
    assert len(r.core.core.assumptions) == 3
