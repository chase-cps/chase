#!/bin/sh

dir=`pwd`
cd third_party/antlr4_runtime
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make install -j
cd $dir


