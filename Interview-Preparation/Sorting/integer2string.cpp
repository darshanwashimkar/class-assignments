#include<iostream>
#include<stdio.h>
char * itoa(int i) {
	char* s;
	sprintf(s, "%d", i);
	return(s);
}

int main(int argv, char* args[]) {
	int A = 223;
	char* stringA = itoa(A);
	std::cout<<stringA<<"gfhgd"<<std::endl;
	return (1);
}
