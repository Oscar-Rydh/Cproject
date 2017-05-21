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
    printf("%d\n", entry->size);

    //return (void*) (new_memory_address + meta_size);
    // Can be used for debugging
    printf("%d\n", (void*) new_memory_address + meta_size);
    printf("%d\n", (void*) entry->data);

    return (void*) entry->data;

}

void free(void* ptr) {
    list_t* current = allocated;

    printf("Size of list_t is: %d\n", sizeof(list_t));
    //printf("ptr to be freed is %d\n", (list_t*)ptr);
    int pointer_to_data = (unsigned int) ptr;
    int pointer_to_list = ptr - sizeof(list_t);
    printf("Pointer to list to be freed is: %d\n", pointer_to_list);
    printf("Pointer to allocated head: %d\n", current);
    //list_t* to_be_available = (list_t*) pointer_to_list;

    //printf("Size of the freed area: %d\n", to_be_available->size);


    // Hantera ifall ptr inte finns i kön överhuvudtaget
    //We want to remove the head
    printf("Current pointer is: %d\n", (int*)current);


    // Find node that is pointing at our target ( call this parent )
    list_t* target = NULL;
    //Check if free is for head
    if ((int*) current != pointer_to_list) {
        while ((int *) current->next != pointer_to_list) {
            current = current->next;
        }
        target = current->next;
        current->next = target->next;
        printf("Freeing something in the middle\n");
    } else {
        //Move head of allocated
        printf("Moving the allocated head to %d\n", allocated->next);
        allocated = allocated->next;
        target = current;
    }
    // // make parent point at targets next ( remove target from queue )

    // // Place target in free queue
     if(freed == NULL) {
         printf("Setting free head to: %d\n", target);
         freed = target;
         target->next = NULL;
     }
     else {
         list_t * free_current = freed;
        while (free_current->next != NULL) {
            free_current = free_current->next;
        }
         free_current->next = target;
         target->next = NULL;
         printf("Appending to freed next the pointer %d\n", target);
    }
    return;
}

int main ( int argc, char **argv ) {
    printf("Program started \n");
    void* a = malloc(260);
    void* b = malloc(360);
    void* c = malloc(180);
    void* d = malloc(500);
    printf("Freeing a\n");
    free(a);
    printf("Freeing c\n");
    free(c);
    printf("Freeing d\n");
    free(d);
    printf("Freeing b\n");
    free(b);
    list_t* current = freed;
    printf("Freehead is: %d\n", current);
    printf("It has size: %d\n", current->size);
    while (current->next != NULL) {
        printf("The next pointer is: %d\n", current->next);
        printf("The next size is: %d\n", current->next->size);
        current = current->next;
    }
    if (allocated == NULL) {
        printf("All of the memory is freed\n");
    }

}