//============================================================================
// Name        : sampleCode.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unordered_map>
using namespace std;

enum States {
	START,
	RUNNING,
	STOP
};

int main() {
	cout << "!!! Largest Sum Contiguous Subarray !!!" << endl; // prints !!! Largest Sum Contiguous Subarray !!!
	std::unordered_map <string, int> map;
	map.insert(std::make_pair<string, int>("abc", 25));
	std::cout<<STOP;
	return 0;
}
