# Contributing to CHASE

CHASE is an open-source project of the University of California, Berkeley,
started at the University of Southern California and the University of
Verona. Contributions are welcome: reports of bugs, questions, demos,
documentation, and code.

## Reporting a bug or asking a question

Open an issue on [chase-cps/chase](https://github.com/chase-cps/chase/issues)
(or on the repository of the part concerned, see below). For a bug, give:

- the version (`chase-cps --version`) and the back ends found
  (`chase-cps doctor`);
- the smallest specification or script that shows the problem, and the
  command that runs it;
- what CHASE answered, and what you expected.

A wrong verdict (a refinement found valid that is not, for instance) is the
most serious kind of bug: please report it even if you are not sure.

## The repositories

| Repository | What it holds |
|---|---|
| [chase](https://github.com/chase-cps/chase) | The entry point: the build of all the modules, the Python package `chase-cps`, the command line, the Docker image, the documentation |
| [core-library](https://github.com/chase-cps/core-library) | The C++ representation of contracts, formulas, and systems, and the contract algebra |
| [chase-backends](https://github.com/chase-cps/chase-backends) | The printers for the back ends (nuXmv, slugs, gr1c, ...) |
| [logics_tool](https://github.com/chase-cps/logics_tool) | The CHASE specification language and its console |
| [DSL_tool](https://github.com/chase-cps/DSL_tool) | The domain languages of requirement patterns |
| [chase-mcp](https://github.com/chase-cps/chase-mcp) | The MCP server for AI agents, and its skill |
| [demos](https://github.com/chase-cps/demos) | The demos |
| [chase-cps.github.io](https://github.com/chase-cps/chase-cps.github.io) | The website |

The C++ modules are submodules of `chase`, in `modules/`. A change to a
module is made in its repository, then the submodule is moved in `chase`.

## Building and testing

Build CHASE from the sources (see "Installation from sources" in the
[README](README.md)):

```
$> git clone --recursive https://github.com/chase-cps/chase
$> cd chase
$> ./install.sh /tmp/chase
```

Install the back ends and run the tests of the Python package:

```
$> PYTHONPATH=python python3 -m chase_cps install nuxmv --accept-license
$> PYTHONPATH=python python3 -m chase_cps install slugs
$> CHASE_INSTALL_DIR=/tmp/chase PYTHONPATH=python python3 -m pytest -q python/tests
```

The tests that need a back end that is not installed are skipped. The
property tests draw random contracts; `CHASE_PROPERTY_RUNS=100` runs more of
them. The demos are run by `tests/demo-suite.sh`, also in the Docker image:

```
$> docker build -t chase . && docker run --rm chase /opt/chase/tests/demo-suite.sh
```

## Proposing a change

1. For anything larger than a fix, open an issue first, so that we can agree
   on the design and the names.
2. Make the change on a branch, one topic per pull request.
3. Add tests: a test for the fix or the feature, and for a change to the
   contract algebra or to a check, a property test that compares it with a
   definition (as in `python/tests/test_properties.py`).
4. Update the documentation (`docs/usage.md`, the docstrings) and add a line
   to `CHANGELOG.md`, under the version not released yet.
5. Open the pull request. The CI builds and tests CHASE on Ubuntu and macOS,
   with AddressSanitizer, and runs the demos in the Docker image; it must
   pass.

A few rules that the code follows:

- The operations and the checks follow their definitions in the
  literature, and the docstring cites the source (the paper and the
  theorem or definition). A new operation comes with that reference.
- The names are the terms of contract theory (`is_implementation`,
  `refines`, `divide`); a name is changed only with the old one kept, with
  a warning, for at least one version.
- The documentation is written plainly: short sentences, the facts, no
  advertising.

## Sending a pull request

Pull requests are sent on GitHub, to the repository of the part that
changes, against its default branch:

| What changes | Repository | Branch |
|---|---|---|
| the Python package, the command line, the Docker image, the build | [chase](https://github.com/chase-cps/chase/pulls) | `master` |
| the C++ representation and the contract algebra | [core-library](https://github.com/chase-cps/core-library/pulls) | `main` |
| the printers for the back ends | [chase-backends](https://github.com/chase-cps/chase-backends/pulls) | `main` |
| the specification language and its console | [logics_tool](https://github.com/chase-cps/logics_tool/pulls) | `main` |
| the domain languages | [DSL_tool](https://github.com/chase-cps/DSL_tool/pulls) | `main` |
| the MCP server and its skill | [chase-mcp](https://github.com/chase-cps/chase-mcp/pulls) | `main` |
| the demos | [demos](https://github.com/chase-cps/demos/pulls) | `main` |
| the website | [chase-cps.github.io](https://github.com/chase-cps/chase-cps.github.io/pulls) | `master` |

1. Fork the repository on GitHub, and clone your fork (for `chase`, with
   `--recursive`).
2. Make the change on a new branch, commit it, and push the branch to your
   fork.
3. Open the pull request from that branch ("Compare & pull request" on
   GitHub), and give the number of the issue it answers.

A change to a C++ module goes to the repository of the module; once it is
merged, the submodule is moved in `chase` (the maintainers do it, or a second
pull request to `chase` does).

## License

CHASE is under the [BSD 3-Clause License](LICENSE). By contributing, you
agree that your contribution is under the same license.
