// https://stackoverflow.com/questions/6755250/format-output-in-a-table-c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
#include <string>
#include <semaphore.h>
#include <sys/time.h>
#include "Reader.h"
#include "Parser.h"
#include "TextTable.h"


#define ERROR           -1              /* Valor utilizado para identificar un error en alguna función. */
#define PSHARED         0               /* Valor utilizado para indicar que los semáforos se comparten entre los hilos de un proceso. */
#define ZERO            0               /* Valor utilizado en las estructuras de control if y for. */
#define EXIT            0               /* Valor utilizado como parámetro para salir del programa en caso de error. */
#define SEM_OFF         0               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 0. */
#define SEM_ON          1               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 1. */
#define CR              1               /* Valor utilizado para identificar a Costa Rica. */
#define NUMBER_SERVERS  1               /* Valor utilizado para indicar el número de servidores a los que se les va a solicitar información. */
#define CANTON          2               /* Valor utilizado para identificar que se quiere buscar un cantón específico. */
#define WORKERS         4               /* Valor utilizado para indicar la cantidad de trabajadores que van a analizar el código fuente de un servidor. */
#define NAME_MAX_SIZE 	25				      /* Valor utilizado para indicar el length máximo del nombre de un lugar como entrada */
#define DELIMITER       ','             /* Valor utilizado para delimitar los datos de un cantón/región/país. */
#define NEW_LINE        '\n'            /* Valor utilizado para almacenar un caracter de salto de línea. */
#define ERASE1          '"'             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define ERASE2          '['             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define ERASE3          ']'             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define ERASE4          ' '             /* Valor utilizado para almacenar un caracter que se elimina de una región específica. */
#define RAW_FILE_NAME   "covid_raw.txt" /* Valor utilizado para indicar el nombre del archivo donde se encuentra el código fuente del servidor. */
#define PARSED_FILE		  "covid.csv"	    /* Valor utilizado para indicar el nombre del archivo donde se encuentran los datos filtrados. */


const int  SIZE              = 5;
const char CHARACTERS[SIZE]  = {'á', 'é', 'í', 'ó', 'ú'};      /* Valores utilizados para almacenar el caracter que se elimina de una región específica. */


void serverReader(std::string, int);
void serverParser(Reader *, Parser *, int);
void findCanton(std::ifstream&, std::string);
void covidData(int, std::ifstream&);
std::string translator(std::string);


/**
 * Punto de entrada del programa al Parser, crea tantos hilos como solicitudes a servidores hayan,
 * los elimina cuando estos hayan terminado su trabajo.
 * @param argc cantidad de parámetros enviados por línea de comandos.
 * @param argv array con strings que almacenan lo indicado por los parámetros.
 * @return 0.
 */

int main(int argc, char ** argv) {

  int answer;
  std::vector<std::string> server_address;
  std::vector<int> workers;
  std::ifstream file(PARSED_FILE);
    
  std::thread servers[NUMBER_SERVERS];
  server_address.push_back(RAW_FILE_NAME);
  workers.push_back(WORKERS);

  for(int i = ZERO; i < NUMBER_SERVERS; ++i) {

    servers[i] = std::thread(serverReader, server_address[i], workers[i]);

  }

  for(int i = ZERO; i < NUMBER_SERVERS; ++i) {

    servers[i].join();

  }
	
	std::cout << "\n1. Cantones de Costa Rica\n";
	std::cout << "2. Cantón específico\n\n";
	std::cout << "Ingrese una opción: ";
	std::cin >> answer;
	
	if(CR == answer){
		covidData(CR, file);
	}
	else if(CANTON == answer) {
		covidData(CANTON, file);
	}
	
  file.close();

  return 0;

}


/**
 * Convierte el nombre de una región en el formato apropiado para compararlo con el nombre de otra región.
 * @param region nombre de la región que se va a convertir.
 * @return std::string con el nombre de la región ya converido.
 */

std::string translator(std::string region){
  
  region.erase(std::remove(region.begin(), region.end(), ERASE4), region.end());
  std::for_each(region.begin(), region.end(), [](char & c) { c = ::tolower(c); });
  
  for(int i = ZERO; i < SIZE; ++i) {

    while(region.find(CHARACTERS[i]) != std::string::npos) {

      std::string::size_type pos = region.find(CHARACTERS[i], ZERO);
      region.replace(pos,std::string(CHARACTERS[i]).length(), CHARACTERS[i]);

    }

  }

  return region;
  
}


/**
 * Le solicita al usuario el nombre de un cantón y busca los datos de COVID-19 de ese cantón en el archivo con los datos filtrados.
 * @param file objeto que guarda el archivo que contiene los datos filtrados.
 * @return un std::string con los datos de COVID-19 del cantón solicitado.
 */

std::string findCanton(std::ifstream &file) {

  bool found = false;
  std::string line, data, answer;
  char block[NAME_MAX_SIZE];
  std::cout << "Ingrese el nombre del cantón: ";
  std::cin.getline(block,NAME_MAX_SIZE);
  std::cin.getline(block,NAME_MAX_SIZE);
  block[NAME_MAX_SIZE] = '\0';
  answer = std::string(block);
  answer = translator(answer);
  while(getline(file, line) && !found) {

    int i = ZERO;
    std::stringstream stream(line);

    while(getline(stream, data, DELIMITER) && !found) {

      if(!i && !answer.compare(translator(data))) {
        answer = line;
        found = true;
      }

      ++i;

    }

  }

  if(!found) {
    std::cout << "El cantón no fue encontrado.\n";
    exit(EXIT);
  }

  return answer;

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

	file.open(PARSED_FILE);

  for(it = parser->regions.begin(); it != parser->regions.end(); ++it) {

    int i = ZERO;
    std::string data;
    std::stringstream stream(*it);

    while(getline(stream, data, DELIMITER)) {

      if(!i) {
        data.erase(std::remove(data.begin(), data.end(), ERASE1), data.end());
        data.erase(std::remove(data.begin(), data.end(), ERASE2), data.end());
        file << data << DELIMITER;
      }
      else {
        data.erase(std::remove(data.begin(), data.end(), ERASE3), data.end());
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
 * @param file objeto que guarda el archivo que contiene los datos filtrados.
 */

void covidData(int region, std::ifstream &file) {

  TextTable t( '-', '|', '+' );
  t.add( "Lugar" );
  t.add( "Activos" );
  t.add( "Recuperados" );
  t.add( "Fallecidos" );
  t.add( "Acumulados" );
  t.endOfRow();
		
  if(region == CR) {
		
		std::string line, data;
		
		while( getline(file, line) ) {

			std::stringstream stream(line);
			while( getline(stream, data, DELIMITER) ) {
				t.add( data );
			}
			
			t.endOfRow();

		}

		file.close();
		
  }
  else if(region == CANTON) {
    std::string data = findCanton(file);
	
	// Dividir la linea con los datos separados por comas
	std::stringstream stream(data);
	while( getline(stream, data, DELIMITER) ) {
		t.add( data );
	}
	t.endOfRow();
	
  }
  
  t.setAlignment( 2, TextTable::Alignment::RIGHT );
  std::cout << t;

}