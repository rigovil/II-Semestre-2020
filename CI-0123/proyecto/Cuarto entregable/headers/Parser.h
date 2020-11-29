#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <fstream>


#define ZERO            0                                                           /*  Para las estructuras de control if y for. */
#define MATCH           0                                                           /*  Utilizado para hacer el match con expresiones regulares. */
#define OFFSET          12                                                          /*  Utilizado para parsear la fecha de última actualización. */
#define CANTON          "0"                                                         /*  Utilizado para identificar que la solicitud es de un cantón. */
#define COUNTRY         "1"                                                         /*  Utilizado para identificar que la solicitud es de un país. */
#define TABLE           "T"                                                         /*  Utilizado para identificar que la solicitud se hace desde terminal. */
#define NO_TABLE        "H"                                                         /*  Utilizado para identificar que la solicitud no se hace desde terminal. */
#define REG_EXP         "\\[\"([a-zA-Z0-9_ñ ])+\",[0-9]+,[0-9]+,[0-9]+,[0-9]+\\]"   /*  Expresión regular para encontrar información en infogram.com. */
#define REG_EXP_T       "\\&t"                                                      /*  Expresión regular para indicar que el cliente es una terminal. */
#define REG_EXP_R       "(\\?canton=).*?(?=\\x20|\\&)"                              /*  Expresión regular indicando que lo solicitado es un cantón. */
#define REG_EXP_C       "(\\?country=).*?(?=\\x20|\\&)"                             /*  Expresión regular indicando que lo solicitado es un país. */
#define REG_EXP_HTML    "<TD(.*)<\\/TD>"                                            /*  Expresión regular para parsear la respuesta HTTP al solicitar datos de un país. */
#define PDELIMITER      "updatedAt"                                                 /*  Expresión regular para encontrar la fecha última de actualización. */
#define PDELIMITER2     '"'                                                         /*  Caracter de un delimitador. */
#define HTTP_400        "400 Bad Request"                                           /*  Indica un código de respuesta en el protocolo HTTP. */
#define HTTP_404        "404 Not Found"                                             /*  Indica un código de respuesta en el protocolo HTTP. */
#define NEW_LINE        "\n"                                                        /*  Indica un salto de línea. */
#define CHAR1           '"'                                                         /*  Almacena un caracter que se elimina de los datos parseados. */
#define CHAR2           '['                                                         /*  Almacena un caracter que se elimina de los datos parseados. */
#define CHAR3           ']'                                                         /*  Almacena un caracter que se elimina de los datos parseados. */
#define DELIMITER       ','                                                         /*  Delimita los datos de un cantón/región/país. */


class Parser {

    public:

        Parser();
        ~Parser();
        void readLine(std::string);
        void cantonCSV(std::string);
        std::string countryCSV(std::string, std::string, bool);
        std::string countryJSON(std::string file_name, bool exists);
        std::string getDate();
        std::string readCountry(std::string);
        std::vector<std::string> readRegion(std::string);
        std::vector<std::string> regions;       /* Array que almacena el string que posee la información de un cantón. */

    private:

        std::string date;

};