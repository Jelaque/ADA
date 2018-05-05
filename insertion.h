#include<algorithm>
typedef double c_arr;
void InsertSort(c_arr *A,const int p,const int n){
	for(int i = p + 1; i < n; ++i)
		for(int j = i; j > p; --j)
			if(A[j-1] > A[j])
				std::swap(A[j-1],A[j]);
}
