#include "FileReader.h"
#include <thread>

#define MAPEO_BLOQUES 1
#define MAPEO_CICLICO 2
#define MAPEO_DINAMICO 3
#define MAPEO_PERSONALIZADO 4

FileReader :: FileReader(std::string nombreArchivo, int trabajadores, int estrategia, std::mutex *mutex, sem_t *semaforos) {
    this->trabajadores = trabajadores;
    this->estrategia = estrategia;
    this->siguienteLinea = new int[trabajadores];
    this->mutex = mutex;
    this->semaforos = semaforos;

    input.open(nombreArchivo, std::fstream::in);
    std::string linea;

    while(std::getline(input, linea)) {
        total_lineas++;
    }

    input.clear();
    input.seekg(0, input.beg);

    switch(estrategia) {
        
        case MAPEO_BLOQUES:
            for(int i = 0; i < trabajadores; ++i) {
                siguienteLinea[i] = (ceil((double)total_lineas/(double)trabajadores)*i);
            }
            break;

        case MAPEO_CICLICO:
            for(int i = 0; i < trabajadores; ++i) {
                siguienteLinea[i] = i;
            }
            break;
        
        case MAPEO_PERSONALIZADO:
            for(int i = 0; i < trabajadores; ++i) {
                siguienteLinea[i] = (total_lineas-i)-1;
            }
            break;
    }
}


FileReader :: ~FileReader() {
    delete siguienteLinea;
    input.close();
}

bool FileReader :: hasNext(int trabajador) {
    bool puedeLeer = false;

    switch(estrategia) {

        case MAPEO_BLOQUES:
            sem_wait(&semaforos[trabajador]);
            puedeLeer = siguienteLinea[trabajador] < total_lineas && siguienteLinea[trabajador] < (ceil((double)total_lineas/(double)trabajadores)*(trabajador+1));
            
            if(!puedeLeer) {
                sem_post(&semaforos[trabajador+1]);
            }
            else {
                sem_post(&semaforos[trabajador]);
            }
            break;
        
        case MAPEO_CICLICO:
            puedeLeer = siguienteLinea[trabajador] < total_lineas;
            break;

        case MAPEO_DINAMICO:
            puedeLeer = lineasLeidas < total_lineas;
            break;

        case MAPEO_PERSONALIZADO:
            sem_wait(&semaforos[trabajador]);
            puedeLeer = siguienteLinea[trabajador] >= 0;

            if(!puedeLeer) {
                sem_post(&semaforos[trabajador+1]);
            }
            else {
                sem_post(&semaforos[trabajador]);
            }
            break;
    }

    return puedeLeer;
}

std::string FileReader :: getNext(int trabajador) {
    std::string linea;

    switch(estrategia) {

        case MAPEO_BLOQUES: {
            std::getline(input, linea);
            ++siguienteLinea[trabajador];
            break;
        }

        case MAPEO_CICLICO: {
            sem_wait(&semaforos[trabajador]);
            std::getline(input, linea);
            siguienteLinea[trabajador]+= trabajadores;
            ++lineasLeidas;

            if(trabajador != trabajadores-1) {
                sem_post(&semaforos[trabajador+1]);
            }
            else {
                sem_post(&semaforos[0]);
            }
            break;
        }

        case MAPEO_DINAMICO: {
            mutex->lock();
            std::getline(input,linea);
            ++lineasLeidas;
            mutex->unlock();
            break;
        }

        case MAPEO_PERSONALIZADO: {
            std::getline(input, linea);
            siguienteLinea[trabajador]-= trabajadores;
            break;
        }
    }

    return linea;
}