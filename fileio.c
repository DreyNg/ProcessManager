#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "struct.h"
#include "linkedlist.h"
#include "fileio.h"

void read_args(int argc, char **argv, char **filename, char **scheduler, char **memory_strat, int *quantum){
    int i;
    while ((i = getopt(argc, argv, "f:s:m:q:")) != -1){
        switch (i){
            case 'f':
                *filename = optarg;
                break;
            case 's':
                *scheduler = optarg;
                break;
            case 'm':
                *memory_strat = optarg;
                break;
            case 'q':
                *quantum = atoi(optarg);
                break;
        }
    }
}

linkedlist_t* read_input(const char *filename) {
    linkedlist_t *list = create_list();

    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        // printf("Error opening file\n");
        return list;
    }

    int arrival_time, cpu_time, memory_size;
    char name[10];
    while (fscanf(fp, "%d %s %d %d", &arrival_time, name, &cpu_time, &memory_size) != EOF) {
        process_t process = {arrival_time, "", cpu_time, memory_size, 0 , 0, cpu_time, -1};
        strcpy(process.name, name);
        insert(list, process, list->size);
    }

    fclose(fp);
    return list;
}
