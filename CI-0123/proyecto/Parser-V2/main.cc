#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
#include <semaphore.h>
#include <sys/time.h>
#include "Reader.h"
#include "Parser.h"


#define NUMBER_SERVERS  1               /* Valor utilizado para indicar el número de servidores a los que se les va a solicitar información. */
#define FILE_NAME       "covid.txt"     /* Valor utilizado para indicar el nombre del archivo donde se encuentra el código fuente del servidor. */
#define WORKERS         4               /* Valor utilizado para indicar la cantidad de trabajadores que van a analizar el código fuente de un servidor. */
#define SEM_ON          1               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 1. */
#define SEM_OFF         0               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 0. */
#define PSHARED         0               /* Valor utilizado para indicar que los semáforos se comparten entre los hilos de un proceso. */
#define ZERO            0               /* Valor utilizado en las estructuras de control if y for. */
#define EXIT            0               /* Valor utilizado como parámetro para salir del programa en caso de error. */
#define ERROR           -1              /* Valor utilizado para identificar un error en alguna función. */
#define CR              "costa rica"    /* Valor utilizado para identificar a Costa Rica. */
#define CA              "centroamerica" /* Valor utilizado para identificar la región centroamericana. */
#define DELIMITER       ','             /* Valor utilizado para delimitar los datos de un cantón/región/país. */
#define NEW_LINE        '\n'            /* Valor utilizado para almacenar un caracter de salto de línea. */
#define ERASE1          '"'             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define ERASE2          '['             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define ERASE3          ']'             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */


void serverReader(std::string, int);
void serverParser(Reader *, Parser *, int);
void covidData(std::string);


/**
 * Punto de entrada del programa al Parser, crea tantos hilos como solicitudes a servidores hayan,
 * los elimina cuando estos hayan terminado su trabajo.
 * @param argc cantidad de parámetros enviados por línea de comandos.
 * @param argv array con strings que almacenan lo indicado por los parámetros.
 * @return 0.
 */

int main(int argc, char ** argv) {

    std::vector<std::string> server_address;
    std::vector<int> workers;
    
    std::thread servers[NUMBER_SERVERS];
    server_address.push_back(FILE_NAME);
    workers.push_back(WORKERS);


    /**
     * 
     * Aquí va el código que le pide al usuario si quiere ver la info de un cantón específico o de Costa Rica.
     * Falta implementar el método covidData que es el que despliega la información de los datos.
     * 
     */


    /**
     * 
     * Aquí va el código que verifica si ya hay un archivo en disco que contiene la información 
     * que pide el usuario a partir de la fecha en que lo solicita.
     * Si ese archivo ya existe entonces los siguientes dos ciclos for no se ejecutarían.
     *
     */


    for(int i = ZERO; i < NUMBER_SERVERS; ++i) {

        servers[i] = std::thread(serverReader, server_address[i], workers[i]);

    }

    for(int i = ZERO; i < NUMBER_SERVERS; ++i) {

        servers[i].join();

    }


    /**
     * 
     * Aquí se llama a la función covidData() que despliega la información en formato de tabla.
     * 
     */

    return 0;

}


/**
 * Se encarga de preparar los datos necesarios para solicitar información a un servidor en específico,
 * solicita el código fuente de la dirección del servidor y crea los hilos para que distintos trabajadores
 * encuentren la información relacionada con los datos del COVID-19 y sea desplegada al usuario.
 * También escribe un archivo a disco con los datos del COVID-19 de cada región.
 * @param server_address dirección del servidor donde será sacada la información.
 * @param workers cantidad de trabajadores que van a analizar el código fuente.
 */

void serverReader(std::string server_address, int workers) {

    int i, result;
    std::thread threads[workers];
    std::vector<std::string>::iterator it;
    std::ofstream file;

    sem_t *semaphores   = new sem_t[workers];
    Parser *parser      = new Parser();
    Reader *reader      = new Reader(server_address, workers, semaphores);

    for(i = ZERO; i < workers; ++i) {

        if(i == ZERO) {
            result = sem_init(&semaphores[i], PSHARED, SEM_ON);
        }
        else {
            result = sem_init(&semaphores[i], PSHARED, SEM_OFF);
        }

        if(ERROR == result) {
            perror("main:serverReader:sem_init");
            exit(EXIT);
        }

        threads[i] = std::thread(serverParser, reader, parser, i);

    }

    for(i = ZERO; i < workers; ++i) {

        threads[i].join();
        result = sem_destroy(&semaphores[i]);

        if(ERROR == result) {
            perror("main:serverReader:sem_destroy");
            exit(EXIT);
        }

    }

    file.open("test.txt");      /* Nombre de prueba para el archivo que se crea en el disco */

    for(it = parser->regions.begin(); it != parser->regions.end(); ++it) {

        int i = ZERO;
        std::string data;
        std::stringstream stream(*it);

        while(getline(stream, data, DELIMITER)) {

            if(!i) {
                data.erase(std::remove(data.begin(), data.end(), ERASE1), data.end());      /* Borra las comillas " */
                data.erase(std::remove(data.begin(), data.end(), ERASE2), data.end());      /* Borra el paréntesis cuadrado [ */
                file << data << DELIMITER;
            }
            else {
                data.erase(std::remove(data.begin(), data.end(), ERASE3), data.end());      /* Borra el paréntesis cuadrado ] */
                file << data << DELIMITER;
            }

            ++i;

        }

        file << NEW_LINE;

    }

    file.close();

    delete semaphores;
    delete parser;
    delete reader;

}


/**
 * A partir de un trabajador en específico, solicita al reader las líneas del código fuente del servidor para ser analizadas.
 * Este proceso se repite tantas veces mientras queden líneas disponibles por analizar del código fuente.
 * @param reader puntero al objeto de la clase Reader que le asigna líneas a los trabajadores para que las analicen.
 * @param parser puntero al objeto de la clase Parser que analiza una línea del código fuente para buscar la información solicitada del COVID-19.
 * @param worker número de trabajador que solicita y analiza las líneas.
 */

void serverParser(Reader * reader, Parser * parser, int worker) {

    std::string line;

    while(reader->hasNext(worker)) {
        line = reader->getNext(worker);
        parser->readLine(line);
    }

}


/**
 * Muestra información del COVID-19 en la región solicitada por el usuario.
 * @param region region solicitada por el usuario.
 */

void covidData(std::string region) {

    if(region == CR) {
        std::cout << "Datos del COVID-19 en Costa Rica:\n\n";
    }
    else {
        std::cout << "Datos del COVID-19 en " << region << ":\n\n";
    }

}