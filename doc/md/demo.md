[<-- Go back to the main page][HOME]

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

[HOME]: ../../README.md
