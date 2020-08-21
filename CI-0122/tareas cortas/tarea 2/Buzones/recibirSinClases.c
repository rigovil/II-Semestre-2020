#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define KEY 0xA12345
#define LABEL_SIZE 64


int main( int argc, char ** argv ) {


struct msgbuf {
       long mtype;     // message type, must be > 0 
       int times;	// Times that label appears
       char label[ LABEL_SIZE ];  // Label to send to mailbox
};

   struct msgbuf A;
   int id, i, size, st;

   id = msgget( KEY, 0666 );
   if ( -1 == id ) {
      perror("t0-recibe: ");
      exit(1);
   }

   size = sizeof( A );
   i = 0;

   st = msgrcv( id,  &A, size, 2019, IPC_NOWAIT );
   while ( st > 0 ) {
      printf("Label: %s, times %d \n", A.label, A.times );
      st = msgrcv( id,  &A, size, 2018, IPC_NOWAIT );
   }

   msgctl( id, IPC_RMID, NULL );

}
