/*
 * LinkedList.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: darshan
 */
#include<iostream>

using namespace std;

template<class T>
class LinkedList{
	struct Node{
		T value;
		Node* next;
	}*root;
public:
	LinkedList(){
		root = NULL;
	}

	int appendToTail(T value){
		Node *newNode = new Node();
		if(newNode == NULL)
			return(-1);
		newNode->value = value;
		newNode->next = NULL;
		if(root==NULL){
			root = newNode;
			return(1);
		}
		Node* temp = root;
		while(temp->next != NULL){
			temp = temp->next;
		}
		temp->next = newNode;
		return(1);
	}

	void traverse(){
		Node* temp = root;
		while(temp != NULL)
		{
			cout<<temp->value<<"  ";
			temp = temp->next;
		}
		cout<<endl;
	}

	void temp(){
		Node* slow = root;
		Node* fast = root;
		while(fast->next != NULL){
			cout<<fast->value<<"  ";
			fast = fast->next->next;
		}
		cout<<"evba"<<endl;
	}
};

int main(int argc, char* args[]){

	LinkedList<int> LL;
	LL.appendToTail(23);
	LL.appendToTail(53);
	LL.traverse();
	LL.temp();
	return(1);
}



