"""Tests of RTL as state machines (chase_cps.rtl): small Verilog modules
read with Yosys and checked against contracts. Skipped without Yosys or
nuXmv."""

import pytest

from chase_cps import Contract, Model, implements, is_receptive
from chase_cps.backends import ADAPTERS
from chase_cps.rtl import RTLError, from_verilog, yosys

pytestmark = pytest.mark.skipif(yosys() is None or not ADAPTERS["nuxmv"].available(),
                                reason="Yosys or nuXmv not installed")

BUF1 = """
module buf1 #(parameter W = 4) (input clk, input reset_i, input v_i, input [W-1:0] data_i,
                                output ready_o, output v_o, output [W-1:0] data_o,
                                input yumi_i);
  logic full;
  logic [W-1:0] d;
  assign ready_o = !full;
  assign v_o = full;
  assign data_o = d;
  always_ff @(posedge clk) begin
    if (reset_i) full <= 1'b0;
    else if (full) full <= !yumi_i;
    else full <= v_i;
    if (v_i && !full) d <= data_i;
  end
endmodule
"""

BUF2 = """
module buf2 #(parameter W = 4) (input clk, input reset_i, input v_i, input [W-1:0] data_i,
                                output ready_param_o, output v_o, output [W-1:0] data_o,
                                input yumi_i);
  logic [W-1:0] mem [0:1];
  logic head, tail;
  logic [1:0] count;
  wire enq = v_i && ready_param_o;
  assign ready_param_o = count != 2'd2;
  assign v_o = count != 2'd0;
  assign data_o = mem[head];
  always_ff @(posedge clk) begin
    if (reset_i) begin
      head <= 1'b0; tail <= 1'b0; count <= 2'd0;
    end else begin
      if (enq) begin mem[tail] <= data_i; tail <= !tail; end
      if (yumi_i) head <= !head;
      count <= count + enq - yumi_i;
    end
  end
endmodule
"""

COUNTER = """
module counter (input clk, input rst_n, input en, output logic [3:0] n);
  always_ff @(posedge clk)
    if (!rst_n) n <= 4'd0;
    else if (en) n <= n + 4'd1;
endmodule
"""

# a slot between a producer and a consumer: the consumer takes only what is
# offered; what is offered stays until it is taken; what is taken was given
SLOT = {"name": "Slot", "inputs": {"v_i": "bool", "data_i": "word[4]", "yumi_i": "bool"},
        "outputs": {"v_o": "bool", "data_o": "word[4]", "ready_o": "bool"},
        "assumptions": ["G (yumi_i -> v_o)"],
        "guarantees": ["G (v_o & !yumi_i -> X v_o)",
                       "G (v_o & !yumi_i -> data_o' = data_o)"]}


def write(tmp_path, name, text):
    f = tmp_path / f"{name}.sv"
    f.write_text(text)
    return str(f)


def test_a_buffer_implements_its_contract(tmp_path):
    m = Model.from_verilog(write(tmp_path, "buf1", BUF1), "buf1", reset="reset_i")
    assert m.variables == {"reset_i": "bool", "v_i": "bool", "data_i": "word[4]",
                           "ready_o": "bool", "v_o": "bool", "data_o": "word[4]",
                           "yumi_i": "bool"} or set(m.variables) >= {"v_i", "data_o"}
    assert m.ivars == {"v_i", "data_i", "yumi_i"}
    c = Contract.from_dict({**SLOT, "guarantees": SLOT["guarantees"] + [
        "G X (v_i & ready_o -> X (v_o & data_o = past(data_i, 1)))"]})
    assert m.start == 1
    assert implements(m, c).status == "valid"
    assert is_receptive(m, c).status == "receptive"
    # always ready: false once it holds a value
    r = implements(m, Contract.from_dict({**SLOT, "guarantees": ["G ready_o"]}))
    assert r.status == "invalid"
    states = r.trace.prefix + r.trace.loop
    assert any(s["v_o"] and not s["ready_o"] for s in states)


def test_memories_names_and_parameters(tmp_path):
    f = write(tmp_path, "buf2", BUF2)
    m = from_verilog(f, "buf2", reset="reset_i", names={"ready_param_o": "ready_o"})
    assert implements(m, Contract.from_dict(SLOT)).status == "valid"
    # two entries: ready again after one value is in
    # two entries: empty, one value in and none taken, still ready (one entry: not)
    two = Contract.from_dict({**SLOT, "guarantees": ["G (!v_o & v_i & !yumi_i -> X ready_o)"]})
    assert implements(m, two).status == "valid"
    one = from_verilog(write(tmp_path, "buf1", BUF1), "buf1", reset="reset_i")
    assert implements(one, two).status == "invalid"
    m8 = from_verilog(f, "buf2", parameters={"W": 8}, reset="reset_i")
    assert m8.variables["data_o"] == "word[8]"


def test_active_low_reset(tmp_path):
    m = from_verilog(write(tmp_path, "counter", COUNTER), "counter", reset="!rst_n")
    c = Contract.from_dict({"name": "Count", "inputs": {"en": "bool"},
                            "outputs": {"n": "word[4]"},
                            "guarantees": ["n = 0", "G (en -> n' = n + 1)",
                                           "G (!en -> n' = n)"]})
    assert implements(m, c).status == "valid"
    assert implements(m, Contract.from_dict({**c.to_dict(), "guarantees": ["G (n < 15)"]})) \
        .status == "invalid"
    # without the reset the counter starts anywhere
    free = from_verilog(write(tmp_path, "counter", COUNTER), "counter")
    assert free.start == 0 and "rst_n" in free.ivars


def test_errors(tmp_path, monkeypatch):
    with pytest.raises(RTLError, match="could not read"):
        from_verilog(write(tmp_path, "bad", "module bad(input a; endmodule"), "bad")
    with pytest.raises(RTLError, match="not ports"):
        from_verilog(write(tmp_path, "buf1", BUF1), "buf1", names={"nope": "x"})
    with pytest.raises(RTLError, match="no file"):
        from_verilog(str(tmp_path / "missing.sv"), "missing")
    import chase_cps.rtl as rtl
    monkeypatch.setattr(rtl, "yosys", lambda: None)
    with pytest.raises(RTLError, match="not installed"):
        rtl.from_verilog(write(tmp_path, "buf1", BUF1), "buf1")
