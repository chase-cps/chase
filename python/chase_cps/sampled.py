"""Sampled signals: from the steps of a formula to continuous time.

The formulas of CHASE count steps. When a step is a sample of a continuous
signal, taken every ``period`` seconds, and each variable changes at most
``lipschitz`` units per second, a formula that holds at the samples with
enough margin holds at every instant (Fainekos and Pappas, Robustness of
temporal logic specifications for continuous-time signals, Theoretical
Computer Science 410, 2009, Corollary 44). ``sampled`` gives the stricter
formula to check at the samples:

- each interval of time is made one step narrower at each end under ``F``
  and ``U`` (the event must happen well inside it), and one step wider
  under ``G`` and ``R`` (the property must hold a little before and after
  it); an operator under a negation is the other one (``!F`` is ``G!``);
- each comparison must hold with the margin ``E``: the most its terms can
  change between two samples. For ``a1 x1 + ... + an xn <= c`` it is
  ``(|a1| L1 + ... + |an| Ln) period``, with Li the Lipschitz constant of
  xi; ``x <= c`` becomes ``x + E < c``.

If the stricter formula holds at the samples, the formula holds in
continuous time. If it does not, nothing is concluded. The intervals must
span more than 3 steps (the period is less than a third of each interval),
and the formula must not use X, a primed variable, past(x, n), <->, or a
Boolean variable (a Boolean can change between two samples without a
sample seeing it). The stricter formula is a formula like any other: a
check of it over every behavior (``verify``, ``refines``) shows that the
property holds in continuous time for every behavior.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from fractions import Fraction
from typing import Mapping, Optional, Union

from . import ltl
from .queries import Trace


class SampledError(ValueError):
    """The formula cannot be read in continuous time; the message says why."""


@dataclass(frozen=True)
class _Timed:
    op: str            # G, F, U, R, with an interval
    steps: tuple       # (first, last)
    args: tuple


class _Keep(ltl._Parser):
    """The parser of CHASE, keeping the operators with an interval."""

    def bounded(self, op, steps, *args):
        return _Timed(op, tuple(steps), tuple(args))


_FLIP = {"<": ">=", "<=": ">", ">": "<=", ">=": "<", "=": "!=", "!=": "="}
_DUAL = {"&": "|", "|": "&", "G": "F", "F": "G", "U": "R", "R": "U"}


def sampled(formula: str, lipschitz: Union[Mapping, float], period, known: bool = False) -> str:
    """The formula to check at the samples so that the formula holds in
    continuous time (see the module), with its intervals as they are written:
    sampled("G[0,20] (T <= 75)", {"T": 0.5}, 1) is "G[0,21] (T + 0.5 < 75)".
    ``lipschitz`` is a number for every variable, or {variable: number};
    ``period`` the time between two samples. Raises SampledError when the
    formula cannot be read in continuous time.

    With ``known``, the other way: what the samples hold when the formula is
    known to hold in continuous time (the guarantees of a part, the
    assumptions of the requirement it refines). The intervals stay; each
    comparison is weaker by the margin (``x <= c`` becomes ``x - E <= c``),
    and a Boolean is kept where every instant holds it (not under F or U)."""
    period = ltl.number(period)
    if period <= 0:
        raise SampledError("the period must be positive")
    rates = lipschitz if isinstance(lipschitz, Mapping) else None
    for v, rate in (rates.items() if rates is not None else [("every variable", lipschitz)]):
        if ltl.number(rate) < 0:
            raise SampledError(f"the Lipschitz constant of {v} is negative ({rate}): it is a bound "
                               "on how fast the variable changes, 0 or more")
    node = _Keep(formula).parse()
    return _text(_margins(_nnf(_widen(node, True, known), False, known), rates, lipschitz, period, known))


def _text(node) -> str:
    """The formula, with the operators that have an interval written with it."""
    held = []

    def hold(n):          # each operator with an interval: a name, written after
        if isinstance(n, _Timed):
            held.append(n)
            return ltl.Var(f"__timed{len(held) - 1}__")
        if isinstance(n, ltl.Unary):
            return ltl.Unary(n.op, hold(n.arg))
        if isinstance(n, ltl.Binary):
            return ltl.Binary(n.op, hold(n.left), hold(n.right))
        return n

    def write(n, top=False):
        if not isinstance(n, _Timed):
            return ltl.to_str(hold(n))
        interval = f"[{n.steps[0]},{n.steps[1]}]"
        args = [write(a) for a in n.args]
        arg = lambda a: a if " " not in a or (a.startswith("(") and a.endswith(")")) else f"({a})"
        text = (f"{n.op}{interval} {arg(args[0])}" if len(args) == 1
                else f"{arg(args[0])} {n.op}{interval} {arg(args[1])}")
        return text if top else f"({text})"
    out = write(node, True)
    while True:           # the names, replaced by their operators
        names = [i for i in range(len(held)) if f"__timed{i}__" in out]
        if not names:
            return out
        for i in names:
            out = out.replace(f"__timed{i}__", write(held[i]))


def _widen(node, positive: bool, known: bool = False):
    """The intervals made narrower (F, U) or wider (G, R), as the polarity
    makes them act (known: as they are)."""
    if isinstance(node, _Timed) and known:
        return _Timed(node.op, node.steps, tuple(_widen(a, positive, known) for a in node.args))
    if isinstance(node, _Timed):
        first, last = node.steps
        if first == last:
            raise SampledError(f"{node.op}[{first},{last}] is one instant: an interval of "
                               "continuous time must be longer")
        if last - first <= 3:
            raise SampledError(f"{node.op}[{first},{last}] spans {last - first} steps: the "
                               "period must be less than a third of each interval (more than 3 steps)")
        acts = node.op if positive else _DUAL[node.op]
        steps = (first + 1, last - 1) if acts in ("F", "U") else (max(first - 1, 0), last + 1)
        return _Timed(node.op, steps, tuple(_widen(a, positive) for a in node.args))
    if isinstance(node, ltl.Unary):
        if node.op == "!":
            return ltl.Unary("!", _widen(node.arg, not positive, known))
        if node.op == "X":
            raise SampledError("X is the next sample, not an instant of continuous time: write "
                               "an interval (F[a,b], G[a,b])")
        if ltl.function(node) == "past":
            raise SampledError("past(x, n) counts samples, not time")
        arg = _widen(node.arg, positive, known)
        acts = node.op if positive else _DUAL.get(node.op, node.op)
        if acts == "F" and not known:                 # F p = F[0,inf) p: from the next sample on
            return ltl.Unary("X", ltl.Unary(node.op, arg))
        return ltl.Unary(node.op, arg)
    if isinstance(node, ltl.Binary):
        if node.op in ("<->", "xor"):
            raise SampledError(f"{node.op} has both polarities: write it with & and |")
        left = _widen(node.left, positive != (node.op == "->"), known)
        right = _widen(node.right, positive, known)
        if known:
            return ltl.Binary(node.op, left, right)
        if node.op == "U" and positive:     # p U q = p U[0,inf) q: q from the next sample on
            return ltl.Binary("&", left, ltl.Unary("X", ltl.Binary("U", left, right)))
        if node.op == "R" and not positive:     # !(p R q) = !p U !q: the same, negated
            return ltl.Binary("|", left, ltl.Unary("X", ltl.Binary("R", left, right)))
        return ltl.Binary(node.op, left, right)
    if isinstance(node, ltl.Var) and node.primed:
        raise SampledError(f"{node.name}' is the next sample, not an instant of continuous time")
    return node


def _nnf(node, negated: bool, known: bool = False, exists: bool = False):
    """The negations pushed down to the comparisons. A Boolean is refused
    (it can change between two samples), or, known, kept where every instant
    holds it and true under F or U (exists)."""
    if isinstance(node, _Timed):          # !G[a,b] p = F[a,b] !p, !(p U[a,b] q) = !p R[a,b] !q
        op = _DUAL[node.op] if negated else node.op
        ex = [exists or op == "R", exists or op == "U"] if len(node.args) == 2 else [exists or op == "F"]
        return _Timed(op, node.steps, tuple(_nnf(a, negated, known, e) for a, e in zip(node.args, ex)))
    if isinstance(node, ltl.Unary):
        if node.op == "!":
            return _nnf(node.arg, not negated, known, exists)
        if node.op in ("G", "F", "X"):
            op = _DUAL[node.op] if negated and node.op != "X" else node.op
            return ltl.Unary(op, _nnf(node.arg, negated, known, exists or op == "F"))
    if isinstance(node, ltl.Binary):
        if node.op == "->":
            return _nnf(ltl.Binary("|", ltl.Unary("!", node.left), node.right), negated, known, exists)
        if node.op in ("&", "|", "U", "R"):
            op = _DUAL[node.op] if negated else node.op
            return ltl.Binary(op, _nnf(node.left, negated, known, exists or op == "R"),
                              _nnf(node.right, negated, known, exists or op == "U"))
        if node.op in _FLIP:
            return ltl.Binary(_FLIP[node.op], node.left, node.right) if negated else node
    if isinstance(node, ltl.Const):
        return ltl.Const(node.value != negated)
    if isinstance(node, ltl.Var) and known:
        return ltl.Const(True) if exists else ltl.Unary("!", node) if negated else node
    if isinstance(node, ltl.Var):
        raise SampledError(f"the Boolean {node.name} can change between two samples")
    raise SampledError(f"{ltl.to_str(node)} is not read in continuous time")


def _linear(node) -> Optional[dict]:
    """{variable: coefficient, None: constant} of a linear term, or None."""
    if isinstance(node, ltl.Num):
        return {None: Fraction(node.value)}
    if isinstance(node, ltl.Var):
        return {node.name: Fraction(1)}
    if isinstance(node, ltl.Unary) and node.op == "neg":
        a = _linear(node.arg)
        return a and {k: -v for k, v in a.items()}
    if isinstance(node, ltl.Binary) and node.op in ("+", "-", "*", "/"):
        a, b = _linear(node.left), _linear(node.right)
        if a is None or b is None:
            return None
        if node.op in ("+", "-"):
            sign = 1 if node.op == "+" else -1
            out = dict(a)
            for k, v in b.items():
                out[k] = out.get(k, 0) + sign * v
            return out
        constant = lambda t: set(t) <= {None}
        if node.op == "*" and (constant(a) or constant(b)):
            c, t = (a, b) if constant(a) else (b, a)
            return {k: v * c.get(None, 0) for k, v in t.items()}
        if node.op == "/" and constant(b) and b.get(None):
            return {k: v / b[None] for k, v in a.items()}
    return None


def _margins(node, rates, rate, period, known=False, exists=False):
    """Each comparison with its margin: strict, or (known) weaker under F or
    U (exists), where the instant that holds it may fall between two samples;
    elsewhere a known comparison holds at the samples as it is."""
    def on(n, inner=exists):
        return _margins(n, rates, rate, period, known, inner)
    if isinstance(node, _Timed):
        if len(node.args) == 2:
            return _Timed(node.op, node.steps, (on(node.args[0], exists or node.op == "R"),
                                                on(node.args[1], exists or node.op == "U")))
        return _Timed(node.op, node.steps, (on(node.args[0], exists or node.op == "F"),))
    if isinstance(node, ltl.Unary):
        return ltl.Unary(node.op, on(node.arg, exists or node.op == "F"))
    if isinstance(node, ltl.Binary) and node.op not in _FLIP:
        # p U q: q at an instant, p before it (at every sample before it); p R q: the other way
        return ltl.Binary(node.op, on(node.left, exists or node.op == "R"),
                          on(node.right, exists or node.op == "U"))
    if known and not exists and isinstance(node, ltl.Binary):
        return node
    if not isinstance(node, ltl.Binary):
        return node
    terms = _linear(ltl.Binary("-", node.left, node.right))
    if terms is None:
        raise SampledError(f"{ltl.to_str(node)} is not linear: its margin is not known")
    change = Fraction(0)
    for v, a in terms.items():
        if v is None or a == 0:
            continue
        if rates is not None and v not in rates:
            raise SampledError(f"no Lipschitz constant for {v}")
        change += abs(a) * Fraction(ltl.number(rates[v] if rates is not None else rate))
    if change == 0:                     # the terms do not change: as written
        return node
    e = ltl.Num(ltl.number(change * period))
    l, r = node.left, node.right
    if known:             # a sample is at most E away from the instant that holds it
        low = ltl.Binary(node.op if node.op in ("<", "<=") else "<=", ltl.Binary("-", l, e), r)
        high = ltl.Binary(node.op if node.op in (">", ">=") else ">=", ltl.Binary("+", l, e), r)
        return {"<": low, "<=": low, ">": high, ">=": high,
                "=": ltl.Binary("&", low, high), "!=": ltl.Const(True)}[node.op]
    below = ltl.Binary("<", ltl.Binary("+", l, e), r)      # l < r - E
    above = ltl.Binary(">", ltl.Binary("-", l, e), r)      # l > r + E
    if node.op in ("<", "<="):
        return below
    if node.op in (">", ">="):
        return above
    if node.op == "=":
        return ltl.Binary("&", below, above)   # cannot hold: equality is not robust
    return ltl.Binary("|", below, above)


@dataclass
class Continuous:
    """Whether the formula holds in continuous time on the signal sampled
    by a trace: True, or None when nothing is concluded (``reason``).
    ``robustness`` is that of the stricter formula on the trace."""
    holds: Optional[bool]
    robustness: object = None
    reason: str = ""


def continuous(formula: str, trace: Trace, lipschitz, period,
               variables: Optional[dict] = None) -> Continuous:
    """Whether the formula holds at every instant of the continuous signal
    that the trace samples every ``period`` (see ``sampled``)."""
    from .robustness import robustness
    try:
        strict = sampled(formula, lipschitz, period)
    except SampledError as e:
        return Continuous(None, reason=str(e))
    r = robustness(strict, trace, variables)
    if r > 0:
        return Continuous(True, r)
    return Continuous(None, r, "the samples do not hold with the margin: nothing is concluded")


# Times in the intervals: [0s,2s], [0ms,500ms] ---------------------------------

_UNITS = {"s": Fraction(1), "ms": Fraction(1, 1000), "us": Fraction(1, 10 ** 6)}
_TIME = re.compile(r"(\d+(?:\.\d+)?)\s*(ms|us|s)\b")
_INTERVAL = re.compile(r"\[([^\[\]]*?\d\s*(?:ms|us|s)\b[^\[\]]*?)\]")


def has_times(text: str) -> bool:
    """Whether an interval of the text is written with times."""
    return bool(_INTERVAL.search(text))


def in_steps(text: str, period) -> str:
    """The text with the times of its intervals in steps of ``period``
    seconds: with a period of 0.1, G[0s,2s] is G[0,20] and F[0ms,500ms] is
    F[0,5]. A time that is not a whole number of steps is refused."""
    if not has_times(text):
        return text
    if period is None:
        raise SampledError("an interval is written with times (" + _INTERVAL.search(text).group(0)
                           + "): give the period, --period SECONDS")
    step = Fraction(str(period))
    if step <= 0:
        raise SampledError("the period must be positive")

    def bound(m):
        t = Fraction(m.group(1)) * _UNITS[m.group(2)]
        n = t / step
        if n.denominator != 1:
            raise SampledError(f"{m.group(0)} is not a whole number of steps of {period} s")
        return str(n.numerator)
    return _INTERVAL.sub(lambda m: "[" + _TIME.sub(bound, m.group(1)) + "]", text)


# A specification read in continuous time ----------------------------------------

def spec(text: str, lipschitz, period, assumptions: str, guarantees: str) -> tuple:
    """The text of a specification of the logics language with each term of
    its contracts read at the samples: "show" (``sampled``: holding at the
    samples, it holds in continuous time) or "known" (``sampled(known=True)``:
    what the samples hold when it holds in continuous time). A known term
    that speaks of the samples themselves (X, x', past) stays as it is: it is
    a fact on the samples, as the model of a plant in steps. For a refinement
    R <= S: R with assumptions "show" and guarantees "known", S with
    assumptions "known" and guarantees "show". Returns (text, {contract: why
    it cannot be read so}): such a contract keeps its terms, and is not to be
    used so."""
    out, part, start, head, body = text.splitlines(keepends=True), None, None, "", []
    name, refused = None, {}
    lines = list(out)
    role = {"A": assumptions, "G": guarantees}

    def end(last):
        statement = "".join(body).strip().rstrip(";").strip()
        if not statement or statement.lower() == "true":
            return
        try:
            new = sampled(statement, lipschitz, period, known=role[part] == "known")
        except SampledError as e:
            if role[part] != "known":         # a known term on the samples (x' = x + u) stays
                refused.setdefault(name, f"line {start + 1}: {statement}: {e}")
            return
        lines[start] = f"{head}{new};\n"
        for k in range(start + 1, last + 1):       # the lines stay where they were
            lines[k] = "\n"
    for number, raw in enumerate(out):
        line = re.sub(r"(//|#).*", "", raw)
        word = line.strip().lower()
        if word.startswith("contract "):
            part, name = None, line.strip()[len("contract "):].strip().rstrip(":").strip()
            continue
        keyword = ""
        if word.startswith(("assumptions:", "guarantees:")):
            part = "A" if word.startswith("assumptions") else "G"
            keyword, line = line.split(":", 1)
            keyword += ": "
            if not line.strip():
                continue
        if part is None or not line.strip():
            continue
        if not body:          # what comes before the term on its first line stays
            start = number
            head = keyword or line[:len(line) - len(line.lstrip())]
        body.append(line)
        if ";" in line:
            end(number)
            body = []
    return "".join(lines), refused


def not_shown(result):
    """The result of a check made stricter for continuous time (spec, sampled):
    a failure is not a counterexample of the signal, as the margins are a
    sufficient condition, so it is unknown, with the reason; its trace stays,
    the samples that do not hold the stricter formula."""
    from dataclasses import replace
    if result.status != "invalid":
        return result
    if hasattr(result, "guarantees"):         # a refinement: its two parts, the one that failed
        return replace(result, status="unknown", assumptions=not_shown(result.assumptions),
                       guarantees=not_shown(result.guarantees))
    return replace(result, status="unknown",
                   reason="not shown in continuous time: the samples of this trace do not hold "
                          "the stricter terms (the margins of the Lipschitz constants are a sufficient "
                          "condition); a smaller rate or period may show it, and the check at the "
                          "samples alone says whether it fails there")
