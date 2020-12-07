#include <stdlib.h>
#include <fstream>
#include <semaphore.h>
#include <vector>
#include <thread>
#include <math.h>
#include <stdio.h>


#define ZERO   0     /* Valor utilizado en las estructuras de control for o para indicar la primera posición de un array. */
#define ONE    1     /* Valor utilizado para operaciones aritméticas. */
#define ERROR  -1    /* Valor utilizado para identificar un error en alguna función. */
#define EXIT   0     /* Valor utilizado como parámetro para salir del programa en caso de error. */


class Reader {

   public:

      Reader(std::string, int, sem_t *);
      ~Reader();
      bool hasNext(int);
      std::string getNext(int);

   private:

      int workers;                  /* Cantidad de trabajadores que analizan un código fuente. */
      int *next_line;               /* Array que le indica a un trabajador específico la siguiente línea que debe leer. */
      int lines_read;               /* Total de líneas leídas en un momento dado. */
      int total_lines = 0;          /* Total de líneas del código fuente, inicializado en cero para evitar errores. */
      sem_t *semaphores;            /* Array de semáforos donde un semáforo le corresponde a un trabajador. */
      std::ifstream source_code;    /* Archivo donde se encuentra el código fuente. */

};