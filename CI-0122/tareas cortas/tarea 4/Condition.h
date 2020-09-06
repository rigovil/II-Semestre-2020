#ifndef _COND_H
#define _COND_H

#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include "Lock.h"

class Condition {

   public:

      Condition();
      ~Condition();
      int Wait( Lock * conditionLock );
      int TimedWait( Lock * conditionLock, long = 0, long = 0 );  // Sec, uSec
      int Signal();
      int Broadcast();

   private:

      pthread_cond_t * vc;

};

#endif