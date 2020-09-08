#include <stdlib.h>
#include <string>
#include <vector>
#include <regex>


#define ZERO     0                                                           /* Valor utilizado para hacer el match con expresiones regulares. */
#define REG_EXP  "\\[\"([a-zA-Z0-9_ñ ])+\",[0-9]+,[0-9]+,[0-9]+,[0-9]+\\]"   /* Expresión regular para encontrar información en infogram.com. */


class Parser {

    public:

        Parser();
        ~Parser();
        void readLine(std::string);
        std::vector<std::string> regions;       /* Array que almacena el string que posee la información de un cantón/país/región. */

};
