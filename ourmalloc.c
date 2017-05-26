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


    //Check if available free memory exists
    //If it does, remove it from the free pool and reallocate it
    if (freed != 0) {
        list_t* current_free = freed;
        list_t* previous = NULL;
        while (current_free != NULL) {
            if (current_free->size >= size) {
                if (previous != NULL) {
                    //Move pointers in free memory
                    previous->next = current_free->next;
                    current_free->next = NULL;
                } else {
                    //Move head of free
                    freed = current_free->next;
                    current_free->next = NULL;
                }
                if (allocated == NULL) {
                    //Set new head for allocated memory
                    allocated = current_free;
                } else {
                    // Add the new allocated memory as head.
                    current_free->next = allocated;
                    allocated = current_free;
                }
                return (void*) current_free->data;
            }
            previous = current_free;
            current_free = current_free->next;
        }
    }
    // Allocate new memory
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

    return (void*) entry->data;

}

void free(void* ptr) {
    list_t* current = allocated;
    int pointer_to_data = (unsigned int) ptr;
    int pointer_to_list = ptr - sizeof(list_t);

    // Find node that is pointing at our target ( call this parent )
    list_t* target = NULL;
    //Check if free is for head
    if ((int*) current != pointer_to_list) {
        while ((int *) current->next != pointer_to_list) {
            current = current->next;
        }
        target = current->next;
        current->next = target->next;
        //printf("Freeing something in the middle\n");
    } else {
        //Move head of allocated
        //printf("Moving the allocated head to %d\n", allocated->next);
        allocated = allocated->next;
        target = current;
    }

    // // Place target in free queue
     if(freed == NULL) {
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
    printf("Freeing b\n");
    free(b);
    printf("Freeing c\n");
    free(c);
    printf("Freeing d\n");
    free(d);
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
    void* e = malloc(260);
    void* f = malloc(360);
    void* g = malloc(180);
    void* h = malloc(500);
    void* k = malloc(1000);
    if (freed == NULL) {
        printf("All free memory was reallocated\n");
    }
    printf("Freed is: %d\n", freed);
    free(k);
    free(h);
    free(f);
    current = freed;
    while(current != NULL) {
        printf("Free memory sizes: %d\n", current->size);
        current=current->next;
    }

    // SIZE WILL BE BIGGER IF A PREVIOUSLY BIGGER ALLOCATION WAS MADE
    void* m = malloc(800);
    void* l = malloc(700);

    current = allocated;
    while(current != NULL) {
        printf("Allocated sizes: %d\n", current->size);
        current=current->next;
    }

    free(e);
    free(l);

    current = freed;
    while(current != NULL) {
        printf("Free memory sizes: %d\n", current->size);
        current=current->next;
    }

}