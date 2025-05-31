#include <stdio.h>
#include <stdlib.h> // For memory allocation, file handling, etc.

typedef struct Node {
    int ch; // Character data
    struct Node *next; // Pointer to the next node
} Node;

Node* create_node(int ch) {
    Node *new_node = (Node *)malloc(sizeof(Node)); // Allocate memory for a new node
    if (new_node == NULL) {
        perror("Error allocating memory for new node");
        exit(EXIT_FAILURE);
    }
    new_node->ch = ch; // Set character data
    return new_node;
}

// Function to add a character to the linked list
void linked_list_push(Node** head, int ch) {
    Node *new_node = create_node(ch); // Create a new node with the character
    if (*head == NULL) {
        *head = new_node; // If no node exists, set it as the first node
        new_node->next = NULL; // Set next pointer to NULL
    } else {
        Node *temp = *head;
        *head = new_node; // Set the new node as the head
        new_node->next = temp; // Point the new node's next to the previous head
    }
}

// Function to return first character from the linked list
int linked_list_pop(Node** head) {
    if (*head == NULL) {
        return -1; // Return -1 if the linked list is empty
    }
    Node *temp = *head; // Store the current head node
    int ch = temp->ch; // Get the character from the head node
    *head = temp->next; // Move the head pointer to the next node
    free(temp); // Free the memory of the old head node
    return ch; // Return the character
}

// Function to free the linked list
void linked_list_free(Node** head, int largest) {
    for (int i = 0; i <= largest; i++) {
        Node *current = head[i];
        while (current != NULL) {
            Node *temp = current; // Store the current node
            current = current->next; // Move to the next node
            free(temp); // Free the memory of the current node
        }
        head[i] = NULL; // Set the head pointer to NULL after freeing
    }
}

typedef struct HeapNode {
    int ch; // Character data (or a special value for internal nodes)
    int f;
    HeapNode *Left;
    HeapNode *Right;
} HeapNode;

HeapNode* create_heap_node(Node** head, int f,int character_node) {
    HeapNode *new_node = (HeapNode *)malloc(sizeof(HeapNode)); // Allocate memory for a new heap node
    if (new_node == NULL) {
        perror("Error allocating memory for new heap node");
        exit(EXIT_FAILURE);
    }
    if(character_node==0)
    new_node->ch = -1;
    else{
        new_node->ch = linked_list_pop(&head[f]); // Use a function to get the character data from key list
        // if linked list is null then assigns -1 because its an internal node
    }
    new_node->f = f; // Set character data
    new_node->Left = NULL; // Initialize left child to NULL
    new_node->Right = NULL; // Initialize right child to NULL
    return new_node;
}

// Function to insert a heap node into the heap
// void insert_heap_node(HeapNode **heap, HeapNode *new_node) {
//     if (*heap == NULL) {
//         *heap = new_node; // If heap is empty, set the new node as the root
//     } else {
//         // Insert the new node into the heap (this is a simplified version, actual heap insertion logic may vary)
//         HeapNode *current = *heap;
//         while (current->Left != NULL && current->Right != NULL && current->ch != -1) {
//             if (current->Left->f < current->Right->f) { // <= can use
//                 current = current->Left; // Move to the left child
//             } else {
//                 current = current->Right; // Move to the right child
//             }
//         }
//         if(current->f < new_node->f) {
//             *heap = create_heap_node(NULL, new_node->f+current->f, 0); // Wraps to create a new heap node with the sum of frequencies
//             (*heap)->Left = new_node; // Set the new_node as the left child
//             (*heap)->Right = current; // Set the current node as the right child

//         }
//         else {
//             *heap = create_heap_node(NULL, new_node->f+current->f, 0); // Wraps to create a new heap node with the sum of frequencies
//             (*heap)->Left = current; // Set the current node as the left child
//             (*heap)->Right = new_node; // Set the new_node as the right child
//         }
//     }
// }

// Function to extract the minimum element from the heap
int heap_extract(int *frequency_characters) {
    if (frequency_characters == NULL || frequency_characters[0] == 0) {
        return -1; // Return -1 if the heap is empty
    }
    int min = frequency_characters[1]; // Get the minimum element (root of the heap)
    frequency_characters[1] = frequency_characters[frequency_characters[0]]; // Replace root with the last element and decrease size
    frequency_characters[0]--; // Decrease the size of the heap
    sift_down(frequency_characters, 0); // Restore the heap property by sifting down the new root
    return min; // Return the extracted minimum element
}

// Function to sift down the heap to maintain the heap property
void sift_down(int *frequency_characters, int index) {
    int size = frequency_characters[0]; // Get the size of the heap
    int left = 2 * index + 1; // Left child index
    int right = 2 * index + 2; // Right child index
    int smallest = index; // Assume the current index is the smallest

    if (left < size && frequency_characters[left] < frequency_characters[smallest]) {
        smallest = left; // Update smallest if left child is smaller
    }
    if (right < size && frequency_characters[right] < frequency_characters[smallest]) {
        smallest = right; // Update smallest if right child is smaller
    }
    if (smallest != index) {
        // Swap the current node with the smallest node
        int temp = frequency_characters[index];
        frequency_characters[index] = frequency_characters[smallest];
        frequency_characters[smallest] = temp;
        sift_down(frequency_characters, smallest); // Recursively sift down the affected subtree
    }
}

void heap_insert(int *frequency_characters, int value) {
    if (frequency_characters == NULL) {
        fprintf(stderr, "Error: Frequency characters array is NULL.\n");
        return; // Exit if the frequency characters array is NULL
    }
    frequency_characters[0]++; // Increase the size of the heap
    frequency_characters[frequency_characters[0]] = value; // Insert the new value at the end of the array
    sift_up(frequency_characters, frequency_characters[0]); // Restore the heap property by sifting up
}

void sift_up(int *frequency_characters, int index) {
    if (index <= 0 || frequency_characters == NULL) {
        return; // Exit if index is invalid or frequency characters array is NULL
    }
    int parent = (index - 1) / 2; // Calculate the parent index
    if (frequency_characters[index] < frequency_characters[parent]) {
        // Swap the current node with its parent
        int temp = frequency_characters[index];
        frequency_characters[index] = frequency_characters[parent];
        frequency_characters[parent] = temp;
        sift_up(frequency_characters, parent); // Recursively sift up the affected subtree
    }
}

//argc = argument count
//argv = argument vector (array of strings)
int main(int argc, char *argv[]) {
    // Check if the user provided an argument
    // printf("Number of arguments: %d\n", argc);
    // for(int i = 0; i < argc; i++) {
    //     printf("Argument %d: %s\n", i, argv[i]);
    // }

    //Set filename to the first argument or a default value
    const char *filename = (argc > 1) ? argv[1] : "default.txt";
    printf("Filename: %s\n", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int character;// Use int to handle EOF correctly
    int frequency_table[128][2]; // Frequency table for ASCII characters
    // Initialize frequency table
    for (int i = 0; i < 128; i++) {
        frequency_table[i][0] = i; // ASCII value
        frequency_table[i][1] = 0; // Frequency count
    }    
    // int index = 0;
    int largest = 0;// Variable to store the largest frequency count
    printf("Reading file contents:\n");
    while ((character = fgetc(file)) != EOF) {// Read each character from the file until EOF
        putchar(character);// Print each character to the console
        // Update frequency table
        if (character >= 0 && character < 128) {
            frequency_table[character][1]++; // Increment frequency count for the character
            if (frequency_table[character][1] > largest) {
                largest = frequency_table[character][1]; // Update largest frequency count
            }
        }
    }

    fclose(file);
    printf("\nFile closed successfully.\n");

    // Print frequency table
    printf("\nCharacter Frequency Table:\n");
    for (int i = 0; i < 128; i++) {
        if (frequency_table[i][1] > 0) { // Only print characters that appeared in the file
            printf("%c\t\t%d\t\t%d\n", frequency_table[i][0], frequency_table[i][0], frequency_table[i][1]);// Print ASCII value and frequency count
        }
    }

    printf("\nLargest frequency count: %d\n", largest);

    Node* key[largest + 1]; // Array of pointers to store keys
    int key_frequency[largest + 1]; // Array to store frequency counts for each key
    for (int i = 0; i <= largest; i++) {
        key[i] = NULL; // Initialize pointers to NULL
        key_frequency[i] = 0; // Initialize frequency counts to 0
    }

    // Example of how to use the linked list
    // printf("address of key[0]: %d\n", &key[0]);
    // printf("value of key[0]: %d\n", key[0]);
    // linked_list_push(&key[0],65); // Example of adding a character to the linked list at index 0
    // printf("address of key[0]: %d\n", &key[0]);
    // printf("value of key[0]: %d\n", key[0]);
    // printf("address of key[1]: %d\n", &key[1]);
    // printf("value of key[1]: %d\n", key[1]);
    
    //Populate the key array with pointers to characters based on frequency
    for (int i = 0; i < 128; i++) {
        int frequency = frequency_table[i][1];
        if (frequency > 0) { // Only consider characters that appeared in the file
            // frequency_table[i][0] =  character's ASCII value
            // key[frequency] = pointer to the linked list of characters with that frequency
            linked_list_push(&key[frequency],frequency_table[i][0]); // Add character to linked list at index of its frequency
            // linked_list_push(): adds a character to a linked list, 
            key_frequency[frequency]++; // Increment the frequency count for this key
        }
    }

    // Print the linked lists for each frequency
    printf("\nLinked Lists of Characters by Frequency:\n");
    for (int i = 0; i <= largest; i++) {
        if (key[i] != NULL) { // Only print non-empty linked lists
            printf("Frequency %d: ", i);
            Node *current = key[i]; // Start from the head of the linked list
            while (current != NULL) {
                printf("%c ", current->ch); // Print character in the linked list
                current = current->next; // Move to the next node
            }
            printf("\n");
        }
    }

    printf("\nFrequency Counts for Each Key:\n");
    for (int i = 0; i <= largest; i++) {
        if (key_frequency[i] > 0) { // Only print keys with non-zero frequency counts
            printf("Frequency %d: %d\n", i, key_frequency[i]); // Print frequency and count
        }
    }

    int unique_characters = 0; // Variable to count unique characters
    for (int i = 0; i < 128; i++) {
        if (frequency_table[i][1] > 0) { // Count only characters that appeared in the file
            unique_characters++;
        }
    }

    printf("\nTotal unique characters: %d\n", unique_characters);

    int frequency_characters[unique_characters+1];
    frequency_characters[0] = unique_characters; // Store the size of frequency_characters at index 0
    int index=1;// Index for frequency characters array
    for(int i=1;i<=largest;i++){
        if(key_frequency[i] > 0) {
            for(int j=0;j<key_frequency[i];j++) {
                frequency_characters[index] = i; // Store frequency counts in an array
                index++; // Increment index for the next frequency character
            }
        }
    }

    printf("\nFrequency Characters:\n");
    for(int i = 0; i <= unique_characters; i++) {
        if (i == 0) {
            printf("Size: %d :", frequency_characters[i]); // Print size of frequency characters
        }
        printf("%d ", frequency_characters[i]); // Print frequency characters
    }

    // Free the linked lists
    printf("\nFreeing linked lists...\n");
    linked_list_free(key, largest);
    printf("Linked lists freed successfully.\n");

    // we heapify the frequency_characters array
    // Parent = i
    // Left Child = 2*i + 1
    // Right Child = 2*i + 2

    // Create a heap structure to store the frequencies

    HeapNode *heap = NULL; // Initialize heap to NULL

    int iterations = unique_characters; // Number of iterations to heapify
    while(iterations>1){
        int left = heap_extract(frequency_characters);
        int right = heap_extract(frequency_characters);
        if(left == -1 || right == -1) {
            fprintf(stderr, "Error: Not enough elements in the heap to extract.\n");
            return 1; // Exit if there are not enough elements to extract
        }
        heap_insert(frequency_characters, left + right); // Insert the sum of the two smallest frequencies back into the heap
        // Create a new heap node for the left and right children
        HeapNode* Parent = create_heap_node(key,left + right,0); // Create a new heap node for the sum of the two smallest frequencies
        Parent->Left = create_heap_node(key,left,1); // Create left child node
        Parent->Right = create_heap_node(key,right,1); // Create right child node
        insert_heap_node(&heap, Parent); // Insert the new heap node into the heap
        iterations--; // Decrease the count of iterations
    }

    return 0;
}
