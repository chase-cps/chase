# Operations of CHASE and Pacti

This document lists the operations of [Pacti][PACTI] (version 0.3.1,
`PolyhedralIoContract` and `IoContractCompound`) next to the corresponding
operations of CHASE: the C++ core library (`Contract::...`) and the Python
package (`chase_cps`).

Pacti represents assumptions and guarantees as conjunctions of linear
inequalities (polyhedra). CHASE represents them as LTL formulas over Boolean
and bounded integer variables. The operations that depend on linear
arithmetic (optimization, variable bounds) have no counterpart in CHASE.

| Pacti | CHASE core library | chase_cps | Notes |
|---|---|---|---|
| `compose` | `Contract::composition` | `Contract.compose` | Pacti also eliminates the internal variables (roadmap O9). |
| `quotient` | `Contract::quotient` | `Contract.quotient` | |
| `merge` | `Contract::merging` | `Contract.merge` | |
| `IoContractCompound.merge` | `Contract::conjunction` | `Contract.conjoin` | |
| (none) | `Contract::disjunction` | | |
| (none) | `Contract::separation` | `Contract.separation` | Adjoint of merging (roadmap O8). |
| (none) | `Contract::saturate` | `Contract.saturate` | (A, A -> G); the algebra saturates its operands. |
| `refines` | `Contract::refinementCheck` (writes a NuSMV model) | `refines` | chase_cps returns a counterexample when the check fails. |
| `simplify` | `simplify` (Boolean formulas only) | `simplify` | Each term is dropped when the context implies it (roadmap O10). |
| `contains_environment` | (none) | `contains_environment` | E -> A. |
| `contains_implementation` | (none) | `contains_implementation` | M & A -> G. |
| `can_compose_with` | name clashes checked by `mergeDeclarations` | `Contract.can_compose_with` | No variable is an output of both contracts. |
| `can_quotient_by` | (none) | (none) | |
| `shares_io_with` | (none) | `Contract.shares_io_with` | |
| `rename_variable(s)` | correspondence maps of the operations | `Contract.rename` | |
| `from_strings`, `from_dict`, `to_dict` | logics language, DSL | `Contract.from_strings`, `from_dict`, `to_dict` | Roadmap E10. |
| `optimize`, `get_variable_bounds` | (none) | (none) | Linear arithmetic only. |
| (none) | (none) | `consistent`, `compatible` | Satisfiability of A -> G and of A, with a witness trace. |
| (none) | GR1C, Slugs back ends | `realizable` | GR(1) realizability, controller, counter-strategy. |

The operations still missing in CHASE are the elimination of internal
variables after composition (roadmap O9) and `can_quotient_by`.

[PACTI]: https://github.com/pacti-org/pacti
