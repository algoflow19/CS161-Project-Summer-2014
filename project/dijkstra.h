/*
 * dijkstra.h
 *
 * Created on: Jul 27, 2014
 * Author: Alextair Mascarenhas
 * From: http://en.wikipedia.org/wiki/Dijkstra's_algorithm
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "graph.h"
#include <vector>
#include <climits>
#include <queue>


struct VertexCompare {
  bool operator() (const Vertex & lhs, const Vertex & rhs) const {
    return (lhs.m_distance < rhs.m_distance);
  }
};
typedef priority_queue<Vertex, vector<Vertex>, VertexCompare> VertexQueue;

Vertex dijkstra(Graph G, Vertex & src){
	// initialize
	AdjacencyList gal = G.getAdjacencyList();
	unsigned n = G.nVertices();
	unsigned m = G.nEdges();
	array<unsigned, n> * distance = new array<unsigned, n>();
	VertexQueue * PQ = new VertexQueue();
	for(AdjacencyList::iterator ve = gal.begin(); ve != gal.end(); ++ve){
		Vertex v = ve->first;
		if (v != src){
			distance[v.m_id] = UINT_MAX;
			v.m_previous = NULL;
		}
		PQ->push(v);
	}



	// clean up
	delete distance;
	delete PQ;

	return src;
}



#endif /* DIJKSTRA_H_ */
