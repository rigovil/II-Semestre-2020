

/*
 *  Establece la definicion de la clase Socket para efectuar la comunicacion
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de la clase se hara en dos etapas, primero
 *  los metodos necesarios para los clientes, en la otra etapa los otros
 *  metodos para los servidores
 */

#ifndef Socket_h
#define Socket_h

class Socket{

    public:
        Socket( bool, bool = false );
        Socket( int );
        Socket( );   //constructor generico para IPv4 Stream
        ~Socket();
        int Connect( const char *, int );
        int Connect( const char *, const char * );
        void Close();
        int Read( char *, int );
        int Write( char *, int );
        int Write( char * );
        int Listen( int );
        int Bind( int );
        Socket * Accept();
        int Shutdown( int );
        void SetIDSocket( int );
        void InitSSL();
        void SSLConnect( char *, int );
        void SSLConnect( char *, char * );
        int SSLRead( void *, int );
        int SSLWrite( const void *, int );
    private:
        
        
        int idSocket;
        bool ipv6;
        void * SSLContext;      // SSL context
        void * SSLStruct;     // SSL BIO
        void InitSSLContext();


};

#endif


