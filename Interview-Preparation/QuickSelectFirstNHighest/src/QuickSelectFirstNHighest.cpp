//============================================================================
// Name        : QuickSelectFirstNHighest.cpp
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


int findNthHighest(int arr[], int l, int h, int n){

		if(((n-1) > h) || ((n-1) < l))
			return(-1);

		int index = partition(arr, l, h);

		if(index == (n-1)){
			return(arr[index]);
		}
		else if((n-1) < index){
			return(findNthHighest(arr, l, index-1, n));
		}
		else
			return(findNthHighest(arr, index+1, h, n));
}


int main() {
	cout << "!!! N Highest Numbers!!!" << endl; // prints !!! N Highest Numbers!!!
	int arr[] = {2, 6, 1, 7, 3, 9, 4, 2, 2, 11};
	cout<<"Answers is: "<<findNthHighest(arr, 0, 9, 5)<<endl;
	printArray(arr, 9);
	return 0;
}
