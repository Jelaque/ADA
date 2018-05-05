#include<iostream>

typedef int c_arr;

void giuliana(c_arr *arr, c_arr len){
	c_arr max, ac, nac, f, l;
	max = l = f = ac = nac = 0;
	for(int i = 0;i < len; ++i){
		std::cout << max << std::endl;
		if(arr[i] > 0)
			ac += arr[i];
		else{
			nac += arr[i];
			if(nac + ac > 0)
				ac += nac;
			else{
				//l = i - 1;
				ac = nac = 0;
			}				
		}
		if (ac > max)
			max = ac;
	}
	std::cout << "Max route: " << max << std::endl;
}

main(){
	c_arr n = 7;
	c_arr ar[] = {6,-7,8,-2,7,-10,4};
	giuliana(ar, n);
	return 0;
}
