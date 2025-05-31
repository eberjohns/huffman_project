#include "min_priority_queue.h" // Includes MinPriorityQueue struct and function prototypes
#include "huffman_node.h"       // Explicitly include, though min_priority_queue.h does

#include <stdlib.h> // For malloc, free, exit
#include <stdio.h>  // For perror, fprintf

// --- HuffmanNode Utility (Moved from huffman_node.h) ---
// HuffmanNode* create_huffman_node(int ch, int freq, HuffmanNode* left, HuffmanNode* right) {
//     HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
//     if (node == NULL) {
//         perror("Failed to allocate HuffmanNode");
//         exit(EXIT_FAILURE);
//     }
//     node->ch = ch;
//     node->frequency = freq;
//     node->left = left;
//     node->right = right;
//     return node;
// }
// --- End HuffmanNode Utility ---

// Helper function to swap HuffmanNode pointers
static void swap_nodes(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Helper function to maintain heap property upwards
static void sift_up(MinPriorityQueue* pq, int index) {
    int parent_index = (index - 1) / 2;
    while (index > 0 && pq->array[parent_index]->frequency > pq->array[index]->frequency) {
        swap_nodes(&pq->array[parent_index], &pq->array[index]);
        index = parent_index;
        parent_index = (index - 1) / 2;
    }
}

// Helper function to maintain heap property downwards
static void sift_down(MinPriorityQueue* pq, int index) {
    int smallest = index;
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;

    if (left_child < pq->size && pq->array[left_child]->frequency < pq->array[smallest]->frequency) {
        smallest = left_child;
    }
    if (right_child < pq->size && pq->array[right_child]->frequency < pq->array[smallest]->frequency) {
        smallest = right_child;
    }

    if (smallest != index) {
        swap_nodes(&pq->array[index], &pq->array[smallest]);
        sift_down(pq, smallest);
    }
}

// Public functions for MinPriorityQueue

MinPriorityQueue* create_min_pq(int capacity) {
    MinPriorityQueue* pq = (MinPriorityQueue*)malloc(sizeof(MinPriorityQueue));
    if (pq == NULL) {
        perror("Failed to allocate MinPriorityQueue");
        exit(EXIT_FAILURE);
    }
    // Allocate space for pointers to HuffmanNodes
    pq->array = (HuffmanNode**)malloc(sizeof(HuffmanNode*) * capacity);
    if (pq->array == NULL) {
        perror("Failed to allocate PQ array");
        free(pq);
        exit(EXIT_FAILURE);
    }
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void insert_pq(MinPriorityQueue* pq, HuffmanNode* node) {
    if (pq == NULL || node == NULL) return; // Basic safety check

    if (pq->size == pq->capacity) {
        // Simple resizing: double the capacity
        int new_capacity = pq->capacity * 2;
        if (new_capacity == 0) new_capacity = 10; // Handle initial 0 capacity case
        HuffmanNode** new_array = (HuffmanNode**)realloc(pq->array, sizeof(HuffmanNode*) * new_capacity);
        if (new_array == NULL) {
            perror("Failed to resize PQ array");
            exit(EXIT_FAILURE);
        }
        pq->array = new_array;
        pq->capacity = new_capacity;
    }

    pq->array[pq->size] = node;
    pq->size++;
    sift_up(pq, pq->size - 1);
}

HuffmanNode* extract_min_pq(MinPriorityQueue* pq) {
    if (pq == NULL || pq->size <= 0) {
        return NULL; // Queue is empty or invalid
    }
    HuffmanNode* min_node = pq->array[0]; // The root is the minimum
    pq->array[0] = pq->array[pq->size - 1]; // Move the last element to the root
    pq->size--;
    sift_down(pq, 0); // Restore heap property by sifting down from the new root
    return min_node;
}

int is_empty_pq(MinPriorityQueue* pq) {
    return (pq == NULL || pq->size == 0);
}

void free_min_pq(MinPriorityQueue* pq) {
    if (pq != NULL) {
        free(pq->array); // Free the array itself
        free(pq);        // Free the PQ struct
    }
}