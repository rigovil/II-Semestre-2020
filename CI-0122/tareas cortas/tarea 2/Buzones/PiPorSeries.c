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

struct my_msgbuf {
   long mtype;
   double casiPi;
};


double calcularSumaParcialPi( int proceso, long inicial, long terminos, int queue ) {

   struct my_msgbuf msg;
   double casiPi  = 0;
   double alterna = 4;
   long divisor   = 0;
   long termino;
   int send;

   for ( termino = inicial; termino < terminos; termino++ ) {
      divisor  = termino + termino + 1;		
      casiPi  += alterna/divisor;		
      alterna *= -1;				
   }

   msg.mtype   = 1;
   msg.casiPi  = casiPi;
   send        = msgsnd(queue, (const void *) &msg, sizeof(msg), IPC_NOWAIT);

   if(-1 == send) {
      perror("msgsnd");
   }

   exit( 0 );

}


int main( int argc, char ** argv ) {

   int proceso, pid, queue, receive;
   long terminos, inicio, fin;
   double resultado = 0;
   struct my_msgbuf msg;

   queue = msgget(0xB78292, 0666 | IPC_CREAT);

   if(-1 == queue) {
      perror("msgget");
      exit(0);
   }

   terminos = 1000000;
   if ( argc > 1 ) {
      terminos = atol( argv[ 1 ] );
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
      if ( ! pid ) {
         calcularSumaParcialPi( proceso, inicio, fin, queue );
      } else {
         printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
      }
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }

   for ( proceso = 1; proceso < 10; proceso++ ) {
      receive = msgrcv(queue, &msg, sizeof(msg), 1, IPC_NOWAIT);
      
      if(-1 == receive) {
         perror("msgrcv");
         exit(0);
      }

      resultado += msg.casiPi;
   }

   msgctl(queue, IPC_RMID, NULL);

   printf( "Valor calculado de Pi es \033[91m %f \033[0m con %ld terminos\n", resultado, terminos );

}
