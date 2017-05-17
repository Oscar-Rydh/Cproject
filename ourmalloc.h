

typedef struct list_t list_t;

struct list_t {
    size_t      size;
    list_t*     next;
    char        data[];
};

void* malloc(size_t size);