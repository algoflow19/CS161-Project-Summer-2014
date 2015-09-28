#!/bin/sh

BENCHMARK_DIR=/usr/class/cs161/project/tests/correct_output/

# build
rm -f data/network*.scc
make clean
make

# find SCCs
./sccfinder data/network1.txt data/network1.txt.scc
./sccfinder data/network2.txt data/network2.txt.scc
./sccfinder data/network3.txt data/network3.txt.scc
./sccfinder data/network4.txt data/network4.txt.scc
./sccfinder data/network5.txt data/network5.txt.scc
./sccfinder data/network6.txt data/network6.txt.scc
./sccfinder data/network7.txt data/network7.txt.scc
./sccfinder data/network8.txt data/network8.txt.scc
./sccfinder data/network9.txt data/network9.txt.scc
./sccfinder data/network10.txt data/network10.txt.scc

# diff against benchmark
diff $BENCHMARK_DIR/network1_output.txt data/network1.txt.scc
diff $BENCHMARK_DIR/network2_output.txt data/network2.txt.scc
diff $BENCHMARK_DIR/network3_output.txt data/network3.txt.scc
diff $BENCHMARK_DIR/network4_output.txt data/network4.txt.scc
diff $BENCHMARK_DIR/network5_output.txt data/network5.txt.scc
diff $BENCHMARK_DIR/network6_output.txt data/network6.txt.scc
diff $BENCHMARK_DIR/network7_output.txt data/network7.txt.scc
diff $BENCHMARK_DIR/network8_output.txt data/network8.txt.scc
diff $BENCHMARK_DIR/network9_output.txt data/network9.txt.scc
diff $BENCHMARK_DIR/network10_output.txt data/network10.txt.scc
