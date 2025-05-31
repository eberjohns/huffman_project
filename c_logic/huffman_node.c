#include <stdio.h>
#include <stdlib.h> // For memory allocation, file handling, etc>
#include "huffman_node.h" // Include your HuffmanNode definitions

HuffmanNode* create_huffman_node(int ch, int freq, HuffmanNode* left, HuffmanNode* right) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (node == NULL) { perror("Failed to allocate HuffmanNode"); exit(EXIT_FAILURE); }
    node->ch = ch;
    node->frequency = freq; // Use 'frequency' instead of 'f' for consistency
    node->left = left;
    node->right = right;
    return node;
}