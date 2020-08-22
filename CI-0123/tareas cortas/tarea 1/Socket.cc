#include "Socket.h"

/*
 * Crea el socket.
 * @param stream tipo del socket
 * @param ipv6 protocol family del socket
 */
Socket :: Socket( bool stream, bool ipv6 ) {

   if(ipv6) {
      if(stream) {
         idSocket = socket(AF_INET6, SOCK_STREAM, 0);
      }
      else {
         idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
      }
   } 
   else {
      if(stream) {
         idSocket = socket(AF_INET, SOCK_STREAM, 0);
      }
      else {
         idSocket = socket(AF_INET, SOCK_DGRAM, 0);
      }
   }

   this->is_ipv6 = ipv6;
   
   if(-1 == idSocket) {
      perror("Socket::Socket");
      exit(0);
   }

}


/*
 * Destruye la instancia del socket.
 */
Socket :: ~Socket(){

    Close();

}


/*
 * Cierra el socket.
 */
void Socket :: Close(){

   if(-1 == close(idSocket)) {
      perror("Socket::Close");
      exit(0);
   }

}


/*
 * Conecta el socket a partir de una dirección y un puerto.
 * @param hostip string con la dirección del servidor.
 * @param port número de puerto al cual se va a hacer la conexión.
 */
int Socket :: Connect( const char * hostip, int port ) {

   int conexion;

   if(this->is_ipv6) {
      struct sockaddr_in6 server_addres6;
      server_addres6.sin6_family = AF_INET6;
      server_addres6.sin6_port = htons(port);
      server_addres6.sin6_flowinfo = 0;
      inet_pton(AF_INET6, hostip, &server_addres6.sin6_addr);
      conexion = connect(idSocket, (struct sockaddr *) &server_addres6, sizeof(server_addres6));
   }
   else {
      struct sockaddr_in server_addres;
      server_addres.sin_family = AF_INET;
      server_addres.sin_port = htons(port);
      inet_pton(AF_INET, hostip, &server_addres.sin_addr);
      conexion = connect(idSocket, (struct sockaddr *) &server_addres, sizeof(server_addres));
   }

   if(-1 == conexion) {
      perror("Socket::Connect");
      exit(0);
   }

   return conexion;

}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( const char * host, const char * service ) {

   return -1;

}


int Socket :: Read( char * text, int len ) {

   int r = read(idSocket, text, len);

   if(-1 == r) {
      perror("Socket::Read");
      exit(0);
   }

   return r;

}


int Socket::Write( char *text, int len ) {

   int w = write(idSocket, text, len);

   if(-1 == w) {
      perror("Socket::Write");
      exit(0);
   }

   return w;
   
}


int Socket::Listen( int queue ) {

    return -1;

}


int Socket::Bind( int port ) {

    return -1;

}


Socket * Socket::Accept(){

    return nullptr;

}


int Socket::Shutdown( int mode ) {

    return -1;

}


void Socket::SetIDSocket(int id){

    idSocket = id;

}
