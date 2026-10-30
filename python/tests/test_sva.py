"""Tests of SVA (chase_cps.sva): reading SVA as contracts and writing
contracts as SVA.

- The meaning of the properties read is compared, attempt by attempt, with a
  reference interpreter of IEEE 1800 on random properties and traces
  (sva_reference.py). It shares only the meaning of an expression at one
  cycle with the translation; the widths of expressions were checked with
  Verilator.
- Contracts written as SVA and read back are equivalent (nuXmv), and the
  SVA written is legal SystemVerilog (pyslang, when installed).
- Verilator, when installed, agrees on the properties that it simulates
  right (Verilator 5.052 gets or, implies, if/else, within, #-#, sequence
  and, and ##[m:n] in an antecedent wrong).
"""

import os
import random
import re
import shutil
import subprocess

import pytest

import sva_reference as R
from chase_cps import Contract, implies, ltl
from chase_cps.backends import ADAPTERS
from chase_cps.sva import SVAError, parse_sva, to_sva

HAS_MC = ADAPTERS["nuxmv"].available()
needs_mc = pytest.mark.skipif(not HAS_MC, reason="nuXmv not installed")

TYPES = {"a": "bool", "b": "bool", "c": "bool", "rst": "bool", "x": "word[4]", "y": "word[4]"}


def read(prop, types=TYPES, **kw):
    """The LTL of one assertion, as text."""
    r = parse_sva(f"P: assert property (@(posedge clk) {prop});", inputs=types, **kw)
    assert not r.skipped, r.skipped
    return ltl.to_str(r.properties[0].formula)


def skipped(prop, types=TYPES):
    r = parse_sva(f"P: assert property (@(posedge clk) {prop});", inputs=types)
    assert len(r.skipped) == 1, r.properties
    return r.skipped[0].reason


# Meaning ------------------------------------------------------------------------------

def test_implications_and_delays():
    assert read("a |-> b") == "G (a -> b)"
    assert read("a |=> b") == "G (a -> X b)"
    assert read("a ##1 b |-> c") == "G ((a & X b) -> X c)"
    assert read("a |-> ##[1:2] b") == "G (a -> (X b | X X b))"
    assert read("a |-> s_eventually b") == "G (a -> F b)"
    assert read("a |-> (b until c)") == "G (a -> (b U c) | G b)"
    assert read("a |-> (b s_until_with c)") == "G (a -> (b U (b & c)))"
    assert read("a [*2] |=> b") == "G ((a & X a) -> X X b)"
    assert read("a ##[1:$] b |-> c") == "G (a -> G X (b -> c))"     # X G = G X
    assert read("a |-> strong(##[1:$] b)") == "G (a -> F X b)"
    assert read("not (a ##1 b)") == "G !(a & X b)"


def test_disable_iff_is_accept_on():
    # the attempt passes when rst holds before it fails
    assert read("disable iff (rst) a |=> b") == "G (a -> (rst | X (b | rst)))"
    assert read("disable iff (rst) a") == "G (a | rst)"


def test_past_and_the_first_cycle():
    r = parse_sva("P: assert property (@(posedge clk) x == $past(x));", inputs=TYPES)
    p = r.properties[0]
    assert ltl.to_str(p.formula) == "G (x' = x)" and p.from_cycle == 1
    assert read("a |=> x == $past(x)") == "G (a -> x' = x)"       # the past exists: cycle 0
    assert read("$rose(a) |-> b") == "G ((X a & !a) -> X b)"
    assert read("$stable(x)") == "G (x' = x)"
    assert read("$changed(x)") == "G (x' != x)"
    # values two cycles apart: past(x, n)
    r = parse_sva("P: assert property (@(posedge clk) x == $past(y, 2));", inputs=TYPES)
    p = r.properties[0]
    assert ltl.to_str(p.formula) == "G X X (x = past(y, 2))" and p.from_cycle == 2
    assert read("a |=> x == $past(y, 3)") == "G (X X a -> X X X (x = past(y, 3)))"


def test_widths_of_verilog():
    # the constant 1 has 32 bits: x + 1 does not wrap around in 4 bits
    assert read("x + 1 > y") == "G (extend(x, 1) + 1 > extend(y, 1))"
    assert read("(x + y) == 4'd3") == "G (x + y = 3)"                 # 4 bits: wraps
    assert read("x == 3") == "G (x = 3)"
    assert read("x < 300") == "G (extend(x, 5) < 300)"
    assert read("{a, b} == 2'b10") == "G (a & !b)"
    assert read("x[3:2] == 2'b01") == "G (x[3:2] = 1)"
    assert read("$onehot0({a, b})") == "G ((a & !b) | (!a & b) | (!a & !b))"
    assert read("x inside {1, [3:5]}") == "G (x = 1 | (x >= 3 & x <= 5))"
    assert read("(a ? x : y) == 4'd2") == "G ((a & x = 2) | (!a & y = 2))"
    assert read("x == (1 << y[1:0])") == \
        "G ((y[1:0] = 0 & x = 1) | (y[1:0] = 1 & x = 2) | (y[1:0] = 2 & x = 4) | (y[1:0] = 3 & x = 8))"
    signed = {"s": "signed word[4]", "t": "signed word[4]", "x": "word[4]"}
    assert read("s < t", signed) == "G (s < t)"
    # signed and unsigned: the comparison is unsigned
    assert read("s < x", signed) == "G (unsigned(s) < x)"


def test_count_overflows_as_in_verilog():
    # count == $past(count) + 1 fails when count wraps around: 32 bits
    text = """module m (input logic clk, output logic [7:0] count);
      G: assert property (@(posedge clk) count == $past(count) + 1);
    endmodule"""
    r = parse_sva(text)
    f = r.contract.guarantees[0]
    assert ltl.to_str(f) == "G (extend(count', 1) = extend(count, 1) + 1)"
    if HAS_MC:
        v = {"count": "word[8]"}
        assert implies(f, "G (count < 255)", v).status == "valid"


def test_a_module():
    text = """
    module m #(parameter W = 4) (input logic clk, input logic rst, input logic req,
                                 input logic [W-1:0] data, output logic gnt);
      default clocking @(posedge clk); endclocking
      default disable iff (rst);
      wire busy = gnt && !req;
      property p_grant(r); r |-> ##[1:2] gnt; endproperty
      A0: assume property (req |=> !req);
      G0: assert property (p_grant(req));
      G1: assert property (busy |-> data != 0);
      C0: cover property (req ##1 gnt);
      assert (req);  // immediate: not read
    endmodule"""
    r = parse_sva(text)
    c = r.contract
    assert c.name == "m" and r.clock == "posedge clk"
    assert c.inputs == {"data": "word[4]", "req": "bool", "rst": "bool"}
    assert c.outputs == {"gnt": "bool"}
    assert [ltl.to_str(f) for f in c.assumptions] == ["G (req -> (rst | X (req -> rst)))"]
    assert [ltl.to_str(f) for f in c.guarantees] == [
        "G (req -> (rst | X (gnt | rst) | X rst | X X (gnt | rst)))",
        "G ((gnt & !req) -> (data != 0 | rst))"]
    assert len(r.covers) == 1 and "immediate" in r.skipped[0].reason


def test_what_is_not_read():
    assert "never fails" in skipped("a |-> ##[1:$] b")
    assert "s_eventually" in skipped("a |-> ##[1:$] b")
    assert "repetition [->" in skipped("a[->1] |-> b")
    assert "[->" in skipped("a ##1 b[->1] |-> c")
    assert "x or z" in skipped("x == 4'bxx00")
    assert "width is not declared" in parse_sva(
        "P: assert property (@(posedge clk) q[3]);", outputs=[]).skipped[0].reason
    r = parse_sva("""
      P: assert property (@(posedge clk) a);
      Q: assert property (@(posedge clk) b);
      S: assert property (@(negedge clk2) c);""", outputs=[])
    assert r.clock == "posedge clk" and "the clock negedge clk2" in r.skipped[0].reason
    assert parse_sva("S: assert property (@(negedge clk2) c);", outputs=[],
                     clock="negedge clk2").properties
    r = parse_sva("P: assert property (@(posedge clk) _x || P);", outputs=[])
    assert "names CHASE does not take" in r.skipped[0].reason
    r = parse_sva("""`define ON 1
      `ifdef ON
      P: assert property (@(posedge clk) a);
      `endif
      Q: assert property (@(posedge clk) `ON);""", outputs=[])
    assert len(r.properties) == 1 and "macro" in r.skipped[0].reason
    with pytest.raises(SVAError, match="inputs or outputs"):
        parse_sva("P: assert property (@(posedge clk) a);")
    r = parse_sva("P: assert property (@(posedge clk) a);", outputs=[])
    assert r.undeclared == ["a"]


# Against the reference interpreter ----------------------------------------------------

def gen_expr(r, depth=0, past2=True):
    simple = [
        lambda: r.choice(["a", "b", "c", "!a", "!b"]),
        lambda: r.choice(["x == y", "x < y", "x >= 4'd9", "x == 3", "y != 0"]),
        lambda: r.choice(["(x + y) == 4'd3", "x + 1 > y", "(x - y) < 2", "x == y + 4'd1",
                          "(x & y) != 0", "x[0]", "x[3:2] == 2'b01", "(x ^ y) == 4'hF",
                          "{a, b} == 2'b10", "$onehot0(x)", "$onehot(y)", "^x",
                          "x inside {1, [3:5]}", "(a ? x : y) == 4'd2", "~x == y",
                          "(x << 1) == y", "(x >> 1) == y", "x * 2 == y", "{x, y} > 8'd100",
                          "$countones(x) == 2", "x == {2'b10, a, b}", "&x", "|y",
                          "(x % 3) == 1", "-x == y", "x == (1 << y[1:0])"]),
        lambda: r.choice(["x == $past(x)", "y == $past(x) + 1", "$rose(a)", "$fell(b)",
                          "$stable(x)", "$changed(y)", "a == $past(b)"]
                         + (["$past(a, 2)", "x != $past(y, 2)", "$past(x, 3) == y + 1",
                             "a |-> $past(b, 2)"[:0] or "$past(x, 2) < $past(y)"]
                            if past2 else [])),
    ]
    if depth > 1:
        return r.choice(simple)()
    return r.choice(simple + [
        lambda: f"({gen_expr(r, depth + 1, past2)} && {gen_expr(r, depth + 1, past2)})",
        lambda: f"({gen_expr(r, depth + 1, past2)} || {gen_expr(r, depth + 1, past2)})"])()


def gen_seq(r, depth=0):
    e = lambda: gen_expr(r, 1)                                          # noqa: E731
    sub = (lambda: f"({gen_seq(r, depth + 1)})") if depth < 1 else e     # noqa: E731
    return r.choice([
        e, lambda: f"{e()} ##1 {e()}", lambda: f"{e()} ##2 {sub()}",
        lambda: f"{e()} ##[1:2] {sub()}", lambda: f"{e()} ##[0:2] {e()}",
        lambda: f"{e()}[*2]", lambda: f"{e()}[*1:3] ##1 {e()}", lambda: f"##1 {e()}",
        lambda: f"##[0:1] {e()}", lambda: f"{sub()} or {sub()}", lambda: f"{sub()} and {sub()}",
        lambda: f"({e()} ##[1:2] {e()}) intersect ({e()} ##2 {e()})",
        lambda: f"{e()} throughout {sub()}",
        lambda: f"({e()} ##1 {e()}) within ({e()} ##3 {e()})",
        lambda: f"first_match({e()} ##[1:3] {e()})", lambda: f"{sub()}[*2]"])()


def gen_prop(r, depth=0):
    s = lambda: gen_seq(r)                                              # noqa: E731
    e = lambda: gen_expr(r)                                             # noqa: E731
    p = (lambda: gen_prop(r, depth + 1)) if depth < 1 else s             # noqa: E731
    out = r.choice([
        s, lambda: f"{s()} |-> {p()}", lambda: f"{s()} |=> {p()}",
        lambda: f"{e()} |-> nexttime {e()}", lambda: f"{e()} |-> nexttime [2] {e()}",
        lambda: f"{e()} |-> ({e()} until {e()})", lambda: f"{e()} |-> ({e()} until_with {e()})",
        lambda: f"{e()} |-> ({e()} s_until {e()})",
        lambda: f"({p()}) or ({p()})", lambda: f"({p()}) and ({p()})",
        lambda: f"({p()}) implies ({p()})", lambda: f"({p()}) iff ({p()})",
        lambda: f"not ({p()})", lambda: f"if ({e()}) ({p()}) else ({p()})",
        lambda: f"if ({e()}) ({p()})", lambda: f"{s()} #-# {p()}", lambda: f"{s()} #=# {p()}",
        lambda: f"{e()} |-> always [0:2] {e()}", lambda: f"{e()} |-> s_always [1:2] {e()}",
        lambda: f"{e()} |-> eventually [1:2] {e()}",
        lambda: f"{e()} |-> s_eventually [0:2] {e()}",
        lambda: f"strong({s()})", lambda: f"weak({s()})"])()
    if depth == 0 and r.random() < 0.4:
        return f"disable iff (rst) {out}"
    return out


def random_trace(r, length):
    return [{"a": r.randrange(2), "b": r.randrange(2), "c": r.randrange(2),
             "rst": int(r.random() < 0.15), "x": r.randrange(16), "y": r.randrange(16)}
            for _ in range(length)]


@pytest.mark.parametrize("seed", range(6))
def test_same_as_the_reference(seed):
    from chase_cps import sva as S
    r = random.Random(seed)
    props = [gen_prop(r) for _ in range(30)]
    trace = random_trace(r, 24)
    text = "\n".join(f"P{i}: assert property (@(posedge clk) {p});" for i, p in enumerate(props))
    reading = parse_sva(text, inputs=TYPES, name="T")
    assert not reading.skipped, reading.skipped
    scan = S._Scanner(text)
    scan.run()
    types = {k: S._decl_of(v) for k, v in TYPES.items()}
    decided = 0
    for (label, kind, initial, spec, _), prop, ptext in zip(scan.asserts, reading.properties,
                                                            props):
        clock, disable, body = spec
        ref = R.Ref(S._Meaning(scan, {}, dict(types), set()), trace, TYPES)
        f = prop.formula
        vals = [f.value] * 25 if isinstance(f, ltl.Const) else R.values(f.arg, trace, TYPES)
        for t in range(prop.from_cycle, len(trace)):
            ours, theirs = vals[t - prop.from_cycle], ref.attempt(body, t, disable)
            if ours is not None and theirs is not None:
                decided += 1
                assert ours == theirs, (label, t, ptext, ltl.to_str(f))
    assert decided > 500


# Writing ------------------------------------------------------------------------------

V = {"a": "bool", "b": "bool", "c": "bool", "x": "word[4]", "y": "word[4]",
     "s": "signed word[4]", "t": "signed word[4]"}


def gen_formula(r, depth=0):
    def term(signed):
        if signed:
            return r.choice(["s", "t", "s + t", "-s", "s >> 1", "s * 2", "signed(x)", "s / 2",
                             "s mod 3", "s - 1"])
        return r.choice(["x", "y", "x + y", "x - 1", "x * y", "(x & y)", "(x | 3)", "(x xor y)",
                         "x << 1", "x >> 2", "(!x)", "x[3:2] :: y[1:0]", "extend(x[1:0], 2)",
                         "(x :: y)[5:2]", "unsigned(s)", "x mod 3", "x / 2"])

    def atom():
        k = r.random()
        if k < 0.35:
            return r.choice(["a", "b", "c", "!a", "a xor b", "a <-> c"])
        if k < 0.7:
            return f"{term(False)} {r.choice(['=', '!=', '<', '>=', '<='])} " \
                   f"{r.choice([term(False), str(r.randrange(16))])}"
        if k < 0.85:
            return f"{term(True)} {r.choice(['<', '>', '=', '<='])} " \
                   f"{r.choice([term(True), str(r.randrange(-8, 8))])}"
        return r.choice(["x' = x + 1", "a' -> b", "y' != y", "s' < s", "x[0]", "x[3]",
                         "(x :: y)[4]", "x'[1:0] = y[3:2]", "X X (x = past(y, 2))",
                         "X (past(a, 1) -> b)", "X X X (past(s, 3) < t)",
                         "X X (x' = past(x, 2) + 1)"])

    if depth > 2:
        return atom()
    f = lambda: gen_formula(r, depth + 1)                               # noqa: E731
    return r.choice([atom, lambda: f"({f()}) & ({f()})", lambda: f"({f()}) | ({f()})",
                     lambda: f"({atom()}) -> ({f()})", lambda: f"X ({f()})",
                     lambda: f"G ({f()})", lambda: f"F ({f()})", lambda: f"({f()}) U ({f()})",
                     lambda: f"({f()}) R ({f()})", lambda: f"!({f()})",
                     lambda: f"({f()}) <-> ({f()})"])()


def random_contract(r):
    fs = [gen_formula(r) for _ in range(3)]
    fs = [f if r.random() < 0.5 else f"G ({f})" for f in fs]
    return Contract.from_dict({"name": "C",
                               "inputs": {"a": "bool", "x": "word[4]", "s": "signed word[4]"},
                               "outputs": {"b": "bool", "c": "bool", "y": "word[4]",
                                           "t": "signed word[4]"},
                               "assumptions": fs[:1], "guarantees": fs[1:]})


def test_writing():
    c = Contract.from_dict({"name": "Arb", "inputs": {"req": "bool", "n": [0, 7]},
                            "outputs": {"gnt": "bool", "cnt": "word[8]"},
                            "assumptions": ["G (req -> X !req)"],
                            "guarantees": ["G (req -> F gnt)", "G (cnt' = cnt + 1)", "!gnt"]})
    text = to_sva(c, reset="!rst_n", bind="arbiter")
    assert "module Arb_sva (" in text
    assert '(* chase = "output" *) input logic [7:0] cnt' in text
    assert "range_n: assume property (@(posedge clk) disable iff (!rst_n) 0 <= n && n <= 7);" \
        in text
    assert "A0: assume property (@(posedge clk) disable iff (!rst_n)\n    req |=> (!(req)));" \
        in text
    assert "G1: assert property (@(posedge clk) disable iff (!rst_n)\n" \
           "    nexttime ((cnt == ($past(cnt) + 8'd1))));" in text
    assert "initial G2: assert property" in text
    assert text.rstrip().endswith("bind arbiter Arb_sva Arb_sva_i (.*);")
    with pytest.raises(SVAError, match="keywords"):
        to_sva(Contract.from_dict({"name": "C", "inputs": {"wire": "bool"}}))
    with pytest.raises(SVAError, match="real"):
        to_sva(Contract.from_dict({"name": "C", "inputs": {"v": "real"}}))


@needs_mc
def test_written_and_read_back_is_equivalent():
    r = random.Random(3)
    for _ in range(25):
        c = random_contract(r)
        back = parse_sva(to_sva(c))
        assert not back.skipped, back.skipped
        c2 = back.contract
        for f1, f2 in zip(c.assumptions + c.guarantees, c2.assumptions + c2.guarantees):
            assert implies(f1, f2, V, timeout=60).status == "valid", (f1, f2)
            assert implies(f2, f1, V, timeout=60).status == "valid", (f1, f2)


def test_written_sva_is_legal_systemverilog():
    pyslang = pytest.importorskip("pyslang")
    r = random.Random(7)
    design = ("module dut(input logic clk, rst_n, a, b, c, input logic [3:0] x, y, "
              "input logic signed [3:0] s, t); endmodule\n")
    for _ in range(60):
        text = to_sva(random_contract(r), reset="!rst_n", bind="dut")
        assert "  input logic rst_n,\n" in text
        comp = pyslang.ast.Compilation()
        comp.addSyntaxTree(pyslang.syntax.SyntaxTree.fromText(text + design))
        errors = [d for d in comp.getAllDiagnostics() if d.isError()]
        assert not errors, pyslang.DiagnosticEngine.reportAll(comp.sourceManager, errors)


# Against Verilator --------------------------------------------------------------------

def verilator_failures(props, trace, tmp):
    """The (property, cycle) of the attempts that fail in Verilator: one
    assertion per attempt, (cyc == k) |-> p."""
    n = len(trace)
    lines = ["module tb;", "  logic clk = 0;", "  int cyc = 0;"]
    for v, t in TYPES.items():
        w = 4 if t == "word[4]" else 1
        vals = ", ".join(f"{w}'d{st[v]}" for st in trace)
        lines.append(f"  logic [{w - 1}:0] {v};")
        lines.append(f"  logic [{w - 1}:0] {v}_v [{n}] = '{{{vals}}};")
    lines.append("  initial begin")
    lines += [f"    {v} = {v}_v[0];" for v in TYPES]
    lines += [f"    repeat ({2 * n + 2}) #5 clk = ~clk;", "    $finish;", "  end",
              "  always @(negedge clk) begin", "    cyc <= cyc + 1;",
              f"    if (cyc + 1 < {n}) begin"]
    lines += [f"      {v} <= {v}_v[cyc + 1];" for v in TYPES]
    lines += ["    end", "  end"]
    for i, p in enumerate(props):
        dis = ""
        if p.startswith("disable iff (rst) "):
            dis, p = "disable iff (rst) ", p[len("disable iff (rst) "):]
        for k in range(n):
            lines.append(f"  P{i}_{k}: assert property (@(posedge clk) {dis}(cyc == {k}) |-> "
                         f"({p})) else $display(\"FAIL %0d %0d\", {i}, {k});")
    lines.append("endmodule")
    sv = os.path.join(tmp, "tb.sv")
    with open(sv, "w") as f:
        f.write("\n".join(lines) + "\n")
    subprocess.run(["verilator", "--binary", "--timing", "--assert", "-Wno-fatal", "-Wno-lint",
                    "-Wno-style", "--top-module", "tb", "-Mdir", os.path.join(tmp, "obj"), sv],
                   capture_output=True, text=True, check=True)
    run = subprocess.run([os.path.join(tmp, "obj", "Vtb")], capture_output=True, text=True)
    return {(int(m.group(1)), int(m.group(2)))
            for m in re.finditer(r"FAIL (\d+) (\d+)", run.stdout)}


@pytest.mark.skipif(not shutil.which("verilator"), reason="Verilator not installed")
def test_same_as_verilator(tmp_path):
    r = random.Random(11)
    e = lambda: gen_expr(r, past2=False)    # Verilator: $past(e, 2) one cycle off  # noqa: E731
    antecedents = [e, lambda: f"{e()} ##1 {e()}", lambda: f"{e()} ##2 {e()}",
                   lambda: f"{e()}[*2]", lambda: f"##1 {e()}"]
    consequents = antecedents + [lambda: f"{e()} ##[1:2] {e()}", lambda: f"{e()} ##[0:2] {e()}"]
    props = []
    for _ in range(20):
        p = f"{r.choice(antecedents)()} {r.choice(['|->', '|=>'])} {r.choice(consequents)()}"
        props.append(f"disable iff (rst) {p}" if r.random() < 0.4 else p)
    trace = random_trace(r, 30)
    theirs = verilator_failures(props, trace, str(tmp_path))
    text = "\n".join(f"P{i}: assert property (@(posedge clk) {p});" for i, p in enumerate(props))
    reading = parse_sva(text, inputs=TYPES)
    assert not reading.skipped
    for i, prop in enumerate(reading.properties):
        vals = R.values(prop.formula.arg, trace + [trace[-1]], TYPES)
        ours = {t + prop.from_cycle for t in range(len(trace) - prop.from_cycle)
                if vals[t] is False}
        assert ours == {k for j, k in theirs if j == i and k >= prop.from_cycle}, props[i]


def test_packed_arrays_and_hierarchical_names():
    text = """module m (input logic clk, input logic [3:0][7:0] data,
                        input logic [1:0][2:0][3:0] cube, output logic [7:0] o, output logic ok);
      P0: assert property (@(posedge clk) o == data[2]);
      P1: assert property (@(posedge clk) data[0][7] |-> ok);
      P2: assert property (@(posedge clk) data[3:2] == 16'hABCD);
      P3: assert property (@(posedge clk) cube[1][2] == 4'd5 && cube[0][0][3]);
      P4: assert property (@(posedge clk) u1.state == 2'd3 |=> u1.busy);
    endmodule"""
    r = parse_sva(text, inputs={"u1.state": "word[2]", "u1.busy": "bool"})
    assert not r.skipped
    assert [ltl.to_str(p.formula) for p in r.properties] == [
        "G (o = data[23:16])", "G (data[7:7] -> ok)", "G (data[31:16] = 43981)",
        "G (cube[23:20] = 5 & cube[3:3])", "G (u1__state != 3 | X u1__busy)"]
    assert r.contract.inputs["data"] == "word[32]" and r.contract.inputs["cube"] == "word[24]"
    assert r.names == {"u1__busy": "u1.busy", "u1__state": "u1.state"}
    sv = to_sva(r.contract, bind="m", names=r.names)
    assert sv.rstrip().endswith("bind m m_sva m_sva_i (.*, .u1__busy(u1.busy), "
                                ".u1__state(u1.state));")
    assert "index 4 of data" in parse_sva(
        "module m (input logic clk, input logic [3:0][7:0] data);\n"
        "P: assert property (@(posedge clk) data[4] == 0); endmodule").skipped[0].reason


@needs_mc
def test_past_two_cycles_back():
    text = """module sync (input logic clk, input logic [7:0] d, output logic [7:0] q);
      P: assert property (@(posedge clk) q == $past(d, 2));
    endmodule"""
    c = parse_sva(text).contract
    v = c.variables
    assert ltl.to_str(c.guarantees[0]) == "G X X (q = past(d, 2))"
    # two flip-flops: q is d two cycles before
    assert implies("G (m' = d & q' = m)", c.guarantees[0], {**v, "m": "word[8]"}).status \
        == "valid"
    assert implies("G (q' = d)", c.guarantees[0], v).status == "invalid"
    back = parse_sva(to_sva(c))
    assert back.contract.guarantees == c.guarantees


def test_generate_blocks_are_not_read():
    text = """module m #(parameter P = 1) (input logic clk, input logic a, b, output logic c);
      A: assert property (@(posedge clk) a |-> c);
      generate if (P) begin : g1
        B: assert property (@(posedge clk) b |-> c);
      end else begin
        C: assert property (@(posedge clk) !c);
      end endgenerate
      if (P) D: assert property (@(posedge clk) a);
      for (genvar i = 0; i < 2; i++) begin : f
        E: assert property (@(posedge clk) b);
      end
      always @(posedge clk) begin
        if (a) x <= 1; else x <= 0;
      end
      F: assert property (@(posedge clk) b |=> c);
    endmodule"""
    r = parse_sva(text)
    assert [p.label for p in r.properties] == ["A", "F"]
    assert [s.label for s in r.skipped] == ["B", "C", "D", "E"]
    assert all("generate" in s.reason for s in r.skipped)
