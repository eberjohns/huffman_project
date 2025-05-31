#include "encoder.h" // Includes function prototypes and huffman_node.h
#include <stdio.h>         // For printf, fprintf

void write_huffman_map_to_file(const char* map_filename) {
    FILE* map_file = fopen(map_filename, "w"); // "w" for text write
    if (map_file == NULL) {
        perror("Error opening map file for writing");
        return;
    }

    printf("\nWriting Huffman map to %s...\n", map_filename);
    for (int i = 0; i < 128; i++) {
        if (huffman_codes[i][0] != '\0') { // If a code exists for this character
            fprintf(map_file, "%d %s\n", i, huffman_codes[i]);
        }
    }
    fclose(map_file);
    printf("Huffman map written successfully.\n");
}

// Define the global array for Huffman codes
char huffman_codes[128][MAX_CODE_LENGTH];

void init_huffman_codes_array() {
    for (int i = 0; i < 128; i++) {
        huffman_codes[i][0] = '\0'; // Initialize all codes to empty strings
    }
}

// Recursive DFS function to generate codes
static void generate_codes_dfs(HuffmanNode* root, char* current_code, int top) {
    // Base Case: Leaf Node
    if (root->left == NULL && root->right == NULL) {
        current_code[top] = '\0'; // Null-terminate the current code string
        if (root->ch >= 0 && root->ch < 128) {
            strcpy(huffman_codes[root->ch], current_code);
        } else {
            fprintf(stderr, "Error: Invalid character ASCII value in leaf node: %d\n", root->ch);
        }
        return;
    }

    // Recursive Step: Internal Node
    if (root->left) {
        current_code[top] = '0';
        generate_codes_dfs(root->left, current_code, top + 1);
    }
    if (root->right) {
        current_code[top] = '1';
        generate_codes_dfs(root->right, current_code, top + 1);
    }
}

void build_huffman_codes(HuffmanNode* root) {
    char current_code_buffer[MAX_CODE_LENGTH];

    init_huffman_codes_array(); // Always initialize before building

    if (root == NULL) {
        printf("Error: Huffman tree is empty, cannot generate codes.\n");
        return;
    }

    // Special case: only one unique character in the text
    if (root->left == NULL && root->right == NULL) {
        if (root->ch >= 0 && root->ch < 128) {
            strcpy(huffman_codes[root->ch], "0"); // Assign '0' as its code
            printf("Special case: Only one unique character '%c' (ASCII %d), assigned code '0'.\n", root->ch, root->ch);
        } else {
             fprintf(stderr, "Error: Single node tree has invalid character: %d\n", root->ch);
        }
        return;
    }

    generate_codes_dfs(root, current_code_buffer, 0);
}

void print_huffman_codes() {
    printf("\n--- Huffman Codes Generated ---\n");
    printf("Char\tASCII\tCode\n");
    printf("----\t-----\t----\n");
    for (int i = 0; i < 128; i++) {
        if (huffman_codes[i][0] != '\0') {
            if (i >= 32 && i <= 126) { // Printable ASCII
                printf("'%c'\t%d\t%s\n", (char)i, i, huffman_codes[i]);
            } else { // Non-printable
                if (i == 10) printf("'\\n'\t%d\t%s\n", i, huffman_codes[i]);
                else if (i == 32) printf("' '\t%d\t%s\n", i, huffman_codes[i]);
                else if (i == 9) printf("'\\t'\t%d\t%s\n", i, huffman_codes[i]);
                else if (i == 0) printf("'\\0'\t%d\t%s\n", i, huffman_codes[i]); // Null char
                else printf("0x%02X\t%d\t%s\n", i, i, huffman_codes[i]);
            }
        }
    }
    printf("-------------------------------\n");
}

// Definition for freeing the Huffman tree
void free_huffman_tree(HuffmanNode* node) {
    if (node == NULL) return;
    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
    free(node);
}

// Add this function, perhaps in huffman_codes.c or a new compression.c
#include <stdio.h> // For FILE operations
#include <string.h> // For strlen

// Function to write a bit to the output file
// This is a lower-level helper for bit-packing
static void write_bit(FILE *outfile, int bit, unsigned char *buffer, int *buffer_pos) {
    if (*buffer_pos == 0) {
        *buffer = 0; // Initialize buffer to all zeros when starting a new byte
    }

    // Set the bit at the current position in the buffer
    if (bit == 1) {
        *buffer |= (1 << (7 - *buffer_pos)); // Set the bit. (7 - buffer_pos) for MSB first
    }

    (*buffer_pos)++; // Move to the next bit position

    // If the buffer is full (8 bits), write the byte to the file
    if (*buffer_pos == 8) {
        fputc(*buffer, outfile);
        *buffer_pos = 0; // Reset for the next byte
        *buffer = 0;     // Clear buffer
    }
}

// Function to write the encoded data to the output file
void encode_and_write_file(const char *input_filename, const char *output_filename) {
    FILE *infile = fopen(input_filename, "r");
    if (infile == NULL) {
        perror("Error opening input file for encoding");
        return;
    }

    // Open the output file in binary write mode
    FILE *outfile = fopen(output_filename, "wb"); // "wb" for binary write
    if (outfile == NULL) {
        perror("Error opening output file for writing compressed data");
        fclose(infile);
        return;
    }

    unsigned char byte_buffer = 0; // Buffer to accumulate bits (1 byte = 8 bits)
    int bit_position = 0;         // Current bit position within the byte_buffer (0-7)

    int character;
    printf("\nEncoding and writing compressed data...\n");
    while ((character = fgetc(infile)) != EOF) {
        if (character >= 0 && character < 128) {
            const char *code = huffman_codes[character]; // Get the Huffman code for the character
            if (code[0] == '\0') { // Character not found in codes (shouldn't happen if frequency table is correct)
                fprintf(stderr, "Warning: No Huffman code found for character '%c' (ASCII %d)\n", (char)character, character);
                continue;
            }

            // Write each bit of the code to the output file
            for (int i = 0; code[i] != '\0'; i++) {
                int bit = code[i] - '0'; // Convert '0' or '1' char to integer 0 or 1
                write_bit(outfile, bit, &byte_buffer, &bit_position);
            }
        } else {
            fprintf(stderr, "Warning: Non-ASCII character (value %d) encountered, skipping.\n", character);
        }
    }

    // After writing all characters, flush any remaining bits in the buffer
    // This is crucial if the total number of bits is not a multiple of 8.
    if (bit_position > 0) {
        fputc(byte_buffer, outfile);
        // You might need to store the number of valid bits in this last byte
        // in the header of the compressed file for decompression.
    }

    fclose(infile);
    fclose(outfile);
    printf("Compression complete. Compressed data written to %s\n", output_filename);
}
