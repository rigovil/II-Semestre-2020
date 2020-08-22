#include <stdio.h>
#include "Socket.h"

int main( int argc, char * argv[] ) {

   Socket s( true );	// Create a new stream socket for IPv4
   char a[512];
   int p;

   p = s.Connect( "163.178.104.187", 80 );
   s.Write(  (char * ) "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n", 36 );
   s.Read( a, 512 );
   printf( "%s\n", a);
   
}
