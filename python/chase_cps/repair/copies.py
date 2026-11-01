"""Repair by copies of the state, for questions on contracts whose terms are
invariants (``G s``), their duals (``F s``), and conditions on the first
step, where s reads one step: no X, U, R, primed variables, or past.

Such a question has no bounded form, so z3 does not read it, and the
counterexamples of the model checker may not end with real parameters (each
removes a small set of values); but it has an equivalent form without
temporal operators, which z3 reads.

Why. A question is a formula over the behaviors: which behaviors, and the
contracts have no model, so any sequence of states is one (the variables
are not constrained from one step to the next), and the parameters are the
same at every step. Let the formula have the invariants G s1, ..., G sn
(F s is !G !s), and conditions b on the first step. On a behavior, G si
holds when si holds at every state the behavior visits: its truth depends
only on the set of states visited, and b on the first state.

Take n + 1 copies of the variables, x0, ..., xn (the parameters shared),
and replace b by b(x0) and G si by si(x0) & ... & si(xn). The formula
is satisfiable over the behaviors when this one is over the copies:

- from the copies to a behavior: the behavior x0, x1, ..., xn, then xn
  forever, visits exactly the copies, so each G si has the value of its
  replacement, and b too;
- from a behavior to the copies: x0 is its first state; xi is a state where
  si does not hold, if there is one, else the first state. Every copy is a
  state of the behavior, and each G si that fails has its witness among
  them, so each replacement has the value of its G si on the behavior.

So the questions on the copies have the same answers as the questions on
the behaviors, for every value of the parameters. They have no temporal
operator, so z3 answers them exactly, and its counterexamples, each made
a set of values by a model-based projection, lead to the least cost in a
few rounds with no model checker (chase_cps.repair._on_copies); for a
check of satisfiability the witnesses are found with the values at once.
The repair is still run through the check itself, as every repair is (the
encoding is checked, not trusted). That check sees the values of a repair
only: impossible and optimal rest on the equivalence above.

This is the small-model property of the formulas of temporal height one:
position 0 and one witness for each temporal subformula keep the truth of
every subformula (Demri and Schnoebelen, The complexity of propositional
linear temporal logics in simple cases, Information and Computation 2002,
Definition 7.2, Lemma 7.1, Proposition 7.2); for F and G, Sistla and
Clarke, The complexity of propositional linear temporal logics, JACM 1985,
Theorems 3.4 and 3.5. The argument reads each atom on one state, so atoms
of arithmetic keep it. It needs the terms flat (G F p depends on the order
of the states) and the steps free (a model, or x', links them).
"""

from __future__ import annotations

from .. import ltl
from ..queries import Implies, Satisfiable


def _one_step(node) -> bool:
    """A condition on one step: no temporal operator, no primed variable,
    no past."""
    if isinstance(node, ltl.Var):
        return not node.primed
    if isinstance(node, (ltl.Const, ltl.Num)):
        return True
    if isinstance(node, ltl.Unary):
        if node.op in ltl.TEMPORAL_UNARY or node.op.startswith("past"):
            return False
        return _one_step(node.arg)
    if isinstance(node, ltl.Binary):
        if node.op in ltl.TEMPORAL_BINARY:
            return False
        return _one_step(node.left) and _one_step(node.right)
    return False


def _atoms(node, out: list) -> bool:
    """Collect the invariants (G s, F s) of a formula into out; False when the
    formula is not a Boolean combination of invariants and conditions on the
    first step."""
    if _one_step(node):
        return True
    if isinstance(node, ltl.Unary) and node.op in ("G", "F"):
        if not _one_step(node.arg):
            return False
        if node not in out:
            out.append(node)
        return True
    if isinstance(node, ltl.Unary) and node.op == "!":
        return _atoms(node.arg, out)
    if isinstance(node, ltl.Binary) and node.op in ltl.BOOLEAN_BINARY + ("xor",):
        return _atoms(node.left, out) and _atoms(node.right, out)
    return False


def invariants(question) -> list | None:
    """The invariants of a question, or None when it is not of this form."""
    out: list = []
    parts = [question.phi, question.psi] if isinstance(question, Implies) else [question.phi]
    for f in parts:
        if not _atoms(f, out):
            return None
    return out


def reducible(asked: list) -> str:
    """Why the questions cannot be written on copies, or ""."""
    for name, q in asked:
        if not isinstance(q, (Implies, Satisfiable)):
            return f"{name} is a question on a state machine"
        if invariants(q) is None:
            return (f"{name} has a term that is not an invariant (G of one step), its dual "
                    "(F of one step), or a condition on the first step")
    return ""


def _copy(node, k: int, params, tag: str):
    """The formula with its variables (not the parameters) renamed to copy k."""
    if isinstance(node, ltl.Var):
        return node if node.name in params else ltl.Var(f"{node.name}{tag}{k}")
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, _copy(node.arg, k, params, tag))
    if isinstance(node, ltl.Binary):
        return ltl.Binary(node.op, _copy(node.left, k, params, tag),
                          _copy(node.right, k, params, tag))
    return node


def _replaced(node, n: int, params, tag: str):
    """The formula on the copies 0..n."""
    if _one_step(node):
        return _copy(node, 0, params, tag)
    if isinstance(node, ltl.Unary) and node.op == "G":
        return ltl.conjunction([_copy(node.arg, k, params, tag) for k in range(n + 1)])
    if isinstance(node, ltl.Unary) and node.op == "F":
        return ltl.disjunction([_copy(node.arg, k, params, tag) for k in range(n + 1)])
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, _replaced(node.arg, n, params, tag))
    return ltl.Binary(node.op, _replaced(node.left, n, params, tag),
                      _replaced(node.right, n, params, tag))


def _tag(names, n: int) -> str:
    """The mark of the copies (x__c0, ...), longer when a name has it already
    (a constant named x__c1 with a variable x)."""
    tag = "__c"
    while any(f"{v}{tag}{k}" in names for v in names for k in range(n + 1)):
        tag = "_" + tag
    return tag


def on_copies(asked: list, params) -> list:
    """The questions written on copies of the state: (name, query) with the
    same answer for every value of the parameters, and no temporal
    operator."""
    out = []
    for name, q in asked:
        n = max(len(invariants(q)), 1)
        tag = _tag(set(q.variables) | set(params), n)
        variables = {}
        for v, t in q.variables.items():
            if v in params:
                variables[v] = t
            else:
                variables.update({f"{v}{tag}{k}": t for k in range(n + 1)})
        if isinstance(q, Implies):
            out.append((name, Implies(_replaced(q.phi, n, params, tag),
                                      _replaced(q.psi, n, params, tag), variables)))
        else:
            out.append((name, Satisfiable(_replaced(q.phi, n, params, tag), variables)))
    return out


def unread(copies: list, params) -> str:
    """Why z3 does not read the questions on the copies (a term such as
    mod), or ""."""
    from ..backends import z3 as backend_z3
    for name, q in copies:
        e = backend_z3.Encoding(q.variables, rigid=params)
        try:
            for f in [q.phi, q.psi] if isinstance(q, Implies) else [q.phi]:
                e.formula(f)
        except ValueError as err:
            return f"{name}: {err}"
    return ""
