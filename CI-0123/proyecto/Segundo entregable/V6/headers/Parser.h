#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>


#define MATCH           0                                                           /*  Utilizado para hacer el match con expresiones regulares. */
#define OFFSET          12                                                          /*  Utilizado para parsear la fecha de última actualización. */
#define REG_EXP         "\\[\"([a-zA-Z0-9_ñ ])+\",[0-9]+,[0-9]+,[0-9]+,[0-9]+\\]"   /*  Expresión regular para encontrar información en infogram.com. */
#define REG_EXP_R       "(region=).*(?=\x20|&)"                                     /*  Expresión regular para parsear la región que quiera el usuario. */
#define PDELIMITER      "updatedAt"                                                 /*  Expresión regular para encontrar la fecha última de actualización. */
#define PDELIMITER2     '"'                                                         /*  Caracter de un delimitador. */
#define HTTP_400        "400 Bad Request"                                           /*  Indica un código de respuesta en el protocolo HTTP. */


class Parser {

    public:

        Parser();
        ~Parser();
        void readLine(std::string);
        std::string readRegion(std::string);
        std::string getDate();
        std::vector<std::string> regions;       /* Array que almacena el string que posee la información de un cantón/país/región. */

    private:

        std::string date;

};