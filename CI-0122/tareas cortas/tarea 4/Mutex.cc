/*  Implantacion de la clase Mutex utilizando PThreads
 *
 *  Autor: CI-0123 Proyecto integrador de redes y sistemas operativos
 *  Fecha: 2020/Ago/31
 *  Repaso: material de concurrencia
 *
 *  The  mutex  object  referenced by mutex shall be locked by a call to pthread_mutex_lock() that
 *  returns zero or [EOWNERDEAD].  If the mutex is already locked by another thread,  the  calling
 *  thread  shall  block  until  the mutex becomes available. This operation shall return with the
 *  mutex object referenced by mutex in the locked state with the calling thread as its owner.  If
 *  a  thread  attempts  to  relock a mutex that it has already locked, pthread_mutex_lock() shall
 *  behave as described in the Relock column of the following  table.  If  a  thread  attempts  to
 *  unlock  a  mutex  that  it has not locked or a mutex which is unlocked, pthread_mutex_unlock()
 *  shall behave as described in the Unlock When Not Owner column of the following table.
 *
 * ┌───────────┬────────────┬────────────────┬───────────────────────┐
 * │Mutex Type │ Robustness │     Relock     │ Unlock When Not Owner │
 * ├───────────┼────────────┼────────────────┼───────────────────────┤
 * │NORMAL     │ non-robust │ deadlock       │ undefined behavior    │
 * ├───────────┼────────────┼────────────────┼───────────────────────┤
 * │NORMAL     │ robust     │ deadlock       │ error returned        │
 * ├───────────┼────────────┼────────────────┼───────────────────────┤
 * │ERRORCHECK │ either     │ error returned │ error returned        │
 * ├───────────┼────────────┼────────────────┼───────────────────────┤
 * │RECURSIVE  │ either     │ recursive      │ error returned        │
 * │           │            │ (see below)    │                       │
 * ├───────────┼────────────┼────────────────┼───────────────────────┤
 * │DEFAULT    │ non-robust │ undefined      │ undefined behavior†   │
 * │           │            │ behavior†      │                       │
 * ├───────────┼────────────┼────────────────┼───────────────────────┤
 * │DEFAULT    │ robust     │ undefined      │ error returned        │
 * │           │            │ behavior†      │                       │
 * └───────────┴────────────┴────────────────┴───────────────────────┘
 *
 */

#include <stdlib.h>
#include <pthread.h>

#include "Mutex.h"

Mutex::Mutex() {
   int resultado;
   pthread_mutexattr_t * atributos;

   this->mutex = (pthread_mutex_t *) calloc( 1, sizeof( pthread_mutex_t ) );
   resultado = pthread_mutex_init( this->mutex, NULL );
   if ( 0 != resultado ) {
      exit( resultado );
   }

}


Mutex::~Mutex() {
   int resultado;

   resultado = pthread_mutex_destroy( this->mutex );
   if ( 0 != resultado ) {
      exit( resultado );
   }

   free( this->mutex );

}

int Mutex::Lock() {
   int resultado;

   resultado = pthread_mutex_lock( this->mutex );
   if ( 0 != resultado ) {
      exit( resultado );
   }

   return resultado;

}


/*
 *  If trying to lock an already locked mutex this call will return EBUSY (>0)
 *  Calling thread must check if lock was acquired or it must retry
 *  If mutex is unlocked this call will return 0 and calling thread must proceed
 *
 *  Warning: Other errors are not handled and are passed to calling thread
 */
int Mutex::TryLock() {
   int resultado;

   resultado = pthread_mutex_trylock( this->mutex );

   return resultado;

}


int Mutex::Unlock() {
   int resultado;

   resultado = pthread_mutex_unlock( this->mutex );
   if ( 0 != resultado ) {
      exit( resultado );
   }

   return resultado;

}
