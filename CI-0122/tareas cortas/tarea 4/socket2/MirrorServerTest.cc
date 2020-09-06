// Este servidor refleja la informacion que le llega (mirror)
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "Socket.h"
#include "ThreadBase.h"


class Hilos : public ThreadBase {

   public:

      Hilos(long id, Socket * s2) {

         this->id = id;
         this->s2 = s2;
         
      }

   private:

      long id;
      Socket *s2;

      void startRoutine() {

         char msg[512];

         s2->Read(msg, 512);
         printf("Mensaje del cliente recibido:\t");
         printf("%s\n", msg);

         s2->Write(msg);
         printf("Mensaje al cliente enviado.\n");
   
      }

};


int main( int argc, char ** argv ) {

   int childpid;
   char a[512];
   Socket s1(true), *s2;
   
   Hilos ** hilo = (Hilos **) calloc(1, sizeof(Hilos));

   s1.Bind( 9866 );		
   s1.Listen( 5 );		

   for( ; ; ) {

      s2 = s1.Accept();	
      hilo[0] = new Hilos(0, s2);
      hilo[0]->Fork();
      hilo[0]->Join();
      free(hilo[0]);
      s2->Close();

   }

   free(hilo);

}
