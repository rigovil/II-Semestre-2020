/*
   C++ class to encapsulate Unix message passing intrinsic structures and
   system calls

*/

#define KEY 0xA12345	// Valor de la llave del recurso

class Buzon {
   public:
      Buzon();
      ~Buzon();
      int Enviar( char *mensaje );
      int Enviar( char *mensaje, int cantidad, long tipo = 1 );
      int Recibir( void *mensaje, long tipo = 1 );	// len: space in mensaje
   private:
      int id;		// Identificador del buzon
};
