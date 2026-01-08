#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Grad {
    char naziv[50];
    int stanovnici;
    struct _Grad *left;
    struct _Grad *right;
} Grad;

typedef struct _DrzavaLista {
    char naziv[50];
    Grad *stabloGradova;
    struct _DrzavaLista *next;
} DrzavaLista;

typedef struct _DrzavaStablo {
    char naziv[50];
    DrzavaLista *listaGradova;
    struct _DrzavaStablo *left;
    struct _DrzavaStablo *right;
} DrzavaStablo;

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

DrzavaLista* insertDrzavaLista(DrzavaLista* head, char* naziv, Grad* stabloGradova) {
    DrzavaLista* novi = (DrzavaLista*)malloc(sizeof(DrzavaLista));
    strcpy(novi->naziv, naziv);
    novi->stabloGradova = stabloGradova;
    novi->next = NULL;

    if (!head || strcmp(naziv, head->naziv) < 0) {
        novi->next = head;
        return novi;
    }

    DrzavaLista* curr = head;
    while (curr->next && strcmp(curr->next->naziv, naziv) < 0)
        curr = curr->next;

    novi->next = curr->next;
    curr->next = novi;
    return head;
}

DrzavaStablo* insertDrzavaStablo(DrzavaStablo* root, char* naziv, Grad* stabloGradova) {
    if (!root) {
        DrzavaStablo* novi = (DrzavaStablo*)malloc(sizeof(DrzavaStablo));
        strcpy(novi->naziv, naziv);
        novi->listaGradova = NULL;

        void inorderGrad(Grad* g) {
            if (!g) return;
            inorderGrad(g->left);
            DrzavaLista* newGrad = (DrzavaLista*)malloc(sizeof(DrzavaLista));
            strcpy(newGrad->naziv, g->naziv);
            newGrad->stabloGradova = NULL;
            newGrad->next = novi->listaGradova;
            novi->listaGradova = newGrad;
            inorderGrad(g->right);
        }
        inorderGrad(stabloGradova);

        novi->left = novi->right = NULL;
        return novi;
    }

    if (strcmp(naziv, root->naziv) < 0)
        root->left = insertDrzavaStablo(root->left, naziv, stabloGradova);
    else
        root->right = insertDrzavaStablo(root->right, naziv, stabloGradova);

    return root;
}

Grad* citajGradove(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Greška%s\n", filename);
        return NULL;
    }

    Grad* root = NULL;
    char naziv[50];
    int stanovnici;
    while (fscanf(f, "%49[^,],%d\n", naziv, &stanovnici) == 2) {
        root = insertGrad(root, naziv, stanovnici);
    }
    fclose(f);
    return root;
}

void citajDrzave(DrzavaLista** headList, DrzavaStablo** rootTree, const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Ne mogu otvoriti fajl %s\n", filename);
        return;
    }

    char drzava[50], gradFile[50];
    while (fscanf(f, "%49s %49s\n", drzava, gradFile) == 2) {
        Grad* stabloGradova = citajGradove(gradFile);
        *headList = insertDrzavaLista(*headList, drzava, stabloGradova);
        *rootTree = insertDrzavaStablo(*rootTree, drzava, stabloGradova);
    }
    fclose(f);
}

void inorderGradovi(Grad* root) {
    if (!root) return;
    inorderGradovi(root->left);
    printf("    %s (%d)\n", root->naziv, root->stanovnici);
    inorderGradovi(root->right);
}

void ispisiListuDrzava(DrzavaLista* head) {
    while (head) {
        printf("%s:\n", head->naziv);
        inorderGradovi(head->stabloGradova);
        head = head->next;
    }
}

void pretraziGradove(Grad* root, int minStanovnika) {
    if (!root) return;
    pretraziGradove(root->left, minStanovnika);
    if (root->stanovnici > minStanovnika)
        printf("    %s (%d)\n", root->naziv, root->stanovnici);
    pretraziGradove(root->right, minStanovnika);
}

int main() {
    DrzavaLista* listaDrzava = NULL;
    DrzavaStablo* stabloDrzava = NULL;

    citajDrzave(&listaDrzava, &stabloDrzava, "drzave.txt");

    printf("Drzave i gradovi:\n");
    ispisiListuDrzava(listaDrzava);

    int minStanovnika;
    char drzavaPretraga[50];
    printf("\nUnesite drzavu: ");
    scanf("%s", drzavaPretraga);
    printf("Unesite min broj stanovnika: ");
    scanf("%d", &minStanovnika);

    DrzavaLista* temp = listaDrzava;
    while (temp) {
        if (strcmp(temp->naziv, drzavaPretraga) == 0) {
            printf("Gradovi sa vise od %d stanovnika:\n", minStanovnika);
            pretraziGradove(temp->stabloGradova, minStanovnika);
            break;
        }
        temp = temp->next;
    }
    if (!temp)
        printf("Drzava %s nije pronadjena.\n", drzavaPretraga);

    return 0;
}
