#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** args) {

    char buf[100];
    // buf = malloc(100);

    FILE *archivo = fopen("regions.html", "r+");

    if(archivo == NULL) {
        printf("fopen\n");
    }

    fgets(buf, 10, archivo);

    printf("%s\n", buf);

    int close = fclose(archivo);

    // if(close != 0) {
    //     printf("fclose\n");
    // }

    // free(buf);

    return 0;
}