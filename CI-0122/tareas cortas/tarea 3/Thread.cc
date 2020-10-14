/*  Esta clase encapsula las funciones para la utilizacion de PThreads
 *  Se provee las estructuras para crear los trabajadores (hilos)
 *
 *  Autor: CI-0123 Proyecto integrador de redes y sistemas operativos
 *  Fecha: 2020/Ago/31
 *  Repaso: material de concurrencia
 *
**/

#include <stdlib.h>
#include <stdio.h>

#include "Thread.h"


/*
 *
 */
Thread::Thread() {

   this->thread = (pthread_t *) calloc( 1, sizeof( pthread_t ) );
   this->attr   = (pthread_attr_t *) calloc( 1, sizeof( pthread_attr_t ) );

}


/*
 *
 */
Thread::~Thread() {

   if ( NULL != this->thread ) {
      free( this->thread );
   }

   if ( NULL != this->attr ) {
      free( this->attr );
   }

   this->thread = NULL;
   this->attr = NULL;

}


/*
 *  This method will fork "forkedRoutine" which need to be implemented by the class inhereting from this one
 *  On this base class "forkedRoutine" is declared as virtual
 *
 */
int Thread::Fork( void *(*startRoutine) (void *), void *arg ) {
   int resultado;

   resultado = pthread_create( this->thread, NULL, startRoutine, arg );
   if ( resultado > 0 ) {
      perror( "Thread::Fork" );
      exit( 1 );
   }

   return resultado;

}


/*
 *
 */
void * Thread::Join() {
   void * retval;

   pthread_join( *this->thread, &retval );

   return retval;

}


/*
 *
 */
void Thread::Exit( void * result ) {

   pthread_exit( result );

}
