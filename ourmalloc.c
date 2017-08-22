#include <stddef.h>
#include <stdio.h>
#include "ourmalloc.h"

list_t available_blocks = { .next = NULL };



void* malloc(size_t size){

    // Aligning size    
    size_t final_size = (size + sizeof(list_t));
    if (size <= 0) {
        return NULL;
    }
    
    list_t* current = &available_blocks;
    list_t* next = available_blocks.next;

    // Search for available blocks
    while (next != NULL && next->size < final_size) {
        current = next;
        next = next->next;
    }

    // No available block was found
    if (next == NULL) {
        void* block;
        block = sbrk(final_size);

        if (block == -1) {
            return NULL;
        }
        next = (list_t*) block;
        next->size= final_size;
    // Use available block
    } else {
        current->next = next->next;
    }

  return  (void*) next->data;
}

void free(void* ptr) {

    if (ptr == NULL) {
        return;
    }
    list_t* to_be_freed = (list_t*) ((char*) ptr - sizeof(list_t));
    list_t* current = &available_blocks;
    list_t* next = available_blocks.next;

    // Iterate to correct possition
    while(next != NULL && to_be_freed > next) {
        current = next;
        next = next->next;
    }
    

    list_t* compare_free_with_next = to_be_freed + to_be_freed->size;
    list_t* compare_free_with_current = (list_t*) ((char*) current + current->size);
    // Address to be freed can be merged with current node
    if (compare_free_with_current == to_be_freed){
        current->size = current->size + to_be_freed->size;
    // Address to be freed can be merged with current next
    } else if (compare_free_with_next == next){
        to_be_freed->size = next->size;
        current->next = to_be_freed;
        to_be_freed->next = next->next;
    // Address to be freed can be merged with both nodes
    } else if  (compare_free_with_current == to_be_freed &&
                compare_free_with_next == next){
        current->size = current->size + to_be_freed->size + next->size; 
        current->next = next->next;
    //Nothing can be merged
    } else {
        current->next = to_be_freed;
        to_be_freed->next = next;
    }
}

void* calloc(size_t nmemb, size_t size) {
    
    if (nmemb == 0 || size == 0) {
        return NULL;
    }

    size_t final_size = nmemb * size;

    char* data_block = malloc(final_size);

    if (data_block == NULL) {
        return NULL;
    }
    size_t i = 0;
    for (i = 0; i < final_size; i++) {
        data_block[i] = 0;
    }
    return (void*) data_block;
}

void* realloc(void* ptr, size_t size){

    if (ptr == NULL) {
        return malloc(size);
    }
    if (ptr != NULL && size == 0) {
        free(ptr);
        return;
    }

    void* block = malloc(size);
    if (block == NULL) {
        return NULL;
    }
    list_t* new_block = (list_t*) ((char*) block - sizeof(list_t));
    list_t* current_block = (list_t*) ((char*) ptr - sizeof(list_t));
    size_t new_size = 0;
    if(current_block->size - sizeof(list_t) < size){
        new_size = current_block->size - sizeof(list_t);
    } else {
        new_size = size;
    }
    // Copy data to reallocated block
    size_t i;
    for (i = 0; i < new_size; i++){
        new_block->data[i] = current_block->data[i];
    }
    free(ptr);

    return new_block->data;

}