/* 
 * File:   Client.cpp
 * Author: darshan
 * 
 * Created on April 25, 2014, 6:49 PM
 */

#include <string>
#include "Client.h"

extern "C"
{   
#include "ccn/ccn.h"
}

std::vector<std::string> Client::files_need_to_download;    
std::vector<int> Client::d_start_time;
std::vector<int> Client::d_share;
std::vector<std::string> Client::files_need_to_share;
std::vector<int> Client::s_start_time;
std::vector<long int> Client::s_file_size;
std::vector<long int> Client::d_file_size;
timeval Client::start;
timeval Client::end;
timeval Client::loop_start;
std::vector<int> Client::download_complete;    
std::vector<int> Client::d_file_size_found;        
int Client::node_id;
char Client::mem_map[30][640];

Client::Client()
{
}

Client::Client(const Client& orig)
{
}

Client::~Client()
{
}

int Client::construct_trace_response(struct ccn *h, struct ccn_charbuf *data, 
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


enum ccn_upcall_res Client::incomingDataRequest(struct ccn_closure *selfp,
        enum ccn_upcall_kind kind, struct ccn_upcall_info *info)
{            
    int res = 0;
    
    //store the incoming interest name
    struct ccn_charbuf *data = ccn_charbuf_create();

    //check for null, length of incoming interest name   
    //size_t name_length = info->pi->offset[CCN_PI_E_Name] - info->pi->offset[CCN_PI_B_Name];
    
    std::string MYMESSAGE;         
    const unsigned char *node_comp = NULL;
    size_t node_size = 0;
    int index;
    std::string fn;
    std::string type;
    int chunk;
    std::string chunk_str;    
    char *chunk_data;
    chunk_data=(char *)malloc(32);
    
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
      
     /* ----------- Extracting requested URL -------------------- */              
        gettimeofday(&loop_start,NULL);
        for(int n=0; n<(info->matched_comps); n++)
        {
          index = n;
          ccn_name_comp_get(info->interest_ccnb, info->interest_comps,index,&node_comp, &node_size);
          fn=fn+"/";
          fn.append(reinterpret_cast<const char*>(node_comp));          
        }                                
                
        ccn_name_comp_get(info->interest_ccnb, info->interest_comps,info->matched_comps,&node_comp, &node_size);
        type.append(reinterpret_cast<const char*>(node_comp));        
        
        if(type=="size")
        {
            int sucess=0;            
            MYMESSAGE = sendSize(fn, &sucess);            
            if(sucess)
            {
              construct_trace_response(info->h, data, info->interest_ccnb, info->pi, MYMESSAGE.c_str(), strlen(MYMESSAGE.c_str()));        
              res = ccn_put(info->h, data->buf, data->length);
              if(res < 0)
              {
                  printf("Can not send reply \n");
              }
            }
        }
        else if(type=="chunk")
        {
          ccn_name_comp_get(info->interest_ccnb, info->interest_comps,info->matched_comps+1,&node_comp, &node_size);
          if(node_comp!=NULL)
          {
            chunk_str.append(reinterpret_cast<const char*>(node_comp));        
            chunk=atoi(chunk_str.c_str());
            int sucess=0;
            int chunk_size=0;
            chunk_size=sendChunk(chunk_data,chunk, fn, &sucess);
            if(sucess)
            {
              construct_trace_response(info->h, data, info->interest_ccnb, info->pi, chunk_data, chunk_size);        
              res = ccn_put(info->h, data->buf, data->length);                                          
              saveReceivedrequest(fn,chunk);
              if(res < 0)
              {
                  printf("Can not send reply \n");
              }
            }
          }
        }
                        
        break;

   default:
    	break;
    }
return(CCN_UPCALL_RESULT_OK);
}

int Client::sendChunk(char *chunk_data, int chunk, std::string file_name, int *sucess)
{
  for(int i=0; i<files_need_to_share.size(); i++)
  {
    if(file_name==files_need_to_share.at(i))
    {
      std::ifstream f_open;
      f_open.open(ConvertStringToFileName(file_name,node_id).c_str(), std::ios::binary);
      f_open.seekg(chunk*32);
      if((s_file_size.at(i)/32)!=chunk)
      {        
        f_open.read(chunk_data,32);
        *sucess=1;
        return(32);        
      }
      else
      {        
        f_open.read(chunk_data,(s_file_size.at(i)-(32*chunk)));
        *sucess=1;
        f_open.close();
        return((s_file_size.at(i)-(32*chunk)));
      }
    }
  }  
}


std::string Client::sendSize(std::string file_name, int *sucess)
{  
  for(int i=0; i<files_need_to_share.size(); i++)
  {    
      if(files_need_to_share.at(i)==file_name)
      {
        *sucess=1;
        std::stringstream strstream;
        std::string f_size_string;
        strstream<<s_file_size.at(i);
        strstream>>f_size_string;      
        return(f_size_string);
      }        
  }
   
  for(int i=0; i<files_need_to_download.size(); i++)
  {
    if(d_file_size_found.at(i)==1)
    {
      if(d_share.at(i)==1)
      {
        if(files_need_to_download.at(i)==file_name)
        {
          *sucess=1;
          std::stringstream strstream;
          std::string f_size_string;
          strstream<<d_file_size.at(i);
          strstream>>f_size_string;
          return(f_size_string);
        }
      }
    }
  }
  return("none");
}

void Client::GetConfiguration()
{
  int res;
  std::string URI;
  URI="/csu/cs557/manager/config/";
  
  //allocate memory for interest 
  struct ccn_charbuf *ccnb = ccn_charbuf_create();
  
  if(ccnb == NULL)
  {
      fprintf(stderr, "Can not allocate memory for interest\n");
      exit(1);
  }
  
  char id[2];
  sprintf(id,"%02d", node_id);  
  URI=URI+id+"/";  
  
  //adding name to interest
  res = ccn_name_from_uri(ccnb, URI.c_str());  
  if(res == -1)
  {
      fprintf(stderr, "Failed to assign name to interest\n");      
      exit(1);
  }
  
  //create the ccn handle
  struct ccn *ccn = ccn_create();
  if(ccn == NULL)
  {
      fprintf(stderr, "Can not create ccn handle\n");
      exit(1);
  }
    
  //connect to ccnd
  res = ccn_connect(ccn, NULL);
  if (res == -1) 
  {
      fprintf(stderr, "Could not connect to ccnd... exiting\n");
      exit(1);
  }  
    
  //allocate buffer for response
  struct ccn_charbuf *resultbuf = ccn_charbuf_create();
  if(resultbuf == NULL)
  {
      fprintf(stderr, "Can not allocate memory for URI\n");
      exit(1);
  }
    
  //setting the parameters for ccn_get
  struct ccn_parsed_ContentObject pcobuf = { 0 };
  int timeout_ms = 2000;
  
  res=-1;
  //express interest
  gettimeofday(&end, NULL);
  while(timevaldiff(&start, &end)<30*1000 && res==-1)
  {
    res = ccn_get(ccn, ccnb, NULL, timeout_ms, resultbuf, &pcobuf, NULL, 0);
    gettimeofday(&end, NULL);
  }

  if (res == -1)
  {
      fprintf(stderr, "Received empty answer for : %s\n", URI.c_str());      
      exit(0);
  }
  
  //extract data from the response
  const unsigned char *ptr;
  size_t length;
  ptr = resultbuf->buf;
  length = resultbuf->length;
  ccn_content_get_value(ptr, length, &pcobuf, &ptr, &length);

  //check if received some data
  if(length == 0)
  {                
      fprintf(stderr, "Received empty answer for : %s\n", URI.c_str());      
      exit(0);
  }
  
  config_file.assign(reinterpret_cast<const char*>(ptr));  
  //print the data
  //printf("Reply: %s\n",  ptr);
  //std::cout<<config_file;
  return;          
}


long Client::timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long msec;
  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
  return msec;
}



void Client::readConfigFile(){
    
    std::stringstream f(config_file);    
    std::string line;
    std::vector<int> vect;    
    int n;
    
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
      
}


void Client::populateNodeSpecificData()
{
  for(int i=0; i<node_id_tab3.size(); i++)
  {
    if(node_id==node_id_tab3.at(i))
    {
      for(int j=0; j<node_id_tab2.size(); j++)
      {
        if(node_id==node_id_tab2.at(j)==node_id)
        {
          if(download_file.at(i)==file_node_having.at(j) && share.at(i)==1)
          {
           files_need_to_share.push_back(download_file.at(i));
           s_start_time.push_back(start_time.at(i));           
           std::stringstream sn;
           sn<<node_id;
           std::string fn, a, b;
           a="/";
           b="_";            
           fn=download_file.at(i);   
           replaceAll(fn,a,b);        
           fn= sn.str()+"-"+fn;           
           std::ifstream file(fn.c_str());
           
           if(file.good())
           { s_file_size.push_back(GetFileSize(fn)); }
           else
           { std::cout<<"File: "<<fn<<" does not exist hence exiting"<<std::endl; exit(0);}
          }
        }
      }
    }
  }
  
  for(int i=0; i<node_id_tab3.size(); i++)
  {
    if(node_id==node_id_tab3.at(i))
    {
      int j;
      for(j=0; j<node_id_tab2.size(); j++)
      {
        if(node_id==node_id_tab2.at(j)==node_id)
        {
          if(download_file.at(i)==file_node_having.at(j) && share.at(i)==1)
          {
            break;
          }
        }
      }
      
      if(j>=node_id_tab2.size())
      {
        files_need_to_download.push_back(download_file.at(i));
        d_start_time.push_back(start_time.at(i));
        d_share.push_back(share.at(i));
        d_file_size.push_back(0);
        d_file_size_found.push_back(0);
        download_complete.push_back(0);                
      }
    }
  }
    
  
  if(files_need_to_share.size()==0 && files_need_to_download.size()==0)
  {
    std::cout<<"Client "<<node_id<<" exiting"<<std::endl;
    exit(0);    
  }     
  
  return;
}

void Client::startListening()
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
    
    gettimeofday(&start,NULL);
    gettimeofday(&end,NULL);
    struct ccn_charbuf *prefix;    
    
    loop_start.tv_sec = start.tv_sec;
    loop_start.tv_usec = start.tv_usec;
    while((timevaldiff(&loop_start, &end))<(20*1000) || allDownloaded()==0)
    {              
        std::vector<std::string> file_to_publish;
        filesLiveToPublish(&file_to_publish);
        
        /* ----------------------------- Run server ------------------------------- */        
        if(file_to_publish.size()>0)
        {    
          for(int i=0; i<file_to_publish.size(); i++)
          {
              /* create prefix we are interested in, register in FIB */
              prefix = ccn_charbuf_create();    

              /* We are interested in anythin starting with ccnx:/ */
              res = ccn_name_from_uri(prefix,file_to_publish.at(i).c_str());

              if (res < 0) 
              {
                  fprintf(stderr, "Can not convert name to URI\n");
                  exit(1);
              }

              struct ccn_closure *in_interest;
              in_interest = (struct ccn_closure*)malloc(sizeof(struct ccn_closure));
              in_interest->p = &Client::incomingDataRequest; 
              in_interest->data = &prefix;

              res = ccn_set_interest_filter(ccn, prefix, in_interest);

              if (res < 0) 
              {
                  fprintf(stderr, "Failed to register interest (res == %d)\n", res);
                  exit(1);
              }
          }
          
              //listen infinitely    
              res = ccn_run(ccn, 50);                              
        }
        
        /* ----------------------------- Server Ends here ------------------------------- */
                
        /* ----------------------------- Client starts here ------------------------------- */
        
        
        if(files_need_to_download.size()>0)
        {                     
            std::vector<std::string> url_list, fn, type;                 
            std::vector<int> chunk;
            getURL(&url_list, &fn, &type, &chunk);                       
            
            for(int m=0; m<url_list.size(); m++)
            {                                 
                gettimeofday(&loop_start,NULL);
                //allocate memory for interest 
                struct ccn_charbuf *ccnb = ccn_charbuf_create();

                if(ccnb == NULL)
                {
                    fprintf(stderr, "Can not allocate memory for interest\n");
                    exit(1);
                }                

                //adding name to interest
                res = ccn_name_from_uri(ccnb, url_list.at(m).c_str());  
                if(res == -1)
                {
                    fprintf(stderr, "Failed to assign name to interest\n");      
                    exit(1);
                }
                  

                //allocate buffer for response
                struct ccn_charbuf *resultbuf = ccn_charbuf_create();
                if(resultbuf == NULL)
                {
                    fprintf(stderr, "Can not allocate memory for URI\n");
                    exit(1);
                }

                //setting the parameters for ccn_get
                struct ccn_parsed_ContentObject pcobuf = { 0 };
                int timeout_ms = 20;                
                
                //express interest                                                
                res = ccn_get(ccn, ccnb, NULL, timeout_ms, resultbuf, &pcobuf, NULL, 0);                
                
                saveSentRequestToFile(fn.at(m), chunk.at(m));
                
                if(res!=-1)
                {
                  //extract data from the response
                  const unsigned char *ptr;
                  size_t length;
                  ptr = resultbuf->buf;
                  length = resultbuf->length;
                  ccn_content_get_value(ptr, length, &pcobuf, &ptr, &length);                  
                  //check if received some data
                  if(length != 0)
                  {           
                      if(type.at(m)=="size")
                      {                        
                        receivedSize(fn.at(m), ptr);                     
                      }
                      if(type.at(m)=="chunk")
                      {
                        receivedChunk(fn.at(m), ptr, chunk.at(m), length); 
                        saveReceivedReply(fn.at(m), chunk.at(m));
                      }
                  }                  
                }                
            }
        }        
        
        /* ----------------------------- Client ends here ------------------------------- */
        
        gettimeofday(&end,NULL);
    }
   
    //cleanup
    ccn_destroy(&ccn);    
    //ccn_charbuf_destroy(&prefix);
    
  return;
}

void Client::receivedChunk(std::string fn, const unsigned char * data, int chunk, int length)
{
  for(int i=0; i<files_need_to_download.size(); i++)
  {
    if(files_need_to_download.at(i)==fn)
    {
      if(download_complete.at(i)==0)
      {                        
        std::ofstream f_open;
        f_open.open(ConvertStringToFileName(fn,node_id).c_str(), std::ios::app |std::ios_base::binary);        
        //f_open.seekp((chunk*32));        
        f_open.write(reinterpret_cast<const char*>(data),length);
        mem_map[i][chunk]='1';
                        
        if((d_file_size.at(i)%32)==0)
        {
          if(chunk==((d_file_size.at(i)/32)-1))
          {            
            download_complete.at(i)=1;
            std::cout<<"\nFile: "<<fn<<" download complete at node: "<<node_id<<std::endl;
          }  
        }
        else
        {
          if(chunk==(d_file_size.at(i)/32))
          {            
            download_complete.at(i)=1;
            std::cout<<"\nFile: "<<fn<<" download complete at "<<node_id<<std::endl;
          }
        }
                
        f_open.close();
      }
    }
  }
}

void Client::receivedSize(std::string fn, const unsigned char* data)
{
  for(int i=0; i<files_need_to_download.size(); i++)
  {
    if(fn==files_need_to_download.at(i))
    {      
      d_file_size_found.at(i)=1;      
      d_file_size.at(i)=atol(reinterpret_cast<const char*>(data));
      
      /* ------------- Create blank file ------------------- */
      char *filedata;
      std::ofstream file;
      std::string f;
      f=files_need_to_download.at(i);
      f=ConvertStringToFileName(f, node_id);
      file.open(f.c_str(), std::ifstream::trunc);
      filedata=(char*)malloc(d_file_size.at(i));
      memset(filedata,0,d_file_size.at(i));
      //file.write(filedata,d_file_size.at(i));
      file.close();
      
      /* ------------- Set memory map ------------------- */
      
      for(int k=0; k<=(d_file_size.at(i)/32); k++)
      { mem_map[i][k]='0'; }                  
      
      break;
    }
  }
  return;
}

void Client::getURL(std::vector<std::string> *url_list, std::vector<std::string> *fn, std::vector<std::string> *type, std::vector<int> *chunk)
{
  url_list->clear();
  for(int i=0; i<files_need_to_download.size(); i++)
  {
    gettimeofday(&end, NULL);
    if((timevaldiff(&start,&end)/1000)>=d_start_time.at(i))
    {
      if(d_file_size_found.at(i)==0)
      {
        fn->push_back(files_need_to_download.at(i));
        type->push_back("size");
        chunk->push_back(0);
        url_list->push_back(files_need_to_download.at(i)+"/size/");                
      }
      else
      {
        if(download_complete.at(i)==0)
        {                    
          int chunk_number=0;
          for(int j=0; j<=(d_file_size.at(i)/32); j++) 
          {
            if(mem_map[i][j]=='0')
            {
              break;
            }
            chunk_number++;
          }
          fn->push_back(files_need_to_download.at(i));
          type->push_back("chunk");
          chunk->push_back(chunk_number);
          std::stringstream chunk_str_stream;
          std::string chunk_str;
          chunk_str_stream<<chunk_number;
          chunk_str_stream>>chunk_str;
          url_list->push_back(files_need_to_download.at(i)+"/chunk/"+chunk_str+"/");
        }
      }
    }
  }
  return;
}

void Client::filesLiveToPublish(std::vector<std::string> *file_live)
{
  file_live->clear();
  
  for(int i=0; i<files_need_to_share.size(); i++)
  {
    gettimeofday(&end, NULL);
    if(((timevaldiff(&start, &end))/1000)>=s_start_time.at(i))
    {
      file_live->push_back(files_need_to_share.at(i));
    }
  }
  
  for(int i=0; i<files_need_to_download.size(); i++)
  {
    gettimeofday(&end, NULL);
    if(((timevaldiff(&start, &end)/1000))>=d_start_time.at(i) && d_share.at(i)==1 && d_file_size_found.at(i)==1)
    {
      file_live->push_back(files_need_to_download.at(i));
    }
  }
  return;
}

long int Client::GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
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

std::string Client::ConvertStringToFileName(std::string filename, int node_id)
{
  std::string a, b;
  a="/";
  b="_";
  replaceAll(filename,a,b);
  std::stringstream sn;
  sn<<node_id;
  filename = sn.str()+"-"+filename;
  return(filename);
}


int Client::allDownloaded()
{
  for(int i=0; i<files_need_to_download.size(); i++)
  {
    if(download_complete.at(i)==0)
    {
      return(0);
    }   
  }
  return(1);
}

void Client::createOutFile()
{
  std::ofstream file;
  std::string f_str;
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << node_id;
  f_str="out/"+ss.str()+".out";        
  file.open(f_str.c_str(), std::ios::trunc);
  file<<"Type : Client\n\n";
  file<<"Node id : "<<node_id<<"\n\n";
  file<<"PID : "<<getpid()<<"\n\n";
  file<<"Time Stamp(sec)\t"<<"Message type\t File \t\t\tChunk\n\n";
  file.close();
  return;
}

void Client::saveSentRequestToFile(std::string fn, int chunk)
{
  std::ofstream file;
  std::string f_str;
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << node_id;
  f_str="out/"+ss.str()+".out";        
  file.open(f_str.c_str(), std::ios::app);
  file<<(timevaldiff(&start,&end)/1000)<<"   TO ?\t\tCLNT_SEG_REQ\t"<<fn<<"\t"<<chunk<<"\n";
  file.close();
}


void Client::saveReceivedReply(std::string fn, int chunk)
{
  std::ofstream file;
  std::string f_str;
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << node_id;
  f_str="out/"+ss.str()+".out";        
  file.open(f_str.c_str(), std::ios::app);
  file<<(timevaldiff(&start,&end)/1000)<<"   FROM ?\t\tCLNT_SEG_REP\t"<<fn<<"\t"<<chunk<<"\n";
  file.close();
}

void Client::saveReceivedrequest(std::string fn, int chunk)
{
  std::ofstream file;
  std::string f_str;
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(2) << node_id;
  f_str="out/"+ss.str()+".out";        
  file.open(f_str.c_str(), std::ios::app);
  file<<(timevaldiff(&start,&end)/1000)<<"   FROM ?\t\tCLNT_SEG_REQ\t"<<fn<<"\t"<<chunk<<"\n";
  file.close();
}