#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <atomic>
#include <iostream>
#include <map>
#include <regex>

// #define REG_EXPRESSION "\[\"(.*?)\"+(.*?)+\]"
// #define REG_EXPRESSION "\[([^\[\]]*)\]"
#define REG_EXPRESSION "\\[\"([a-zA-Z0-9_ñ ])+\",[0-9]+,[0-9]+,[0-9]+,[0-9]+\\]"
// #define REG_EXPRESSION "\"\""

class ParserHTML {

    public:
        ParserHTML();
        ~ParserHTML();
        void leerLinea(std::string);
        std::map<std::string, std::atomic_int> etiquetas_HTML;
};
