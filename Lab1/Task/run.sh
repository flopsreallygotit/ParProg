#!/bin/sh

mpic++ main.cpp -o main
mpirun -np 4 ./main
