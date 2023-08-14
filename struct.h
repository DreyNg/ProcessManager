#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
    int arrival_time;
    char name[10];
    int cpu_time;
    int memory_size;
    double time_turnaround;
    double time_overhead;
    int remaining_time;
    int memory_alloc_index;
} process_t;
// link list
typedef struct node {
    process_t process;
    struct node *next;
} node_t;

typedef struct {
    node_t *head;
    int size;
} linkedlist_t;

#endif /* STRUCT_H */