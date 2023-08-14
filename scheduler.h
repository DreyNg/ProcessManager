#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "linkedlist.h"

void do_sjf(linkedlist_t *processes, int quantum, char *memory_strat, int* memory);
void do_rr(linkedlist_t *processes, int quantum, char *memory_strat, int* memory);

#endif
