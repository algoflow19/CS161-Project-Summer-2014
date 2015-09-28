#!/bin/sh


# build
make clean
make

# find SCCs
date
./sccfinder data/1n7e.txt data/1n7e.txt.scc
date
