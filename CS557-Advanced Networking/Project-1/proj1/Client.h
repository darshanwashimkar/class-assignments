/* 
 * File:   Client.h
 * Author: darshan
 *
 * Created on March 26, 2014, 4:49 PM
 */

#ifndef CLIENT_H
#define	CLIENT_H
#include "mainheader.h"

class NodeFileMap
{
public:
    GroupAssign group;   
    std::fstream file_strem[100];
    int no_of_distinct_file_to_download;
    std::vector<std::string> only_download_file;
    std::vector<long int> only_download_file_size;
    std::vector<long int> only_download_file_pointer;
    
};


class Client {
public:
    Client();
    Client(const Client& orig);
    virtual ~Client();
    int startUdpServer();
    int saveInfoInFile(); 
    int connectToManager();
    void printValues();
    void startListening();
    void replaceAll(std::string&, const std::string&, const std::string&);
    pid_t pid;
    int id;    
    int fd;/* our socket */
    uint16_t manager_tcp_port;
    std::vector<std::string> download_files;
    std::vector<int> chunk_counter;
    std::vector<std::string> files_node_having;    
    std::vector<int> start_time;
    std::vector<int> share;
    int request_timeout;
    int drop_probability;
    int packat_delay;
    timeval actual_start_time; 
    void sendPacket(GroupShowInterest, int, uint16_t, std::string);
    void requestTracker();    
    void receivedGroupAssign(GroupAssign *);
    void sendDataRequest();
    void sendRequest(std::string,uint16_t, uint16_t);
    long int GetFileSize(std::string);
    std::fstream myfile;
    std::vector<std::string> original_files;
    std::vector<long int> filesize;    
    long timevaldiff(struct timeval *, struct timeval *);
private:
    uint16_t my_udp_port;
    uint16_t tracker_udp_port;    
    struct sockaddr_in myaddr;	/* our address */        
    NodeFileMap nfm;
};




#endif	/* CLIENT_H */

