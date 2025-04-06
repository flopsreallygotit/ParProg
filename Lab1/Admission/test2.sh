#/bin/sh

mpic++ test2.cpp -o test2
mpirun -np 2 ./test2
