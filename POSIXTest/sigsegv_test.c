#include<stdio.h>
#include<stdlib.h>

int main() {
    int *p = malloc(sizeof(int));
    *p = 123;
    printf("*P=%d\n", *p);
    for(int i = 0; i < 10240; i++){
        p++;
    }

    printf("Out of stack: *P=%d\n", *p);
    // Write a block of memory out of stack
    *p = 1234;
    printf("EDITED: %d\n", *p);
    
    p = 0x1234;
    *p = 1234;
    printf("Mem: 0x1234, Vaule: %d", *p);

    return 0;
}
