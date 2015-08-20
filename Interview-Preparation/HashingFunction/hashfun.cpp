/*
 * hashfun.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: darshan
 */
#include <iostream>

using namespace std;
unsigned long hash(char *);

int main(int argc, char * args[] ){
	cout<<hash("123");
	return(0);
}



unsigned long hash(char *str){
	unsigned long hash = 5381;
	int c;
	while(c = *str++){
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}


unsigned long hasher (char *str){
	unsigned long hash = 5381;
	int c;
	while(c = *str++){
		hash = ((hash << 5) + hash) + c;
	}

	return hash;
}
