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
#include <unordered_set>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <cmath>

using namespace std;

struct Vertex;

struct Vertex {
	unsigned m_id;
	mutable unsigned m_index;
	mutable unsigned m_lowlink;
	// for Dijkstra's Algorithm
	unsigned m_distance;
	Vertex * m_previous;
    unsigned caller;
    unsigned neighbor;


	explicit Vertex(unsigned i) : m_id(i), m_previous(NULL){
        	m_index = 0;
        	m_lowlink = 0;
            caller = 0;
            neighbor = 0;
	}

	bool operator==(const Vertex & rhs) const {
		return m_id == rhs.m_id;
	}

	bool operator<(const Vertex & rhs) const {
		return m_id < rhs.m_id;
	}

	Vertex():m_id(0), m_index(0),m_lowlink(0), m_distance(0), m_previous(NULL), caller(0), neighbor(0){}
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
	std::deque<unsigned> verticeStack;
	unsigned index;
    unsigned *sortedsccs;
    unsigned triangles;
public:
	Graph() : m_nVertices(0), m_nEdges(0), index(1) { gVertices.push_back(Vertex()); }

	Graph(const Graph & other){
		m_fileName = other.m_fileName;
		m_nVertices = other.m_nVertices;
		m_nEdges = other.m_nEdges;
        index = 1; 
    //    gVertices = other.gVertices;
	}

	Graph(const string fileName, unsigned n, unsigned m){
		m_fileName = fileName;
		m_nVertices = n;
		m_nEdges = m;
        index = 1;
     //   gVertices.insert(gVertices.begin(), Vertex(0));
	}

	pair<Vertices::iterator, Vertices::iterator> getVertices(){
		return make_pair(gVertices.begin(), gVertices.end());
	}

	void sample(double samplePercentage) {
		// gen random vertices to preserve
		mt19937 rng;
		rng.seed(std::random_device()());
		uniform_int_distribution < mt19937::result_type > dist(0, m_nVertices-1);
		unsigned nDiscardVertices = (unsigned)ceil(m_nVertices*(100.0 - samplePercentage)/100.0);
		set<unsigned> verticesToDiscard;
		for(unsigned i = 0; i < nDiscardVertices; ++i){
			while(!verticesToDiscard.insert(dist(rng)).second);
		}

		// edge: prune target nodes
		for(Edges::iterator e = gEdges.begin(); e != gEdges.end(); ++e){
			vector<unsigned> survivors;
			set_difference(e->second.begin(), e->second.end(),
					verticesToDiscard.begin(), verticesToDiscard.end(),
					back_inserter(survivors));
			e->second.clear();
			e->second.insert(survivors.begin(), survivors.end());
		}

		// edge: prune source nodes
		for (set<unsigned>::const_iterator s = verticesToDiscard.begin(); s != verticesToDiscard.end(); ++s){
			Edges::iterator f = gEdges.find(*s);
			if (f != gEdges.end()){
				unsigned nErased = f->second.size();
				m_nEdges -= nErased;
				gEdges.erase(f);
			}
		}

		// prune vertices
		Vertices survivingVertices;
		for(Vertices::const_iterator v = gVertices.begin(); v != gVertices.end(); ++v){
			if (verticesToDiscard.find(v->m_id) == verticesToDiscard.end()){
				survivingVertices.push_back(*v);
			}
		}
		gVertices.swap(survivingVertices);
		m_nVertices = gVertices.size();
	}

    static shared_ptr<Graph> buildGraph(char *input_file){
        cout << "\nbuilding graph";
        ifstream is(input_file);
        string line;

        // get graph dimensions
        getline(is, line);
        unsigned n = atoi(line.c_str());
        getline(is, line);
        unsigned m = atoi(line.c_str());
        //cout << "\nn = " << n << ", m = " << m;
        shared_ptr<Graph> G = make_shared<Graph>(Graph(input_file, n, m)); // allocate on the heap for large graphs

        G->addRawVertices();
        while (getline(is, line)) {
            istringstream iss(line);
            unsigned u, v;
            if (!(iss >> u >> v)) {
                break;
            }
            //cout << "\n" << u << " -> " << v;
            // Remove comment on addEdge if you need it for random Graph
            //		G->addEdge(Vertex(u), Vertex(v));
            G->addRawEdges(u,v);
        }
        return G;
    }

    shared_ptr<Graph> reverse() const {
        shared_ptr<Graph> R = make_shared<Graph>(Graph(*this));
        R->gEdges.clear();    
        for(Edges::const_iterator s = gEdges.begin(); s != gEdges.end(); ++s){
            set<unsigned> ts = s->second;
            for (set<unsigned>::const_iterator t = ts.begin(); t != ts.end(); ++t) {
            	R->addRawEdges(*t, s->first);
            } 
        }
        return R;
    }

    bool hasEdge(unsigned u, unsigned v) const {
         Edges::const_iterator f = gEdges.find(u);
         if (f != gEdges.end()){
             return f->second.find(v) != f->second.end();
         }
         return false;
    }

    void getEdgesStartingFrom (unsigned u, unordered_set<unsigned> & targetVertices) {
        Edges::iterator f = gEdges.find(u);
        if (f != gEdges.end()) {
            targetVertices.insert(f->second.begin(), f->second.end());
        }
    }

	void findsccs() {
        sortedsccs = new unsigned[5];
        for (int i = 0; i < 5; i++) 
            sortedsccs[i] = 0;
        triangles = 0;
        bool *quickcheck =new bool[m_nVertices + 1]();
		for (unsigned i = 1; i <= m_nVertices; ++i){
            if (sortedsccs[4] > (m_nVertices - i + 1))
                break;
			if (!(gVertices[i].m_index))
				strongConnect(i, quickcheck);
		}
        delete[] quickcheck;
	}

    void addsccs(unsigned count) {
        int pos = -1;
        if (count > sortedsccs[0])
            pos = 0;
        else if (count > sortedsccs[1])
            pos = 1;
        else if (count > sortedsccs[2])
            pos = 2;
        else if (count > sortedsccs[3])
            pos = 3;
        else if (count > sortedsccs[4])
            pos = 4;

        if (pos != -1) {
            for (int i = 4; i > pos; i--) {
                sortedsccs[i] = sortedsccs[i-1];
            }
            sortedsccs[pos] = count;
        }

    }

	void sortandgetsccs(array<unsigned, 5>&largest_sccs) {
		for (unsigned i = 0; i < largest_sccs.size(); i++) {
			largest_sccs[i] = sortedsccs[i];
		}
	}

	void strongConnect(unsigned i, bool *quickcheck) {
		gVertices[i].m_index = index;
		gVertices[i].m_lowlink = index;
       	gVertices[i].caller = 0;
		index += 1;
       	unsigned gim_m_id = gVertices[i].m_id;
		verticeStack.push_front(gim_m_id);
       	quickcheck[gim_m_id] = true;
       	gVertices[i].neighbor = 0;
		while(true) {
			Edges::iterator g = gEdges.find(gVertices[i].m_id);
			if (g != gEdges.end() ) {
                std::set<unsigned>::iterator h = g->second.begin();
                std::advance(h , gVertices[i].neighbor); 
                if (h != g->second.end()) {
			    	gVertices[i].neighbor += 1;
			    	if (!(gVertices[*h].m_index)) {
				    	gVertices[*h].caller = i;
				    	gVertices[*h].m_index = index;
  				    	gVertices[*h].m_lowlink = index;
				    	gVertices[*h].neighbor = 0;
				    	unsigned new_m_id = gVertices[*h].m_id;
				    	verticeStack.push_front(new_m_id);
				    	quickcheck[new_m_id] = true;
				    	index += 1;
				    	i=(*h);
				    } else if (quickcheck[*h] == true) {
				    	unsigned gim_lowlink = gVertices[i].m_lowlink;
				    	unsigned ghm_index = gVertices[*h].m_index;
				    	gVertices[i].m_lowlink = (gim_lowlink < ghm_index?gim_lowlink:ghm_index);
				    }
               } else {
                    goto end_of_neighbors;
               } 
			} else {
end_of_neighbors:
				if (gVertices[i].m_lowlink == gVertices[i].m_index) {
					unsigned count = 0;
            		unsigned node = 0;
            		do {
               			node = verticeStack.at(0);
               			verticeStack.pop_front();
               			count++;
                        quickcheck[node] = false; 
            		} while (node != gVertices[i].m_id);
            		if (count == 3)
               			triangles++;
            		addsccs(count);
				}
				unsigned prev = gVertices[i].caller;
				if (prev != 0) {
					unsigned gpm_lowlink = gVertices[prev].m_lowlink;
					unsigned gim_lowlink = gVertices[i].m_lowlink;
					gVertices[prev].m_lowlink = (gpm_lowlink < gim_lowlink)?gpm_lowlink:gim_lowlink;
					i = prev;
				} else {
					break;
				}
			}
		}
	}

   	unsigned getTriangles() const {
        return triangles;
   	}

	unsigned nVertices() const {
		return m_nVertices;
	}

	unsigned nEdges() const {
		return m_nEdges;
	}

	static double avg(const vector<unsigned> & data){
		double sum = accumulate(data.begin(), data.end(), 0);
		return sum / data.size();
	}

	double avgOutDegree() const {
		vector<unsigned> outDegrees;
		for(Edges::const_iterator e = gEdges.begin(); e != gEdges.end(); ++e){
			outDegrees.push_back(e->second.size());
		}

		return avg(outDegrees);
	}

	double avgInDegree() const {
		vector<unsigned> inDegrees;
		shared_ptr<Graph> R = this->reverse();
		for (Edges::const_iterator e = R->gEdges.begin(); e != R->gEdges.end(); ++e) {
			inDegrees.push_back(e->second.size());
		}

		return avg(inDegrees);
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
        	gVertices.reserve(m_nVertices+1);
        	gVertices.insert(gVertices.begin(), Vertex(0));
		for (unsigned i=1; i<=m_nVertices; i++) {
			gVertices.push_back(Vertex(i));
		}
	}

    void getRawVerticesSize() {
        cout << "\n\n" << "Vcount = " << gVertices.size();
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
