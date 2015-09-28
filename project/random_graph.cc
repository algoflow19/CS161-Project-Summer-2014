#include <assert.h>
#include <cstdlib>
#include <random>
#include <iostream>
#include <fstream>

using namespace std;

/*
 * Main takes three arguments:
 * 1). number of nodes
 * 2). Number of edges
 * 3). output file
 * Inspired by: http://stackoverflow.com/questions/7114043/random-number-generation-in-c11-how-to-generate-how-do-they-work
 */
int main(int argc, char **argv) {
	assert(argc == 4);
	unsigned n = strtoul(argv[1], NULL, 0);
	unsigned m = strtoul(argv[2], NULL, 0);
	char *output_file = argv[3];

	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> custom_dist(1, n);
	ofstream os;
	os.open(output_file);
	os << n << endl << m;
	for (unsigned i = 0; i != m; ++i){
		os << "\n" << custom_dist(rng) << " " << custom_dist(rng);
	}
	os.close();

	return 0;
}
