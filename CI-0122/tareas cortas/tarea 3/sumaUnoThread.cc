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
#include "ThreadBase.h"
#include "Thread.h"
#include "Mutex.h"


// Shared variables
long total;
Mutex * mutex;



/*
 *  Esta clase hereda de la clase ThreadBase, la cual encapsula la funcionalidad de hilos por medio de PThreads
 *  La clase ThreadBase define un metodo virtual que debe ser implantado para poder utilizarla
 *  El metodo virtual es precisamente lo que vamos a ejecutar en los hilos que se crean
 *  
 */
class MisHilos : public ThreadBase {

public:
   MisHilos( long number ) {
      this->number = number;
   }

   long getNumber() {
      return this->number;
   }


private:

/*
 *  Esta es la rutina que van a llamar los hilos cuando se crean "Fork"
 *  La clase esta definida para la prueba de sumar utilizando un mutex
 *
 */
   void startRoutine() {
      long myTotal = 0;
      int i;

      for ( i = 0; i < 1000; i++ ) {
         myTotal++;
      }

      mutex->Lock();
      total += myTotal;
      mutex->Unlock();

   }

// Class variables
   long number;

};


/*
 *  Do some work, by now add one to a variable
 */
void * AddOneWithMutex( void * param ) {
   int i;
   long myTotal = 0;

   for ( i = 0; i < 1000; i++ ) {
      myTotal++;
   }

   mutex->Lock();
   total += myTotal;
   mutex->Unlock();

   return 0;
//   pthread_exit( 0 );

}


/*
 *  Do some work, by now add one to a variable
 */
void * AddOne( void * param ) {
   int i;

   for ( i = 0; i < 1000; i++ ) {
      total++;
   }

   return 0;
//   pthread_exit( 0 );

}


/*
  Serial test
*/
long SerialTest( long hilos ) {
   long i, hilo;

   for ( hilo = 0; hilo < hilos; hilo++ ) {

      for ( i = 0; i < 1000; i++ ) {
         total++;			// Suma uno
      }

   }

   return total;

}


/*
  Fork test with NO race condition
*/
long ForkTestNoRaceCondition( long hilos ) {
   long hilo;
   MisHilos ** threads;	// Thread vector used for joining threads

   threads = (MisHilos **) calloc( hilos, sizeof( MisHilos ) );
   mutex = new Mutex();

   for ( hilo = 0; hilo < hilos; hilo++ ) {
      threads[ hilo ] = new MisHilos( hilo );
      threads[ hilo ]->Fork();
   }

   for ( hilo = 0; hilo < hilos; hilo++ ) {	// Joining Threads is required for correct sum calculation
      threads[ hilo ]->Join();
      delete threads[ hilo ];
   }

   delete mutex;

   free( threads );

   return total;

}


/*
  Fork test with race condition
*/
long ForkTestRaceCondition( long hilos ) {
   long hilo;
   Thread ** threads;	// Thread vector used for joining threads

   threads = (Thread **) calloc( hilos, sizeof( Thread ) );

   for ( hilo = 0; hilo < hilos; hilo++ ) {
      threads[ hilo ] = new Thread();
      threads[ hilo ]->Fork( AddOne, (void *) hilo );
   }

   for ( hilo = 0; hilo < hilos; hilo++ ) {
      threads[ hilo ]->Join();
      delete threads[ hilo ];
   }

   free( threads );

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

