#include "../headers/Response.h"

/**
 *  Constructor de la clase Response.
 */

Response::Response() {
}


/**
 *  Destructor de la clase Response.
 */

Response::~Response() {
}


/**
 *  Verifica si el client que envía una solicitud es un browser o bash.
 *  @param info resultado del parser de leer la solicitud en HTML.
 *  @return true si es bash, false si es browser.
 */

bool Response::BashOrBrowser(std::string info) {
    return !info.compare("T");
}


/**
 *  Construye la localización como parte del protocolo HTTP.
 *  @param info resultado del parser de leer la solicitud en HTML.
 *  @return location del protocolo HTTP.
 */

std::string Response::Location(std::string info) {

    std::string location;
    if(!info.compare("0")) {
        location = "Location: 127.0.0.1:8080/?canton=";
    }
    else if(!info.compare("1")) {
        location = "Location: 127.0.0.1:8080/?country=";
    }
    else {
        location = "Location: 127.0.0.1:8080/";
    }

    return location;

}


/**
 *  Verifica el tipo de request del cliente.
 *  @param info resultado del parser de leer la solicitud en HTML.
 *  @return entero que indica el tipo de request.
 */

int Response::CountryOrCanton(std::string info) {

    int requestType;
    if(!info.compare("0")) {
        requestType = 0;
    }
    else if(!info.compare("1")) {
        requestType = 1;
    }
    else {
        requestType = -1;
    }

    return requestType;

}


/**
 *  Construye el código de estado para el protocolo HTTP.
 *  @param messageBody el cuerpo HTML del mensaje.
 *  @return código de estado.
 */

std::string Response::StatusCode(std::string messageBody) {

    std::string statusCode;
    if(!messageBody.compare("400 Bad Request")) {
        statusCode = "400 Bad Request";
    }
    else if(!messageBody.compare("404 Not Found")) {
        statusCode = "404 Not Found";
    }
    else {
        statusCode = "200 OK";
    }

    return statusCode;

}


/**
 *  Crea el mensaje en protocolo HTTP para la respuesta que se le envía al cliente.
 *  Formato de una respuesta según el protocolo: 
 * 
 *      HTTP-Version SP Status-Code SP Reason-Phrase CRLF     (status line)
 *      Response Header Fields CRLF                           (información adicional del servidor)
 *      CRLF
 *      Message Body                                          (cuerpo del mensaje en HTML)
 *       
 *  @param statusCode código de estado.
 *  @param location location del request.
 *  @param region region solicitada.
 *  @param messageBody cuerpo del mensaje.
 *  @return respuesta al cliente según el protocolo HTTP.
 */

std::string Response::BuildRequest(std::string statusCode, std::string location, std::string region, std::string messageBody) {
    
    time_t now;
    char date[80];
    struct tm * info;

    time(&now);
    info = gmtime(&now);
    strftime(date, 80, "%c GMT", info);

    std::string httpResponse;
    httpResponse.append("HTTP/1.1");        /*                  */
    httpResponse.append(" ");               /*   status line    */
    httpResponse.append(statusCode);        /*       HTTP       */
    httpResponse.append("\r\n");            /*                  */

    httpResponse.append(date);              /* versión del http y el código de estado */
    httpResponse.append("\r\n");            /*                 -----                  */
    httpResponse.append(location);
    httpResponse.append(region);            /*                HEADERS                 */
    httpResponse.append("\r\n");
    httpResponse.append("Content-Type: html");
    httpResponse.append("\r\n");
    httpResponse.append("\r\n");            /*                 -----                  */
    httpResponse.append(messageBody);       /*             contenido HTML             */

    return httpResponse;

}


/**
 *  Crea la respuesta que se le envía a un cliente tipo bash 
 *  en formato de una tabla.
 *  @param isCountry indica si la consulta es de un país o no.
 *  @param RegionData datos de la región solicitada.
 *  @return std::string con la respuesta.
 */

std::string Response::ResponseToBash(bool isCountry, std::string RegionData) {
    
    TextTable table( '-', '|', '+' );
    std::stringstream stringStreamTable;
    std::string response, line, data, stringTable;
    std::vector<std::string> rows = {"Country", "Total Cases", "New Cases", "Total Deaths", "New Deaths", "Total Recovered", "New Recovered", "Active Cases", "Serious Critical", "Total Cases/1M", "Deaths/1M", "Total tests", "Tests/1M", "Population"};

    /* Verifica primero si hay un error con la region */

    if(!RegionData.compare("404 Not Found") || !RegionData.compare("-1")) {
        response.append("404 Not Found");
    }
    else {
        std::stringstream stream(RegionData);

        if(isCountry) {
            while(getline(stream, line)) {
                int i = 0;
                std::stringstream DataStream(line);

                while(getline(DataStream, data, ',')) {
                    table.add(rows[i]);
                    table.add(data);
                    table.endOfRow();
                    ++i;
                }
            }
        }
        else {
            table.add( "Lugar" );
            table.add( "Activos" );
            table.add( "Recuperados" );
            table.add( "Fallecidos" );
            table.add( "Acumulados" );
            table.endOfRow();

            while(getline(stream, line)) {
                std::stringstream DataStream(line);

                while(getline(DataStream, data, ',')) {
                    table.add(data);
                }

                table.endOfRow();
            }
        }

        stringStreamTable << table;
        stringTable = stringStreamTable.str();
        response.append(stringTable);
    }

    return response;

}


/**
 *  Crea la respuesta que se le envía a un cliente tipo browser
 *  en formato HTML.
 *  @param isCountry indica si la consulta es de un país o no.
 *  @param RegionData datos de la región solicitada.
 *  @return std:string con la respuesta HTML.
 */

std::string Response::ResponseToBrowser(bool isCountry, std::string RegionData) {

    std::stringstream stream(RegionData);
    std::string response, data, cantonData;

    if(!RegionData.compare("400 Bad Request")) {
        response.append("400 Bad Request");
    }
    else {
        if(isCountry) {
            if(RegionData.compare("404 Not Found")) {
                response.append("<HTML><TITLE>COVID-19</TITLE>\n");
                response.append("<TABLE BORDER=1>\n");
                response.append("<TR><TH> Country </TH>\n");
                response.append("<TH> Total Cases </TH> <TH> New Cases </TH> <TH> Total Deaths </TH> <TH> New Deaths </TH> <TH> Total Recovered </TH> <TH> New Recovered </TH> <TH> Active Cases </TH> <TH> Serious Critical </TH> <TH> Total Cases/1M </TH> <TH> Deaths/1M </TH> <TH> Total tests </TH> <TH> Tests/1M </TH> <TH> Population </TH> </TR>");
                response.append("<tr>\n");

                while(getline(stream, data, ',')) {
                    response.append("<TD align=\"center\">");
                    response.append(data);
                    response.append("</TD>\n");
                }
                response.append("</tr>\n\n");
            }
            else {
                response.append("404 Not Found");
            }
        }
        else {
            if(RegionData.compare("-1")) {
                response.append("<HTML>\n<TITLE>COVID-19 Statistics</TITLE>\n");
                response.append("<TABLE BORDER=1>\n");
                response.append("<TR>\n");
                response.append("<TH> Region </TH>\n");
                response.append("<TH>Activos</TH>");
                response.append("<TH>Recuperados</TH>");
                response.append("<TH>Fallecidos</TH>");
                response.append("<TH>Acumulados</TH>\n");
                response.append("</TR>\n\n");

                while(getline(stream, cantonData)) {
                    std::stringstream cantonStream(cantonData);
                    response.append("<tr>\n");

                    while(getline(cantonStream, data, ',')) {
                        response.append("<TD align=\"center\">");
                        response.append(data);
                        response.append("</TD>\n");
                    }
                    response.append("</tr>\n\n");
                }
            }
            else {
                response.append("404 Not Found");
            }
        }
    }

    return response;

}