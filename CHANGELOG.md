# Changelog

## Unreleased

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
