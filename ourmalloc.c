#include <stddef.h>
#include <stdio.h>
#include "ourmalloc.h"
#define N (8 * 1024*1024 / sizeof(size_t))

list_t* head = NULL;

void* malloc(size_t size){

    printf("\n\nMalloc called with size: %d\n", size);
    printf("head: %d\n", head);

    void* new_memory_address = sbrk(size);
    printf("Allocated: %d\n", new_memory_address);

    if ((int*)new_memory_address == -1) {
        printf("Could not allocate memory\n");
    }

    list_t* entry = (list_t*) new_memory_address;
    entry->size = size;
    entry->next = NULL;
    
    //list_t entry;
    //entry.size = size;
    //entry.next = NULL;
    //entry.data = new_data;

    if (head == NULL) {
        printf("Running head");
        head = entry;
        //*head = entry;
    } else {
        printf("Running else\n");
        list_t* current = head;
        //list_t* next = (list_t*) new_memory_address;
        printf("Next is %d\n", current->next);
        printf("NULL IS %d\n", NULL);
        while (current->next != NULL) {
            printf("Starting iteration\n");


            //list_t* current_next = current.next;
            //current = *current_next;
            printf("Current next is: %d\n", current->next);
            current = current->next;
            printf("Nexit size: %d\n", current->size);
        }
        current->next = entry;
        printf("Assigned next is: %d\n", current->next);
        //*next = current;
    }


    //return entry;
    //return entry.data;
}


int main ( int argc, char **argv ) {
    printf("Program started \n");
    malloc(10);
    malloc(20);
    malloc(30);
    malloc(40);
    //printf("address start: %d", test);
}