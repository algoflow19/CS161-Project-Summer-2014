all: scc rg np

scc: scc_finder.cc
	g++ -std=c++11 -march=native -Wall -O4 scc_finder.cc -o sccfinder
#	g++ -std=c++11 -march=native -Wall -ggdb scc_finder.cc -o sccfinder

rg: random_graph.cc
	g++ -std=c++11 -march=native -Wall -O4 random_graph.cc -o randomgraph

np: network_properties.cc
	g++ -std=c++11 -march=native -Wall -O4 network_properties.cc -o networkproperties
#	g++ -std=c++11 -march=native -Wall -ggdb network_properties.cc -o networkproperties

clean:
	rm -f sccfinder randomgraph networkproperties
