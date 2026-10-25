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

| logics_tool | Python | chase-cps check | MCP tool |
|---|---|---|---|
| `show C` | `print(c)` | | |
| `compose C1 C2 R` | `c1.compose(c2)` | `C1+C2` in a contract argument | `compose` |
| `conjunction C1 C2 R` | `c1.conjunction(c2)` | | `conjunction` |
| `disjunction C1 C2 R` | `c1.disjunction(c2)` | | `disjunction` |
| `merging C1 C2 R` | `c1.merging(c2)` | | `merging` |
| `quotient C1 C2 R` | `c1.quotient(c2)` | | `quotient` |
| `separation C1 C2 R` | `c1.separation(c2)` | | `separation` |
| `saturate C` | `c.saturate()` | | `saturate` |
| `simplify C` | `c.simplify()` | | `simplify` |
| `verify C file` | `verify(c)` | `--verify` (default) | `verify` |
| `refinement C1 C2 file` | `refinement(c1, c2)` | `--refinement C1 C2` | `refinement` |
| `synthesize C file solver` | `synthesize(c)` | `--synthesize C` | `synthesize` |

In the console, the variables of two contracts are matched by name (the
default), by a map entered by hand (`manual` as last argument), or by a map
read from a file (its name as last argument). In Python, variables with the same name
are the same variable, and `c.rename({...})` renames them.

The Python package, the command line, and the MCP server have also:

- `compatible(c)` and `consistent(c)`, the two checks of `verify`;
- `implies(phi, psi, variables)` and `satisfiable(phi, variables)` on
  formulas;
- `contains_environment(c, e)` (E -> A) and `contains_implementation(c, m)`
  (M & A -> G);
- `remove_redundant(c)`, which drops the terms implied by the others;
- `load_spec(path)`, `parse_spec(text)`, and `to_spec(contracts)`, which
  read and write specification files.

## Results

The checks return a result with a `status`: `valid` or `invalid` for
implications and refinement, `sat` or `unsat` for satisfiability,
compatibility, consistency, and verify, `realizable` or `unrealizable` for
synthesis, and `unknown` with a `reason` when the check does not finish or
no tool is installed. A failed check comes with a `trace`: the states of
`prefix`, then the states of `loop` repeated forever. `synthesize` returns
the `controller`, or the `counter_strategy` of the environment.

`refinement` checks the assumptions and the guarantees separately; `failed`
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
