//============================================================================
// Name        : RotateArrayN.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

void swap(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

void reverseArray(int A[], int sI, int eI){
	int j = eI;
	for(int i = sI; i < ((sI+eI)/2); i++){
		swap(A[i], A[j]);
		j--;
	}
}

void printArray(int A[], int size){
	cout<<endl;
	for(int i = 0; i < size; i++)
		cout<<A[i]<<" ";
}

void rotateArrayN(int A[], int size, int d){
	reverseArray(A, 0, size-1);
	printArray(A, 10);
	reverseArray(A, 0, d-1);
	printArray(A, 10);
	reverseArray(A, d, size-1);
	printArray(A, 10);
}

int main() {
	int A[] = {0,1,2,3,4,5,6,7,8,9};
	//reverseArray(A, 0, 4);
	rotateArrayN(A, 10, 3);
	printArray(A, 10);
	return 0;
}
