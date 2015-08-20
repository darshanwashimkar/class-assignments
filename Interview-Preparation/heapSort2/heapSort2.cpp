/*
 * heapSort2.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: darshan
 */

#include<iostream>

#define L(x) 2*(x+1)-1
#define R(x) 2*(x+1)
#define swap(x,y) x+=y; y=x-y; x=x-y;

using namespace std;

void heapify(int A[], int i, int size){
	int l = L(i);
	int r = R(i);
	int large = i;

	if( l < size && A[l] > A[i]){
		large = l;
	}
	if(r < size && A[r] > A[large]){
		large=r;
	}
	if(large!=i){
		swap(A[i], A[large]);
		heapify(&A[0], large, size);
	}
}

void printarray(int A[], int size){
	cout<<endl;
	for(int i=0; i < size; i++){
			std::cout<<A[i]<<" ";
	}
}

void buildheap(int A[], int size){
	for(int i = (size-1)/2; i >= 0; i--){
		heapify(&A[0], i, size);
	}
}

void heapSort(int A[], int size){
	buildheap(&A[0], size);
	for(int i = (size-1); i > 0; i--){
		swap(A[i], A[0]);
		size=size-1;
		heapify(&A[0], 0, size);
	}
}


int main(){
	int array[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
	for(int i=0; i < 10; i++){
		std::cout<<array[i]<<" ";
	}
	heapSort(&array[0], 10);
	printarray(&array[0], 10);
}
