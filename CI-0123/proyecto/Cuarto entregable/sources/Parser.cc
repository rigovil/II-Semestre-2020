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
 * Las coincidencias son la clasificación de los datos cantonales que se buscan en el código fuente del servidor.
 * @param line una línea del código fuente obtenida del servidor que provee los datos cantonales.
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
 * Utiliza expresiones regulares para determinar si el cliente solicita un cantón o un país. 
 * @param user_region código HTTP que recibe el servidor cuando un usuario solicita un cantón o país.
 * @return {bash/browser, country/canton, nombre del canton o pais}
 */

std::vector<std::string> Parser :: readRegion(std::string user_region) {
    
    bool match_CLI, match_canton, match_country, match_region;
    std::string canton, country;
    std::vector<std::string> region_info;
    std::smatch matches_CLI, matches_canton, matches_region, matches_country;
    std::regex reg_exp_CLI (REG_EXP_T);
    std::regex reg_exp_canton (REG_EXP_R);
    std::regex reg_exp_region (REG_EXP_RG);

    match_CLI       = std::regex_search(user_region, matches_CLI, reg_exp_CLI);
    match_canton    = std::regex_search(user_region, matches_canton, reg_exp_canton);
    match_region    = std::regex_search(user_region, matches_region, reg_exp_region);

    if(match_canton) {
        canton = matches_canton.str(MATCH);
        canton.erase(canton.begin(), canton.begin()+7);
        
        if(match_CLI) {
            region_info.push_back(CLI);
        }
        else {
            region_info.push_back(NO_CLI);
        }

        region_info.push_back(CANTON);
        region_info.push_back(canton);
    }
    else if(match_region) {
        country = matches_region.str(MATCH);
        country.erase(country.begin(), country.begin()+22);
        
        if(match_CLI) {
            region_info.push_back(CLI);
        }
        else {
            region_info.push_back(NO_CLI);
        }

        region_info.push_back(COUNTRY);
        region_info.push_back(country);
    }
    else {
        region_info.push_back(HTTP_400);
        region_info.push_back("-1");
        region_info.push_back(" ");
    }
    
    //std::cout << region_info << "\n";
    return region_info;

}


/**
 * Utiliza expresiones regulares para separar las estadísticas del país que el usuario quiere solicitar del resto del protocolo HTTP. 
 * @param user_country código HTTP que recibe el servidor cuando un usuario solicita un país.
 * @return datos del país parseados o un código de error.
 */

std::string Parser :: readCountry(std::string user_country) {

    bool match;
    std::string country, flag_404;
    std::smatch matches;
    std::regex reg_exp (REG_EXP_HTML);
    std::regex reg_exp_sp ("\\<.*[a-z]\\>(?=\\x20[0])");
   
    match = std::regex_search(user_country, matches, reg_exp);

    if(match) {
        country = matches.str(MATCH);
        country.erase(std::remove(country.begin(), country.end(), ','), country.end());
        country = std::regex_replace(country, std::regex("<TD>  </TD>"), "<TD> ? </TD>");
        country = std::regex_replace(country, std::regex("<TD> N/A </TD>"), "<TD> ? </TD>");
        
        if(std::regex_search(country, matches, reg_exp_sp)) {
            country = HTTP_404;
        }
        else {
            country = std::regex_replace(country, std::regex("\\<.*>(?=\\x20[A-Z])"), "");
            country = std::regex_replace(country, std::regex("\\x20+\\<.+?(?=[0-9]|\\+|\\?)"), ",");
            country = std::regex_replace(country, std::regex("  </TD>"), "");

            if(country.length()) { country.erase(0,1); }
        
            flag_404 = country.substr(0,3);

            if(!flag_404.compare("TD>")){
                country = HTTP_404;
            }
        }
    }
    else {
        country = HTTP_404;
    }
    
    return country;
    
}


/**
 * Crea el archivo .csv que contiene los datos cantonales.
 * @param file_name nombre del archivo .csv.
 */

void Parser :: cantonCSV(std::string file_name) {
    
    std::ofstream file;
    std::vector<std::string>::iterator it;

    // file_name = "./" + file_name;

    file.open(file_name);
    file << this->getDate() << NEW_LINE;
    
    for(it = this->regions.begin(); it != this->regions.end(); ++it) {
      int i = ZERO;
      std::string data;
      std::stringstream stream(*it);

      while(getline(stream, data, DELIMITER)) {

        if(!i) {
          data.erase(std::remove(data.begin(), data.end(), CHAR1), data.end());
          data.erase(std::remove(data.begin(), data.end(), CHAR2), data.end());
          file << data << DELIMITER;
        }
        else {
          data.erase(std::remove(data.begin(), data.end(), CHAR3), data.end());
          file << data << DELIMITER;
        }

        ++i;

      }
      
      file << NEW_LINE;
      
    }

    file.close();

}




/**
 * Crea o modifica el archivo .csv que contiene los datos de los países.
 * @param file_name nombre del archivo .csv.
 * @param response respuesta HTTP del servidor que ofrece los datos de un país.
 * @param exists si el archivo existe o no.
 * @return los datos del país parseados.
 */

std::string Parser :: countryCSV(std::string file_name, std::string response, bool exists) {

    std::string data;
    std::ofstream file;

    if(exists) {
        file.open(file_name, std::ios_base::app);
    }
    else {
        file.open(file_name);
        file << "Total Cases,New Cases,Total Deaths,New Deaths,Total Recovered,New Recovered,Active Cases,Serious Critical,Total Cases/1M,Deaths/1M,Total tests,Tests/1M,Population" << NEW_LINE;
    }

    data = this->readCountry(response);
   
    if((data.compare(HTTP_400)) && data.compare(HTTP_404)) {
      file << data << NEW_LINE;
    }

    file.close();

    return data;

}


/**
 * Devuelve la fecha de última actualización del código fuente del servidor.
 * @return fecha última de actualización del servidor.
 */

std::string Parser :: getDate() {

    return this->date;

}