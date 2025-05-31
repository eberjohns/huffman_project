#ifndef DECODER_H
#define DECODER_H

#include "huffman_node.h" // Assumes HuffmanNode is defined here
#include "encoder.h" // To access MAX_CODE_LENGTH if needed

// Function to build a Huffman tree for decoding from the character-to-code map
HuffmanNode* build_decoding_tree_from_map_file(const char* map_filename);

// Function to read compressed bits and decode
void decode_and_write_file(const char* compressed_filename, const char* output_filename, HuffmanNode* decoding_tree_root);

#endif // DECODER_H