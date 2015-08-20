/* 
 * File:   tracker.cpp
 * Author: darshan
 * 
 * Created on March 20, 2014, 3:22 PM
 */

#include "tracker.h"
#include "manager.h"
#include "stdint.h"

tracker::tracker() {
}

tracker::tracker(const tracker& orig) {
}

tracker::~tracker() {
}

int tracker::connectToManager(int port_no)
{
    int socket_desc;
    struct sockaddr_in server;
    tracker t;
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cout << "\nTracker : Could not create socket";
    }
         
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port_no);
 
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }     
    std::cout<< "\nTracker : Connected\n";         
    uint32_t id = 1;            
    send(socket_desc , &id, sizeof(uint32_t), 0);
    std::cout << "\n\nTracker : port address - " << my_udp_port << "\n";
    send(socket_desc , &my_udp_port, sizeof(my_udp_port), 0);        
       
    return 0;
}


int tracker::startUdpServer()
{
        struct sockaddr_in myaddr;	/* our address */	
	unsigned int alen;	/* length of address (for getsockname) */

	/* create a udp/ip socket */
	/* request the Internet address protocol */
	/* and a reliable 2-way byte stream (TCP/IP) */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}

	printf("created socket: descriptor = %d\n", fd);

	/* bind to an arbitrary return address */
	/* in this case, assume it's the client side, so we won't
	/* care about the port number since no application will initiate service here */
	/* INADDR_ANY is the IP address and 0 is the socket */
	/* htonl converts a long integer (e.g. address) to a network */
	/* representation (IP-standard byte ordering) */

	/* if you want to use a specific port number (such as myport), use    */
	/*	 	myaddr.sin_port = htons(myport)    */
	/* instead of	myaddr.sin_port = htons(0);    */

	memset((void *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 1;
	}
        listen(fd , 25);        
	alen = sizeof(myaddr);
	if (getsockname(fd, (struct sockaddr *)&myaddr, &alen) < 0) {
		perror("getsockname failed");
		return 1;
	}

	printf("bind complete. Port number = %d\n", ntohs(myaddr.sin_port));
        my_udp_port = ntohs(myaddr.sin_port);        
        return 0;
}

void tracker::listenForRequest()
{    
    timeval start, end;
    gettimeofday(&start, NULL);    
    while(1)
    {
        int n;                       
        GroupShowInterest interest;        
        socklen_t len;
        len = sizeof(cliaddr);
        n = recvfrom(fd,&interest,sizeof(interest),0,(struct sockaddr *)&cliaddr,&len);                                                        
        //gettimeofday(&start, NULL);
        
        for(int i=0; i<interest.number_of_files;i++)                // For all received files
        {
            int should_add=1;
            if(interest.type[i]==3)                                 // IF file share type is 3
            {                
                for(int j=0; j<fgt.file_name.size(); j++)           // Search in File Group Table
                {
                    if(interest.file_name[i]==fgt.file_name.at(j))  // If received file name is same as File Group Table file name
                    {
                        if(interest.client_node_id==fgt.node_id.at(j))  // And also ID is same
                        {
                            should_add=0;                            
                        }
                    }
                }
            }
            
            else if(interest.type[i]==2)
            {
                for(int j=0; j<fgt.file_name.size(); j++)           // Search in File Group Table
                {
                    if(interest.file_name[i]==fgt.file_name.at(j))  // If received file name is same as File Group Table file name
                    {
                        if(interest.client_node_id==fgt.node_id.at(j))  // And also ID is same
                        {
                            should_add=0;
                        }
                    }
                }
                sendReply(interest);
            }
            
            else if(interest.type[i]==1)
            {                                             
              sendReply(interest);
              should_add=0;
            }
                        
            // need to add or not?            
            if(should_add==1)
            {
                fgt.file_name.push_back(interest.file_name[i]);
                fgt.node_id.push_back(interest.client_node_id);
                char tempip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(cliaddr.sin_addr), tempip, INET_ADDRSTRLEN);
                fgt.ip.push_back(tempip);
                fgt.port_number.push_back(ntohs(cliaddr.sin_port));                
            }                                    
            
           if(interest.type[i]==3)
           {
            std::ofstream myfile;
            timeval t;
            gettimeofday(&t, NULL);
            int difference;
            difference=timevaldiff(&actual_start_time, &t);
            
            myfile.open ("out/tracker.out", std::ofstream::app);
            myfile << "\n\n"<<interest.client_node_id<<"\t";
            myfile << floor(difference/1000)+1 <<"\t"<<interest.file_name[i]<<"\t";
            myfile.close();
            }
        }     
        
        gettimeofday(&end, NULL);
        if(timevaldiff(&start,&end)>=30*1000)
        {
            break;
        }
    }
        
}

void tracker::sendReply(GroupShowInterest interest)
{
    GroupAssign ga;
    uint16_t no_of_neighbors=0;
    uint16_t count_neighbors=0;
    ga.msg_type=2;
    ga.number_of_files=interest.number_of_files;        
    
    for(int i=0; i<interest.number_of_files; i++)
    {   
        no_of_neighbors=0;        
        for(int j=0; j<fgt.file_name.size(); j++)
        {
            if(interest.file_name[i]==fgt.file_name.at(j))
            {                                                                                
                ga.neighbors_id[count_neighbors]=fgt.node_id.at(j);
                ga.neighbors_ip[count_neighbors]=fgt.ip.at(j);
                ga.neighbors_port[count_neighbors]=fgt.port_number.at(j);
                no_of_neighbors++;
                count_neighbors++;
            }
            
        }        
        ga.file_name[i]=interest.file_name[i];
        ga.number_of_neighbors[i]=no_of_neighbors;                                                 
        
        std::ofstream myfile;
        timeval t;
        gettimeofday(&t, NULL);
        int difference;
        difference=timevaldiff(&actual_start_time, &t);

        myfile.open ("out/tracker.out", std::ofstream::app);
        myfile << "\n\n"<<interest.client_node_id<<"\t";
        myfile << floor(difference/1000)+1 <<"\t"<<interest.file_name[i]<<"\t";            
        myfile<< myfile.hex<< ga.file_name[i];
        myfile.close();
    }   
        
    sendto(fd,&ga,sizeof(ga),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
}
int tracker::saveInfoInFile()
{
    std::ofstream myfile;
    myfile.open ("out/tracker.out", std::ofstream::trunc);
    myfile << "type : " <<"Tracker";
    myfile << "\n\nPid : "<< pid;
    myfile << "\n\ntport : "<< my_udp_port;
    myfile.close();
    return 0;
}

long tracker::timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}