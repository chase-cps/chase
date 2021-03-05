#!/bin/bash

mkdir -p specs

num_joints=$1
freedom_degrees=$2

file=specs/exp_$1_$2.ltl


echo "NAME: ICE_Robotic_Arm_Joints;" > $file 
echo >> $file

for i in $(seq 1 $num_joints)
do
    cat BaseFiles/BaseJoint.ltl | sed "s/NUM/$i/g" | sed "s/MAX/$freedom_degrees/g" >> $file
    echo >> $file
done

cat BaseFiles/Gripper.ltl >> $file


# - Generation of the complete system
echo >> $file
echo "################################################################" >> $file
echo "################################################################" >> $file
echo >> $file
echo "################################################################" >> $file
echo "################################################################" >> $file
echo >> $file

echo "CONTRACT System:" >> $file
for i in $(seq 1 $num_joints)
do
    cat BaseFiles/BaseSystem_Declaration.ltl | sed "s/NUM/$i/g" | sed "s/MAX/$freedom_degrees/g" >> $file
    echo >> $file
done

cat BaseFiles/Gripper_declaration.ltl >> $file 

echo "  Assumptions:" >> $file
for i in $(seq 1 $num_joints)
do
    cat BaseFiles/BaseSystem_Assumptions.ltl | sed "s/NUM/$i/g" | sed "s/MAX/$freedom_degrees/g" >> $file
    echo >> $file
done

cat BaseFiles/Gripper_assumptions.ltl >> $file 

echo "   Guarantees:" >> $file
for i in $(seq 1 $num_joints)
do
    cat BaseFiles/BaseSystem_Guarantees.ltl | sed "s/NUM/$i/g" | sed "s/MAX/$freedom_degrees/g" >> $file
    echo >> $file
done

cat BaseFiles/Gripper_guarantees.ltl >> $file 




