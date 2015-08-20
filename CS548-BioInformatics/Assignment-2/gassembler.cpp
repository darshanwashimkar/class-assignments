//============================================================================
// Name        : gassembler.cpp
// Author      : Darshan Washimkar
// Email       : darshan.wash@gmail.com
// Version     : 1.1
// Description : This program is a genome assembler developed as an
//               assignment for CS548 class.
//============================================================================

#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <tr1/functional>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cmath>
#include <tr1/unordered_map>

using namespace std;

/* Specifying read length */
#define READ_LENGTH 100

/* Class that handles and reads both the files and provide the kmers in given READ*/
class filehandler{
	ifstream file1, file2, *file_in_use;
	int head, tail;

public:
	filehandler() {
		head = 0;
		tail = 0;
	}

	int openFiles(string f1, string f2) {
		file1.open(f1.c_str());
		if(!file1.is_open()) {
			return(0);
		}
		file2.open(f2.c_str());
		if(!file2.is_open()) {
			return(0);
			file1.close();
		}
		file_in_use = &file1;

		return(1);
	}

	int nextLine(string *line) {
		string s;

		if(file_in_use->good()) {
			getline(*file_in_use, s, '\n');

			if(file_in_use->eof()) {
				if(file_in_use == &file2){
					return(0);
				}
				file_in_use = &file2;
				getline(*file_in_use, s, '\n');
			}

			/* Read reqquired line */
			getline(*file_in_use, *line, '\n');

			for(int i=0; i<2; i++)
				getline(*file_in_use, s, '\n');
		}
		else{
			cout<<"Error in reading file";
			return(0);
		}

		return (1);
	}

	int nextKMer(int k, vector<std::string> *kmers_of_read){
		string line;

		if(nextLine(&line)) {
			/* initialize tails to start forming the k-mer*/
			for(tail = 0; tail <= (READ_LENGTH-k); tail++) {
				kmers_of_read->push_back(line.substr(tail, k));
			}
		}
		else{
			return(0);
		}
		return (1);
	}

	void close(){
		file1.close();
		file2.close();
	}

};

/* Data structure to store all K-mers*/
class kmerStore {
public:
	std::string kmers;
	unsigned int weight;
	kmerStore *right;
	kmerStore *left;

	kmerStore(){
		kmers = "\0";
		weight = 0;
		right = NULL;
		left =  NULL;
	}
};


/* Global variable to save time on iterations */
int d = 1;
int k_size = 0;
unsigned int number_of_nodes = 0;
unsigned int count_node = 0;
kmerStore *root = NULL;

/* Create Binary Search Tree with the time complexity of nlong(n) */
kmerStore* InsertKmer(kmerStore *kmers_store, std::string kmer, int *k) {
	std::string kminus1 = kmer.substr(0, *k-1);

	if(kmers_store == NULL){
		kmerStore *node;
		node = new kmerStore();
		node->kmers = kmer;
		node->weight = 1;
		kmers_store = node;
		number_of_nodes++;		
		return(kmers_store);
	}

	int compare = kminus1.compare(kmers_store->kmers.substr(0,*k-1));
	/* search (k-1) mer */
	if( compare == 0) {
		if(kmer.size() >= *k) {
			char kth = kmer[*k-1];
			/* search kth mer*/
			if(kmers_store->kmers.substr((*k-1), kmers_store->kmers.size()).find(kth) != std::string::npos) {
				kmers_store->weight++;
				return(kmers_store);
			}
			kmers_store->kmers = kmers_store->kmers + kth;			
		}		
		kmers_store->weight++;
		return(kmers_store);
	}		
	else if(compare < 0) {
		kmers_store->left = InsertKmer(kmers_store->left, kmer, k);
		return(kmers_store);
	}
	else {
		kmers_store->right = InsertKmer(kmers_store->right, kmer, k);
		return(kmers_store);
	}
}

/* Read all kmers with complexity of n */
int readKmers(kmerStore *kmers_store, int *k) {

	if(kmers_store == NULL){	
		return(1);
	}
	readKmers(kmers_store->left, k);
	std::sort(kmers_store->kmers.begin()+(*k-1), kmers_store->kmers.end());
//	cout <<kmers_store->kmers.substr(0, *k-1)<< " --> "<<kmers_store->kmers.substr(*k-1, kmers_store->kmers.size())<<" --> "<<kmers_store->weight<<endl;
	if(kmers_store->weight!= 0){
		for(int j = 0; j < (kmers_store->kmers.size()-(*k-1)); j++) {
			cout<<kmers_store->kmers.substr(0, *k-1)<<kmers_store->kmers[*k-1+j] << endl;
		}
	}
	readKmers(kmers_store->right, k);
	return(1);	
}


/*  
 *   Check if calculated (K-1)mer is present in our data structure.
 *   If present then merge it and delete that (k-1)mer from our data structure.
 */
void findAndMerge(string *string_with_d, kmerStore * ref_node, kmerStore *node){
	if(node==NULL){
		return;
	}

	int result = string_with_d->compare(node->kmers.substr(0,k_size-1));
	if(result == 0){
		if(node->weight == 0 || node->weight > ref_node->weight){			
			return;	
		}		

		string kth_r = ref_node->kmers.substr(k_size-1,node->kmers.size());
		string kth_n = node->kmers.substr(k_size-1,node->kmers.size());

		/* Add weights and mearge adjacencies */
		ref_node->weight = ref_node->weight + node->weight;
		for(int w = 0; w < kth_n.size(); w++) {	
			if(kth_r.find(kth_n[w]) == std::string::npos) {
				ref_node->kmers = ref_node->kmers + kth_n[w];
			}
		}	

		/* Delete that node */				
		node->weight = 0;
	}
	if(result < 0){
		findAndMerge(string_with_d,ref_node, node->left);
	}
	else{
		findAndMerge(string_with_d,ref_node, node->right);
	}		
	
	return;
}

/* Calcularing all (K-1)mers for perticular (K-1)mer with distance <=d */
void permute(string arr, int pos, int distance, string candidates, string *orig, kmerStore *ref_node)
{
	if (pos == arr.size()) {
		if(arr.compare(*orig) == 0) {
			return;
		}      	
		findAndMerge(&arr, ref_node, root);
		return;
	}

	// distance > 0 means we can change the current character,
	// so go through the candidates
	if (distance > 0) {
		char temp = arr[pos];
		for (int i = 0; i < candidates.size(); i++) {
			arr[pos] = candidates[i];
			int distanceOffset = 0;
			// different character, thus decrement distance
			if (temp != arr[pos])
				distanceOffset = -1;
			permute(arr, pos+1, distance + distanceOffset, candidates, orig, ref_node);
		}
		arr[pos] = temp;
	}
	// otherwise just stick to the same character
	else
		permute(arr, pos+1, distance, candidates, orig, ref_node);
}

/* Merge all (K-1)mers with the hamming difference <= d */
void mergeMers(kmerStore *node){

	if(node==NULL){
		return;
	}
	mergeMers(node->left);	
	if(node->weight != 0) {
		string kminus1 = node->kmers.substr(0,(k_size-1));
		permute(kminus1, 0, d, "ACTGN", &kminus1, node);
	}

	mergeMers(node->right);
	return;
}

int main(int argc, char **argv) {
	int c;
	int long_index = 0;
	string file_name1, file_name2;
	filehandler fd;
	std::vector<std::string> kmers_of_read;
	ofstream myfile;

	/* Specifying parameter definitions  */
	static struct option long_options[] = {
	        {"r1", required_argument, 0, 'a' },
	        {"r2", required_argument, 0, 'b' },
	        {0, 0, 0, 0}
	};


	/* Reading input parameters */
	while((c = getopt_long(argc, argv,":k:a:b:d:",
            long_options, &long_index )) != -1) {
	    switch(c) {
		    case 'k':
		    	k_size = atoi(optarg);
		    	break;
		    case 'a':
		    	file_name1 = string(optarg);
		    	break;
		    case 'b':
		    	file_name2 = string(optarg);
		    	break;
		    case 'd':
			d = atoi(optarg);
			break;
		    case '?':
		    	cout<<"Usage: gassembler -k <number of mears (K-mers)>\n";
		    	return EXIT_FAILURE;
		    	break;
		}
	}

	/* Open both file and check if it exist */
	if(!fd.openFiles(file_name1,file_name2)) {
		std::cout<<"Error in opening files\n";
		return(EXIT_FAILURE);
	}

	/* Ask for all k mers from single read and loop it to read complete input files */
	for(;fd.nextKMer(k_size,&kmers_of_read);) {
		for(int i =0; i< kmers_of_read.size(); i++) {
			 /* Insert kmer in the data strucure */			
			root = InsertKmer(root, kmers_of_read.at(i), &k_size);
			root = InsertKmer(root, kmers_of_read.at(i).substr(1, k_size-1), &k_size);			
		}
		kmers_of_read.clear();
	}
	
	/* Merge all (K-1)mers with the hamming difference <= d */
	mergeMers(root);
	
	/* Read Kmers remained after merging */
	readKmers(root, &k_size);
	
	/* Close both the files */
	fd.close();

	return (EXIT_SUCCESS);
}
