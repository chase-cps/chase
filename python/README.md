# chase-cps

Python interface to [CHASE][WEB] (Contract-based Heterogeneous Analysis and
Systems Exploration), a framework for requirement capture, formalization, and
validation of cyber-physical systems with assume-guarantee contracts.

The package contains the CHASE core library (contract representation and
algebra) and a common interface to model checkers and synthesis tools.

```python
from chase_cps import Contract, refines

spec = Contract.from_strings(inputs=["req"], outputs=["grant"],
                             assumptions=["G F !req"],
                             guarantees=["G (req -> F grant)"], name="Spec")
impl = Contract.from_strings(inputs=["req"], outputs=["grant"],
                             guarantees=["G (req -> X grant)"], name="Impl")

result = refines(impl, spec)
print(result.status)          # valid, or invalid with a counterexample trace
```

Contracts can be saturated, composed (`compose`), conjoined, disjoined,
merged, and divided (`quotient`, `separation`), and simplified (`simplify`).
The checks on contracts are `refines`, `consistent`, `compatible`,
`realizable`, `contains_environment`, and `contains_implementation`; the
checks on formulas are `implies` and `satisfiable`. `load_spec` and
`parse_spec` read the contracts of a specification written in the logics
language of CHASE, and `to_spec` writes one.

## Installation

`$> pip install chase-cps`

Wheels are available for Python 3.10 to 3.15 on Linux (x86_64 and aarch64,
glibc 2.28 or later) and macOS 11 or later (arm64 and x86_64). Windows is not
supported: use WSL or the CHASE Docker image.

## Requirements

The package contains the CHASE library. The checks call external tools,
which are not installed with the package:

- [nuXmv][NUXMV] (free for non-commercial and academic use) or
  [NuSMV][NUSMV] for `refines`, `consistent`, `compatible`, `implies`,
  `satisfiable`, `simplify`, `contains_environment`, and
  `contains_implementation`. nuXmv is preferred: it proves the properties
  with IC3.
- [slugs][SLUGS] for `realizable`.

The command `chase-cps install` downloads them from their sites (or builds
slugs from its sources) into `~/.chase`:

````
$> chase-cps install nuxmv --accept-license    # Linux x86_64, macOS arm64
$> chase-cps install nusmv                     # Linux x86_64
$> chase-cps install slugs                     # needs a C++ compiler, make, and Boost
````

On macOS, nuXmv needs the libraries of Homebrew (`brew install libxml2 gmp
libedit`); on Debian and Ubuntu, `sudo apt-get install libxml2 libedit2`.
The tools can also be installed by hand: they are looked up in the `PATH`
and in `~/.chase/bin`, or given with the environment variables
`CHASE_NUXMV`, `CHASE_NUSMV`, and `CHASE_SLUGS`. The command

`$> chase-cps doctor`

shows the tools found and how to install the missing ones. When both nuXmv
and NuSMV are installed, the environment variable `CHASE_BACKEND` (`nuxmv`,
`nusmv`) selects one.

## Command line

`chase-cps check` checks the contracts of a specification written in the
logics language of CHASE, or of a JSON file, and prints the counterexamples:

````
$> chase-cps check specs.ltl                                  # compatibility and consistency
$> chase-cps check specs.ltl --refines Joint1+Joint2 Joints   # composition refines Joints
$> chase-cps check specs.ltl --realizable Joint1 --json
````

The exit status is 0 when every check passes, and 1 when a check fails.

The source code and the installation instructions are in the
[chase][REPO] repository.

[WEB]: https://chase-cps.github.io
[REPO]: https://github.com/chase-cps/chase
[NUXMV]: https://nuxmv.fbk.eu/
[NUSMV]: https://nusmv.fbk.eu/
[SLUGS]: https://github.com/VerifiableRobotics/slugs
