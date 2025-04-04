#ifndef LOGLIST_H
#define LOGLIST_H

typedef struct Socials {
    char EmailAddress[40];
    char Telegram[40];
    char Instagramm[40];
} Socials;

typedef struct Person {
    int id;
    char LastName[20];
    char FirstName[20];
    char MiddleName[20];
    char Workplace[40];
    struct Socials Social;
} Person;

typedef struct Node {
    Person contact;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct DoublyLinkedList {
    Node* head;
    Node* tail;
} DoublyLinkedList;

void initList(DoublyLinkedList* list);
void addContact(DoublyLinkedList* list, int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm);
void deleteContact(DoublyLinkedList* list, int id);
void changeContact(DoublyLinkedList* list, int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm);
void viewContacts(DoublyLinkedList* list);

#endif