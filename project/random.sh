#!/bin/sh


# build
rm -f data/network*.scc
make clean
make

N=$1
M=$2
OUTFILE=$3

for i in {1..10}
do
    rm -f $OUTFILE
    ./randomgraph $N $M $OUTFILE
    ./sccfinder $OUTFILE "$OUTFILE.scc"
    cat "$OUTFILE.scc"
done
