#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 2048

int allocate_memory(process_t* process, int* memory) {
    int best_start = -1; // starting address of best-fit memory block
    int best_size = MEMORY_SIZE + 1; // size of best-fit memory block
    int best_end = -1;
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) { // check if block is free
            int size = 0; // size of current memory block
            for (int j = i; j < MEMORY_SIZE && memory[j] == 0; j++) {
                size++;
            }
            if (size >= process->memory_size && size < best_size ) { // check if block is large enough and best fit so far
                if (i + size == best_end) continue;
                
                best_start = i;
                best_size = size;
                best_end = i + size;

            }
        }
    }
    if (best_start != -1) { // allocate memory
        process->memory_alloc_index = best_start;
        for (int i = best_start; i < best_start + process->memory_size; i++) {
            memory[i] = 1;
        }
        return 1;
    }
    return 0;
}

void free_memory(process_t* process, int* memory) {
    for (int i = process->memory_alloc_index; i < process->memory_alloc_index + process->memory_size; i++) {
        memory[i] = 0;
    }
    // merge adjacent free blocks if they exist
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == 0) {
            int size = 0;
            for (int j = i; j < MEMORY_SIZE && memory[j] == 0; j++) {
                size++;
            }
            if (size > 0) {
                // merge free block with current block
                for (int k = i; k < i + size; k++) {
                    memory[k] = 0;
                }
            }
        }
    }
}
