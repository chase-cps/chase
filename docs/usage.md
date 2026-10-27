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
  `F`, `X`, `U`, `R`,
  `P(...)` (probability), `true`, `false`, the relations
  `= != < <= > >=`, the arithmetic `+ - * /`, and `x'` for the value of `x`
  in the next step. `#` starts a comment.

All the binary operators have the same precedence, and `G`, `F`, `X` bind
looser than them: `G a -> b` is `G (a -> b)`. Parentheses avoid surprises.

## Formulas in Python

The Python package, the command line (JSON files), and the MCP server read
formulas in the standard LTL syntax: the same operators, plus `W` (weak
until) and the aliases `[]` (`G`), `<>` (`F`), `&&`, `||`, `=>`, `<=>`,
`==`. They
have the usual precedence, from the loosest: `<->`, `->`, `|`, `&`,
`U R W`, the unary operators, the relations, `+ -`, `* /`. So `G a -> b` is
`(G a) -> b`. Names cannot be keywords of either syntax (`G`, `F`, `X`, `U`,
`R`, `W`, `P`, `NAME`, `CONTRACT`, ...). Numbers may have a fraction
(`20.5`); they are read exactly.

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
| implementation M ⊨ C | | `implements(m, c)` | `--implements MODEL C` | `implements` |
| acceptance of an environment E | | `accepts(c, e)` | | `accepts` |
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
gives the terms that conflict. A component M satisfies
the contract (`implements(m, c)`) when M & A -> G is valid, and the contract
accepts an environment E (`accepts(c, e)`) when E -> A is valid; M and E are
LTL formulas over the variables of the contract, or state machines (see
below). `remove_redundant(c)` drops
the assumptions implied by the other assumptions, and the guarantees implied
by the assumptions and the other guarantees.

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
the next state. `implements(m, c)` checks that every behavior of m satisfies
A -> G, and `accepts(c, e)` that every behavior of e satisfies A; the model
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
from chase_cps import Contract, Model, implements, is_receptive

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
print(implements(blocking, spec).status, r.status, r.details["input"])
```

```
valid not_receptive {'req': True}
```

The machine implements the contract only because, while it grants, it
refuses a request.

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
the satisfaction of a contract by a component, which is `implements`.

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
  `accepts`, `implements`, and `unsat_core` (the context is in every test of
  the search and never in the core).

The variables of the context must be variables of the check. The
realizability checks (`is_realizable`, `synthesize`, `is_well_separated`,
`remove_vacuous`, `unrealizable_core`) do not take a context: whether a
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

## 3. GR(1) synthesis

| Analysis | logics_tool | Python | chase-cps check | MCP tool |
|---|---|---|---|---|
| realizability | | `is_realizable(c)` | `--realizable C` | `is_realizable` |
| synthesis | `synthesize C file solver` | `synthesize(c)` | `--synthesize C` | `synthesize` |
| unrealizable core | | `unrealizable_core(c)` | `--realizable C --core` | `unrealizable_core` |
| well-separation | | `is_well_separated(c)` | `--well-separated C` | `is_well_separated` |
| vacuity | | `is_vacuous(c)` | `--vacuous C` | `is_vacuous` |
| redundancy in the GR(1) game | | `remove_vacuous(c)` | | `remove_vacuous` |

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

`remove_vacuous(c)` removes the terms whose removal does not change the
GR(1) specification: its realizability and the controllers that realize it
(the inherent vacuity of Maoz and Shalom, ESEC/FSE 2020, which is a
redundancy: a term implied by others in the game). Each term is checked
against a premise set that depends on its kind (initial, safety, justice)
and on its side (assumption or guarantee), one call to nuXmv per term; a
safety term is compared with the bodies of the other safety terms only, as
a propositional implication over the current and next values. The terms are
checked in order against the terms not yet removed, so of two equal terms
one is kept. The result has the `contract`, the `removed` terms with their
part and kind, and with `core=True` the terms that imply each one.

`remove_redundant(c)` (layer 2) instead keeps A -> G: it may remove a term
that only follows from the others over whole traces, which can change the
synthesis problem; `remove_vacuous` does not.

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

The full API of the Python package is in the
[API reference](https://chase-cps.github.io/api/).
