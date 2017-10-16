#include <stddef.h>
typedef struct block_t block_t;

struct block_t {
    unsigned    reserved:1;
    char        kval;
    block_t*    prev;
    block_t*    next;
};

// https://github.com/Thoronion/Project-EDAF35
// https://github.com/HaraldNordgren/edaf35


void* malloc(size_t size);

void* calloc(size_t count, size_t size);

void* realloc(void* ptr, size_t size);

void free(void* ptr);