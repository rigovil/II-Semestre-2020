#include "Buzon.h"

Buzon :: Buzon(bool queue) {

    if(!queue) {
        id = msgget(KEY, 0666 | IPC_CREAT);
    }
    else {
        id = msgget(KEY, 0666);
    }

    if(-1 == id) {
        perror("Buzon::Buzon");
        exit(0);
    }

}


// Buzon :: ~Buzon() {

//     if(-1 == msgctl(id, IPC_RMID, NULL)) {
//         perror("Buzon::~Buzon");
//         exit(0);
//     }

// }


/**
 * Envía un mensaje utilizando buzones.
 * @param mensaje dirección del buffer donde se encuentra el mensaje.
 * @return 0 en caso de éxito.
 */

int Buzon :: Enviar(const char * mensaje) {

    int resultado;
    struct msgbuf {
        long mtype;
        char buf[MSG_SIZE];
    };

    struct msgbuf msg;
    msg.mtype = 1;
    strncpy(msg.buf, mensaje, MSG_SIZE);
    
    resultado = msgsnd(id, (void *) &msg, sizeof(msg), IPC_NOWAIT);

    if(-1 == resultado) {
        perror("Buzon::Enviar");
        exit(0);
    }

    return resultado;

}


/**
 * Envía un mensaje utilizando buzones.
 * @param mensaje dirección del buffer donde se encuentra el mensaje.
 * @param cantidad ???
 * @param tipo identificador del mensaje en mtype.
 * @return 0 en caso de éxito.
 */

int Buzon :: Enviar(const char * mensaje, int cantidad, long tipo) {

    int resultado;
    struct msgbuf {
        long mtype;
        int cantidad;
        char buf[MSG_SIZE];
    };

    struct msgbuf msg;
    msg.mtype       = tipo;
    msg.cantidad    = cantidad;
    strncpy(msg.buf, mensaje, MSG_SIZE);

    resultado = msgsnd(id, (void *) &msg, sizeof(msg), IPC_NOWAIT);

    if(-1 == resultado) {
        perror("Buzon::Enviar");
        exit(0);
    }

    return resultado;

}


/**
 * Recibe un mensaje utilizando buzones.
 * @param mensaje dirección del buffer donde se almacena el mensaje.
 * @param len tamaño en bytes del buffer.
 * @param tipo identificador del mensaje.
 * @return cantidad de bytes recibidos en el mensaje.
 */

int Buzon :: Recibir(void * mensaje, int len, long tipo) {

    int resultado;
    resultado = msgrcv(id, mensaje, len, tipo, IPC_NOWAIT);

    if(-1 == resultado) {
        perror("Buzon::Recibir");
    }

    return resultado;
    
}