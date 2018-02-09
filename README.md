# Contract-based Heterogeneous Analysis and System Exploration (CHASE)
## Software Description
### Purpose and Function
CHASE is a framework for requirement capture, formalization, and validation 
for cyber-physical systems.

CHASE combines a practical front-end formal specification language based on 
patterns (i.e., specification templates) with a rigorous verification back-end 
based on assume-guarantee contracts. The front-end language can express temporal
properties of networks using a declarative style, and supports automatic 
translation from natural-language constructs to low-level mathematical languages. 
The verification back-end leverages the mathematical formalism of contracts to 
reason about system requirements and determine inconsistencies and dependencies
between them.

### Intended Use

CHASE features a modular and extensible software infrastructure that can support
different domain-specific languages, modeling formalisms, and analysis tools. 
It has been applied to industrial design examples, including control of aircraft 
power distribution networks and arbitration of a mixed-criticality automotive bus.

## News: 
### A paper about CHASE has been accepted at the IEEE/ACM Design Automation and Test in Europe (DATE) conference 2018
Pierluigi Nuzzo, Michele Lora, Yishai Feldman, Alberto L. Sangiovanni-Vincentelli,  
"CHASE: Contract-Based Requirement Engineering for Cyber-Physical System Design"

The *scalability* directory, within the *demo* directory contains the specification files used in the experimental section of the paper.

## Index
- [Installation and requirements][INST]
- [Tools and usage][TOOLS]
- [Demonstrators][DEMO]

## Acknowledgement

This work was supported in part by the TerraSwarm Research Center, one of six 
centers supported by the STARnet phase of the Focus Center Research Program 
(FCRP) a Semiconductor Research Corporation program sponsored by MARCO and DARPA.

[INST]: doc/md/installation.md
[TOOLS]: doc/md/tools.md
[DEMO]: doc/md/demo.md
