#include "linked_list.h" // Include its own header

Node* create_node(int ch) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Error allocating memory for new node");
        exit(EXIT_FAILURE);
    }
    new_node->ch = ch;
    new_node->next = NULL; // Initialize next to NULL
    return new_node;
}

void linked_list_push(Node** head, int ch) {
    Node *new_node = create_node(ch);
    new_node->next = *head; // New node points to current head
    *head = new_node;       // New node becomes the new head
}

int linked_list_pop(Node** head) {
    if (*head == NULL) {
        return -1; // Indicate empty list or error
    }
    Node *temp = *head;
    int ch = temp->ch;
    *head = temp->next;
    free(temp);
    return ch;
}

void linked_list_free(Node** head_array, int largest_freq_index) { // Renamed largest to largest_freq_index for clarity
    for (int i = 0; i <= largest_freq_index; i++) {
        Node *current = head_array[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp);
        }
        head_array[i] = NULL; // Ensure the head pointer in the array is NULL
    }
}