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
node_4_F0_out = Real('node_4_F0_out')

## assumptions:
node_4_assumptions = And(
    node_4_F1_in + node_4_F2_in + node_4_F3_in + node_4_F0_in <= 1,
    node_4_F1_in + node_4_F2_in + node_4_F3_in + node_4_F0_in >= 0,
    node_4_F0_in >= 0, node_4_F0_in <= 1,
    node_4_F1_in >= 0, node_4_F1_in <= 1,
    node_4_F2_in >= 0, node_4_F2_in <= 1,
    node_4_F3_in >= 0, node_4_F3_in <= 1
    )

## guarantee:
node_4_guarantees = And(
    node_4_F1_out + node_4_F2_out + node_4_F3_out + node_4_F0_out <= 1,
    node_4_F1_out + node_4_F2_out + node_4_F3_out + node_4_F0_out >= 0,
    node_4_F0_out >= 0, node_4_F0_out <= 1,
    node_4_F1_out >= 0, node_4_F1_out <= 1,
    node_4_F2_out >= 0, node_4_F2_out <= 1,
    node_4_F3_out >= 0, node_4_F3_out <= 1,
    node_4_F0_out <= node_4_F0_in,
    node_4_F1_out >= node_4_F1_in,
    node_4_F2_out == node_4_F2_in,
    node_4_F3_out == node_4_F3_in,
    node_4_F1_in + node_4_F2_in + node_4_F3_in + node_4_F0_in == 
        node_4_F1_out + node_4_F2_out + node_4_F3_out + node_4_F0_out)

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
node_5_F0_out = Real('node_5_F0_out')

## assumptions:
node_5_assumptions = And(
    node_5_F1_in + node_5_F2_in + node_5_F3_in + node_5_F0_in <= 1,
    node_5_F1_in + node_5_F2_in + node_5_F3_in + node_5_F0_in >= 0,
    node_5_F0_in >= 0, node_5_F0_in <= 1,
    node_5_F1_in >= 0, node_5_F1_in <= 1,
    # node_5_F2_in == 0,
    node_5_F2_in >= 0, node_5_F2_in <= 1,
    node_5_F3_in >= 0, node_5_F3_in <= 1)

## guarantee:
node_5_guarantees = And(
    node_5_F1_out + node_5_F2_out + node_5_F3_out + node_5_F0_out <= 1,
    node_5_F1_out + node_5_F2_out + node_5_F3_out + node_5_F0_out >= 0,
    node_5_F0_out >= 0, node_5_F0_out <= 1,
    node_5_F1_out >= 0, node_5_F1_out <= 1,
    node_5_F2_out >= 0, node_5_F2_out <= 1,
    node_5_F3_out >= 0, node_5_F3_out <= 1,
    node_5_F0_out <= node_5_F0_in,
    node_5_F2_out >= node_5_F2_in,
    node_5_F1_out == node_5_F1_in,
    node_5_F3_out == node_5_F3_in,
    node_5_F1_in + node_5_F2_in + node_5_F3_in + node_5_F0_in ==
        node_5_F1_out + node_5_F2_out + node_5_F3_out + node_5_F0_out)

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
node_6_F0_out = Real('node_6_F0_out')

## assumptions:
node_6_assumptions = And(
    node_6_F1_in + node_6_F2_in + node_6_F3_in + node_6_F0_in <= 1,
    node_6_F1_in + node_6_F2_in + node_6_F3_in + node_6_F0_in >= 0,
    #node_6_F3_in >= 0, node_6_F3_in <= 1,
    node_6_F3_in == 0,
    node_6_F0_in >= 0, node_6_F0_in <= 1,
    node_6_F1_in >= 0, node_6_F1_in <= 1,
    node_6_F2_in >= 0, node_6_F2_in <= 1
    )
    

## guarantee:
node_6_guarantees = And(
    node_6_F1_out + node_6_F2_out + node_6_F3_out + node_6_F0_out <= 1,
    node_6_F1_out + node_6_F2_out + node_6_F3_out + node_6_F0_out >= 0,
    node_6_F0_out >= 0, node_6_F0_out <= 1,
    node_6_F1_out >= 0, node_6_F1_out <= 1,
    node_6_F2_out >= 0, node_6_F2_out <= 1,
    node_6_F3_out >= 0, node_6_F3_out <= 1,
    node_6_F0_out <= node_6_F0_in,
    node_6_F3_out >= node_6_F3_in,
    node_6_F1_out == node_6_F1_in,
    node_6_F2_out == node_6_F2_in,
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
node_0_F0_out = Real('node_0_F0_out')

## assumptions:
node_0_assumptions = And(
    node_0_F1_in + node_0_F2_in + node_0_F3_in + node_0_F0_in <= 1,
    node_0_F1_in + node_0_F2_in + node_0_F3_in + node_0_F0_in >= 0,
    node_0_F1_in >= 0, node_0_F1_in <= 1,
    node_0_F2_in >= 0, node_0_F2_in <= 1,
    node_0_F3_in >= 0, node_0_F3_in <= 1,
    node_0_F0_in >= 0, node_0_F0_in <= 1)

## guarantees:
node_0_guarantees = And(
    node_0_F0_in == node_0_F0_out,
    node_0_F1_in == node_0_F1_out,
    node_0_F2_in == node_0_F2_out,
    node_0_F3_in == node_0_F3_out,
    node_0_F1_out >= 0, node_0_F1_out <= 1,
    node_0_F2_out >= 0, node_0_F2_out <= 1,
    node_0_F3_out >= 0, node_0_F3_out <= 1,
    node_0_F0_out >= 0, node_0_F0_out <= 1,
    node_0_F1_out + node_0_F2_out + node_0_F3_out + node_0_F0_out <= 1,
    node_0_F1_out + node_0_F2_out + node_0_F3_out + node_0_F0_out >= 0,
    node_0_F1_in + node_0_F2_in + node_0_F3_in + node_0_F0_in == node_0_F1_out + node_0_F2_out + node_0_F3_out + node_0_F0_out)

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
node_7_F2_out = node_6_F2_in
node_7_F3_out = node_6_F3_in
node_7_F0_out = node_6_F0_in

## assumptions:
node_7_assumptions = And(
    node_7_F1_in + node_7_F2_in + node_7_F3_in + node_7_F0_in <= 1,
    node_7_F1_in + node_7_F2_in + node_7_F3_in + node_7_F0_in >= 0,
    node_7_F1_in >= 0, node_7_F1_in <= 1,
    node_7_F2_in >= 0, node_7_F2_in <= 1,
    node_7_F3_in >= 0, node_7_F3_in <= 1,
    node_7_F0_in >= 0, node_7_F0_in <= 1)

## guarantees:
node_7_guarantees = And(
    node_7_F0_in == node_7_F0_out,
    node_7_F1_in == node_7_F1_out,
    node_7_F2_in == node_7_F2_out,
    node_7_F3_in == node_7_F3_out,
    node_7_F1_out >= 0, node_7_F1_out <= 1,
    node_7_F2_out >= 0, node_7_F2_out <= 1,
    node_7_F3_out >= 0, node_7_F3_out <= 1,
    node_7_F0_out >= 0, node_7_F0_out <= 1,
    node_7_F1_out + node_7_F2_out + node_7_F3_out + node_7_F0_out <= 1,
    node_7_F1_out + node_7_F2_out + node_7_F3_out + node_7_F0_out >= 0,
    node_7_F1_in + node_7_F2_in + node_7_F3_in + node_7_F0_in == node_7_F1_out + node_7_F2_out + node_7_F3_out + node_7_F0_out)

################################################################################
################################################################################
# Transport node 2
## input variables:
node_2_F1_in = Real('node_2_F1_in')
node_2_F2_in = Real('node_2_F2_in')
node_2_F3_in = Real('node_2_F3_in')
node_2_F0_in = Real('node_2_F0_in')

node_2_F1_inf = Real('node_2_F1_inf')
node_2_F2_inf = Real('node_2_F2_inf')
node_2_F3_inf = Real('node_2_F3_inf')
node_2_F0_inf = Real('node_2_F0_inf')

## output variables:
node_2_F1_out = node_4_F1_in
node_2_F2_out = node_4_F1_in
node_2_F3_out = node_4_F2_in
node_2_F0_out = node_4_F0_in

node_2_F1_outf = Real('node_2_F1_outf')
node_2_F2_outf = Real('node_2_F2_outf')
node_2_F3_outf = Real('node_2_F3_outf')
node_2_F0_outf = Real('node_2_F0_outf')


## assumptions:
node_2_assumptions = And(
    node_2_F1_in + node_2_F2_in + node_2_F3_in + node_2_F0_in +
    node_2_F1_inf + node_2_F2_inf + node_2_F3_inf + node_2_F0_inf <= 1,
    node_2_F1_in + node_2_F2_in + node_2_F3_in + node_2_F0_in +
    node_2_F1_inf + node_2_F2_inf + node_2_F3_inf + node_2_F0_inf >= 0,
    node_2_F0_in >= 0, node_2_F0_in <= 1,
    node_2_F1_in >= 0, node_2_F1_in <= 1,
    node_2_F2_in >= 0, node_2_F2_in <= 1,
    node_2_F3_in >= 0, node_2_F3_in <= 1,
    node_2_F0_inf >= 0, node_2_F0_inf <= 1,
    node_2_F1_inf >= 0, node_2_F1_inf <= 1,
    node_2_F2_inf >= 0, node_2_F2_inf <= 1,
    node_2_F3_inf >= 0, node_2_F3_inf <= 1)

## guarantees:
node_2_guarantees = And(
    node_2_F0_in + node_2_F0_inf == node_2_F0_out + node_2_F0_outf,
    node_2_F1_in + node_2_F1_inf == node_2_F1_out + node_2_F1_outf,
    node_2_F2_in + node_2_F2_inf == node_2_F2_out + node_2_F2_outf,
    node_2_F3_in + node_2_F3_inf == node_2_F3_out + node_2_F3_outf,
    node_2_F0_out >= 0, node_2_F0_out <= 1,
    node_2_F1_out >= 0, node_2_F1_out <= 1,
    node_2_F2_out >= 0, node_2_F2_out <= 1,
    node_2_F3_out >= 0, node_2_F3_out <= 1,
    node_2_F0_outf >= 0, node_2_F0_outf <= 1,
    node_2_F1_outf >= 0, node_2_F1_outf <= 1,
    node_2_F2_outf >= 0, node_2_F2_outf <= 1,
    node_2_F3_outf >= 0, node_2_F3_outf <= 1,
    node_2_F1_in + node_2_F2_in + node_2_F3_in + node_2_F0_in +
    node_2_F1_inf + node_2_F2_inf + node_2_F3_inf + node_2_F0_inf == 
    node_2_F1_out + node_2_F2_out + node_2_F3_out + node_2_F0_out +
    node_2_F1_outf + node_2_F2_outf + node_2_F3_outf + node_2_F0_outf)

################################################################################
################################################################################
# Transport node 3
## input variables:
node_3_F1_in = node_6_F1_out
node_3_F2_in = node_6_F2_out
node_3_F3_in = node_6_F3_out
node_3_F0_in = node_6_F0_out

node_3_F1_inf = Real('node_3_F1_inf')
node_3_F2_inf = Real('node_3_F2_inf')
node_3_F3_inf = Real('node_3_F3_inf')
node_3_F0_inf = Real('node_3_F0_inf')

## output variables:
node_3_F1_out = Real('node_3_F1_out')
node_3_F2_out = Real('node_3_F2_out')
node_3_F3_out = Real('node_3_F3_out')
node_3_F0_out = Real('node_3_F0_out')

node_3_F1_outf = Real('node_3_F1_outf')
node_3_F2_outf = Real('node_3_F2_outf')
node_3_F3_outf = Real('node_3_F3_outf')
node_3_F0_outf = Real('node_3_F0_outf')

## assumptions:
node_3_assumptions = And(
    node_3_F1_in + node_3_F2_in + node_3_F3_in + node_3_F0_in +
    node_3_F1_inf + node_3_F2_inf + node_3_F3_inf + node_3_F0_inf <= 1,
    node_3_F1_in + node_3_F2_in + node_3_F3_in + node_3_F0_in +
    node_3_F1_inf + node_3_F2_inf + node_3_F3_inf + node_3_F0_inf >= 0,
    node_3_F0_in >= 0, node_3_F0_in <= 1,
    node_3_F1_in >= 0, node_3_F1_in <= 1,
    node_3_F2_in >= 0, node_3_F2_in <= 1,
    node_3_F3_in >= 0, node_3_F3_in <= 1,
    node_3_F0_inf >= 0, node_3_F0_inf <= 1,
    node_3_F1_inf >= 0, node_3_F1_inf <= 1,
    node_3_F2_inf >= 0, node_3_F2_inf <= 1,
    node_3_F3_inf >= 0, node_3_F3_inf <= 1)

## guarantees:
node_3_guarantees = And(
    node_3_F0_in + node_3_F0_inf == node_3_F0_out + node_3_F0_outf,
    node_3_F1_in + node_3_F1_inf == node_3_F1_out + node_3_F1_outf,
    node_3_F2_in + node_3_F2_inf == node_3_F2_out + node_3_F2_outf,
    node_3_F3_in + node_3_F3_inf == node_3_F3_out + node_3_F3_outf,
    node_3_F0_out >= 0, node_3_F0_out <= 1,
    node_3_F1_out >= 0, node_3_F1_out <= 1,
    node_3_F2_out >= 0, node_3_F2_out <= 1,
    node_3_F3_out >= 0, node_3_F3_out <= 1,
    node_3_F0_outf >= 0, node_3_F0_outf <= 1,
    node_3_F1_outf >= 0, node_3_F1_outf <= 1,
    node_3_F2_outf >= 0, node_3_F2_outf <= 1,
    node_3_F3_outf >= 0, node_3_F3_outf <= 1,
    node_3_F1_in + node_3_F2_in + node_3_F3_in + node_3_F0_in +
    node_3_F1_inf + node_3_F2_inf + node_3_F3_inf + node_3_F0_inf == 
    node_3_F1_out + node_3_F2_out + node_3_F3_out + node_3_F0_out +
    node_3_F1_outf + node_3_F2_outf + node_3_F3_outf + node_3_F0_outf)

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
    node_8_F1_in + node_8_F2_in + node_8_F3_in + node_8_F0_in >= 0,
    node_8_F1_in >= 0, node_8_F1_in <= 1,
    node_8_F2_in >= 0, node_8_F2_in <= 1,
    node_8_F3_in >= 0, node_8_F3_in <= 1,
    node_8_F0_in >= 0, node_8_F0_in <= 1
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
    node_9_F1_in2 + node_9_F2_in2 + node_9_F3_in2 + node_9_F0_in2 >= 0,
    node_9_F1_in1 >= 0, node_9_F1_in1 <= 1,
    node_9_F2_in1 >= 0, node_9_F2_in1 <= 1,
    node_9_F3_in1 >= 0, node_9_F3_in1 <= 1,
    node_9_F0_in1 >= 0, node_9_F0_in1 <= 1,
    node_9_F1_in2 >= 0, node_9_F1_in2 <= 1,
    node_9_F2_in2 >= 0, node_9_F2_in2 <= 1,
    node_9_F3_in2 >= 0, node_9_F3_in2 <= 1,
    node_9_F0_in2 >= 0, node_9_F0_in2 <= 1
)

## guarantees:
node_9_guarantees = And(
        node_9_F1_out + node_9_F2_out + node_9_F3_out + node_9_F0_out ==
    node_9_F1_in1 + node_9_F2_in1 + node_9_F3_in1 + node_9_F0_in1 +
    node_9_F1_in2 + node_9_F2_in2 + node_9_F3_in2 + node_9_F0_in2,
    node_9_F1_out == node_9_F1_in1 + node_9_F1_in2,
    node_9_F2_out == node_9_F2_in1 + node_9_F2_in2,
    node_9_F3_out == node_9_F3_in1 + node_9_F3_in2,
    node_9_F0_out == node_9_F0_in1 + node_9_F0_in2
)

################################################################################
################################################################################
# Crossing node 1
node_1_F1_in1 = node_3_F1_outf
node_1_F2_in1 = node_3_F2_outf
node_1_F3_in1 = node_3_F3_outf
node_1_F0_in1 = node_3_F0_outf

node_1_F1_in2 = node_2_F1_outf
node_1_F2_in2 = node_2_F2_outf
node_1_F3_in2 = node_2_F3_outf
node_1_F0_in2 = node_2_F0_outf

node_1_F1_out1 = node_3_F1_inf
node_1_F2_out1 = node_3_F2_inf
node_1_F3_out1 = node_3_F3_inf
node_1_F0_out1 = node_3_F0_inf

node_1_F1_out2 = node_2_F1_inf
node_1_F2_out2 = node_2_F2_inf
node_1_F3_out2 = node_2_F3_inf
node_1_F0_out2 = node_2_F0_inf

node_1_F1_sinked = Real('node_1_F1_sinked')
node_1_F2_sinked = Real('node_1_F2_sinked')
node_1_F3_sinked = Real('node_1_F3_sinked')

##
# ASSUMPTIONS AND GUARANTEES OF C1
##
node_1_assumptions = And(
    node_1_F1_in1 + node_1_F0_in1 + node_1_F2_in1 + node_1_F3_in1 +
    node_1_F1_in2 + node_1_F0_in2 + node_1_F2_in2 + node_1_F3_in2 <= 1,
    node_1_F1_in1 + node_1_F0_in1 + node_1_F2_in1 + node_1_F3_in1 +
    node_1_F1_in2 + node_1_F0_in2 + node_1_F2_in2 + node_1_F3_in2 >= 0,
    node_1_F1_in1 >= 0, node_1_F1_in1 <= 1,
    node_1_F2_in1 >= 0, node_1_F2_in1 <= 1,
    node_1_F3_in1 >= 0, node_1_F3_in1 <= 1,
    node_1_F0_in1 >= 0, node_1_F0_in1 <= 1,
    node_1_F1_in2 >= 0, node_1_F1_in2 <= 1,
    node_1_F2_in2 >= 0, node_1_F2_in2 <= 1,
    node_1_F3_in2 >= 0, node_1_F3_in2 <= 1,
    node_1_F0_in2 >= 0, node_1_F0_in2 <= 1
)

node_1_guarantees = And(
    node_1_F1_out1 + node_1_F0_out1 + node_1_F2_out1 + node_1_F3_out1 +
    node_1_F1_out2 + node_1_F0_out2 + node_1_F2_out2 + node_1_F3_out2 <= 1,
    node_1_F1_out1 + node_1_F0_out1 + node_1_F2_out1 + node_1_F3_out1 +
    node_1_F1_out2 + node_1_F0_out2 + node_1_F2_out2 + node_1_F3_out2 >= 0,
    node_1_F1_out1 >= 0, node_1_F1_out1 <= 1,
    node_1_F2_out1 >= 0, node_1_F2_out1 <= 1,
    node_1_F3_out1 >= 0, node_1_F3_out1 <= 1,
    node_1_F1_out2 >= 0, node_1_F1_out2 <= 1,
    node_1_F2_out2 >= 0, node_1_F2_out2 <= 1,
    node_1_F3_out2 >= 0, node_1_F3_out2 <= 1,
    node_1_F0_out1 + node_1_F0_out2 == 
        node_1_F1_sinked + node_1_F2_sinked + node_1_F3_sinked 
        + node_1_F0_in1 + node_1_F0_in2,   
    node_1_F1_sinked == node_1_F1_in1 + node_1_F1_in2,
    node_1_F2_sinked == node_1_F2_in1 + node_1_F2_in2,
    node_1_F3_sinked == node_1_F3_in1 + node_1_F3_in2
)

## Contracts saturation.
node_0_guarantees = And(node_0_assumptions, node_0_guarantees)
node_1_guarantees = Implies(node_1_assumptions, node_1_guarantees)
node_2_guarantees = Implies(node_2_assumptions, node_2_guarantees)
node_3_guarantees = Implies(node_3_assumptions, node_3_guarantees)
node_4_guarantees = Implies(node_4_assumptions, node_4_guarantees)
node_5_guarantees = Implies(node_5_assumptions, node_5_guarantees)
node_6_guarantees = Implies(node_6_assumptions, node_6_guarantees)
node_7_guarantees = Implies(node_7_assumptions, node_7_guarantees)
node_8_guarantees = Implies(node_8_assumptions, node_8_guarantees)
node_9_guarantees = Implies(node_9_assumptions, node_9_guarantees)

### Contract warehouse

warehouse_assumptions = True;

warehouse_guarantees = And(
    node_1_F3_sinked >= 0.2,
    node_1_F2_sinked >= 0.2,
    node_1_F1_sinked >= 0.2
)

warehouse_guarantees = Implies(warehouse_assumptions, warehouse_guarantees)

##### Algebra stuff...
system_guarantees = And(
    node_0_guarantees,
    node_1_guarantees,
    node_2_guarantees,
    node_3_guarantees,
    node_4_guarantees,
    node_5_guarantees,
    node_6_guarantees,
    node_7_guarantees,
    node_8_guarantees,
    node_9_guarantees,
    warehouse_guarantees
)

system_assumptions = And(
    node_0_assumptions,
    node_1_assumptions,
    node_2_assumptions,
    node_3_assumptions,
    node_4_assumptions,
    node_5_assumptions,
    node_6_assumptions,
    node_7_assumptions,
    node_8_assumptions,
    node_9_assumptions,
    warehouse_assumptions
)

system_assumptions = Or(
    system_assumptions,
    Not(system_guarantees)
)

system_guarantees = Implies(system_assumptions, system_guarantees)

#print(system_guarantees)
#print("Assumptions: ")
#solve(system_assumptions)
#print("Guarantees")
#solve(system_guarantees)

print("A and G")
solve(And(system_assumptions, system_guarantees))