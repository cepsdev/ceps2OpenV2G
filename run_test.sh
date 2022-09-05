#!/bin/bash

echo
echo "Running $1"

echo "--------------------------------"
LD_LIBRARY_PATH=$(pwd)/bin:$LD_LIBRARY_PATH ceps \
 $1 test/base/symbolic_test.ceps \
 --pluginlibceps2openV2G-plugin.so
echo "--------------------------------"
