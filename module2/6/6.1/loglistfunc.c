#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "loglist.h"

void initList(DoublyLinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    Person initialContacts[10] = {
        {0, "LastName10", "FirstName0", "MiddleName0", "Company0", {"0@gmail.com", "0_tg", "0_inst"}},
        {1, "LastName88", "FirstName1", "MiddleName1", "Company1", {"1@gmail.com", "1_tg", "1_inst"}},
        {2, "LastName15", "FirstName2", "MiddleName2", "Company2", {"2@gmail.com", "2_tg", "2_inst"}},
        {3, "LastName55", "FirstName3", "MiddleName3", "Company3", {"3@gmail.com", "3_tg", "3_inst"}},
        {4, "LastName76", "FirstName4", "MiddleName4", "Company4", {"4@gmail.com", "4_tg", "4_inst"}},
        {5, "LastName44", "FirstName5", "MiddleName5", "Company5", {"5@gmail.com", "5_tg", "5_inst"}},
        {6, "LastName99", "FirstName6", "MiddleName6", "Company6", {"6@gmail.com", "6_tg", "6_inst"}},
        {7, "LastName11", "FirstName7", "MiddleName7", "Company7", {"7@gmail.com", "7_tg", "7_inst"}},
        {8, "LastName11", "FirstName8", "MiddleName8", "Company8", {"8@gmail.com", "8_tg", "8_inst"}},
        {9, "LastName70", "FirstName9", "MiddleName9", "Company9", {"9@gmail.com", "9_tg", "9_inst"}}
    };
    for (int i = 0; i < 10; i++) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->contact = initialContacts[i];
        newNode->next = NULL;
        newNode->prev = NULL;

        if (list->head == NULL) {
            list->head = list->tail = newNode;
        } else {
            Node* current = list->head;
            while (current != NULL && strcmp(current->contact.LastName, initialContacts[i].LastName) < 0) {
                current = current->next;
            }
            if (current == NULL) {
                list->tail->next = newNode;
                newNode->prev = list->tail;
                list->tail = newNode;
            } else if (current == list->head) {
                newNode->next = list->head;
                list->head->prev = newNode;
                list->head = newNode;
            } else {
                newNode->next = current;
                newNode->prev = current->prev;
                current->prev->next = newNode;
                current->prev = newNode;
            }
        }
    }
}

void addContact(DoublyLinkedList* list, int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->contact.id = id;
    strncpy(newNode->contact.LastName, LastName, 20);
    strncpy(newNode->contact.FirstName, FirstName, 20);
    strncpy(newNode->contact.MiddleName, MiddleName, 20);
    strncpy(newNode->contact.Workplace, Workplace, 40);
    strncpy(newNode->contact.Social.EmailAddress, Email, 40);
    strncpy(newNode->contact.Social.Telegram, Telegram, 40);
    strncpy(newNode->contact.Social.Instagramm, Instagramm, 40);
    newNode->next = NULL;
    newNode->prev = NULL;

    if (list->head == NULL) {
        list->head = list->tail = newNode;
    } else {
        Node* current = list->head;
        while (current != NULL && strcmp(current->contact.LastName, LastName) < 0) {
            current = current->next;
        }
        if (current == NULL) {
            list->tail->next = newNode;
            newNode->prev = list->tail;
            list->tail = newNode;
        } else if (current == list->head) {
            newNode->next = list->head;
            list->head->prev = newNode;
            list->head = newNode;
        } else {
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
        }
    }
}

void deleteContact(DoublyLinkedList* list, int id) {
    Node* current = list->head;
    while (current != NULL && current->contact.id != id) {
        current = current->next;
    }
    if (current == NULL) {
        return;
    }
    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }
    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        list->tail = current->prev;
    }
    free(current);
}

void changeContact(DoublyLinkedList* list, int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm) {
    deleteContact(list, id);
    addContact(list, id, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
}

void viewContacts(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        printf("id: %d ", current->contact.id);
        printf("Last Name: %s ", current->contact.LastName);
        printf("First Name: %s ", current->contact.FirstName);
        printf("Middle Name: %s ", current->contact.MiddleName);
        printf("Workplace: %s ", current->contact.Workplace);
        printf("Email Address: %s ", current->contact.Social.EmailAddress);
        printf("Telegram: %s ", current->contact.Social.Telegram);
        printf("Instagramm: %s \n", current->contact.Social.Instagramm);
        current = current->next;
    }
}