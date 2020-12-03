#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
   char i; 
   for ( i='a'; i<='d'; i++ ) {
       
       if ( 'c' == i ) {
          fork();
       }
       printf( "%d", i );
       
   }

}