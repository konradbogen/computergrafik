#!/bin/bash

#cmake .. -DCMAKE_BUILD_TYPE=Release --fresh
#make -j$(nproc)
#./CGPraktikum ../results/test.ppm
#open ../results/test.ppm

#make clean
#rm -rf CMakeCache.txt CMakeFiles/
#make
#cmake .. --fresh
#./CGPraktikum ../results/test.ppm
#open ../results/test.ppm

#!/bin/bash

# Full clean rebuild (safe for parallel)
make clean
rm -rf CMakeCache.txt CMakeFiles/ cmake_install.cmake Makefile

cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc) || exit 1

./CGPraktikum ../results/test.ppm
open ../results/test.ppm  # Linux alternative to 'open'
