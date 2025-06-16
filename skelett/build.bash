#!/bin/bash

# cmake .. --fresh
cmake ..
make
./CGPraktikum ../results/test.ppm
open ../results/test.ppm
