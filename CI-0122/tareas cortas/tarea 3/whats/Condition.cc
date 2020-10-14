#include "Condition.h"


Condition::Condition() {
   this->vc = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
   pthread_cond_init(this->vc, NULL);
}


Condition::~Condition() {
   pthread_cond_destroy(this->vc);
   free(this->vc);
}


int Condition::Wait( Lock * conditionLock ) {
   return pthread_cond_wait(this->vc, conditionLock->getLock());
}


int Condition::TimedWait( Lock * conditionLock, long sec, long nsec ) {
   struct timespec abstime;
   abstime.tv_sec = sec;
   abstime.tv_nsec = nsec;

   return pthread_cond_timedwait(this->vc, conditionLock->getLock(), (const struct timespec *) &abstime);
}


int Condition::Signal() {
   return pthread_cond_signal(this->vc);
}


int Condition::Broadcast() {
   return pthread_cond_broadcast(this->vc);
}