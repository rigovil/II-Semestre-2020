/*
 *  Establece la definicion de la clase Socket para efectuar la comunicacion
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de la clase se hara en dos etapas, primero
 *  los metodos necesarios para los clientes, en la otra etapa los otros
 *  metodos para los servidores
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#ifndef Socket_h
#define Socket_h

class Socket{

    public:
        Socket( bool, bool = false );
        Socket( int );
        ~Socket();
        int Connect( const char *, int );
        int Read( char *, int );
        int Write( char *, int );
        int Write( char * );
        int Listen( int );
        int Bind( int );
        int Shutdown( int );
        int recvFrom( void *, int, void * );
        int sendTo( const void *, int, void * );
        void SetIDSocket( int );
        void Close();
        Socket * Accept();
        
    private:
        int idSocket;
        bool is_ipv6;
};

#endif