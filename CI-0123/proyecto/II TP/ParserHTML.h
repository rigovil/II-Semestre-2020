#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <atomic>
#include <iostream>
#include <map>
#include <regex>

#define REG_EXPRESSION "(<[^!][^ >]*)"

class ParserHTML {

    public:
        ParserHTML();
        ~ParserHTML();
        void leerLinea(std::string);
        std::map<std::string, std::atomic_int> etiquetas_HTML;
};
