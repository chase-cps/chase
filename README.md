# News: 
## A paper about CHASE has been accepted at the IEEE/ACM Design Automation and Test in Europe (DATE) conference 2018
Pierluigi Nuzzo, Michele Lora, Yishai Feldman, Alberto L. Sangiovanni-Vincentelli,  
"CHASE: Contract-Based Requirement Engineering for Cyber-Physical System Design"

The *scalability* directory, within the *demo* directory contains the specification files used in the experimental section of the paper.

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
- Install [TuLiP][3]
- Install the [ANTLR 4 C++ Runtime library][5]


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

# Tools

After building the sources, two tools are available: Chase and EPStool.

## Chase

It is the main tool of the framework. It takes as input a problem specification expressed using the CHASE Specification Language and analyzes its consistency. In the present version, a TuLiP-compatible Python specification is formulated as shown, for instance, in `demo/Terraswarm/specs` (without timing specifications) and `demo/Timing/specs` (with timed specifications).

>**Usage**
>
> `chase -i input_file.txt [ -o output_file.py] [-V]`

> To print the help:
>
> `$> chase -h`

> **Parameters:**
>
> - -i _Specifies the `.txt` input file containing the specification._
> - -o _Specifies the python output file. Default: input file with extension `.py` rather than `.txt`._
> - -V _Enables verbose mode._

To execute (in verbose mode) the `chase` tool on a simple example, go to the main directory of Chase and type

`./bin/chase -i demo/Terraswarm/specs/template_0_synthesizable.txt -o out.py -V`

## EPSTool
This tool requires a file specifying the architecture of the EPS and a file with the system requirements expressed using a set of patterns (templates). Examples of architectural specifications can be found in `./demo/EPS/architecture_specs/`.  Examples of system requirements can be found in `./demo/EPS/dynamic_specs/`.

>**Usage**:
>
`$> epstool -a Architecture_specs -b Behaviors_specs -o outfile [-p]`

>`$> epstool -h`

>**Parameters:**
>
>- -a _Architecture specs file_
>- -b _Dynamics specs file_
>- -o _Output file_
>- -p _Use "physical" contactors with delay_

For instance, to execute `epstool` on a simple example, type

`./bin/epstool -a demo/EPS/architecture_specs/template_0_topology.txt -b demo/EPS/dynamic_specs/template_0_synthesizable_nc.txt -o out.py`

# Backend

In the current version, the execution of `chase` and  `epstool` generates a TuLiP-compatible Python specification. Considering the previous examples, the `out.py` file can be executed as follows:

`$> python out.py`

However, it is possible to support a set of options while executing a generic `file.py` generated by `chase` or `epstool`, as shown below.

 > **Usage:**
 >
 >`python file.py -g -f GRAPHIC_REPRESENTATION_FILE -i -p PYTHON_IMPLEMENTATION_FILE`
 >
 > To print the help:
 >
 > `python file.py -h`
 >
 > **Parameters:**
 >
 > - `-h`: _Shows the help message._
 > - `-g, --graphical`: _Produces a graphical representation of the finite state machine of the controller if the specification is realizable. Disabled by default._
 > - `-i, --implement`: _Produces a Python implementation of the controller FSM. Disabled by default._
 > - `-p PYTHON_FILE`: _Specifies the path of the output Python file containing the controller FSM. It is allowed only if `-i, --implement` are used. The default value is `automaton.py`.
 > - `-f GRAPHIC_FILE`: _Specifies the path of the output graphic file with the controller FSM. It is allowed only if `-g, --graphical` are used. The default value is `graphical.py`. The supported graphic formats (specified by the file name) are: `eps, dot, jpg`, and `png`.

For instance, type the following command to generate both the graphical (`eps` file) and Python implementation of the discrete control strategy from `out.py`, generated as shown above

`$> python out.py -g -f fsm.eps -i -p fsm.py`


# Demonstrations

The demo directory contains three different demonstrations:

- **Automotive**: Demonstration of the `chase` tool on a Automotive, double-priority communication bus with real time intervals and resources contention.
- **EPS**: Old demonstration of the `epstool`, using a previous version of the specification language.
- **Terraswarm**: Demonstration of the `chase` tool on an aircraft power distribution network without real time intervals in the specification. Presented at the *TerraSwarm Annual Meeting* held in Berkeley on October 26 and 27.
- **Timing**: Demonstration of the `chase` tool on an aircraft power distribution network with real time intervals in the specification.

To execute one of the demos, go to the selected demo directory and type

`$> make`

For instance, to execute the *Terraswarm* demo, type

> `$> cd demo/Terraswarm`

> `$> make`

**Terraswarm** and **Timing** demos allow choosing among different problems:

- `template_0`: Provided for all the demos, it includes a network topology with two sources.
- `template_1`: Provided for all the demos, it includes a topology with four sources.
- `template_2`: Provided only in the *Timing* demo, it includes a network topology with three generators.

To choose the problem, set the `MODEL` environment variable and then run the demo. For instance, to use `template_2` in the *Timing* demo, type

> `$> cd demo/Timing`

> `$> export MODEL=template_2`

> `$> make`

The **Automotive** demo presents only one scenario. As such, the variable MODEL is constant.


[1]: http://tulip-control.sourceforge.net/
[2]: https://github.com/slivingston/gr1c
[3]: http://www.antlr.org/
[4]: https://msdn.microsoft.com/en-us/commandline/wsl/about
[5]: https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
