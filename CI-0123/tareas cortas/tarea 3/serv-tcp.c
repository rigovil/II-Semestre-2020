/*  Server side (www) implementation for TCP client-server model with intermediate
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
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

int handleRequest( int client ) {
   char * a, * b;
   long value;

   a = calloc( 512, sizeof( char ) );
   b = calloc( 512, sizeof( char ) );
   read( client, a, 512 );	// Read a request from client
   value = atol( a );
   sprintf( b, "%ld", value + value );
   write( client, b, strlen( b ) );		// Write to client
   close( client );

   free( b );
   free( a );
}


int main( int argc, char ** argv ) {
   int childpid, len, sockfd, st;
   char a[512];
   struct sockaddr_in otherAddr, servAddr;

   // Creating socket file descriptor
   if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
      perror("socket creation failed");
      exit( EXIT_FAILURE );
   }

   len = 1;
   st = setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &len, sizeof( len ) ); 
   if ( -1 == st ) {
      perror("socket options failed");
      exit( EXIT_FAILURE );
   }

   memset( &servAddr, 0, sizeof( servAddr ) );
	
   servAddr.sin_family = AF_INET;
   servAddr.sin_port = htons( 9877 );
   servAddr.sin_addr.s_addr = htonl( INADDR_ANY );
   len = sizeof( servAddr );

   st = bind( sockfd, (const struct sockaddr *) &servAddr, len );
   if ( -1 == st ) {
      perror("socket bind failed");
      exit( EXIT_FAILURE );
   }

   st = listen( sockfd, 5 );	// Wait for a conection
   if ( -1 == st ) {
      perror("socket listen failed");
      exit( EXIT_FAILURE );
   }

   for( ; ; ) {
      len = sizeof( otherAddr );
      st = accept( sockfd, (struct sockaddr *) &otherAddr, &len );
      if ( -1 == st ) {
         perror("socket accept failed");
         exit( EXIT_FAILURE );
      }
      childpid = fork();	// Create a child to serve the request
      if ( childpid < 0 ) {
         perror( "server: fork error" );
      } else {
         if (0 == childpid) {		// child code
            close( sockfd );		// Close original socket in child
            handleRequest( st );
            exit( 0 );			// Exit
         }
      }
      close( st );		// Close socket in parent
   }

}
