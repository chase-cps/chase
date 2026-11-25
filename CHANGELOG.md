# Changelog

## Unreleased

Added:

- `c1.compose(c2, eliminate=True)`: the composition of GR(1) contracts
  with the connecting variables eliminated, so that the result has only
  the inputs and outputs of the system (Incer et al., Pacti, ACM TCPS 2025).
  `chase_cps.cave` computes the eliminations, by refinement for the
  assumptions and by relaxation for the guarantees, term by term over a
  finite window of steps with z3 (Narter, Zhao, Incer, arXiv 2610.37534,
  Algorithms 1 and 2). The result is GR(1) and sound, not always exact.
  Its guarantees are simplified where its assumptions hold
  (`chase_cps.cave.simplify`), which does not change the contract.

## 0.7.2 (2026-11-02)

Long names in the diagrams, and the PDF of a large diagram.

Changed:

- Long names keep the boxes narrow: a title too long keeps the end of its
  path (the part that tells the parts apart: `…downstream.baf.bapg_rd`)
  and the start of the words after it; a note is broken at its ⊗, · and /
  into three lines at most; a long port name keeps its end. The whole of
  each name is in its tip.
- A check that holds says in its panel what it has shown (for example,
  for `M ⊨ C`: in an environment that meets the assumptions, every run of
  the model meets the guarantees), not "Holds." alone.
- The wheels are built with less work repeated: the compiled C++ is kept
  between releases, and the tests run in full on Python 3.12 and 3.15 (a
  call of the library on the other versions).

Fixed:

- The PDF of a large diagram had only its top left corner when the page
  was inside a frame, or in a browser that keeps its own paper size: the
  diagram is printed from a page of its own, the size of the diagram, and
  fitted to the page.

## 0.7.1 (2026-11-02)

The diagrams after a review of every demo, and ports of two integer ranges.

Added:

- A variable shared as integers of two ranges (a port of 0..100 feeding one
  of 0..9) is one integer of the union of the ranges. In each contract its
  own range becomes a term: an assumption where it is an input, a guarantee
  where it is an output. The checks then say whether the values fit (a
  counterexample, the term of the range it breaks), and the wire is not
  red. Other types that differ are still an error.

Changed:

- The panel and the waveform float over the diagram, of frosted glass: the
  diagram goes on under them.
- One corner for every box: tags, contracts, the toolbar, the panel, the
  waveform.
- The hole of a quotient is pale yellow, so that it shows when the diagram
  is small.
- A C function with no clause has the contract (⊤, ⊤): its tag is `M ⊨ C`,
  not `safe`. The check is the same (no overflow, division by zero, or
  access out of bounds).
- The values of a step are written on the ports over the wires, and the
  same value at the two ends of a short wire once.
- The counterexample table scrolls by whole columns, its end too; long
  lines of code wrap in the panel.
- A quotient by a composition names its parts (`Pipeline / (Producer ⊗
  Consumer)`); a range of one value is written `{0}`.
- Export: the SVG has the style of each element written on it, so other
  programs (Inkscape, Illustrator, slides) draw it as the page does, with
  its symbols; it has no background. The PNG and the PDF are on white, and
  the PDF is one page the size of the diagram.

Fixed:

- The PDF of a diagram was blank (the rules of the page for printing hid
  it).
- The wires from environments to what they drive could be drawn far above
  the boxes (Deadline): the wires of the top level are placed with their
  row.

## 0.7.0 (2026-11-02)

The waveform of a counterexample, and the panel of a diagram after a try
by three users (a CPS engineer, a hardware verification engineer, a
formal methods researcher).

Added:

- The waveform: the Waveform button of the panel opens the counterexample
  in a drawer under the diagram, as a waveform viewer shows a simulation.
  A row for each variable (those of the requirement first), a column for
  each step; Booleans high and low, numbers as boxes with their values, or
  as a line (Enter). The step that fails is red, the loop is marked ↺ and
  drawn once more, faded. The cursor is the step on the diagram and in the
  table. Zoom (+, −, 0, Ctrl or ⌘ with the wheel), pan (drag, Shift with
  the wheel), the arrows, Alt with an arrow for the next change of a row,
  Home, End, F (the step that fails), L (the loop), Esc; the top edge sets
  its height. In a narrow window it takes the place of the panel.
- A system whose parts do not agree on a signal (driven by two parts, or of
  two types) draws the wires of that signal red, with what is wrong in
  their tip, and warns; its refinement is said not checked, with the
  reason.

Changed:

- Booleans of a counterexample are written ⊤ and ⊥, as in the terms.
- The counterexample is marked at the step where its term is seen to fail:
  `G (p -> X X q)` at the step of `X X q`, `F[0,6] p` at 6, an environment
  at the step where the assumption breaks. A term broken by the whole run
  (`F p` that never holds) marks no step, and the panel says so.
- A number is never shown rounder than it is (17.996, not 18); the table
  writes short values, with the whole value in its tip.
- The panel: the counterexample at the same place in every problem (titles
  of two lines, headings of one height, the table as wide as the panel, the
  sentences under it); rows of the problems of one height; a check found by
  a core lists its terms (a click shows one on the diagram), with the end
  that sets apart terms that begin alike; "How to fix" names the constants
  that can change, and is worded for each kind of check.
- A problem opened in a large diagram brings its box into view at a size
  that can be read.
- The values of a step stay on the ports inside a box, and read over wires.

Fixed:

- A contract named in Python with a dot or a space (`Contract("u_core.alu",
  ...)`) works in the algebra: it is written under a name the language
  takes. A variable with such a name is said before the parser fails.
- A System whose quotient cannot be computed draws the hole with its ports.

## 0.6.5 (2026-11-02)

The viewer, after a try of the editor extension.

Changed:

- The panel goes from the list of the checks that fail to the details of
  one, and back (← Problems, or Esc); one check that fails opens its
  details at once. The code of a C function is folded in its details.
- A check that fails is said in plain words, with the names of the parts,
  models, and contracts in bold ("Room and Thermostat do not refine
  Comfort", "The C function thermostat does not implement Thermostat",
  "The terms of X conflict"), and a second line says where its
  counterexample fails and with which values, the terms in conflict, or
  the constants that can be repaired.
- The table of a counterexample picks the step shown on the diagram (a
  click on a column, or the arrow keys); the step that fails first is a red
  column, another step picked a blue one. The cells are centered and the
  steps of one width. A table wider than the panel keeps the names of the
  variables in place, with a scroll bar always drawn, the steps in view,
  and a page back and on.
- Each repair says which constants it changes; a search with no answer
  says why and can be tried again with twice the time; one with no value
  says to edit the range.
- The buttons of the panel are icons of one style, with their tips.

Fixed:

- A System with a contract whose components leave signals that no one
  drives: the part not written is drawn as the quotient of the contract by
  the components, as `from_spec` does.

## 0.6.4 (2026-11-01)

Added:

- Repair of constants inside invariants (`G` of one step, `F` of one step,
  conditions on the first step): `method="copies"` writes the questions on
  copies of the state, with the same answers and no temporal operator, and
  finds the least cost with z3; chosen by `auto` for these terms (two real
  constants inside `G`, which the counterexamples of nuXmv did not end on,
  now take a fraction of a second, with the least cost shown). The copies
  are the small-model property of the formulas of temporal height one
  (Demri and Schnoebelen, 2002). It reads the context; an integer variable
  beside a real constant, whose counterexamples remove one value each,
  goes by the projection of the copies; when it has no answer (an
  optimization that z3 does not end), `auto` goes on with the other
  methods and says so.

Fixed:

- Repair of a state machine against a contract with constants: the
  constants are declared in the model (FROZENVAR); the questions failed
  with "the model does not declare" them.
- Repair of a state machine with a reset within a context: the context is
  read after the steps of the reset, as the check reads it.
- Repair: a term that z3 does not read (`mod`) makes the repair unknown
  with the reason, not an error; a final check with no answer says so,
  not that the check fails.
- Viewer: with no named constant, "How to fix" says how to declare one,
  instead of reading as if the file had one.

## 0.6.3 (2026-11-01)

0.6.2 was tagged and tested on TestPyPI, not published: its demos found
two more bugs.

Fixed:

- Continuous time: a term in parentheses under an interval keeps them;
  `G[0,9] ((a | b) & (c | d))` was written without them, and the check
  read it as a conjunction. A named constant needs no Lipschitz constant.
- Repair: a cost with a factor that is 0 (`3 * (1 - wmax)` at `wmax = 1`)
  no longer raises a division by 0.

## 0.6.2 (2026-11-01, not published)

Fixed:

- Traces of nuXmv: the state that closes a loop is dropped also when the
  model has inputs, and the loop starts where the trace breaks the
  specification (nuXmv marks every state equal to the last one as a start
  of the loop; from the last mark, a counterexample of a model with inputs
  could hold the specification).
- RTL with a reset: the reset is driven by the model, so a contract that
  names it (an SVA checker with `disable iff`) is checked for receptiveness
  without it.
- `remove_redundant`: terms in conflict (A & G unsatisfiable) are all kept,
  and the reason says so; a merge with discharge of viewpoints in conflict
  no longer drops terms.
- slugs: a term with `-`, `*`, `/`, or `mod` is unknown with the reason and
  how to write it.

Added:

- `is_implementation`: a failed check has in `details.violated` the
  guarantees its counterexample breaks, as a refinement has.
- Viewer: a counterexample opens at the step where a term fails first; a
  refinement that fails on the assumptions marks the assumptions of the
  parts that its trace breaks; a quotient names the candidate in its hole;
  a small diagram fits the window larger; the boxes inside a box are in the
  order that sends the fewest wires back, and opening a box does not move
  the others.

## 0.6.1 (2026-10-31)

0.6.0 was tagged but not published: the tests of its wheels, run without a
back end, had a test that needs one.

Added:

- SMV models with their specifications read enumerated types (as the
  integers 0, 1, ... in the order written, in the contract and in the
  model) and the DEFINEs that the specifications use (replaced by their
  definitions).

Changed:

- Tests: a refinement of the constants of a composition needs a back end
  (skipped without one); the robustness computed by z3 may be unknown when
  z3 does not reach the best value (at most 5% of the random cases).

## 0.6.0 (2026-10-31, not published)

- A refinement in continuous time (`--lipschitz`) that the stricter check
  does not show is unknown, not invalid. A decomposition drawn in its
  contract keeps the inputs of the contract that no part drives; an input
  of a part that the contract does not give is drawn open.

Added:

- Continuous time on the command line: `chase-cps check FILE --period SECONDS
  --lipschitz VAR=RATE --refines R S` checks the refinement between the
  samples too; with a period, the intervals may be written with times
  (`G[0s,2s]`).
- Sampled signals (`chase_cps.sampled`): `sampled` gives the stricter
  formula of Fainekos and Pappas (Corollary 44) from the Lipschitz
  constants and the sampling period; holding at the samples, it shows the
  formula in continuous time (`continuous` on a trace, or any check over
  every behavior). `chase-cps view --period` draws the steps of a
  counterexample as times.
- SMV models with their specifications (`chase_cps.smvspec`): an SMV file
  with `LTLSPEC` or `INVARSPEC` is a component, its specifications the
  guarantees of its contract, checked with nuXmv; `chase-cps view
  FILE.smv` and VS Code draw it. A counterexample shows booleans as T and
  F in its table.
- Contracts written in C, in the clauses of ESBMC (`__ESBMC_requires`,
  `__ESBMC_ensures`) or of CBMC (`__CPROVER_requires`,
  `__CPROVER_ensures`), or in ACSL (Frama-C, `/*@ requires ...; ensures
  ...; */`): `load_c(path)` reads each function as a contract
  with the function as its implementation, and `chase-cps view FILE.c`
  (and VS Code) draws and checks them. A function with no clause is
  checked as code alone (the tag `safe`).
- A C function called once per step implements a contract over steps
  (`is_implementation(CFunction(...), c)` with `G`, `X`, `G[a,b]`): checked
  on one call against the formula of one step; unknown for a function that
  keeps a state, or a contract of another form. A diagram draws it inside
  the decomposition it is a part of, with its code.
- Repair: a time limit for the whole repair (`time_limit`, `--time-limit`),
  stopped there with the least cost proved so far (`bound`).
- Repair (`chase_cps.repair`, `chase-cps repair`): the values of named
  constants of the contracts that make a check pass, at the least cost (by
  default the least change), each checked by the check itself. Bounded
  formulas over the reals are repaired by projection (quantifier
  elimination of z3, one optimization); the other checks of every behavior
  by counterexamples (nuXmv, ESBMC, z3), each counterexample becoming a
  condition on the parameters, generalized over the reals on bounded
  formulas; checks of some behavior by witnesses or by enumeration. The
  result is holds (the check passes with the present values: a
  verification), repaired (optimal when no values of lower cost pass),
  impossible, or unknown.
- Named constants of contracts: `constants=` in Python and JSON, `constant`
  in specification files (kept by name, no longer only by value),
  `instantiate`, `terms`; a composition keeps them.
- Diagrams (`chase_cps.viz`, `chase-cps view`): the named constants of a
  contract are shown by name in its terms, with their values on a line of
  their own (K); the steps of an interval (`G[0,9] p`, unfolded into `X`)
  are shown folded again; a C function is drawn as the implementation of a
  contract (`--implementation heater.c:thermostat C`, also in the `# view:`
  lines of a file).
- Repair in the diagram: a click on the result of a check (⪯, M ⊨ C,
  E ⊨ A, a red A∧G) opens its panel, with the named constants of its
  contracts, the repairs found, a repair drawn on the diagram when pointed
  at, and Apply, which writes the value in the file (VS Code, and
  `chase-cps view --serve`: POST /repair and /apply). A click on ⪯ no longer
  folds the box: the button at its top right does. With two or more checks
  that fail, the tools list them.
- `python/benchmarks/repair_scaling.py`: how the composition, the checks,
  the worst case, and the repair grow with the size of a family of timing
  contracts (as ContrACT builds them).
- Robustness (`chase_cps.robustness`): how much a trace satisfies or
  violates a formula, by the quantitative semantics of signal temporal
  logic in discrete time, on lasso traces. `Contract.robustness(trace)`
  gives it for the assumptions, the guarantees, and the contract. A
  refinement that fails names the terms its counterexample violates, with
  how much for the terms that compare numbers (`violated`).
- The Python syntax (and so the command line and the MCP server) takes the
  intervals of steps of the specification files: `G[0,3] p`, `F]0,2] p`,
  `p U[1,2] q`, `p R[0,2] q`, unfolded into `X`. With robustness, this is
  signal temporal logic in discrete time.
- Robustness reads `past(x, n)`: the history variable of nuXmv in the
  trace, or the value n steps before.
- z3 back end (`pip install chase-cps[z3]`): the checks of formulas over a
  bounded number of steps (no `G`, `F`, `U`, `R`; `X`, `x'`, `past(x, n)`,
  and the intervals of steps are fine), static contracts among them, go to
  z3 when it is installed, exact for linear arithmetic over the integers
  and the reals with any Boolean structure. `minimize` and `maximize` give
  the best value of a term under such constraints (a formula, or a
  contract), with the values that reach it; with a formula as the
  objective, its robustness: the worst case of a property of signal
  temporal logic under some dynamics. The bound that z3 gives is checked
  (whether a better value exists, and whether it is reached); they need z3
  4.12 or later. `chase-cps doctor` lists z3. The Docker image installs z3
  with pip (the package of Ubuntu is z3 4.8).
- `to_smv` and `to_smt` write a check as the input of nuXmv or NuSMV, and
  of SMT solvers (z3, cvc5), for the tools one runs oneself: the check,
  then its arguments, as in `to_smv("refines", c1, c2)` or
  `to_smv("impl", model, c)`; one named question per question of the
  check, with how to read the answer. `chase-cps export FILE --to LANG`
  takes the checks of `chase-cps check` (`--refines C1 C2`, `--impl MODEL
  C`, ...), and also writes `gr1` (`--realizable`), `sva`, `spec`, and
  `json`.
- `chase-cps check` has short names for its checks: `--compat`,
  `--consist`, `--impl`, `--env`, `--recep` (the long names still work);
  `--verify`, `--compat`, and `--consist` take the contracts to check.
- C functions as implementations of contracts: `is_implementation(
  CFunction("saturate.c", "saturate"), contract)` checks with ESBMC, or
  CBMC, that every call allowed by the assumptions satisfies the
  guarantees, and that the function has no overflow, division by zero, or
  access out of bounds; a failure gives the values of the call.
  `to_c("impl", f, c)` writes the program they check; on the command line,
  `--impl FILE.c:FUNCTION CONTRACT`. `chase-cps check` prints the
  counterexample of a failed implementation. `chase-cps install esbmc`
  installs ESBMC on Linux, and the Docker image has it.

Changed:

- `ltl.to_str` prints a negative number as `-3` (it was `(0 - 3)`, the
  spelling of the logics language, which `ltl.to_chase` keeps).
- Folded compositions grow linearly with the number of contracts; they
  doubled at each step. A composition is saturated already (A -> G is G
  when A is X | !G), and its assumption X | !G is composed as X (the rest
  is implied). Both are equivalences, tested against the closed form.
- Repair by counterexamples generalizes each counterexample by a
  model-based projection (polynomial on linear arithmetic), not by
  quantifier elimination, which could run for minutes and did not stop at
  its timeout. The projection method is chosen by default only for
  questions with at most 12 variables to eliminate.
- Diagrams: the panel of a check says first why it fails: which side, and
  the counterexample (one column per step, the loop marked; the variables
  of the requirement first), the reason given by ESBMC for C code; then how
  to fix it: the values of the constants, or, when the check reads none,
  that no value can fix it. The icon of a C function opens its code (and
  the functions of its file it calls), with the lines of the file.
- Diagrams: intervals of steps are folded also inside a formula
  (F[0,10] G[0,5] p); the parts of a decomposition that have a model (a C
  function, RTL) are drawn as components inside the contract they refine.
- A value with no finite decimal expansion (a repair can find 17/21) is
  printed as a quotient, (17 / 21), and read back as one.
- `chase-cps doctor` lists the back ends by domain (contracts and LTL,
  GR(1), static contracts and signal temporal logic, RTL, C programs), with
  the optional ones marked and how to install them. `chase-cps[reals]` is
  now `chase-cps[z3]` (the former name still works).
- `to_gr1(c, tool="gr1c")`: the argument `format` is now `tool`, as in
  the other functions that write files for tools.
- The logics language (specification files, `.ltl`) has the precedence of
  LTL tools and of the Python syntax: from the tightest, `! G F X`, then
  `U R`, `&`, `|`, `->`, `<->`; `U`, `R`, `->` group to the right. Before,
  all the binary operators had the same precedence and `G`, `F`, `X` were
  looser than them, so `G a -> b` was `G (a -> b)`; it is now
  `(G a) -> b`. In arithmetic, `*` and `/` now come before `+` and `-`.
  Every specification file of the demos and the tests reads the same as
  before (they have parentheses).
- Formulas are printed with the parentheses a reader needs, whatever the
  precedence: two different logical operators (`a -> (b | c)`,
  `(a & b) U c`), and a comparison or a binary formula under a unary
  operator (`G (x <= 4)`, `X (x' = y)`). The diagrams, the results, and the
  messages show them so.

Fixed:

- `a = b` and `a != b` between Booleans (`G (w != c)`, `G (w -> (c' = c))`)
  made realizability and synthesis unknown, with the error of the parser
  of slugs as the reason: structured slugs compares only numbers. They are
  now written `a <-> b` and `!(a <-> b)`.
- A diagram of a contract whose A & G is unsatisfiable drew the tags A and
  G twice after `verify`: the checks that `verify` runs to tell which part
  fails were kept on the contract too. Only the check called is kept now.

## 0.5.0 (2026-10-30)

Added:

- Diagrams (`chase_cps.viz`, `chase-cps view`): contracts, components and
  systems drawn as a web page. Refinement is nesting, a quotient is a hole
  with what refines it inside, a component opens on the states of its
  model (RTL through Yosys, JSON, SMV, or a controller), an environment is
  drawn on the left of what it drives, and the results of the checks are
  tags on the boxes, with their cores. Each result of the algebra
  remembers its operands (`Contract.origin`). `Component` and `System`
  bind contracts to implementations; `System.check` checks each
  implementation and the composition against the contract of the system. The
  page works offline with a local ELK (`elk=`, `--elk`); `viz.from_spec`
  draws a specification file, as `chase-cps view` does.
- `load_dsl(path)` reads a problem written in the domain-specific language
  of CHASE (the electric power system patterns of DATE18 and FDL2024) into
  a contract, named after the file (or `name=`). The graph of the
  architecture is not part of the contract and is skipped.
- `to_gr1(c, format="slugs" | "gr1c")` writes a GR(1) contract as the input
  of slugs (its structured format) or of gr1c, for users who run a GR(1)
  solver themselves; for gr1c, relations with arithmetic or between
  integers are written as the values that satisfy them.
- `chase-cps view` reads SystemVerilog Assertions (`.sv`), writes the data
  of the diagram (`--format json`), and draws the structure only
  (`--no-checks`); with `--serve`, the page takes each new drawing in place
  and keeps its view, and keeps the last diagram when the file cannot be
  drawn. The explanations of the page show in the webview of an editor too.
- `chase-cps view --check` checks that the assumptions and the guarantees
  of each contract can hold together (A∧G, with the core in red when they
  cannot), and the refinements, implementations, and environments drawn.
  The results are kept next to the file (`FILE.results.json`, readable
  JSON), found by a hash of what each check reads: a check of contracts that
  have not changed is not run again, and a diagram drawn without checks
  shows the results kept. `--budget` limits the time of the checks.
- A part not written yet (the contract of a system with some outputs that
  none of its parts drives) is drawn as a hole: the quotient of the
  contract by the parts.
- `chase-cps view` puts the contracts of a file together by their ports,
  as Lingua Franca does: an output and an input of the same name are wired,
  contracts with the same ports are versions of one part (the first one is
  wired), and a contract whose outputs several parts drive has them drawn
  inside it, its refinement checked with `--check` (and on save in an
  editor). The MCP tool `visualize` checks these refinements by default.
- `chase-cps view --implementation MODEL C` draws C as a component with
  its model (SMV, JSON, or RTL read with Yosys), checked with `--check`; a
  Verilog or SystemVerilog file with no assertions is drawn as its module.
  The options of `view` can be written at the top of the file
  (`# view: ...`, `// view: ...` in Verilog). The page saves the diagram as
  SVG, PNG, or PDF, and in an editor a double click on a box, or on a term
  (of a core, for instance), goes to its line in the file.
- RTL read with Yosys finds the files named by `` `include `` next to the
  sources, not only `.svh` and `.vh` headers.
- `chase-cps view` of a Verilog module that uses modules of other files
  takes the files next to it that have them.
- The checks keep their results on the contracts they check, with what the
  contract said then (`Contract.checked()`): a diagram draws them with no
  `checks=`, and drops them when the contract changes.
- A name that is not declared in a specification file is reported with
  its contract and its line.
- `viz.diagram(..., open=[...])` and `chase-cps view --open` draw boxes open
  when the page loads: the terms of a contract, the states of a small
  model, and the core of a check that fails.

Changed:

- Demos: AIAgents is now PaymentAssistant; DATE18 and DATE21 use slugs
  instead of gr1c; `DATE18/eps.py` checks the power system in Python; the
  READMEs show each step with its diagram, as the website does.
- `ensure` of a contract without assumptions keeps its terms: (true, G and
  the new guarantee), the same contract as before, instead of the saturated
  formulas of the composition.

Fixed:

- A copy of a temporal formula lost its interval, so an operation of the
  console (compose, for instance) turned `G[0,2] p` into `G p`.
- The console commands `stl` and `synthesize` crashed without a contract,
  or with the name of no contract.
- `synthesize C file gr1c` wrote a broken file when a relation compares
  two variables or has arithmetic, which gr1c cannot read; it now writes
  nothing and says to use `to_gr1(c, format="gr1c")`.
- `R` (release) in a specification file crashed the parser of logics_tool
  (exit code 134 or 138): it built only `U`.
- The interval of a temporal operator in a specification file
  (`G[0,3] p`, `x U[1,2] y`) was dropped, so `G[0,3] p` was read as
  `G p`. The Python package now unfolds it into `X`, and refuses an
  interval that is not a range of whole steps, and `X[a,b]`.
- A probability, `P(...)`, now gives the name of the contract and says
  that probabilities are not checked, instead of a traceback.
- A negative constant (`w >= -0.8`, `n > -3`) made the operations computed
  by the C++ library (composition without discharge, for instance) fail
  with "Invalid number format": the logics language reads `-0.8` as one
  token and rejects it. The formulas are now written with `(0 - 0.8)`.

## 0.4.2 (2026-10-28)

Changed:

- `is_implementation(m, c)` and `is_environment(e, c)` are the new names of
  `implements(m, c)` and `accepts(c, e)`, after the terms of the theory
  (M is an implementation of C, E is an environment of C); the environment
  now comes first, as the implementation. The old names still work, with a
  DeprecationWarning. Command line: `--implementation MODEL C` (was
  `--implements`, still read) and the new `--environment MODEL C`. MCP
  server: the tools are named `is_implementation` and `is_environment`.

New:

- `conjoin(..., discharge=True)`: the result keeps the terms and is the
  same contract as the conjunction. Each guarantee is guaranteed under the
  assumptions of its contract that the other's do not imply; when the
  assumptions of one contract imply those of the other, the assumption is
  theirs and has no disjunction (with the same assumptions, (A, G1 & G2)).
- Docker image published on ghcr.io/chase-cps/chase at each release
  (`docker pull ghcr.io/chase-cps/chase`), without nuXmv, whose license does
  not allow redistribution: the checks use NuSMV, built from its sources
  without zChaff (LGPL), and `chase-cps install nuxmv --accept-license` adds
  nuXmv in a container. The image also has Yosys (yowasp-yosys) and the
  `chase-cps` command; the demo suite runs Charger, LMFSM, and AHB with
  NuSMV, and BufferSwap.
- `chase-cps install nusmv` says that the build of FBK is linked with
  zChaff, for non-commercial use only.
- `CONTRIBUTING.md`: reporting bugs, building and testing, and proposing
  changes.

## 0.4.1 (2026-10-28)

New:

- RTL (`chase_cps.rtl`): `Model.from_rtl(files, top)` reads a Verilog or
  SystemVerilog module with Yosys as a state machine, for `implements`,
  `accepts`, and `is_receptive`. The ports are the variables of the
  contracts (1 bit: Boolean, more: words); `reset` holds the reset in the
  first step; `names` renames ports. `pip install chase-cps[rtl]` installs
  Yosys (yowasp-yosys, ISC license).
- `merge(..., discharge=True)` and `divide(..., discharge=True)`: the result
  keeps the terms of the contracts and is the same contract as the closed
  form, then the redundant terms are dropped. The merge is (A1 & A2,
  G1 & G2); the quotient assumes A1 and the guarantees of the part (under
  the assumptions of the part that A1 does not imply), and guarantees G1
  and the assumptions of the part. The results can go to `is_realizable`
  and `synthesize`, which do not take the closed forms.
- Demos: BufferSwap (a buffer of a circuit replaced, with the contracts in
  SVA and the RTL of BaseJump STL), LMFSM (the state machine of the Ten
  Lockheed Martin Cyber-Physical Challenges), and AHB (the arbiter of the
  AMBA AHB bus as two viewpoints in SVA).

Fix:

- `parse_sva` read the assertions inside generate blocks, those of both
  branches of a `generate if`; they are skipped, with the reason.

## 0.4.0 (2026-10-28)

New:

- Words (`chase_cps.words`): unsigned and signed bit vectors of a fixed
  width, `"word[8]"` and `"signed word[8]"`, as in nuXmv and SystemVerilog,
  with the arithmetic modulo 2^n. Operators: `+ - * / mod`, the bitwise
  `& | xor !`, the shifts `<< >>`, the concatenation `::`, and the bits
  `x[7:0]`, `x[3]` (a single bit is a Boolean). A formula whose operands do
  not have the same width and signedness, or a constant that does not fit,
  is refused when the contract is built. The checks give the words to nuXmv;
  the values of the traces are integers. `extend(x, n)`, `signed(x)`, and
  `unsigned(x)` change the width and the signedness, as in nuXmv. The
  contract algebra works on
  contracts with words: each comparison over words goes to the core library
  as a Boolean variable and comes back after the operation (the results are
  the same as on integers). State machines may declare words. Not in the
  logics language (`to_spec`) and not for slugs. MCP server: the types
  `"word[n]"` and `"signed word[n]"`.
- SystemVerilog Assertions (`chase_cps.sva`): `parse_sva` reads the
  `assume property` and `assert property` statements of a text as a
  contract, with the widths of Verilog, `$past` and the functions on it,
  sequences of bounded length, `disable iff`, named properties and
  sequences, and definitions; the statements not read are listed with the
  reason. `to_sva` writes a contract as a checker module (and a bind
  statement). Packed arrays of several dimensions are words; hierarchical
  names (`u1.state`) are read as one name (`u1__state`, mapped back in
  `names`). The meaning is that of the standard (IEEE 1800-2017, Annex F
  for `disable iff`), not that of a tool. Tested against a reference
  interpreter of the standard, against Verilator, and against Jasper.
- `past(x, n)`: the value of x n steps before (any value in the first n
  steps), for variables of any type. nuXmv gets it as history variables;
  the contract algebra treats a formula with it as one proposition; slugs
  and the logics language do not take it. `parse_sva` writes with it the
  comparisons of values two cycles apart or more (`y == $past(x, 2)`).
- The engine of nuXmv can be chosen: `engine="portfolio"` (the default),
  `"ic3"`, `"bdd"`, or `"bmc"`, or in the back end, `"nuxmv/bdd"`, as the
  results name it. Command line: `--engine`. `engine="ic3"` with NuSMV and
  an unknown engine are refused (before, they ran BDDs).

Changes:

- nuXmv runs IC3 and BDDs at the same time by default (`portfolio`; before,
  IC3 alone): the first verdict wins. BDDs are much faster than IC3 when a
  counterexample needs a long loop. The verdicts are the same; a
  counterexample may differ from one run to the next, and the one of BDDs
  can be a loop without a prefix.
- MCP server: the tools have the names of the Python functions:
  `parse_spec` and `to_spec` replace `read_spec` and `write_spec`, and
  `parse_sva`, `to_sva` are new.
- `mod` and `xor` are keywords of the formulas: they cannot be names.

Fixes:

- Variables named as keywords or functions of nuXmv (`count`, `abs`,
  `next`, `init`, `E`, ...) made nuXmv fail with a syntax error; they are
  renamed in the models given to it.
- IC3 of nuXmv 2.2.0 gives wrong answers on formulas with `signed()` and
  `unsigned()` (it finds `G X (s < signed(x)) -> X G (s < signed(x))`
  false); these formulas are checked with BDDs.

## 0.3.2 (2026-10-27)

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
- `remove_vacuous` is removed: it is a removal of redundant terms, and is
  now `remove_redundant(c, synth=True)`, which keeps realizability and the
  controllers (`remove_redundant(c)` keeps A -> G, as before). The result
  is a `Redundancy`, with the `removed` terms (`RedundantTerm`: part, and
  kind with synth). `Vacuity` and `VacuousTerm` are removed. "Vacuity" is
  now only `is_vacuous`.

New:

- `is_vacuous`: vacuity in synthesis (Bloem, Chockler, Ebrahimi, and
  Strichman, FMSD 2021). Can the component win without an output that a
  liveness guarantee asks for, never or from some step on? With the
  strategy, and with `core=True` the assumptions that allow it. The queries
  are those of the exact vacuity checks of ACIES.
- `remove_redundant(c, core=True)`: each removed term comes with a minimal
  set of the terms that imply it.
- State machines (`chase_cps.models`): a `Model` from SMV text or a file,
  from a controller of `synthesize`, or from a state machine in JSON.
  `implements` and `accepts` take them, and `is_receptive` checks that a
  model does not refuse the inputs that the assumptions allow (the state
  and the input when it does). `synthesize` keeps the initial terms of the
  contract with the controller, since slugs does not mark the initial nodes.
- `chase-cps check`: `--implements MODEL C`, `--receptive MODEL C`
  (an SMV or a JSON file), and `--vacuous C`.
- MCP server: the tools `is_vacuous` and `is_receptive`; `implements` and
  `accepts` take a state machine (`smv`, `machine`, `controller`);
  `remove_redundant` takes `synth` and `core`, and the tool `remove_vacuous`
  is removed. The skill says when to use each operation and check.

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
