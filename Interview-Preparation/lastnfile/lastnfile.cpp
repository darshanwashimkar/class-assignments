#include<iostream>
#include<fstream>
#include<queue>

using namespace std;

string lastXthLine(string filename, int n){
	ifstream infile;
	string line;
	queue<string> Q;
	infile.open(filename.c_str());

	if(infile.is_open()){
		while(std::getline(infile,line)){
			Q.push(line);
			if(Q.size()>n){
				Q.pop();
			}
		}
	}

	return(Q.front());
}

int main(int argc, char* args[]){
	cout<<"dddddddddddd"<<endl;
	string str = lastXthLine("../input", 4);
	cout<<str<<endl;
	cout<<"uuuuuuu"<<endl;
	return(0);
}
