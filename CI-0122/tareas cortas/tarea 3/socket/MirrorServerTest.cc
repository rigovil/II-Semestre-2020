// Este servidor refleja la informacion que le llega (mirror)
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "Socket.h"

void * mirror(void * arg) {

   char msg[512];
   Socket *s2 = (Socket *) arg;

   s2->Read(msg, 512);
   printf("Mensaje del cliente recibido:\t");
   printf("%s\n", msg);

   s2->Write(msg);
   printf("Mensaje al cliente enviado.\n");

   return NULL;
   
}

int main( int argc, char ** argv ) {

   int childpid;
   char a[512];
   Socket s1(true), *s2;
   
   pthread_t * hilo = (pthread_t *) calloc(1, sizeof(pthread_t));

   s1.Bind( 9866 );		
   s1.Listen( 5 );		

   for( ; ; ) {

      s2 = s1.Accept();	
      pthread_create(&hilo[0], NULL, mirror, (void *) s2);
      pthread_join(hilo[0], NULL);
      s2->Close();

   }

   free(hilo);

}
