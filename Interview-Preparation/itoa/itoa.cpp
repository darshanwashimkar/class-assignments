#include<iostream>
#include<sstream>
#include<stdio.h>
#include <climits>

using namespace std;

const char * itos(int i){
	stringstream ss;
	ss << i;
	return(ss.str().c_str());
}

int atoinew(int i, char* s, unsigned int size){
	if(snprintf(s,size, "%d",i)<0){
		return(-1);
	}
	return(0);
}


std::string itoa(int num){
	//char *str = (char*)malloc(sizeof(char)*7);
	std::string str;
	bool sign = false;

	if(num==0){
		str = "0";
		return(str);
	}
	else if(num < 0){
		sign = true;
		num = -(num);
	}

	while(num){
		str = (char)(num%10 + 48) + str;
		num = num/10;
	}
	if(sign){
		str = "-" + str;
	}
	return str;
}

int main(){
	cout<<"asds";
	std::string str = itoa(-82212);
	cout<<" "<<str<<"  "<<INT_MIN;
	return(0);
}
