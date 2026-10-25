#!/bin/bash
date > results.txt
date > results.log

t=43200

for i in 1 2 3 4
do
    for j in 04 08 12 16 20 24 28 32
    do
        patternsOnNetwork -i specs/auto_$i\_$j.txt -o current >> results.log 2>> results.log
        python3 ~/software/slugs/tools/StructuredSlugsParser/compiler.py \
            current.structuredSlugs > current.slugsin 2>> results.log
        timeout $t /usr/bin/time -f "$i , $j:\t%e" -o results.txt --append \
            slugs --explicitStrategy --jsonOutput current.slugsin > out.json 2>> results.log
    done
done
date >> results.txt
date >> results.log
