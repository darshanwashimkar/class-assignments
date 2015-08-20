/*
 * Scan.h
 *
 *  Created on: Apr 12, 2014
 *      Author: darshan
 */

#ifndef SCAN_H_
#define SCAN_H_
#include "vector"
#include "string"
#include <ctype.h>

class DestDetails
{
public:
	std::string destIp;
	std::vector <int> port;
};

class SCANDATA
{
public:
	std::string protocol;
	std::string srcIp;
	std::vector<DestDetails> dest_details;
};

class SordedOnSrcIP
{
public:
	std::string srcIP;
	std::vector<std::string> ip_scanned;
	std::vector<int> port_scanned;
};

class Scan {
public:
	Scan();
	virtual ~Scan();
	void CheckIfNewdata(std::string, std::string, int, int, std::string);
	void printSummary(int, int);
	void PrintVerbosity(int, int);
	void print();
	void sord_on_basic_of_srcIP(int, int);
	std::vector <SCANDATA> scan_data;
};

#endif /* SCAN_H_ */
