#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
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
 
void addContact(Person* contactlist, int cnt, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm) {
    if (cnt >= Max_Person)
    {
        return;
    }
    Person NewCont;
    NewCont.id = cnt;
    strncpy(NewCont.LastName, LastName, 20);
    strncpy(NewCont.FirstName, FirstName, 20);
    strncpy(NewCont.MiddleName, MiddleName, 20);
    strncpy(NewCont.Workplace, Workplace, 40);
    strncpy(NewCont.Social.EmailAddress, Email, 40);
    strncpy(NewCont.Social.Telegram, Telegram, 40);
    strncpy(NewCont.Social.Instagramm, Instagramm, 40);
    *contactlist = NewCont;
}
 
void DeleteContact(Person* Chdel, Person contactlist[], int id, int cnt) {
    Person Del[Max_Person];
    int i = 0;
    for (i; i < id; i++) { Del[i] = contactlist[i]; }
    for (i = id; i < cnt - 1; i++) { Del[i] = contactlist[i + 1]; Del[i].id = i;}
    for (i = 0; i < cnt - 1; i++) { Chdel[i] = Del[i]; }
}
 
void ChangeContact(Person* contactlist, int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm)
{
    Person ChCont;
    ChCont.id = id;
    strncpy(ChCont.LastName, LastName, 20);
    strncpy(ChCont.FirstName, FirstName, 20);
    strncpy(ChCont.MiddleName, MiddleName, 20);
    strncpy(ChCont.Workplace, Workplace, 40);
    strncpy(ChCont.Social.EmailAddress, Email, 40);
    strncpy(ChCont.Social.Telegram, Telegram, 40);
    strncpy(ChCont.Social.Instagramm, Instagramm, 40);
    *contactlist = ChCont;
}
 
void ViewContact(Person contactlist[], int cnt) {
    for (int i = 0; i < cnt; i++)
    {
        printf("id: %d ", contactlist[i].id);
        printf("Last Name: %s ", contactlist[i].LastName);
        printf("First Name: %s ", contactlist[i].FirstName);
        printf("Middle Name: %s ", contactlist[i].MiddleName);
        printf("Workplace: %s ", contactlist[i].Workplace);
        printf("Email Address: %s ", contactlist[i].Social.EmailAddress);
        printf("Telegram: %s ", contactlist[i].Social.Telegram);
        printf("Instagramm: %s \n", contactlist[i].Social.Instagramm);
    }
}
 
int main() {
    int cmd = 1;
    int id = 0;
    int Optional;
    int id_cnt = 0;
    Person clist[Max_Person];
    int cnt = 0;
    char FirstName[20];
    char LastName[20];
    char MiddleName[20] = { 0 };
    char Email[40] = { 0 };
    char Workplace[40] = { 0 };
    char Telegram[40] = { 0 };
    char Instagramm[40] = {0};
    while (cmd)
    {
        printf("Menu\n");
        printf("1. Add new contact\n");
        printf("2. Change existing contact\n");
        printf("3. Delete existing contact\n");
        printf("4. View PhoneBook\n");
        printf("0. Exit\n");
        printf("Enter Option ");
        scanf("%d", &cmd);
        fseek(stdin, 0, SEEK_END);
        switch (cmd) {
        case 1:
            Person result;
            printf("Enter Last Name: ");
            scanf("%s", LastName);
            printf("Enter First Name: ");
            scanf("%s", FirstName);
            printf("Do you want to write optional fields? [1-yes / 0-no] ");
            scanf("%d", &Optional);
            if (Optional == 1) {
                printf("Enter Middle Name: ");
                scanf("%s", MiddleName);
                printf("Enter Workplace: ");
                scanf("%s", Workplace);
                printf("Enter Email Address: ");
                scanf("%s", Email);
                printf("Enter Telegram: ");
                scanf("%s", Telegram);
                printf("Enter Instagramm: ");
                scanf("%s", Instagramm);
            }
            addContact(&result, id_cnt, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
            if (Optional == 1)
            {
                strncpy(MiddleName, "", 20);
                strncpy(Workplace, "", 40);
                strncpy(Email, "", 40);
                strncpy(Telegram, "", 40);
                strncpy(Instagramm, "", 40);
            }
            clist[cnt] = result;
            cnt++;
            id_cnt++;
            break;
        case 2:
            Person chRes;
            printf("Enter id: ");
            scanf("%d", &id);
            printf("Enter Last Name: ");
            scanf("%s", LastName);
            printf("Enter First Name: ");
            scanf("%s", FirstName);
            printf("Do you want to change optional fields? [1-yes / 0-no] ");
            scanf("%d", &Optional);
            if (Optional == 1) {
                printf("Enter Middle Name: ");
                scanf("%s", MiddleName);
                printf("Enter Workplace: ");
                scanf("%s", Workplace);
                printf("Enter Email Address: ");
                scanf("%s", Email);
                printf("Enter Telegram: ");
                scanf("%s", Telegram);
                printf("Enter Instagramm: \n");
                scanf("%s", Instagramm);
            }
            ChangeContact(&chRes, id, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
            strncpy(clist[id].LastName, chRes.LastName, 20);
            strncpy(clist[id].FirstName, chRes.FirstName, 20);
            if (Optional == 1)
            {
                strncpy(clist[id].MiddleName, chRes.MiddleName, 20);
                strncpy(clist[id].Workplace, chRes.Workplace, 40);
                strncpy(clist[id].Social.EmailAddress, chRes.Social.EmailAddress, 40);
                strncpy(clist[id].Social.Telegram, chRes.Social.Telegram, 40);
                strncpy(clist[id].Social.Instagramm, chRes.Social.Instagramm, 40);
            }
            break;
        case 3:
            Person chDel[Max_Person];
            printf("Enter id: ");
            scanf("%d", &id);
            DeleteContact(chDel, clist, id, cnt);
            for (int i = 0; i < cnt - 1; i++) {
                clist[i] = chDel[i];
            }
            cnt--;
            break;
        case 4:
            ViewContact(clist, cnt);
            break;
        case 0:
            break;
        default:
            break;
        }
    }
}
