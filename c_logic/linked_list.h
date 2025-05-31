// linked_list.h
#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h> // For malloc, free
#include <stdio.h>  // For perror

typedef struct Node {
    int ch;
    struct Node *next;
} Node;

// Function declarations for linked list
Node* create_node(int ch);
void linked_list_push(Node** head, int ch);
int linked_list_pop(Node** head);
void linked_list_free(Node** head, int largest_freq_index); // Corrected parameter name

#endif // LINKED_LIST_H