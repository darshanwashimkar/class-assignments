/*
 * BST.cpp
 *
 *  Created on: Jul 3, 2015
 *      Author: darshan
 */

#include<iostream>


using namespace std;

template<class T>
class BST{
	struct Node{
		string key;
		T value;
		Node *left;
		Node *right;
	};
	Node* head;
public:
	BST(){
		head = NULL;
	}

	Node* createNewNode(string &key, T &value){
		Node *newNode = new Node();
		if(newNode == NULL){
			return NULL;
		}
		newNode->left = NULL;
		newNode->left = NULL;
		newNode->key = key;
		newNode->value = value;
		return(newNode);
	}

	int insert(string key, T value){
		if(head == NULL){
			head = createNewNode(key, value);
			return(1);
		}

		Node* temp =  head;
		Node* prev = head;

		while(temp != NULL){
			int value = key.compare(temp->key);
			if(value == 0 ){
				temp->value = value;
				return(1);
			}
			else if(value < 0){
				prev = temp;
				temp = temp->left;
			}
			else{
				prev = temp;
				temp = temp->right;
			}
		}

		if(key.compare(prev->key) < 0){
			prev->left = createNewNode(key, value);
			if(prev->left== NULL){
				return(0);
			}
		}
		if(key.compare(prev->key) > 0){
			prev->right = createNewNode(key, value);
			if(prev->right== NULL){
				return(0);
			}
		}
		return(1);
	}


	int remove(string key){
		if(head==NULL){
			return(0);
		}

		Node* cur = head;
		Node* prev = NULL;
		while(cur != NULL){
			int compare = key.compare(cur->key);
			int direction;
			if(prev != NULL){
				direction = key.compare(prev->key);
			}

			if(compare == 0){

				if(cur->left == NULL && cur->right == NULL){
					delete(cur);

					if(prev == NULL){
						head = NULL;
						return(1);
					}
					else if(direction < 0){
						prev->left = NULL;
						return(1);
					}
					else if(direction > 0){
						prev->right = NULL;
						return(1);
					}
				}
				else if(cur->right == NULL ){
					if(direction <= 0){
						prev-> left = cur -> left;
						delete(cur);
					}
					else if(direction > 0){
						prev->left = cur -> right;
						delete(cur);
					}
					return(1);
				}
				else if(cur->left == NULL){
					if(direction <= 0){
						prev-> right = cur -> left;
						delete(cur);
					}
					else if(direction > 0){
						prev->right = cur -> right;
						delete(cur);
					}
					return(1);
				}
				else{
					Node* curOfNew = cur->right;
					Node* prevOfNew = cur;

					while(curOfNew->left != NULL){
						prevOfNew = curOfNew;
						curOfNew = curOfNew->left;
					}
					if(curOfNew->right == NULL){
						curOfNew->left = cur->left;
						curOfNew->right = cur->right;
						prevOfNew->left = NULL;
					}
					else{
						prevOfNew->left = curOfNew->right;
						curOfNew->left = cur->left;
						curOfNew->right = cur->right;
					}
					if(direction <= 0){
						prev-> left = curOfNew;
						delete(cur);
					}
					else{
						prev-> right = curOfNew;
						delete(cur);
					}
					return(1);
				}
			}
			else if(compare < 0){
				prev = cur;
				cur = cur->left;
			}
			else{
				prev = cur;
				cur = cur->right;
			}
		}
		return(0);
	}

	void preOrder(){
		preOrderPrint(head);
	}

	void preOrderPrint(Node* node){
		if(node == NULL){
			return;
		}
		cout<<node->key << "  "<<node->value<<endl;
		preOrderPrint(node->left);
		preOrderPrint(node->right);
	}

	unsigned long hash(char* str){
		unsigned long hash = 5381;
		int c;
		while(c = *str++){
			hash = ((hash << 5) + hash) + c;
		}
		return(hash);
	}

	void inOrder(){
			inOrderPrint(head);
	}

	void inOrderPrint(Node* node){
		if(node == NULL){
			return;
		}
		inOrderPrint(node->left);
		cout<<node->key << "  "<<node->value<<endl;
		inOrderPrint(node->right);
	}

};



int main(int argc, char* args[]){
	BST<int> B;
	B.insert("5", 332);
	B.insert("2", 232);
	B.insert("1", 432);
	B.insert("3", 432);
	B.insert("7", 432);
	B.insert("6", 432);
	B.insert("9", 432);
	B.insert("8", 432);
	B.insert("A", 432);


	B.preOrder();
	cout<<"-------------------"<<endl;
	B.inOrder();

	cout<<"####################"<<endl;
	B.remove("7");

	B.preOrder();
	cout<<"-------------------"<<endl;
	B.inOrder();
	char z = 125;
	cout<<endl<<z<<(char)126<<endl;

	return(0);
}
