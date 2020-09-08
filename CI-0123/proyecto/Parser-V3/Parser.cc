#include "Parser.h"


/**
 * Constructor de la clase Parser.
 */

Parser :: Parser() {
}


/**
 * Destructor de la clase Parser.
 */

Parser :: ~Parser() {
}


/**
 * Utiliza expresiones regulares para buscar coincidencias en una línea en específico. 
 * Las coincidencias son la clasifiación de los datos por regiones que se buscan en el código fuente de un servidor,
 * para obtener los datos de COVID-19 de esa región.
 * @param line una línea del código fuente obtenida del servidor.
 */

void Parser :: readLine(std::string line) {

    std::string region;
    std::smatch matches;
    std::regex reg_exp (REG_EXP);

    while(std::regex_search(line, matches, reg_exp)) {

        region = matches.str(ZERO);
        regions.push_back(region);
        line = matches.suffix().str();

    }

}