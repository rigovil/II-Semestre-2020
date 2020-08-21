/*
 * Calcula el numero PI utilizando una serie "infinita"
 * Debe recibir la cantidad de iteraciones como parametro
 *
 *   pi = (-1)^i x 4/(2xi + 1)
 *
 *  Autor: Programacion Paralela y Concurrente (Francisco Arroyo)
 *  Fecha: 2020/Ago/08
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define KEY 0xB78292
#define ARRAY_SIZE 10

/*
   Realiza la acumulacion de terminos desde una posicion inicial hasta un termino final
*/
double calcularSumaParcialPi( double Pi[], int proceso, long inicial, long terminos ) {
   double casiPi = 0;
   double alterna = 4;
   long divisor = 0;
   long termino;

   for ( termino = inicial; termino < terminos; termino++ ) {
      divisor = termino + termino + 1;		// 2 x termino + 1
      casiPi += alterna/divisor;		// 4 / (2xi + 1)
      alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
   }
   Pi[ proceso ] = casiPi;			// Guarda el resultado en el vector y finaliza
   exit( 0 );
}


int main( int argc, char ** argv ) {
   long terminos, inicio, fin;
   int proceso, i;
   int pid, id_ShM;
   double *casiPi;
   // double casiPi[ 10 ] = { 0 };

   id_ShM = shmget(KEY, sizeof(double)*ARRAY_SIZE, IPC_CREAT | 0666);
   casiPi = (double *) shmat(id_ShM, 0, 0);

   terminos = 1000000;
   if ( argc > 1 ) {
      terminos = atol( argv[ 1 ] );
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      inicio = proceso * terminos/10;
      fin = (proceso + 1) * terminos/10;
      pid = fork();
      if ( ! pid ) {
         calcularSumaParcialPi( casiPi, proceso, inicio, fin );
      } else {
         printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
      }
   }

   for ( proceso = 0; proceso < 10; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }

   for ( proceso = 1; proceso < 10; proceso++ ) {
      casiPi[ 0 ] += casiPi[ proceso ];
   }

   printf( "Valor calculado de Pi es \033[91m %f \033[0m con %ld terminos\n", casiPi[ 0 ], terminos );

   shmdt(casiPi);
   shmctl(id_ShM, IPC_RMID, NULL);
}
