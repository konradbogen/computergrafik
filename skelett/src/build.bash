#!/bin/bash

cmake ..
make
./CGPraktikum ../results/test.ppm
open ../results/test.ppm
