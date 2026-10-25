# Changelog

## 0.1.0

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
- GR(1) realizability with slugs, with the controller or the counter-strategy
  of the environment.
- Command line: `chase-cps install` installs nuXmv, slugs, and NuSMV in
  `~/.chase`, `chase-cps doctor` shows the tools found, and `chase-cps check`
  checks the contracts of a specification and prints the counterexamples.
- Wheels for Python 3.10 to 3.15 on Linux (x86_64, aarch64) and macOS (arm64,
  x86_64).

### Core library and tools

- Separation of contracts, and Python bindings for disjunction and merging.
- The console of logics_tool has the commands disjunction and merging, and
  `show` prints the temporal operators (it printed their codes).
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
