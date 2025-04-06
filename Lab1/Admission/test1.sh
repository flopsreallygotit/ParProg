#!/bin/sh

mpic++ test1.cpp -o test1 -lm
mpirun -np 4 ./test1
