#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define Max_Person 50
typedef struct Socials {
    char EmailAddress[40];
    char Telegram[40];
    char Instagramm[40];
}Socials; 

typedef struct Person {
    int id;
    char LastName[20];
    char FirstName[20];
    char MiddleName[20];
    char Workplace[40];
    struct Socials Social;
}Person;

void addContact (Person contactlist[], int cnt, char* LastName, char* FirstName){
    contactlist[cnt].id = cnt;
    strncpy(contactlist[cnt].LastName, LastName, 20);
    strncpy(contactlist[cnt].FirstName, FirstName, 20);
}
void DeleteContact (int id) {

}

int main(){
    int cmd = 1;
    char option;
    Person clist[Max_Person];
    char temp;
    int cnt = 0;
    char* FirstName;
    char* LastName;
    char* MiddleName;
    char* Email;
    char* Workplace;
    char* Telegram;
    char* Instagramm;
    while (cmd)
    {
        printf("Menu\n");
        printf("1. Add new contact\n");
        printf("2. Change existing contact\n");
        printf("3. Delete existing contact\n");
        printf("4. Viev PhoneBook\n");
        printf("0. Exit\n");
        printf("Enter Option ");
        scanf("%d", &cmd);
        switch (cmd) {
            case 1:
                cnt++;
                char *FirstName=(char*)malloc(sizeof(char)*20);
                scanf("%s", FirstName);
                char *LastName=(char*)malloc(sizeof(char)*20);
                scanf("%s", LastName);
                /*char *MiddleName=(char*)malloc(sizeof(char)*20);
                scanf("%s", MiddleName);
                char *Workplace=(char*)malloc(sizeof(char)*20);
                scanf("%s", Workplace);
                char *Email=(char*)malloc(sizeof(char)*20);
                scanf("%s", Email);
                char *Telegram=(char*)malloc(sizeof(char)*20);
                scanf("%s", Telegram);
                char *Instagramm=(char*)malloc(sizeof(char)*20);
                scanf("%s", Instagramm);
                ("%s", LastName);*/
                addContact(cnt, LastName, FirstName);
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                //printf("id: %d\n", contactlist[cnt].id);
               //printf("LastName: %s\n", contactlist[cnt].LastName);
               //printf("FirstName: %s\n", contactlist[cnt].FirstName);
                break;
            case 0:
                break;
            default:
        }
    }
    //printf("Name %s %s %s\n", contactlist[cnt].FirstName, contactlist[cnt].LastName, contactlist[cnt].MiddleName);
    ///printf("Workplace %s\n", contactlist[cnt].Workplace);
    //printf("Socials %s, %s, %s \n", contactlist[cnt].Social.EmailAddress, contactlist[cnt].Social.Telegram, contactlist[cnt].Social.Instagramm);
}