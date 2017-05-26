
typedef struct block_t block_t;

struct block_t {
    unsigned    reserved:1;
    char        kval;
    block_t*     succ;
    block_t*     pred;
};



// void* malloc(size_t size);