#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include <sstream>


class Connection{
  public:
    Connection(/*std::string*/);
    ~Connection();
    std::string informationProcessingDataServer(std::string);
    void informationProcessingIntermediateServer(std::string);
    std::string findRegionIP(std::string);

  //private:
    std::string connetion;
    std::list<std::string> listOfIP;
    std::list<std::string> listOfRegions;
};
