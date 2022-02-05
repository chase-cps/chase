import re
from z3 import *

# Shelving Row in node 4
## input variables:
node_4_F1_in = Real('node_4_F1_in')
node_4_F2_in = Real('node_4_F2_in')
node_4_F3_in = Real('node_4_F3_in')
node_4_F0_in = Real('node_4_F0_in')

## output variables:
node_4_F1_out = Real('node_4_F1_out')
node_4_F2_out = Real('node_4_F2_out')
node_4_F3_out = Real('node_4_F3_out')
node_4_F0_out = Real('node_4_F3_out')

## assumptions:
node_4_assumptions = And(
    node_4_F1_in + node_4_F2_in + node_4_F3_in + node_4_F0_in <= 1,
    node_4_F1_in + node_4_F2_in + node_4_F3_in + node_4_F0_in >= 0)

## guarantee:
node_4_guarantees = And(
    node_4_F1_out + node_4_F2_out + node_4_F3_out + node_4_F0_out <= 1,
    node_4_F1_out + node_4_F2_out + node_4_F3_out + node_4_F0_out >= 0,
    node_4_F0_out <= 0,
    node_4_F1_out >= 0,
    node_4_F2_out >= 0,
    node_4_F3_out >= 0,
    node_4_F1_in + node_4_F2_in + node_4_F3_in + node_4_F0_in == node_4_F1_out + node_4_F2_out + node_4_F3_out + node_4_F0_out)

################################################################################
################################################################################
# Shelving Row in node 5
## input variables:
node_5_F1_in = Real('node_5_F1_in')
node_5_F2_in = Real('node_5_F2_in')
node_5_F3_in = Real('node_5_F3_in')
node_5_F0_in = Real('node_5_F0_in')

## output variables:
node_5_F1_out = Real('node_5_F1_out')
node_5_F2_out = Real('node_5_F2_out')
node_5_F3_out = Real('node_5_F3_out')
node_5_F0_out = Real('node_5_F3_out')

## assumptions:
node_5_assumptions = And(
    node_5_F1_in + node_5_F2_in + node_5_F3_in + node_5_F0_in <= 1,
    node_5_F1_in + node_5_F2_in + node_5_F3_in + node_5_F0_in >= 0)

## guarantee:
node_5_guarantees = And(
    node_5_F1_out + node_5_F2_out + node_5_F3_out + node_5_F0_out <= 1,
    node_5_F1_out + node_5_F2_out + node_5_F3_out + node_5_F0_out >= 0,
    node_5_F0_out <= 0,
    node_5_F1_out >= 0,
    node_5_F2_out >= 0,
    node_5_F3_out >= 0,
    node_5_F1_in + node_5_F2_in + node_5_F3_in + node_5_F0_in == node_5_F1_out + node_5_F2_out + node_5_F3_out + node_5_F0_out)

################################################################################
################################################################################
# Shelving Row in node 6
## input variables:
node_6_F1_in = Real('node_6_F1_in')
node_6_F2_in = Real('node_6_F2_in')
node_6_F3_in = Real('node_6_F3_in')
node_6_F0_in = Real('node_6_F0_in')

## output variables:
node_6_F1_out = Real('node_6_F1_out')
node_6_F2_out = Real('node_6_F2_out')
node_6_F3_out = Real('node_6_F3_out')
node_6_F0_out = Real('node_6_F3_out')

## assumptions:
node_6_assumptions = And(
    node_6_F1_in + node_6_F2_in + node_6_F3_in + node_6_F0_in <= 1,
    node_6_F1_in + node_6_F2_in + node_6_F3_in + node_6_F0_in >= 0)

## guarantee:
node_6_guarantees = And(
    node_6_F1_out + node_6_F2_out + node_6_F3_out + node_6_F0_out <= 1,
    node_6_F1_out + node_6_F2_out + node_6_F3_out + node_6_F0_out >= 0,
    node_6_F0_out <= 0,
    node_6_F1_out >= 0,
    node_6_F2_out >= 0,
    node_6_F3_out >= 0,
    node_6_F1_in + node_6_F2_in + node_6_F3_in + node_6_F0_in == node_6_F1_out + node_6_F2_out + node_6_F3_out + node_6_F0_out)

################################################################################
################################################################################
# Transport node 0
## input variables:
node_0_F1_in = node_4_F1_out
node_0_F2_in = node_4_F2_out
node_0_F3_in = node_4_F3_out
node_0_F0_in = node_4_F0_out

## output variables:
node_0_F1_out = Real('node_0_F1_out')
node_0_F2_out = Real('node_0_F2_out')
node_0_F3_out = Real('node_0_F3_out')
node_0_F0_out = Real('node_0_F3_out')

## assumptions:
node_0_assumptions = And(
    node_0_F1_in + node_0_F2_in + node_0_F3_in + node_0_F0_in <= 1,
    node_0_F1_in + node_0_F2_in + node_0_F3_in + node_0_F0_in >= 0)

## guarantees:
node_0_guarantees = And(
    node_0_F0_in == node_0_F0_out,
        node_0_F1_in == node_0_F1_out,
            node_0_F2_in == node_0_F2_out,
                node_0_F3_in == node_0_F3_out)

################################################################################
################################################################################
# Transport node 7
## input variables:
node_7_F1_in = Real('node_7_F1_in')
node_7_F2_in = Real('node_7_F2_in')
node_7_F3_in = Real('node_7_F3_in')
node_7_F0_in = Real('node_7_F0_in')

## output variables:
node_7_F1_out = node_6_F1_in
node_7_F2_out = node_6_F1_in
node_7_F3_out = node_6_F1_in
node_7_F0_out = node_6_F1_in

## assumptions:
node_7_assumptions = And(
    node_7_F1_in + node_7_F2_in + node_7_F3_in + node_7_F0_in <= 1,
    node_7_F1_in + node_7_F2_in + node_7_F3_in + node_7_F0_in >= 0)

## guarantees:
node_7_guarantees = And(
    node_7_F0_in == node_7_F0_out,
        node_7_F1_in == node_7_F1_out,
            node_7_F2_in == node_7_F2_out,
                node_7_F3_in == node_7_F3_out)

################################################################################
################################################################################
# Transport node 2
## input variables:
node_2_F1_in = Real('node_2_F1_in')
node_2_F2_in = Real('node_2_F2_in')
node_2_F3_in = Real('node_2_F3_in')
node_2_F0_in = Real('node_2_F0_in')

## output variables:
node_2_F1_out = node_4_F1_in
node_2_F2_out = node_4_F1_in
node_2_F3_out = node_4_F2_in
node_2_F0_out = node_4_F0_in

## assumptions:
node_2_assumptions = And(
    node_2_F1_in + node_2_F2_in + node_2_F3_in + node_2_F0_in <= 1,
    node_2_F1_in + node_2_F2_in + node_2_F3_in + node_2_F0_in >= 0)

## guarantees:
node_2_guarantees = And(
    node_2_F0_in == node_2_F0_out,
        node_2_F1_in == node_2_F1_out,
            node_2_F2_in == node_2_F2_out,
                node_2_F3_in == node_2_F3_out)

################################################################################
################################################################################
# Transport node 3
## input variables:
node_3_F1_in = node_6_F1_out
node_3_F2_in = node_6_F2_out
node_3_F3_in = node_6_F3_out
node_3_F0_in = node_6_F0_out

## output variables:
node_3_F1_out = Real('node_3_F1_out')
node_3_F2_out = Real('node_3_F2_out')
node_3_F3_out = Real('node_3_F3_out')
node_3_F0_out = Real('node_3_F3_out')

## assumptions:
node_3_assumptions = And(
    node_3_F1_in + node_3_F2_in + node_3_F3_in + node_3_F0_in <= 1,
    node_3_F1_in + node_3_F2_in + node_3_F3_in + node_3_F0_in >= 0)

## guarantees:
node_3_guarantees = And(
    node_3_F0_in == node_3_F0_out,
        node_3_F1_in == node_3_F1_out,
            node_3_F2_in == node_3_F2_out,
                node_3_F3_in == node_3_F3_out)

################################################################################
################################################################################
# Crossing node 8
## input variables:
node_8_F1_in = node_0_F1_out
node_8_F2_in = node_0_F2_out
node_8_F3_in = node_0_F3_out
node_8_F0_in = node_0_F0_out

## Output variables:
node_8_F1_out1 = node_5_F1_in
node_8_F2_out1 = node_5_F2_in
node_8_F3_out1 = node_5_F3_in
node_8_F0_out1 = node_5_F0_in

node_8_F1_out2 = node_7_F1_in
node_8_F2_out2 = node_7_F2_in
node_8_F3_out2 = node_7_F3_in
node_8_F0_out2 = node_7_F0_in

## assumptions:
node_8_assumptions = And(
    node_8_F1_in + node_8_F2_in + node_8_F3_in + node_8_F0_in <= 1,
    node_8_F1_in + node_8_F2_in + node_8_F3_in + node_8_F0_in >= 0
)

## guarantees:
node_8_guarantees = And(
    node_8_F1_in + node_8_F2_in + node_8_F3_in + node_8_F0_in ==
    node_8_F1_out1 + node_8_F2_out1 + node_8_F3_out1 + node_8_F0_out1 +
    node_8_F1_out2 + node_8_F2_out2 + node_8_F3_out2 + node_8_F0_out2,
    node_8_F1_in == node_8_F1_out1 + node_8_F1_out2,
    node_8_F2_in == node_8_F2_out1 + node_8_F2_out2,
    node_8_F3_in == node_8_F3_out1 + node_8_F3_out2,
    node_8_F0_in == node_8_F0_out1 + node_8_F0_out2
)

################################################################################
################################################################################
# Crossing node 9
## input variables:
node_9_F1_in1 = node_5_F1_out
node_9_F2_in1 = node_5_F2_out
node_9_F3_in1 = node_5_F3_out
node_9_F0_in1 = node_5_F0_out

node_9_F1_in2 = node_3_F1_out
node_9_F2_in2 = node_3_F2_out
node_9_F3_in2 = node_3_F3_out
node_9_F0_in2 = node_3_F0_out

## Output variables:
node_9_F1_out = node_2_F1_in
node_9_F2_out = node_2_F2_in
node_9_F3_out = node_2_F3_in
node_9_F0_out = node_2_F0_in



## assumptions:
node_9_assumptions = And(
    node_9_F1_in1 + node_9_F2_in1 + node_9_F3_in1 + node_9_F0_in1 +
    node_9_F1_in2 + node_9_F2_in2 + node_9_F3_in2 + node_9_F0_in2 <= 1,
    node_9_F1_in1 + node_9_F2_in1 + node_9_F3_in1 + node_9_F0_in1 +
    node_9_F1_in2 + node_9_F2_in2 + node_9_F3_in2 + node_9_F0_in2 >= 0
)

## guarantees:
node_9_guarantees = And(
    node_9_F1_out == node_9_F1_in1 + node_9_F1_in2,
    node_9_F2_out == node_9_F2_in1 + node_9_F2_in2,
    node_9_F3_out == node_9_F3_in1 + node_9_F3_in2,
    node_9_F0_out == node_9_F0_in1 + node_9_F0_in2
)



