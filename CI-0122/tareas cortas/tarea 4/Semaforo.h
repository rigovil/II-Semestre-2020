/*  Esta clase encapsula las funciones para la utilizacion de semaforos
 *  Se provee las estructuras para realizar la sincronizacion de trabajadores
 *  a traves de los metodos tradicionales Signal (V), Wait (P)
 *
 *  Autor: CI-0123 Proyecto integrador de redes y sistemas operativos
 *  Fecha: 2020/Ago/31
 *  Repaso: material de concurrencia
 *
**/

#ifndef _SEM_H
#define _SEM_H

#include <semaphore.h>

class Semaforo {
public:
   Semaforo( int = 1 );
   ~Semaforo();
   int Signal();
   int Wait();
   int tryWait();
   int timedWait( long = 0, long = 0 );

private:
   sem_t * semId;

};

#endif
