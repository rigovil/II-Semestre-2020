#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>


int main(int argc, char ** argv) {

    int i = 0;      // lo uso para saber que dato estoy sacando
    std::string prueba = "['San Jose', 12, 80, 71, 400, 521]";
    std::string dato;
    

    // aquí va el for que recorre todos los string parseados //
        std::stringstream stream(prueba);

        while(getline(stream, dato, ',')) {
            if(!i) {
                std::cout << "Nombre cantón: " << dato << "\n";

                // escribe en el archivo
            }
            else {
                std::cout << "Dato: " << std::stoi(dato) << "\n";

                // escribe en el archivo
            }
            ++i;
        }
        
        i = 0;

        // salta de línea en el archivo

    // aqui terminaría el for //

}