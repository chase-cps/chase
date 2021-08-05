[<-- Go back to the main page][HOME]

# Tools

After building the sources, some tools are available. 
This document describe the different tools.

## dsl_parser


The tool takes as input a problem specification expressed using the CHASE Specification Language and analyzes its 
consistency. In the present version, it encodes the problem for GR1C, which is then used to perform consistency
checking and synthesis of the specification.

The CHASE specification language accepted by the tool allows specifying a set of requirements about the system behavior
over an architecture represented by a network.

More information about the language will be soon available in a language guide.

>**Usage**
>
> `dsl_parser -i input_file.txt [ -o output_file.py] [-V]`

> To print the help:
>
> `$> chase -h`

> **Parameters:**
>
> - -i _Specifies the `.txt` input file containing the specification._
> - -o _Specifies the output file._
> - -b _Specifies the backend to be used. It may either be: `slugs` (default value) or `gr1c`._
> - -V _Enables verbose mode._

To execute (in verbose mode) the `chase` on a simple example, go to the main directory of Chase and type

`$> ./bin/dsl_parser -i demo/DATE_2018/2_04_03.txt -o out.structuredSlugs -V`

It will generate a  _structured slugs_ file. The same can be obtained by using the following command:

`$> ./bin/dsl_parser -i demo/DATE_2018/2_04_03.txt -b slugs -o out.structuredSlugs -V`

Meanwhile, the following command:

`$> ./bin/dsl_parser -i demo/DATE_2018/2_04_03.txt -b gr1c -o out.spc -V`

will generate a _GR1C_ specification.

For the usage of [slugs][SLUGS] and [GR1C][GR1C], we refer the reader to the tools documentation.

[<-- Go back to the main page][HOME]

[SLUGS]: https://github.com/VerifiableRobotics/slugs
[GR1C]: https://github.com/slivingston/gr1c

[HOME]: ../../README.md
