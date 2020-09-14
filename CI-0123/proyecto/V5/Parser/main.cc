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
#include <sys/stat.h>
#include "Reader.h"
#include "Parser.h"
#include "Socket.h"
#include "TextTable.h"


#define ERROR           -1              /* Valor utilizado para identificar un error en alguna función. */
#define EXIT            0               /* Valor utilizado como parámetro para salir del programa en caso de error. */
#define PSHARED         0               /* Valor utilizado para indicar que los semáforos se comparten entre los hilos de un proceso. */
#define ZERO            0               /* Valor utilizado para las estructuras de control if y for. */
#define SEM_OFF         0               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 0. */
#define SEM_ON          1               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 1. */
#define CR              1               /* Valor utilizado para identificar a Costa Rica. */
#define NUMBER_SERVERS  1               /* Valor utilizado para indicar el número de servidores a los que se les va a solicitar información. */
#define TWO             2               /* Valor utilizado para ciertas estructuras de control. */
#define CANTON          2               /* Valor utilizado para identificar que se quiere buscar un cantón específico. */
#define ALIGNMENT       2               /* Valor utilizado para identificar el alineamiento de la tabla que muestra los datos. */
#define EXIT_MENU       3               /* Valor utilizado para identificar cuando el usuario quiere salir del programa. */
#define WORKERS         4               /* Valor utilizado para indicar la cantidad de trabajadores que van a analizar el código fuente de un servidor. */
#define SIZE            5               /* Valor utilizado para identificar el tamaño del array con los caracteres que se eliminan. */
#define S_SIZE          8               /* Valor utilizado para indicar el tamaño de las lecturas del Socket de servidor. */
#define POW             10              /* Valor utilizado para indicar el valor de la potencia. */
#define DATE            20              /* Valor utilizado para indicar el tamaño del array donde se almacena la fecha. */
#define NAME_MAX_SIZE 	25				      /* Valor utilizado para indicar el length máximo del nombre de un lugar como entrada */
#define FILE_BYTES      281272          /* Valor utilizado para indicar cuántos bytes se deben leer del código fuente obtenido del servidor. */
#define DELIMITER       ','             /* Valor utilizado para delimitar los datos de un cantón/región/país. */
#define NEW_LINE        '\n'            /* Valor utilizado para almacenar un caracter de salto de línea. */
#define CHAR1           '"'             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define CHAR2           '['             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define CHAR3           ']'             /* Valor utilizado para almacenar un caracter que se elimina de los datos parseados. */
#define CHAR4           ' '             /* Valor utilizado para almacenar un caracter que se elimina de una región específica. */
#define MENU            "menu.txt"      /* Valor utilizado para indicar el nombre del archivo donde se encuentra el menú. */
#define PARSED_FILE		  "covid.csv"	    /* Valor utilizado para indicar el nombre del archivo donde se encuentran los datos filtrados. */
#define RAW_FILE_NAME   "covid_raw.txt" /* Valor utilizado para indicar el nombre del archivo donde se encuentra el código fuente del servidor. */


const std::string SPEC_CHARACTERS[]   = {"á", "é", "í", "ó", "ú"};      /* Valores utilizados para el caracter que se elimina de una región específica. */
const std::string CHARACTERS[]        = {"a", "e", "i", "o", "u"};      /* Valores utilizados para el caracter que reemplaza a los caracteres con tilde. */


bool query();
void getFile();
void serverReader(std::string, int);
void serverParser(Reader *, Parser *, int);
void findRegion(std::ifstream&, std::string);
void covidData(int, std::ifstream&);
std::string translator(std::string);


/**
 * Punto de entrada del programa al Parser, crea tantos hilos como solicitudes a servidores hayan,
 * los elimina cuando estos hayan terminado su trabajo. Despliega el menú que interacciona con el usuario.
 * @param argc cantidad de parámetros enviados por línea de comandos.
 * @param argv array con strings que almacenan lo indicado por los parámetros.
 * @return 0.
 */

int main(int argc, char ** argv) {

  int answer = ONE;
  std::vector<std::string> server_address;
  std::vector<int> workers;
  std::ifstream file(PARSED_FILE);
  std::ifstream file_menu(MENU);
  std::stringstream menu;

  file_menu.open(MENU);
  menu << file_menu.rdbuf();

  if(query()) {
    getFile();

    std::thread servers[NUMBER_SERVERS];
    server_address.push_back(RAW_FILE_NAME);
    workers.push_back(WORKERS);

    for(int i = ZERO; i < NUMBER_SERVERS; ++i) {

      servers[i] = std::thread(serverReader, server_address[i], workers[i]);

    }

    for(int i = ZERO; i < NUMBER_SERVERS; ++i) {

      servers[i].join();

    }
  }
  else {
    std::cout << "\nYa se poseen los datos más recientes.\n";
  }

  while(answer != EXIT_MENU) {
	
    std::cout << menu.str();
    std::cin >> answer;
    
    if(CR == answer) {
      covidData(CR, file);
    }
    else if(CANTON == answer) {
      covidData(CANTON, file);
    }

  }
	
  file.close();
  file_menu.close();

  return ZERO;

}


/**
 * Convierte el nombre de una región en el formato apropiado para compararlo con el nombre de otra región.
 * @param region nombre de la región que se va a convertir.
 * @return std::string con el nombre de la región ya converido.
 */

std::string translator(std::string region) {

  region.erase(std::remove(region.begin(), region.end(), CHAR4), region.end());
  std::for_each(region.begin(), region.end(), [](char & c) { c = ::tolower(c); });

  for(int i = ZERO; i < SIZE; ++i) {

    while(region.find(SPEC_CHARACTERS[i]) != std::string::npos) {

      std::string::size_type pos = region.find(SPEC_CHARACTERS[i], ZERO);
      region.replace(pos, std::string(SPEC_CHARACTERS[i]).length(), CHARACTERS[i]);

    }
  
  }

  return region;
  
}


/**
 * Le solicita al usuario el nombre de un cantón y busca los datos de COVID-19 de ese cantón en el archivo con los datos filtrados.
 * @param file objeto que guarda el archivo que contiene los datos filtrados.
 * @return un std::string con los datos de COVID-19 del cantón solicitado.
 */

std::string findRegion(std::ifstream &file) {

  bool found = false, first = true;
  char block[NAME_MAX_SIZE];
  std::string line, data, answer;
  
  std::cout << "Ingrese el nombre del cantón:\n";

  std::cin.getline(block, NAME_MAX_SIZE);
  std::cin.getline(block, NAME_MAX_SIZE);

  answer = std::string(block);
  answer = translator(answer);

  while(getline(file, line) && !found) {

    if(!first) {
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
    else {
      first = false;
    }

  }

  if(!found) {
    std::cout << "\nEl cantón no fue encontrado.\n";
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
  std::string date;
  std::thread threads[workers];
  std::vector<std::string>::iterator it;
  std::ofstream file_w;
  std::ifstream file_r;

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

	file_r.open(PARSED_FILE);
  std::getline(file_r, date);
  file_r.close();

  if(!date.compare(parser->getDate())) {
    std::cout << "\nEl servidor todavía no se ha actualizado, se mostrarán los datos más recientes.\n";
  }
  else {
    file_w.open(PARSED_FILE);
    file_w << parser->getDate() << NEW_LINE;

    for(it = parser->regions.begin(); it != parser->regions.end(); ++it) {

      int i = ZERO;
      std::string data;
      std::stringstream stream(*it);

      while(getline(stream, data, DELIMITER)) {

        if(!i) {
          data.erase(std::remove(data.begin(), data.end(), CHAR1), data.end());
          data.erase(std::remove(data.begin(), data.end(), CHAR2), data.end());
          file_w << data << DELIMITER;
        }
        else {
          data.erase(std::remove(data.begin(), data.end(), CHAR3), data.end());
          file_w << data << DELIMITER;
        }

        ++i;

      }

      file_w << NEW_LINE;

    }

    file_w.close();

  }

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

  TextTable table( '-', '|', '+' );
  table.add( "Lugar" );
  table.add( "Activos" );
  table.add( "Recuperados" );
  table.add( "Fallecidos" );
  table.add( "Acumulados" );
  table.endOfRow();
		
  if(region == CR) {
    bool first = true;
		std::string line, data;
		
		while(getline(file, line)) {

      if(!first) {
        std::stringstream stream(line);

        while(getline(stream, data, DELIMITER)) {

          table.add(data);

        }
        
        table.endOfRow();
      }
      else {
        first = false;
      }

		}
  }

  else if(region == CANTON) {
    std::string data = findRegion(file);
    std::stringstream stream(data);

    while(getline(stream, data, DELIMITER)) {

      table.add(data);

    }

    table.endOfRow();
	
  }
  
  std::cout << NEW_LINE;
  table.setAlignment(ALIGNMENT, TextTable::Alignment::RIGHT);
  std::cout << table;

}


/**
 * Solicita al servidor la informacion de los datos regionales del COVID-19.
 */

void getFile() {

  Socket s('s', false);
  FILE *file, *raw;
  char fchar, buf[S_SIZE]; 
  int ret, head;
  bool html       = false;
  char * os       = (char *) "www.infogram.com";
  char * request  = (char *) "GET https://infogram.com/9cf384f0-cdc2-488e-8ea6-cca496a38a90 HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
  char * header   = (char *) "HEAD https://infogram.com/9cf384f0-cdc2-488e-8ea6-cca496a38a90 HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

  memset(buf, ZERO, S_SIZE);

  if(file = fopen(RAW_FILE_NAME, "r")) {
    ret = remove(RAW_FILE_NAME);

    if(ZERO != ret) {
      perror("getFile:fopen:'r'");
      exit(EXIT);
    }

    fclose(file);
  }

  file = fopen(RAW_FILE_NAME, "w");

  if(NULL == file){
    printf("getFile:fopen:'w'");
    exit(EXIT);
  }

  s.InitSSL();
  s.SSLConnect(os, (char*) "https");
  s.SSLWrite((void *) request, strlen(request));
    
  for(int i = FILE_BYTES; i > ZERO; i -= S_SIZE) { 

    s.SSLRead(buf, S_SIZE);
    fputs(buf, file);
    memset(buf, ZERO, S_SIZE);

  }

  fclose(file);
  file = fopen(RAW_FILE_NAME, "r");
  head = ZERO;

  while(fchar != CHAR4) {

    fchar = fgetc(file);

  }

  fchar = fgetc(file);

  for(int i = TWO; i >= ZERO; i--) {

    head += (fchar - '0') * pow(POW, i);
    fchar = fgetc(file);

  }

  switch(head) {

    case 200:
      html = true;
      printf("Conexión establecida y datos recuperados.\n");
      break;

    case 400:
      printf("Recurso no encontrado.\n");
      exit(EXIT);
      break;

    case 404:
      printf("Recurso no encontrado.\n");
      exit(EXIT);
      break;

    case 501:
      printf("No implementado.\n");
      exit(EXIT);
      break;

    case 505:
      printf("Version HTTP no soportada.\n");
      exit(EXIT);
      break;

  }

}


/**
 * Consulta el archivo del código fuente para verificar si es posible que el servidor haya actualizado los datos cantonales del COVID-19.
 * @return un booleano que indica si se debe conectar con el servidor.
 */

bool query() {

  int status;
  bool need;
  char date[DATE];
  time_t now, f;
  struct tm *today;
  struct stat filestat;

  status = stat(PARSED_FILE, &filestat);

  if(ERROR == status) {
    need = true;
  }
  else {
    strftime(date, 20, "%d-%m-%R", localtime(&filestat.st_mtime));

    now = time(0);
    today = localtime(&now);

    if(today->tm_mday != (date[0]-'0')*10 +date[1]-'0' && (date[6]-'0')*10 +date[7]-'0' > 17) {
      need = true;
    }
    else {
      need = false;
    }
  }
  
  return need;

}