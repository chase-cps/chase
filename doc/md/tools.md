[<-- Go back to the main page][HOME]

# Tools

After building the sources, two tools are available: Chase and EPStool.

## Chase

It is the main tool of the framework. It takes as input a problem specification
expressed using the CHASE Specification Language and analyzes its consistency.
In the present version, a TuLiP-compatible Python specification is formulated as
shown, for instance, in `demo/Terraswarm/specs` (without timing specifications)
and `demo/Timing/specs` (with timed specifications).

>**Usage**
>
> `chase -i input_file.txt [ -o output_file.py] [-V]`

> To print the help:
>
> `$> chase -h`

> **Parameters:**
>
> - -i _Specifies the `.txt` input file containing the specification._
> - -o _Specifies the python output file._
> - -V _Enables verbose mode._

To execute (in verbose mode) the `chase` tool on a simple example, go to the
main directory of Chase and type

`./bin/chase -i demo/Terraswarm/specs/template_0_synthesizable.txt -o out.py -V`



[<-- Go back to the main page][HOME]
[HOME]: ../../README.md
