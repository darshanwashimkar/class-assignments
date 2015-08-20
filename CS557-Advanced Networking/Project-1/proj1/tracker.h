/* 
 * File:   tracker.h
 * Author: Darshan
 *
 * Created on March 20, 2014, 3:22 PM
 */

#ifndef TRACKER_H
#define	TRACKER_H
#include "mainheader.h"



class tracker {
public:
    tracker();
    tracker(const tracker& orig);
    virtual ~tracker();
    int connectToManager(int);
    int startUdpServer();
    int saveInfoInFile();
    void listenForRequest();
    void sendReply(GroupShowInterest);
    long timevaldiff(struct timeval *, struct timeval *);
    int id;
    pid_t pid;
    timeval actual_start_time;
private:
    uint16_t manager_port;
    uint16_t my_udp_port;       
    struct sockaddr_in cliaddr;
    int fd;	/* udp socket */    
    FileGroupTable fgt;
};


#endif	/* TRACKER_H */

