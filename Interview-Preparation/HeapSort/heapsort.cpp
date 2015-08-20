/*
 * heapsort.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: darshan
 */


#include<iostream>

#define L(x) 2*(x+1) - 1
#define R(x) (2*(x+1))
#define SIZE(A) sizeof(A)/sizeof(A[0])
#define swap(i,j) i+=j;j=i-j; i=i-j;

using namespace std;

void heapify(int A[], int i, int size){
	int l =  L(i);
	int r =  R(i);
	int large = i;

	if((l<size) && A[l] > A[i]){
		large = l;
	}
	if((r<size) && A[r]> A[large]){
		large = r;
	}
	if(i!=large){

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

void buildHeap(int A[], int size){
	for(int i = ((size/2)-1); i >= 0; i--){
		heapify(&A[0], i, size);
	}
}

void mergeSort(int A[], int size){
	buildHeap(&A[0], size);
	printarray(&A[0], size);
	cout<<endl<<"------------------------";
	for(int i = size -1; i > 0; i-- ){
		swap(A[0], A[i]);
		size = size - 1;
		heapify(&A[0], 0, size);
		printarray(&A[0], size);
	}
	cout<<endl<<"------------------------";
}




int main(){

	int array[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};

	for(int i=0; i < SIZE(array); i++){
		std::cout<<array[i]<<" ";
	}

	//heapify(&array2[0], 0, SIZE(array)-1);
	 //buildHeap(&array[0], SIZE(array));
	mergeSort(&array[0], SIZE(array));
	printarray(&array[0], 10);

	return(0);
}
