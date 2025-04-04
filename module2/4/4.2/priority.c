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

void freeNode(Node* node) {
    if (node != NULL) {
        free(node);
    }
}

int main() {
    PriorityQueue queue;
    initQueue(&queue);

    enqueue(&queue, 10, "Message 1");
    enqueue(&queue, 200, "Message 2");
    enqueue(&queue, 50, "Message 3");
    enqueue(&queue, 150, "Message 4");
    enqueue(&queue, 255, "Message 5");

    Node* element = dequeue(&queue);
    if (element != NULL) {
        printf("Most priority: priority = %d, message = %s\n", element->priority, element->message);
        freeNode(element);
    }

    element = dequeueWithPriority(&queue, 50);
    if (element != NULL) {
        printf("Extract priority 50: priority = %d, message = %s\n", element->priority, element->message);
        freeNode(element);
    }
    element = dequeueWithMinPriority(&queue, 240);
    if (element != NULL) {
        printf("Extract priority no less than 240: priority = %d, message = %s\n", element->priority, element->message);
        freeNode(element);
    }
}
