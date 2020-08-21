/*
 *  C++ class to encapsulate Unix semaphore intrinsic structures and system calls
 *  Author: Programacion Concurrente (Francisco Arroyo)
 *  Version: 2020/Ago/08
 *
 * Ref.: https://en.wikipedia.org/wiki/Semaphore_(programming)
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define KEY 0xB78292

class Semaforo {
   public:
      Semaforo( int ValorInicial = 0 );
      ~Semaforo();
      int Signal();	// Puede llamarse V
      int Wait();	// Puede llamarse P
   private:
      int id;		// Identificador del semaforo
};

