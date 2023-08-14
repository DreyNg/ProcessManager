#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

// Create an empty linked list
linkedlist_t* create_list() {
    linkedlist_t *list = malloc(sizeof(linkedlist_t));
    list->head = NULL;
    list->size = 0;
    return list;
}

// Add a new process at the given index
void insert(linkedlist_t *list, process_t process, int index) {
    if (index < 0 || index > list->size) {
        printf("insert: Invalid index\n");
        return;
    }

    node_t *new_node = malloc(sizeof(node_t));
    new_node->process = process;

    if (index == 0) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        node_t *prev = list->head;
        for (int i = 0; i < index - 1; i++) {
            prev = prev->next;
        }
        new_node->next = prev->next;
        prev->next = new_node;
    }

    list->size++;
}

// Get the process at the given index
process_t* get_process(linkedlist_t *list, int index) {
    if (index < 0 || index >= list->size) {
        // printf("getProcess: Invalid index\n");
        return NULL;
    }

    node_t *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return &(current->process);
}

// Remove the process at the given index
void remove_at(linkedlist_t *list, int index) {
    if (index < 0 || index >= list->size) {
        // printf("removeAT: Invalid index\n");
        return;
    }

    node_t *to_delete;

    if (index == 0) {
        to_delete = list->head;
        list->head = list->head->next;
    } else {
        node_t *prev = list->head;
        for (int i = 0; i < index - 1; i++) {
            prev = prev->next;
        }
        to_delete = prev->next;
        prev->next = to_delete->next;
    }

    free(to_delete);
    list->size--;
}

// Move the first element to the end of the list
int move_to_end(linkedlist_t *list) {
    if (list->head == NULL || list->head->next == NULL) {
        return 0;
    }
    node_t *head = list->head;
    node_t *new_head = NULL;
    while (head != NULL) {
        if (head->next == NULL) {
            new_head = head;
        }
        head = head->next;
    }
    new_head->next = list->head;
    list->head = list->head->next;
    new_head->next->next = NULL;
    return 1;
}

