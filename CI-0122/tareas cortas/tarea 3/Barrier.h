#ifndef _BARRIER_H
#define _BARRIER_H

#include <stdio.h>
#include <cstdlib>
#include <pthread.h>

class Barrier {

   public:

      Barrier(unsigned int);
      ~Barrier();
      void Wait();

   private:

      pthread_barrier_t * barrier;
      pthread_barrierattr_t * attr;
      unsigned int count;

};

#endif