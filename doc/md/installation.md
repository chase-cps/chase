[<-- Go back to the main page][HOME]

# Requirements

## Compilation and Dependencies
The framework requires the following third party software in order to be built 
and executed:

- GCC or Clang toolchain for C++:
    - tested with *g++-9.3*
    - any compiler supporting C++-11 should be able to compile CHASE
- GNU Make
- Python 3
- [gr1c][GR1C]: a collection of tools for GR(1) synthesis,  used to check
    realizability of GR(1) specifications and synthesize discrete control
    strategies from them.
- [slugs][SLUGS]: a stand-alone reactive synthesis tool for generalized 
reactivity(1) synthesis.
- [ANTLR 4 (ANother Tool for Language Recognition) C++ Runtime library][ANTLR]:
    ANTLR4 is a parser generator for reading, processing, executing, or
    translating structured text or binary files. Its C++ Runtime library is used
    to parse Domain Specific Languages in CHASE.
- Graphviz

## Operating Systems
The framework mostly relies on the C++11 Standard and  should be portable to any
architecture and operating system supporting it. However, due to external
dependencies, CHASE has been tested on:
- Linux Ubuntu 20.04+
- Windows 10 , using [*Bash on Ubuntu on Windows*][WSL] 

# Installation

## Preliminaries
- Install [slugs][SLUGS].
- Install [GR1C][GR1C].


## Notes on ANTLR4

Due to potential incompatibilites with of CHASE with some releases of ANTLR4, a
compatible version of the sources of ANTLR4 Runtime for C++ is provided in the
`third_party/` directory of the CHASE release. While you may want to install
your own version fo ANTLR4, you do not need to do so to use CHASE. 
*The compilation of CHASE takes also care of building ANTLR4 libraries.*

## Building CHASE

The compilation relies on CMake. To compile the source code, from the main
project directory you must create a working directory:

`$> mkdir objdir`

Move in the directory just created, run cmake and compile:

`$> cd objdir`

`$> cmake ..`

`$> make`

Indeed, you may want to specify a installation directory through the CMake
parameter CMAKE_INSTALL_PREFIX. Then, you can install the compiled version of
the software:

`$> make install`

For further configurations of the compilation, please refer to the documentation
of CMake.


## Building the API documentation

Documentation requires Doxygen. The Doxyfile is in `doc/Doxyfile`. The Makefile
defines a target to build the API documentation:

`$> make doc`

It will generate HTML and LaTeX documentation in `doc/apidoc`.

N.B.: The use of the target `clean` will also delete the documentation.


[<-- Go back to the main page][HOME]

[SLUGS]: https://github.com/VerifiableRobotics/slugs
[GR1C]: https://github.com/slivingston/gr1c
[ANTLR]: http://www.antlr.org/
[WSL]: https://msdn.microsoft.com/en-us/commandline/wsl/about
[ANTLR-CPP]: https://github.com/antlr/antlr4/blob/master/doc/cpp-target.md
[HOME]: ../../README.md
