/*
 * roundrobin.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: Darshan
 */
#include<iostream>

using namespace std;

int RR(string s1, string s2){
	char c;
	if(s1.size()!=s2.size()){
		return(-1);
	}

	int j=0;
	int i = 0;
	int z = 0;

	while(i<s1.size() && z < (s1.size()*2)){
		if(s1[i] == s2[j]){
			i++;
		}
		j++;
		z++;
		if(j >= s1.size()){
			j = 0;
		}
	}
	if(s1.size() <= i){
		return(1);
	}
	cout<<i<<" "<<j<<" "<<z<<endl;
	return(-1);
}

int rotation(string s1, string s2){
	string S = s1 + s1;
	if(S.find(s2) == string::npos){
		cout<<"ssssssssssssss"<<endl;
	}
	return(1);
}

int main(int argv, char* args[]){
	string s1 = "darshan";
	string s2 = "shazdar";
	//cout<<RR(s1, s2)<<endl;
	cout<<rotation(s1, s2)<<endl;
	return(0);
}
