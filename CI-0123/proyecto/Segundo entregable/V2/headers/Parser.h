#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>


#define ZERO        0                                                           /* Valor utilizado para hacer el match con expresiones regulares. */
#define OFFSET      12                                                          /* Valor utilizado para parsear la fecha de última actualización. */
#define REG_EXP     "\\[\"([a-zA-Z0-9_ñ ])+\",[0-9]+,[0-9]+,[0-9]+,[0-9]+\\]"   /* Expresión regular para encontrar información en infogram.com. */
#define REG_EXP_R   "[a-zA-Z%20]+(?= HTTP)"                                     /* Expresión regular para parsear la región que quiera el usuario. */
#define PDELIMITER   "updatedAt"                                                /* Expresión regular para encontrar la fecha última de actualización. */
#define PDELIMITER2  '"'                                                        /* Caracter para almacenar un delimitador. */

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
