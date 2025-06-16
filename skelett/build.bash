#!/bin/bash

# cmake .. --fresh
make clean
rm -rf CMakeCache.txt CMakeFiles/
make
cmake .. --fresh
./CGPraktikum ../results/test.ppm
open ../results/test.ppm
