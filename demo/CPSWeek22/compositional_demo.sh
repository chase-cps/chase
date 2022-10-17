#!/bin/sh

PS1="COMPOSITIONAL MODEL DEMO >> "
clear
red () {
    echo "\033[0;31m$1"
}

redn () {
    echo -n "\033[0;31m$1"
}

black () {
    echo "\033[0m$1"
}

blackn () {
    echo -n "\033[0m$1"
}

less specs.ltl
clear && red "Verification of the refinement relation"
blackn "COMPOSITIONAL MODEL DEMO >> logics_frontend -i specs.ltl -o workdir -c verification.chase"
read ans

logics_frontend -i specs.ltl -o workdir -c verification.chase -V

red "Run NuSMV to verify refinement"
ref=`nuxmv workdir/refinement.smv | egrep "is true" | wc -l `

if test $ref -eq 2
then
    black "Refinement Check: OK"
    red "Synthesis of the control strategies"

    blackn "COMPOSITIONAL MODEL DEMO >> logics_frontend -i specs.ltl -o workdir -c synthesis.chase"
    read ans
    logics_frontend -i specs.ltl -o workdir -c synthesis.chase

    python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
        workdir/joint1.structuredSlugs > workdir/joint1.slugsin
    python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
        workdir/joint2.structuredSlugs > workdir/joint2.slugsin
    python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
        workdir/joint3.structuredSlugs > workdir/joint3.slugsin

    slugs --explicitStrategy --jsonOutput workdir/joint1.slugsin > workdir/joint1.json
    slugs --explicitStrategy --jsonOutput workdir/joint2.slugsin > workdir/joint2.json
    slugs --explicitStrategy --jsonOutput workdir/joint3.slugsin > workdir/joint3.json

    echo
    black "FSMs generated: press enter to view the FSMs" ans
    less workdir/joint1.json
fi


