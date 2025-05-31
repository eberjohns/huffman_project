#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <stdio.h>
#include <stdlib.h> // For memory allocation, file handling, etc>

typedef struct HuffmanNode {
    int ch;             // Character (or a special value like -1 for internal nodes)
    int frequency;
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

HuffmanNode* create_huffman_node(int ch, int freq, HuffmanNode* left, HuffmanNode* right);

#endif // HUFFMAN_NODE_H