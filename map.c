#include <stdio.h>
#include <stdlib.h>

/* A statically allocated variable (goes in .bss) */
int foo;

/* A statically allocated, pre-initialized variable (goes in .data) */
volatile int stuff = 7;

/* This function is defined in recurse.c */
extern void recur(int i);

int main(int argc, char *argv[]) {
    /* A stack allocated variable */
    volatile int i = 0;

    /* Dynamically allocate some stuff on the heap */
    volatile char *buf1 = malloc(100);
    volatile char *buf2 = malloc(100);

    printf("Addresses in map.c:\n");
    printf("  Address of main:      %p\n", (void *)main);
    printf("  Address of recur:     %p\n", (void *)recur);
    printf("  Address of foo:       %p\n", (void *)&foo);
    printf("  Address of stuff:     %p\n", (void *)&stuff);
    printf("  Address of stack var: %p\n", (void *)&i);
    printf("  Address of heap buf1: %p\n", (void *)buf1);
    printf("  Address of heap buf2: %p\n", (void *)buf2);

    recur(3);

    return 0;
}
