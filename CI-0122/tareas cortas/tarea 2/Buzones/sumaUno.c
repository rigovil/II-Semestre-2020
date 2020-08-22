/*
 * Suma uno a un total mil veces por cada proceso generado
 * Recibe como parametro la cantidad de procesos que se quiere arrancar
 * Author: Programacion Concurrente (Francisco Arroyo)
 * Version: 2020/Ago/08
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf {
   long mtype;
   long suma;
};

long total = 0;

/*
 *  Do some work, by now add one to a variable
 */
long AddOne( int queue ) {

   int i, send;
   long mi_suma = 0;
   struct my_msgbuf msg;

   for ( i = 0; i < 1000; i++ ) {
      usleep( 1 );
      mi_suma++;			// Suma uno
   }

   msg.mtype = 1;
   msg.suma = mi_suma;
   send     = msgsnd(queue, (void *) &msg, sizeof(msg), IPC_NOWAIT);

   if(-1 == send) {
      perror("msgsnd");
   }

   exit( 0 );

}


/*
  Serial test
*/
int SerialTest( int procesos, long * total ) {

   int i, proceso;

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      for ( i = 0; i < 1000; i++ ) {
         (* total)++;			// Suma uno
      }
   }

}


/*
  Fork test
*/
int ForkTest( int procesos, long * total, int queue ) {

   int proceso, pid, receive;
   struct my_msgbuf msg;

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      pid = fork();
      if ( ! pid ) {
         AddOne( queue );
      }
   }

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
      receive   = msgrcv(queue, &msg, sizeof(msg), 1, IPC_NOWAIT);
      
      if(-1 == receive) {
         perror("msgrcv");
         exit(0);
      }

      (*total) += msg.suma;
   }

}

/*
 *
 */
void startTimer( struct timeval * timerStart) {
   gettimeofday( timerStart, NULL );
}


/*
 *  time elapsed in ms
 */
double getTimer( struct timeval timerStart ) {
   struct timeval timerStop, timerElapsed;

   gettimeofday(&timerStop, NULL);
   timersub(&timerStop, &timerStart, &timerElapsed);
   return timerElapsed.tv_sec*1000.0+timerElapsed.tv_usec/1000.0;
}


int main( int argc, char ** argv ) {

   long procesos;
   int proceso, queue;
   struct timeval timerStart;
   double used;

   queue = msgget(0xB78292, 0666 | IPC_CREAT);

   if(-1 == queue) {
      perror("msgget");
      exit(0);
   }

   procesos = 100;
   if ( argc > 1 ) {
      procesos = atol( argv[ 1 ] );
   }

   startTimer( & timerStart );
   total = 0;
   SerialTest( procesos, & total );
   used = getTimer( timerStart );
   printf( "Serial version:      Valor acumulado por %ld procesos es \033[91m %ld \033[0m en %f ms\n", procesos, total, used );

   startTimer( & timerStart );
   total = 0;
   ForkTest( procesos, & total, queue );
   used = getTimer( timerStart );
   printf( "Fork   version:      Valor acumulado por %ld procesos es \033[91m %ld \033[0m en %f ms\n", procesos, total, used );

   msgctl(queue, IPC_RMID, NULL);

}
