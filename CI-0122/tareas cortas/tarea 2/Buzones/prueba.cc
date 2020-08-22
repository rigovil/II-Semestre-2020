#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define KEY 0xB78292
int main(int argc, char ** argv) {
    double a;
    sscanf("12.2", "%le", &a);
    printf("%f\n", a+1);

    double b = 12.012;
    char c[sizeof(double)];
    sprintf(c, "%1.5f", b);
    printf("%s\n", c);

}