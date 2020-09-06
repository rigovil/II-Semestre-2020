/*
 *  Primer ejemplo para PThreads
 *
 *  Autor: CI-0123 Proyecto integrador de redes y sistemas operativos
 *  Fecha: 2020/Ago/31
 *  Repaso: material de concurrencia
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 

/* Global variable:  accessible to all threads */
int thread_count;  

void *Hello(void* rank);  /* Thread function */


int main( int argc, char* argv[] ) {
   long       thread;
   pthread_t* thread_handles; 

   thread_count = 1;
   if ( argc > 1 ) {
      thread_count = strtol( argv[1], NULL, 10 );
   }

   thread_handles = malloc (thread_count*sizeof(pthread_t)); 

   for ( thread = 0; thread < thread_count; thread++ ) {  
      pthread_create( &thread_handles[ thread ], NULL, Hello, (void*) thread );
   }

   printf( "Hello from the main thread\n" );

   for ( thread = 0; thread < thread_count; thread++ ) { 
      pthread_join(thread_handles[thread], NULL);
   }

   free( thread_handles );

   return 0;

}

void hola() {
   printf( "Otro metodo\n" );
}

void *Hello( void* rank ) {
   long my_rank = (long) rank;

   printf("Hello from thread %ld of %d\n", my_rank, thread_count);
   hola();

   return NULL;
}
