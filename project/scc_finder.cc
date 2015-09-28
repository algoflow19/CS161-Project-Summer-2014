#include <assert.h>
#include <array>
#include <string>
#include <memory>
#include "graph.h"

using namespace std;

// prototypes.
void find_sccs(char *input_file, array<unsigned, 5> &largest_sccs);
void print_sccs(char *output_file, array<unsigned, 5> &scc_sizes);

/**
 * Given an input file (input_file) and an array (largest_sccs) of size 5,
 * fills the 5 largest scc sizes into (largest_sccs) in decreasing order.
 * in decreasing order. In the case when there are less than
 * 5 components, you should fill 0 for the remaining values in (largest_sccs)
 * (i.e if there are only 2 components of size 100 and 50, you should fill:
 * largest_sccs[0] = 100
 * largest_sccs[1] = 50
 * largest_sccs[2] = 0
 * largest_sccs[3] = 0
 * largest_sccs[4] = 0
 */
void find_sccs(char *input_file, array<unsigned, 5> &largest_sccs) {
	shared_ptr<Graph> G = Graph::buildGraph(input_file);
	G->findsccs();
	G->sortandgetsccs(largest_sccs);
    cout << "\n Triangles :" << G->getTriangles() << endl;

	// Tarjan's algorithm is supposedly faster than Kosaraju's
	// http://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
}

/* print the output in the format we want. don't change this */
void print_sccs(char *output_file, array<unsigned, 5> &scc_sizes) {
	ofstream os;
	os.open(output_file);
	for (unsigned i = 0; i < scc_sizes.size(); i++) {
		os << scc_sizes[i];
		if (i < 4)
			os << "\t";
	}
	os.close();
}

/*
 * Main takes two arguments: 1) input file and 2) output file.
 * You should fill the find_sccs function.
 */
int main(int argc, char **argv) {
	assert(argc == 3);

	cout << "\n\ninput file: " << argv[1];
	cout << "\noutput file: " << argv[2];
    cout << "\n\n";

	array<unsigned, 5> scc_sizes;
	find_sccs(argv[1], scc_sizes);

	// Output the first 5 sccs into a file.
	print_sccs(argv[2], scc_sizes);
	return 0;
}
