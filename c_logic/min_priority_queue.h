#ifndef MIN_PRIORITY_QUEUE_H
#define MIN_PRIORITY_QUEUE_H

#include "huffman_node.h" // Includes the definition of HuffmanNode

// Define the MinPriorityQueue struct
typedef struct MinPriorityQueue {
    HuffmanNode **array; // Array of POINTERS to HuffmanNodes
    int size;           // Current number of elements in the heap
    int capacity;       // Maximum capacity of the heap
} MinPriorityQueue;

// Declare functions for MinPriorityQueue
MinPriorityQueue* create_min_pq(int capacity);
void insert_pq(MinPriorityQueue* pq, HuffmanNode* node);
HuffmanNode* extract_min_pq(MinPriorityQueue* pq);
int is_empty_pq(MinPriorityQueue* pq); // Add a helper to check if PQ is empty
// You'll also want a function to free the PQ
void free_min_pq(MinPriorityQueue* pq);

#endif // MIN_PRIORITY_QUEUE_H