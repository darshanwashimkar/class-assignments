//============================================================================
// Name        : Largest Sum Contiguous Subarray.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

inline int max(int a, int b){
	return(a>b?a:b);
}

// Returns the index range having highest largest sume and print largest sum.
// Based on Kadane’s Algorithm

std::pair<int,int> largestSumContiguousSubarray(std::vector<int> &arr){
	if(arr.size()== 0){
		throw "Need at least one number";
	}
	int cur_max = arr[0];
	int max_to_this = arr[0];
	int f = 0, r = 0;

	for(int i = 1; i < arr.size(); i++){
		if(arr[i] > (cur_max+arr[i])){
			cur_max = arr[i];
			r = i;
		}
		else{
			cur_max = cur_max + arr[i];
		}

		if(cur_max >= max_to_this){
			max_to_this = cur_max;
			f = i;
		}
	}
	cout<<"Answer: "<<max_to_this;
	return(std::make_pair<int,int>(r, f));
}

int main() {
	int a[] = {-2,-3,4,-1,-9,-10,5,-3};
	std::vector<int> arr(a, a + sizeof(a) / sizeof(a[0]));
	std::vector<int> arr1;
	cout << "!!! Largest Sum Contiguous Sub Array !!!" << endl;
	std::pair<int,int> z;
	try{
		 z = largestSumContiguousSubarray(arr);
	}
	catch(const char* msg){
		cout<<msg<<endl;
	}

	std::cout<<std::endl<<"Range is: "<<z.first<<" "<<z.second<<std::endl;
	return 0;
}
