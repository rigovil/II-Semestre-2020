#include <stdio.h>
#include "Socket.h"


int main( int argc, char ** argv ) {

   Socket s(true);
   char buffer[ 512 ];

   s.Connect( "127.0.0.1", 9866 ); 
   s.Write( argv[1] );	
   printf("Mensaje al servidor enviado.\n");
   s.Read( buffer, 512 );	
   printf("Mensaje del servidor recibido:\t");
   printf( "%s\n", buffer );	

}
