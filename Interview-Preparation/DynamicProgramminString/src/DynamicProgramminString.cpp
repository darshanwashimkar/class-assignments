//============================================================================
// Name        : DynamicProgramminString.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

int min(int x, int y, int z){
	int min = x;
	if(y<x)
		min = y;
	if(z<min)
		min = z;
	return(min);
}

int editDistance(string S1, string S2){
	int penalty = 1;
	int m = S1.size()+1;
	int n = S2.size()+1;
	int *D = new int[n * m];
	for(int i = 0; i < m; i++)
		D[i] = i;

	for(int i = 0; i < n; i++)
		D[i*m] = i;

	for(int i = 1; i < n; i++){
		for(int j = 1; j < m; j++){
			int topCell = D[(i-1)*m + j];
			topCell+= penalty;
			int leftCell = D[(i*m) + (j-1)];
			leftCell += penalty;
			int topLeft = D[(i-1)*m + (j-1)];
			if(S1[j-1] != S2[i-1]){
				topLeft+= penalty;
			}
			D[(i*m)+ j] = min(topCell, leftCell, topLeft);
		}
	}


	for(int i = 0; i < m*n; i++){
		if(i%m==0)
			cout<<std::endl;
		cout<<D[i];
	}

	return(D[n*m - 1]);
}

int main() {
	cout << " "<<editDistance("SUNDAY", "SATURDAY");
	return 0;
}
