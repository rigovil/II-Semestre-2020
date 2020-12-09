#include "../headers/Connection.h"
#include <algorithm>

Connection::Connection(/*std::string connectionIP*/){

}


Connection::~Connection(){

}

/*
* va en servidor de datos
*/
std::string Connection::informationProcessingDataServer(std::string connectionIP){
  //bool isDataServer = false;
  std::string messageWithIP;

  if(connectionIP[0] == '1'){ //chequear valor antes de la primer ","
    messageWithIP = "0,172.16.123.36,Caribbean";
  }else{
    messageWithIP = "También sos servidor de datos";
  }

  return messageWithIP;
}

/*
* va en servidor intermedio
*/
void Connection::informationProcessingIntermediateServer(std::string connectionIP){
  int word = 0;

  if(connectionIP[0] == '0'){
    std::stringstream stream(connectionIP);

    while (std::getline(stream, connectionIP, ',')) {
      if(word == 1){
        listOfIP.push_front(connectionIP);
      }else if(word == 2){
        listOfRegions.push_front(connectionIP);
      }
      ++word;
    }
  }
}

std::string Connection::findRegionIP(std::string regionToFind){
  std::list<std::string>::iterator it_IP; // initialize
  std::list<std::string>::iterator it_Regions;
  bool found = false;
  std::string IP_address = "-1";

  it_IP = listOfIP.begin();
  it_Regions = listOfRegions.begin();

  while( (!found) && ( it_IP != listOfIP.end() ) && ( it_Regions != listOfRegions.end() ) ){
     if( regionToFind == (*it_Regions) ){ // verify if the region is the same in list's element
        found = true;
        IP_address = (*it_IP);

     }

     ++it_IP;
     ++it_Regions;
  }
  
  return IP_address;
}