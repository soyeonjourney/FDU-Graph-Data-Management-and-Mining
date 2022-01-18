#ifndef __READSD_H__
#define __READSD_H__
#include <array>
#include "sparsehash/sparse_hash_map"
#include "timer.h"
using namespace std;
using google::sparse_hash_map;

struct readsd {
// data:
	double c;
	int r, n, t, qCnt;
	double rtime, t1, t2;
	Timer tm;

	vector<array<int, 3> > *leaf;
	vector<sparse_hash_map<int, array<int, 3> > > *inode;

	vector<vector<int> > ef, eb;


	char gName[125];

// method:
	readsd(char * gName_, int n_, int r_, double c_, int t_);
	~readsd();
	void queryAll(int x, double * ansVal);
	void insEdge(int x, int y);
	void delEdge(int x, int y);

};


#endif


