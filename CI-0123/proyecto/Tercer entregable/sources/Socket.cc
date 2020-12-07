#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../headers/Socket.h"


/**
 * Crea el socket.
 * @param stream tipo del socket.
 * @param ipv6 protocol family del socket.
 */

struct sockaddr_in addr;

Socket :: Socket() {

  this->ipv6        = false;
  this->idSocket    = socket(AF_INET, SOCK_STREAM, 0);
  this->SSLContext  = nullptr;
  this->SSLStruct   = nullptr;

  if(this->idSocket < 0){
    printf("Error al crear Socket\n");
    exit(8);
  }

}

Socket :: Socket(char stream, bool ipv6) {

  if(!ipv6) {
    this->ipv6 = false;

    if(stream == STREAM) {
      this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
    }
    else {
      this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
    }

    printf("\nEstableciendo conexión con el servidor...\n" );
  }
  else{
    this->ipv6 = true;

    if(stream == STREAM) {
      this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
    }
    else{
      this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
    }
  }

  if(ERROR == this->idSocket) {
    perror("Socket::Socket");
    exit(EXIT);
  }

  this->SSLContext  = nullptr;
  this->SSLStruct   = nullptr;

}


/**
 * Crea una instancia Socket a partir de un id que ya ha sido asignado.
 * @param id el identificador del socket que ya ha sido creado.
 */

Socket :: Socket( int id ) {

  this->idSocket = id;
  this->ipv6 = false;
  this->SSLContext  = nullptr;
  this->SSLStruct   = nullptr;

}


/**
 * Destruye la instancia del socket. Cierra la conexión SSL.
 */

Socket :: ~Socket() {

  if (nullptr != this->SSLContext) {
    SSL_CTX_free( (SSL_CTX *) this->SSLContext );
  }
   
  if (nullptr != this->SSLStruct) {
    SSL_free( (SSL *) this->SSLStruct );
  }

  // Close();

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

int Socket :: Connect(const char * hostip, int port) {

  struct hostent * hp;
  struct in_addr ip_addr;
  struct sockaddr_in addr;

  hp = gethostbyname(hostip);

  if(!hp) {
    perror("Connect Error:");
    exit(EXIT);
  }

  
  ip_addr         = *(struct in_addr *)(hp->h_addr);
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(port);
  inet_aton(inet_ntoa(ip_addr), &addr.sin_addr);

  int ret = connect(idSocket, (sockaddr*) &addr, sizeof(addr));

  if(ERROR == ret){
    perror("Connect Error:");
    exit(EXIT);
  }

  return ret;

}


/**
 * Conecta el socket a partir de una dirección y un servicio.
 * @param host string con la dirección del servidor.
 * @param service servicio de la conexión.
 * @return 0.
 */

int Socket :: Connect( const char * host, const char * service) {

  int ret;
  struct addrinfo hints, *result, *rp;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags    = 0;
  hints.ai_protocol = 0;

  ret = getaddrinfo(host, service, &hints, &result);
  if(ERROR2 != ret) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(EXIT);
  }
  
  for(rp = result; rp; rp = rp->ai_next) {
    
    ret = connect(this->idSocket, rp->ai_addr, rp->ai_addrlen);
    if (ERROR2 == ret)
      break;

  }
  
  freeaddrinfo(result);

  return ret;

}

int Socket :: Read( char * text, int len ) {

   int r = read(idSocket, (void *) text, len);

   if(-1 == r) {
      perror("Socket::Read");
      exit(0);
   }

   return r;

}


/*
 *  Escribe la tira de caracteres a traves de socket
 */

int Socket :: Write( char * text ) {

   int w = write(idSocket, (const void *) text, strlen((const char *) text));

   if(-1 == w) {
      perror("Socket::Write");
      exit(0);
   }

   return w;

}

int Socket :: Write( char * text, int len ) {

   int w = write(idSocket, (const void *) text, len);

   if(-1 == w) {
      perror("Socket::Write");
      exit(0);
   }

   return w;
   
}

/*
 *  Indica al sistema operativo que el socket va a actuar de manera pasiva
 *  Utilizara conexiones establecidas por medio de Accept
 */

int Socket :: Listen( int queue ) {

  int ret = listen(idSocket, queue);

  if(ret < 0) {
    printf("error en listen\n");
    exit(5);
  }

  return ret;

}

/**
 * Asocia al socket con el puerto indicado como parametro
 */

int Socket :: Bind( int port ) {
   
  int st = 0;
  const int on = 1;

  st = setsockopt( this->idSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof( on ) );
  if ( -1 == st ) {
    perror( "Socket::Socket, SO_REUSEADDR, set socket options" );
  }

  st = setsockopt( this->idSocket, SOL_SOCKET, SO_REUSEPORT, (char *) &on, sizeof( on ) );
  if ( -1 == st ) {
    perror( "Socket::Socket, SO_REUSEPORT, set socket options" );
    exit(2);
  }
   
  if(this->ipv6){
    addr.sin_family = AF_INET6; 
  }
  else{
    addr.sin_family = AF_INET;
  }

  addr.sin_addr.s_addr  = htonl(INADDR_ANY);
  addr.sin_port         = htons(port);

  int len = sizeof(addr);
  int ret = bind(this->idSocket, (sockaddr*) &addr, len);

  if(ret) {
    printf("error en bind\n");
    exit(6);
  }

  return ret;

}


/*
 *  Acepta conexiones desde los clientes
 *  Devuelve una nueva instancia de la clase Socket para manejar la conexion de un cliente
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

/*
 *  Cierra parcialmente la conectividad de un socket, puede ser por escrituras o lecturas
 *  El parametro "mode" indica el tipo de cierre que se quiere efectuar
 */
int Socket::Shutdown( int mode ) {

  int ret = shutdown(this->idSocket, mode);

  if(ret < 0){
    printf("error en shutdown\n");
    exit(9);
  }

  return ret;
}

/*
 *  Cambia la variable de instancia
 */
void Socket::SetIDSocket(int id){
  this->idSocket = id;
}


/**
 * Inicia una conexión SSL.
 */

void Socket :: InitSSLContext() {

  const SSL_METHOD * method  = TLS_client_method();
  SSL_CTX * context          = SSL_CTX_new(method);

  if(nullptr == context) {
    perror("Socket::InitSSLContext");
    exit(EXIT);
  }

  this->SSLContext = (void *) context;

}


/**
 * Prepara el socket para hacer la conexión SSL.
 */

void Socket :: InitSSL() {

  this->InitSSLContext();
  SSL * ssl = SSL_new(((SSL_CTX *) this->SSLContext));

  if(nullptr == ssl) {
    perror("Socket::InitSSL");
    exit(EXIT);
  }
   
  this->SSLStruct = (void *) ssl;

}


/**
 * Realiza la conexión SSL a partir de un host y un puerto.
 * @param hostip string con la dirección del servidor.
 * @param port número de puerto al cual se va a hacer la conexión.
 * @return 0. 
 */

void Socket :: SSLConnect(char * host, int port) {

  this->Connect(host, port);
  SSL_set_fd((SSL *) this->SSLStruct, this->idSocket);

  int ret = SSL_connect( (SSL *) this->SSLStruct );

  if(ERROR == ret) {
    perror("Socket::SSLConnect");
    exit(EXIT);
  }

}


/**
 * Conecta el socket a partir de una dirección y un servicio.
 * @param host string con la dirección del servidor.
 * @param service servicio de la conexión.
 * @return 0.
 */

void Socket :: SSLConnect(char * host, char * service) {

  this->Connect(host, service);
  SSL_set_fd((SSL *) this->SSLStruct, this->idSocket);

  int ret = SSL_connect((SSL *) this->SSLStruct);

  if(ERROR == ret) {
    perror("Socket::SSLConnect");
    exit(EXIT);
  }

}


/**
 * Lee información enviada por un servidor a partir de un socket.
 * @param buffer dirección de memoria donde se almacena el mensaje enviado por el servidor o el cliente.
 * @param size cantidad de bytes que se leen de la información enviada por el servidor o el cliente.
 * @return la cantidad de bytes recibidos por el servidor o el cliente.
 */

int Socket :: SSLRead(void * buffer, int size) {

  int ret = SSL_read((SSL *) this->SSLStruct, buffer, size);

  if(ERROR == ret) {
    perror("Socket::SSLRead");
    exit(EXIT);
  }

  return ret;

}


/**
 * Envía información a un servidor o un ciente mediante un socket especificando el tamaño máximo del mensaje.
 * @param buffer dirección de memoria donde se encuentra el mensaje a enviar.
 * @param size cantidad de bytes que se van a enviar.
 * @return la cantidad de bytes enviados.
 */

int Socket :: SSLWrite(const void * buffer, int size) {

  int ret = SSL_write((SSL *) this->SSLStruct, buffer, size);

  if(ERROR == ret) {
    perror("Socket::SSLWrite");
    exit(EXIT);
  }

  return ret;

}