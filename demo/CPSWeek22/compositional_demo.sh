#!/bin/sh

echo "Verification of the refinement relation"
echo
read ans
logics_frontend -i specs.ltl -o workdir -c verification.chase -V

echo "Run NuSMV to verify refinement"
ref=`nuxmv workdir/refinement.smv | egrep "is true" | wc -l `

if test $ref -eq 2
then
    echo "Refinement Check: OK"

    echo "Synthesis of the control strategies"
    echo
    read ans
    logics_frontend -i specs.ltl -o workdir -c synthesis.chase

    python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
        workdir/joint1.structuredSlugs > workdir/joint1.slugsin
    python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
        workdir/joint2.structuredSlugs > workdir/joint2.slugsin
    python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
        workdir/joint3.structuredSlugs > workdir/joint3.slugsin

    slugs --explicitStrategy workdir/joint1.slugsin > workdir/joint1.fsm
    slugs --explicitStrategy workdir/joint2.slugsin > workdir/joint2.fsm
    slugs --explicitStrategy workdir/joint3.slugsin > workdir/joint3.fsm

    echo
    read -p "FSMs generated: press enter to view the FSMs" ans
    vim -o workdir/joint*.fsm
fi


