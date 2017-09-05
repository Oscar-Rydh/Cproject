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

// Hitta ledigt minne och returnera pekare
void* malloc(size_t size) {
    
    // Hitta minsta möjliga K där 2^K >= size

    // Hitta första blocket freelist[K]. 
    // Om freelist[K] är tom, leta efter större block
    // Om inget större block finns heller, returnera NULL

    // Om det fanns ett block i freelist[K], returnera det
    // Om det bara fanns större block, splitta ett av de rekursivt tills freelist[K] finns

}

// Hitta count st minnen av storlek size, fyll med nollor och returnera pekare
void* calloc(size_t count, size_t size) {
}

// Allokera om ptr till storlek size och returnera ptr
// Om inte minnet inte kan göras större, allokera nytt och kopiera innehåll samt free:a det gamla. Returnera pekare
// Om ptr = NULL, utför malloc
// Om size = 0 och ptr != NULL, allokera så lite minne som möjligt och returnera pekare
void* realloc(void* ptr, size_t size) {
}

// Deallokera ptr
// Om ptr = NULL, gör ingenting
void free(void* ptr) {
}

int main ( int argc, char **argv ) {
    init();
    printf("I am running\n");
}