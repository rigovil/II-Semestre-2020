#include "Buzon.h"

Buzon::Buzon(bool existe) {
    if(existe) {
        id = msgget(KEY, 0600);
    }
    else {
        id = msgget(KEY, 0600 | IPC_CREAT);
    }
}


int Buzon::Enviar(const char *msg) {
    struct msgbuf {
        long mtype;
        char array[SIZE];
    };

    struct msgbuf message;
    message.mtype = 1;
    strncpy(message.array, msg, SIZE);
    return msgsnd(id, (void *) &message, sizeof(message), IPC_NOWAIT);
}


int Buzon::Enviar(const char *mensaje, int cantidad, long tipo) {
    int resultado;
    struct msgbuf {
        long mtype;
        int cantidad;
        char buffer[SIZE];
    };

    struct msgbuf message;
    message.mtype = tipo;
    message.cantidad = cantidad;
    strncpy(message.buffer, mensaje, SIZE);
    return msgsnd(id, (void *) &message, sizeof(message), IPC_NOWAIT);
}


int Buzon::Recibir(void *mensaje, int len, long tipo) {
    return msgrcv(id, mensaje, len, tipo, IPC_NOWAIT);
}