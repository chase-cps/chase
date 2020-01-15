# CHASE representation library

The CHASE representation library is meant to provide a unified representation 
for systems specifications based on a set of heterogeneous formal representations.
It aims at providing a complete API for the representation and manipulation
of such specifications, with the purpose of enhancing automation of formal
reasoning in system design.

The library is released with CHASE, and all the tools are built on top of such
library. The detailed documentation of the API is provided by relying on Doxygen.
The CMake of the project specifies the ``doc`` target which allows to generate
the Doxygen documentation for the library. I.e., to generate the documentation,
the user just need to type the following command from the build directory:

``$> cmake ..``

``$> make doc``

The last command assumes Doxygen to be installed in the machine;
it guarantees the generation of the classic Doxygen format documentation.

## Abstract Syntax Tree

Any representation in CHASE is based on a Abstract Syntax Tree (AST).
 
__The complete explanation of the AST will be available soon.__

### Extending the library

Indeed, the library is meant to be extended in order to support a larger number
of formal representations. This section will describe the steps necessary to
extend the library by adding new classes that can be used to build the AST
representing a system.

__The detailed description will be available soon.__
 
  