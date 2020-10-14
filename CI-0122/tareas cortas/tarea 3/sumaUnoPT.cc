/*
 * Suma uno a un total mil veces por cada proceso generado
 * Recibe como parametro la cantidad de procesos que se quiere arrancar
 *
 *  Autor: CI-0123 Proyecto integrador de redes y sistemas operativos
 *  Fecha: 2020/Ago/31
 *  Repaso: material de concurrencia
 *
**/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "Mutex.h"

// Shared variables
long total;
Mutex * mutex;


/*
 *  Do some work, by now add one to a variable
 */
void * AddOneWithMutex( void * param ) {
   int i;
   long myTotal = 0;

   for ( i = 0; i < 1000; i++ ) {
      myTotal++;
//   usleep( 1 );
   }

   mutex->Lock();
   total += myTotal;
   mutex->Unlock();

   pthread_exit( 0 );

}


/*
 *  Do some work, by now add one to a variable
 */
void * AddOne( void * param ) {
   int i;

   for ( i = 0; i < 1000; i++ ) {
      total++;
//      usleep( 1 );
   }

   pthread_exit( 0 );

}


/*
  Serial test
*/
long SerialTest( long hilos ) {
   long i, hilo;

   for ( hilo = 0; hilo < hilos; hilo++ ) {

      for ( i = 0; i < 1000; i++ ) {
         total++;			// Suma uno
//         usleep( 1 );
      }

   }

   return total;

}


/*
  Fork test with NO race condition
*/
long ForkTestNoRaceCondition( long hilos ) {
   long hilo;
   pthread_t * pthilos;

   mutex = new Mutex();
   pthilos = (pthread_t *) calloc( hilos, sizeof( pthread_t ) );

   for ( hilo = 0; hilo < hilos; hilo++ ) {
      pthread_create( & pthilos[ hilo ], NULL, AddOneWithMutex, (void *) hilo );
   }

   for ( hilo = 0; hilo < hilos; hilo++ ) {
      pthread_join( pthilos[ hilo ], NULL );
   }

   free( pthilos );
   delete mutex;

   return total;

}

/*
  Fork test with race condition
*/
long ForkTestRaceCondition( long hilos ) {
   long hilo;
   pthread_t * pthilos;

   pthilos = (pthread_t *) calloc( hilos, sizeof( pthread_t ) );
   for ( hilo = 0; hilo < hilos; hilo++ ) {
      pthread_create( & pthilos[ hilo ], NULL, AddOne, (void *) hilo );
   }

   for ( hilo = 0; hilo < hilos; hilo++ ) {
      pthread_join( pthilos[ hilo ], NULL );
   }

   free( pthilos );

   return total;

}


int main( int argc, char ** argv ) {
   long hilos;
   clock_t start, finish;
   double used;


   hilos = 100;
   if ( argc > 1 ) {
      hilos = atol( argv[ 1 ] );
   }

   start = clock();
   total = 0;
   SerialTest( hilos );
   finish = clock();
   used = ((double) (finish - start)) / CLOCKS_PER_SEC;
   printf( "Serial version:      Valor acumulado es \033[91m %ld \033[0m con %ld hilos in %f seconds\n", total, hilos, used );

   start = clock();
   total = 0;
   ForkTestRaceCondition( hilos );
   finish = clock();
   used = ((double) (finish - start)) / CLOCKS_PER_SEC;
   printf( "PThr, Race Cond.:    Valor acumulado es \033[91m %ld \033[0m con %ld hilos in %f seconds\n", total, hilos, used );

   start = clock();
   total = 0;
   ForkTestNoRaceCondition( hilos );
   finish = clock();
   used = ((double) (finish - start)) / CLOCKS_PER_SEC;
   printf( "PThr, NO Race Cond.:  Valor acumulado es \033[91m %ld \033[0m con %ld hilos in %f seconds\n", total, hilos, used );
}
