#include "reads.h"
#include "readsd.h"
#include "readsrq.h"
#include "timer.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <map>
#include <sys/time.h>
#include <sys/resource.h>
using namespace std;


int main(int argc, char ** argv) {
	int n = atoi(argv[2]);
	int qi = atoi(argv[3]);
	int r = 100;
	double c = 0.6;
	int t = 10;
	int rq = 10;
	double * ansVal = new double[n];
	Timer TIMER;
	double time = 0.0;
	
	reads i1(argv[1], n, r, c, t);
//querying one-to-all simrank value from node qi
	TIMER.reset();
	i1.queryAll(qi, ansVal);	
	time = TIMER.getTime();
	printf("Algorithm i1 running time: %f\n", time);

	FILE *fp1 = fopen("i1.txt", "w");
	fprintf(fp1, "The simrank from node %d:\n", qi);
	for (int i = 0; i < n; i++)
	{
		fprintf(fp1, "%f\n", ansVal[i]);
	}
	fclose(fp1);

//	readsd i2(argv[1], n, r, c, t);
//querying one-to-all simrank value from node o
//	TIMER.reset();
//	i2.queryAll(3133, ansVal);
//	time = TIMER.getTime();
//	printf("Algorithm i2 running time: %f\n", time);
//
//	FILE *fp2 = fopen("i2.txt", "w");
//	for (int i = 0; i < n; i++)
//	{
//		fprintf(fp2, "%f\n", ansVal[i]);
//	}
//	fclose(fp2);
//
//	readsrq i3(argv[1], n, r, rq, c, t);
//querying one-to-all simrank value from node o
//	TIMER.reset();
//	i3.queryAll(3133, ansVal);
//	time = TIMER.getTime();
//	printf("Algorithm i3 running time: %f\n", time);
//
//	FILE *fp3 = fopen("i3.txt", "w");
//	for (int i = 0; i < n; i++)
//	{
//		fprintf(fp3, "%f\n", ansVal[i]);
//	}
//	fclose(fp3);


	delete [] ansVal;

}
