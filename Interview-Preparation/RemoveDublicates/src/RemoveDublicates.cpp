//============================================================================
// Name        : RemoveDublicates.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <map>
#include <vector>

using namespace std;

void removeDublicates(int array[], int n){
	std::map<int, int> HM;
	for(int i = 0; i < n; i++){
		std::pair<std::map<int,int>::iterator, bool> rVal;
		rVal = HM.insert(std::make_pair<int, int>(array[i], 1));
		if(rVal.second == false){
			cout<<i<<"  ";
		}
	}
}

int main() {
	int A[] = {12,21,11,12,12,21};
	removeDublicates(A, 6);
	cout<<"----------"<<endl;
	return 0;
}
