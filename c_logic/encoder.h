#ifndef ENCODER_H
#define ENCODER_H

// Add this at the top of your file, or in a new header like huffman_codes.h
#include <stdio.h>
#include <stdlib.h> // For memory allocation, file handling, etc>
#include "huffman_node.h" // Include your HuffmanNode definitions
#include <string.h> // Required for strcpy

#define MAX_CODE_LENGTH 256 // Max possible code length (for robustness, n-1 for n characters, so 128-1=127, but keep some buffer)

// Declare the global array for codes (defined in huffman_codes.c)
extern char huffman_codes[128][MAX_CODE_LENGTH];

// Declare functions for code generation
void init_huffman_codes_array();
void build_huffman_codes(HuffmanNode* root);
void print_huffman_codes();
// Function to free the Huffman tree (declaration here, definition in main.c or a separate file)
void free_huffman_tree(HuffmanNode* node);
void encode_and_write_file(const char *input_filename, const char *output_filename);
void write_huffman_map_to_file(const char* map_filename);



#endif // ENCODER_H