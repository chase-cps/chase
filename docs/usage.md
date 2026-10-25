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
  then `variable name;`. Constants are declared as `integer constant N 5;`,
  and propositions as `proposition p is x > 3;`.
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
`R`, `W`, `P`, `NAME`, `CONTRACT`, ...).

A contract is built from strings or from a dictionary (the JSON format of
the command line and of the MCP server):

````
from chase_cps import Contract

c = Contract.from_strings(inputs=["req"], outputs=["grant"],
                          assumptions=["G F !req"],
                          guarantees=["G (req -> F grant)"], name="Spec")
d = Contract.from_dict({"name": "Spec", "inputs": {"req": "bool", "n": [0, 3]},
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
