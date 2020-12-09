#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <semaphore.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <bits/stdc++.h> 
#include "headers/Reader.h"
#include "headers/Parser.h"
#include "headers/Socket.h"
#include "headers/Response.h"
#include "headers/Constants.h"



bool fileExists(std::string);
bool needleInHaystack(std::string, std::string, bool);
bool queryCanton();
void getCantones();
void queryCountry();
void cantonParsed(std::string, int);
void cantonLineParsed(Reader *, Parser *, int);
void * receive(void *);
void * listen80(void *);
void * listen51(void *);
void * startUpBroadcast(void *);
std::string responseHTTP(char *);
std::string translator(std::string);
std::string covidData(std::string, bool, bool);
std::string findRegion(std::string, bool);
std::string getCountry(std::string country);



/**
 * Arranca el servidor para que pueda escuchar peticiones del cliente.
 * @param argc cantidad de parámetros enviados por línea de comandos.
 * @param argv array con strings que almacenan lo indicado por los parámetros.
 * @return 0.
 */

int main(int argc, char ** argv) {

  pthread_t * threads = (pthread_t *) calloc(3, sizeof(pthread_t));
  pthread_create(&threads[0], NULL, startUpBroadcast, NULL);
  pthread_create(&threads[1], NULL, listen51, NULL);
  pthread_create(&threads[2], NULL, listen80, NULL);

  for(int i = 0; i < 3; ++i) {
    pthread_join(threads[i], NULL);
  }

  return ZERO;
  
}



/****************************************************************/
/* MÉTODOS PARA CONEXIONES CON SERVIDORES DE DATOS E INTERMEDIOS*/
/****************************************************************/



/**
 * Envía un broadcast a todos los servidores intermedios para indicar
 * que el servidor esta activo.
 * @param arg argumento para pthreads.
 * @return NULL.
 */

void * startUpBroadcast(void * arg) {

  int sockfd, broadcast = 1;
  socklen_t n, len;
  char buffer[MAXLINE];
  const char * hello = "0,172.16.123.35,caribbean";
  struct sockaddr_in servaddr;

  // std::cout << "Broadcast enviado..." << std::endl;

  if((sockfd = socket( AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

  setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
  memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT50);

	for(int i = 0; i < 9; ++i) {
		servaddr.sin_addr.s_addr = inet_addr((const char *) BroadCastAddr[i]);
		sendto(sockfd, hello, strlen(hello), MSG_WAITALL, (const struct sockaddr *) &servaddr, sizeof(servaddr));
  }
	
  for(;;) {
		n = recvfrom(sockfd, (char *) buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
	}

	close(sockfd);

}



/**
 * Escucha en el puerto 51000 por servidores intermedios.
 * @param arg argumento para pthreads.
 */

void * listen51(void * arg) {

  int sockfd;
  socklen_t n, len;
  char buffer[MAXLINE];
  const char * hello = "0,172.16.123.35,caribbean";
  struct sockaddr_in servaddr, cliaddr;

  // std::cout << "Servidor escuchando en puerto 51000..." << std::endl;

  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

  memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

  servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT51);

  if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("bind failed");
		exit( EXIT_FAILURE );
	}

  len = sizeof(cliaddr);

	for(;;) {
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
		buffer[n] = '\0';
		printf("Servidor intermedio activo: %s\n", buffer);
		sendto(sockfd, hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
	}

}



/**
 * Inicializa el servidor en el puerto 80 donde va a escuchar por las solicitudes HTTP del cliente.
 * Utiliza hilos implementados por pthreads para manejar cada consulta recibida.
 */

void * listen80(void * arg) {

  pthread_t * threads = (pthread_t *) calloc(1, sizeof(pthread_t));
  Socket s1, * s2;
  s1.Bind(PORT80);
  s1.Listen(QUEUE);
  // std::cout << "Servidor escuchando en puerto 80..." << std::endl;

  while(true) {
    s2 = s1.Accept();
    pthread_create(&threads[0], NULL, receive, (void *) s2);
    pthread_join(threads[0], NULL);
    s2->Close();
    delete s2;
  }

  free(threads);

}



/**
 * Función que ejecutan los hilos pthreads del servidor cuando se recibe una consulta por parte de un cliente.
 * @param arg puntero void con la referencia al socket que atiende la consulta del cliente.
 * @return NULL.
 */

void * receive(void * arg) {

  char * request;
  char * response;
  Socket * s2;
  std::string region, http_response;

  s2      = (Socket *) arg;
  request = (char *) malloc(BYTES_READ);
  
  s2->Read(request, BYTES_READ);
  http_response = responseHTTP(request);
  response      = (char *) malloc(http_response.length()+1);
  strcpy(response, http_response.c_str());

  s2->Write(response);
  
  free(response);
  free(request);
  return NULL;
  
}



/****************************************************************/
/* MÉTODOS PARA SOLICITAR DATOS DEL COVID-19 A OTROS SERVIDORES */
/****************************************************************/



/**
 * Realiza una conexión con el servidor que provee los datos de COVID-19 de un país específico.
 * Llama al método encargado de crear o modificar el .csv que maneja los datos de los países.
 * @param country país solicitado por el cliente.
 */

std::string getCountry(std::string country) {

  Socket s;
  Parser parser;
	char buf[BYTES_READ2];
	char * host         = (char *) "os.ecci.ucr.ac.cr";
  country             = std::regex_replace(country, std::regex("-"), "%20");
  std::string request = ("GET /covid/corona.cgi?country=" + country + " HTTP/1.1\r\nhost: redes.ecci\r\n\r\n");

	s.Connect(host, (char *) "http");
	s.Write((char *) request.c_str());
	s.Read(buf, BYTES_READ2);

  return parser.countryCSV(COUNTRY_FILE, buf, fileExists(COUNTRY_FILE));

}



/**
 * Realiza una conexión con el servidor Infogram para obtener la información de los datos cantonales del COVID-19.
 * Llama al método encargado de parsear todo el código fuente enviado por el servidor.
 */

void getCantones() {

  Socket s('s', false);
  FILE *file;
  char fchar, buf[S_SIZE+1]; 
  int ret, head;
  bool html       = false;
  char * host     = (char *) "www.infogram.com";
  char * request  = (char *) "GET https://infogram.com/9cf384f0-cdc2-488e-8ea6-cca496a38a90 HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

  memset(buf, ZERO, S_SIZE);

  file = fopen(RAW_FILE_NAME, "w");

  if(NULL == file){
    printf("getFile:fopen:'w'");
    exit(EXIT);
  }

  s.InitSSL();
  s.SSLConnect(host, (char *) "https");
  s.SSLWrite((void *) request, strlen(request));
    
  for(int i = FILE_BYTES; i > ZERO; i -= S_SIZE) { 
    s.SSLRead(buf, S_SIZE);
    buf[8] = '\0';
    fputs(buf, file);
    memset(buf, ZERO, S_SIZE+1);
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
      printf("Conexión establecida y datos cantonales recuperados.\n");
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

  cantonParsed(RAW_FILE_NAME, WORKERS);

}



/**
 * Lee el código fuente de la dirección del servidor Infogram y crea los hilos para que distintos trabajadores
 * encuentren la información relacionada con los datos cantonales del COVID-19.
 * También escribe un archivo a disco con los datos del COVID-19 de cada cantón.
 * @param file_name nombre del archivo que contiene el código fuente enviado por el servidor..
 * @param workers cantidad de trabajadores que van a analizar el código fuente.
 */

void cantonParsed(std::string file_name, int workers) {

  bool need = true;
  int i, result;
  std::string date;
  std::thread threads[workers];
  std::ofstream file_w;
  std::ifstream file_r;

  sem_t *semaphores   = new sem_t[workers];
  Parser *parser      = new Parser();
  Reader *reader      = new Reader(file_name, workers, semaphores);

  for(i = ZERO; i < workers; ++i) {
    if(i == ZERO) {
      result = sem_init(&semaphores[i], PSHARED, SEM_ON);
    }
    else {
      result = sem_init(&semaphores[i], PSHARED, SEM_OFF);
    }

    if(ERROR == result) {
        perror("sem_init");
        exit(EXIT);
    }

    threads[i] = std::thread(cantonLineParsed, reader, parser, i);
  }

  for(i = ZERO; i < workers; ++i) {
    threads[i].join();
    result = sem_destroy(&semaphores[i]);

    if(ERROR == result) {
      perror("sem_destroy");
      exit(EXIT);
    }
  }

  file_r.open(PARSED_FILE);

  if(file_r.is_open()) {
    std::getline(file_r, date);
    file_r.close();
    if(!date.compare(parser->getDate())) { need = false; }
  }

  if(need)  { parser->cantonCSV(PARSED_FILE); }
  else      { printf("El servidor todavía no se ha actualizado, se mostrarán los datos más recientes.\n"); }
  
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

void cantonLineParsed(Reader * reader, Parser * parser, int worker) {

  std::string line;

  while(reader->hasNext(worker)) {
    line = reader->getNext(worker);
    parser->readLine(line);
  }

}



/*******************************************************************************************/
/* MÉTODOS PARA CREAR UNA RESPUESTA CON LOS DATOS DE COVID-19 A PARTIR DEL PROTOCOLO HTTP  */
/*******************************************************************************************/



/**
 * Crea el header del protocolo HTTP para la respuesta que se le envía al cliente.
 * Formato de una respuesta según el protocolo: 
 * 
 *      HTTP-Version SP Status-Code SP Reason-Phrase CRLF     (status line)
 *      Response Header Fields CRLF                           (información adicional del servidor)
 *      CRLF
 *      Message Body                                          (cuerpo del mensaje en HTML)
 *       
 * @param buf puntero con la solicitud HTTP del cliente.
 * @return respuesta al cliente según el protocolo HTTP.
 */

std::string responseHTTP(char * buf) {

  bool bash;
  int requestType;
  Parser parser;
  Response response;
  std::string location, region, messageBody, statusCode, httpResponse, httpRequest = buf;
  std::vector<std::string> requestInfo;

  requestInfo = parser.readRegion(httpRequest);
  region      = requestInfo[2];
  location    = response.Location(requestInfo[1]);
  bash        = response.BashOrBrowser(requestInfo[0]);
  requestType = response.CountryOrCanton(requestInfo[1]);

  switch(requestType) {
    case 0:
      if(queryCanton()) { getCantones(); }
      messageBody = covidData(translator(region), false, bash);
      break;

    case 1:
      queryCountry();
      messageBody = covidData(region, true, bash);
      break;

    default:
      messageBody = HTTP_400;
      break;
  }

  statusCode    = response.StatusCode(messageBody);
  httpResponse  = response.BuildRequest(statusCode, location, region, messageBody);

  return httpResponse;

}



/**
 * Envía el message body que contiene los datos del COVID-19 de la región solicitada.
 * @param region region solicitada por el usuario.
 * @param isCountry indica si lo que se solicita es un país o no.
 * @param bash indica si el cliente es una terminal para enviarle datos que no estén en HTML.
 * @return std::string con el código HTML que se envía al usuario.
 */

std::string covidData(std::string region, bool isCountry, bool bash) {

  Response response;
  std::ifstream file;
  std::string data;
  std::vector<std::string> caribbean = {"anguilla","antigua-and-barbuda","aruba","bahamas","barbados","british-virgin-islands","cayman-islands","cuba","dominica","dominican-republic","grenada","guadeloupe","haiti","jamaica","martinique","montserrat","saint-kitts-and-nevis","saint-lucia","saint-martin","sint-maarten","trinidad-and-tobago","turks-and-caicos"};
  file.open(PARSED_FILE);
  data = findRegion(region, isCountry);
  file.close();

  // if(isCountry && !data.compare(std::to_string(ERROR))) {
  if(isCountry && !data.compare(std::to_string(ERROR)) && find(caribbean.begin(), caribbean.end(), translator(region)) != caribbean.end()) {
    data = getCountry(region);
  }

  if(bash) {
    return response.ResponseToBash(isCountry, data);
  }
  else {
    return response.ResponseToBrowser(isCountry, data);
  }

}



/************************************************/
/* MÉTODOS AUXILIARES PARA AGILIZAR EL PROGRAMA */
/************************************************/



/**
 * Método que encuentra si una palabra está contenida dentro de otra.
 * @param haystack posible palabra con la que se va a hacer match.
 * @param needle palabra que se busca si está contenida dentro del haystack.
 * @param country indica si lo que se busca es un país o cantón.
 * @return true si needle está contenida dentro de haystack y si es un cantón.
 */

bool needleInHaystack(std::string haystack, std::string needle, bool country){
  
  return !country && (haystack.find(needle) != std::string::npos) && !needle.empty();

}



/** Indica si un archivo existe en el directorio local.
 * @param file_name nombre de archivo.
 * @return true si el archivo existe.
 */

bool fileExists(std::string file_name) {

  int status;
  bool exists = true;
  struct stat filestat;
  
  status = stat(file_name.c_str(), &filestat);

  if(ERROR == status) {
    exists = false;
  }

  return exists;

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
 * Busca en los archivos .csv el cantón o país que el usuario quiere consultar.
 * @param answer cantón o país solicitado por el usuario.
 * @param country indica si lo que pide el usuario es un país o no.
 * @return datos de COVID-19 del cantón o país solicitado.
 */

std::string findRegion(std::string answer, bool country) {

  bool found, match = false, first = true;
  std::ifstream file;
  std::string line, data, covid_data, region;

  if(country) {
    file.open(COUNTRY_FILE);
  }
  else {
    file.open(PARSED_FILE);
  }

  answer = translator(answer);

  while(getline(file, line)) {
    found = false;
    
    if(!first) {
      int i = ZERO;
      std::stringstream stream(line);

      while(getline(stream, data, DELIMITER) && !found) {
        region = translator(data);

        if(!i && (!answer.compare(region) || needleInHaystack(region, answer, country))) {
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
    covid_data = std::to_string(ERROR);
  }
  
  file.close();
  return covid_data;
  
}



/**
 * Consulta el archivo del código fuente para verificar si es posible que el servidor haya actualizado los datos cantonales del COVID-19.
 * @return un booleano que indica si se debe conectar con el servidor.
 */

bool queryCanton() {

  bool need;
  int status;
  char date[DATE];
  char newName[DATE];
  std::string name = "covid";
  time_t now, f;
  struct tm *today;
  struct stat filestat;
  std::ifstream file;

  status = stat(PARSED_FILE, &filestat);
  // file.open(PARSED_FILE);

  if(ERROR == status) {
    // printf("crea csv\n");  // funciona
    need = true;
  }
  else {
    strftime(date, 20, "%d-%m-%R", localtime(&filestat.st_mtime));

    now = time(0);
    today = localtime(&now);

    if(today->tm_mday != (date[0]-'0')*10 +date[1]-'0' && (date[6]-'0')*10 +date[7]-'0' > 17) {
      need = true;
      strftime(date, 20, "-%d-%m.csv", localtime(&filestat.st_mtime));
      name += date;
      strcpy(newName,name.c_str());
      rename(PARSED_FILE,newName);
    }
    else { need = false; }
  }
  
  return need;

}



/**
 * Verifica si el archivo con la información de los países consultados previamente podría
 * necesitar de una actualización o si los datos aún pueden ser considerados como válidos.
 * Se toma como validos despues del medio dia de cada dia diferente.
 * @return un booleano que indica si se debe conectar con el servidor para actualizar los datos.
 */

void queryCountry() {

  int status;
  bool need, exists = true;
  char date[DATE];
  char newName[DATE];
  std::string name = "country";
  time_t now, f;
  struct tm *today;
  struct stat filestat;

  status = stat(COUNTRY_FILE, &filestat);

  if(ERROR == status) {
    need = true;
    exists = false;
  }
  else {
    strftime(date, 20, "%d-%m-%R", localtime(&filestat.st_mtime));

    now = time(0);
    today = localtime(&now);

    if(today->tm_mday != (date[0]-'0')*10 +date[1]-'0' && (date[6]-'0')*10 +date[7]-'0' > 12) {
      need = true;
    }
    else {
      need = false;
    }
  }
  
  if(need && exists) {
  	printf("Datos de paises antiguos, eliminando para actualizacion...\n");
  	strftime(date, 20, "-%d-%m.csv", localtime(&filestat.st_mtime));
    name += date;
    strcpy(newName,name.c_str());
    rename(COUNTRY_FILE,newName);
  }

}