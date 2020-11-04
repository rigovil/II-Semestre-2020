#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <arpa/inet.h>
#include <thread>
#include <sstream>
#include <fstream>
#include <string>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "headers/Reader.h"
#include "headers/Parser.h"
#include "headers/Socket.h"
#include "headers/Constants.h"
#include "headers/TextTable.h"
#include <netdb.h>
#include <regex>
#include <netinet/in.h>
#include <bits/stdc++.h>

#define REGEX           ".*\\r\\n"              /* Expresión regular para parsear solamente el contenido de la respuesta. */
#define NAME_MAX_SIZE 	64				              /* Indica el length máximo del nombre de un lugar como entrada */
#define PORT            8080                    /* Indica el puerto al que se conecta. */
#define BUFFER_SIZE     16384                   /* Indica el tamaño del buffer que recibe la respuesta HTTP del servidor. */
#define ADDRESS         "127.0.0.1"             /* Indica la dirección del serivodr al que se va a conectar. */

std::string buildRequest(bool);

int main(int argc, char ** argv) {
  
  Socket client;
  char * buffer;
  bool region_type;
  std::string request_built;

  buffer = (char *) malloc(BUFFER_SIZE);
  
  std::cout << "Deseo saber los datos COVID-19 de:\n"
            << "Digite 0 si corresponde a un cantón.\n"
            << "Digite 1 si corresponde a un país.\n";
  std::cin  >> region_type;
  
  request_built = buildRequest(region_type);
  
  client.Connect(ADDRESS, PORT);
  client.Write((char *) request_built.c_str());
  client.Read(buffer, BUFFER_SIZE);

  printf("%s\n", buffer);

  free(buffer);
  
  return ZERO;
  
}


/**
 * Consulta el nombre de la región de acuerdo con su tipo.
 * @param region_type si la consulta es de un país o cantón.
 */

std::string buildRequest(bool region_type) {
  
  std::string region_name = "";
  std::string request_built;
  char * block = (char *) malloc(NAME_MAX_SIZE);
  
  if(!region_type){
    std::cout << "\nDigite el nombre del cantón:\n";

    std::cin.getline(block, NAME_MAX_SIZE);
    std::cin.getline(block, NAME_MAX_SIZE);
    
    region_name = std::string(block);

    request_built.append("GET /?canton=");
    region_name = std::regex_replace(region_name, std::regex(" "), "%20");
    request_built.append(region_name);
    request_built.append("&t HTTP/1.1\r\nhost: redes.ecci\r\n\r\n");
  } 
  else {
    std::cout << "\nDigite el nombre del país:\n";

    std::cin.getline(block, NAME_MAX_SIZE);
    std::cin.getline(block, NAME_MAX_SIZE);
    
    region_name = std::string(block);

    request_built.append("GET /?country=");
    region_name = std::regex_replace(region_name, std::regex(" "), "%20");
    request_built.append(region_name);
    request_built.append("&t HTTP/1.1\r\nhost: redes.ecci\r\n\r\n");
  }

  free(block);
  
  return request_built;
  
}