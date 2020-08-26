#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>
#include <semaphore.h>
#include "FileReader.h"
#include "ParserHTML.h"

#define PARAMETROS 3

void maestroLector(std::string, int, int);
void contarEtiquetas(FileReader *, ParserHTML *, int);

int main(int argc, char ** argv) {
    int NUMERO_ARCHIVOS;
    std::vector<std::string> archivos;
    std::vector<int> trabajadores;
    std::vector<int> estrategias;

    std::cout << "Ingrese el numero de archivos: ";
    std::cin >> NUMERO_ARCHIVOS;

    while(!std::cin || NUMERO_ARCHIVOS <= 0) {
        std::cout << "Ingrese el numero de archivos: ";
        std::cin >> NUMERO_ARCHIVOS;
    }
    
    std::thread lectores[NUMERO_ARCHIVOS];

    for(int i = 0; i < NUMERO_ARCHIVOS; ++i) {
        std::string archivo;
        int cantidad, estrategia;

        std::cout << "\nIngrese el nombre del archivo: ";
        std::cin >> archivo;
        archivos.push_back(archivo);

        std::cout << "Ingrese la cantidad de trabajadores: ";
        std::cin >> cantidad;
        trabajadores.push_back(cantidad);

        while(trabajadores[i] < 1) {
            std::cout << "Ingrese la cantidad de trabajadores: ";
            std::cin >> trabajadores[i];
        }

        std::cout << "Ingrese la estrategia: (1 = bloques, 2 = ciclico, 3 = dinamico, 4 = personalizado): ";
        std::cin >> estrategia;
        estrategias.push_back(estrategia);

        while(estrategias[i] < 1 || estrategias[i] > 4) {
            std::cout << "Ingrese la estrategia: (1 = bloques, 2 = ciclico, 3 = dinamico, 4 = personalizado): ";
            std::cin >> estrategias[i];
        }
    }   

    for(int i = 0; i < NUMERO_ARCHIVOS; ++i) {
        lectores[i] = std::thread(maestroLector, archivos[i], trabajadores[i], estrategias[i]);
    }

    for(int i = 0; i < NUMERO_ARCHIVOS; ++i) {
        lectores[i].join();
    }

    return 0;   
}

void maestroLector(std::string archivo, int cantidadTrabajadores, int estrategia) {
    clock_t start = clock();

    std::thread trabajadores[cantidadTrabajadores];
    sem_t *semaforos = new sem_t[cantidadTrabajadores];
    std::mutex *mutex = new std::mutex;

    ParserHTML *parser = new ParserHTML();
    FileReader *lector = new FileReader(archivo, cantidadTrabajadores, estrategia, mutex, semaforos);

    for(int i = 0; i < cantidadTrabajadores; ++i) {
        if(i == 0) {
            sem_init(&semaforos[i], 0, 1);
        }
        else {
            sem_init(&semaforos[i], 0, 0);
        }

        trabajadores[i] = std::thread(contarEtiquetas, lector, parser, i);
    }

    for(int i = 0; i < cantidadTrabajadores; ++i) {
        trabajadores[i].join();
        sem_destroy(&semaforos[i]);
    }

    clock_t end = clock();

    double time = (double)(end-start) / CLOCKS_PER_SEC;

    int k = 0;
    std::map<std::string, std::atomic_int>::iterator it;
    std::cout << "\nETIQUETAS DEL ARCHIVO \"" << archivo << "\" Y TIEMPO DE DURACIÓN\n\n";
    std::cout << "Tiempo de procesamiento: " << time << "\n\n";

    for(it = parser->etiquetas_HTML.begin(); it != parser->etiquetas_HTML.end(); ++it) {
        std::cout << it->first << " --> " << it->second << "\t\t";
        if((k+1) % 4 == 0) {
            std::cout << "\n";
        }
        ++k;
    }

    std::cout << "\n";

    delete semaforos;
    delete mutex;
    delete parser;
    delete lector;
}

void contarEtiquetas(FileReader *lector, ParserHTML *parser, int trabajador) {
    std::string linea;

    while(lector->hasNext(trabajador)) {
        linea = lector->getNext(trabajador);
        parser->leerLinea(linea);
    }
}