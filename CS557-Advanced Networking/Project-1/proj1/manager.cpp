/* 
 * File:   manager.cpp
 * Author: Darshan
 * 
 * Created on March 20, 2014, 5:45 PM
 */

#include "manager.h"


manager::manager() {
    number_of_clients=0;
    file_location="config.txt";
    request_timeout=0;
}

manager::manager(const manager& orig) {
    file_location="config.txt";
    number_of_clients=0;    
    request_timeout=0;    
}

manager::~manager() {
}

void manager::readConfigFile(){
    
    std::ifstream f(file_location.c_str());
    std::string line;
    std::vector<int> vect;    
    int n;
 
    if (f.is_open())
  {
    // Reading Number of Clients
        
        while ( getline (f,line) )
    {                            
           line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
           line.erase(std::find_if(line.rbegin(), line.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), line.end());
           if(line[0]=='#' || line.length()==0)
           {
               continue;
           }           
           else
           {    break;  }
    }
    std::istringstream(line) >> number_of_clients;    
    
    // Reading request timeout     
    
    while ( getline (f,line) )
    {                            
           line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
           line.erase(std::find_if(line.rbegin(), line.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), line.end());
           if(line[0]=='#' || line.length()==0)
           {
               continue;
           }           
           else
           {    break;  }
    }    
    std::istringstream(line) >> request_timeout;
    
    // Reading Node ID, Packet delay, Packet drop probability  -----------------------
    
    while ( getline (f,line) )
    {                            
           line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
           line.erase(std::find_if(line.rbegin(), line.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), line.end());
           if(line[0]=='#' || line.length()==0)
           {
               continue;
           }           
           else
           {    break;  }
    }    
    
    do
    {
        std::istringstream iss(line);
        while(iss >> n)
        {
              vect.push_back(n);
        }
        
        if(vect.at(0) == -1)
        {
            break;
        }
        
        node_id_tab1.push_back(vect.at(0));
        packet_delay.push_back(vect[1]);
        drop_probability.push_back(vect[2]);            
        vect.pop_back();
        vect.pop_back();
        vect.pop_back();
    } while ( getline (f,line) );
 
    // Reading which node contain which file ------------------------------
    
    while ( getline (f,line) )
    {                            
           line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
           line.erase(std::find_if(line.rbegin(), line.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), line.end());
           if(line[0]=='#' || line.length()==0)
           {
               continue;
           }           
           else
           {    break;  }
    }
    std::vector<std::string> strvect;
do
    {
        std::string temp;
        std::istringstream iss(line);        
        while(iss >> temp)
        {
            strvect.push_back(temp);
        }
        
        if(strvect.at(0) == "-1")
        {
            strvect.pop_back();
            strvect.pop_back();
            break;
        }
        
        node_id_tab2.push_back(std::atoi(strvect.at(0).c_str()));            
        file_node_having.push_back(strvect.at(1));
        strvect.pop_back();
        strvect.pop_back();       
    } while ( getline (f,line) );
    
     // Reading the download tasks ------------------------------
    
    while ( getline (f,line) )
    {                            
           line.erase(line.begin(), std::find_if(line.begin(), line.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
           line.erase(std::find_if(line.rbegin(), line.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), line.end());
           if(line[0]=='#' || line.length()==0)
           {
               continue;
           }           
           else
           {    break;  }
    }
    
    
do
    { 
        std::string temp;
        std::istringstream iss(line);        
        while(iss >> temp)
        {
            strvect.push_back(temp);
        }
        
        if(strvect.at(0) == "-1")
        {
            strvect.pop_back();
            strvect.pop_back();
            break;
        }
        
        node_id_tab3.push_back(std::atoi(strvect.at(0).c_str()));            
        download_file.push_back(strvect.at(1));
        start_time.push_back(std::atoi(strvect.at(2).c_str()));
        share.push_back(std::atoi(strvect.at(3).c_str()));
        strvect.pop_back();

        strvect.pop_back();
        strvect.pop_back();
        strvect.pop_back();
        
    } while ( getline (f,line) );    
    
    f.close();
  }
  else
  {
        std::cout << "There is problem in opening configuration file";
        exit(0);
  }
  
    std::cout << "File reading complete. **\n";
}


int manager::createProcessesConnection()
{
    int socket_desc , new_socket , c;       
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 0 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 25);
    
    socklen_t socklen = sizeof(struct sockaddr);
    getsockname(socket_desc, (struct sockaddr *)&server , &socklen);
    // Save port number of manager process    
    port_no=ntohs(server.sin_port);
        
    //Fork Processes     
    forkProcesses();    
    c = sizeof(struct sockaddr_in);
    
    // Fork connection accepting sockets
    int pids;
    std::cout << "Manager ---- " <<port_no;
    std::cout.flush();
    pids=fork();
    if(pids>=0)
    {
        if(pids==0)
        {
            for(int i=0; i<=number_of_clients; i++)
            {
                new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);        
                std::cout << "\nConnection accepted\n";
                uint32_t id;

                recv(new_socket, &id, sizeof(uint32_t) , 0);                
                if(id==1) // Connected peer is Tracker process
                {                                
                    communicateTracker(new_socket);                                                
                }
                else if(id==2)
                {
                     send(new_socket , &tracker_udp_port, sizeof(tracker_udp_port), 0);
                }                        
            }
            exit(0);
        }        
    }
    else
    {
        std::cout << "\nProblem in creating connection process in manager class\n";        
    }        
            
    return 0;
}



void manager::communicateTracker(int new_socket)
{    
    if (new_socket<0)
    {
        perror("accept failed");
        return;
    }
    
    recv(new_socket, &tracker_udp_port, sizeof(tracker_udp_port) , 0);             
    
    std::cout << "\nManager: Tracker port is :----------------- " <<tracker_udp_port <<"\n";    
}


void manager::forkProcesses()
{    
    
    tracker_pid = fork();
    if(tracker_pid>=0)
    {        
        if(tracker_pid==0)
        {                                          
            std::cout << "\nTracker process started";                       
            tracker track;
            gettimeofday(&track.actual_start_time, NULL);
            track.pid = getpid();            
            track.startUdpServer();
            track.saveInfoInFile();
            track.connectToManager(port_no);       
            track.listenForRequest();
            std::cout<<" \n I am Tracker and I am Dying....";
            std::cout.flush();
            exit(0);
        }
        else
        {                                       
            for(int i = 0; i < number_of_clients; i++)
            {                
                pid_t temp_pid;
                temp_pid=fork();
                if (temp_pid == 0 )
                {         
                        Client c;
                        gettimeofday(&c.actual_start_time, NULL);
                        usleep(5000);                        
                        c.pid = getpid();
                        c.manager_tcp_port=port_no;
                        c.id = i;                        
                        c.drop_probability =0;
                        c.packat_delay = 0;
                        for(int j=0; j<node_id_tab1.size(); j++)
                        {
                            if(node_id_tab1.at(j)==i)
                            {
                                c.drop_probability=drop_probability.at(j);
                                c.packat_delay = packet_delay.at(j);
                            }
                        }
                        
                        for(int j=0; j<node_id_tab2.size(); j++)
                        {                            
                            if(node_id_tab2.at(j)==i)
                            {
                                c.files_node_having.push_back(file_node_having.at(j));                                
                            }
                            
                            c.original_files.push_back(file_node_having.at(j));
                                
                            std::stringstream sn;
                            sn<<node_id_tab2.at(j);
                            std::string fn, a, b;
                            a="/";
                            b="_";            
                            fn=file_node_having.at(j);        
                            c.replaceAll(fn,a,b);        
                            fn= sn.str()+"-"+fn;

                            c.filesize.push_back(c.GetFileSize(fn));                             
                        }
                        
                        for(int j=0; j<node_id_tab3.size(); j++)
                        {
                            if(node_id_tab3.at(j)==i)
                            {
                                c.download_files.push_back(download_file.at(j));
                                c.start_time.push_back(start_time.at(j));
                                c.share.push_back(share.at(j));
                                c.chunk_counter.push_back(0);
                            }                        
                        }
                        
                        c.request_timeout=request_timeout;                        
                        c.startUdpServer();
                        c.connectToManager();
                        c.saveInfoInFile();                        
                        c.startListening();
                        //c.requestTracker();
                        //c.printValues();                        
                        std::cout<<" \n I am Dying...."<<c.id;
                        std::cout.flush();
                        c.myfile.close();
                        exit(0);                                        
                }
                                
            }                                               
            return;
        }
    }
    else
    {
        std::cout << "Problem in tracker process creation";
        exit(0);
    }
}


void manager::error(const char *msg)
{
    perror(msg);
    exit(1);
}

void manager::printvalues()
{
    std::cout << tracker_pid << std::endl;
    std::cout << tracker_udp_port << std::endl;
    std::cout << number_of_clients << std::endl;
    std::cout << file_location << std::endl;
    std::cout << request_timeout << std::endl;
    
    for(int i=0; i<node_id_tab1.size(); i++)
    { std::cout << node_id_tab1.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<packet_delay.size(); i++)
    { std::cout << packet_delay.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<drop_probability.size(); i++)
    { std::cout << drop_probability.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<node_id_tab2.size(); i++)
    { std::cout << node_id_tab2.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<file_node_having.size(); i++)
    { std::cout << file_node_having.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<node_id_tab3.size(); i++)
    { std::cout << node_id_tab3.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<download_file.size(); i++)
    { std::cout << download_file.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<start_time.size(); i++)
    { std::cout << start_time.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<share.size(); i++)
    { std::cout << share.at(i) << "  "; }
    
    std::cout<<std::endl;
    
    for(int i=0; i<client_pid.size(); i++)
    { std::cout << client_pid.at(i) << "  "; }
    
    std::cout << socket_desc << std::endl;
    std::cout << port_no << std::endl; 
    
}
