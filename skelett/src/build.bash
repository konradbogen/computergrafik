#!/bin/bash

cmake .. -DCMAKE_BUILD_TYPE=Release --fresh
make -j$(nproc)
./CGPraktikum ../results/test.ppm
open ../results/test.ppm