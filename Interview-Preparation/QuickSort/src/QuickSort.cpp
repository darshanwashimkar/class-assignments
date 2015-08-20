//============================================================================
// Name        : QuickSort.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

void printArray(int arr[], int size){
	for(int i = 0; i < size; i++){
		std::cout<<arr[i]<<" ";
	}
	std::cout<<endl;
}

void swaping(int *a, int *b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int arr[], int p, int h){

	int x = arr[p];
	int j = h;
	int i = p;

	do{
		while(arr[i] <= x)
			i++;

		while(arr[j] > x)
			j--;

		if(i<j){
			swaping(&arr[i], &arr[j]);
		}
		else{
			swaping(&arr[p], &arr[j]);
			return(j);
		}
	}
	while(1);
}



void quickSort(int arr[], int l, int h){
	if(l<h){
		int index = partition(arr, l, h);
		quickSort(arr, l, index-1);
		quickSort(arr, index+1, h);
	}
}


int main() {
	cout << "!!! Quick Sort !!!" << endl; // prints !!! Quick Sort !!!
	int arr[] = {2, 6, 1, 7, 3, 9, 4, 10, 42, 11};
	quickSort(arr, 0, 9);
	printArray(arr, 10);
	return 0;
}
