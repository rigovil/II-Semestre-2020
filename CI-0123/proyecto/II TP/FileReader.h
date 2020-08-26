#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <mutex>
#include <shared_mutex>
#include <semaphore.h>
#include <vector>
/**
 * Read a text file by lines accordingly a number of clients
 *
 * Author: CI0117 Programacion paralela y concurrente
 * Date: 2020/May/28
 *
 *
 * The file can be read using four strategies:
 *   a) The file is divided in almost equal parts (number of lines) the first part is assingned to first worker, second part to second worker, and so on
 *   b) The file is assigned one line to each worker, first worker takes all lines which line number modulus number of clients is zero, the second which modulus is one, and so on
 *   c) The file is read by demand, one line for each worker who needs it
 *   d) A personal developed strategy 
 *
 * Project restrictions
 *   - This class will hold in memory only one line for each worker (max. 1024 bytes)
 *   - The file can be opened just one time for each instance of this class
 *
 */

class FileReader {

   private:
      sem_t *semaforos;
      std::mutex *mutex;
      std::ifstream input;
      int trabajadores;
      int estrategia;
      int *siguienteLinea;
      int total_lineas = 0;
      
      int lineasLeidas = 0;    // para mapeo dinámico

   public:
      FileReader(std::string, int, int, std::mutex *, sem_t *);
      ~FileReader();
      bool hasNext(int);
      std::string getNext(int);
};