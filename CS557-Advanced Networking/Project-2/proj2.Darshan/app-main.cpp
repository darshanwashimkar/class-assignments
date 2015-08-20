/* 
 * File:   app-main.cpp
 * Author: darshan
 * Email:  Darshan.Washimkar@colostate.edu
 * Created on April 24, 2014, 2:50 PM
 *
 */

#include "app-header.h"

using namespace std;

int main(int argc, char** argv) {
  
  manager m;
  mkdir("out",0700);
  m.readConfigFile();
  m.CheckIfNoOneIsHavingFile();
  m.ForkClients();
  m.StartServer();
  std::cout<<"Manager - Exiting"<<std::endl;
  return 0;
}