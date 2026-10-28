"""Helpers of test_sva.py: a reference interpreter of SVA on finite traces
(straight from the definitions of IEEE 1800-2017 chapter 16), and a
three-valued evaluation of LTL on finite traces."""

import re

from chase_cps import ltl, words
from chase_cps import sva as S

_PLACE = re.compile(r"^(.*)@(-?\d+)$")


def k_not(a):
    return None if a is None else not a


def k_and(*xs):
    if any(x is False for x in xs):
        return False
    if any(x is None for x in xs):
        return None
    return True


def k_or(*xs):
    return k_not(k_and(*[k_not(x) for x in xs]))


class Ref:
    def __init__(self, meaning, trace, types):
        self.m = meaning
        self.trace = trace
        self.types = types
        self.n = len(trace)
        self.cache = {}

    # Boolean expressions -------------------------------------------------------
    def boolean(self, e, t, pol, top):
        """e at t; at and after `top` (the first cycle of the disable
        condition) every Boolean holds in positive polarity, none in negative."""
        if top is not None and t >= top:
            return pol
        key = (id(e), t)
        if key not in self.cache:
            self.cache[key] = self.eval_formula(self.m.formula(e, 0), t)
        return self.cache[key]

    def eval_formula(self, f, t):
        """A formula with placeholders x@d, at t."""
        vals, types = {}, {}
        ok = [True]

        def ren(node):
            if isinstance(node, ltl.Var):
                m = _PLACE.match(node.name)
                name, d = m.group(1), int(m.group(2))
                u = t + d
                if u < 0:
                    ok[0] = "past"
                    return node
                if u >= self.n:
                    if ok[0] is True:
                        ok[0] = False
                    return node
                key = f"{name}__{d + 100}"
                vals[key] = self.trace[u][name]
                if self.types[name] == "bool":
                    vals[key] = bool(vals[key])
                types[key] = self.m.types and _ctype(self.m, name)
                return ltl.Var(key)
            if isinstance(node, ltl.Unary):
                return ltl.Unary(node.op, ren(node.arg))
            if isinstance(node, ltl.Binary):
                return ltl.Binary(node.op, ren(node.left), ren(node.right))
            return node

        g = ren(f)
        if ok[0] == "past":
            return None           # before the first cycle: not compared
        if ok[0] is False:
            return None
        return bool(words.evaluate(g, vals, types))

    # sequences: three-valued match of s from t to e (e inclusive, e >= t) ------
    def match(self, s, t, e, pol, top):
        k = s.k
        if k == "paren":
            return self.match(s.a[0], t, e, pol, top)
        if k == "delay":
            left, right = s.a
            lo = self.m.const(s.v[0])
            hi = self.m.const(s.v[1])
            out = []
            if left is None:
                for d in range(lo, hi + 1):
                    if d == 0:
                        out.append(self.match(right, t, e, pol, top))
                    else:
                        out.append(self.match(right, t + d, e, pol, top) if t + d <= e else False)
                return k_or(*out)
            for mid in range(t, e + 1):
                a = self.match(left, t, mid, pol, top)
                if a is False:
                    continue
                for d in range(lo, hi + 1):
                    start = mid + d
                    if start > e:
                        continue
                    out.append(k_and(a, self.match(right, start, e, pol, top)))
            return k_or(*out)
        if k == "rep":
            op, lo, hi = s.v
            lo, hi = self.m.const(lo), self.m.const(hi)
            out = []
            for count in range(max(lo, 1), hi + 1):
                out.append(self.repeat(s.a[0], count, t, e, pol, top))
            return k_or(*out)
        if k == "bin" and s.v in ("or", "and", "intersect", "within", "throughout"):
            if s.v == "throughout":
                bs = [self.boolean(s.a[0], u, pol, top) for u in range(t, e + 1)]
                return k_and(self.match(s.a[1], t, e, pol, top), *bs)
            a, b = s.a
            if s.v == "or":
                return k_or(self.match(a, t, e, pol, top), self.match(b, t, e, pol, top))
            if s.v == "intersect":
                return k_and(self.match(a, t, e, pol, top), self.match(b, t, e, pol, top))
            if s.v == "and":
                out = []
                for e1 in range(t, e + 1):
                    out.append(k_and(self.match(a, t, e1, pol, top), self.match(b, t, e, pol, top)))
                    if e1 < e:
                        out.append(k_and(self.match(a, t, e, pol, top),
                                         self.match(b, t, e1, pol, top)))
                return k_or(*out)
            # a within b
            out = []
            for t1 in range(t, e + 1):
                for e1 in range(t1, e + 1):
                    out.append(k_and(self.match(b, t, e, pol, top), self.match(a, t1, e1, pol, top)))
            return k_or(*out)
        if k == "first_match":
            here = self.match(s.a[0], t, e, pol, top)
            earlier = [self.match(s.a[0], t, e2, pol, top) for e2 in range(t, e)]
            return k_and(here, *[k_not(x) for x in earlier])
        # a Boolean
        if e != t:
            return False
        return self.boolean(s, t, pol, top)

    def repeat(self, s, count, t, e, pol, top):
        if count == 1:
            return self.match(s, t, e, pol, top)
        out = []
        for mid in range(t, e):
            a = self.match(s, t, mid, pol, top)
            if a is False:
                continue
            out.append(k_and(a, self.repeat(s, count - 1, mid + 1, e, pol, top)))
        return k_or(*out)

    def ends(self, s, t):
        """The cycles where a match from t may end (bounded sequences)."""
        horizon = 12
        return range(t, t + horizon)

    # properties ----------------------------------------------------------------
    def prop(self, p, t, pol=True, top=None):
        k = p.k
        if t >= self.n:
            return None
        if k == "paren":
            return self.prop(p.a[0], t, pol, top)
        if k == "not":
            return k_not(self.prop(p.a[0], t, not pol, top))
        if k == "bin":
            op = p.v
            a, b = p.a
            if op in ("and", "or") and not self.m.is_property(p):
                return self.seq_prop(p, t, pol, top)
            if op == "and":
                return k_and(self.prop(a, t, pol, top), self.prop(b, t, pol, top))
            if op == "or":
                return k_or(self.prop(a, t, pol, top), self.prop(b, t, pol, top))
            if op == "implies":
                return k_or(k_not(self.prop(a, t, not pol, top)), self.prop(b, t, pol, top))
            if op == "iff":
                return k_and(k_or(k_not(self.prop(a, t, not pol, top)), self.prop(b, t, pol, top)),
                             k_or(k_not(self.prop(b, t, not pol, top)), self.prop(a, t, pol, top)))
            if op in ("|->", "|=>"):
                out = []
                for e in self.ends(a, t):
                    mt = self.match(a, t, e, not pol, top)   # under a negation
                    if mt is False:
                        continue
                    nxt = e + (1 if op == "|=>" else 0)
                    out.append(k_or(k_not(mt), self.prop(b, nxt, pol, top)))
                return k_and(*out)
            if op in ("#-#", "#=#"):
                out = []
                for e in self.ends(a, t):
                    mt = self.match(a, t, e, pol, top)
                    if mt is False:
                        continue
                    nxt = e + (1 if op == "#=#" else 0)
                    out.append(k_and(mt, self.prop(b, nxt, pol, top)))
                return k_or(*out)
            if op in ("until", "s_until", "until_with", "s_until_with"):
                # p until q = q | (p & X (p until q)); _with: q & p; beyond the trace: None
                val = None
                for u in range(self.n - 1, t - 1, -1):
                    q = self.prop(b, u, pol, top)
                    pu = self.prop(a, u, pol, top)
                    if op.endswith("_with"):
                        q = k_and(q, pu)
                    val = k_or(q, k_and(pu, val))
                return val
        if k == "if":
            c = p.a[0]
            then = k_or(k_not(self.boolean(c, t, not pol, top)), self.prop(p.a[1], t, pol, top))
            if len(p.a) == 2:
                return then
            # !c -> else: !c as a Boolean in negative polarity
            notc = self.boolean_not(c, t, not pol, top)
            return k_and(then, k_or(k_not(notc), self.prop(p.a[2], t, pol, top)))
        if k == "nexttime":
            r, _ = p.v
            n = 1 if r is None else self.m.const(r[0])
            return self.prop(p.a[0], t + n, pol, top)
        if k == "always":
            r, _ = p.v
            lo, hi = self.m.const(r[0]), self.m.const(r[1])
            return k_and(*[self.prop(p.a[0], t + i, pol, top) for i in range(lo, hi + 1)])
        if k == "eventually":
            r, _ = p.v
            lo, hi = self.m.const(r[0]), self.m.const(r[1])
            return k_or(*[self.prop(p.a[0], t + i, pol, top) for i in range(lo, hi + 1)])
        if k in ("strong", "weak"):
            return self.seq_prop(p.a[0], t, pol, top)
        return self.seq_prop(p, t, pol, top)

    def boolean_not(self, c, t, pol, top):
        if top is not None and t >= top:
            return pol
        return k_not(self.boolean(c, t, True, None))

    def seq_prop(self, s, t, pol, top):
        return k_or(*[self.match(s, t, e, pol, top) for e in self.ends(s, t)])

    def attempt(self, spec, t, disable):
        """The value of the property of an attempt at t."""
        top = None
        if disable is not None:
            for u in range(t, self.n):
                v = self.boolean(disable, u, True, None)
                if v is True:
                    top = u
                    break
        v = self.prop(spec, t, True, top)
        return v


def _ctype(meaning, v):
    return S._ctype(meaning, v)


# three-valued evaluation of LTL on a finite trace ---------------------------------

def atom_value(f, trace, t, types):
    """Value of a formula without temporal operators at t, or None past the end."""
    need = 1 if _primed(f) else 0
    if t + need >= len(trace):
        return None
    state = dict(trace[t])
    variables = dict(types)
    for v in ltl.variables(f):
        state[v + "__n"] = trace[t + need][v] if need else None
        variables[v + "__n"] = types[v]
    g = _unprime(f)
    st = {k: (bool(v) if types.get(k.replace("__n", "")) == "bool" and v is not None else v)
          for k, v in state.items()}
    return bool(words.evaluate(g, st, variables))


def _primed(f):
    if isinstance(f, ltl.Var):
        return f.primed
    if isinstance(f, ltl.Unary):
        return _primed(f.arg)
    if isinstance(f, ltl.Binary):
        return _primed(f.left) or _primed(f.right)
    return False


def _unprime(f):
    if isinstance(f, ltl.Var):
        return ltl.Var(f.name + "__n") if f.primed else f
    if isinstance(f, ltl.Unary):
        return ltl.Unary(f.op, _unprime(f.arg))
    if isinstance(f, ltl.Binary):
        return ltl.Binary(f.op, _unprime(f.left), _unprime(f.right))
    return f


def temporal(f):
    if isinstance(f, ltl.Unary):
        return f.op in ("G", "F", "X") or temporal(f.arg)
    if isinstance(f, ltl.Binary):
        return f.op in ("U", "R") or temporal(f.left) or temporal(f.right)
    return False


def values(f, trace, types):
    """List of three-valued values of f at 0..len(trace) (the last is beyond)."""
    n = len(trace)
    if not temporal(f):
        return [atom_value(f, trace, t, types) for t in range(n)] + [None]
    if isinstance(f, ltl.Unary):
        a = values(f.arg, trace, types)
        if f.op == "!":
            return [k_not(x) for x in a]
        out = [None] * (n + 1)
        for t in range(n - 1, -1, -1):
            if f.op == "X":
                out[t] = a[t + 1]
            elif f.op == "G":
                out[t] = k_and(a[t], out[t + 1])
            elif f.op == "F":
                out[t] = k_or(a[t], out[t + 1])
        return out
    a, b = values(f.left, trace, types), values(f.right, trace, types)
    op = f.op
    if op in ("&", "|", "->", "<->", "xor"):
        def comb(x, y):
            if op == "&":
                return k_and(x, y)
            if op == "|":
                return k_or(x, y)
            if op == "->":
                return k_or(k_not(x), y)
            if x is None or y is None:
                return None
            return (x == y) if op == "<->" else (x != y)
        return [comb(x, y) for x, y in zip(a, b)]
    out = [None] * (n + 1)
    for t in range(n - 1, -1, -1):
        if op == "U":
            out[t] = k_or(b[t], k_and(a[t], out[t + 1]))
        else:   # R
            out[t] = k_and(b[t], k_or(a[t], out[t + 1]))
    return out
