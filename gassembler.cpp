//============================================================================
// Name        : gassembler.cpp
// Author      : Darshan Washimkar
// Email       : darshan.wash@gmail.com
// Version     : 1.0
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
	kmerStore *right;
	kmerStore *left;

	kmerStore(){
		kmers = "\0";
		right = NULL;
		left =  NULL;
	}
};

/* Create Binary Search Tree with the time complexity of nlong(n) */
kmerStore* InsertKmer(kmerStore *kmers_store, std::string kmer, int *k) {
	std::string kminus1 = kmer.substr(0, kmer.size()-1);
	char kth = kmer[kmer.size()-1];

	if(kmers_store == NULL){
		kmerStore *node;
		node = new kmerStore();
		node->kmers = kmer;
		kmers_store = node;
		return(kmers_store);
	}

	int compare = kminus1.compare(kmers_store->kmers.substr(0,*k-1));
	/* search (k-1) mer */
	if( compare == 0) {
		/* search kth mer*/
		if(kmers_store->kmers.substr((*k-1), kmers_store->kmers.size()).find(kth) != std::string::npos) {
			return(kmers_store);
		}
		kmers_store->kmers = kmers_store->kmers + kth;
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
	for(int j = 0; j < (kmers_store->kmers.size()-(*k-1)); j++) {
		cout<<kmers_store->kmers.substr(0, *k-1)<<kmers_store->kmers[*k-1+j] << endl;
	}
	readKmers(kmers_store->right, k);
	return(1);	
}

int main(int argc, char **argv) {
	int c, k;
	int long_index = 0;
	string file_name1, file_name2;
	filehandler fd;
	std::vector<std::string> kmers_of_read;
	kmerStore *kmers_store = NULL;
	ofstream myfile;

	myfile.open ("exec_time.txt", ios::trunc);

	/* Specifying parameter definitions  */
	static struct option long_options[] = {
	        {"r1", required_argument, 0, 'a' },
	        {"r2", required_argument, 0, 'b' },
	        {0, 0, 0, 0}
	};

	myfile<<"--------Started--------"<<endl;
	std::time_t result = std::time(0);
	myfile<< std::asctime(std::localtime(&result))
			  << result << " seconds since the Epoch"<<endl;
	myfile.flush();

	/* Reading input parameters */
	while((c = getopt_long(argc, argv,":k:a:b:",
            long_options, &long_index )) != -1) {
	    switch(c) {
		    case 'k':
		    	k = atoi(optarg);
		    	break;
		    case 'a':
		    	file_name1 = string(optarg);
		    	break;
		    case 'b':
		    	file_name2 = string(optarg);
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
	for(;fd.nextKMer(k,&kmers_of_read);) {
		for(int i =0; i< kmers_of_read.size(); i++) {
			 /* Insert kmer in the data strucure */			
			kmers_store = InsertKmer(kmers_store, kmers_of_read.at(i), &k);			
		}
		kmers_of_read.clear();
	}

	readKmers(kmers_store, &k);

	/* To track down how much time it took to finish exection */
	myfile<<"--------ENDED--------"<<endl;
	result = std::time(0);
	myfile << std::asctime(std::localtime(&result))
			  << result << " seconds since the Epoch\n";

	/* Close both the files */
	fd.close();

	return (EXIT_SUCCESS);
}
