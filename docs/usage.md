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
  then `variable name;`. An integer without a range ranges over 0 to 16.
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

## Operations

The operations are named with verbs. The operations of the contract algebra
return a new contract; the checks read as predicates (`refines(a, b)`: does
`a` refine `b`?).

| Contract theory | logics_tool | Python | chase-cps check | MCP tool |
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
| refinement ⪯ | `refines C1 C2 file` | `refines(c1, c2)` | `--refines C1 C2` | `refines` |
| compatibility and consistency | `verify C file` | `verify(c)` | `--verify` (default) | `verify` |
| realizability | | `is_realizable(c)` | `--realizable C` | `is_realizable` |
| synthesis | `synthesize C file solver` | `synthesize(c)` | `--synthesize C` | `synthesize` |
| unrealizable core | | `unrealizable_core(c)` | `--realizable C --core` | `unrealizable_core` |
| unsat core | | `unsat_core(c)` | `--core` | `unsat_core` |
| well-separation | | `is_well_separated(c)` | `--well-separated C` | `is_well_separated` |
| vacuity | | `remove_vacuous(c)` | | `remove_vacuous` |

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
assumptions and guarantees of c1 are dropped, then the redundant terms. Each
test is one call to nuXmv. The result is an abstraction of the composition:
a refinement proved on it holds for the composition; a failed one may not.

The console also accepts the names of its first version: `conjunction` for
`conjoin`, `quotient` for `divide`, and `refinement` for `refines`. It has
`show C` to print a contract (`print(c)` in Python).

In the console, the variables of two contracts are matched by name (the
default), by a map entered by hand (`manual` as last argument), or by a map
read from a file (its name as last argument). In Python, variables with the
same name are the same variable, and `c.rename({...})` renames them.

`is_realizable` checks GR(1) realizability with slugs and gives the
counter-strategy of the environment when the contract is unrealizable;
`synthesize` also builds the controller. The console writes the input file of
slugs or gr1c, which decide realizability and build the controller.

The Python package, the command line, and the MCP server have also:

- `is_compatible(c)` and `is_consistent(c)`, the two checks of `verify`;
- `implies(phi, psi, variables)` and `is_satisfiable(phi, variables)` on
  formulas;
- `accepts(c, e)`: the contract accepts the environment E (E -> A), and
  `implements(m, c)`: M is an implementation of the contract (M & A -> G);
- `remove_redundant(c)`, which drops the terms implied by the others;
- `load_spec(path)`, `parse_spec(text)`, and `to_spec(contracts)`, which
  read and write specification files.

## Cores

When a check fails, a core is a minimal set of terms that explains it:
removing any term of the core makes the check pass.

- `unrealizable_core(c)`: the guarantees that are unrealizable under all the
  assumptions. The search minimizes the justice guarantees, then the safety,
  then the initial ones (QuickCore, Maoz and Shalom, ICSE 2021), with one
  call to slugs per test.
- `unsat_core(c)`: the terms of A & G that cannot hold together. If they are
  all assumptions, the contract is incompatible; all guarantees, the
  guarantees contradict each other; both, the guarantees cannot hold when
  the assumptions do. This is the consistency check of OCRA
  (`ocra_check_consistency -j`). One call to nuXmv per test.

Both also take formulas: `unsat_core(formulas, variables)` and
`unrealizable_core(guarantees, inputs, outputs, assumptions)`, and then
return formulas. The checks give the core of their failure with
`core=True`: `is_realizable(c, core=True)`, `is_compatible(c, core=True)`
(the assumptions), `is_consistent(c, core=True)` (the guarantees), and
`is_satisfiable(phi, variables, core=True)`. The result is a `Core` with a
`status`: `found` with the `core`, `none` when the check does not fail, or
`unknown` with a `reason` when a test does not finish.

## Well-separation

A contract is well-separated when the component cannot satisfy it by
forcing the environment to violate the assumptions (Klein and Pnueli, HVC
2010; Maoz and Ringert, FSE 2016). A contract that is not well-separated is
realizable for a bad reason: the controller may break its environment
instead of meeting the guarantees. `is_well_separated(c)` answers
`separated` or `not_separated` with the `controller` that forces the
violation; `with_guarantees=True` makes the component keep its initial and
safety guarantees (Def. 4 of Maoz and Ringert), and `core=True` gives a
minimal set of assumptions that causes the problem. The check is one call
to slugs. It detects the violations that can be forced from every initial
state; those forced only from some reachable state need the winning region,
which slugs does not give.

## Vacuity

`remove_vacuous(c)` removes the terms whose removal does not change the
GR(1) specification: its realizability and the controllers that realize it
(inherent vacuity, Maoz and Shalom, ESEC/FSE 2020). Each term is checked
against a premise set that depends on its kind (initial, safety, justice)
and on its side (assumption or guarantee), one call to nuXmv per term; a
safety term is compared with the bodies of the other safety terms only, as
a propositional implication over the current and next values. The terms are
checked in order against the terms not yet removed, so of two equal terms
one is kept. The result has the `contract`, the `removed` terms with their
part and kind, and with `core=True` the terms that make each one vacuous.

`remove_redundant(c)` instead keeps A -> G: it may remove a term that only
follows from the others over whole traces, which can change the synthesis
problem; `remove_vacuous` does not.

## Real variables

A variable of type `"real"` (`real variable x;`) takes any real value. The
checks handle real variables in this order, and the result says in `reals`
which way answered:

1. `regions`: when a real variable is only compared with constants
   (`kias > 40`, `kias' <= 30.5`), only the interval between the constants
   matters. With the constants c1 < ... < ck the variable is replaced by
   the index of its region among (-inf, c1), c1, (c1, c2), ..., ck,
   (ck, inf). The answer is exact, traces have real values (one in each
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

## Results

The checks return a result with a `status`: `valid` or `invalid` for
implications and refinement, `sat` or `unsat` for satisfiability,
compatibility, consistency, and verify, `realizable` or `unrealizable` for
realizability and synthesis, and `unknown` with a `reason` when the check
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
