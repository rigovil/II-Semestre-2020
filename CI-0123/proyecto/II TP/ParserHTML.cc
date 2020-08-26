#include "ParserHTML.h"

ParserHTML :: ParserHTML() {
}

ParserHTML :: ~ParserHTML() {

}

void ParserHTML :: leerLinea(std::string linea) {
    std::smatch matches;
    std::regex reg_exp (REG_EXPRESSION);
    std::string etiq;

    while(std::regex_search(linea, matches, reg_exp)) {
        etiq = matches.str(1);
        etiq.erase(0,1);

        if(etiquetas_HTML[etiq] >= 0) {
            ++etiquetas_HTML[etiq];
        }

        linea = matches.suffix().str();  // resetea el match
    }
}