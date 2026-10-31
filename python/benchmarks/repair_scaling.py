"""How the checks and the repair grow with the size of a contract.

A family of timing contracts as ContrACT builds them (MEMOCODE 2026): a
chain of n reactions after a timer, each with a latency; every fourth
reaction has a side reaction, and the next one starts at the later of the
two (a disjunction: the binary variables of the MILP). The requirement: the
last reaction starts at most lam after the timer, lam 10% below the worst
case, so that the check fails. The repair changes k constants: lam and the
latencies of the first k - 1 reactions.

For each size: the composition, the refinement check (z3), the worst case
(one optimization), and the repair by each method, each run in a process of
its own, stopped after LIMIT seconds (the quantifier elimination of z3 does
not stop at its timeout).

    python repair_scaling.py                     # the table, in Markdown
    python repair_scaling.py --sizes 4 8 16 --k 1 2 --limit 60
"""

import argparse
import json
import multiprocessing as mp
import time

LATENCY = 10


def family(n: int, k: int):
    from chase_cps import Contract
    ev = lambda v: [f"t_{v}", f"d_{v}", f"T_{v}"]
    real = lambda vs: {x: "real" for x in vs}
    cs = [Contract.from_strings(name="Tick", outputs=real(ev("r0")),
                                guarantees=["t_r0 <= d_r0", "d_r0 <= T_r0", "T_r0 - d_r0 <= 0"])]
    prev, side = "r0", None
    for i in range(1, n + 1):
        v = f"r{i}"
        x = f"x{i}" if i < k else str(LATENCY)
        consts = {f"x{i}": LATENCY} if i < k else None
        ins = ev(prev) + ([f"T_{side}"] if side else [])
        start = [f"d_{v} = T_{prev}"] if not side else \
            [f"d_{v} >= T_{prev}", f"d_{v} >= T_{side}", f"d_{v} <= T_{prev} | d_{v} <= T_{side}"]
        cs.append(Contract.from_strings(name=f"R{i}", inputs=real(ins), outputs=real(ev(v)),
                                        assumptions=[f"t_{prev} <= d_{prev}"],
                                        guarantees=[f"t_{v} = t_{prev}", *start, f"t_{v} <= d_{v}",
                                                    f"d_{v} <= T_{v}", f"T_{v} - d_{v} <= {x}"],
                                        constants=consts))
        side = None
        if i % 4 == 0 and i < n:        # a side reaction, slower, that the next one waits for
            s = f"s{i}"
            cs.append(Contract.from_strings(name=f"S{i}", inputs=real(ev(prev)), outputs=real(ev(s)),
                                            assumptions=[f"t_{prev} <= d_{prev}"],
                                            guarantees=[f"t_{s} = t_{prev}", f"d_{s} = T_{prev}",
                                                        f"d_{s} <= T_{s}", f"T_{s} - d_{s} <= {2 * LATENCY}"]))
            side = s
        prev = v
    return cs, prev


def composed(n, k):
    from chase_cps import Contract
    cs, last = family(n, k)
    c = cs[0]
    for x in cs[1:]:
        c = c.compose(x)
    return c, last


def requirement(last, lam):
    from chase_cps import Contract
    return Contract.from_strings(name="Req", inputs={"t_r0": "real", "d_r0": "real"},
                                 outputs={f"t_{last}": "real", f"d_{last}": "real"},
                                 assumptions=["t_r0 = d_r0"], guarantees=[f"d_{last} - t_{last} <= lam"],
                                 constants={"lam": lam})


def measure(task, n, k, out):
    from chase_cps import maximize, refines, repair
    t = time.time()
    c, last = composed(n, k)
    r = {"compose": time.time() - t}
    t = time.time()
    worst = maximize(f"d_{last} - t_{last}", [c.assumption, c.guarantee, "t_r0 = d_r0"], c.variables).value
    r["worst"] = time.time() - t
    r["worst_value"] = float(worst)
    req = requirement(last, round(0.9 * float(worst)))
    t = time.time()
    r["refines"] = time.time() - t if refines(c, req).status else None
    if task != "checks":
        params = {"lam": (0, 10 * float(worst))}
        params.update({f"x{i}": (0, 10 * LATENCY) for i in range(1, k)})
        t = time.time()
        rep = repair("refines", c, req, parameters=params, method=task, timeout=out.get("limit", 60))
        r["repair"] = time.time() - t
        r.update(status=rep.status, rounds=rep.rounds, method=rep.method,
                 values={a: float(b) for a, b in rep.values.items()})
    out["result"] = r


def run(task, n, k, limit):
    with mp.Manager() as m:
        out = m.dict(limit=limit)
        p = mp.Process(target=measure, args=(task, n, k, out))
        p.start()
        p.join(limit)
        if p.is_alive():
            p.kill()
            return {"timeout": limit}
        return dict(out.get("result") or {"error": "no result"})


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sizes", type=int, nargs="+", default=[4, 8, 16, 32, 64])
    ap.add_argument("--k", type=int, nargs="+", default=[1, 2, 4])
    ap.add_argument("--limit", type=float, default=120)
    ap.add_argument("--projection-up-to", type=int, default=16,
                    help="run the projection (quantifier elimination) up to this n only")
    ap.add_argument("--json")
    a = ap.parse_args()
    rows = []
    print("| n | k | variables | compose (s) | refines (s) | worst case (s) | auto (s) | counterexamples (s) | projection (s) | repair |")
    print("|---|---|---|---|---|---|---|---|---|---|")
    for n in a.sizes:
        for k in a.k:
            res = {m: run(m, n, k, a.limit) for m in ("auto", "counterexamples", "projection")
                   if m != "projection" or n <= a.projection_up_to}
            res.setdefault("projection", {"error": "not run"})
            base = next((v for v in res.values() if "compose" in v), {})
            cell = lambda v, key="repair": f"> {v['timeout']:.0f}" if "timeout" in v else \
                (f"{v[key]:.2f}" if v.get(key) is not None else v.get("error", "-"))
            auto = res["auto"]
            what = f"{auto.get('status', '-')} {auto.get('values', '')} in {auto.get('rounds', '-')} rounds" \
                if "status" in auto else "-"
            variables = 3 * (n + 1 + max(0, (n - 1) // 4))
            print(f"| {n} | {k} | {variables} | {cell(base, 'compose')} | {cell(base, 'refines')} | "
                  f"{cell(base, 'worst')} | {cell(auto)} | {cell(res['counterexamples'])} | "
                  f"{cell(res['projection'])} | {what} |", flush=True)
            rows.append({"n": n, "k": k, **{m: v for m, v in res.items()}})
    if a.json:
        with open(a.json, "w") as f:
            json.dump(rows, f, indent=1)


if __name__ == "__main__":
    main()
