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

Contracts can be composed (`compose`), conjoined, disjoined, merged, and
divided (`quotient`, `separation`). The checks are `implies`, `satisfiable`,
`refines`, `consistent`, `compatible`, `realizable`, `contains_environment`,
`contains_implementation`, and `remove_redundant`. `load_spec` and
`parse_spec` read the contracts of a specification written in the logics
language of CHASE, and `to_spec` writes one.

The checks need external tools in the `PATH`:

- [nuXmv][NUXMV] or [NuSMV][NUSMV] for implication, satisfiability, and
  refinement (nuXmv is preferred: it uses IC3);
- [slugs][SLUGS] for GR(1) realizability and synthesis.

The environment variable `CHASE_BACKEND` (`nuxmv`, `nusmv`) selects the model
checker when both are installed.

The source code and the installation instructions are in the
[chase][REPO] repository.

[WEB]: https://chase-cps.github.io
[REPO]: https://github.com/chase-cps/chase
[NUXMV]: https://nuxmv.fbk.eu/
[NUSMV]: https://nusmv.fbk.eu/
[SLUGS]: https://github.com/VerifiableRobotics/slugs
