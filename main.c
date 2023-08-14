#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "struct.h"
#include "linkedlist.h"
#include "fileio.h"
#include "memory.h"
#include "scheduler.h"

#define MEMORY_SIZE 2048



int main(int argc, char **argv){

    // reading input/ args
    char *filename = NULL;
    char *scheduler = NULL;
    char *memory_strat = NULL;
    int quantum = 0;
    read_args(argc, argv, &filename, &scheduler, &memory_strat, &quantum);
    linkedlist_t *processes = read_input(filename);

    // memory block allocation table
    int memory[MEMORY_SIZE] = {0}; 

    // choose scheduler
    if (strcmp(scheduler, "RR") == 0){
        do_rr(processes, quantum, memory_strat, memory);
    }
    else if (strcmp(scheduler, "SJF") == 0){
        do_sjf(processes, quantum, memory_strat, memory);
    }

    return 0;
}