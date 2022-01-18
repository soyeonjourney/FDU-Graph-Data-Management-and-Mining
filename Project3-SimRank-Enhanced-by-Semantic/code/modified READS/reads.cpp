#include "reads.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include "sparsehash/dense_hash_map"
using google::dense_hash_map;
using namespace std;

#define DOUBLE 1

reads::reads(char * gName_, int n_, int r_, double c_, int t_) {
	sprintf(gName, "%s", gName_);
	n = n_, r = r_, c = c_, t = t_;
	t1 = t2 = qCnt = 0;

	char iName[125];
	sprintf(iName, "%s.reads.%d_%d_%lf_%d", gName, n, r, c, t);


	FILE * fg = fopen(gName, "r");
	if (fg == NULL) {
		printf("No graph %s\n", gName);
		exit(0);
	}

	Timer tm;
	ef.resize(n);
	eb.resize(n);
	wei.resize(n);
	float w;

	for (int x, y; fscanf(fg, "%d%d%f", &x, &y, &w) != EOF; ) {
		ef[x].push_back(y);
		eb[y].push_back(x);
		wei[y].push_back(w);
		
#if DOUBLE
		ef[y].push_back(x);
		eb[x].push_back(y);
		wei[x].push_back(w);
#endif
	}

	for (int i = 0; i < n; i++)
	{
		float sum = 0.0;
		for (int j = 0; j < wei[i].size(); j++)
		{
			sum += wei[i][j];
			if (j)
			{
				wei[i][j] = sum;
			}
		}
		
		for (int j = 0; j < wei[i].size(); j++)
		{
			wei[i][j] /= sum;
		}

	}

//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < wei[i].size(); j++)
//		{
//			printf("%8f", wei[i][j]);
//		}
//		printf("\n");
//	}

	rtime = tm.getTime();

	fclose(fg);

	int q0, q1, q2;
	vector<pair<int, int> > q;
	vector<int> pos;
	int cc = int(RAND_MAX * sqrt(c));
	vector<int> sta, tmp(n);
	int tmpCnt;


	nt.resize(r);
	for (int i = 0; i < r; i++)
		nt[i].resize(n);

	for (int i = 0; i < r; i++) {
		q.resize(0);
		pos.assign(n, -1);

		for (int j = 0, p; j < n; j++)
			if (!eb[j].empty()) {
				float rr = (double) rand() / (double) RAND_MAX;
				int count = 0;
				for (; count < eb[j].size(); count++)
				{
					if (rr <= wei[j][count])
					{
						break;
					}
				}
				if (pos[p = eb[j][count]] < 0) {
					pos[p] = q.size();
					q.push_back(make_pair(p, -1));
				}
				nt[i][j] = pos[p];
			} else nt[i][j] = -1;

		q0 = 0; q1 = q.size();
		for (int j = 0, p, rr; j < t - 1 && q0 < q1; j++) {
			int tt;
			for (; q0 < q1; q0++) {
				if (!eb[q[q0].first].empty() && ( (rr = rand()) < cc) ) {
					float pivot = (double) rand() / (double) RAND_MAX;
					int count = 0;
					for (; count < eb[q[q0].first].size(); count++)
					{
						if (pivot <= wei[q[q0].first][count])
						{
							break;
						}
					}

					p = eb[q[q0].first][count];
					if (pos[p] < q1) {
						pos[p] = q.size();
						q.push_back(make_pair(p, -1));
					}
					q[q0].second = pos[p];
				}
			}
			q1 = q.size();
		}

		tmpCnt = 0;
		for (int j = 0, qid; j < n; j++)
			if (nt[i][j] > -1) {
				for (sta.resize(1), sta[0] = nt[i][j]; q[*sta.rbegin()].second >= 0;)
					sta.push_back(q[*sta.rbegin()].second);

				if ((qid = - q[*sta.rbegin()].second - 2) < 0) {
					q[*sta.rbegin()].second = - tmpCnt - 2;
					tmp[tmpCnt] = pos[tmpCnt] = j;
					tmpCnt++;
				} else {
					nt[i][tmp[qid]] = j;
					tmp[qid] = j;
				}
				for (int k = sta.size() - 2; k >= 0; k--)
					q[sta[k]].second = q[*sta.rbegin()].second;
			}

		for (int j = 0; j < tmpCnt; j++)
			nt[i][tmp[j]] = pos[j];

	}

//	for (int i = 0; i < n; i++) {
//		random_shuffle(ef[i].begin(), ef[i].end());
//		random_shuffle(eb[i].begin(), eb[i].end());
//	}

	return;

}


reads::~reads() {
}


void reads::queryAll(int x, double * ansVal) {

	memset(ansVal, 0, sizeof(double)*n);
	if (eb[x].empty()) {
		ansVal[x] = 1;
		return;
	}
	double cc = c / r;
	for (int i = 0; i < r; i++)
	{
		int k = nt[i][x];
		while (k != x)
		{
			ansVal[k] += cc;
			k = nt[i][k];
		}
	}
	ansVal[x] = 1;

	return;
	/*
	if (qCnt >= 20 && t1 < t2 || qCnt < 20 && qCnt % 2 == 0 ) {
		if (qCnt < 20) tm.reset();

		dense_hash_map<int, double> sim1;
		sim1.set_empty_key(-1);

		bool * nID = new bool[n];
		memset(nID, 0, sizeof(bool)*n);

		double cc = (1 - c) * r, c1 = r / c;

		int p, q;
		for (auto i = eb[x].begin(); i != eb[x].end(); i++) {
			for (auto j = eb[p = *i].begin(); j != eb[p].end() && j - eb[p].begin() < 10; j++)
				for (auto k = ef[q = *j].begin(); k != ef[q].end() && k - ef[q].begin() < 10; k++)
					if (*k != p)
						sim1[*k] += cc / eb[p].size() / eb[*k].size();
			sim1[*i] += c1;
		}

		for (int i = 0, cnt, q; i < r; i++) {
			for (auto j = eb[x].begin(); j != eb[x].end(); j++)
				nID[*j] = 1;

			for (auto j = eb[x].begin(); j != eb[x].end(); j++)
				if (nID[q = *j] && nt[i][q] != q && nt[i][q] != -1) {
					cnt = 0;

					for (int p = nt[i][q]; p != q; p = nt[i][p]) {
						if (nID[p]) cnt++, nID[p] = 0;
						else sim1[p] += c;
					}

					if (cnt != 0) {
						cc = c * cnt;
						for (int p = nt[i][q]; p != q; p = nt[i][p])
							sim1[p] += cc;
						sim1[q] += cc;
					}
				}

		}



		cc = c * c / eb[x].size() / r;
		for (auto it = sim1.begin(); it != sim1.end(); it++)
			for (auto jt = ef[p = it->first].begin(); jt != ef[p].end(); jt++)
				ansVal[*jt] += it->second * cc / eb[*jt].size();

		delete [] nID;

		if (qCnt++ < 20) t1 += tm.getTime();

	} else {

		if (qCnt < 20) tm.reset();

		double * sim1 = new double[n];
		memset(sim1, 0, sizeof(double)*n);

		bool * nID = new bool[n];
		memset(nID, 0, sizeof(bool)*n);

		double cc = (1 - c) * r, c1 = r / c;


		int p, q;
		for (auto i = eb[x].begin(); i != eb[x].end(); i++) {
			for (auto j = eb[p = *i].begin(); j != eb[p].end() && j - eb[p].begin() < 10; j++)
				for (auto k = ef[q = *j].begin(); k != ef[q].end() && k - ef[q].begin() < 10; k++)
					if (*k != p)
						sim1[*k] += cc / eb[p].size() / eb[*k].size();
			sim1[*i] += c1;
		}




		for (int i = 0, cnt, q; i < r; i++) {
			for (auto j = eb[x].begin(); j != eb[x].end(); j++)
				nID[*j] = 1;

			for (auto j = eb[x].begin(); j != eb[x].end(); j++)
				if (nID[q = *j] && nt[i][q] != q && nt[i][q] != -1) {
					cnt = 0;

					for (int p = nt[i][q]; p != q; p = nt[i][p]) {
						if (nID[p]) cnt++, nID[p] = 0;
						else sim1[p] += c;
					}

					if (cnt != 0) {
						cc = c * cnt;
						for (int p = nt[i][q]; p != q; p = nt[i][p])
							sim1[p] += cc;
						sim1[q] += cc;
					}
				}

		}


		for (int i = 0; i < n; i++)
			if (sim1[i] > 0)
				for (auto j = ef[i].begin(); j != ef[i].end(); j++)
					ansVal[*j] += sim1[i];

		cc = c * c / r / eb[x].size();
		for (int i = 0; i < n; i++)
			if (!eb[i].empty())
				ansVal[i] = ansVal[i] * cc / eb[i].size();

		delete [] nID;

		if (qCnt++ < 20) t2 += tm.getTime();


	}
	*/
}




