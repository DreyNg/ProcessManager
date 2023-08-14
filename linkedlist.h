#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "struct.h"

linkedlist_t* create_list();
void insert(linkedlist_t *list, process_t process, int index);
process_t* get_process(linkedlist_t *list, int index);
void remove_at(linkedlist_t *list, int index);
int move_to_end(linkedlist_t *list);

#endif /* LINKEDLIST_H_ */
