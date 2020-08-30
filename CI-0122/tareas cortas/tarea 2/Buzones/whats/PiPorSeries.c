/*
 * Calcula el numero PI utilizando una serie "infinita"
 * Debe recibir la cantidad de iteraciones como parametro
 *
 *   pi = (-1)^i x 4/(2xi + 1)
 *
 *  Autor: Programacion Paralela y Concurrente (Francisco Arroyo)
 *  Fecha: 2020/Ago/08
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

struct msgbuf_pi {
   long mtype;
   double casiPi;
};


double calcularSumaParcialPi( int proceso, long inicial, long terminos, int cola ) {
   struct msgbuf_pi pi;
   double casiPi = 0;
   double alterna = 4;
   long divisor = 0, termino;

   for ( termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		
      casiPi+= alterna/divisor;		
      alterna*= -1;				
   }

   pi.mtype = 1;
   pi.casiPi = casiPi;
   msgsnd(cola, (const void *)&pi, sizeof(pi), IPC_NOWAIT);

   exit( 0 );

}


int main( int argc, char ** argv ) {

   int proceso, pid, cola;
   long terminos, inicio, fin;
   double resultado = 0;
   struct msgbuf_pi pi;

   cola = msgget(0xB78292, 0666 | IPC_CREAT);

   terminos = 1000000;
   if ( argc > 1 ) {
      terminos = atol( argv[ 1 ] );
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
      if ( ! pid ) {
         calcularSumaParcialPi( proceso, inicio, fin, cola );
      } else {
         printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
      }
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }

   for ( proceso = 1; proceso < 10; proceso++ ) {
      msgrcv(cola, &pi, sizeof(pi), 1, IPC_NOWAIT);
      resultado+= pi.casiPi;
   }

   msgctl(cola, IPC_RMID, NULL);
   printf( "Valor calculado de Pi es \033[91m %f \033[0m con %ld terminos\n", resultado, terminos );

}
