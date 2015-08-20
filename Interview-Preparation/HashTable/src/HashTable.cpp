//============================================================================
// Name        : HashTable.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdlib>

using namespace std;

class HashTable{
	int *values;
	string *keys;
	int max_size;
	int cur_size;

	unsigned long longhash(const char *str){
			unsigned long hash = 5381;
			int c;
			while(c = *str++){
				hash = ((hash << 5) + hash) + c;
			}
			return hash;
	}

	unsigned long hash(string key){
		return((longhash(key.c_str())%max_size));
	}

public:
	HashTable(size_t size){
		values = new int[size];
		keys = new string[size];
		max_size = size;
		cur_size = 0;
	}

	int insert(string key, int value){
		if(cur_size == max_size){
			return(-1);
		}

		int index = hash(key);
		while(cur_size <= max_size){
			if(keys[index].empty()){
				values[index] = value;
				keys[index] = key;
				cur_size++;
				return(1);
			}
			index++;
			if(index == max_size){
				index = 0;
			}
		}

		return(-1);
	}

	void printHashTable(){
		for(int i = 0; i < max_size; i++){
			cout<<keys[i]<<" - "<<values[i]<<endl;
		}
	}

	int get(string key){
		if(cur_size == 0){
			return(-1);
		}

		int index = hash(key);
		while(cur_size!=0){
			if(keys[index] == key){
				return(values[index]);
			}
			index++;
			if(index == max_size){
				index = 0;
			}
		}
		return(-1);
	}

};

int main(int argc, char* args[]){
	HashTable HT(3);
	HT.insert("412", 12);
	HT.insert("642", 99);
	HT.insert("512", 20);
	HT.printHashTable();
	cout<<HT.get("642")<<endl;
	cout<<HT.insert("abc", 20)<<endl;

	return(0);
}
