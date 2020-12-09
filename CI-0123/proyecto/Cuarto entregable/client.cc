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
#define PORT            8081                    /* Indica el puerto al que se conecta. */
#define BUFFER_SIZE     16384                   /* Indica el tamaño del buffer que recibe la respuesta HTTP del servidor. */
#define ADDRESS         "172.16.123.35"         /* Indica la dirección del serivdor al que se va a conectar. */
// #define ADDRESS "127.0.0.1"

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
  client.Close();

  printf("%s\n", buffer);

  free(buffer);
  
  return ZERO;
  
}


/**
 * Consulta el nombre de la región de acuerdo con su tipo.
 * @param region_type si la consulta es de un país o cantón.
 */

std::string buildRequest(bool region_type) {
  
  std::string country_name = "";
  std::string request_built;
  char * block = (char *) malloc(NAME_MAX_SIZE);
  
  if(!region_type){
    std::cout << "\nDigite el nombre del cantón:\n";

    std::cin.getline(block, NAME_MAX_SIZE);
    std::cin.getline(block, NAME_MAX_SIZE);
    
    country_name = std::string(block);

    request_built.append("GET /?canton=");
    country_name = std::regex_replace(country_name, std::regex(" "), "%20");
    request_built.append(country_name);
    request_built.append("&t HTTP/1.1\r\nhost: redes.ecci\r\n\r\n");
  } 
  else {
    int region = 0;
    int regions_len = 0;
    std::vector<std::string> regions;
    int region_len = 0;
    std::string reg_req = ""; // region request

    regions.push_back("1-South America");
    regions.push_back("2-Caribbean");
    regions.push_back("3-Eastern Europe");
    regions.push_back("4-Western Europe");
    regions.push_back("5-South Eastern Asia");
    regions.push_back("6-Western Africa");
    regions.push_back("7-Eastern Africa");

    regions_len = regions.size();
    
    std::cout << "\nRegiones a seleccionar:" << std::endl;
    for(int i=0; i<regions_len; ++i){
      std::cout << "   " << regions[i] << std::endl;
    }

    std::cout << " Número de región a consultar [1-7]: ";
    std::cin >> region;
    std::cout << std::endl;

    --region; // adjust index
    
    if( ! ( (region>=0) && (region<regions_len) ) ) {
      std::cout << "Se debe ingresar un número entre 1 y 7." << std::endl;
      std::cout << "Opción 2 seleccionada por omisión." << std::endl;
      region = 1;
    }
    
    // region selection
    region_len = regions[ region ].size();
    reg_req = regions[ region ].substr( 2, region_len );
    std::cout << "Región seleccionada: " << reg_req << std::endl;
    
    
    std::cout << "Digite el nombre del país: ";

    std::cin.getline(block, NAME_MAX_SIZE);
    std::cin.getline(block, NAME_MAX_SIZE);
    
    country_name = std::string(block);

    region_len = regions[ region ].size() ;
    reg_req = regions[ region ].substr( 2, region_len );
    
    std::for_each(reg_req.begin(), reg_req.end(), [](char & c){
      c = ::tolower(c);
    });
    
    reg_req = std::regex_replace(reg_req, std::regex(" "), "-" );

    // prepare country
    std::for_each(country_name.begin(), country_name.end(), [](char & c){
      c = ::tolower(c);
    });
    
    country_name = std::regex_replace(country_name, std::regex(" "), "-" );

    
    // GET /?region=nombre-region&pais=nombre-pais HTTP/1.1\r\nhost: redes.ecci
    request_built.append("GET /?region="); // nombre-region&pais=nombre-pais HTTP/1.1\r\nhost: redes.ecci
    request_built.append(reg_req); // &pais=nombre-pais HTTP/1.1\r\nhost: redes.ecci
    request_built.append("&pais="); // nombre-pais HTTP/1.1\r\nhost: redes.ecci
    request_built.append(country_name); //  HTTP/1.1\r\nhost: redes.ecci
    request_built.append("&t HTTP/1.1\r\nhost: redes.ecci\r\n\r\n"); // done!

  }

  free(block);
  
  return request_built;
  
}