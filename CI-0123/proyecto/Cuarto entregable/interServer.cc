/**
 *
 *   Client side implementation of UDP client-server model using broadcast address
 *
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include "headers/Constants.h"
#include "headers/Socket.h"
#include "headers/Connection.h"


#define PORT80	8080
#define PORT50	50000
#define PORT51	51000
#define PORT81  8081
#define MAXLINE 1024


void * receiveClient(void *);
void getRegionCountry(std::string request, std::string &region, std::string &country);


typedef struct Args {
	Socket *socket;
	Connection *connection;
};


/**
 * Envia un broadcast a los otros servidores para indicar que este proxy esta activo.
 * @param arg parametro de pthread.
 */

void * startUpBroadcast(void * arg) {

	Connection *connection = (Connection *) arg;
	int n, sockfd, broadcast = 1;
	socklen_t len;
	char buffer[MAXLINE];
	const char * hello = "1,172.16.123.35";
	struct sockaddr_in servaddr;

	if((sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT51);

	for(int i = 0; i < 9; ++i) {
		servaddr.sin_addr.s_addr = inet_addr(BroadCastAddr[i]);
		sendto(sockfd, (const char *) hello, strlen(hello), MSG_WAITALL, (const struct sockaddr *) &servaddr, sizeof(servaddr));
	}

	for(;;) {
		n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
		buffer[n] = '\0';
		printf("Servidor de datos activo: %s\n", buffer);
		connection->informationProcessingIntermediateServer(buffer);
	}

}


/**
 * Escucha en el puerto 50000 por nuevos servidores de datos activos.
 * @param arg parametro de pthread.
 */

void * Listen50(void* arg) {

	Connection *connection = (Connection *) arg;
	int n, sockfd;
	socklen_t len;
	char buffer[MAXLINE];
	const char * hello = "1,172.16.123.35";
	struct sockaddr_in servaddr, cliaddr;

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT50);

	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	len = sizeof(cliaddr);

	for(;;){
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		printf("Servidor de datos activo: %s\n", buffer);
		connection->informationProcessingIntermediateServer(buffer);
	}

}


/**
 * Escucha en el puerto 8081 por solicitudes del cliente.
 * @param arg parametro de pthread.
 */

void* Listen81(void* arg) {

	pthread_t * threads = (pthread_t *) calloc(1, sizeof(pthread_t));
	Connection *connection = (Connection *) arg;;
	Socket socket, * socketClient;
	socket.Bind(PORT81);
	socket.Listen(2);

	while(true) {
		Args *args = new Args;
		socketClient = socket.Accept();
		args->connection = connection;
		args->socket = socketClient;
		pthread_create(&threads[0], NULL, receiveClient, (void *) args);
    	pthread_join(threads[0], NULL);
		socketClient->Close();
		delete socketClient;
	}
	
}


/**
 * Hilo que maneja la consulta de un cliente.
 * @param arg parametro de pthread.
 */

void * receiveClient(void * arg) {

	bool canton;
	std::smatch match_canton;
	std::regex regexp ("canton");
	char * clientRequest = (char *) malloc(512);
	char * response = (char *) malloc(16384);
	std::string region, country, request, serverIP = "172.16.123.35";

	Args *args = (Args *) arg;
	Connection *connection = (Connection *) args->connection;
	Socket socketServer, *socketClient = (Socket *) args->socket;

	socketClient->Read(clientRequest, 512);
	request = clientRequest;

	canton = std::regex_search(request, match_canton, regexp);

	if(!canton) { 
		getRegionCountry(clientRequest, region, country);
		if(region.compare("caribbean")){ 
			request = std::regex_replace(request, std::regex("&t"), "");
		}
		serverIP = connection->findRegionIP(region);
	}

	socketServer.Connect(serverIP.c_str(), PORT80);
	socketServer.Write((char *) request.c_str());
	socketServer.Read(response, 16384);
	socketServer.Close();

	socketClient->Write(response);

}


/**
 * Inicializa el servidor intermedio, creando hilos para las distinas funcionalidades del servidor.
 */

int main() {

	Connection *connection = new Connection();
	pthread_t hilos[3];
	pthread_create(&hilos[0],NULL, startUpBroadcast, (void *) connection);
	pthread_create(&hilos[1],NULL, Listen50, (void *) connection);
	pthread_create(&hilos[2],NULL, Listen81, (void *) connection);

	for(int i = 0; i < 2; ++i){
		 pthread_join(hilos[i],NULL);
	}

	return 0;

}


/**
 * Lee el request y separa el nombre de la region y el pais.
 * @param arg parametro de pthread.
 */

void getRegionCountry(std::string request, std::string &region, std::string &country){

	std::string divide_req = request;
	int req_l = divide_req.size();

	std::vector<int> pos;

	std::vector<char> chars = { '=', '&', '=', ' ' };
	int chars_l = chars.size();

	int chars_i = 0;
	int req_i = 0;
	int p = 0;
    
    while( (req_i<req_l) && (chars_i<chars_l) ){
    
        if( divide_req[req_i] == chars[chars_i] ){
            p = req_i;
            if( ('=' == chars[chars_i] ) && (p <= (req_l-2) ) ){
                ++p;
            }
            pos.push_back( p );
            ++chars_i; 
        }
        
        ++req_i; 
    }
    
    if( chars_l == pos.size() ){
        
        int length = pos[1]-pos[0];
        region = divide_req.substr( pos[0], length );
        
        length = pos[3]-pos[2];
        country = divide_req.substr( pos[2], length );
        
    }
    
}