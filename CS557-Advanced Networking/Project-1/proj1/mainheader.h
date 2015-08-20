/* 
 * File:   mainheader.h
 * Author: darshan
 *
 * Created on March 30, 2014, 9:20 PM
 */

#ifndef MAINHEADER_H
#define	MAINHEADER_H
#include <iostream>
#include "iomanip"
#include "string.h"
#include "cstring"
#include "fstream"
#include "algorithm"
#include "sstream"
#include "vector"
#include "math.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/time.h"
#include "netinet/in.h"
#include "sys/socket.h"
#include "netdb.h"
#include "errno.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/stat.h"

class AskChunk
{
public:
    uint16_t msg_type;
    std::string c_file_name;
    uint16_t client_id;
    uint32_t chunk;
};

class FileGroupTable
{
public:
  std::vector<std::string> file_name;
  std::vector<int> node_id;
  std::vector<uint16_t> port_number;
  std::vector<std::string> ip;  
};

class GroupAssign
{
public:
    GroupAssign();
    uint16_t msg_type;
    uint16_t number_of_files;
    std::string file_name[100];
    uint16_t number_of_neighbors[100];
    uint16_t neighbors_id[200];
    std::string neighbors_ip[200];
    uint16_t neighbors_port[200];
};

class GroupShowInterest {    
public:
    uint16_t msg_type;
    uint16_t client_node_id;
    uint16_t number_of_files;
    std::string file_name[50];
    uint16_t type[50];
};


#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* MAINHEADER_H */

