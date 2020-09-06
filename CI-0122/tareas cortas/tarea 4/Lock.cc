#include "Lock.h"


/**
 * Crea un mutex para ser usado junto con una variable de condición.
 */

Lock::Lock() {

   this->lock = (pthread_mutex_t *) calloc(1, sizeof(pthread_mutex_t));

   int ret = pthread_mutex_init(this->lock, NULL);

   if(0 != ret) {
      perror("Lock::Lock");
      exit(1);
   }

}


/**
 * Destruye el mutex y libera el espacio asignado.
 */

Lock::~Lock() {

   int ret = pthread_mutex_destroy(this->lock);

   if(0 != ret) {
      perror("Lock::~Lock");
      exit(1);
   }

   free(this->lock);

}


/**
 * Un hilo accede al mutex, bloqueando al resto.
 * @return 0.
 */

int Lock::Acquire() {
   
   int ret = pthread_mutex_lock(this->lock);
   
   if(0 != ret) {
      perror("Lock::Acquire");
      exit(1);
   }

   return ret;

}


/**
 * Un hilo libera el mutex.
 * @return 0.
 */

int Lock::Release() {
   
   int ret = pthread_mutex_unlock(this->lock);

   if(0 != ret) {
      perror("Lock::Release");
      exit(1);
   }

   return ret;

}


/**
 * Envía el mutex cuando se utiliza con una variable de condición.
 * @return puntero al mutex.
 */

pthread_mutex_t * Lock::getLock() {

   return this->lock;

}
