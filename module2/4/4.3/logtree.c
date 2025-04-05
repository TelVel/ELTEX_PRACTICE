#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Max_Person 50

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
    struct Person* left;
    struct Person* right;
    int height;
} Person;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(Person* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

Person* newNode(int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm) {
    Person* node = (Person*)malloc(sizeof(Person));
    node->id = id;
    strncpy(node->LastName, LastName, 20);
    strncpy(node->FirstName, FirstName, 20);
    strncpy(node->MiddleName, MiddleName, 20);
    strncpy(node->Workplace, Workplace, 40);
    strncpy(node->Social.EmailAddress, Email, 40);
    strncpy(node->Social.Telegram, Telegram, 40);
    strncpy(node->Social.Instagramm, Instagramm, 40);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Person* rightRotate(Person* y) {
    Person* x = y->left;
    Person* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Person* leftRotate(Person* x) {
    Person* y = x->right;
    Person* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(Person* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

Person* insert(Person* node, int id, char* LastName, char* FirstName, char* MiddleName, char* Workplace, char* Email, char* Telegram, char* Instagramm) {
    if (node == NULL)
        return newNode(id, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);

    if (id < node->id)
        node->left = insert(node->left, id, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
    else if (id > node->id)
        node->right = insert(node->right, id, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && id < node->left->id)
        return rightRotate(node);

    if (balance < -1 && id > node->right->id)
        return leftRotate(node);

    if (balance > 1 && id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Person* minValueNode(Person* node) {
    Person* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Person* deleteNode(Person* root, int id) {
    if (root == NULL)
        return root;

    if (id < root->id)
        root->left = deleteNode(root->left, id);
    else if (id > root->id)
        root->right = deleteNode(root->right, id);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Person* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Person* temp = minValueNode(root->right);
            root->id = temp->id;
            strncpy(root->LastName, temp->LastName, 20);
            strncpy(root->FirstName, temp->FirstName, 20);
            strncpy(root->MiddleName, temp->MiddleName, 20);
            strncpy(root->Workplace, temp->Workplace, 40);
            strncpy(root->Social.EmailAddress, temp->Social.EmailAddress, 40);
            strncpy(root->Social.Telegram, temp->Social.Telegram, 40);
            strncpy(root->Social.Instagramm, temp->Social.Instagramm, 40);
            root->right = deleteNode(root->right, temp->id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void preOrder(Person* root) {
    if (root != NULL) {
        printf("id: %d ", root->id);
        printf("Last Name: %s ", root->LastName);
        printf("First Name: %s ", root->FirstName);
        printf("Middle Name: %s ", root->MiddleName);
        printf("Workplace: %s ", root->Workplace);
        printf("Email Address: %s ", root->Social.EmailAddress);
        printf("Telegram: %s ", root->Social.Telegram);
        printf("Instagramm: %s \n", root->Social.Instagramm);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void printTree(Person* root, int space) {
    if (root == NULL)
        return;
    
    space += 5;
    
    printTree(root->right, space);
    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%d\n", root->id);
    printTree(root->left, space);
}

int main() {
    int cmd = 1;
    int id = 0;
    int Optional;
    int id_cnt = 0;
    Person* root = NULL;
    root = insert(root, 0, "LastName10", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 55, "LastName51", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 99, "LastName12", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 44, "LastName44", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 88, "LastName33", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 12, "LastName88", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 55, "LastName99", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 19, "LastName66", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 22, "LastName22", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    root = insert(root, 9, "LastName19", "FirstName0", "MiddleName0", "Company0", "0@gmail.com", "0_tg", "0_inst");
    char FirstName[20];
    char LastName[20];
    char MiddleName[20] = { 0 };
    char Email[40] = { 0 };
    char Workplace[40] = { 0 };
    char Telegram[40] = { 0 };
    char Instagramm[40] = { 0 };
    while (cmd) {
        printf("Menu\n");
        printf("1. Add new contact\n");
        printf("2. Change existing contact\n");
        printf("3. Delete existing contact\n");
        printf("4. View PhoneBook\n");
        printf("5. View Tree\n");
        printf("0. Exit\n");
        printf("Enter Option ");
        scanf("%d", &cmd);
        fseek(stdin, 0, SEEK_END);
        switch (cmd) {
        case 1:
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
            root = insert(root, id_cnt, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
            id_cnt++;
            break;
        case 2:
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
            root = deleteNode(root, id);
            root = insert(root, id, LastName, FirstName, MiddleName, Workplace, Email, Telegram, Instagramm);
            break;
        case 3:
            printf("Enter id: ");
            scanf("%d", &id);
            root = deleteNode(root, id);
            break;
        case 4:
            preOrder(root);
            break;
        case 5:
            printTree(root, 0);
            break;
        case 0:
            break;
        default:
            break;
        }
    }
    return 0;
}
