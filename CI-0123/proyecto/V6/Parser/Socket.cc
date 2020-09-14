#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Socket.h"
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
/* 
   char tipo: el tipo de socket que quiere definir
      true para "stream
      false para "datagram"
   bool ipv6: si queremos un socket para IPv6
 */
Socket::Socket( bool stream, bool ipv6 ){
  if(!ipv6){
    this->ipv6 = false;
    if(stream){
      this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
    }else {
      this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
    }
    printf("\nEstableciendo conexión con el servidor...\n" );
  }else{
    this->ipv6 = true;
    if(stream){
      this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
    }else{
      this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
    }
    printf("Socket IPv6 creado\n" );
  }

  if(this->idSocket == -1){
    printf("Error en crear Socket\n" );
    exit(1);
  }

  this->SSLContext = nullptr;
  this->SSLStruct = nullptr;
}


Socket::~Socket(){
    // Close();
    if ( nullptr != this->SSLContext ) {
      SSL_CTX_free( (SSL_CTX *) this->SSLContext );
   }
   if ( nullptr != this->SSLStruct ) {
      SSL_free( (SSL *) this->SSLStruct );
   }
}


// void Socket::Close(){
//   if(int stat = close(idSocket) < 0){
//     printf("Error en destruir Socket\n" );
//     exit(2);
//   }
// }

/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   int port: ubicacion del proceso, por ejemplo 80
 */
int Socket::Connect( const char * hostip, int port ) {

  struct hostent *hp;
  struct in_addr ip_addr;

  hp = gethostbyname(hostip);
  if (!hp) {
    printf("Error en hostname\n");
    exit(EXIT_FAILURE);
  }
  ip_addr = *(struct in_addr *)(hp->h_addr);
  printf("IP: %s\n", inet_ntoa(ip_addr));
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  inet_aton(inet_ntoa(ip_addr), &addr.sin_addr); //transforma de IPv4  a binario para la estructura
  addr.sin_port = htons(port);       //asigna el puerto a ser usado

  int st = connect(idSocket, (sockaddr*) &addr, sizeof(addr)) < 0;
  if(-1 != st){
    printf("Error conectando\n");
  }

  return st;
}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( const char *host, const char *service ) {
  size_t len;
  int st;
  struct addrinfo hints, *result, *rp;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  st = getaddrinfo( host, service, &hints, &result);
  if(st != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(st));
    exit(6);
  }
  for ( rp = result; rp; rp = rp->ai_next ) {
    st = connect( this->idSocket, rp->ai_addr, rp->ai_addrlen );
    if ( 0 == st )
      break;
  }

  freeaddrinfo( result );

  return st;
}


/**
 *
 */

void Socket::InitSSLContext() {
   const SSL_METHOD * method = TLS_client_method();
   SSL_CTX * context = SSL_CTX_new( method );

   if ( nullptr == context ) {
        perror( "Socket::InitSSLContext" );
        exit( 23 );
    }

    this->SSLContext = (void *) context;

}


/**
 *
 */

void Socket::InitSSL() {

   this->InitSSLContext();
   SSL * ssl = SSL_new( ((SSL_CTX *) this->SSLContext ) );
   if ( nullptr == ssl ) {
      perror( "Socket::InitSSL" );
      exit( 23 );
   }
   this->SSLStruct = (void *) ssl;

}


/**
 * get host by name
 */

void Socket::SSLConnect( char * host, int port ) {
   int resultado;

   this->Connect( host, port );	// Establish a non ssl connection first
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   resultado = SSL_connect( (SSL *) this->SSLStruct );
   if ( -1 == resultado ) {
      perror( "Socket::SSLConnect" );
      exit( 23 );
   }
}


/**
 *
 */

void Socket::SSLConnect( char * host, char * service ) {
   int resultado;

   this->Connect( host, service );
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   resultado = SSL_connect( (SSL *) this->SSLStruct );
   if ( -1 == resultado ) {
      perror( "Socket::SSLConnect" );
      exit( 23 );
   }
}


/**
 *
 */

int Socket::SSLRead( void * buffer, int size ) {
   int resultado;

   resultado = SSL_read( (SSL *) this->SSLStruct, buffer, size );
   if ( -1 == resultado ) {
      perror( "Socket::SSLRead" );
      exit( 23 );
   }

   return resultado;

}


/**
 *
 */

int Socket::SSLWrite( const void *buffer, int size ) {
   int resultado;

   resultado = SSL_write( (SSL *) this->SSLStruct, buffer, size );
   if ( -1 == resultado ) {
      perror( "Socket::SSLWrite" );
      exit( 23 );
   }

   return resultado;

}
