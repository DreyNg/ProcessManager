#ifndef MEMORY_H
#define MEMORY_H

#include "struct.h"

int allocate_memory(process_t* process, int* memory);
void free_memory(process_t* process, int* memory);

#endif /* MEMORY_H */
