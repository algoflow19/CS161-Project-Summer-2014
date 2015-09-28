/*
 * floydwarshall.h
 *
 * Created on: Jul 27, 2014
 * Author: Alextair Mascarenhas
 * From: http://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
 */


#ifndef FLOYDWARSHALL_H_
#define FLOYDWARSHALL_H_

#include <vector>
#include <climits>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <unordered_set>
#include <unordered_map>
#include "graph.h"
#include "sparsematrix.h"


void dumpMatrix(const SparseMatrix & m){
	unsigned n = m.n();
	for(unsigned i = 0; i != n; ++i){
		cout << endl;
		for(unsigned j = 0; j != n; ++j){
			unsigned val = m.get(i, j);
			if (val == SparseMatrix::DefaultValue){
				cout << " -";
			} else {
				cout << " " << val;
			}
		}
	}
}

enum DistanceMetric {MAX, AVG};

double calcDistanceProperty(const SparseMatrix & distances, DistanceMetric dm){

	cout << "\ncalc dist props" << flush;
	unsigned n = distances.n();
	vector<unsigned> validDistances;
	for (unsigned i = 0; i != n; ++i) {
		for (unsigned j = 0; j != n; ++j) {
			unsigned val = distances.get(i, j);
			if (val != 0 && val != SparseMatrix::DefaultValue){
				validDistances.push_back(val);
			}
		}
	}
	assert(validDistances.size() > 0);

	switch(dm){
	case MAX:
	{
		vector<unsigned>::const_iterator me = max_element(validDistances.begin(), validDistances.end());
		return *me;
	}
	case AVG:
	{
		double sum = accumulate(validDistances.begin(), validDistances.end(), 0);
		return sum / validDistances.size();
	}
	default:
	{
		assert(false);
		break;
	}
	}

	return 0;
}

typedef unordered_map<unsigned, unsigned> IDMap;

void floydWarshall(SparseMatrix & distances, Graph & G){
	unsigned n = distances.n();

	// setup ID maps
	pair<Vertices::iterator, Vertices::iterator> iterators = G.getVertices();
	IDMap newToOldID;
	unsigned i = 0;
	for(Vertices::iterator v = iterators.first; v != iterators.second; ++v){
		newToOldID[i] = v->m_id;
		++i;
	}

	// initialize
	for (unsigned i = 0; i != n; ++i) {
		for (unsigned j = 0; j != n; ++j) {
			if (i == j) {
				distances.set(i, i, 0);
			} else if (G.hasEdge(newToOldID[i], newToOldID[j])) {
				distances.set(i, j, 1);
			}
		}
	}

	//dumpMatrix(distances);

	// calc distances
	for (unsigned k = 0; k != n; ++k) {
		cout << " " << k << flush;
		for (unsigned i = 0; i != n; ++i) {
			for (unsigned j = 0; j != n; ++j) {
				unsigned total = distances.get(i, k) + distances.get(k, j);
				if (distances.get(i, j) > total) {
					distances.set(i, j, total);
				}
			}
		}
	}
}


#endif /* FLOYDWARSHALL_H_ */
