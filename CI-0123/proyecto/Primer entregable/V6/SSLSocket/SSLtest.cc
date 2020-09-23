#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <sys/stat.h>

#include "Socket.h"

bool consulta(){
	struct stat filestat;
    struct tm *hoy;
    time_t now, f;
    char dat[20];
    bool need = false;
    
    stat("covid.txt",&filestat);
    
    strftime(dat, 20, "%d-%m-%R", localtime(&filestat.st_mtime));
    /*
          D D - M M - h h : m m
        [ 0 1 2 3 4 5 6 7 8 9 10 ]

    */
    printf(" File modify time %s", ctime(&filestat.st_mtime));


    //dat = ctime(&filestat.st_mtime);
    now = time(0);
    hoy = localtime(&now);

    printf("Hoy Dia: %d Mes: %d Hora: %d\n", hoy->tm_mday, (hoy->tm_mon + 1), hoy->tm_hour);
    printf("Archivo Dia: %d Mes: %d Hora: %d Min: %d\n", ((dat[0]-'0')*10 +dat[1]-'0'), ((dat[3]-'0')*10 +dat[4]-'0'), ((dat[6]-'0')*10 +dat[7]-'0'), (dat[9]-'0')*10 +dat[10]-'0');
    //printf("Archivo Dia: %d\n", ((dat[0]-'0')*10 +dat[1]-'0'));

    if(hoy->tm_mday != (dat[0]-'0')*10 +dat[1]-'0' && (dat[6]-'0')*10 +dat[7]-'0' > 17){
      printf("Actualiza archivo\n");
      return true;
    }else{
      printf("No Actualiza\n");
      return false;
    }
}

void getArchivo(){
	FILE *file;
   int st;
   //en caso de existir, se reehace el archivo
   if(file = fopen("covid.txt", "r")){
      st = remove("covid.txt");
      if(st != 0){
         perror("Error borrando archivo");
         exit(1);
      }else{
         printf("Archivo borrado con exito\n");
      }
      fclose(file);
   }
   printf("Archivo No Existe\n");
   file =fopen("covid.txt", "w");
   if(file == NULL){
      printf("No se pudo crear el arachivo\n");
   }
   
   /*
   *  Proceso Socket
   */
   
   char * os = (char *) "www.infogram.com";
   char * request = (char *) "GET https://infogram.com/9cf384f0-cdc2-488e-8ea6-cca496a38a90 HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   Socket s( 's', false );	// Create a new stream socket for IPv4
   char a[16000]; //SSLRead solo soporta 16kB a la vez

   memset( a, 0, 16000 );
   s.InitSSL();
   s.SSLConnect( os, (char*)"https" );
   s.SSLWrite(  (void *) request, strlen( request ) );
   for(int i = 0 ; i < 26; i ++){ //26 es mas que suficiente para llegar a la info necesaria
      s.SSLRead( a, 16000 );
      fputs(a, file);
      memset( a, 0, 16000 );
   }
   fclose(file);
}

//ec2-3-229-45-136.compute-1.amazonaws.com 

int main( int argc, char * argv[] ) {
   
  if( consulta() ){
  	getArchivo();
  }else{
  	printf("No hay datos nuevos aun\n");
  }
   
  return 0;
}