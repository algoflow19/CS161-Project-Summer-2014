/*
 * sparse_matrix.h
 *
 * Created on: Aug 5, 2014
 * Author: Alextair Mascarenhas
 */

#ifndef SPARSE_MATRIX_H_
#define SPARSE_MATRIX_H_

#include <unordered_map>
using namespace std;


typedef unordered_map<unsigned, unordered_map<unsigned, unsigned> > MapBasedMatrix;

class SparseMatrix {
private:
	MapBasedMatrix m_matrix;
	unsigned m_n;
	unsigned m_m;

public:
	static const int DefaultValue = INT_MAX;

	SparseMatrix(unsigned n, unsigned m) : m_n(n), m_m(m) {}

	unsigned get(unsigned i, unsigned j) const {
		MapBasedMatrix::const_iterator r = m_matrix.find(i);
		if (r != m_matrix.end()){
			unordered_map<unsigned, unsigned>::const_iterator c = r->second.find(j);
			if (c != r->second.end()){
				return c->second;
			}
		}
		return DefaultValue;
	}

	void set(unsigned i, unsigned j, unsigned val){
		MapBasedMatrix::const_iterator r = m_matrix.find(i);
		if (r == m_matrix.end()){
			m_matrix[i] = unordered_map<unsigned, unsigned>();
		}
		m_matrix[i][j] = val;
	}

    pair<MapBasedMatrix::iterator, MapBasedMatrix::iterator> getRows(){
        return make_pair(m_matrix.begin(), m_matrix.end());
    } 

	unsigned n() const {return m_n;}
	unsigned m() const {return m_m;}
};


#endif /* SPARSE_MATRIX_H_ */
