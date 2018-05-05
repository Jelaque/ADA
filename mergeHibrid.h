#include<cmath>
#include<iostream>
#include<algorithm>
typedef double c_arr;

void InsertSort(c_arr *A,const int p,const int n){
	for(int i = p + 1; i < n; ++i)
		for(int j = i; j > p; --j)
			if(A[j-1] > A[j])
				std::swap(A[j-1],A[j]);
}

void Intercala(c_arr *A,const int p,const int q,const int r){
  c_arr *B = new c_arr[r-p];
  int i = p, j = q;
  for(int k = 0;i < q;++k,++i)
    B[k] = A[i];
  for(int k = r-p-1;j < r;++j,--k)
    B[k] = A[j];
  i = p;
  j = r;
  for(int k = p;k < r; ++k){
    if(B[i] <= B[j]){
      A[k] = B[i];
      ++i;
    }
    else{
      A[k] = B[j];
      --j;
    }
  }
  delete[] B;
}

void mergeHibrid(c_arr *A,const int p,const int r,int q){
	if(r-p < 64)
		InsertSort(A,p,r);
	else{
		if(p<r){
			q = floor((p+r)/2);
			mergeHibrid(A,p,q,q);
			mergeHibrid(A,q,r,q);
			Intercala(A,p,q,r);
		}
	}
}
