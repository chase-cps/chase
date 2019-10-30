[<-- Go back to the main page][HOME]

# Requirements

## Compilation and Dependencies
The framework requires the following third party software in order to be built and executed:

- GCC or Clang toolchain for C++:
	- tested with *g++-5* and *clang++-3.8*
	- any compiler supporting C++-11 can be used
- CMake 2.6+
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

However, it should be sufficient to run the install-deps script in the main chase project directory:

`$> sh install-deps.sh`


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
