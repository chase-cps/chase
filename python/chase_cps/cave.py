"""Context-aided variable elimination (CAVE) for GR(1) contracts.

Composition and quotient must drop the variables that are not on the
interface of the result (Incer et al., Pacti, ACM TCPS 9(1), 2025). Given a
target phi, a context Gamma, and the variables p to drop, CAVE returns a
formula without p:

- by refinement, Psi with Gamma & Psi -> phi (exactly: forall p. Gamma -> phi);
- by relaxation, Psi with Gamma & phi -> Psi (exactly: exists p. Gamma & phi).

For GR(1) formulas (initial terms, G f and G F f, with f using X only) the
exact answers need not be GR(1). This module follows Narter, Zhao, and Incer,
"Computing GR(1) Assume-Guarantee Contracts" (arXiv 2610.37534, Section 4,
Definition 4.1, Algorithms 1 and 2): the initial term, the invariant, and
each liveness term are solved apart, each over the finite window of steps
that it reads, with the invariant of the context at each of these steps
(the shifted invariant). A window is a formula over the values of the
variables at steps 0..h, which z3 eliminates (``qe2``). The result is GR(1)
and sound: by refinement it implies the exact answer, by relaxation it is
implied by it. It need not be exact: a term that needs the steps before the
window, or two liveness terms at once, is not used.

The variables are Booleans, reals, and integer ranges; words and
``past(x, n)`` are not supported.
"""

from __future__ import annotations

from fractions import Fraction

from . import ltl
from .backends.z3 import Encoding, _z3 as z3, bounded
from .gr1 import gr1_kind

QE_TACTICS = ("qe2", "qe")
SIMPLIFY_MS = 2000


class EliminationError(ValueError):
    """Raised when the variables cannot be eliminated: a term not GR(1), a
    type that is not supported, or a result that z3 does not give without
    quantifiers."""


# GR(1) terms -----------------------------------------------------------------

def split(terms: list[ltl.Node]) -> tuple[list, list, list]:
    """The cores of the initial terms, of the invariants (G f: f), and of the
    liveness terms (G F f: f). As in the paper, an initial term may read the
    next steps (X, no G, F, U, R); slugs, unlike this module, takes initial
    terms without X."""
    init, inv, liv = [], [], []
    for t in terms:
        kind = gr1_kind(t)
        if kind == "init" or (kind is None and bounded(t)):
            init.append(t)
        elif kind == "safety":
            inv.append(t.arg)
        elif kind == "justice":
            liv.append(t.arg.arg)
        else:
            raise EliminationError(f"not a GR(1) term: {ltl.to_str(t)}")
    for core in init + inv + liv:
        _check_core(core)
    return init, inv, liv


def _check_core(node: ltl.Node) -> None:
    if isinstance(node, ltl.Unary):
        if ltl.function(node) or ltl.bits(node):
            raise EliminationError(f"not supported in a variable elimination: {ltl.to_str(node)}")
        _check_core(node.arg)
    elif isinstance(node, ltl.Binary):
        if node.op in ltl.WORD_OPS:
            raise EliminationError(f"not supported in a variable elimination: {ltl.to_str(node)}")
        _check_core(node.left)
        _check_core(node.right)


def depth(node: ltl.Node, d: int = 0) -> int:
    """The number of steps after the first that a core reads (its next depth):
    X and a primed variable each move one step."""
    if isinstance(node, ltl.Var):
        return d + 1 if node.primed else d
    if isinstance(node, ltl.Unary):
        return depth(node.arg, d + 1 if node.op == "X" else d)
    if isinstance(node, ltl.Binary):
        return max(depth(node.left, d), depth(node.right, d))
    return d


# One window ------------------------------------------------------------------

class _Window:
    """The formulas of one finite subproblem over the values at steps 0..h."""

    def __init__(self, types: dict):
        for v, t in types.items():
            if t not in ("bool", "real") and not isinstance(t, tuple):
                raise EliminationError(f"variable {v} has type {t}: only Booleans, reals, "
                                       "and integer ranges are supported")
        self.types = types
        self.enc = Encoding(types)

    def core(self, node: ltl.Node, step: int = 0):
        return self.enc.formula(node, step)

    def shifted(self, inv: list[ltl.Node], h: int):
        """The invariant of the context at steps 0..h (Gamma_inv^[0:h])."""
        if not inv:
            return z3.BoolVal(True)
        g = ltl.conjunction(inv)
        return z3.And(*[self.core(g, k) for k in range(h + 1)])

    def copies(self, drop: set[str]) -> list:
        return [v for (name, _), v in self.enc.vars.items() if name in drop]

    def ranges(self, xs: list):
        """The ranges of the integer variables among xs."""
        out = []
        for (name, _), v in self.enc.vars.items():
            t = self.types.get(name)
            if isinstance(t, tuple) and any(v.eq(x) for x in xs):
                out += [v >= t[0], v <= t[1]]
        return z3.And(*out) if out else z3.BoolVal(True)

    def forall(self, drop: set[str], context, phi):
        """Psi without the copies of ``drop`` with context & Psi -> phi."""
        xs = self.copies(drop)
        hyp = z3.And(self.ranges(xs), context)
        return _eliminate(z3.ForAll, xs, z3.Implies(hyp, phi),
                          lambda psi: z3.Implies(z3.And(hyp, psi), phi))

    def exists(self, drop: set[str], context, phi):
        """Psi without the copies of ``drop`` with context & phi -> Psi."""
        xs = self.copies(drop)
        hyp = z3.And(self.ranges(xs), context, phi)
        return _eliminate(z3.Exists, xs, hyp, lambda psi: z3.Implies(hyp, psi))


def _eliminate(quantifier, xs, body, sound):
    """The quantifier-free form of quantifier(xs, body). Each result of z3 is
    checked against ``sound`` (the defining implication, which must be
    valid) before it is used: ``simplify`` before ``qe2`` has given wrong
    results (z3 5.1 and 5.3), so it is not run before the elimination."""
    if not xs:
        return z3.simplify(body)
    for qe in QE_TACTICS:
        goal = z3.Goal()
        goal.add(quantifier(xs, body))
        try:
            result = z3.Then(qe, "simplify", "ctx-simplify")(goal)
        except z3.Z3Exception:
            continue
        parts = [z3.And(*g) if len(g) else z3.BoolVal(True) for g in result]
        f = parts[0] if len(parts) == 1 else z3.Or(*parts)
        if _has_quantifier(f) or not _valid(sound(f)):
            continue
        g = _simplified(f)
        return g if _valid(sound(g)) else f
    raise EliminationError(f"z3 could not eliminate {sorted(str(x) for x in xs)}")


def _simplified(f):
    """An equivalent formula with the parts implied by the others removed
    (z3 ``ctx-solver-simplify``, within SIMPLIFY_MS), or f."""
    goal = z3.Goal()
    goal.add(f)
    try:
        result = z3.TryFor(z3.Tactic("ctx-solver-simplify"), SIMPLIFY_MS)(goal)
    except z3.Z3Exception:
        return f
    parts = [z3.And(*g) if len(g) else z3.BoolVal(True) for g in result]
    return z3.simplify(parts[0] if len(parts) == 1 else z3.Or(*parts))


def _has_quantifier(f) -> bool:
    if z3.is_quantifier(f):
        return True
    return any(_has_quantifier(c) for c in f.children())


def _valid(f) -> bool:
    s = z3.Solver()
    s.add(z3.Not(f))
    return s.check() == z3.unsat


def _unsat(f) -> bool:
    s = z3.Solver()
    s.add(f)
    return s.check() == z3.unsat


# From z3 back to LTL -----------------------------------------------------------

def _step_of(v) -> tuple[str, int]:
    name = v.decl().name()
    if "@" in name:
        base, k = name.rsplit("@", 1)
        return base, int(k)
    return name, 0


def _steps(f) -> set[int]:
    if z3.is_const(f) and f.decl().kind() == z3.Z3_OP_UNINTERPRETED:
        return {_step_of(f)[1]}
    out: set[int] = set()
    for c in f.children():
        out |= _steps(c)
    return out


def _x(node: ltl.Node, k: int) -> ltl.Node:
    for _ in range(k):
        node = ltl.Unary("X", node)
    return node


def _term(f, base: int) -> ltl.Node:
    """An arithmetic term read at step ``base``: a variable one step after
    is primed, n steps before is past(x, n)."""
    if z3.is_rational_value(f) or z3.is_int_value(f):
        value = Fraction(f.as_fraction()) if z3.is_rational_value(f) else Fraction(f.as_long())
        value = ltl.number(value)
        return ltl.Unary("neg", ltl.Num(-value)) if value < 0 else ltl.Num(value)
    if z3.is_const(f) and f.decl().kind() == z3.Z3_OP_UNINTERPRETED:
        name, k = _step_of(f)
        if k == base:
            return ltl.Var(name)
        if k == base + 1:
            return ltl.Var(name, primed=True)
        if k < base:
            return ltl.Unary(f"past {base - k}", ltl.Var(name))
        raise EliminationError(f"{f} cannot be read at step {base}")
    kind = f.decl().kind()
    args = [_term(c, base) for c in f.children()]
    if kind == z3.Z3_OP_TO_REAL:
        return args[0]
    if kind == z3.Z3_OP_UMINUS:
        return ltl.Unary("neg", args[0])
    ops = {z3.Z3_OP_ADD: "+", z3.Z3_OP_SUB: "-", z3.Z3_OP_MUL: "*", z3.Z3_OP_DIV: "/"}
    if kind in ops:
        node = args[0]
        for a in args[1:]:
            node = ltl.Binary(ops[kind], node, a)
        return node
    raise EliminationError(f"the term {f} has no LTL form")


def to_ltl(f) -> ltl.Node:
    """A z3 formula over the values at steps (x, x@1, ...) as a core:
    x@k is X^k x, and an atom comparing numbers of steps k and k+1 is
    X^k of the atom with the variables of step k+1 primed."""
    if z3.is_true(f):
        return ltl.Const(True)
    if z3.is_false(f):
        return ltl.Const(False)
    kind = f.decl().kind()
    if z3.is_const(f) and kind == z3.Z3_OP_UNINTERPRETED:
        name, k = _step_of(f)
        return _x(ltl.Var(name), k)
    args = f.children()
    if kind == z3.Z3_OP_NOT:
        return ltl.Unary("!", to_ltl(args[0]))
    if kind in (z3.Z3_OP_AND, z3.Z3_OP_OR):
        op = "&" if kind == z3.Z3_OP_AND else "|"
        unit, zero = (True, False) if op == "&" else (False, True)
        parts = [to_ltl(a) for a in args]
        if ltl.Const(zero) in parts:
            return ltl.Const(zero)
        parts = [a for a in parts if a != ltl.Const(unit)]
        if not parts:
            return ltl.Const(unit)
        node = parts[0]
        for a in parts[1:]:
            node = ltl.Binary(op, node, a)
        return node
    if kind == z3.Z3_OP_IMPLIES:
        return ltl.Binary("->", to_ltl(args[0]), to_ltl(args[1]))
    if kind == z3.Z3_OP_XOR:
        return ltl.Binary("xor", to_ltl(args[0]), to_ltl(args[1]))
    if kind == z3.Z3_OP_ITE and z3.is_bool(f):
        c, a, b = (to_ltl(x) for x in args)
        return ltl.Binary("|", ltl.Binary("&", c, a), ltl.Binary("&", ltl.Unary("!", c), b))
    if kind in (z3.Z3_OP_EQ, z3.Z3_OP_IFF) and z3.is_bool(args[0]):
        return ltl.Binary("<->", to_ltl(args[0]), to_ltl(args[1]))
    if kind == z3.Z3_OP_DISTINCT and len(args) == 2 and z3.is_bool(args[0]):
        return ltl.Unary("!", ltl.Binary("<->", to_ltl(args[0]), to_ltl(args[1])))
    rel = {z3.Z3_OP_EQ: "=", z3.Z3_OP_DISTINCT: "!=", z3.Z3_OP_LE: "<=", z3.Z3_OP_LT: "<",
           z3.Z3_OP_GE: ">=", z3.Z3_OP_GT: ">"}
    if kind in rel and len(args) == 2:
        steps = _steps(f)
        base = min(steps) if steps else 0
        if steps and max(steps) - base > 1:
            base = max(steps) - 1       # the earlier steps as past(x, n)
        return _x(ltl.Binary(rel[kind], _term(args[0], base), _term(args[1], base)), base)
    raise EliminationError(f"the formula {f} has no LTL form")


def _shifted_down(f):
    """A liveness core moved to start at its first step: G F X^k f is G F f."""
    steps = _steps(f)
    k = min(steps) if steps else 0
    if k == 0:
        return f
    pairs = []
    for v in _consts(f):
        name, s = _step_of(v)
        label = name if s == k else f"{name}@{s - k}"
        pairs.append((v, z3.Const(label, v.sort())))
    return z3.substitute(f, *pairs)


def _consts(f) -> list:
    seen, out = set(), []

    def walk(g):
        if z3.is_const(g) and g.decl().kind() == z3.Z3_OP_UNINTERPRETED:
            if g.get_id() not in seen:
                seen.add(g.get_id())
                out.append(g)
            return
        for c in g.children():
            walk(c)
    walk(f)
    return out


def _lift(init, inv, liv: list[tuple[str, object]]) -> list[ltl.Node]:
    """The GR(1) terms of the certificates, without the trivial ones."""
    out = []
    if init is not None and not z3.is_true(init):
        out.append(to_ltl(init))
    if inv is not None and not z3.is_true(inv):
        out.append(ltl.Unary("G", to_ltl(inv)))
    for op, f in liv:
        if z3.is_true(f):
            continue
        core = to_ltl(f)
        out.append(ltl.Unary("G", ltl.Unary("F", core)) if op == "GF" else ltl.Unary("G", core))
    return out


# Refinement (Algorithm 1) ------------------------------------------------------

def refine(target: list[ltl.Node], context: list[ltl.Node], drop, types: dict) -> list[ltl.Node]:
    """GR(1) terms Psi without the variables ``drop`` such that
    context & Psi -> target (Narter et al., arXiv 2610.37534, Algorithm 1).

    The initial term is solved at step 0 with the initial terms of the
    context; the invariant at each step with the shifted invariant of the
    context. Each liveness term G F phi has a recurrent candidate G F psi
    (with the shifted invariant only) and, for each liveness term G F gamma
    of the context, a candidate G psi (with gamma as well); the first valid
    one is kept, else the first one that is satisfiable, else the recurrent
    one.
    """
    drop = set(drop)
    g_init, g_inv, g_liv = split(context)
    t_init, t_inv, t_liv = split(target)

    w = _Window(types)
    h = max([depth(c) for c in t_init + g_init], default=0)
    ctx = z3.And(w.core(ltl.conjunction(g_init)), w.shifted(g_inv, h))
    init = w.forall(drop, ctx, w.core(ltl.conjunction(t_init))) if t_init else None

    inv = None
    if t_inv:
        w = _Window(types)
        phi = ltl.conjunction(t_inv)
        inv = w.forall(drop, w.shifted(g_inv, depth(phi)), w.core(phi))

    liv = []
    for phi in t_liv:
        w = _Window(types)
        candidates = [("GF", w.forall(drop, w.shifted(g_inv, depth(phi)), w.core(phi)))]
        for gamma in sorted(g_liv, key=lambda g: (depth(g), ltl.to_str(g))):
            w = _Window(types)
            h = max(depth(phi), depth(gamma))
            candidates.append(("G", w.forall(drop, z3.And(w.shifted(g_inv, h), w.core(gamma)),
                                             w.core(phi))))
        liv.append(_select(candidates))
    return _lift(init, inv, liv)


def _select(candidates):
    for op, f in candidates:
        if _valid(f):
            return op, z3.BoolVal(True)
    for op, f in candidates:
        if not _unsat(f):
            return op, _shifted_down(f) if op == "GF" else f
    op, f = candidates[0]
    return op, f


# Relaxation (Algorithm 2) ------------------------------------------------------

def relax(target: list[ltl.Node], context: list[ltl.Node], drop, types: dict) -> list[ltl.Node]:
    """GR(1) terms Psi without the variables ``drop`` such that
    context & target -> Psi (Narter et al., arXiv 2610.37534, Algorithm 2).

    The initial term and the invariant are projected over their windows;
    each liveness term of the context and of the target is projected apart,
    and the projections that are true, equivalent to another, or strictly
    implied by another are dropped.
    """
    drop = set(drop)
    g_init, g_inv, g_liv = split(context)
    t_init, t_inv, t_liv = split(target)

    w = _Window(types)
    h = max([depth(c) for c in t_init + g_init], default=0)
    ctx = z3.And(w.core(ltl.conjunction(g_init)), w.shifted(g_inv, h))
    # An empty initial term or invariant is true: its projection still keeps
    # what the context says of the remaining variables.
    init = w.exists(drop, ctx, w.core(ltl.conjunction(t_init)))

    w = _Window(types)
    phi = ltl.conjunction(t_inv)
    inv = w.exists(drop, w.shifted(g_inv, depth(phi)), w.core(phi))

    projections = []
    for core in g_liv + t_liv:
        w = _Window(types)
        p = w.exists(drop, w.shifted(g_inv, depth(core)), w.core(core))
        if not _valid(p):
            projections.append(_shifted_down(p))
    return _lift(init, inv, [("GF", p) for p in _strongest(projections)])


# Simplification in a context ------------------------------------------------------

def simplify(terms: list[ltl.Node], context: list[ltl.Node], types: dict) -> list[ltl.Node]:
    """The GR(1) terms, each equivalent to the one given wherever the
    context holds: the parts of a core that the context makes true or false
    at the steps where the core is read are replaced by true or false (the
    initial term with the initial terms of the context, every core with the
    shifted invariant of the context). The terms that become true are
    dropped. On every trace of the context, the result holds iff the terms
    do, so the guarantees of a saturated contract may be simplified with its
    assumptions."""
    g_init, g_inv, _ = split(context)
    out = []
    for t in terms:
        kind = gr1_kind(t)
        w = _Window(types)
        if kind == "init" or (kind is None and bounded(t)):
            core, wrap = t, None
            h = max([depth(c) for c in g_init + [core]])
            ctx = z3.And(w.core(ltl.conjunction(g_init)), w.shifted(g_inv, h))
        elif kind == "safety":
            core, wrap = t.arg, "G"
            ctx = w.shifted(g_inv, depth(core))
        elif kind == "justice":
            core, wrap = t.arg.arg, "GF"
            ctx = w.shifted(g_inv, depth(core))
        else:
            raise EliminationError(f"{ltl.to_str(t)} is not a GR(1) term")
        f = w.core(core)
        g = _under(ctx, f)
        s = _simplified(g)
        if _valid(z3.Implies(ctx, s == f)):
            g = s
        if z3.is_true(g):
            continue
        if g.eq(f):
            out.append(t)
            continue
        c = to_ltl(g)
        out.append(c if wrap is None else ltl.Unary("G", c) if wrap == "G"
                   else ltl.Unary("G", ltl.Unary("F", c)))
    return out


def _under(ctx, f):
    """f with its Boolean parts that ctx makes true or false replaced by true
    or false, so that ctx -> (f <-> result)."""
    if _valid(z3.Implies(ctx, f)):
        return z3.BoolVal(True)
    if _valid(z3.Implies(ctx, z3.Not(f))):
        return z3.BoolVal(False)
    if z3.is_and(f) or z3.is_or(f):
        unit = z3.is_and(f)
        parts = [_under(ctx, c) for c in f.children()]
        parts = [c for c in parts if not (z3.is_true(c) if unit else z3.is_false(c))]
        if not parts:
            return z3.BoolVal(unit)
        g = parts[0] if len(parts) == 1 else (z3.And if unit else z3.Or)(*parts)
        return f if g.eq(f) else g
    if z3.is_not(f):
        g = _under(ctx, f.children()[0])
        return f if g.eq(f.children()[0]) else z3.Not(g)
    return f


def _strongest(cores: list) -> list:
    """The cores without the equivalent copies and those strictly implied by
    another: G F a implies G F b when a implies b."""
    kept = []
    for c in cores:
        if not any(_valid(c == k) for k in kept):
            kept.append(c)
    out = []
    for b in kept:
        implied = any(_valid(z3.Implies(a, b)) and not _valid(z3.Implies(b, a))
                      for a in kept if a is not b)
        if not implied:
            out.append(b)
    return out
