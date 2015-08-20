/* 
 * File:   Client.cpp
 * Author: darshan
 * 
 * Created on March 26, 2014, 4:49 PM
 */

#include <iostream>

#include "Client.h"

GroupAssign::GroupAssign()
{
    number_of_files=0;
}
Client::Client() {
}

Client::Client(const Client& orig) {
}

Client::~Client() {
}

int Client::startUdpServer()
{        	
	unsigned int alen;	/* length of address (for getsockname) */

	/* create a udp/ip socket */
	/* request the Internet address protocol */
	/* and a reliable 2-way byte stream (TCP/IP) */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket");
		return 0;
	}

	printf("\n%d created socket: descriptor = %d\n",id, fd);

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

	alen = sizeof(myaddr);
	if (getsockname(fd, (struct sockaddr *)&myaddr, &alen) < 0) {
		perror("getsockname failed");
		return 1;
	}

	printf("bind complete. Port number = %d\n", ntohs(myaddr.sin_port));
        my_udp_port = ntohs(myaddr.sin_port);                
        return 0;
}

int Client::saveInfoInFile()
{    
    std::string file_name;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << id;
    file_name="out/"+ss.str()+".out";
    myfile.open (file_name.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary | std::ofstream::trunc);
    myfile << "type : " <<"Client";
    
    myfile << "\n\nmyID : "<< std::setfill('0') << std::setw(2) << id;
    
    myfile << "\n\nPid : "<< pid;
    
    myfile << "\n\ntport : "<< tracker_udp_port;
    
    myfile << "\n\nmyport : " <<my_udp_port;            
    myfile.flush();
    
    nfm.no_of_distinct_file_to_download=0;
    std::stringstream sn;
    sn<<id;
    for(int i=0; i<download_files.size();i++)
    {  
        int set=1;
        for(int j=0; j<files_node_having.size(); j++)
            if(download_files.at(i)==files_node_having.at(j))
                set=0;
         if(set)
         {
            std::string fn, a, b;
            a="/";
            b="_";            
            fn=download_files.at(i);        
            replaceAll(fn,a,b);        
            fn= sn.str()+"-"+fn;        
            nfm.file_strem[nfm.no_of_distinct_file_to_download].open (fn.c_str(), std::fstream::in | std::fstream::out | std::fstream::binary |std::fstream::app);                                                            
            
            for(int e=0; e<original_files.size(); e++)
            {
                if(original_files.at((e))==download_files.at(i))
                {                    
                    char *allocate_file;                   
                    allocate_file=(char *)malloc(filesize.at(e));
                    std::memset(allocate_file, 0, filesize.at(e));                                
                    nfm.file_strem[nfm.no_of_distinct_file_to_download].seekp(0);
                    nfm.file_strem[nfm.no_of_distinct_file_to_download].write(allocate_file,filesize.at(e));
                    nfm.file_strem[nfm.no_of_distinct_file_to_download].flush();                    
                    nfm.only_download_file.push_back(download_files.at(i));
                    nfm.only_download_file_size.push_back(filesize.at(e));
                    nfm.only_download_file_pointer.push_back(0);
                }
            }
            nfm.no_of_distinct_file_to_download++;            
         }
    }
    return 0;
}

int Client::connectToManager()
{
    int socket_desc;
    struct sockaddr_in server;    
    
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        std::cout << "\nTracker : Could not create socket\n";
    }
         
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(manager_tcp_port);
        
    std::cout.flush();
    
    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }     
    int ident=2;
    send(socket_desc , &ident, sizeof(uint32_t), 0);        
    recv(socket_desc, &tracker_udp_port, sizeof(tracker_udp_port) , 0);    
    
    return 0;
}

void Client::startListening()
{
    AskChunk ask;
    GroupAssign ga;    
    timeval t, start, end;
    gettimeofday(&start, NULL);
    
    t.tv_sec=0;
    t.tv_usec=1;
    fd_set read_fd_set;
    FD_ZERO(&read_fd_set);     
    int l=0;
    for(int f=0; f<100000; f++)
    {        
         timeval temp_t=t;
         FD_SET(fd,&read_fd_set);  
         if(select(fd + 1,&read_fd_set,NULL,NULL,&temp_t) ==-1)
         {
              std::cout << "Error in select";
         }       
         
         if(FD_ISSET(fd,&read_fd_set))          // Received data at UDP port
         {
            //usleep(packat_delay*1000);             
            socklen_t len;
            uint16_t a;       // To check message type
            len = sizeof(myaddr);               
            recvfrom(fd,&a,sizeof(a),MSG_PEEK,(struct sockaddr *)&myaddr,&len);        
            
            if(a==2)        // Received GROUP ASSIGN message
            {                
                int n=recvfrom(fd,&ga,sizeof(GroupAssign),0,(struct sockaddr *)&myaddr,&len);                                                                        
                receivedGroupAssign(&ga);                                                                
            }
            if(a==4)
            {            
                recvfrom(fd,&a,sizeof(a),MSG_PEEK,(struct sockaddr *)&myaddr,&len);        
                std::cout<<"KKKK"<<std::endl;
                std::cout.flush();
            }
         }           
        else
        {   sendDataRequest();                   
            gettimeofday(&end, NULL);
            if(l==0)
            { requestTracker(); l++;}
            if(timevaldiff(&start, &end)>=request_timeout*10)
            {
                //requestTracker();                           
                gettimeofday(&start, NULL);
            }
        } 
    }           
}

void Client::sendDataRequest()
{    
    
    int z=0;    
    if(nfm.group.number_of_files>0)
    {                
        for(int i=0; i<nfm.group.number_of_files; i++)
        {                
            for(int j=0; j<nfm.group.number_of_neighbors[i]; j++)
            {                
                sendRequest(nfm.group.file_name[z], nfm.group.neighbors_id[z], nfm.group.neighbors_port[z]);
                z++;
            }
        }
    }
}

void Client::sendRequest(std::string c_file_name, uint16_t client_id, uint16_t client_port)
{            
    
    AskChunk send;
    send.msg_type=4;
    send.c_file_name=c_file_name;
    send.client_id=id;                    
        
    
    for(int i=0; i< nfm.only_download_file.size(); i++)
    {
        if(nfm.only_download_file.at(i)==c_file_name)
        {
            nfm.file_strem[i].seekg(nfm.only_download_file_pointer.at(i));
            if(nfm.file_strem[i].get()=='\0')
            {
                
                send.chunk=nfm.only_download_file_pointer.at(i); 
                nfm.only_download_file_pointer.at(i)++;                
                socklen_t len;
                struct sockaddr_in nodeaddr;      

                bzero(&nodeaddr,sizeof(nodeaddr));
                nodeaddr.sin_family = AF_INET;
                nodeaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
                nodeaddr.sin_port=htons(client_port);
                len = sizeof(nodeaddr);                
                
                timeval t;
                gettimeofday(&t, NULL);
                int difference;
                difference=timevaldiff(&actual_start_time, &t);                
                
                
                myfile << "\n\n"<<((difference/1000)+1)<<"\tTO\t"<<client_id<<"\t"<<nfm.only_download_file.at(i)<<"\t"<<nfm.only_download_file_pointer.at(i);
                if(nfm.only_download_file_size.at(i)<=nfm.only_download_file_pointer.at(i)*4)
                {
                    myfile <<"\n\n"<<"DOWNLOAD COMPLETE";
                    myfile.close();
                    exit(0);
                }
                sendto(fd,&send,sizeof(send),0, (struct sockaddr *)&nodeaddr,sizeof(nodeaddr));                
                return;
            }
        }
    }
}

void Client::receivedGroupAssign(GroupAssign *ga)
{   
    
    int z=0;    
            
    nfm.group.number_of_files=ga->number_of_files;
    
    for(int i=0; i<ga->number_of_files; i++)
    {
        nfm.group.file_name[i]=ga->file_name[i];        
        
        nfm.group.number_of_neighbors[i]=ga->number_of_neighbors[i];
        std::cout<<ga->number_of_neighbors[i] << "------*\n"<<std::endl;
        std::cout.flush();
        timeval t;
        gettimeofday(&t, NULL);
        int difference;
        difference=timevaldiff(&actual_start_time, &t);
        myfile << "\n\n"<<(floor(difference/1000)+1)<<"\tFROM\t"<<"T\t"<<"GROUP ASSIGN\t";
        
        for(int j=0; j<ga->number_of_neighbors[i]; j++)
        {            
            nfm.group.neighbors_id[z]=ga->neighbors_id[z];
            nfm.group.neighbors_ip[z]=ga->neighbors_ip[z];
            nfm.group.neighbors_port[z]=ga->neighbors_port[z];                                    
            myfile << ga->neighbors_id[z]<<" ";
            z++;            
        }
    }    
    return;
}

void Client::sendPacket(GroupShowInterest mesg, int size, uint16_t port, std::string ipaddr)
{           
   socklen_t len;
   struct sockaddr_in servaddr;      

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=inet_addr(ipaddr.c_str());
   servaddr.sin_port=htons(port);
   len = sizeof(servaddr);
   sendto(fd,&mesg,size,0, (struct sockaddr *)&servaddr,sizeof(servaddr));             
}

void Client::requestTracker()
{      
    
    if(download_files.size()>0)
    {        
            GroupShowInterest interest;    
            interest.msg_type=1;
            interest.client_node_id=id;    
            interest.number_of_files=download_files.size();               

            for(int i=0; i<download_files.size(); i++)
            {
                interest.file_name[i]=download_files[i];
                if(share.at(i)==1)
                {   interest.type[i]=2;     }
                else
                {   interest.type[i]=1;     }            
            }

            for(int i=0; i<download_files.size(); i++ )
            {        
                for(int j=0; j<files_node_having.size(); j++)
                {

                    if(files_node_having.at(j)==download_files.at(i))
                    {                
                        interest.type[i]=3;
                    }
                }
                
                //Print data
                timeval t;
                gettimeofday(&t, NULL);
                int difference;
                difference=timevaldiff(&actual_start_time, &t);
                myfile <<"\n\n"<<floor(difference/1000)+1;
                myfile <<"\tTO\t"<<"T" <<"\tGROUP_SHOW_INTEREST";
                myfile << "\t"<<download_files.at(i)<<"\n";
                myfile.flush();                                
            }  
            
            
            sendPacket(interest, sizeof(interest), tracker_udp_port, "127.0.0.1");
    }
    else
        return;
}
void Client::printValues()
{
    
  usleep(1000*id);
  std::cout.flush();
  std::cout << "\n\n\n-------------------------------------------\n";
  std::cout << "-"<< request_timeout << std::endl;
  std::cout << "-"<<id << std::endl;
  std::cout << "-"<<packat_delay << std::endl;
  std::cout << "-"<<drop_probability << std::endl;
  if(download_files.size()>0)
  {
      for(int i=0; i<download_files.size(); i++)
      {          
          std::cout << download_files.at(i) << "  ";
          std::cout << start_time.at(i) << "  ";
          std::cout << share.at(i) << "  ";          
      }
  }
    
  if(files_node_having.size()>0)
  {
      std::cout << std::endl;
      for(int i=0; i<files_node_having.size(); i++)
      {          
          std::cout << files_node_having.at(i) << "  ";          
      }
  }
  
  std::cout << "\n-------------------------------------------\n\n\n";
  std::cout.flush();
    
}


int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET); //go back to where we were
    return sz;
}

long Client::timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}

void Client::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

long int Client::GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}