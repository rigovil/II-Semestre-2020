#ifndef _LOCK_H
#define _LOCK_H

#include <stdio.h>
#include <cstdlib>
#include <pthread.h>

class Lock {
   public:
      Lock();
      ~Lock();
      int Acquire();
      int Release();
      pthread_mutex_t * getLock();

   private:
      pthread_mutex_t * lock;

};

#endif
