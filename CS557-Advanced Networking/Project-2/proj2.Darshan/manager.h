/* 
 * File:   manager.h
 * Author: darshan
 *
 * Created on April 24, 2014, 11:34 PM
 */

#include "app-header.h"
#include "vector"
extern "C"
{   
#include "ccn/ccn.h"
}


#ifndef MANAGER_H
#define	MANAGER_H

class manager {
public:
    manager();
    manager(const manager& orig);
    virtual ~manager();    
    void readConfigFile();
    void CheckIfNoOneIsHavingFile();
    void StartServer();        
    long timevaldiff(struct timeval *, struct timeval *);
    void ForkClients();        
    std::string file_location;    
    int number_of_clients;
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
    
    
private:

};

#endif	/* MANAGER_H */

