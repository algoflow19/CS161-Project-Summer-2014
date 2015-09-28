/*
 * graph.h
 *
 *  Created on: Jul 25, 2014
 *  Author: Alextair Mascarenhas
 *  Inspired by: http://stackoverflow.com/questions/14133115/implementation-of-an-adjacency-list-graph-representation
 */

#ifndef GRAPH_H_
#define GRAPH_H_

//#include <tr1/unordered_map>
//#include <tr1/unordered_set>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Vertex;

struct Vertex {
	unsigned m_id;
	mutable unsigned m_index;
	mutable unsigned m_lowlink;
	// for Dijkstra's Algorithm
	unsigned m_distance;
	Vertex * m_previous;


	explicit Vertex(unsigned i) : m_id(i), m_previous(NULL){
        	m_index = 0;
        	m_lowlink = 0;
	}

	bool operator==(const Vertex & rhs) const {
		return m_id == rhs.m_id;
	}

	bool operator<(const Vertex & rhs) const {
		return m_id < rhs.m_id;
	}

	Vertex():m_id(0), m_index(0),m_lowlink(0), m_distance(0), m_previous(NULL){}
};
//typedef std::tr1::unordered_map<Vertex, std::unordered_set<Vertex> > AdjacencyList;
typedef map<Vertex, std::set<Vertex> > AdjacencyList;
typedef std::vector<Vertex> Vertices;
typedef std::unordered_map<unsigned, std::set<unsigned> > Edges;

class Graph {
private:
	AdjacencyList m_adjList;
	unsigned m_nVertices;
	unsigned m_nEdges;
	string m_fileName;
   	// SAR
   	Vertices gVertices;
   	Edges gEdges;
	std::vector<int> sccs;
	std::deque<unsigned> verticeStack;
	unsigned index;
public:
	Graph() : m_nVertices(0), m_nEdges(0), index(1) { gVertices.push_back(Vertex()); }

	Graph(const Graph & other){
		m_fileName = other.m_fileName;
		m_nVertices = other.m_nVertices;
		m_nEdges = other.m_nEdges;
        index = 1; 
//        gVertices(other.gVertices);
	}

	Graph(const string fileName, unsigned n, unsigned m){
		m_fileName = fileName;
		m_nVertices = n;
		m_nEdges = m;
        index = 1;
        gVertices.push_back(Vertex());
	}

	void findsccs() {
//        unsigned size = gVertices.size();
//        if (m_nVertices == 0 || size == 0 || size == 1)
//            return;
		for (unsigned i = 1; i <= m_nVertices; ++i){
			if (!(gVertices[i].m_index))
				strongConnect(i);
		}
	}

	void sortandgetsccs(array<unsigned, 5>&largest_sccs) {
		std::sort(sccs.begin(), sccs.end(), std::greater<int>());
		for (unsigned i = 0; i < largest_sccs.size(); i++) {
			largest_sccs[i] = sccs[i];
		}
	}

	void strongConnect(unsigned i) {
		gVertices[i].m_index = index;
		gVertices[i].m_lowlink = index;
		index += 1;
		verticeStack.push_front(gVertices[i].m_id);
		Edges::iterator g = gEdges.find(gVertices[i].m_id);
		if (g != gEdges.end()) {
			for (set<unsigned>::iterator h = g->second.begin(); h != g->second.end(); ++h) {
//				Vertices::iterator f = gVertices.find(*h);
				if(!(gVertices[*h].m_index)) {
					strongConnect(*h);
					gVertices[i].m_lowlink = (gVertices[i].m_lowlink < gVertices[*h].m_lowlink?gVertices[i].m_lowlink:gVertices[*h].m_lowlink);
				} else if (std::find(verticeStack.begin(), verticeStack.end(), *h) != verticeStack.end()) {
					gVertices[i].m_lowlink = (gVertices[i].m_lowlink < gVertices[*h].m_lowlink?gVertices[i].m_lowlink:gVertices[*h].m_lowlink);
				}
			}
		}

		if (gVertices[i].m_lowlink == gVertices[i].m_index) {
			unsigned count = 0;
			unsigned node = 0;
			do {
				node = verticeStack.at(0);
				verticeStack.pop_front();
				count++;
			} while (node != gVertices[i].m_id);
			sccs.push_back(count);
		}
	}

	unsigned nVertices() const {
		return m_nVertices;
	}

	unsigned nEdges() const {
		return m_nEdges;
	}

	void addEdge(const Vertex & u, const Vertex & v){
		// ensure u is a key in m_adjList
		AdjacencyList::iterator i = m_adjList.find(u);
		if (i == m_adjList.end()){
			set<Vertex> s;
			s.insert(v);
			m_adjList[u] = s;
		} else {
			i->second.insert(v);
		}

		// ensure v is a key in m_adjList
		AdjacencyList::iterator j = m_adjList.find(v);
		if (j == m_adjList.end()) {
			set<Vertex> s;
			m_adjList[v] = s;
		}
	}

	// quick and dirty!
	// saves us the trouble of building out separate functions
	AdjacencyList & getAdjacencyList() {
		return m_adjList;
	}

	void addRawVertices() {
		for (unsigned i=1; i<=m_nVertices; i++) {
			gVertices.push_back(Vertex(i));
		}
	}

	void addRawEdges(unsigned u, unsigned v) {
		Edges::iterator g = gEdges.find(u);
		if (g == gEdges.end()) {
			set<unsigned> s;
			s.insert(v);
			gEdges[u] = s;
		} else {
			g->second.insert(v);
		}
	}

	void dump() const {
		cout << "\n\n" << m_fileName << "\nn = " << m_nVertices << ", m = " << m_nEdges;
		for (AdjacencyList::const_iterator i = m_adjList.begin(); i != m_adjList.end(); ++i){
			cout << "\n" << i->first.m_id << " -> {";
			for(set<Vertex>::const_iterator j = i->second.begin(); j != i->second.end(); ++j){
				if (j != i->second.begin()){
					cout << ", ";
				}
				cout << j->m_id;
			}
			cout << "}";
		}
        cout << endl;
	}

	void rawDump() const {
		cout << "\n\n" << m_fileName << "\nn = " << m_nVertices << ", m = " << m_nEdges << ", Vcount = " << (gVertices.size()-1) << ", Ecount = " << gEdges.size();
		for (Edges::const_iterator g = gEdges.begin(); g != gEdges.end(); ++g) {
			cout << "\n" << g->first << " -> {";
			for (set<unsigned>::const_iterator h = g->second.begin(); h != g->second.end(); ++h) {
				if (h != g->second.begin()) {
					cout << ", ";
				}
				cout << *h;
			}
			cout << "}";
		}
		cout << endl;
	}
};

#endif /* GRAPH_H_ */
