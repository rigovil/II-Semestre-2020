// Este servidor refleja la informacion que le llega (mirror)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "Socket.h"

int main( int argc, char ** argv ) {

   int childpid;
   char a[512];
   Socket s1(true), *s2;

   s1.Bind( 9866 );		
   s1.Listen( 5 );		

   for( ; ; ) {
      s2 = s1.Accept();	
      childpid = fork();	

      if ( childpid < 0 ) {
         perror("server: fork error");
      }
      else if (0 == childpid) {  
         s1.Close();	
         s2->Read( a, 512 ); 
         printf("Mensaje del cliente recibido:\t");
         printf( "%s\n", a );
         s2->Write( a );	
         printf("Mensaje al cliente enviado.\n");
         exit( 0 );	
      }

      s2->Close();
   }

}
