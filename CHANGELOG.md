# Changelog

## 0.3.2 (not released yet)

Changes of behavior:

- `verify` checks that the assumptions and the guarantees can hold together
  (A & G satisfiable). This implies compatibility and consistency, which it
  still reports; before, a contract whose guarantees cannot hold when its
  assumptions do passed `verify` (A -> G holds when A fails). With
  `core=True` an unsat result carries the terms that conflict. On the 513
  contracts of the ContrAgent libraries, the results are the same. The
  console `verify` command writes a third LTLSPEC, the negation of A & G.
  `chase-cps check` without options prints one `verify` line per contract.
- `is_well_separated` keeps the initial and safety guarantees of the
  component by default (`with_guarantees=True`, Maoz and Ringert, Def. 4):
  a violation of the assumptions that the component can force only by
  breaking its own guarantees is not a defect of the assumptions.
  `with_guarantees=False` gives the check of before.

New:

- `is_vacuous`: vacuity in synthesis (Bloem, Chockler, Ebrahimi, and
  Strichman, FMSD 2021). Can the component win without an output that a
  liveness guarantee asks for, never or from some step on? With the
  strategy, and with `core=True` the assumptions that allow it. The queries
  are those of the exact vacuity checks of ACIES. `remove_vacuous` is
  unchanged; it is now described as what it is, the removal of the terms
  that are redundant in the GR(1) game.
- State machines (`chase_cps.models`): a `Model` from SMV text or a file,
  from a controller of `synthesize`, or from a state machine in JSON.
  `implements` and `accepts` take them, and `is_receptive` checks that a
  model does not refuse the inputs that the assumptions allow (the state
  and the input when it does). `synthesize` keeps the initial terms of the
  contract with the controller, since slugs does not mark the initial nodes.
- `chase-cps check`: `--implements MODEL C`, `--receptive MODEL C`
  (an SMV or a JSON file), and `--vacuous C`.
- MCP server: the tools `is_vacuous` and `is_receptive`; `implements` and
  `accepts` take a state machine (`smv`, `machine`, `controller`). The skill
  says when to use each operation and check.

Faster:

- An implication check of one term against other terms uses only the terms
  that share variables with it, directly or through other terms, when all
  of them are satisfiable (checked once): the answer is the same.
  Composition with discharge and `remove_redundant` give the same results.
  RoboticJoints, composition with discharge of 8 joints: 140 s in 0.3.1,
  21 s (demos/RoboticJoints/results/discharge_speedup.csv).

## 0.3.1 (2026-10-27)

- The Python package, its documentation, the command line, and the tools of
  the MCP server are organized in three layers:
  1. contracts and their algebra (`chase_cps.contract`), which hold for any
     assume-guarantee contracts;
  2. checks: on contracts (`chase_cps.checks`: refines, verify,
     is_compatible, is_consistent, implements, accepts, remove_redundant),
     and the queries on formulas they reduce to (`chase_cps.logic`:
     implies, is_satisfiable, unsat_core);
  3. GR(1) synthesis with slugs (`chase_cps.gr1`: is_realizable,
     synthesize, unrealizable_core, is_well_separated, remove_vacuous).

  All the names are still in `chase_cps`, and the modules of 0.3.0
  (`chase_cps.checks`, `chase_cps.cores`) still give them: no code has to
  change. docs/usage.md has a section and a table for each layer.
- The documentation says what each query checks: `implies` the validity of
  an implication, `is_satisfiable` the satisfiability of a formula, which is
  not the satisfaction of a contract by a component (`implements`).
- `chase-cps check --help` groups the options: checks on contracts (nuXmv),
  GR(1) synthesis (slugs).
- Release: ccache keeps the C++ objects between the builds of the Python
  versions, and a tag builds the wheels once and uploads them to TestPyPI;
  once they are checked, a manual run publishes the same files to PyPI.

## 0.3.0 (2026-10-26)

- `context=`: formulas that every behavior satisfies (the axiom of an
  encoding of finite traces, a physical law), for the checks built on
  implication and satisfiability: `implies`, `is_satisfiable`,
  `is_compatible`, `is_consistent`, `verify`, `refines`,
  `remove_redundant`, `accepts`, `implements`, and `unsat_core`, also in
  the MCP server. The behaviors that violate the context are left out; as
  an assumption, the context would not help, since a behavior that
  violates an assumption satisfies the contract. See "Context" in
  docs/usage.md.
- `load_spec` and `parse_spec` warn when an integer is compared with a
  constant outside its range (e.g. `credits >= 90` with `credits` declared
  without a range, so 0 to 16): the comparison has the same value at every
  step.
- The operations no longer leak memory. Before, every operation in Python
  (and every contract read from a file) kept the objects of the core
  library for good, 2 to 16 KB per operation. Now a contract owns its
  declarations and formulas, and a formula its operands, and `chase_cps`
  frees the objects of the core library once it has read the result
  (`pychase.utilities.destroy`). logics_tool: a proposition defined by a
  relation (`proposition p is x > 3;`) gets its own copy of the relation
  at each use, and a contract its own copy of the declarations of the
  system it uses.
- Core library: `refinement` put the formulas of the two contracts in the
  result instead of copies of them; saturating a contract with assumptions
  and no guarantees crashed (logics_tool `saturate`); the simplification
  turned a `LargeOr` of two formulas into an `and`.
- DSL_tool: computing the precision of the timings read one value past the
  end of an array (found by AddressSanitizer); the result was not used, and
  the specifications it writes are the same.
- CI: a job builds CHASE with AddressSanitizer and runs the Python tests
  and the console scripts of the demos.

## 0.2.0 (2026-10-26)

- Implication and coimplication of contracts (the adjoints of conjunction
  and disjunction), the reciprocal, and the two Boolean actions that add an
  assumption or a guarantee (Incer, Benveniste, Sangiovanni-Vincentelli,
  arXiv 2309.08875). In the core library and the console of logics_tool
  (`imply`, `coimply`, `invert`), and in Python and the MCP server (`imply`,
  `coimply`, `invert`, `assume`, `ensure`).
- `compose(..., discharge=True)`: composition that keeps the terms of the
  contracts and drops the assumptions discharged by the other contract and
  the redundant terms (Incer, PhD thesis 2022, Algorithm 2). When the
  contracts feed each other, the assumptions are discharged in one
  direction only, the one that drops more of them.
- Cores: `unrealizable_core` (with slugs, QuickCore order) and `unsat_core`
  (of A & G, with nuXmv), from a contract or from formulas, and `core=True`
  on `is_realizable`, `is_compatible`, `is_consistent`, and `is_satisfiable`;
  `chase-cps check --core`.
- `is_well_separated`: whether the component can win by forcing the
  environment to violate its assumptions, with the strategy and a minimal
  set of assumptions; `chase-cps check --well-separated`.
- `remove_vacuous`: removes the terms whose removal keeps the GR(1)
  specification (inherent vacuity, Maoz and Shalom 2020), with the kind of
  each term and the terms that make it vacuous.
- The GR(1) back end reports a contract whose variables do not have the
  roles of slugs (an initial assumption that reads an output, a safety
  assumption that reads the next value of an output) before calling slugs,
  with the term.
- `load_spec` and `parse_spec`: the variables declared outside the contracts
  are given to the contracts that use them. Before, they were left out of
  the inputs and outputs of the contracts.
- A temporal operator applied to an operand of a relation or of arithmetic
  (`X x = 3`) is rejected with a message (write `x' = 3`).
- Real variables (`"real"`, `real variable x;` in the CHASE language) and
  numbers with a fraction (`20.5`), read exactly. Before, a real variable
  was read as a Boolean one and `20.5` as `20`; a real constant was read as
  an integer (`real constant K 2.5;` gave 2). The checks handle real
  variables in this order, and say which way answered (`reals`):
  regions, exact, when the real variables are compared with constants or
  with each other, without arithmetic (also for realizability and
  synthesis, with the controller written with the conditions on the real
  variables); the SMT engine of nuXmv; predicate abstraction, which
  returns only the answers that hold for the real variables, refined with
  z3 when it is installed (`pip install chase-cps[reals]`). See "Real
  variables" in docs/usage.md. Without real variables the checks are
  unchanged.
- The NuSMV printer of chase-backends writes real variables (with their
  range as INVAR) and real constants for nuXmv; the slugs and gr1c printers
  report real variables instead of writing them as Boolean ones.
- logics_tool: `refinement` (and the other operations) crashed when a
  contract used a variable declared outside the contracts. A contract now
  declares the declarations of the system it uses, and the NuSMV printer
  writes the constants (`DEFINE`), which it skipped.
- nuXmv no longer waits at its prompt after an error until the timeout: the
  error is reported at once.
- logics_tool: `verify` and `refinement` say when the file they write has
  real variables or real numbers, which only nuXmv reads (`go_msat`, then
  `check_ltlspec_ic3`); the file starts with a comment that says the same.
- The example of the README and of the page of the package on PyPI used
  `refinement`, which is `refines` since 0.1.0. A new test runs the Python
  examples of the README files and of docs/usage.md, and checks that the
  names in the table of the operations exist (Python, chase-cps check, and
  the console of logics_tool).
- A syntax error in a specification (`load_spec`, `parse_spec`, `chase-cps
  check`, the MCP tool `read_spec`) gives the line, the column, and the
  token in the message; before, the parser wrote them on the standard error
  only, and `parse_spec` named a temporary file.
- A variable type that is not `bool`, `real`, or (low, high) with integer
  bounds is reported with the type (`'integer'` was shown letter by letter,
  and `[1.5, 3]` was read as `(1, 3)`).
- New demos: Charger (unrealizable core, synthesis, well-separation,
  vacuity, and composition with discharge of an arbiter and two robots)
  and LiftPlusCruise (requirements of the FRET case study with real
  variables: consistency, a behavior, and the requirements in conflict).
  The demo suite runs both.

Known issue: the operations of the core library (composition, conjunction,
quotient, ...) do not free the memory they use, about 10 to 15 KB per
operation (about 9 KB in 0.1.2). A later version will.

## 0.1.2 (2026-10-25)

- The slugs back end wrote the constants `true` and `false` as nothing, so
  `G F false` became an empty liveness condition and any term with a
  constant was written wrongly. It now writes them, and a formula node it
  cannot write is reported instead of skipped.
  The gr1c back end of logics_tool had the same problem with the constants.
- A new test writes contracts for slugs, compiles them with the slugs
  compiler, reads them back, and checks with nuXmv that the assumptions and
  guarantees read back are equivalent to those of the contract, for fixed
  cases and for random contracts (`CHASE_SLUGS_FUZZ`).
- The comparison with Pacti is no longer in the documentation.

## 0.1.1 (2026-10-25)

- The slugs back end dropped the temporal operators F and G nested in a
  term, so `G (req -> F grant)` was checked as `G (req -> grant)` and
  `is_realizable` could answer unrealizable for a realizable contract (or
  the converse for assumptions). Terms outside GR(1) are now reported:
  `is_realizable` and `synthesize` return unknown with the term.

## 0.1.0 (2026-10-25)

First release of the CHASE Python package, `chase-cps`, and of this
repository as the entry point to CHASE.

### Python package

- Contracts written in the standard LTL syntax (`Contract.from_strings`,
  `Contract.from_dict`), or read from and written to the logics language of
  CHASE (`load_spec`, `parse_spec`, `to_spec`).
- The contract algebra of the core library: composition, conjunction,
  disjunction, merging, quotient, separation, and saturation.
- Checks with nuXmv (or NuSMV, where nuXmv cannot be used): implication,
  satisfiability, refinement, consistency, compatibility, and containment of
  an environment or an implementation. A failed check returns a counterexample trace.
  `remove_redundant` drops the terms implied by the others, and
  `Contract.simplify` rewrites the formulas as the simplify command of
  logics_tool.
- GR(1) realizability (`is_realizable`) and synthesis (`synthesize`) with
  slugs, with the controller or the counter-strategy of the environment.
- The operations are named with verbs, the same in Python, on the command
  line, in the MCP server, and in the console of logics_tool: `compose`,
  `conjoin`, `disjoin`, `merge`, `divide` (quotient), `separate`,
  `saturate`, `simplify`, `refines`, `verify`, and `synthesize`.
- Command line: `chase-cps install` installs nuXmv, slugs, and NuSMV in
  `~/.chase`, `chase-cps doctor` shows the tools found, and `chase-cps check`
  checks the contracts of a specification and prints the counterexamples.
- Wheels for Python 3.10 to 3.15 on Linux (x86_64, aarch64) and macOS (arm64,
  x86_64).

### Core library and tools

- Separation of contracts, and Python bindings for disjunction and merging.
- The console of logics_tool has the commands `disjoin`, `merge`, and
  `separate`; `conjunction`, `quotient`, and `refinement` are now `conjoin`,
  `divide`, and `refines`, and the old names are still accepted. `show`
  prints the temporal operators (it printed their codes).
- The quotient follows again the closed form of Incer et al. (MEMOCODE 2018):
  the guarantees are (G1 & A2) | !(A1 & G2).
- Primed variables kept their prime when a formula was cloned, and the NuSMV
  back end writes them as `next(x)`. Before, `x' = x + 1` became `x = x + 1`,
  which made the refinement checks of the RoboticJoints demo hold vacuously.
- Errors in a specification raise an exception instead of ending the process.
- Missing return in the ANTLR runtime, which crashed the Release builds.
- C++17, CMake 3.18, Python found with FindPython, and relative run paths in
  all the modules.

### Repository

- The modules are collected as git submodules, built at once by `install.sh`
  or CMake, and shipped in a Docker image.
- Continuous integration on Linux and macOS, with the demos and the Python
  tests.
