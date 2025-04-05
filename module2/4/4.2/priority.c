#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIORITY 255

typedef struct Node {
    int priority;
    char message[256];
    struct Node* next;
} Node;

typedef struct {
    Node* head;
} PriorityQueue;

void initQueue(PriorityQueue *queue) {
    queue->head = NULL;
}

void enqueue(PriorityQueue *queue, int priority, const char *message) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->priority = priority;
    strncpy(newNode->message, message, sizeof(newNode->message) - 1);
    newNode->message[sizeof(newNode->message) - 1] = '\0';
    newNode->next = NULL;

    if (queue->head == NULL || queue->head->priority > priority) {
        newNode->next = queue->head;
        queue->head = newNode;
    } else {
        Node* current = queue->head;
        while (current->next != NULL && current->next->priority <= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

Node* dequeue(PriorityQueue *queue) {
    if (queue->head == NULL) {
        printf("Empty!\n");
        return NULL;
    }
    Node* temp = queue->head;
    queue->head = queue->head->next;
    return temp;
}

Node* dequeueWithPriority(PriorityQueue *queue, int priority) {
    Node* current = queue->head;
    Node* prev = NULL;

    while (current != NULL && current->priority != priority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Element with priority %d not found!\n", priority);
        return NULL;
    }

    if (prev == NULL) {
        queue->head = current->next;
    } else {
        prev->next = current->next;
    }

    return current;
}

Node* dequeueWithMinPriority(PriorityQueue *queue, int minPriority) {
    Node* current = queue->head;
    Node* prev = NULL;

    while (current != NULL && current->priority > minPriority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Element with priority no less than %d not found!\n", minPriority);
        return NULL;
    }

    if (prev == NULL) {
        queue->head = current->next;
    } else {
        prev->next = current->next;
    }

    return current;
}

void printQueue(PriorityQueue *queue) {
    printf("Current Queue:\n");
    Node* current = queue->head;
    while (current != NULL) {
        printf("Priority: %d, Message: %s\n", current->priority, current->message);
        current = current->next;
    }
}

void freeNode(Node* node) {
    if (node != NULL) {
        free(node);
    }
}

void displayMenu() {
    printf("\nPriority Queue Menu\n");
    printf("1. Add new message\n");
    printf("2. Extract highest priority message\n");
    printf("3. Extract message with specific priority\n");
    printf("4. Extract message with minimum priority\n");
    printf("5. View all messages\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    PriorityQueue queue;
    initQueue(&queue);
    enqueue(&queue, 10, "Message 1");
    enqueue(&queue, 200, "Message 2");
    enqueue(&queue, 50, "Message 3");
    enqueue(&queue, 150, "Message 4");
    enqueue(&queue, 255, "Message 5");
    int choice;
    int priority;
    char message[256];
    Node* element;

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer

        switch (choice) {
            case 1:
                printf("Enter priority (0-255): ");
                scanf("%d", &priority);
                getchar();
                printf("Enter message: ");
                fgets(message, sizeof(message), stdin);
                message[strcspn(message, "\n")] = '\0'; // Remove trailing newline
                enqueue(&queue, priority, message);
                printf("Message added successfully!\n");
                break;

            case 2:
                element = dequeue(&queue);
                if (element != NULL) {
                    printf("Extracted message: Priority %d - '%s'\n", 
                           element->priority, element->message);
                    freeNode(element);
                }
                break;

            case 3:
                printf("Enter priority to extract: ");
                scanf("%d", &priority);
                element = dequeueWithPriority(&queue, priority);
                if (element != NULL) {
                    printf("Extracted message: Priority %d - '%s'\n", 
                           element->priority, element->message);
                    freeNode(element);
                }
                break;

            case 4:
                printf("Enter minimum priority: ");
                scanf("%d", &priority);
                element = dequeueWithMinPriority(&queue, priority);
                if (element != NULL) {
                    printf("Extracted message: Priority %d - '%s'\n", 
                           element->priority, element->message);
                    freeNode(element);
                }
                break;

            case 5:
                printQueue(&queue);
                break;

            case 0:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 0);

    // Free any remaining nodes
    while (element = dequeue(&queue)) {
        freeNode(element);
    }
}