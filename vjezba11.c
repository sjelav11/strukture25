#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 11

typedef struct _Grad {
    char naziv[50];
    int stanovnici;
    struct _Grad* left;
    struct _Grad* right;
} Grad;

typedef struct _Drzava {
    char naziv[50];
    Grad* stabloGradova;
    struct _Drzava* next;
} Drzava;

/*HASH FUNKCIJA*/

int hash(char* naziv) {
    int suma = 0;
    for (int i = 0; i < 5 && naziv[i] != '\0'; i++)
        suma += (int)naziv[i];
    return suma % TABLE_SIZE;
}

Grad* insertGrad(Grad* root, char* naziv, int stanovnici) {
    if (!root) {
        Grad* novi = (Grad*)malloc(sizeof(Grad));
        strcpy(novi->naziv, naziv);
        novi->stanovnici = stanovnici;
        novi->left = novi->right = NULL;
        return novi;
    }

    if (stanovnici < root->stanovnici)
        root->left = insertGrad(root->left, naziv, stanovnici);
    else if (stanovnici > root->stanovnici)
        root->right = insertGrad(root->right, naziv, stanovnici);
    else {
        if (strcmp(naziv, root->naziv) < 0)
            root->left = insertGrad(root->left, naziv, stanovnici);
        else
            root->right = insertGrad(root->right, naziv, stanovnici);
    }
    return root;
}

Grad* readCities(char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Ne mogu otvoriti %s\n", filename);
        return NULL;
    }

    Grad* root = NULL;
    char naziv[50];
    int stanovnici;

    while (fscanf(f, "%49[^,],%d\n", naziv, &stanovnici) == 2)
        root = insertGrad(root, naziv, stanovnici);

    fclose(f);
    return root;
}

/*UMETANJE DRŽAVE U HASH */
void insertState(Drzava* table[], char* naziv, Grad* gradovi) {
    int index = hash(naziv);

    Drzava* novi = (Drzava*)malloc(sizeof(Drzava));
    strcpy(novi->naziv, naziv);
    novi->stabloGradova = gradovi;
    novi->next = NULL;

    if (!table[index] || strcmp(naziv, table[index]->naziv) < 0) {
        novi->next = table[index];
        table[index] = novi;
        return;
    }

    Drzava* curr = table[index];
    while (curr->next && strcmp(curr->next->naziv, naziv) < 0)
        curr = curr->next;

    novi->next = curr->next;
    curr->next = novi;
}

void readStates(Drzava* table[], char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Ne mogu otvoriti %s\n", filename);
        return;
    }

    char drzava[50], gradFile[50];
    while (fscanf(f, "%49s %49s\n", drzava, gradFile) == 2) {
        Grad* gradovi = readCities(gradFile);
        insertState(table, drzava, gradovi);
    }
    fclose(f);
}

void printCities(Grad* root) {
    if (!root) return;
    printCities(root->left);
    printf("    %s (%d)\n", root->naziv, root->stanovnici);
    printCities(root->right);
}

void printHashTable(Drzava* table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("\n[%d]\n", i);
        Drzava* temp = table[i];
        while (temp) {
            printf("%s:\n", temp->naziv);
            printCities(temp->stabloGradova);
            temp = temp->next;
        }
    }
}

int main() {
    Drzava* hashTable[TABLE_SIZE] = { NULL };
    readStates(hashTable, "drzave.txt");
  
    printf("HASH TABLICA DRZAVA:\n");
    printHashTable(hashTable);
    return 0;
}
