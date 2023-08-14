#ifndef FILEIO_H
#define FILEIO_H

#include "linkedlist.h"

void read_args(int argc, char **argv, char **filename, char **scheduler, char **memory_strat, int *quantum);
linkedlist_t* read_input(const char *filename);

#endif /* FILEIO_H */
