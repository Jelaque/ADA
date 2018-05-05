#include<iostream>
#include<fstream>
#include"mergeHibrid.h"
#include"merge.h"
#include<cstdlib>
#include<vector>
using namespace std;
typedef double carr;
void genArr(carr *&arr, int n){
	arr = new carr[n];
	for(int i=0; i!=n; i++){
		arr[i] = rand() % n;
	}
}

main(){
	ofstream file;
	file.open("stadistics.csv");
	file << "n data,t1,t2 \n";
	long n = 0;
	double t1= 0, t2 =0;
	time_t time_i, time_f;
	carr *A;
	carr *B;
	//vector<carr> V;
	vector<double> p1,p2;
	for(;n <= 2048; n+=10){
		cout << n << endl;
		for(int i = 0; i < 2000; ++i){
			genArr(A,n);
			double B[n];
			//for(int j = 1; j <= n; ++j)
			//	V.push_back(rand() % n);
			time_i = clock();
			MergeSort(A,B,n);
			time_f = clock();
			t1 = static_cast<double>(time_f - time_i)/CLOCKS_PER_SEC;
			time_i = clock();
			mergeHibrid(B,0,n,0);
    	time_f = clock();
			t2 = static_cast<double>(time_f - time_i)/CLOCKS_PER_SEC;
			p1.push_back(t1);
			p2.push_back(t2);
			delete[] A;
			//V.clear();
			B = NULL;
			A = NULL;
		}
		for(auto it = p1.begin() ; it != p1.end(); ++it)
			t1 += *it;
		for(auto it = p2.begin() ; it != p2.end(); ++it)
      t2 += *it;
		file << n << "," << t1/100 << "," << t2/100 << "\n";
		t1 = t2 = 0;
		p1.clear();
		p2.clear();
	}

	return 0;
}
