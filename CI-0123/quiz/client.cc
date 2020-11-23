#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <regex>
#include "Socket.h"

int main(int argc, char ** argv) {
   
   char buffer[1024];
   std::string server, request, line, response, country; 
   std::string request1 = "GET /";
   std::string request2 = "HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   std::smatch matches;
   std::regex reg_exp ("\\r\\n\\r\\n(.*\\n)+");

   if(argc < 3) {
      printf("ERROR: parametros insuficientes\n");
      exit(0);
   }

   country = argv[2];
   if(argc > 3) {
      
      for(int i = 3; i < argc; ++i) {
         country.append("%20");
         country.append(argv[i]);
      }
   }

   server = argv[1];
   request = request1 + country + " " + request2;
   
   Socket s(true, false);
   s.Connect(server.c_str(), 80);
   s.Write((char *) request.c_str());
   s.Read(buffer, 1024);

   std::string str = buffer;

   std::regex_search(str, matches, reg_exp);
   response = matches.str(0);
   response = response.substr(4);
   std::cout << response;

   return 0;

}