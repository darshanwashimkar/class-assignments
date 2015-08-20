//============================================================================
// Name        : eval_assembly.cpp
// Author      : Darshan Washimkar
// Email       : darshan.wash@gmail.com
// Version     : 1.0
// Description : This program evaluates a genome assembly and
//               outputs the values of N75, N80 and largest contig
//============================================================================

#include <iostream>
#include <getopt.h>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <vector>

using namespace std;


int main(int argc, char **argv) {
	ifstream file;
	int c, long_index = 0;
	string line;
	std:vector<unsigned long> c_len;	
	unsigned long long total_size = 0;
	unsigned long long size_75;
	unsigned long long size_80;
	unsigned long long count = 0;
	unsigned long long N80 = 0;
	unsigned long long N75 = 0;

	/* Specifying parameter definitions  */
	static struct option long_options[] = {
	        {"input", required_argument, 0, 'a' },
	        {0, 0, 0, 0}
	};


	/* Reading input parameters */
	while((c = getopt_long(argc, argv,":a:",
            long_options, &long_index )) != -1) {
	    switch(c) {
		    case 'a':			
		    	file.open(string(optarg));
			if(!file.is_open()) {
				cout<<"Could not open file\n";
				goto fail;
			}			
		    	break;
		    case '?':
		    	cout<<"Usage: gassembler -k <number of mears (K-mers)>\n";
		    	goto fail;
		    	break;
		}
	}	

	unsigned long contig_length;
	contig_length = 0;
	/* Reading file line by line*/
	while ( getline (file,line,'\n')){		
		if(line[0] == '>'){							
			if(contig_length == 0){
				continue;
			}	
			else{
				c_len.push_back(contig_length);
				total_size = total_size + contig_length;				
				contig_length = 0;
			}											
		}
		else{
			contig_length = contig_length + line.size();
		}		
	}
	c_len.push_back(contig_length);
	total_size = total_size + contig_length;

	/* Sort contig sizes */
	std::sort(c_len.begin(), c_len.end(), std::greater<int>());
	
	size_80 = total_size*(0.8);		
	size_75 = total_size*(0.75);
	if(((total_size*80)%100)!= 0)
		size_80 = size_80 + 1;
	if(((total_size*75)%100)!= 0)
		size_75 = size_75 + 1;

	/* Output longest contig size */
	cout<< "Longest contig size is: "<< c_len.at(0)<<endl;	

	/*  Calculating N80 and N75 */
	for(int i=0; N80 == 0 ; i++){		
		count = count + c_len.at(i);

		if(count >= size_75 && N75 == 0 ){
			N75 = c_len.at(i);
		}

		if(count >= size_80 && N80 == 0 ){
			N80 = c_len.at(i);
			break;
		}		
	}
	cout<< "N75 is = "<<N75<<endl;
	cout<< "N80 is = "<<N80<<endl;

	success:
		/* Close the file */
		file.close();
		return EXIT_SUCCESS;
	fail:
		/* Close the file */
		file.close();
		return EXIT_FAILURE;
}
