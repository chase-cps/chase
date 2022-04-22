#!/bin/sh

logics_frontend -i specs.ltl -o workdir -c singlemodel.chase -V
python3 /home/lora/third_party/slugs/tools/StructuredSlugsParser/compiler.py \
     workdir/single_model.structuredSlugs > workdir/single_model.slugsin
slugs --explicitStrategy workdir/single_model.slugsin > workdir/single_mode.fsm
