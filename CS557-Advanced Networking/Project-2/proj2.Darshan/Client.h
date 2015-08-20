/* 
 * File:   Client.h
 * Author: darshan
 *
 * Created on April 25, 2014, 6:49 PM
 */

#ifndef CLIENT_H
#define	CLIENT_H
#include "app-header.h"

class Client {
public:
    Client();
    Client(const Client& orig);
    virtual ~Client();
    void GetConfiguration();
    void readConfigFile();
    static long timevaldiff(struct timeval *, struct timeval *);
    void readcorrect();    
    void populateNodeSpecificData();
    long int GetFileSize(std::string);
    static void filesLiveToPublish(std::vector<std::string> *);
    static void replaceAll(std::string&, const std::string&, const std::string&);
    void startListening();
    static enum ccn_upcall_res incomingDataRequest(struct ccn_closure *, enum ccn_upcall_kind, struct ccn_upcall_info *);
    static int construct_trace_response(struct ccn *h, struct ccn_charbuf *, const unsigned char *, const struct ccn_parsed_interest *,const char *, int);    
    static void getURL(std::vector<std::string> *, std::vector<std::string> *, std::vector<std::string> *, std::vector<int> *);
    static std::string sendSize(std::string, int*);
    static int sendChunk(char *,int, std::string, int *);
    static void receivedSize(std::string, const unsigned char *);
    static void receivedChunk(std::string, const unsigned char *, int, int);
    static std::string ConvertStringToFileName(std::string, int);
    static int allDownloaded();
    static void createOutFile();
    static void saveSentRequestToFile(std::string, int);
    static void saveReceivedReply(std::string, int);
    static void saveReceivedrequest(std::string, int);
    static std::vector<std::string> files_need_to_download;    
    static std::vector<int> d_start_time;
    static std::vector<int> d_share;
    static std::vector<long int> d_file_size;
    static std::vector<int> d_file_size_found;
    static char mem_map[30][640];
    static std::vector<int> download_complete;    
    static std::vector<std::string> files_need_to_share;
    static std::vector<int> s_start_time;    
    static std::vector<long int> s_file_size;    
    static timeval start;
    static timeval end;    
    static timeval loop_start;
    static int node_id;
    std::string config_file;
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

#endif	/* CLIENT_H */

