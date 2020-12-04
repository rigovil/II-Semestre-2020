#include <stdlib.h>
#include <string>
#include <regex>
#include <iostream>

//#define REG "{[a-zA-Z0-9\":,\\-\\.]*}(?=\\])"
//#define REG "\\{"[a-zA-Z0-9\": ,.-]+\\}\\]"
//#define REG "\{"[a-zA-Z0-9": ,.-]+\}\]"

//#define REG "{}]"
#define REG "\\{[a-zA-Z0-9\":,\\s\\-\\.]*\\}(?=\\])"

int main(int argc, char ** argv) {
    std::string match;
    std::smatch matches;
    std::regex reg_exp(REG);
    //std::string prueba = argv[1];
    //std::string prueba = "{\"a-aa.a,Z.Z32::.:,,,\n\t s---}]";
    
    //std::string prueba = "{\"Country\":\"Costa Rica\",\"CountryCode\":\"CR\",\"Province\":\"\",\"City\":\"\",\"CityCode\":\"\",\"Lat\":\"9.75\",\"Lon\":\"-83.75\",\"Confirmed\":127012,\"Deaths\":1588,\"Recovered\":78238,\"Active\":47186,\"Date\":\"2020-11-18T00:00:00Z\"},{\"Country\":\"Costa Rica\",\"CountryCode\":\"CR\",\"Province\":\"\",\"City\":\"\",\"CityCode\":\"\",\"Lat\":\"9.75\",\"Lon\":\"-83.75\",\"Confirmed\":128231,\"Deaths\":1599,\"Recovered\":78851,\"Active\":47781,\"Date\":\"2020-11-19T00:00:00Z\"}]";
    
    std::string prueba = "{\"Country\":\"Costa Rica\",\"CountryCode\":\"CR\",\"Province\":\"\",\"City\":\"\",\"CityCode\":\"\",\"Lat\":\"9.75\",\"Lon\":\"-83.75\",\"Confirmed\":127012,\"Deaths\":1588,\"Recovered\":78238,\"Active\":47186,\"Date\":\"2020-11-18T00:00:00Z\"},{\"Country\":\"Costa Rica\",\"CountryCode\":\"CR\",\"Province\":\"\",\"City\":\"\",\"CityCode\":\"\",\"Lat\":\"9.75\",\"Lon\":\"-83.75\",\"Confirmed\":128231,\"Deaths\":1599,\"Recovered\":78851,\"Active\":47781,\"Date\":\"2020-11-19T00:00:00Z\"}]";
    
    std::regex_search(prueba, matches, reg_exp);
    match = matches.str(0);
    std::cout << match << "\n";

    int n = 5;
    std::vector< std::string > JSON_countries( n , "" );
    JSON_countries[0] = "Dominica";
    JSON_countries[1] = "Saint Vincent and the Grenadines";
    JSON_countries[2] = "Saint Barthélemy";
    JSON_countries[3] = "Puerto Rico";
    JSON_countries[4] = "Turks and Caicos Islands";
    //JSON_countries[5] = "United States Virgin Islands"; // en el server del profe

    std::vector< std::string > JSON_countries_names( n , "" );
    JSON_countries_names[0] = "Dominica";
    JSON_countries_names[1] = "saint-vincent-and-the-grenadines";
    //JSON_countries_names[2] = "Saint Barthélemy";
    //JSON_countries_names[3] = "Puerto Rico";
    //JSON_countries_names[4] = "Turks and Caicos Islands";

    int l = JSON_countries.size(); // vector's length

    for(int i=0; i<l; ++i ){
        std::cout << JSON_countries[i] << std::endl;
    }


}