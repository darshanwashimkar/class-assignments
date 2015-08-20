/* 
 * File:   manager.h
 * Author: Darshan
 *
 * Created on March 20, 2014, 5:45 PM
 */

#ifndef MANAGER_H
#define	MANAGER_H
#define MAXHOSTNAME 256

#include "mainheader.h"
#include "tracker.h"
#include "Client.h"


class manager {
public:
    manager();
    manager(const manager& orig);
    virtual ~manager();    
    void readConfigFile();
    void forkProcesses();    
    int createProcessesConnection();    
    void error(const char *);
    int listenForConnection();
    void communicateTracker(int);
    pid_t tracker_pid;
    uint16_t tracker_udp_port;
    void communicateClient();
    void printvalues();
private:

    int number_of_clients;
    std::string file_location;
    int request_timeout;
    std::vector <int> node_id_tab1;    
    std::vector <int> packet_delay;
    std::vector<int> drop_probability;
    std::vector<int> node_id_tab2;
    std::vector<std::string> file_node_having;
    std::vector<int> node_id_tab3;
    std::vector<std::string> download_file;    
    std::vector<int> start_time;
    std::vector<int> share;    
    std::vector<pid_t> client_pid;          
    int socket_desc;
    uint16_t port_no;
    struct sockaddr_in server, client;    
};

#endif	/* MANAGER_H */

