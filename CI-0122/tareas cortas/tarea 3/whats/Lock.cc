#include "Lock.h"


Lock::Lock() {
   this->lock = (pthread_mutex_t *) calloc(1, sizeof(pthread_mutex_t));
   pthread_mutex_init(this->lock, NULL);
}


Lock::~Lock() {
   pthread_mutex_destroy(this->lock);
   free(this->lock);
}


int Lock::Acquire() {
   return pthread_mutex_lock(this->lock);
}


int Lock::Release() {
   return pthread_mutex_unlock(this->lock);
}


pthread_mutex_t * Lock::getLock() {
   return this->lock;
}
