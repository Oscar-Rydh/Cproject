#include <stddef.h>
#include <stdio.h>
#include "ourmalloc.h"
#define N (8 * 1024*1024 / sizeof(size_t))

list_t* head = NULL;

void* malloc(size_t size){

    // How much size do we need to store meta information?
    int meta_size = sizeof(list_t) + sizeof(size) - sizeof(size_t);

    // Allocate the memory
    void* new_memory_address = (list_t*) sbrk(size + meta_size);

    if (new_memory_address == -1) {
        printf("Could not allocate memory\n");
    }

    list_t* entry = (list_t*) new_memory_address;
    entry->size = size;

    if (head == NULL) {
        head = entry;
    } 
    else {
        list_t* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = entry;
    }

    printf("%p\n", new_memory_address + meta_size);
    printf("%p\n", entry->data);
    printf("%d\n", meta_size);

    return (void*) (new_memory_address + meta_size);
}


int main ( int argc, char **argv ) {
    printf("Program started \n");
    void* a = malloc(10);
}