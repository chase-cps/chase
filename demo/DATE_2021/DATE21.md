## IEEE/ACM Design Automation and Test in Europe 2021 - University Booth

This demo has been originally presented at the University Booth of the IEEE/ACM
Design Automation and Test in Europe 2021. The demo is meant to guide the user 
in using the two main modes of use provided by CHASE, _i.e._, using the Domain
Specific Language (DSL) provided with the framework, or using the Logic-based
language available in CHASE.

### Content of the directory
The directory of the demo is organized into two subdirectories:
- `bin/`: contains some utility scripts to use synthesis toolchains.
- `specs/`: contains the specification files to be used during the demo.

### DSL-based Flow

The first part of the demo is based on the DSL proposed in [1]. The objective
is to realize a controller for a system starting from a set of requirements.
The system is the Electrical Power System (EPS) described in [1]. 

The requirements of the system are the following:
#### Requirements
- If possible, left DC buses shall be powered by left generators
- Failed generators must be disconnected within 15 ms
- Generators shall never connect in parallel through AC buses
- A DC bus shall never be unpowered for more than 15 ms

#### Assumptions
- When a contactor receives an open signal, it opens in 10ms or less
- When a contactor receives an close signal, it closes in 10ms or less
- Generators do not recover from failures
- AC buses do not fail
- Rectifiers do not fail
- DC buses do not fail
- Loads do not fail
- At most 1 generator may fail

The requirements are expressed using the DSL in
`specs/template_0_unrealizable.txt`.

The first step, is to formalize the requirements by using CHASE:

`$> dsl_tool -i specs/template_0_unrealizable.txt -o /tmp/template_0 -b gr1c`

Then, verify the realizability of the specifications, using GR1C:

`$> gr1c -r /tmp/template_0.spc`\
`Not realizable.`

However, the requirements are not realizable. Thus, replace the second and
fourth requirements with the following:
- Failed generators must be disconnected within 20 ms
- A DC bus shall never be unpowered for more than 30 ms

The file `specs/template_0_realizable.txt` specifies these requirements.
Thus, first let's verify its realizability and try to synthesize it. For 
this step, let's use the utilities scripts:

`$> dsl_tool -i specs/template_0_synthesizable.txt -o /tmp/template_0 -b gr1c`
`$> bash bin/synthesize_gr1c /tmp/template_0`

The last command, will generate a graphical representation of the
Mealy Machine implementing the control strategy for the system.

### Logic-based Flow

The second part of the demo is based on the logic-based language of CHASE. In
particular, it is based on a set of contracts expressed in LTL. 
The system being modeled is a robotic arm, which has to pick, turn and place
a piece of material.

The formalization is provided in `specs/robotic_arm.ltl`.
The specification is manipulated using the `logics_frontend`:

`logics_frontend -i specs/robotic_arm.ltl -o /tmp/`

The command will open the front-end console, which provides a set of commands
that can be seen by using the `help` command.

First, let's read the system specification:

`$> show`

The system is described by six contracts: 
- _Mission_: modeling the actions to be completed by the robot;
- _Coordinator_: modeling the controller of the system;
- _Move_: modeling the move action;
- _Turn_: modeling the turn action;
- _Pick_: modeling the pick action;
- _Place_: modeling the place action.

The objective is to compose the system by using the logics frontend.

`$> compose Mission Coordinator Top`\
`$> compose Move Turn MT`\
`$> compose MT Pick MTP`\
`$> compose MTP Place Bottom`\
`$> compose Top Bottom System`

Finally, it is possible to verify the consistency and compatibility of the
_System_ contract.

`$> verify System system`

The last command generates the `system.smv` to be used in NuSMV to verify
the consistency and the compatibility of the contract:

`$> exit`\
`$> NuSMV /tmp/system.smv`

For both assertions, the model checker returns a counter strategy: the
contract is consistent and compatible.

### References
[1] Nuzzo, Pierluigi, et al. _CHASE: Contract-based requirement engineering for 
cyber-physical system design._ in Proceeding of the 2018 Design, Automation and 
Test in Europe Conference & Exhibition (DATE). IEEE, 2018.