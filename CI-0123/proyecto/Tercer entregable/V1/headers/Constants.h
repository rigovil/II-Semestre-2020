/* CONSTANTES GENERALES */

#define ERROR           -1              /*  Identifica un error en alguna función. */
#define EXIT            0               /*  Parámetro para salir del programa en caso de error. */
#define PSHARED         0               /*  Indica que los semáforos se comparten entre los hilos de un proceso. */
#define SEM_OFF         0               /*  Indica que un semáforo se inicializa con el valor de 0. */
#define L1              0               /*  Indica dónde se empieza a cortar un string. */
#define ONE             1               /*  Utilizado para operaciones aritméticas. */
#define SEM_ON          1               /*  Indica que un semáforo se inicializa con el valor de 1. */
#define TWO             2               /*  Para ciertas estructuras de control. */
#define WORKERS         4               /*  Indica la cantidad de trabajadores que van a analizar el código fuente de un servidor. */
#define ARRAY_SIZE      4               /*  Indica el tamaño de los array que almacenan datos de "Otros" y totales. */
#define L2              4               /*  Indica dónde termina a cortar un string. */
#define SIZE            8               /*  Para identificar el tamaño del array con los caracteres que se eliminan. */
#define S_SIZE          8               /*  Indica el tamaño de las lecturas del Socket de servidor. */
#define POW             10              /*  Indica el valor de la potencia. */
#define DATE            20              /*  Indica el tamaño del array donde se almacena la fecha. */
#define BYTES_READ      512             /*  Indica la longitud máxima en bytes de una consulta HTTP. */
#define BYTES_READ2     1024            /*  Indica la longitud máxima en bytes de una consulta HTTP. */
#define FILE_BYTES      281272          /*  Indica cuántos bytes se deben leer del código fuente obtenido del servidor. */
#define CHAR4           ' '             /*  Almacena un caracter que se elimina de una región específica. */
#define INFO1           "Otro"          /*  Indica los datos que no han sido asignados a una región. */
#define INFO2           "Total"         /*  Indica el total de datos. */
#define CR              "costarica"     /*  Identifica a Costa Rica. */
#define PARSED_FILE		"covid.csv"	    /*  Indica el nombre del archivo donde se encuentran los datos de los cantones. */
#define COUNTRY_FILE    "country.csv"   /*  Indica el nombre del archivo donde se encuentran los datos de los países. */
#define RAW_FILE_NAME   "covid_raw.txt" /*  Indica el nombre del archivo donde se encuentra el código fuente del servidor. */

/* CONSTANTES PARA EL PROTOCOLO HTTP */

#define CRLF            "\r\n"                            /*  Indica el fin de una línea según el protocolo HTTP. */
#define SP              " "                               /*  Indica un espacio según el protocolo HTTP. */
#define HTTP_VERSION    "HTTP/1.1"                        /*  Indica la versión del protocolo HTTP. */
#define HTTP_200        "200 OK"                          /*  Indica un código de respuesta en el protocolo HTTP. */
#define HTTP_400        "400 Bad Request"                 /*  Indica un código de respuesta en el protocolo HTTP. */
#define HTTP_404        "404 Not Found"                   /*  Indica un código de respuesta en el protocolo HTTP. */
#define HTTP_501        "501 Not Implemented"             /*  Indica un código de respuesta en el protocolo HTTP. */
#define HTTP_505        "501 HTTP Version not supported"  /*  Indica un código de respuesta en el protocolo HTTP. */

/* OTRAS CONSTANTES */

const std::string SPEC_CHARACTERS[]   = {"á", "é", "í", "ó", "ú", "%", "2", "0"};   /*  Caracteres que se eliminan de un string específico. */
const std::string CHARACTERS[]        = {"a", "e", "i", "o", "u", "", "", ""};      /*  Caracteres que reemplazan a los caracteres eliminados. */