#!/bin/sh

PS1="SINGLE MODEL DEMO >> " 
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


nano specs.ltl
clear && echo -n "SINGLE MODEL DEMO >> logics_frontend -i specs.ltl -o workdir -c singlemodel.chase"
read ans
logics_frontend -i specs.ltl -o workdir -c singlemodel.chase -V
python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
     workdir/single_model.structuredSlugs > workdir/single_model.slugsin
slugs --explicitStrategy workdir/single_model.slugsin > workdir/single_mode.fsm
