#include "../headers/Parser.h"


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

    bool found = false;
    std::string region;
    std::smatch matches;
    std::regex reg_exp (REG_EXP);

    while(std::regex_search(line, matches, reg_exp)) {
        
        found = true;
        region = matches.str(MATCH);
        regions.push_back(region);
        line = matches.suffix().str();

    }

    if(found) {
        size_t pos = line.find(PDELIMITER);
        line.erase(MATCH, pos + OFFSET);
        this->date = line.substr(MATCH, line.find(PDELIMITER2));
    }

}


/**
 * Utiliza expresiones regulares para separar la región que el usuario quiere solicitar del resto del protocolo HTTP. 
 * @param user_region código HTTP que recibe el servidor cuando un usuario solicita una región.
 * @return la región parseada o un código de error.
 */

std::string Parser :: readRegion(std::string user_region) {
    
    bool match;
    std::string region;
    std::smatch matches;
    std::regex reg_exp (REG_EXP_R);

    match = std::regex_search(user_region, matches, reg_exp);

    if(match) {
        region = matches.str(MATCH);
        region.erase(region.begin(), region.begin()+7);
    }
    else {
        region = HTTP_400;
    }
    
    return region;

}


/**
 * Devuelve la fecha de última actualización del código fuente del servidor.
 * @return fecha última de actualización del servidor.
 */

std::string Parser :: getDate() {

    return this->date;

}