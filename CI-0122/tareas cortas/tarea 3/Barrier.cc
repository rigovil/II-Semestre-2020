#include "Barrier.h"


/** 
 * Crea una barrera para hilos POSIX con los atributos predefinidos.
 * @param count cantidad de hilos que deben llegar a la barrera antes de que todos puedan continuar.
 */

Barrier::Barrier(unsigned int count) {

    this->barrier = (pthread_barrier_t *) calloc(1, sizeof(pthread_barrier_t));
    this->count = count;

    int ret = pthread_barrier_init(this->barrier, this->attr, this->count);

    if(0 != ret) {
        perror("Barrier::Barrier");
        exit(1);
    }

}


/**
 * Destruye la barrera y libera el espacio asignado.
 */

Barrier::~Barrier() {

    int ret = pthread_barrier_destroy(this->barrier);

    if(0 != ret) {
        perror("Barrier::~Barrier");
        exit(1);
    }

    free(this->barrier);

}


/**
 * El hilo que llama a la función espera hasta que la cantidad designada de hilos
 * lleguen a la barrera y así hacer que todos puedan continuar.
 */

void Barrier::Wait() {

    int ret = pthread_barrier_wait(this->barrier);

    if(0 != ret && PTHREAD_BARRIER_SERIAL_THREAD != ret) {
        perror("Barrier::Wait");
        exit(1);
    }

}
