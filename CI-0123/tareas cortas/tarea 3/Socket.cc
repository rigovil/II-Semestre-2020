#include "Socket.h"

/**
 * Crea el socket.
 * @param stream tipo del socket, true si es stream, false si es datagram
 * @param ipv6 protocol family del socket.
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


/**
 * Crea una instancia Socket a partir de un id que ya ha sido asignado.
 * @param id el identificador del socket que ya ha sido creado.
 */

Socket :: Socket( int id ) {

   this->idSocket = id;
   this->is_ipv6 = false;

}


/**
 * Destruye la instancia del socket.
 */

Socket :: ~Socket(){

    Close();

}


/**
 * Cierra y destruye el socket.
 */

void Socket :: Close(){

   if(-1 == close(idSocket)) {
      perror("Socket::Close");
      exit(0);
   }

}


/**
 * Conecta el socket a partir de una dirección y un puerto.
 * @param hostip string con la dirección del servidor.
 * @param port número de puerto al cual se va a hacer la conexión.
 * @return 0.
 */

int Socket :: Connect( const char * hostip, int port ) {

   int conexion;

   if(this->is_ipv6) {
      struct sockaddr_in6 server_address6;
      server_address6.sin6_flowinfo  = 0;
      server_address6.sin6_family    = AF_INET6;
      server_address6.sin6_port      = htons(port);
      inet_pton(AF_INET6, hostip, &server_address6.sin6_addr);
      conexion = connect(idSocket, (struct sockaddr *) &server_address6, sizeof(server_address6));
   }
   else {
      struct sockaddr_in server_address;
      server_address.sin_family   = AF_INET;
      server_address.sin_port     = htons(port);
      inet_pton(AF_INET, hostip, &server_address.sin_addr);
      conexion = connect(idSocket, (struct sockaddr *) &server_address, sizeof(server_address));
   }

   if(-1 == conexion) {
      perror("Socket::Connect");
      exit(0);
   }

   return conexion;

}


/**
 * Lee información enviada por un servidor a partir de un socket.
 * @param text dirección de memoria donde se almacena el mensaje enviado por el servidor o el cliente.
 * @param len cantidad máxima de bytes que se leen de la información enviada por el servidor o el cliente.
 * @return la cantidad de bytes recibidos por el servidor o el cliente.
 */

int Socket :: Read( char * text, int len ) {

   int r = read(idSocket, (void *) text, len);

   if(-1 == r) {
      perror("Socket::Read");
      exit(0);
   }

   return r;

}


/**
 * Envía información a un servidor o un ciente mediante un socket especificando el tamaño máximo del mensaje.
 * @param text dirección de memoria donde se encuentra el mensaje a enviar.
 * @param len cantidad máxima de bytes que se van a enviar.
 * @return la cantidad de bytes enviados.
 */

int Socket :: Write( char * text, int len ) {

   int w = write(idSocket, (const void *) text, len);

   if(-1 == w) {
      perror("Socket::Write");
      exit(0);
   }

   return w;
   
}


/**
 * Envía información a un servidor o un cliente mediante un socket sin especificar el tamaño máximo del mensaje.
 * @param text dirección de memoria donde se encuentra el mensaje a enviar.
 * @return la cantidad de bytes enviados.
 */

int Socket :: Write( char * text ) {

   int w = write(idSocket, (const void *) text, strlen((const char *) text));

   if(-1 == w) {
      perror("Socket::Write");
      exit(0);
   }

   return w;

}


/**
 * Marca un socket como pasivo pues será el que escucha por nuevas conexiones para aceptarlas.
 * @param queue cantidad máxima de conexiones encoladas esperando ser atendidas por el servidor.
 * @return 0.
 */

int Socket :: Listen( int queue ) {

   int l = listen(idSocket, queue);

   if(-1 == l) {
      perror("Socket::Listen");
      exit(0);
   }

   return l;

}


/**
 * Le asigna y le asocia una dirección al socket.
 * @param port número de puerto por el cual el socket va a estar asociado. 
 * @return 0.
 */

int Socket :: Bind( int port ) {

   struct sockaddr_in server_address;
   server_address.sin_family      = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port        = htons(port);

   int b = bind(idSocket, (const struct sockaddr *) &server_address, sizeof(server_address));

   if(-1 == b) {
      perror("Socket::Bind");
      exit(0);
   }

   return b;

}


/**
 * Acepta la primera conexión pendiente de la cola que posee el socket que está haciendo listen,
 * por lo que crea un nuevo socket para que esa conexión pendiente se comunique directamente con el servidor.
 * @return una instancia de la clase Socket que posee el id retornado por el syscall accept.
 * 
 */

Socket * Socket :: Accept() {

   struct sockaddr_in server_address;
   socklen_t server_address_size = sizeof(server_address);
   
   int a = accept(idSocket, (struct sockaddr *) &server_address, &server_address_size);

   if(-1 == a) {
      perror("Socket::Accept");
      exit(0);
   }

   return new Socket(a);

}


/**
 * Apaga una conexión asociada al socket.
 * @param how define cómo se va apagar, si las recepciones o transmisiones se desactivan o no.
 * @return 0.
 */

int Socket :: Shutdown( int how ) {

   int s = shutdown(idSocket, how);

   if(-1 == s) {
      perror("Socket::Shutdown");
      exit(0);
   }

   return s;

}


/**
 * Envía un mensaje UDP a través del socket.
 * @param msg mensaje UDP.
 * @param len tamaño del mensaje.
 * @param addr puntero a una estructura sockaddr_in.
 * @return bytes enviados.
 */

int Socket :: sendTo( const void * msg, int len, void * addr ) {

   int broadcast = 1;
   setsockopt(idSocket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
   int ret = sendto(idSocket, msg, len, 0, (const sockaddr *) addr, sizeof(sockaddr_in));

   if(-1 == ret) {
      perror("Socket::sendTo");
      exit(0);
   }

   return ret;

}


/**
 * Recibe un mensaje UPD a través del socket.
 * @param buf donde se almacena el mensaje.
 * @param len tamaño del buffer.
 * @param addr puntero a una estructura sockaddr_in.
 * @return bytes leídos.
 */

int Socket :: recvFrom( void * buf, int len, void * addr ) {

   socklen_t socklen = sizeof(sockaddr);
   int ret = recvfrom(idSocket, buf, len, 0, (sockaddr *) addr, &socklen);

   if(-1 == ret) {
      perror("Socket::recvFrom");
      exit(0);
   }

   return ret; 
   
}