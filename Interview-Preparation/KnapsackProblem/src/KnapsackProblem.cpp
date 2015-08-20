//============================================================================
// Name        : KnapsackProblem.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : You are a renowned thief who has recently switched
// from stealing precious metals to stealing cakes because of the insane
// profit margins. You end up hitting the jackpot, breaking into the world's
// largest privately owned stock of cakes—the vault of the Queen of England.
//============================================================================
//
#include <iostream>
#include <vector>
using namespace std;

int max(int a, int b){
	return (a>b?a:b);
}

void printTable(std::vector<std::vector<int> > &K){
	std::cout<<std::endl;
	for(int i = 0; i < K[0].size(); i++)
		std::cout<<i<<" ";
	std::cout<<std::endl;

	for(int i = 0; i < K.size(); i++){
		for(int j = 0; j < K[i].size(); j++){
			std::cout<<K[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}


int maximizeMyMoney(std::vector<std::pair<int, int> > &cakes, int capacity){

	std::vector<std::vector<int> > K(cakes.size()+1, std::vector<int>(capacity+1));

	for(int w = 0; w <= cakes.size(); w++){
		for(int c = 0; c <= capacity; c++){
			if(w == 0 || c == 0){
				K[w][c] = 0;
				continue;
			}

			int max_money = K[w-1][c];
			if(c < cakes[w-1].first){
				K[w][c] = max_money;
			}
			else
				K[w][c] = max(max_money, cakes[w-1].second + K[w][c - cakes[w-1].first]);
		}
	}

	return(K.at(K.size()-1).at( K.at(K.size()-1).size() - 1 ));
}


int main() {
	std::vector<std::pair<int, int> > cakes; // (weight, monetary)
	cakes.push_back(std::make_pair<int, int>(1, 30));
	cakes.push_back(std::make_pair<int, int>(50, 200));
	//cakes.push_back(std::make_pair<int, int>(4, 16));
	//cakes.push_back(std::make_pair<int, int>(2, 9));

	std::cout << "!!! KnapsackProblem !!!" << endl; // prints !!! KnapsackProblem !!!
	std::cout<<"Answer: ";
	std::cout<<maximizeMyMoney(cakes, 100);
	return 0;
}
