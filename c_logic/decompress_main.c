#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman_node.h" // Needed for HuffmanNode struct
#include "encoder.h" // To use huffman_codes array or related functions
#include "decoder.h" // For new decoding functions

// Definition for freeing the Huffman tree
void free_huffman_tree(HuffmanNode* node) {
    if (node == NULL) return;
    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
    free(node);
}


int main(int argc, char *argv[]) {
    if (argc < 4) { // program_name, compressed_file, map_file, output_file
        fprintf(stderr, "Usage: %s <compressed_input_file> <map_file> <decompressed_output_file>\n", argv[0]);
        return 1;
    }

    const char *compressed_filename = argv[1];
    const char *map_filename = argv[2];
    const char *decompressed_filename = argv[3];

    printf("Compressed Input: %s\n", compressed_filename);
    printf("Map File: %s\n", map_filename);
    printf("Decompressed Output: %s\n", decompressed_filename);

    // --- DECODING PROCESS ---
    // 1. Read the map file and build the decoding tree
    HuffmanNode* decoding_tree_root = build_decoding_tree_from_map_file(map_filename);
    if (decoding_tree_root == NULL) {
        fprintf(stderr, "Error: Failed to build decoding tree from map file.\n");
        return 1;
    }

    // 2. Read compressed bits and decode
    decode_and_write_file(compressed_filename, decompressed_filename, decoding_tree_root);

    // 3. Cleanup
    free_huffman_tree(decoding_tree_root); // Assuming free_huffman_tree works on any HuffmanNode*

    printf("Decompression complete.\n");
    return 0;
}