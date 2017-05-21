#include <stddef.h>
#include <stdio.h>
#include "ourmalloc.h"
#define N (8 * 1024*1024 / sizeof(size_t))

list_t* allocated = NULL;
list_t* freed = NULL;

void* malloc(size_t size){

    /* 
        How much size do we need to store meta information?

        There seems to be some kind of bug here.

        entry->data
        is not equal to  
        new_memory_address + meta_size

        entry->data is 8 smaller. sizeof(size_t) is 8 smaller too.
        We have subtracted sizeof(size_t) here to fix this mistake, 
        but this is probably not a correct fix. Should this be changed?
    */
    int meta_size = sizeof(list_t) + sizeof(size) - sizeof(size_t);

    // Allocate the memory
    void* new_memory_address = (list_t*) sbrk(size + meta_size);

    if (new_memory_address == -1) {
        printf("Could not allocate memory\n");
    }

    list_t* entry = (list_t*) new_memory_address;
    entry->size = size;

    if (allocated == NULL) {
        allocated = entry;
    } 
    else {
        list_t* current = allocated;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = entry;
    }

    printf("%p\n", new_memory_address + meta_size);
    printf("%p\n", entry->data);
    printf("%d\n", meta_size);

    //return (void*) (new_memory_address + meta_size);
    // Can be used for debugging
    printf("%d\n", (void*) new_memory_address + meta_size);
    printf("%d\n", (void*) entry->data);

    return (void*) entry->data;

}

void free(void* ptr) {
    list_t* current = allocated;

    printf("Size of list_t is: %d\n", sizeof(list_t));
    printf("ptr is %d\n", (int*)ptr);

    list_t* our_list = (int*) ptr - sizeof(list_t);

    printf("Size of the freed area: %d", our_list->size);


    // Hantera ifall ptr inte finns i kön överhuvudtaget
    // Find node that is pointing at our target ( call this parent )
    // while (&(current->next->data) != ptr) {
    //     current = current->next;
    // }
    // list_t* target = current->next;
    
    // // make parent point at targets next ( remove target from queue )
    // current->next = target->next;
    // // Place target in free queue
    // if(freed == NULL) {
    //     freed = target;
    //     freed->next = NULL;
    // }
    // else {
    //     // Handle this
    // }
}

int main ( int argc, char **argv ) {
    printf("Program started \n");
    void* a = malloc(26);
    malloc(26);
    malloc(26);
    malloc(26);
    malloc(26);
    malloc(26);
    malloc(26);
    // void* b = malloc(1);
    free(a);
}