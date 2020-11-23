/*  Client side implementation for TCP client-server model with intermediate
 *
 *                         |                                           |
 *   Client                |   Intermediate(Balancer)                  |     www server
 *     Create socket c     |      Create socket i                      |       Create socket w
 *                         |      Bind( 9876 )                         |       Bind( 9877 )
 *     Connect to server --|----> Listen                               |
 *                         |      newConnection = Accept               |
 *                         |         Create a new worker               |
 *                         |            handleRequest( newConnection ) |
 *     Write request A ----|-------------> Read request A              |
 *                         |               Create socket y             |
 *                         |               Connect to www server ------|-----> Listen
 *                         |                                           |       newConnection = Accept
 *                         |                                           |       Create a new worker
 *                         |                                           |       handleRequest( newConnection )
 *                         |               Write request A ------------|--------> Read request A
 *                         |                                           |
 *                         |                                           |          Process request (multiple by 2 in our example)
 *                         |                                           |
 *                         |               Read response B <-----------|--------- Write response B
 *     Read response B <---|-------------- Write response B            |
 *     Display response B  |               close socket y              |
 *     close socket c      |               close newConnection         |          close newConnection
 *                         |                                           |
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 9876


int main( int argc, char ** argv ) { 
   int sockfd, n, len, st;
   char * buffer;
   char * hello = "2020";
   struct sockaddr_in servaddr;


   if ( argc > 1 ) {
      hello = argv[ 1 ];
   }

   buffer = calloc( 512, sizeof( char ) );

   // Creating socket file descriptor
   if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
      perror("socket creation failed");
      exit( EXIT_FAILURE );
   }

   memset( &servaddr, 0, sizeof( servaddr ) );
	
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons( PORT );
   inet_pton( AF_INET, "192.168.1.189", &servaddr.sin_addr );
   len = sizeof( servaddr );

   st = connect( sockfd,(struct sockaddr *) &servaddr, len );
   if ( -1 == st ) {
      perror("socket connect failed");
      exit( EXIT_FAILURE );
   }

   st = write( sockfd, hello, strlen( hello ) );
   st = read( sockfd, buffer, 512 );

   printf( "Result: %s\n", buffer );

   close( sockfd );

   free( buffer );

   return 0;

}
