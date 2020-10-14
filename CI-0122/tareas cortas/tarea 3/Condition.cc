#include "Condition.h"


/**
 * Crea una variable de condición para hilos POSIX con los atributos predefinidos.
 */

Condition::Condition() {

   this->vc = (pthread_cond_t *) calloc(1, sizeof(pthread_cond_t));

   int ret = pthread_cond_init(this->vc, NULL);

   if(0 != ret) {
      perror("Condition::Condition");
      exit(1);
   }

}


/**
 * Destruye la variable de condición y libera el espacio asignado.
 */

Condition::~Condition() {

   int ret = pthread_cond_destroy(this->vc);

   if(0 != ret) {
      perror("Condition::~Condition");
      exit(1);
   }

   free(this->vc);

}


/**
 * Bloquea un hilo en la variable de condición.
 * @param conditionLock puntero a una instancia de la clase Lock.
 * @return 0.
 */

int Condition::Wait( Lock * conditionLock ) {
   
   int ret = pthread_cond_wait(this->vc, conditionLock->getLock());

   if(0 != ret) {
      perror("Condition::Wait");
      exit(1);
   }

   return ret;

}


/**
 * Bloquea un hilo en la variable de condición hasta un tiempo definido.
 * @param conditionLock puntero a una instancia de la clase Lock.
 * @param sec segundos.
 * @param nsec nanosegundos.
 * @return 0.
 */

int Condition::TimedWait( Lock * conditionLock, long sec, long nsec ) {
   
   struct timespec abstime;
   abstime.tv_nsec   = nsec;
   abstime.tv_sec    = sec;

   int ret = pthread_cond_timedwait(this->vc, conditionLock->getLock(), (const struct timespec *) &abstime);

   if(0 != ret) {
      perror("Condition::TimedWait");
      exit(1);
   }

   return ret;

}


/**
 * Desbloquea un hilo bloqueado por la variable de condición siguiendo una política predefinida.
 * @return 0.
 */

int Condition::Signal() {
   
   int ret = pthread_cond_signal(this->vc);

   if(0 != ret) {
      perror("Condition::Signal");
      exit(1);
   }

   return ret;

}


/**
 * Desbloquea todos los hilos bloqueados por la variable de condición.
 * @return 0.
 */

int Condition::Broadcast() {
   
   int ret = pthread_cond_broadcast(this->vc);

   if(0 != ret) {
      perror("Condition::Broadcast");
      exit(1);
   }

   return ret;

}