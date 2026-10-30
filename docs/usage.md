# Using CHASE

CHASE can be used in four ways, which share the same operations and the same
names:

- the console of `logics_tool`, which reads a specification file and runs
  commands interactively or from a script;
- the Python package `chase_cps`;
- the command line `chase-cps`, which runs checks on a specification file;
- the [MCP server](https://github.com/chase-cps/chase-mcp), which gives the
  operations to AI agents.

The console writes the input files of the external tools (e.g. an SMV model
for nuXmv), to be run by hand. The Python package, the command line, and the
MCP server run the tools and return the result, with a counterexample when a
check fails.

## Specification files

The specification files (`.ltl`) are written in the logics language of
CHASE. They are read by the console, by `chase_cps.load_spec`, and by
`chase-cps check`.

````
NAME: arbiter;

CONTRACT Spec:
    input boolean variable req;
    output boolean variable grant;
    input integer (0:3) variable n;
    Assumptions:
        G(F(!(req)));
    Guarantees:
        G(req -> F(grant));
````

- `NAME: name;` names the system; `CONTRACT name:` starts a contract.
- Variables: `input` or `output`, `boolean`, `integer (low:high)`, or `real`,
  then `variable name;`. An integer without a range ranges over 0 to 16;
  `load_spec` and `parse_spec` warn when an integer is compared with a
  constant outside its range, since the comparison then has the same value
  at every step.
  Declarations outside the contracts are shared: each contract gets those it
  uses. A real is declared without a range (the Python package rejects
  `real (low:high)`: write the range as an assumption). Constants are
  declared as `integer constant N 5;` or `real constant K 2.5;`, and
  propositions as `proposition p is x > 3;`.
- `Assumptions:` and `Guarantees:` list formulas, each ending with `;`.
- Formulas: `!` (or `~`), `&` (or `/\`), `|` (or `\/`), `->`, `<->`, `G`,
  `F`, `X`, `U`, `R`, `true`, `false`, the relations
  `= != < <= > >=`, the arithmetic `+ - * /`, and `x'` for the value of `x`
  in the next step. `#` starts a comment.
- `G`, `F`, `U`, and `R` take an interval of steps: `G[0,3] p` is `p` now
  and in the next three steps, `F]1,4] p` is `p` in one of the steps 2 to 4
  (`]` or `[` facing out leaves the bound out). The bounds are whole
  numbers, and the Python package unfolds the operator into `X`:
  `G[0,2] p` is read as `p`, `X p`, `X X p`.
- `P(...)` (a probability) is read by logics_tool, but no check takes it:
  the Python package refuses a contract that has one.

All the binary operators have the same precedence, and `G`, `F`, `X` bind
looser than them: `G a -> b` is `G (a -> b)`. Parentheses avoid surprises.

A problem written in the domain-specific language of CHASE (the patterns of
electric power systems, `DATE18/` and `FDL2024/` in the demos) is read by
`chase_cps.load_dsl(path)` into one contract, with the formulas its
patterns give; the architecture graph of the problem is left out.

## Formulas in Python

The Python package, the command line (JSON files), and the MCP server read
formulas in the standard LTL syntax: the same operators, plus `W` (weak
until) and the aliases `[]` (`G`), `<>` (`F`), `&&`, `||`, `=>`, `<=>`,
`==`. They
have the usual precedence, from the loosest: `<->`, `->`, `|` `xor`, `&`,
`U R W`, the unary operators, the relations, `<< >>`, `+ -`, `* / mod`,
`::`, the bits `x[7:0]`. So `G a -> b` is `(G a) -> b`. Names cannot be
keywords of either syntax (`G`, `F`, `X`, `U`, `R`, `W`, `P`, `NAME`,
`CONTRACT`, `mod`, `xor`, ...). Numbers may have a fraction (`20.5`); they
are read exactly. The operators of words are below (Words).

`x'` is the value of `x` at the next step, and `past(x, n)` its value n
steps before, for a variable of any type: `G X X (y = past(x, 2))` says that
y is x delayed by two steps. In the first n steps `past(x, n)` is any value
of the type, so write it under `X` as here. The model checkers keep the
past values in history variables; the contract algebra treats a formula
with `past` as one proposition. GR(1) synthesis (slugs) and the logics
language do not take it.

A contract is built from strings or from a dictionary (the JSON format of
the command line and of the MCP server):

````
from chase_cps import Contract

c = Contract.from_strings(inputs=["req"], outputs=["grant"],
                          assumptions=["G F !req"],
                          guarantees=["G (req -> F grant)"], name="Spec")
d = Contract.from_dict({"name": "Spec", "inputs": {"req": "bool", "n": [0, 3], "v": "real"},
                        "outputs": {"grant": "bool"},
                        "assumptions": ["G F !req"],
                        "guarantees": ["G (req -> F grant)"]})
````

## SystemVerilog Assertions

`parse_sva` reads the `assume property` and `assert property` statements of
SystemVerilog (a module, a bind file, or the statements alone) as a
contract: the assumptions and the guarantees. `to_sva` writes a contract as
a checker module that any tool reading SVA can check, formal or simulator.
Nothing in either is specific to a tool.

````
from chase_cps import Contract, ltl, parse_sva, to_sva

r = parse_sva("""
module arbiter (input logic clk, input logic rst, input logic req,
                output logic gnt, output logic [7:0] count);
  default clocking @(posedge clk); endclocking
  default disable iff (rst);
  A0: assume property (req |=> !req);
  G0: assert property (req |-> ##[1:2] gnt);
  G1: assert property (gnt |=> count == $past(count) + 1);
endmodule""")
print(r.contract.inputs, r.contract.outputs)
for p in r.properties:
    print(p.label, ltl.to_str(p.formula))
print(r.skipped)                    # the statements not read, with the reason

c = Contract.from_dict({"name": "Arb", "inputs": {"req": "bool"},
                        "outputs": {"gnt": "bool", "count": "word[8]"},
                        "assumptions": ["G (req -> X !req)"],
                        "guarantees": ["G (req -> X gnt | X X gnt)",
                                       "G (gnt -> count' = count + 1)"]})
print(to_sva(c, reset="rst", bind="arbiter"))
````

It prints, for the first property, `A0 G (req -> rst | X (req -> rst))`:
`disable iff (rst)` is in the formula. The checker written for `Arb`:

````systemverilog
module Arb_sva (
  input logic clk,
  input logic rst,
  (* chase = "input" *) input logic req,
  (* chase = "output" *) input logic gnt,
  (* chase = "output" *) input logic [7:0] count
);
  // assumptions
  A0: assume property (@(posedge clk) disable iff (rst)
    req |=> (!(req)));
  // guarantees
  G0: assert property (@(posedge clk) disable iff (rst)
    req |-> (((nexttime (gnt)) or (nexttime (nexttime (gnt))))));
  G1: assert property (@(posedge clk) disable iff (rst)
    gnt |=> ((count == ($past(count) + 8'd1))));
endmodule

bind arbiter Arb_sva Arb_sva_i (.*);
````

The attribute `(* chase = ... *)` keeps the role of each signal for
`parse_sva`; tools ignore it. In `to_sva`, `+ 8'd1` keeps the arithmetic of
8 bits that the formula has.

Reading:

- One step of LTL is one cycle of the clock. An assertion is checked at
  every cycle: `assert property (p)` is the guarantee `G p`, `initial assert
  property (p)` is `p`. `assume property` and `restrict property` are
  assumptions; `cover property` is kept apart (`covers`).
- The widths are those of Verilog. The operands are extended to the width
  of the context first, so in `count == $past(count) + 1` the sum has 32
  bits (as the constant 1) and does not wrap around: the property fails when
  `count` goes from 255 to 0. Vectors are words, 1-bit signals Booleans.
- `$past(e, n)`, `$rose`, `$fell`, `$stable`, `$changed`, `$onehot`,
  `$onehot0`, `$countones`. A property that looks before the first cycle is
  checked from the cycle where the past exists (`from_cycle` of the
  property): `x == $past(x)` from cycle 1. Values one cycle apart are
  written with `x'`, values further apart with `past(x, n)`:
  `y == $past(x, 2)` is `G X X y = past(x, 2)`, from cycle 2.
- Sequences of bounded length: `##n`, `##[m:n]`, `[*m:n]`, `and`, `or`,
  `intersect`, `throughout`, `within`, `first_match`. `##[m:$]` is read in
  an antecedent and in a strong sequence. In a consequent, as in
  `a |-> ##[1:$] b`, it is weak in an assert and never fails, so it is not
  read: write `a |-> s_eventually b`.
- `disable iff (r)` passes the attempt when r holds before it fails;
  `default clocking` and `default disable iff` apply.
- Named properties and sequences (with arguments) and `assign` or `wire x =
  e` definitions are put in place. `parameters` gives the values of
  parameters.
- A contract has one clock: with several, `clock` chooses one (the clock of
  most properties by default) and the properties of the others are skipped.
- The directions come from `inputs` or `outputs` (names, or a dictionary
  of types), then from the ports of the module; signals without a
  declaration are read as 1-bit signals and listed in `undeclared`.
- Packed arrays are words: `logic [3:0][7:0] x` has 32 bits, `x[2]` is
  bits 23 to 16. Hierarchical names are read as one name: `u1.state` is
  `u1__state`, and `names` maps them back (`to_sva(..., names=r.names)`
  connects them in the bind statement).

Not read, and listed in `skipped` with the reason: local variables, `[->n]`
and `[=n]`, unbounded repetition, unpacked arrays, `x` and `z` digits,
immediate assertions, and the assertions inside generate blocks (`generate`,
`if`, `for` at the level of the module).

Writing: a formula `G p` is checked at every cycle, any other formula once,
at the first cycle (`initial`); `past(x, n)` is `$past(x, n)`, the same
once the past exists. Words are vectors (`logic [7:0]`), integer
ranges `int` with their range assumed (inputs) or asserted (outputs); real
variables cannot be written. `reset` disables the properties while it holds
(`"rst"`, `"!rst_n"`), and `bind` adds a bind statement that puts the
checker in a design module, its ports connected by name.

The meaning is that of the standard, IEEE 1800-2017 (chapter 16, and Annex F
for `disable iff`), not that of a tool. Tools may differ in corner cases: when
the failure of an attempt is already decided by the values before a reset,
but a sequence of the attempt is still running when the reset comes, the
standard gives a failure and some tools (Jasper) give a disabled attempt. To
compare with a tool, run its checks on the SVA. The reading is tested against
a reference interpreter of the standard on random properties and traces,
against Verilator, and against Jasper (each property and the SVA written back
from its contract proved equivalent); a contract written and read back is
equivalent (nuXmv).

## Three layers

The operations of CHASE are in three layers:

1. **Contracts and their algebra.** The operations that build a contract
   from contracts (composition, conjunction, quotient, ...). They hold for
   any assume-guarantee contracts and are computed by the core library, in
   closed form, without a solver.
2. **Checks.** The checks on contracts (refinement, compatibility,
   consistency, implementation) reduce to queries on formulas of a theory:
   validity (`implies`) and satisfiability (`is_satisfiable`), with their
   cores. The back ends decide them: nuXmv, or NuSMV, for LTL over Boolean,
   integer, and real variables.
3. **GR(1) synthesis.** Realizability, synthesis, and the analyses built on
   them (unrealizable cores, well-separation, vacuity). They are about one
   way of using contracts, reactive synthesis from the GR(1) fragment of
   LTL with slugs, not about contracts in general.

In Python the layers are the modules `chase_cps.contract` (the `Contract`
and its methods), `chase_cps.checks` and `chase_cps.logic`, and
`chase_cps.gr1`; all the names are also in `chase_cps`. The operations are
named with verbs: the operations of the algebra return a new contract, the
checks read as predicates (`refines(a, b)`: does `a` refine `b`?).

## 1. Contracts and their algebra

| Operation | logics_tool | Python | chase-cps check | MCP tool |
|---|---|---|---|---|
| composition ∥ | `compose C1 C2 R` | `c1.compose(c2)` | `C1+C2` in a contract argument | `compose` |
| conjunction ∧ | `conjoin C1 C2 R` | `c1.conjoin(c2)` | | `conjoin` |
| disjunction ∨ | `disjoin C1 C2 R` | `c1.disjoin(c2)` | | `disjoin` |
| merging • | `merge C1 C2 R` | `c1.merge(c2)` | | `merge` |
| quotient / | `divide C1 C2 R` | `c1.divide(c2)` | | `divide` |
| separation ÷ | `separate C1 C2 R` | `c1.separate(c2)` | | `separate` |
| implication → | `imply C1 C2 R` | `c1.imply(c2)` | | `imply` |
| coimplication | `coimply C1 C2 R` | `c1.coimply(c2)` | | `coimply` |
| reciprocal C⁻¹ | `invert C R` | `c.invert()` | | `invert` |
| adding an assumption b·C | | `c.assume(b)` | | `assume` |
| adding a guarantee C·b | | `c.ensure(b)` | | `ensure` |
| saturation | `saturate C` | `c.saturate()` | | `saturate` |
| (formula simplification) | `simplify C` | `c.simplify()` | | `simplify` |

Implication and coimplication are the adjoints of conjunction and
disjunction, as the quotient and separation are of composition and merging
(Incer, Benveniste, Sangiovanni-Vincentelli, arXiv 2309.08875, Table 1).
`c1.imply(c2)` is the largest contract C such that C conjoined with c1
refines c2. The reciprocal (G, A) is the contract seen from its environment.
`c.assume(b)` gives (b & A, b -> G), an abstraction of c, and `c.ensure(b)`
gives (b -> A, b & G), which refines c.

`c1.compose(c2, discharge=True)` keeps the terms of the two contracts instead
of the closed form (Incer, PhD thesis 2022, Algorithm 2). When c1 drives the
inputs of c2 and not the reverse, the assumptions of c2 implied by the
assumptions and guarantees of c1 are dropped, then the redundant terms. This
variant uses the checks of layer 2 (one implication per test), so it needs
nuXmv. The result is an abstraction of the composition: a refinement proved
on it holds for the composition; a failed one may not.

`c1.merge(c2, discharge=True)`, `c1.divide(c2, discharge=True)`, and
`c1.conjoin(c2, discharge=True)` keep the terms too, and the result is the
same contract as the closed form: only composition gives an abstraction.
The merge is (A1 & A2, G1 & G2), whose saturation is the merge. The quotient assumes
the assumptions of c1 and the guarantees of c2, and guarantees the
guarantees of c1 and the assumptions of c2; the assumptions of c2 implied by
those of c1 are discharged first, and when some are left, each guarantee g
of c2 is assumed as A2' -> g, with A2' the ones left. Then the redundant
terms are dropped. In the conjunction, each guarantee of c1 is guaranteed
under A1', the assumptions of c1 that those of c2 do not imply (under
A1 | A2, A1 is the same as A1'), and the same for c2; the assumption is
A1' | A2' with the other terms of A1 and A2. When the assumptions of one
contract imply those of the other, there is no disjunction left: with the
same assumptions, the conjunction is (A, G1 & G2). The closed forms are
rarely GR(1); the terms often are, so use these before `is_realizable` and
`synthesize`. Disjunction and separation have no such form: their guarantee
or their assumption is a disjunction (G1 | G2, (A1 & G2) | !(G1 & A2)).

The console also accepts the names of its first version: `conjunction` for
`conjoin`, `quotient` for `divide`, and `refinement` for `refines`. It has
`show C` to print a contract (`print(c)` in Python).

In the console, the variables of two contracts are matched by name (the
default), by a map entered by hand (`manual` as last argument), or by a map
read from a file (its name as last argument). In Python, variables with the
same name are the same variable, and `c.rename({...})` renames them.

## 2. Checks

### Checks on contracts

| Check | logics_tool | Python | chase-cps check | MCP tool |
|---|---|---|---|---|
| refinement ⪯ | `refines C1 C2 file` | `refines(c1, c2)` | `--refines C1 C2` | `refines` |
| A and G hold together (implies compatibility and consistency) | `verify C file` | `verify(c)` | `--verify` (default) | `verify` |
| compatibility | | `is_compatible(c)` | | `is_compatible` |
| consistency | | `is_consistent(c)` | | `is_consistent` |
| implementation M ⊨<sub>M</sub> C | | `is_implementation(m, c)` | `--implementation MODEL C` | `is_implementation` |
| environment E ⊨<sub>E</sub> C | | `is_environment(e, c)` | `--environment MODEL C` | `is_environment` |
| receptiveness of a state machine | | `is_receptive(m, c)` | `--receptive MODEL C` | `is_receptive` |
| redundant terms | | `remove_redundant(c)` | | `remove_redundant` |

A contract (A, G) is read saturated, as A -> G. `refines(c1, c2)` checks
that A2 -> A1 and (A1 -> G1) -> (A2 -> G2) are valid. A contract is
compatible when some environment satisfies A, and consistent when some
behavior satisfies A -> G. The two can hold with different behaviors: a
contract can be compatible and consistent although its guarantees cannot
hold when its assumptions do. `verify` checks that A & G is satisfiable:
some environment meets the assumptions and some component the guarantees,
on the same behavior. This implies compatibility and consistency (with the
same witness); when A & G is unsatisfiable, `verify` also checks the two,
to tell whether the assumptions alone cannot hold, and with `core=True`
gives the terms that conflict. M is an implementation of the contract
(`is_implementation(m, c)`) when M & A -> G is valid, and E is an
environment of it (`is_environment(e, c)`) when E -> A is valid; M and E
are LTL formulas over the variables of the contract, or state machines (see
below). The names before 0.4.2, `implements(m, c)` and `accepts(c, e)`,
still work. `remove_redundant(c)` drops
the assumptions implied by the other assumptions, and the guarantees implied
by the assumptions and the other guarantees. Which terms it may drop depends
on what the contract is for next:

| Next | Call | Keeps |
|---|---|---|
| the algebra and the checks (compose, refines, verify, ...) | `remove_redundant(c)` | A -> G |
| synthesis (is_realizable, synthesize, ...) | `remove_redundant(c, synth=True)` | realizability and the controllers |

The two differ only when the assumptions are wrong, for instance
unsatisfiable, or constrain the outputs; each one keeps what the other may
change (see "Redundancy in the GR(1) game" below). With `core=True`, each
removed term comes with a minimal set of the terms that imply it.

The console writes the input file of nuXmv for `verify` and `refines`, to
be run by hand (for `verify`, three LTLSPEC: the negations of A, of the
guarantees, and of A & G; each one false means that the property holds); the Python package, the command line, and the MCP server run
nuXmv and return the result.

### State machines

An implementation or an environment can be a state machine, a `Model`
(`chase_cps.models`): a program in SMV, the input language of nuXmv, with
its variables, initial states, and transitions. It describes discrete
behaviors, one step at a time, like the traces of the contracts. A model
comes from SMV text or a file (`Model.from_smv`), from a controller of
`synthesize` (`Model.from_controller`), or from a state machine in JSON
(`Model.from_dict`): at each step, in its state, the machine takes a
transition whose `when` holds, sets the outputs of that step, and moves to
the next state. `is_implementation(m, c)` checks that every behavior of m satisfies
A -> G, and `is_environment(e, c)` that every behavior of e satisfies A; the model
declares the variables of the contract (inputs as IVAR or VAR).

A machine that refuses an input has no behavior with it: it may implement a
contract only because the input never happens. `is_receptive(m, c)` checks
that in every reachable state, every next value of the inputs that the
safety assumptions allow has a transition from which the machine can go on
forever, and that every initial value allowed by the initial assumptions
starts a behavior (a CTL check with BDDs; the inputs must be finite). The
other assumptions are not used, which asks for more inputs than OCRA does.
`not_receptive` comes with the state and the input refused.

```python
from chase_cps import Contract, Model, is_implementation, is_receptive

spec = Contract.from_strings(inputs=["req"], outputs=["grant"], name="Spec",
                             guarantees=["G (req -> X grant)"])
blocking = Model.from_smv('''MODULE main
IVAR
  req : boolean;
VAR
  grant : boolean;
ASSIGN
  init(grant) := FALSE;
TRANS
  next(grant) = req
TRANS
  !(grant & req)
''', name="Blocking")
r = is_receptive(blocking, spec)
print(is_implementation(blocking, spec).status, r.status, r.details["input"])
```

```
valid not_receptive {'req': True}
```

The machine implements the contract only because, while it grants, it
refuses a request.

### RTL

A Verilog or SystemVerilog module is a state machine too.
`Model.from_rtl(files, top)` reads it with
[Yosys](https://yosyshq.net/yosys/) (`pip install chase-cps[rtl]` installs
it, compiled to WebAssembly; a `yosys` in the path is used first): Yosys
flattens the design, maps the memories to registers, and writes it in SMV.
The ports are the variables of the contracts: a 1-bit port is a Boolean, a
wider one a word. One step is one cycle of the clock, and the clock inputs
(`clk`, `clk_i`, `clock`, or `clocks=[...]`) are left out.

````
from chase_cps import Contract, Model, is_implementation

m = Model.from_rtl(["bsg_dff_reset.sv", "bsg_dff_en.sv", "bsg_one_fifo.sv"],
                   "bsg_one_fifo", parameters={"width_p": 4}, reset="reset_i",
                   names={"ready_and_o": "ready_o"})
slot = Contract.from_dict({"name": "Slot",
                           "inputs": {"v_i": "bool", "data_i": "word[4]", "yumi_i": "bool"},
                           "outputs": {"v_o": "bool", "data_o": "word[4]", "ready_o": "bool"},
                           "assumptions": ["G (yumi_i -> v_o)"],
                           "guarantees": ["G (v_o & !yumi_i -> X v_o)"]})
print(is_implementation(m, slot).status)
````

`parameters` sets parameters of the top module, `names` renames ports as
the variables of the contracts, and `defines` gives macros. With `reset`
(`"reset_i"`, or `"!rst_n"` for an active-low reset) the reset holds in the
first step only, and the contracts are checked from the next step, the
first one after the reset; the reset is then not an input of the model, so
the contracts do not list it as an input. Without `reset` it is an input
like the others, and the registers start with any value. The checks are those of nuXmv: they suit small
and medium designs.

### Queries on formulas

| Query | logics_tool | Python | chase-cps check | MCP tool |
|---|---|---|---|---|
| validity of phi -> psi | | `implies(phi, psi, variables)` | | `implies` |
| satisfiability of phi | | `is_satisfiable(phi, variables)` | | `is_satisfiable` |
| unsat core | | `unsat_core(c)` | `--core` | `unsat_core` |

The checks on contracts reduce to these queries, in `chase_cps.logic`.
`implies` checks validity: phi -> psi holds for every behavior; the answer
is `valid`, or `invalid` with a counterexample (a behavior that satisfies
phi and violates psi). `is_satisfiable` checks that some behavior (a value
of each variable at each step) satisfies the formula; the answer is `sat`
with such a behavior, or `unsat`. This satisfiability of a formula is not
the satisfaction of a contract by a component, which is `is_implementation`.

When a check fails, a core is a minimal set of terms that explains it:
removing any term of the core makes the check pass. `unsat_core(c)` gives
the terms of A & G that cannot hold together. If they are all assumptions,
the contract is incompatible; all guarantees, the guarantees contradict
each other; both, the guarantees cannot hold when the assumptions do. This
is the consistency check of OCRA (`ocra_check_consistency -j`). The search
is delta debugging (Zeller and Hildebrandt, TSE 2002), one call to nuXmv per
test. `unsat_core(formulas, variables)` takes formulas and returns formulas.
The checks give the core of their failure with `core=True`:
`is_compatible(c, core=True)` (the assumptions), `is_consistent(c,
core=True)` (the guarantees), and `is_satisfiable(phi, variables,
core=True)`. The result is a `Core` with a `status`: `found` with the
`core`, `none` when the check does not fail, or `unknown` with a `reason`
when a test does not finish.

### Context

Some formulas are not an assumption of a component nor a guarantee: they
hold for every behavior that can happen at all. Examples are the axiom of
an encoding of finite traces (`alive & (alive U G !alive)`, where `alive`
is true for the length of the trace), a physical law, or "one tool call at
each step". The checks built on implication and satisfiability take them
as `context`, a formula or a list of formulas, and leave out the behaviors
that violate them:

```python
from chase_cps import Contract, verify

FINITE = "alive & (alive U G !alive)"
c = Contract.from_strings(inputs=["alive"], outputs=["a"], name="C",
                          guarantees=["G (alive -> a)", "G (alive -> !a)"])
assert verify(c).status == "sat"     # alive false from the start
assert verify(c, context=FINITE).status == "unsat"
```

A context is not an assumption. A behavior that violates an assumption
satisfies the contract (A -> G); one that violates the context does not
count. So `is_consistent(c.assume(FINITE))` is still sat, and a refinement
check with the axiom as an assumption of both contracts compares other
contracts. (`verify(c.assume(FINITE))` is unsat, since it checks A & G.)
The checks are:

- `implies(phi, psi, variables, context=k)`: k & phi -> psi;
- `is_satisfiable(phi, variables, context=k)`: k & phi;
- `is_compatible`, `is_consistent`, `verify`, `refines`, `remove_redundant`,
  `is_environment`, `is_implementation`, and `unsat_core` (the context is in every test of
  the search and never in the core).

The variables of the context must be variables of the check. The
realizability checks (`is_realizable`, `synthesize`, `is_well_separated`,
`is_vacuous`, `unrealizable_core`, and `remove_redundant` with `synth=True`)
do not take a context: whether a
formula constrains the environment or the component is a question of
roles, which a context does not say.

### Real variables

A variable of type `"real"` (`real variable x;`) takes any real value. The
checks handle real variables in this order, and the result says in `reals`
which way answered:

1. `regions`: when a real variable is only compared with constants
   (`kias > 40`, `kias' <= 30.5`) or with other real variables of the same
   step (`kias = kgs`), without arithmetic, only the interval between the
   constants matters. With the constants c1 < ... < ck the variable is
   replaced by the index of its region among (-inf, c1), c1, (c1, c2), ...,
   ck, (ck, inf); variables compared with each other share their constants,
   and each interval has one place per variable, so that their order is
   kept. The answer is exact, traces have real values (one in each
   region), and realizability and synthesis work: the controller of
   `synthesize` gives for each node the values of the variables, the real
   ones as conditions (`30.5 < kias < 40`), and the list of the regions.
2. `smt`: otherwise the SMT engine of nuXmv checks the formulas with the
   real variables. It is exact for linear arithmetic; with nonlinear
   arithmetic (`x * y`) it often does not finish, so it gets at most 30
   seconds. NuSMV has no real type. Realizability and synthesis are
   unknown in this case.
3. `predicates`: when nuXmv gives no answer, each comparison becomes a
   Boolean variable (`x <= 10` and `x > 10` are the same one, negated). The
   abstraction allows values that are impossible (`x > 10` and `x < 5` at
   once), so only a valid implication or an unsatisfiable formula is
   returned as such. With z3 (`pip install chase-cps[reals]`), the
   combinations of one or two comparisons that no values satisfy are
   excluded first, and an invalid or sat answer is checked: z3 looks for
   real values for every state of the trace. If it finds them, the answer
   and the trace are those of the real variables (an irrational value, such
   as a square root, is given as a close fraction); if not, the comparisons
   of a core are excluded and the check is repeated. The other answers are
   unknown, with the trace of the abstraction.

Numbers with a fraction compared with integers (`n <= 20.5`) also need the
SMT engine of nuXmv. Without real variables none of this is used.

The console of logics_tool writes the file of `verify` and `refinement`
and does not run the model checker. When the file has real variables or
real numbers, the console says so, and the file starts with a comment:
NuSMV cannot read it, and nuXmv checks it with its SMT engine:

````
$> nuXmv -int refinement.smv
nuXmv > go_msat
nuXmv > check_ltlspec_ic3
````

`go`, followed by `check_ltlspec`, stops with an error on such a file.

### Words

A word is a bit vector of a fixed width, as a register of a circuit:
`"word[8]"` is unsigned (0..255), `"signed word[8]"` is in two's complement
(-128..127). They are the words of nuXmv (`unsigned word[8]`) and the
vectors of SystemVerilog (`logic [7:0]`, `logic signed [7:0]`). Their
arithmetic wraps around: for a `word[8]` x equal to 255, x + 1 = 0. The
operators:

| Operator | Meaning |
|---|---|
| `+ - * / mod` | arithmetic modulo 2^n (`/` and `mod` truncate towards 0 for signed words) |
| `& \| xor !` | bitwise; in a relation, parenthesize them: `(x & y) = 0` |
| `<< >>` | shifts by a number (`>>` keeps the sign of a signed word) |
| `x :: y` | concatenation: x on the high bits, unsigned, the sum of the widths |
| `x[7:4]`, `x[3]` | bits, unsigned; a single bit can be used as a Boolean |
| `= != < <= > >=` | comparisons, signed or unsigned as the operands |

Both operands of an operation or a comparison have the same width and
signedness; an integer constant takes the type of the other operand
(`x = 5`) and must fit in it. Change a width with bits (`y[7:0]`) or a
concatenation of words (`x :: y`). A formula that does not type is refused
when the contract is built.

````
from chase_cps import Contract, refines

inc = Contract.from_dict({"name": "Inc", "inputs": {"a": "word[8]"},
                          "outputs": {"b": "word[8]"},
                          "guarantees": ["G (b = a + 1)"]})
odd = Contract.from_dict({"name": "Odd", "inputs": {"a": "word[8]"},
                          "outputs": {"b": "word[8]"},
                          "assumptions": ["G (a[0] = 0)"],
                          "guarantees": ["G (b[0] = 1)"]})
print(refines(inc, odd).status)     # valid: a even, so a + 1 is odd
````

The checks give the words to nuXmv (and NuSMV) as they are; the values in
the traces are integers. The contract algebra of the core library reads the
logics language, which has no words: each comparison over words goes to it
as a Boolean variable and comes back after the operation, which only
combines the formulas, so the result is the one of the formulas. For the
same reason a contract with words cannot be written in the logics language
(`to_spec`). State machines (`Model`) may declare words. slugs does not
take them: GR(1) synthesis needs integer ranges.

## 3. GR(1) synthesis

| Analysis | logics_tool | Python | chase-cps check | MCP tool |
|---|---|---|---|---|
| realizability | | `is_realizable(c)` | `--realizable C` | `is_realizable` |
| synthesis | `synthesize C file solver` | `synthesize(c)` | `--synthesize C` | `synthesize` |
| unrealizable core | | `unrealizable_core(c)` | `--realizable C --core` | `unrealizable_core` |
| well-separation | | `is_well_separated(c)` | `--well-separated C` | `is_well_separated` |
| vacuity | | `is_vacuous(c)` | `--vacuous C` | `is_vacuous` |
| redundancy in the GR(1) game | | `remove_redundant(c, synth=True)` | | `remove_redundant` with `synth` |

These analyses are about reactive synthesis from contracts in the GR(1)
fragment of LTL, with slugs, in `chase_cps.gr1`. A contract is realizable
when a component that controls the outputs can satisfy A -> G against every
environment that controls the inputs; this depends on the synthesis
approach, and is not a property of contracts in general. The terms must be
GR(1): initial (no temporal operator), safety (G f, with X only in f), or
justice (G F f). The checks do not take a context: whether a formula
constrains the environment or the component is a question of roles, which a
context does not say.

`is_realizable` gives the counter-strategy of the environment when the
contract is unrealizable; `synthesize` also builds the controller. The
console writes the input file of slugs or gr1c, which decide realizability
and build the controller.

`to_gr1(c, format=...)` writes a GR(1) contract as the input of a GR(1)
solver, to run by hand, as `to_sva` does for the tools of SystemVerilog.
`format="slugs"` (the default) is the file that chase-cps gives to slugs,
in the structured format of slugs; `format="gr1c"` is the input of gr1c
(`gr1c -r spec.spc`). The inputs are the variables of the environment and
the outputs those of the system. Structured slugs has no subtraction or
multiplication; gr1c compares an integer only with a constant and starts
its domains at 0, so for gr1c a relation with arithmetic or between
variables is written as the values that satisfy it, and an integer is
written from 0.

### Unrealizable cores

`unrealizable_core(c)` gives a minimal set of guarantees that is
unrealizable under all the assumptions. The search minimizes the justice
guarantees, then the safety, then the initial ones (QuickCore, Maoz and
Shalom, ICSE 2021), with one call to slugs per test.
`unrealizable_core(guarantees, inputs, outputs, assumptions)` takes formulas
and returns formulas, and `is_realizable(c, core=True)` gives the core of an
unrealizable result.

### Well-separation

A contract is well-separated when the component cannot satisfy it by
forcing the environment to violate the assumptions (Klein and Pnueli, HVC
2010; Maoz and Ringert, FSE 2016). A contract that is not well-separated is
realizable for a bad reason: the controller may break its environment
instead of meeting the guarantees. `is_well_separated(c)` answers
`separated` or `not_separated` with the `controller` that forces the
violation. The component must keep its initial and safety guarantees while
it forces the violation (Def. 4 of Maoz and Ringert): a violation it can
force only by breaking its own guarantees is not a defect of the
assumptions. `with_guarantees=False` leaves the guarantees out (Def. 2, the
default before 0.3.2). `core=True` gives a
minimal set of assumptions that causes the problem. The check is one call
to slugs. It detects the violations that can be forced from every initial
state; those forced only from some reachable state need the winning region,
which slugs does not give.

### Vacuity

A contract is vacuous when the component can win without an output that a
liveness guarantee asks for: the guarantee is discharged by the assumptions
and obliges nothing. `is_vacuous(c)` checks it as a strengthening of the
guarantees (Bloem, Chockler, Ebrahimi, and Strichman, FMSD 2021), with at
most two calls to slugs per output y:

- never: Real(A, G + {G !y}), a winning strategy never sets y;
- eventually: Real(A, G + {!p, G (p -> X p), G (p -> !y), G F p}) with a
  fresh output p, a winning strategy stops setting y from some step on.

The outputs checked are those that occur only positively in a justice
guarantee, or the `outputs` given. The result is `vacuous` with each vacuous
output, its mode and the strategy, `not_vacuous`, `unrealizable` when the
contract itself is, or `unknown`; with `core=True`, each vacuous output has
a minimal set of the temporal assumptions under which the component still
wins without it (one deletion pass). Two weaker formulations are not used:
the test on a controller M & A -> phi[y <- false] (Kupferman and Vardi) and
the rewrite Real(A, G[y <- false]) both call a good controller vacuous when
an assumption ties the release of the environment to y, such as
G ((req & grant) -> X !req): the controller grants, the release follows,
and phi[grant <- false] holds because it granted. Forcing y false in the
behavior asks the right question.

```python
from chase_cps import Contract, is_vacuous, ltl

trusting = Contract.from_strings(
    inputs=["req"], outputs=["grant"], name="Trusting",
    assumptions=["!req", "G F !req"],
    guarantees=["!grant", "G (grant -> req)", "G F (!req | grant)"])
r = is_vacuous(trusting, core=True)
v = r.vacuous[0]
print(r.status, v.output, v.mode, [ltl.to_str(t) for t in v.core.core])
```

```
vacuous grant never ['G F !req']
```

The assumption G F !req makes G F (!req | grant) hold without any grant.

### Redundancy in the GR(1) game

`remove_redundant(c, synth=True)` removes the terms whose removal does not
change the GR(1) specification: its realizability and the controllers that
realize it (the inherent vacuity of Maoz and Shalom, ESEC/FSE 2020, which is
a redundancy: a term implied by others in the game). Each term is checked
against a premise set that depends on its kind (initial, safety, justice)
and on its side (assumption or guarantee), one call to nuXmv per term; a
safety term is compared with the bodies of the other safety terms only, as
a propositional implication over the current and next values. The terms are
checked in order against the terms not yet removed, so of two equal terms
one is kept. The result has the `contract`, the `removed` terms with their
part and kind, and with `core=True` the terms that imply each one.

`remove_redundant(c)` without `synth` keeps A -> G instead. The two differ
when the assumptions are wrong:

| Contract | `remove_redundant(c)` | `remove_redundant(c, synth=True)` |
|---|---|---|
| A: G F (x & !x); G: G g, G !g | drops both guarantees (A is unsatisfiable): realizable | drops nothing: unrealizable, as c |
| A: G F g; G: G g | drops nothing | drops G F g: the component keeps G g anyway, but the contract is stronger |

In the game, the component must keep its safety guarantees even when the
environment breaks its justice assumptions, so these do not imply them; an
assumption that the guarantees make hold does not matter in the game, but
it does for A -> G.

## Diagrams

`chase_cps.viz` draws contracts, components, and systems as a web page:

- A contract is a rounded box; a click on its name shows its assumptions
  (A) and guarantees (G).
- A component with a model (RTL, a state machine in JSON or SMV, or a
  controller) is a square box; its model opens on its states and moves. The
  icon of a synthesized model (a controller of slugs) has a spark.
- Refinement is nesting: a decomposition, or a contract, is drawn inside
  the box of the contract it refines, on as many levels as there are
  refinements. A box with boxes inside folds them away (the button at its
  top right, or a click on ⪯). A quotient is a hatched hole next to the
  known parts, with what refines it inside.
- An environment is a grey box on the left of what it drives: the outside
  of the design, checked against the assumptions only. Something that has a
  contract of its own is a component.
- The results of the checks are tags next to the name of a box: A
  (compatible), G (consistent), A∧G (feasible, shown when it fails), R
  (realizable), WS (well-separated), ¬V (no vacuous output), M ⊨ C, E ⊨ A,
  and ⪯ (refines), green when the check holds,
  red when it does not. A click on a red tag with a core puts the terms of
  the core on a red ground.
- The operations are written as in Benveniste et al. (Contracts for System
  Design, 2018): ⊗ composition, ∧ conjunction and • merging (viewpoints of
  one component: one box with the ports of the result, the viewpoints
  inside), / quotient, ⪯ refinement.
- The context of the checks is a tag above the diagram.

The diagram fills the window: drag to move it, the wheel (or a pinch) zooms
around the pointer, and the button with four corners (or the key 0) fits it
to the window, centered.

The structure comes from the contracts: each result of the algebra
remembers its operands (`Contract.origin`). The page lays the boxes out
with ELK, loaded from a CDN.

```python
from chase_cps import Component, Contract, System, viz, verify, is_realizable, refines

a = Contract.from_strings(name="A", inputs=["x"], outputs=["y"], guarantees=["G (x -> X y)"])
b = Contract.from_strings(name="B", inputs=["y"], outputs=["z"], guarantees=["G (y -> z)"])
top = Contract.from_strings(name="Top", inputs=["x"], outputs=["y", "z"],
                            guarantees=["G (x -> X z)"])
s = System("S", [Component("a", a), Component("b", b)], contract=top)
s.check()                      # the composition against Top: drawn inside it
q = top.divide(a, name="Q")    # what the part next to a must do
greedy = Contract.from_strings(name="Greedy", inputs=["r1", "r2"], outputs=["g1", "g2"],
                               guarantees=["G !(g1 & g2)", "G (r1 -> g1)", "G (r2 -> g2)"])
k = "G !(r1 & r2)"             # the requests never come together
d = viz.diagram(s, q, greedy,
                refines=[(b, q, refines(b, q))],
                checks=[(greedy, verify(greedy, context=k)), (greedy, is_realizable(greedy, core=True))],
                environments=[("G F x", s)],
                context=k)
viz.save(d, "plan.html")       # viz.show(d) opens the browser; in Jupyter, a system draws itself
```

`checks` takes `(item, result)` with the result of `verify` (A, G, and
A∧G), `is_realizable` (R), `is_well_separated` (WS), `is_vacuous` (¬V, with
the guarantees that name a vacuous output as its core), or `is_receptive`;
with `core=True`, the core comes with the check that fails. An unrealizable
core shows the guarantees only, since it keeps all the assumptions. A result
that is unknown does not say which check it is: `(item, "realizable",
result)`, or `well_separated`, `vacuous`, `receptive`. The results of
`is_compatible` and `is_consistent` (sat or unsat) always come with their
name: `(item, "consistent", result)`. An item that refines several
contracts is drawn inside each of them, and each candidate for a quotient
gets a copy of the quotient with its own hole. `environments` takes
an LTL formula, a `Model`, or a `Component` whose implementation is the
environment.
`open` takes the items drawn open when the page loads: the terms of a
contract, the states of a model (with 8 states or fewer), and the core of a
check that fails.

On the command line, `chase-cps view FILE` writes the page of a
specification; `--refines R A` draws R inside A, `--environment MODEL C`
draws an environment, `--check` checks the refinements and the
environments, `--verify` and `--realizable` mark the checks of each
contract drawn, `--open C...` draws contracts open, and `--serve` draws
the page again whenever the file changes. `viz.from_spec(path, ...)` makes the same diagram in Python.

The page loads ELK (elkjs 0.9.3) from a CDN. `viz.html(d, elk=PATH)` and
`chase-cps view --elk PATH` put a local `elk.bundled.js` in the page
instead, for a page that works offline. With `CHASE_VIZ_OUTPUT=FILE` in the
environment, `viz.show` writes the page to FILE and opens no browser
(`CHASE_VIZ_ELK=PATH` puts ELK in it).

The diagrams are also drawn by the MCP server of CHASE (the `visualize`
tool of chase-mcp, for AI agents) and by the CHASE extension of VS Code,
which shows the diagram of a specification, or of a Python script that
calls `viz.show`, beside the editor and draws it again on save.

## Results

The checks return a result with a `status`: `valid` or `invalid` for
implications and refinement, `sat` or `unsat` for satisfiability,
compatibility, consistency, and verify, `receptive` or `not_receptive` for
receptiveness, `realizable` or `unrealizable` for realizability and
synthesis, `separated` or `not_separated` for well-separation, `vacuous` or
`not_vacuous` for vacuity, and `unknown` with a `reason` when the check
does not finish or no tool is installed. A failed check comes with a
`trace`: the states of `prefix`, then the states of `loop` repeated forever.
`synthesize` returns the `controller`, and both `is_realizable` and
`synthesize` return the `counter_strategy` of the environment when the
contract is unrealizable.

`refines` checks the assumptions and the guarantees separately; `failed`
says which part fails, and its result has the counterexample.

## External tools

The checks use [nuXmv](https://nuxmv.fbk.eu/) (free for academic and
non-commercial use) and [slugs](https://github.com/VerifiableRobotics/slugs)
for synthesis:

````
$> chase-cps install nuxmv --accept-license
$> chase-cps install slugs
$> chase-cps doctor
````

### Engines

nuXmv checks the formulas with IC3 and BDDs at the same time (`portfolio`):
the first verdict wins, and the other run is stopped. Both engines prove
and refute, so the verdict is the same whichever answers; the
counterexample may differ. One engine can be chosen with `engine`, or in
the name of the back end, as the results name it:

| Engine | Proves | Finds counterexamples | Good for |
| --- | --- | --- | --- |
| `portfolio` (default with nuXmv) | yes | yes | IC3 and BDDs at the same time (IC3 and BMC for reals) |
| `ic3` | yes | yes | large state spaces, words, reals |
| `bdd` | yes | yes | a counterexample that needs a long loop (a counter that goes around) |
| `bmc` | no | up to `bmc_bound` steps (20) | short counterexamples |

````
from chase_cps import implies

r = implies("cnt = 254 & G (cnt' = cnt + 1)", "X (cnt = 0)", {"cnt": "word[8]"},
            "nuxmv/bdd")
print(r.status, r.backend)      # invalid nuxmv/bdd
````

The counterexample is a loop of 256 steps: BDDs find it at once, IC3 takes
minutes, and the default gives the answer of BDDs. Formulas with `signed()`
or `unsigned()` are checked with BDDs only: IC3 of nuXmv 2.2.0 gives wrong
answers on them. On the command line the option is `--engine`; the environment
variable `CHASE_BACKEND` may also name an engine (`nuxmv/portfolio`).
NuSMV has no IC3: without nuXmv, the default first looks for a
counterexample with BMC, then proves with BDDs. The SMT engine of nuXmv
(real variables with arithmetic) works with `ic3` and `bmc` only.
Receptiveness is a CTL check and always uses BDDs.

The full API of the Python package is in the
[API reference](https://chase-cps.github.io/api/).
