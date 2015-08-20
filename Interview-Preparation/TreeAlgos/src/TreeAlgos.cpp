//============================================================================
// Name        : TreeAlgos.cpp
// Author      : Darshan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

class Node{
public:
	int data;
	Node *left;
	Node *right;
	Node(int data){
		this->left = NULL;
		this->right = NULL;
		this->data = data;
	}
};

class Tree{
public:
	Node *root;

	Tree(){
		root = NULL;
	}

	Node* insertNode(Node *node, int data, char direction = 'L'){
		Node* newNode = new Node(data);
		if(newNode == NULL){
			return(NULL);
		}

		if(node == NULL){
			node = newNode;
		}

		if(direction == 'l' || direction == 'L'){
			node->left = newNode;
		}
		else if(direction == 'r' || direction == 'R'){
			node->right = newNode;
		}
		else
			return(NULL);

		return(newNode);
	}

	void preOrder(Node* node){
		if(node == NULL)
			return;
		std::cout<<node->data<<" ";
		preOrder(node->left);
		preOrder(node->right);

	}
	void inOrder(Node* node){
		if(node == NULL)
			return;

		inOrder(node->left);
		std::cout<<node->data<<" ";
		inOrder(node->right);

	}
	void postOrder(Node* node){
		if(node == NULL)
			return;
		postOrder(node->left);
		postOrder(node->right);
		std::cout<<node->data<<" ";
	}

	void printPath(std::vector<int> &path, int i){
		std::cout<<std::endl;
		for(; i < path.size(); i++){
			std::cout<<path.at(i)<<" ";
		}
	}

	void printPathOfSum(Node * node, std::vector<int> &path, int sum){
		if(node == NULL){
			return;
		}
		path.push_back(node->data);
		int t = 0;
		for(int i = path.size() - 1; i >= 0; i--){
			t += path.at(i);
			if(t == sum){
				printPath(path, i);
			}
		}
		printPathOfSum(node->left, path, sum);
		printPathOfSum(node->right, path, sum);
		path.pop_back();
	}

	void printPathOfSum(int sum){
		std::vector<int> path;
		printPathOfSum(root, path, sum);
	}

	bool createTree(){

		Node * temp1, *temp2;
		root = insertNode(root, 10);
		temp1 = insertNode(root, -1, 'L');
		temp2 = insertNode(root, 2, 'R');
		insertNode(temp1, 3, 'L');
		insertNode(temp1, 5, 'R');
		insertNode(temp2, 2, 'L');

		return(true);
	}
};

int main() {
	Tree *T = new Tree;
	T->createTree();
	T->preOrder(T->root);
	std::cout<<std::endl;
	T->inOrder(T->root);
	T->printPathOfSum(4);
	return 0;
}
