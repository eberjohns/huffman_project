#include "decoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strlen

// External HuffmanNode creation function (from min_priority_queue.c)
extern HuffmanNode* create_huffman_node(int ch, int freq, HuffmanNode* left, HuffmanNode* right);
// External Huffman tree freeing function (from huffman_codes.c)
extern void free_huffman_tree(HuffmanNode* node);

// This function builds the decoding tree from the char-to-code map
// It's a bit tricky: for each code, you traverse/create nodes.
HuffmanNode* build_decoding_tree_from_map_file(const char* map_filename) {
    FILE* map_file = fopen(map_filename, "r");
    if (map_file == NULL) {
        perror("Error opening map file for decoding");
        return NULL;
    }

    HuffmanNode* root = create_huffman_node(-1, 0, NULL, NULL); // Root of the decoding tree
    char code_str[MAX_CODE_LENGTH]; // Buffer for reading code strings
    int ascii_val;

    while (fscanf(map_file, "%d %s", &ascii_val, code_str) == 2) {
        HuffmanNode* current_node = root;
        // Traverse the tree, creating nodes as needed
        for (int i = 0; code_str[i] != '\0'; i++) {
            if (code_str[i] == '0') {
                if (current_node->left == NULL) {
                    current_node->left = create_huffman_node(-1, 0, NULL, NULL);
                }
                current_node = current_node->left;
            } else if (code_str[i] == '1') {
                if (current_node->right == NULL) {
                    current_node->right = create_huffman_node(-1, 0, NULL, NULL);
                }
                current_node = current_node->right;
            } else {
                fprintf(stderr, "Error: Invalid character in Huffman code string in map file: %s\n", code_str);
                // Free partially built tree before returning NULL
                free_huffman_tree(root);
                fclose(map_file);
                return NULL;
            }
        }
        // At the end of the code string, we should be at a leaf node
        // (or an internal node that needs to become a leaf for this character)
        if (current_node->left != NULL || current_node->right != NULL) {
            fprintf(stderr, "Warning: Code for %d (%s) overlaps with another code path. This map is invalid for Huffman.\n", ascii_val, code_str);
            // This indicates a non-prefix code, which Huffman guarantees, so this would imply a bad map file.
        }
        current_node->ch = ascii_val; // Set the character for this leaf node
        // Frequency is not strictly needed for decoding, but set to 0 for consistency
        current_node->frequency = 0;
    }

    fclose(map_file);
    return root;
}

// Function to read and decode bits from the compressed file
void decode_and_write_file(const char* compressed_filename, const char* output_filename, HuffmanNode* decoding_tree_root) {
    FILE* compressed_file = fopen(compressed_filename, "rb"); // "rb" for binary read
    if (compressed_file == NULL) {
        perror("Error opening compressed file for decoding");
        return;
    }

    FILE* output_file = fopen(output_filename, "w"); // "w" for text write
    if (output_file == NULL) {
        perror("Error opening output file for decompressed data");
        fclose(compressed_file);
        return;
    }

    unsigned char byte_buffer;
    int bit_index = 0; // Current bit within the byte (0-7)
    HuffmanNode* current_decode_node = decoding_tree_root;
    int bytes_read;

    // Read the first byte
    bytes_read = fread(&byte_buffer, 1, 1, compressed_file);

    while (bytes_read > 0) {
        for (bit_index = 0; bit_index < 8; bit_index++) {
            // Get the current bit (MSB first: 7 - bit_index)
            int bit = (byte_buffer >> (7 - bit_index)) & 1;

            if (bit == 0) {
                current_decode_node = current_decode_node->left;
            } else { // bit == 1
                current_decode_node = current_decode_node->right;
            }

            // Check if we've reached a leaf node (a character)
            if (current_decode_node->left == NULL && current_decode_node->right == NULL) {
                fputc(current_decode_node->ch, output_file);
                current_decode_node = decoding_tree_root; // Reset to root for next character
            }
        }
        // Read the next byte
        bytes_read = fread(&byte_buffer, 1, 1, compressed_file);
    }

    // --- IMPORTANT NOTE ---
    // The current bit-reading loop assumes the compressed file ends exactly on a byte boundary
    // AND that there are no padding bits. In a real compressor, you'd need to store
    // the *actual* number of bits in the last byte (if it's not a full 8 bits) in the header.
    // For your visualizer, you might simplify or assume full bytes for now.

    fclose(compressed_file);
    fclose(output_file);
}