#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIORITY 255

typedef struct Node {
    int priority;
    char message[256];
    struct Node* next;
} Node;

// Структура очереди
typedef struct {
    Node* head;
} PriorityQueue;

// Инициализация очереди
void initQueue(PriorityQueue *queue) {
    queue->head = NULL;
}

// Добавление элемента в очередь
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

// Извлечение элемента с наивысшим приоритетом
Node* dequeue(PriorityQueue *queue) {
    if (queue->head == NULL) {
        printf("Очередь пуста!\n");
        return NULL;
    }
    Node* temp = queue->head;
    queue->head = queue->head->next;
    return temp;
}

// Извлечение элемента с указанным приоритетом
Node* dequeueWithPriority(PriorityQueue *queue, int priority) {
    Node* current = queue->head;
    Node* prev = NULL;

    while (current != NULL && current->priority != priority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Элемент с приоритетом %d не найден!\n", priority);
        return NULL;
    }

    if (prev == NULL) {
        queue->head = current->next;
    } else {
        prev->next = current->next;
    }

    return current;
}

// Извлечение элемента с приоритетом не ниже заданного
Node* dequeueWithMinPriority(PriorityQueue *queue, int minPriority) {
    Node* current = queue->head;
    Node* prev = NULL;

    while (current != NULL && current->priority > minPriority) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Элемент с приоритетом не ниже %d не найден!\n", minPriority);
        return NULL;
    }

    if (prev == NULL) {
        queue->head = current->next;
    } else {
        prev->next = current->next;
    }

    return current;
}

// Освобождение памяти узла
void freeNode(Node* node) {
    if (node != NULL) {
        free(node);
    }
}

int main() {
    PriorityQueue queue;
    initQueue(&queue);

    // Добавление элементов в очередь
    enqueue(&queue, 10, "Message 1");
    enqueue(&queue, 200, "Message 2");
    enqueue(&queue, 50, "Message 3");
    enqueue(&queue, 150, "Message 4");
    enqueue(&queue, 255, "Message 5");

    Node* element = dequeue(&queue);
    if (element != NULL) {
        printf("Извлечен наиболее приоритетный элемент: Приоритет = %d, Сообщение = %s\n", element->priority, element->message);
        freeNode(element);
    }

    element = dequeueWithPriority(&queue, 50);
    if (element != NULL) {
        printf("Извлечен элемент с приоритетом 50: Приоритет = %d, Сообщение = %s\n", element->priority, element->message);
        freeNode(element);
    }
    element = dequeueWithMinPriority(&queue, 240);
    if (element != NULL) {
        printf("Извлечен первый элемент с приоритетом не меньше чем 240: Приоритет = %d, Сообщение = %s\n", element->priority, element->message);
        freeNode(element);
    }
}
