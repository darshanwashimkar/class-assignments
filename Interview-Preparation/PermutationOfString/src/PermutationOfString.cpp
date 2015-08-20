//============================================================================
// Name        : PermutationOfString.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
using namespace std;

void swap(char * a, char * b){
	char temp = *a;
	*a = *b;
	*b = temp;
}

void printPermutations(char *str, int l, int r){
	if(l==r){
		std::cout<<str<<endl;
		return;
	}

	for(int i = l; i <= r; i++){
		swap((str+l), (str+i));
		printPermutations(str, l+1, r);
		swap((str+l),(str+i));
	}
}

int main() {
	char S[] ="ABC";
	printPermutations(S, 0, strlen(S)-1);
	return 0;
}
