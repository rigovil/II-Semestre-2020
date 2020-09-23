#include "Reader.h"


/**
 * Crea una instancia de la clase Reader, que es la que posee la información del servidor que se está anlizando,
 * para solicitar su código fuente, así como la cantidad de trabajadores que son los que se encarga de solicitar
 * y analizar las líneas del código fuente, asigna igualmente semáforos a cada trabajador para que no hayan líneas
 * que se analicen más de una vez. Le asigna a cada trabajador la primera línea que debe leer y analizar.
 * La asignación de líneas se hace a partir de una estrategia de mapeo cíclico, es decir: entrega al primer trabajador (0) 
 * todas las líneas cuyo resto de dividir el número de esa línea entre t sea 0; al segundo trabajador (1) 
 * le entrega todas las líneas cuyo resto de dividir el número de esa línea entre t sea 1 y así sucesivamente. t = cantidad de trabajadores.
 * @param sc_file_name nombre del archivo donde se encuentra el código fuente.
 * @param workers cantidad de trabajadores que analizan el código fuente.
 * @param semaphores array de semáforos donde cada uno le corresponde a un trabajador.
 */

Reader :: Reader(std::string sc_file_name, int workers, sem_t * semaphores) {

    this->workers       = workers;
    this->next_line     = new int[workers];
    this->semaphores    = semaphores;

    source_code.open(sc_file_name, std::fstream::in);
    std::string line;

    while(std::getline(source_code, line)) {

        total_lines++;

    }

    source_code.clear();
    source_code.seekg(ZERO, source_code.beg);

    for(int i = ZERO; i < workers; ++i) {

        next_line[i] = i;

    }

}


/**
 * Destructor de la instancia, devuelve el espacio asignado para el array de líneas asignadas por trabajador.
 */

Reader :: ~Reader() {

    delete next_line;
    source_code.close();

}


/**
 * Le indica a un trabajador si este todavía posee líneas del código fuente por leer y analizar.
 * @param worker número del trabajador.
 * @return un valor booleano indicando si al trabajador le restan líneas por analizar o no.
 */

bool Reader :: hasNext(int worker) {

    return (next_line[worker] < total_lines);

}


/**
 * Solicita la siguiente línea del código fuente a partir de un número de trabajador dado.
 * Le avisa al siguiente trabajador, utilizando semáforos, que es su turno de solicitar la siguiente línea del código fuente siguiendo el mapeo cíclico.
 * @param worker número de trabajador que va a leer la línea.
 * @return std::string con la línea correspondiente del código fuente.
 */

std::string Reader :: getNext(int worker) {

    int result;
    std::string line;

    result = sem_wait(&semaphores[worker]);

    if(ERROR == result) {
        perror("Reader:getNext:sem_wait");
        exit(EXIT);
    }

    std::getline(source_code, line);

    ++lines_read;
    next_line[worker] += workers;

    if(worker != workers - ONE) {
        result = sem_post(&semaphores[worker + ONE]);
    }
    else {
        result = sem_post(&semaphores[ZERO]);
    }

    if(ERROR == result) {
        perror("Reader:getNext:sem_post");
        exit(EXIT);
    }

    return line;

}