

/*  Esta clase encapsula las funciones para la utilizacion de PThreads
 *  Se provee las estructuras para crear los trabajadores (hilos)
 *
 *  Define un metodo virtual "startRoutine" que las otras clases que hereden de esta deben implantar
 *  Este metodo virtual es el que ejecutaran los hilos una vez que son creados
 *  El programador que hereda de esta clase es el encargado de definir "startRoutine"
 *
 *  Autor: CI-0123 Proyecto integrador de redes y sistemas operativos
 *  Fecha: 2020/Ago/31
 *  Repaso: material de concurrencia
 *
**/

#ifndef _THREADBASE_H
#define _THREADBASE_H

#include <pthread.h>


class ThreadBase {
public:
   ThreadBase();
   ~ThreadBase();
   int Fork();
   void * Join();
   void Exit( void * );
   

private:
   void virtual startRoutine() = 0;

/*
 *  This routine is used to encapsulate PThreads call to start new thread
 *  It has the general form of a PThreads routine employed "pthread_create" function ( void * func( void * ) )
 *  On the Fork method, a reference to "this" is passed as parameter, which is reinterpreted as a reference to an object of this class
 *  
 */
   static void* ThreadBaseHelper( void * arg ) {

      ThreadBase * threadBase = reinterpret_cast<ThreadBase*>(arg);
      threadBase->startRoutine();	// Same as: ((ThreadBase *) arg)->startRoutine();
      threadBase->Exit( 0 );

      return 0;

}

// Class variables
   pthread_t * thread;
   pthread_attr_t * attr;

};

#endif


