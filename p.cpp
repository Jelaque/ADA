#include<iostream>
#include"mergeHibrid.h"
#include <stdlib.h>
#include <time.h>
using namespace std;
main(){
	double *A;
	long n = 2000;
  A = new double[n];
	for (int i = 0; i < n; ++i)
		A[i] = rand() % n;
	mergeHibrid(A,0,n,0);
	for (int i = 0; i < n; ++i)
    cout << A[i] << " ";
	delete[] A;
	return 0;
}
