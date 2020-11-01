// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "synch.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/*
 *  It increments 3 program counters (prev, now and next), 4-byte increment (32 bits)
 */
void IncrementPCRegs(){
   //int prevPC = machine->ReadRegister( PrevPCReg );
   int PC = machine->ReadRegister( PCReg );
   int nextPC = machine->ReadRegister( NextPCReg );

   machine->WriteRegister( PrevPCReg, PC );
   machine->WriteRegister( PCReg, nextPC );
   machine->WriteRegister( NextPCReg, nextPC+4 );
}


/*
 *  System call interface: Halt()
 */
void NachOS_Halt() {		// System call 0

	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();

}


/*
 *  System call interface: void Exit( int )
 */
void NachOS_Exit() {		// System call 1
}


/*
 *  System call interface: SpaceId Exec( char * )
 */
void NachOS_Exec() {		// System call 2
}


/*
 *  System call interface: int Join( SpaceId )
 */
void NachOS_Join() {		// System call 3
}


/*
 *  System call interface: void Create( char * )
 */
void NachOS_Create() {		// System call 4
  int regName =  machine->ReadRegister(4);
	char* name = NULL;
	int c;
	int i = 0;
	while(c != 0){
		machine->ReadMem(regName, 1, &c);
		name[i] = c;
		++i;
	}

	int fd = open(name, O_CREAT);
	if(fd < 0){
		printf("Error creating file\n");
	}

	//Manejo para dar un NachosHandle al archivo
  //currentThread->tabla->Open()
}


/*
 *  System call interface: OpenFileId Open( char * )
 */
void NachOS_Open() {		// System call 5
  int regName = machine->ReadRegister(4);
	char* name = NULL;
	int c;
	int i = 0;
	while(c != 0){
		machine->ReadMem(regName, 1, &c);
		name[i] = c;
		++i;
	}

	int fd = open(name, O_CREAT);
	if(fd < 0){
		printf("Error opening file\n");
	}
	//Manejo para indicar que el archivo esta abierto
}


/*
 *  System call interface: OpenFileId Write( char *, int, OpenFileId )
 */
void NachOS_Write() {		// System call 6
  int file = machine-> ReadRegister(6);
	int size = machine->ReadRegister(5);
	int regBuf = machine->ReadRegister(4);
	int c, w;
	switch (file) {
		case ConsoleInput:
			machine->WriteRegister(2,-1);
			break;
		case ConsoleOutput:
			for(int i = 0; i < size; ++i){
				machine->ReadMem(regBuf + i, 1, &c);
				printf("%c\n", c);
			}
			machine->WriteRegister(2,size);
			break;
		case ConsoleError:
			break;
		default:
			//Verificacion de que el archivo esta abierto
			//obtener UnixHandle de tabla
			char* buffer = (char*)calloc(size,sizeof(char));
			for(int i = 0; i < size; ++i){
				machine->ReadMem(regBuf + i, 1, &c);
				buffer[i] = c;
			}
			w = write(file, buffer, size);
			machine->WriteRegister(2,w);
			break;
	}
}


/*
 *  System call interface: OpenFileId Read( char *, int, OpenFileId )
 */
void NachOS_Read() {		// System call 7
  int file = machine-> ReadRegister(6);
	int size = machine->ReadRegister(5);
	int regBuf = machine->ReadRegister(4);
	//Verificacion de que el archivo esta abierto
	//obtener UnixHandle de tabla
	char* buffer = (char*)calloc(size,sizeof(char));
	int r = read(file,buffer,size);
	if(r < 0){
		printf("Error in Read\n");
		return;
	}
	int c;
	for(int i = 0; i < size; ++i){
			c = buffer[i];
			machine->WriteMem( regBuf + i, 1, c);
	}
	machine->WriteRegister(2,r);
}


/*
 *  System call interface: void Close( OpenFileId )
 */
void NachOS_Close() {		// System call 8
}


/*
 *  System call interface: void Fork( void (*func)() )
 */
void NachOS_Fork() {		// System call 9
}


/*
 *  System call interface: void Yield()
 */
void NachOS_Yield() {		// System call 10
}


/*
 *  System call interface: Sem_t SemCreate( int )
 */
void NachOS_SemCreate() {		// System call 11

   int initialValue = machine->ReadRegister(4);
   Semaphore *sem = new Semaphore("NachOs Sem", initialValue);

   int semid = currentThread->getObjectsTable()->Create((int *) sem);
   printf("%d\n", semid);
   machine->WriteRegister(2, semid);

}


/*
 *  System call interface: int SemDestroy( Sem_t )
 */
void NachOS_SemDestroy() {		// System call 12

   int status = -1;
   int semid = machine->ReadRegister(4);

   if(currentThread->getObjectsTable()->Exists(semid)) {
      status = 0;
      currentThread->getObjectsTable()->Delete(semid);
   }

   machine->WriteRegister(2, status);

}


/*
 *  System call interface: int SemSignal( Sem_t )
 */
void NachOS_SemSignal() {		// System call 13

   int status = -1;
   int semid = machine->ReadRegister(4);
   Semaphore * sem = (Semaphore *) currentThread->getObjectsTable()->getObject(semid);

   if(currentThread->getObjectsTable()->Exists(semid)) {
      status = 0;
      sem->V();
   }

   machine->WriteRegister(2, status);

}


/*
 *  System call interface: int SemWait( Sem_t )
 */
void NachOS_SemWait() {		// System call 14

   int status = -1;
   int semid = machine->ReadRegister(4);
   Semaphore * sem = (Semaphore *) currentThread->getObjectsTable()->getObject(semid);

   if(currentThread->getObjectsTable()->Exists(semid)) {
      status = 0;
      sem->P();
   }

   machine->WriteRegister(2, status);

}


/*
 *  System call interface: Lock_t LockCreate( int )
 */
void NachOS_LockCreate() {		// System call 15
}


/*
 *  System call interface: int LockDestroy( Lock_t )
 */
void NachOS_LockDestroy() {		// System call 16
}


/*
 *  System call interface: int LockAcquire( Lock_t )
 */
void NachOS_LockAcquire() {		// System call 17
}


/*
 *  System call interface: int LockRelease( Lock_t )
 */
void NachOS_LockRelease() {		// System call 18
}


/*
 *  System call interface: Cond_t LockCreate( int )
 */
void NachOS_CondCreate() {		// System call 19
}


/*
 *  System call interface: int CondDestroy( Cond_t )
 */
void NachOS_CondDestroy() {		// System call 20
}


/*
 *  System call interface: int CondSignal( Cond_t )
 */
void NachOS_CondSignal() {		// System call 21
}


/*
 *  System call interface: int CondWait( Cond_t )
 */
void NachOS_CondWait() {		// System call 22
}


/*
 *  System call interface: int CondBroadcast( Cond_t )
 */
void NachOS_CondBroadcast() {		// System call 23
}


/*
 *  System call interface: Socket_t Socket( int, int )
 */
void NachOS_Socket() {			// System call 30
   /* Socket creates a Unix socket
    *  socket family: 0 for AF_INET, 1 for AF_INET6
    *  socket type: 0 for SOCK_STREAM, 1 for SOCK_DGRAM
    *  socket protocol: 0
    */
   Socket_t id = 0;
   int family = machine->ReadRegister( 4 );
   int type = machine->ReadRegister(5);

   currentThread->getFileTable()->Open();

   if(family){
      if(type){
         id = socket( AF_INET6, SOCK_DGRAM, 0 );
      } else {
         id = socket( AF_INET6, SOCK_STREAM, 0 );
      }
      currentThread->getFileTable()->Mark(); // IPv6
   } else {
      if(type){
         id = socket( AF_INET, SOCK_DGRAM, 0 );

      } else {
         id = socket( AF_INET, SOCK_STREAM, 0 );

      }
      currentThread->getFileTable()->Clear(); // IPv4
   }
   
   if(-1 == id){
		perror( "NachOS_Socket()" );
      printf("id: %d\n", id);
		exit( 30 );
	}

   machine->WriteRegister(2, id);

}


/*
 *  System call interface: Socket_t Connect(Sock_t, char *, int )
 */
void NachOS_Connect() {		// System call 31

   int resultado;
	struct sockaddr_in servidor;

   Socket_t SockId = machine->ReadRegister( 4 );
   //readMem para leer cosas de la memoria // ¿Cómo leer un byte?
   //bool ReadMem(int addr, int size, int* value);
   int IP_Addr_CharPtr = machine->ReadRegister( 5 );
   int port = machine->ReadRegister( 6 );

   char* ip_address = NULL;
   int c = 1;
   char buffer[ 100 ];
   int i=0;
   while( c != 0 ){
      machine->ReadMem(IP_Addr_CharPtr, 1, &c);
      buffer[i]=c;
      //printf("%c",c);
      IP_Addr_CharPtr = IP_Addr_CharPtr+1;
      ++i;
   }
   buffer[i]='\0';
   // printf("\n");
   // i=0;
   // while( buffer[i] != 0 ){
      // printf("%c",buffer[i]);
      // ++i;
   // }
   // printf("\n");

   bzero((char *)&servidor, sizeof(servidor)); // servidor = { 0 }

   
   if( currentThread->getFileTable()->Test() ){
		servidor.sin_family = AF_INET6;
	}else{
		servidor.sin_family = AF_INET; // IPv4
	}

	servidor.sin_family = AF_INET_NachOS; // AF_INET, IPv4
	inet_pton( AF_INET_NachOS, (const char *) buffer, &servidor.sin_addr ); // IPv4
   servidor.sin_port = htons(port);

	//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	resultado = connect( SockId, (struct sockaddr *) &servidor, sizeof(servidor) );

   if(-1 == resultado){
		perror( "NachOS_Connect()" );
		exit(31);
	}

	machine->WriteRegister(2, resultado);


}


/*
 *  System call interface: int Bind( Socket_t, int )
 */
void NachOS_Bind() {		// System call 32

   Socket_t SockId = machine->ReadRegister( 4 );
   int port = machine->ReadRegister( 4 );
   struct sockaddr_in address;
   int resultado = 0;

   address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl( INADDR_ANY ); // ?? Verificar si esta constante da un posible error // INADDR_ANY (0.0.0.0) means any address for binding // https://man7.org/linux/man-pages/man7/ip.7.html
	address.sin_port = htons( port );

   resultado = bind( SockId, (const struct sockaddr *)& address, sizeof(address) );
   if(-1 == resultado){
		perror( "NachOS_Bind()" );
		exit(32);
	}

   machine->WriteRegister(2, resultado);

}


/*
 *  System call interface: int Listen( Socket_t, int )
 */
void NachOS_Listen() {		// System call 33

   Socket_t SocketId = machine->ReadRegister( 4 );
   int backlog = machine->ReadRegister( 5 );
   int resultado;

	resultado = listen( SocketId, backlog ); // backlog = queu = el tamaño de la cola
   if(-1 == resultado){
		perror( "NachOS_Listen()" );
		exit(33);
	}
   machine->WriteRegister(2, resultado);

}


/*
 *  System call interface: int Accept( Socket_t )
 */
void NachOS_Accept() {		// System call 34

   int newSocket;
   Socket_t SockId = machine->ReadRegister(4);

	socklen_t address_size;
	struct sockaddr_in address;
	// int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
	address_size = sizeof(address);
   newSocket = accept( SockId, (struct sockaddr *) & address, &address_size );
	if(-1 == newSocket){
		perror( "NachOS_Accept()" );
		exit(34);
	}
	//newSocket = new Socket(newSocket); // ? se guardaría en la tabla
   machine->WriteRegister(2, newSocket);

}


/*
 *  System call interface: int Shutdown( Socket_t, int )
 */
// int shutdown(int socket, int how);
	// how mode the type of shutdown. The values are as follows:
	// SHUT_RD: Disables further receive operations.
	// SHUT_WR: Disables further send operations.
	// SHUT_RDWR: Disables further send and receive operations.

void NachOS_Shutdown() {	// System call 25

   Socket_t SockId = machine->ReadRegister(4);
   int mode = machine->ReadRegister(5);
   int resultado;

	if( 1 == mode ){
		resultado = shutdown(SockId, SHUT_WR); // ? Verificar que el valor de las constantes se mantega y que no cambie como en el AF?INET
	} else {
		if( 0 == mode ){
			resultado = shutdown(SockId, SHUT_RD);
		}
		else{
			resultado = shutdown(SockId, SHUT_RDWR);
		}
	}

	if( -1 == resultado ){
		perror( "NachOS_Shutdown" );
		exit(35);
	}

   machine->WriteRegister(2, resultado);

}


//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch ( which ) {

       case SyscallException:
          switch ( type ) {
             case SC_Halt:		// System call # 0
                NachOS_Halt();
                break;
             case SC_Exit:		// System call # 1
                NachOS_Exit();
                break;
             case SC_Exec:		// System call # 2
                NachOS_Exec();
                break;
             case SC_Join:		// System call # 3
                NachOS_Join();
                break;

             case SC_Create:		// System call # 4
                NachOS_Create();
                break;
             case SC_Open:		// System call # 5
                NachOS_Open();
                break;
             case SC_Read:		// System call # 6
                NachOS_Read();
                break;
             case SC_Write:		// System call # 7
                NachOS_Write();
                break;
             case SC_Close:		// System call # 8
                NachOS_Close();
                break;

             case SC_Fork:		// System call # 9
                NachOS_Fork();
                break;
             case SC_Yield:		// System call # 10
                NachOS_Yield();
                break;

             case SC_SemCreate:         // System call # 11
                NachOS_SemCreate();
                break;
             case SC_SemDestroy:        // System call # 12
                NachOS_SemDestroy();
                break;
             case SC_SemSignal:         // System call # 13
                NachOS_SemSignal();
                break;
             case SC_SemWait:           // System call # 14
                NachOS_SemWait();
                break;

             case SC_LckCreate:         // System call # 15
                NachOS_LockCreate();
                break;
             case SC_LckDestroy:        // System call # 16
                NachOS_LockDestroy();
                break;
             case SC_LckAcquire:         // System call # 17
                NachOS_LockAcquire();
                break;
             case SC_LckRelease:           // System call # 18
                NachOS_LockRelease();
                break;

             case SC_CondCreate:         // System call # 19
                NachOS_CondCreate();
                break;
             case SC_CondDestroy:        // System call # 20
                NachOS_CondDestroy();
                break;
             case SC_CondSignal:         // System call # 21
                NachOS_CondSignal();
                break;
             case SC_CondWait:           // System call # 22
                NachOS_CondWait();
                break;
             case SC_CondBroadcast:           // System call # 23
                NachOS_CondBroadcast();
                break;

             case SC_Socket:	// System call # 30
               NachOS_Socket();
               break;
             case SC_Connect:	// System call # 31
               NachOS_Connect();
               break;
             case SC_Bind:	// System call # 32
               NachOS_Bind();
               break;
             case SC_Listen:	// System call # 33
               NachOS_Listen();
               break;
             case SC_Accept:	// System call # 32 // debería ser 34
               NachOS_Accept();
               break;
             case SC_Shutdown:	// System call # 33 // debería ser 35
               NachOS_Shutdown();
               break;

             default:
                printf("Unexpected syscall exception %d\n", type );
                ASSERT( false );
                break;
          }
          IncrementPCRegs();
          break;

       case PageFaultException: {
          break;
       }

       case ReadOnlyException:
          printf( "Read Only exception (%d)\n", which );
          ASSERT( false );
          break;

       case BusErrorException:
          printf( "Bus error exception (%d)\n", which );
          ASSERT( false );
          break;

       case AddressErrorException:
          printf( "Address error exception (%d)\n", which );
          ASSERT( false );
          break;

       case OverflowException:
          printf( "Overflow exception (%d)\n", which );
          ASSERT( false );
          break;

       case IllegalInstrException:
          printf( "Ilegal instruction exception (%d)\n", which );
          ASSERT( false );
          break;

       default:
          printf( "Unexpected exception %d\n", which );
          ASSERT( false );
          break;
    }
}
