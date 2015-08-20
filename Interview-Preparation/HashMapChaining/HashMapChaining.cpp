/*
 * HashMapChaining.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: darshan
 */

#include <iostream>
#include <vector>
#include <list>

using namespace std;


class HashMap{
	std::vector< std::list<std::pair<string, int > > > map;
	unsigned long table_size;

	/* DJB hash function */
	unsigned long hash(const char *str){
		unsigned long hash = 5381;
		char c;
		while(c = *str++){
			hash = ((hash << 5) + hash) + c;
		}
		return(hash % table_size);
	}

public:
	HashMap(size_t size = 100){
		map.resize(size);
		table_size = size;
	}

	int insert(string key, int value){
		pair<string, int> temp(key, value);
		unsigned long index = hash(key.c_str());
		std::list< pair <string, int> >::iterator result = findifexits(index, key);
		if( result == map.at(index).end()){
			map.at(index).push_back(temp);
			return(1);
		}
		else{
			(*result).second = value;
			return(1);
		}
	}

	std::list< pair <string, int> >::iterator findifexits(unsigned long &index, string key){
		std::list< pair <string, int> >::iterator it;

		for(it= map.at(index).begin(); it != map.at(index).end(); it++){
			if((*it).first == key){
				break;
			}
		}

		return(it);
	}

	int del(string key){
		unsigned long index = hash(key.c_str());
		std::list< pair <string, int> >::iterator it = findifexits(index, key);
		if(it!= map.at(index).end()){
			map.at(index).erase(it);
			return(1);
		}
		return(-1);
	}
	int find(string key);

	void printTable(){
		list<pair<string, int> >::iterator j;
		for(int i = 0; i < table_size; i++){
			for(j= map.at(i).begin(); j != map.at(i).end(); j++){
				cout<<(*j).first<<"  "<<(*j).second<<endl;
			}
		}
	}


};

int main(){
	HashMap HM(10);
	HM.insert("uuuuuuuu", 100);
	HM.insert("uuuuuuauu", 200);
	HM.printTable();

	cout<<"-------------------"<<endl;
	HM.del("uuuuuuuu");

	HM.printTable();
	cout<<"-------------------"<<endl;
	HM.del("uuuuuuauu");
	HM.printTable();
	cout<<"-------------------"<<endl;
	return(0);
}
