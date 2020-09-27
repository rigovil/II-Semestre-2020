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
#include "headers/Reader.h"
#include "headers/Parser.h"
#include "headers/Socket.h"
#include "headers/TextTable.h"


#define ERROR           -1              /* Valor utilizado para identificar un error en alguna función. */
#define EXIT            0               /* Valor utilizado como parámetro para salir del programa en caso de error. */
#define PSHARED         0               /* Valor utilizado para indicar que los semáforos se comparten entre los hilos de un proceso. */
#define ZERO            0               /* Valor utilizado para las estructuras de control if y for. */
#define SEM_OFF         0               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 0. */
#define L1              0               /* Valor utilizado para indicar dónde se empieza a cortar un string. */
#define SEM_ON          1               /* Valor utilizado para indicar que un semáforo se inicializa con el valor de 1. */
#define NUMBER_SERVERS  1               /* Valor utilizado para indicar el número de servidores a los que se les va a solicitar información. */
#define TWO             2               /* Valor utilizado para ciertas estructuras de control. */
#define CANTON          2               /* Valor utilizado para identificar que se quiere buscar un cantón específico. */
#define ALIGNMENT       2               /* Valor utilizado para identificar el alineamiento de la tabla que muestra los datos. */
#define EXIT_MENU       3               /* Valor utilizado para identificar cuando el usuario quiere salir del programa. */
#define WORKERS         4               /* Valor utilizado para indicar la cantidad de trabajadores que van a analizar el código fuente de un servidor. */
#define ARRAY_SIZE      4               /* Valor utilizado para indicar el tamaño de los array que almacenan datos de "Otros" y totales. */
#define L2              4               /* Valor utilizado para indicar dónde termina a cortar un string. */
#define SIZE            8               /* Valor utilizado para identificar el tamaño del array con los caracteres que se eliminan. */
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
#define INFO1           "Otro"          /* Valor utilizado para indicar los datos que no han sido asignados a una región. */
#define INFO2           "Total"         /* Valor utilizado para indicar el total de datos. */
#define MENU            "menu.txt"      /* Valor utilizado para indicar el nombre del archivo donde se encuentra el menú. */
#define CR              "costarica"     /* Valor uitilzado para identificar a Costa Rica */
#define PARSED_FILE		  "covid.csv"	    /* Valor utilizado para indicar el nombre del archivo donde se encuentran los datos filtrados. */
#define HTML            "regions.html"  /* Valor utilizado para indicar el nombre del archivo donde se encuentra el código HTML. */
#define RAW_FILE_NAME   "covid_raw.txt" /* Valor utilizado para indicar el nombre del archivo donde se encuentra el código fuente del servidor. */


const std::string SPEC_CHARACTERS[]   = {"á", "é", "í", "ó", "ú", "%", "2", "0"};      /* Valores utilizados para el caracter que se elimina de una región específica. */
const std::string CHARACTERS[]        = {"a", "e", "i", "o", "u", "", "", ""};      /* Valores utilizados para el caracter que reemplaza a los caracteres con tilde. */


bool query();
void getFile();
void startServer();
void serverReader(std::string, int);
void serverParser(Reader *, Parser *, int);
void * receive(void* arr);
std::string covidData(std::string);
std::string translator(std::string);
std::string findRegion(std::ifstream&, std::string);
bool needleInHaystack(std::string, std::string);


/**
 * Punto de entrada del programa al Parser, crea tantos hilos como solicitudes a servidores hayan,
 * los elimina cuando estos hayan terminado su trabajo. Recibe la solicitud del usuario para mostrar los datos.
 * @param argc cantidad de parámetros enviados por línea de comandos.
 * @param argv array con strings que almacenan lo indicado por los parámetros.
 * @return 0.
 */

int main(int argc, char ** argv) {
  
  int answer;
  std::vector<std::string> server_address;
  std::vector<int> workers;
  
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
  
  startServer();
  
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

std::string findRegion(std::ifstream &file, std::string answer) {

  bool found, match = false, first = true;
  std::string line, data, covid_data, region;
  
  while(getline(file, line)) {

    found = false;
    
    if(!first) {
      int i = ZERO;
      std::stringstream stream(line);
      
      while(getline(stream, data, DELIMITER) && !found) {
        
        region = translator(data);

        if(!i && (!answer.compare(region) || needleInHaystack(region, answer))) {
          match       = true;
          found       = true;
          covid_data += line + NEW_LINE;
        }
        
        ++i;
        
      }
    }
    else {
      first = false;
    }
    
  }

  if((!match) || !answer.compare(translator(INFO1))) {
    covid_data = ERROR;
  }
  
  file.seekg(ZERO);
  
  return covid_data;
  
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
  remove(RAW_FILE_NAME);
  
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
 * Envía el código HTML que contiene los datos del COVID-19 de la región solicitada.
 * @param region region solicitada por el usuario.
 * @return std::string con el código HTML que se envía al usuario.
 */

std::string covidData(std::string region) {

  std::string html_s;
  std::ifstream file(PARSED_FILE);

  html_s.append("<HTML>\n<TITLE>COVID-19 Statistics</TITLE>\n");
  html_s.append("<TABLE BORDER=1>\n");
  html_s.append("<TR>\n");
  html_s.append("<TH> Region </TH>\n");
  html_s.append("<TH>Activos</TH>");
  html_s.append("<TH>Recuperados</TH>");
  html_s.append("<TH>Fallecidos</TH>");
  html_s.append("<TH>Acumulados</TH>\n");
  html_s.append("</TR>\n\n");
		
	if(region == CR) {
		bool first = true, first_o = true;
		std::string line, data;
		std::vector<int> otro(ARRAY_SIZE);
		std::vector<int> total(ARRAY_SIZE);
		
		while(getline(file, line)) {

			if(!first) {
        int i = ZERO;
				std::stringstream stream(line);
        std::string region = line.substr(L1, L2);
				
				if(INFO1 != region) {
          html_s.append("<tr>\n");

					while(getline(stream, data, DELIMITER)) {
						
						html_s.append("<TD align=\"center\">");
            html_s.append(data);
            html_s.append("</TD>\n");

						if(i) {
							total[i-1] += std::stoi(data);
						}

						++i;

					}

          html_s.append("</tr>\n\n");
				}
        else {
					if(first_o) {
						while(getline(stream, data, DELIMITER)) {

							if(i) {
                total[i-1] += std::stoi(data);
								otro[i-1] += std::stoi(data);
							}

							++i;

						}

						first_o = false;
					}
				}
			}
			else {
				first = false;
			}

		}

    /**
     * Bloque HTML para la categoría "Otros"
     */

    html_s.append("<tr>\n");
    html_s.append("<TD align=\"center\">");
    html_s.append(INFO1);
    html_s.append("</TD>\n");

    for(int i = ZERO; i < ARRAY_SIZE; ++i) {
      
      html_s.append("<TD align=\"center\">");
      html_s.append(std::to_string(otro[i]));
      html_s.append("</TD>\n");

    }

    html_s.append("</tr>\n\n");

    /**
     * Bloque HTML para la categoría "Total" 
     */
    
    html_s.append("<tr>\n");
    html_s.append("<TD align=\"center\">");
    html_s.append(INFO2);
    html_s.append("</TD>\n");

    for(int i = ZERO; i < ARRAY_SIZE; ++i) {

      html_s.append("<TD align=\"center\">");
      html_s.append(std::to_string(total[i]));
      html_s.append("</TD>\n");

    }

    html_s.append("</tr>\n\n");
  }
  else {
    std::string data = findRegion(file, region), region_data;
    std::stringstream stream(data);

    if(data != std::to_string(ERROR)) {
      while(getline(stream, region_data)) {

        std::stringstream stream_region(region_data);
        html_s.append("<tr>\n");
        
        while(getline(stream_region, data, DELIMITER)) {

          html_s.append("<TD align=\"center\">");
          html_s.append(data);
          html_s.append("</TD>\n");

        }

        html_s.append("</tr>\n\n");

      }
    }
  }

  html_s.append("</TABLE>\n");
  html_s.append("</HTML>");

  file.close();

  return html_s;

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

  fclose(file);
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


/**
 * Método que encuentra si una palabra está contenida dentro de otra
 */

bool needleInHaystack(std::string haystack, std::string needle){
  
  return haystack.find(needle) != std::string::npos;

}


void startServer() {

  pthread_t * threads = (pthread_t *) calloc(1, sizeof(pthread_t));
 
  Socket s1, *s2;
  s1.Bind(9702);
  s1.Listen(5);   

  while(true) {
    
    s2 = s1.Accept();
    pthread_create(&threads[0], NULL, receive, (void *) s2);
    pthread_join(threads[0], NULL);
    s2->Close();

  }

  free(threads);

}


void * receive(void * arg) {

  char buf[512];
  char * buffer_sent;
  Socket *s2;
  Parser parser;
  std::string region, html;

  s2 = (Socket *) arg;

  s2->Read(buf, 512);
  region  = buf;
  region  = translator(parser.readRegion(region));
  html    = covidData(region);

  buffer_sent = new char[html.length()+1];
  strcpy(buffer_sent, html.c_str());
  s2->Write(buffer_sent); 

  free(buffer_sent);

  return NULL;
  
}