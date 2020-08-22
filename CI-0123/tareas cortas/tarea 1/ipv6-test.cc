/*
 *  Esta prueba solo funciona utilizando un equipo de la red interna de la ECCI, por lo que
 *  deberan conectarse por la VPN para realizarla
 *  La direccion IPv6 provista es una direccion privada
 *  Tambien deben prestar atencion al componente que esta luego del "%" en la direccion y que hace
 *  referencia a la interfaz de red utilizada para la conectividad
 *
 */
#include <stdio.h>
#include <string.h>
#include "Socket.h"

int main( int argc, char * argv[] ) {

   Socket s( true, true );
   char a[512];

   memset( a, 0, 512 );
   s.Connect( (char *) "fe80::ffb4:ea86:1aa4:19c%eno1", (char *) "http" );
   s.Write(  (char *) "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n", 36 );
   s.Read( a, 512 );
   printf( "%s\n", a);

}
