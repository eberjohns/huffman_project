// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strlen, etc. if needed later
#include <math.h> // For round() to round percentage


#include "huffman_node.h"         // Defines HuffmanNode struct (indirectly used by PQ and codes)
#include "min_priority_queue.h"   // PQ functions
#include "encoder.h"        // Code generation functions
#include "linked_list.h"          // Linked list functions

HuffmanNode* create_huffman_node(int ch, int freq, HuffmanNode* left, HuffmanNode* right) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    if (node == NULL) { perror("Failed to allocate HuffmanNode"); exit(EXIT_FAILURE); }
    node->ch = ch;
    node->frequency = freq; // Use 'frequency' instead of 'f' for consistency
    node->left = left;
    node->right = right;
    return node;
}

long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb"); // Open in binary read mode
    if (file == NULL) {
        perror("Error opening file to get size");
        return -1; // Indicate error
    }

    fseek(file, 0, SEEK_END); // Go to the end of the file
    long size = ftell(file);  // Get the current position (which is the size)
    fclose(file);             // Close the file

    return size;
}

int main(int argc, char *argv[]) {
    // Check if enough arguments are provided for COMPRESSION
    // Now expecting: program_name, input_file, output_compressed_file, output_map_file
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <input_text_file> <output_compressed_file> <output_map_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *output_compressed_filename = argv[2]; // This will store the actual compressed bits
    const char *output_map_filename = argv[3];        // This will store the char-to-code map

    printf("Input Filename: %s\n", filename);
    printf("Compressed Output Filename: %s\n", output_compressed_filename);
    printf("Map Output Filename: %s\n", output_map_filename);
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int character;
    int frequency_table[128][2];
    for (int i = 0; i < 128; i++) {
        frequency_table[i][0] = i;
        frequency_table[i][1] = 0;
    }
    int largest_freq_val = 0; // Renamed 'largest' for clarity, this is the max frequency value

    printf("Reading file contents:\n");
    while ((character = fgetc(file)) != EOF) {
        //putchar(character);
        if (character >= 0 && character < 128) {
            frequency_table[character][1]++;
            if (frequency_table[character][1] > largest_freq_val) {
                largest_freq_val = frequency_table[character][1];
            }
        }
    }
    fclose(file);
    printf("\nFile closed successfully.\n");

    printf("\nCharacter Frequency Table:\n");
    for (int i = 0; i < 128; i++) {
        if (frequency_table[i][1] > 0) {
            printf("'%c'\t\t%d\t\t%d\n", frequency_table[i][0], frequency_table[i][0], frequency_table[i][1]);
        }
    }
    printf("\nLargest frequency count: %d\n", largest_freq_val);

    // Linked list setup
    Node* key[largest_freq_val + 1]; // Array of linked list heads
    for (int i = 0; i <= largest_freq_val; i++) {
        key[i] = NULL;
    }

    int unique_characters = 0;
    for (int i = 0; i < 128; i++) {
        int frequency = frequency_table[i][1];
        if (frequency > 0) {
            linked_list_push(&key[frequency], frequency_table[i][0]);
            unique_characters++; // Count unique characters here
        }
    }
    printf("\nTotal unique characters: %d\n", unique_characters);

    // Print linked lists (optional, for debugging)
    printf("\nLinked Lists of Characters by Frequency (for debugging):\n");
    for (int i = 0; i <= largest_freq_val; i++) {
        if (key[i] != NULL) {
            printf("Frequency %d: ", i);
            Node *current = key[i];
            while (current != NULL) {
                printf("'%c' ", current->ch);
                current = current->next;
            }
            printf("\n");
        }
    }


    // --- Huffman Tree Building ---
    MinPriorityQueue* pq = NULL;
    HuffmanNode* huffman_root = NULL;

    if (unique_characters == 0) {
        printf("No characters found in file. Huffman tree cannot be built.\n");
        // Free linked lists (if not already handled)
        linked_list_free(key, largest_freq_val);
        return 0; // Exit gracefully
    }

    // Initialize the Min-Priority Queue for HuffmanNodes
    // Capacity should be at least unique_characters, it will resize if needed
    pq = create_min_pq(unique_characters + 1); // +1 just for a small buffer

    // Populate PQ with initial leaf HuffmanNodes
    // Iterate through frequencies from 1 up to largest_freq_val
    for (int freq = 1; freq <= largest_freq_val; freq++) {
        // For each frequency, extract characters from the linked list at that 'key' index
        while (!is_empty_pq(pq) || key[freq] != NULL) { // Need a way to check if linked list at key[freq] is empty
            if (key[freq] != NULL) {
                int ch = linked_list_pop(&key[freq]);
                if (ch != -1) { // -1 means list was empty
                    HuffmanNode* leaf_node = create_huffman_node(ch, freq, NULL, NULL);
                    insert_pq(pq, leaf_node);
                } else {
                    // This specific linked list is empty, break and move to next frequency
                    break;
                }
            } else {
                // If key[freq] is NULL, it means no characters had this frequency.
                // Move to the next frequency.
                break;
            }
        }
    }

    // A crucial step that was missing: after all initial leaf nodes are inserted,
    // the PQ needs to be ready. The loop above isn't quite right for this.
    // Let's re-do the PQ population to be simpler and more standard:

    // Repopulate PQ (correct approach)
    // First, free the old PQ if it was created and populated incorrectly
    if (pq != NULL) {
        // You'll need to decide how to free nodes if they are still in PQ and not extracted.
        // For this example, let's assume `create_min_pq` is called once and is empty before this loop.
        free_min_pq(pq);
        pq = create_min_pq(unique_characters + 1);
    }


    for (int i = 0; i < 128; i++) {
        if (frequency_table[i][1] > 0) {
            HuffmanNode* leaf_node = create_huffman_node(frequency_table[i][0], frequency_table[i][1], NULL, NULL);
            insert_pq(pq, leaf_node);
        }
    }

    // Build the Huffman Tree
    while (pq->size > 1) { // Continue until only one node (the root) remains
        HuffmanNode* left_child = extract_min_pq(pq);
        HuffmanNode* right_child = extract_min_pq(pq);

        if (left_child == NULL || right_child == NULL) {
            fprintf(stderr, "Error: Not enough nodes in PQ to build tree. This shouldn't happen with correct logic.\n");
            // Free any remaining nodes in PQ if you exit early
            while(!is_empty_pq(pq)) {
                free_huffman_tree(extract_min_pq(pq));
            }
            free_min_pq(pq);
            linked_list_free(key, largest_freq_val);
            return 1;
        }

        // Create a new internal parent node
        HuffmanNode* parent_node = create_huffman_node(-1, left_child->frequency + right_child->frequency, left_child, right_child);
        insert_pq(pq, parent_node);
    }

    // The last node in the PQ is the root of the Huffman tree
    if (pq->size == 1) {
        huffman_root = extract_min_pq(pq);
    } else if (unique_characters == 1) {
        // If unique_characters is 1, the while loop (pq->size > 1) never ran.
        // The one node inserted initially is the root.
        // Need to extract it if it's still in the PQ.
        huffman_root = extract_min_pq(pq);
    } else if (unique_characters > 1 && pq->size == 0) {
        // This indicates an error where all nodes were processed but no root was formed.
        fprintf(stderr, "Error: Huffman tree construction failed. PQ is empty but multiple unique characters were present.\n");
        huffman_root = NULL; // Explicitly set to NULL to avoid issues
    } else { // Should not happen
        fprintf(stderr, "Error: Unexpected PQ size after Huffman tree construction: %d\n", pq->size);
        huffman_root = NULL; // Explicitly set to NULL
    }

    if (huffman_root != NULL) {
        // 1. Build Huffman Codes (this populates the huffman_codes global array)
        build_huffman_codes(huffman_root);
        print_huffman_codes(); // Optional: Print to console

        // 2. Write the character-to-code map to the specified file
        write_huffman_map_to_file(output_map_filename);

        // 3. Encode the input file and write compressed bits to the specified output file
        encode_and_write_file(filename, output_compressed_filename);
        
        // --- NEW: Display Compression Statistics ---
        printf("\n--- Compression Statistics ---\n");

        long size_before_compression = get_file_size(filename);
        long size_compressed_data = get_file_size(output_compressed_filename);
        long size_map_file = get_file_size(output_map_filename);

        if (size_before_compression != -1 && size_compressed_data != -1 && size_map_file != -1) {
            long size_after_compression = size_compressed_data + size_map_file;

            printf("Original File: %s (Size: %ld bytes)\n", filename, size_before_compression);
            printf("Compressed Data File: %s (Size: %ld bytes)\n", output_compressed_filename, size_compressed_data);
            printf("Map File: %s (Size: %ld bytes)\n", output_map_filename, size_map_file);
            printf("Total Compressed Size (Data + Map): %ld bytes\n", size_after_compression);

            if (size_before_compression > 0) {
                double compression_ratio = (double)size_after_compression / size_before_compression;
                double percentage_reduction = (1.0 - compression_ratio) * 100.0;
                double percentage_of_original = compression_ratio * 100.0;

                printf("Compression Ratio: %.2f%%\n", percentage_of_original); // Size after / Size before * 100
                printf("Space Saved: %.2f%%\n", percentage_reduction); // (1 - Ratio) * 100
            } else {
                printf("Cannot calculate percentage for an empty input file.\n");
            }
        } else {
            fprintf(stderr, "Could not retrieve all file sizes for compression statistics.\n");
        }
        printf("------------------------------\n");

    } else {
        printf("Failed to build Huffman tree. Codes cannot be generated, and file cannot be compressed.\n");
    }

    // Free the PQ structure itself
    free_min_pq(pq);

    // Free the linked lists that held character frequencies
    printf("\nFreeing linked lists...\n");
    linked_list_free(key, largest_freq_val);
    printf("Linked lists freed successfully.\n");

    // Free the entire Huffman tree to prevent memory leaks
    printf("Freeing Huffman tree...\n");
    free_huffman_tree(huffman_root);
    printf("Huffman tree freed successfully.\n");

    return 0;
}