#include "buddy.h"

// Defines N
#define N 10

// The list that contains free elements
// At place i, blocks of size s^i is stored
block_t* freelist[N+1];

void init() {
    // Allocate all memory at first
    freelist[N] = (block_t*) sbrk((1L << N));
}

// void* malloc(size_t size) {

// }

int main ( int argc, char **argv ) {
    init();
    printf("I am running\n");
}