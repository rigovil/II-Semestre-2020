#include "Semaforo.h"
#include <stdio.h>

Semaforo::Semaforo(int valorInicial) {

   union semun {
      int val;
      struct semid_ds *buf;
      unsigned short *array;
      struct seminfo *__buf;
   } arg;

   arg.val = valorInicial;
   id = semget(KEY, 1, IPC_CREAT | 0666);
   semctl(id, 0, SETVAL, arg);

}


Semaforo::~Semaforo() {

   semctl(id, 0, IPC_RMID);

}

int Semaforo::Signal() {

   int op;
   struct sembuf sops_s = {0,1,0};
   op = semop(id, &sops_s, 1);
   
   return op;

}

int Semaforo::Wait() {
   
   int op;
   struct sembuf sops_w = {0,-1,0};
   op = semop(id, &sops_w, 1);

   return op;

}