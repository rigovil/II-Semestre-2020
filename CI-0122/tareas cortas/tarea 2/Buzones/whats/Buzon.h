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

#define KEY 0x12345
#define SIZE 64

class Buzon {
   public:
      Buzon( bool );
      int Enviar( const char *mensaje );
      int Enviar( const char *mensaje, int cantidad, long tipo = 1 );
      int Recibir( void *mensaje, int len, long tipo = 1 );
   private:
      int id;		
};
