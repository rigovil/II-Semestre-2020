#ifndef Socket_h
#define Socket_h


#define ERROR   -1      /* Valor utilizado para identificar un error en algún método. */
#define ERROR2  0       /* Valor utilizado para identificar un error. */
#define EXIT    0       /* Valor utilizado para identificar el parámetro de salida. */
#define STREAM  's'     /* Valor utilizado para identificar un socket de tipo stream. */


class Socket {

    public:
	Socket( );
        Socket(char, bool = false);
        Socket(int);
        ~Socket();
        int Connect(const char *, int);
        int Connect(const char *, const char *);
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
        void SSLConnect(char *, int);
        void SSLConnect(char *, char *);
        int SSLRead(void *, int);
        int SSLWrite(const void *, int);

    private:
        
        int idSocket;
        bool ipv6;
        void * SSLContext;
        void * SSLStruct;
        void InitSSLContext();

};

#endif