#include<iostream>
typedef double c_arr;
void Intercala(c_arr *&A,int p,int q,int r){
	c_arr B[r-p];
	int i, j;
	for(i; i < q; i++)
		B[i] = A[i];
	std::cout << p << " " << q << " " << r << "\n";
	for(j = q; j < r; j++)
		B[r+q-j-1] = A[j];
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
	//for(int i=p;i<r;++i)
 // std::cout << q << "\n";
}
