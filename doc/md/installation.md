[<-- Go back to the main page][HOME]

# Requirements

## Compilation and Dependencies
The framework requires the following third party software in order to be built and executed:

- GCC or Clang toolchain for C++:
	- tested with *g++-5* and *clang++-3.8*
	- any compiler supporting C++-11 can be used
- GNU Make
- [The Temporal Logic Planning (TuLiP) toolbox][1]: a Python package for automatic synthesis of correct-by-construction embedded control software. Supported versions: 1.1+ 
- [gr1c][2]: a collection of tools for GR(1) synthesis,  used to check realizability of GR(1) specifications and synthesize discrete control strategies from them
- [ANTLR 4 (ANother Tool for Language Recognition) C++ Runtime library][3]: ANTLR4 is a parser generator for reading, processing, executing, or translating structured text or binary files. Its C++ Runtime library is used to parse Domain Specific Languages in CHASE.
- Python 2 or 3 (TuLiP requirements)
- Graphviz

## Operating Systems
The framework mostly relies on the C++11 Standard and  should be portable to any architecture and operating system supporting it. However, due to external dependencies, CHASE is tested on:
- Linux Ubuntu 14.04+
- Mac OSX 10.11 (El Capitan)+
- Windows 10 Anniversary Edition, using [*Bash on Ubuntu on Windows*][4] 

# Installation

## Preliminaries
- Install [GR1C][2]
- Install [TuLiP][1]
- Install the [ANTLR 4 C++ Runtime library][5]

## Notes on ANTLR4

- Due to potential incompatibilites with of CHASE with some releases of ANTLR4, a compatible version of the sources of ANTLR4 Runtime for C++ is provided in the `third_party/` directory of the CHASE release. To install the furnished version of ANTLR4, please refer to the Readme file in the antlr4 release available in the third party directory.

- To uncompress the given antlr4 release

`$> cd third_party`

`$> tar xjf antlr4_runtime_sources.tar.bz2`

`$> cd runtime-source`

- To use your own ANTLR4 release, please set the `ANTLR4_HOME` environment variable. In Linux:

`$> export ANTLR4_HOME=<path to your ANTLR4 C++ runtime installation>`

## Building CHASE

`$> export CHASE_COMPILER=compiler`

e.g., type

`$> export CHASE_COMPILER=clang++`

to use *Clang* rather than *GCC*.
The compilation is based on *GNU Make*. The `Makefile` is located in the main directory of the release. Use the command:

`$> make`

which creates two executables in the `bin/` directory:

- `chase`: takes as input a problem expressed using the CHASE formal specification language, creates an internal representation of the specification as a contract, checks the contract consistency (realizability of the specification) and produces a design artifact (e.g., a finite automaton representing the discrete control protocol) if the contract is consistent.
- `epstool`: provides a demonstration of the capabilities of CHASE (a small example of contract-based requirement engineering) specifically targeted to an aircraft Electric Power System (EPS).

It is possible to clean the generated binary files through the command:

`$> make clean`

## Building the API documentation

Documentation requires Doxygen. The Doxyfile is in `doc/Doxyfile`. The Makefile defines a target to build the API documentation:

`$> make dox`

It will generate HTML and LaTeX documentation in `doc/apidoc`.

N.B.: The use of the target `clean` will also delete the documentation.


[<-- Go back to the main page][HOME]

[1]: http://tulip-control.sourceforge.net/
[2]: https://github.com/slivingston/gr1c
[3]: http://www.antlr.org/
[4]: https://msdn.microsoft.com/en-us/commandline/wsl/about
[5]: https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
[HOME]: ../../README.md
