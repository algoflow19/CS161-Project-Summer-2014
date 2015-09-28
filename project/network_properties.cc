/*
 * network_properties.cc
 *
 * Created on: Jul 27, 2014
 * Author: Alextair Mascarenhas
 */

#include "floydwarshall.h"
#include "graph.h"
#include "sparsematrix.h"

int main(int argc, char **argv) {

	assert(argc >= 2);
	double samplingPercent = 0;
	cout << "\n\ninput file: " << argv[1] << endl;
	if (argc > 2){
		samplingPercent = atoi(argv[2]);
		cout << "\nsampling % = " << samplingPercent << endl;
	}

	// build graph & calc distances
	shared_ptr<Graph> G = Graph::buildGraph(argv[1]);

	// avg in degree
	double avgInDeg = G->avgInDegree();
	cout << "\navg in degree = " << avgInDeg << endl;

	// avg out degree
	double avgOutDeg = G->avgOutDegree();
	cout << "\navg out degree = " << avgOutDeg << endl;

	// sample
	G->sample(samplingPercent);
	unsigned n = G->nVertices();
	cout << "\nn = " << n << endl;

	// floyd warshall all pairs distances
	shared_ptr<SparseMatrix> distances = make_shared<SparseMatrix>(n, n);
	floydWarshall(*distances, *G);

	// network diameter
	double networkDiameter = calcDistanceProperty(*distances, DistanceMetric::MAX);
	cout << "\n\nnetwork diameter = " << networkDiameter << endl;

	// avg path length
	double avgPathLength = calcDistanceProperty(*distances, DistanceMetric::AVG);
	cout << "\naverage path length = " << avgPathLength << endl;

    return 0;
}


