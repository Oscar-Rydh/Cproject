#include "buddy.h"
#include <stdio.h>
#include <math.h>

// Global definitions
#define MAX_K 10
#define BLOCK_META_SIZE sizeof(struct block_t)
#define MAX_BLOCK_SIZE (1L << MAX_K)

// The list that contains free elements
// At place i, blocks of size s^i is stored
block_t* freelist[MAX_K+1];

/**
 * Internal Helpers
 */
static void split_block(struct block_t* block) {

    // Reduce kval of block
    --block->kval;

    // Calculate address of other half
    size_t block_size = (1L << block->kval);
    char* buddy_address = ((char*) block) + block_size;
    struct block_t* buddy = (struct block_t*) buddy_address;

    // Set attributes of buddy
    buddy->reserved = 0;
    buddy->kval = block->kval;

    // Set attributes of self
    block->reserved = 0;

    // Add both to freelist
    buddy->next = freelist[block->kval];
    if(buddy->next) {
        buddy->next->prev = buddy;
    }
    buddy->prev = block;
    block->next = buddy;
    block->prev = NULL;
    freelist[block->kval] = block;
}

static char calc_min_kval_for_size(size_t size) {
    int K = MAX_K;
    while(pow(2, K-1) >= size) {
        // printf("Potential size: %d\n", (int)pow(2, K-1));
        // printf("K: %d\n", K);
        --K;
    }
    printf("K: %d\n", K);
    // printf("Size that will be malloced: %d\n", (int)pow(2, K));
    return (char)K;
}

static struct block_t* get_block_with_kval_if_exist(char K) {

    printf("Will get a block or NULL with K: %d\n", (int)K);

    // Retrieve reference to the block
    struct block_t* block = freelist[K];
    if(!block) {
        printf("Could not get block with K: %d. Returning NULL\n", (int)K);
        return NULL;
    }

    // Set the block as reserved
    block->reserved = 1;

    // Remove block from freelist
    freelist[K] = block->next;
    if(block->next) {
        freelist[K]->prev = NULL;
    }
    printf("Found block with K: %d\n", (int)K);
    return block;
}

static struct block_t* find_free_block_with_atleast_kval(char K) {

    printf("Will find a free block with K: %d\n", (int)K);

    char temp_K = K;
    struct block_t* block;
    
    while(!(block = get_block_with_kval_if_exist(temp_K)) && temp_K < MAX_K) {
        ++temp_K;
    }

    // If no block could be found
    if(!block) {
        printf("Could not find any block at all\n");
        return NULL;
    }

    // Split the block until it is at the right level
    while(block->kval > K) {
        split_block(block);
        block = get_block_with_kval_if_exist(block->kval);
    }

    return block;
}

/**
 * Malloc methods starts here
 */
// Hitta ledigt minne och returnera pekare
void* malloc(size_t size) {
    // Respect the BLOCK_META_SIZE
    size_t true_size = (int)size + BLOCK_META_SIZE;

    printf("Size to malloc: %d\n", (int)true_size);

    // Hitta minsta möjliga K där 2^K >= size
    char K = calc_min_kval_for_size(true_size);    

    // Hitta första blocket freelist[K]. 
    // Om freelist[K] är tom, leta efter större block
    // Om inget större block finns heller, returnera NULL
    // Om det fanns ett block i freelist[K], returnera det
    // Om det bara fanns större block, splitta ett av de rekursivt tills freelist[K] finns
    struct block_t* block = find_free_block_with_atleast_kval(K);
    
    // There was no suitable block
    if(!block) {
        printf("Could not find a suitable block at all\n");
        return NULL;
    }

    // Return block address and move pointer BLOCK_META_SIZE steps
    printf("K of block: %d\n", (int)block->kval);
    
    // Print freelist status for verification
    int temp_k;
    for(temp_k = MAX_K; temp_k > 0; temp_k--)
        printf("Freelist K: %d. %s\n", temp_k, freelist[temp_k] == NULL ? "false" : "true");
    return block + BLOCK_META_SIZE;
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

void init() {
    // Allocate all memory at first
    block_t* starting_block = (block_t*) sbrk((1L << MAX_K));
    starting_block->kval = MAX_K;
    starting_block->reserved = 0;
    starting_block->prev = NULL;
    starting_block->next = NULL;
    freelist[MAX_K] = starting_block;
}

int main ( int argc, char **argv ) {
    printf("I am running\n");
    init();
    malloc(16);
}