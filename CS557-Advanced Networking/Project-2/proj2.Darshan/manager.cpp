/* 
 * File:   manager.cpp
 * Author: darshan
 * 
 * Created on April 24, 2014, 11:34 PM
 */
#include "manager.h"
#include "Client.h"
#include "functional"
#define CONFIG_FILE_NAME "manager.config"

timeval start,end;

std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

int construct_trace_response(struct ccn *h, struct ccn_charbuf *data, 
        const unsigned char *interest_msg, const struct ccn_parsed_interest *pi,const char *mymessage, int size)
{

    //printf("path:construct trace response");
    //**this function takes the interest, signs the content and returns to 
    //upcall for further handling

    //copy the incoming interest name in ccn charbuf
    struct ccn_charbuf *name = ccn_charbuf_create();
    struct ccn_signing_params sp = CCN_SIGNING_PARAMS_INIT;
    int res;
    //printf("Received interest, name length: %d\n", size);
    res = ccn_charbuf_append(name, interest_msg + pi->offset[CCN_PI_B_Name],
            pi->offset[CCN_PI_E_Name] - pi->offset[CCN_PI_B_Name]);

//    printf("%s\n", ccn_charbuf_as_string(name));
    if(res == -1)
    {
        fprintf(stderr, "Can not copy interest name to buffer\n");
        exit(1);
    }

    //sign the content, check if keystore exsists
    res = ccn_sign_content(h, data, name, &sp, 	mymessage, size);

    if(res == -1)
    {
        fprintf(stderr, "Can not sign content\n");
        exit(1);
    }
    
    //free memory and return
    ccn_charbuf_destroy(&sp.template_ccnb);
    ccn_charbuf_destroy(&name);
    return res;
}


enum ccn_upcall_res incoming_interest(struct ccn_closure *selfp,
        enum ccn_upcall_kind kind, struct ccn_upcall_info *info)
{    
    //this is the callback function, all interest matching ccnx:/trace
    //will come here, handle them as appropriate  
    int res = 0;
    
    //store the incoming interest name
    struct ccn_charbuf *data = ccn_charbuf_create();

    //check for null, length of incoming interest name   
    //size_t name_length = info->pi->offset[CCN_PI_E_Name] - info->pi->offset[CCN_PI_B_Name];
    
    std::string MYMESSAGE;
    MYMESSAGE = get_file_contents(CONFIG_FILE_NAME);
    //define answer     
       
    //switch on type of event
    switch (kind) 
    {
    case CCN_UPCALL_FINAL:
        return CCN_UPCALL_RESULT_OK;    
        break;

    case CCN_UPCALL_CONTENT:  
        printf("received content\n");
        break;
                
    case CCN_UPCALL_INTEREST:                
        construct_trace_response(info->h, data, info->interest_ccnb, info->pi, MYMESSAGE.c_str(), strlen(MYMESSAGE.c_str()));
        //printf("Sending Hello World\n");
        res = ccn_put(info->h, data->buf, data->length);
        if(res < 0)
        {
            printf("Can not send reply \n");
        }
        gettimeofday(&start,NULL);
        break;

   default:
    	break;
    }
return(CCN_UPCALL_RESULT_OK);
}


manager::manager()
{
}

manager::manager(const manager& orig)
{
}

manager::~manager()
{
}

void manager::readConfigFile(){
    
    std::ifstream f(CONFIG_FILE_NAME);
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
      
}

void manager::CheckIfNoOneIsHavingFile()
{
  for(int i=0; i<download_file.size(); i++)
  {
    int j=0;
    for(j=0; j<file_node_having.size(); j++)
    {
      if(file_node_having.at(j)==download_file.at(i))
      {
        break;
      }
    }
    if(j>=file_node_having.size())
    {
      std::cout<<"\nError in Configuration file. Configuration file contains one or more downloading task where not even one node is having required file. \n Exiting...\n";
      exit(0);
    }
  }
  return;
}

void manager::StartServer()
{
    int res;
     
    /* create ccn handle */
    struct ccn *ccn = NULL;
    
    /* connect to CCN */
    ccn = ccn_create();
    
    if (ccn_connect(ccn, NULL) == -1) 
    {
        fprintf(stderr, "Could not connect to ccnd");
        exit(1);
    }
    
    /* create prefix we are interested in, register in FIB */
    struct ccn_charbuf *prefix = ccn_charbuf_create();    
    
    /* We are interested in anythin starting with ccnx:/ */
    res = ccn_name_from_uri(prefix,"/csu/cs557/manager/config");
    
    if (res < 0) 
    {
        fprintf(stderr, "Can not convert name to URI\n");
        exit(1);
    }
    
    struct ccn_closure *in_interest;
    in_interest = (struct ccn_closure*)malloc(sizeof(struct ccn_closure));
    in_interest->p = &incoming_interest; 
    in_interest->data = &prefix;
    
    res = ccn_set_interest_filter(ccn, prefix, in_interest);
    
    if (res < 0) 
    {
        fprintf(stderr, "Failed to register interest (res == %d)\n", res);
        exit(1);
    }
    
    gettimeofday(&start,NULL);
    gettimeofday(&end,NULL);
    while((timevaldiff(&start, &end))<(10*1000))
    {
      //listen infinitely    
      res = ccn_run(ccn, 50);      
      gettimeofday(&end,NULL);             
    }
    
    //cleanup
    ccn_destroy(&ccn);
    ccn_charbuf_destroy(&prefix);
    return;    
}


long manager::timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}


void manager::ForkClients()
{                                                    
    for(int i = 0; i < number_of_clients; i++)
    {                
        pid_t temp_pid;
        temp_pid=fork();
        if (temp_pid >= 0 )
        {         
          if(temp_pid==0)
          {
            
            Client c;
            c.node_id=i;
            gettimeofday(&c.start, NULL);
            c.createOutFile();
            c.GetConfiguration();
            c.readConfigFile();            
            c.populateNodeSpecificData();            
            c.startListening();
            std::cout<<"Client "<<c.node_id<<" exiting"<<std::endl;
            exit(0);
          }                    
        }
        else
        {
          std::cout<<"Problem in forking process"<<std::endl;
        }
    }                    
    
    return;    
}

