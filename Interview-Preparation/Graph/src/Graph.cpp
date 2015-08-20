//============================================================================
// Name        : Graph.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;


class Graph{
	int V;
	std::vector<int> * adj;
public:
	Graph(int N){
		V = N;
		adj = new std::vector<int>[N];
	}
	void addEdge(int from, int to){
		adj[from].push_back(to);
	}

	void BFS(int node_name){
		std::vector<bool> visited(V);
		for(int i = 0; i < V; i++){
			visited.at(i) = false;
		}
		std::stack<int> S;
		S.push(node_name);

		while(!S.empty()){
			int cur_node = S.top();
			S.pop();
			cout<<cur_node<<"  ";
			visited.at(cur_node) = true;

			for(int i = 0; i < adj[cur_node].size(); i++){
				if(visited[adj[cur_node].at(i)] == false){
					S.push(adj[cur_node].at(i));
				}
			}
		}
	}
};

int pow(int base, int exp){
	int result =1;
	while(exp){
		if(exp & 1){
			result *= base;
		}
		exp >>= 1;
		base *= base;
	}
	return result;
}

int main() {
	Graph g(4);
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(2, 0);
	g.addEdge(2, 3);
	g.addEdge(3, 3);
	cout<<"das"<<endl;
	std::string str = "abc";
	std::sort(str.begin(), str.end());
	cout<< endl<<str.compare("abc")<<endl;
	cout<<endl<<pow(2, 5)<<endl;
	g.BFS(2);

	return 0;
}
