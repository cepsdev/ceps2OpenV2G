#!/bin/bash

mkdir bin -rf 2>/dev/null

cd bin
CEPSCORE=../ceps/core MACHINES4CEPS=../machines4ceps LOG4CEPS=../log4ceps make -j 32
cd ..
