/* 
 * File:   app-main.cpp
 * Author: Darshan
 *
 * Created on March 19, 2014, 10:51 PM
 */

#include <cstdlib>
#include "manager.h"
#include "sys/stat.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    manager m;
    mkdir("out",0700);
    std::cout<< "\nHi\n";
    m.readConfigFile();        
    m.createProcessesConnection();            
    return 0;
}

