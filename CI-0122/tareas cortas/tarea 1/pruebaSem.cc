#include <stdio.h>
#include <unistd.h>
#include "Semaforo.h"

int primero() {

   printf( "Voy primero\n" );

}

int segundo() {

   printf( "Voy segundo\n" );

}

int pruebaSerial() {

   primero();
   segundo();

}

int main() {

   int pid;
   Semaforo s(0);	

   pruebaSerial();

   pid = fork();
   if(pid) {     // padre
      s.Wait();
      segundo();
   }
   else {
      primero();  // hijo
      s.Signal();
   }

}


/*
   Salida esperada:
     Voy primero
     Voy segundo
     Voy primero
     Voy segundo
*/

