//============================================================================
// Name        : Stack.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

template<class T>
class Stack{
	T *items;
	int top;
	int max;
public:
	Stack(int size){
		items = new T[size];
		max = size;
		top = 0;
	}

	int push(T item){
		if(top == max){
			return(-1);
		}
		items[top++] = item;
		return(1);
	}
	T pop(){
		if(top == 0){
			return(NULL);
		}

		return(items[--top]);
	}
};

int main() {
	Stack<int> S(2);
	S.push(1);
	S.push(2);
	cout<<S.pop()<<endl;
	cout<<S.pop()<<endl;
	if(S.pop() != NULL)
	cout<<S.pop()<<endl;
	return 0;
}
