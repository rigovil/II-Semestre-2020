#include <stdio.h>
#include <string.h>

#include "Socket.h"

int main( int argc, char * argv[] ) {
   char * os = (char *) "www.infogram.com";
   char * request = (char *) "GET https://infogram.com/9cf384f0-cdc2-488e-8ea6-cca496a38a90 HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   Socket s( 's', false );	// Create a new stream socket for IPv4
   char a[1024];

   memset( a, 0, 1024 );
   s.InitSSL();
   s.SSLConnect( os, 443 );
   s.SSLWrite(  (void *) request, strlen( request ) );
   s.SSLRead( a, 1024 );
   printf( "%s\n", a);
   s.SSLRead( a, 1024 );
   printf( "%s\n", a);
}

//ec2-3-229-45-136.compute-1.amazonaws.com 