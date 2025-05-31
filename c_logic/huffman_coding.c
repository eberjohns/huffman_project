#include <stdio.h>
#include <stdlib.h>

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
    int index = 0;
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
    for (int i = 0; i <= largest; i++) {
        key[i] = NULL; // Initialize pointers to NULL
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

    // Free the linked lists
    printf("\nFreeing linked lists...\n");
    linked_list_free(key, largest);
    printf("Linked lists freed successfully.\n");

    return 0;
}
