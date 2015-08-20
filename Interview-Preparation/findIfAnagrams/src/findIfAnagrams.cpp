//============================================================================
// Name        : findIfAnagrams.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

bool findIfAnagrams(std::string S1, std::string S2){
	sort(S1.begin(), S1.end());
	sort(S2.begin(), S2.end());
	if(S1.compare(S2) == 0){
		return(true);
	}
	else{
		return(false);
	}

}

int howManyAnagrams(std::string strA [], int n){
	int count = 0;
	std::string copy[n];
	for (int i=0;i<n;i++){
		copy[i] = strA[i];
		sort(copy[i].begin(), copy[i].end());
	}
	sort(copy, copy+n);

	std::map<string, int> hash;
	std::pair<std::map<string, int>::iterator, bool> iter;
	for (int i=0;i<n;i++){
		iter = hash.insert(std::pair<string, int>(copy[i], 1));
		if(iter.second == true){
			count ++;
		}
	}
	return(count);
}

int main() {
	std::string S1 = "ABC";
	std::string S2 = "CAD";
	//cout<<findIfAnagrams(S1,S2);

	std::string wordArr[] = {"cat", "dog", "tac", "god", "act", "opoi"};
    int size = sizeof(wordArr) / sizeof(wordArr[0]);
    cout<<howManyAnagrams(wordArr, size)<<endl;
    return 0;
}
