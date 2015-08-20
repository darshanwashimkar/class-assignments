//============================================================================
// Name        : Prime.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <math.h>
using namespace std;

bool isPrime(int num){
	int temp = 2;
	while(temp <= (int)sqrt(num)){
		if(num%temp == 0)
			return(false);
		temp++;
	}

	return(true);
}

int main() {
	cout<<isPrime(51)<<"a";
	return 0;
}
