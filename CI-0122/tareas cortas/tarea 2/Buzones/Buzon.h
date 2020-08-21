/*
   C++ class to encapsulate Unix message passing intrinsic structures and
   system calls

*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY 0xB78292
#define MSG_SIZE 32

class Buzon {
   public:
      Buzon();
      ~Buzon();
      int Enviar( char *mensaje );
      int Enviar( char *mensaje, int cantidad, long tipo = 1 );
      int Recibir( void *mensaje, int len, long tipo = 1 );
   private:
      int id;		
};
