#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <sys/time.h>
#include "../headers/TextTable.h"

#define REGEX   ".*\\r\\n"  /* Expresión regular para parsear solamente el contenido de la respuesta. */

class Response {
    public:
        Response();
        ~Response();
        bool BashOrBrowser(std::string);
        int CountryOrCanton(std::string);
        std::string Location(std::string);
        std::string StatusCode(std::string);
        std::string ResponseToBash(bool, std::string);
        std::string ResponseToBrowser(bool, std::string);
        std::string BuildRequest(std::string, std::string, std::string, std::string);
};