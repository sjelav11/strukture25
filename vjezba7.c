#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dir {
    char name[50];
    struct Dir *firstChild;
    struct Dir *nextSibling;
    struct Dir *parent;
} Dir;

/* Kreiranje direktorija */
Dir* createDir(char *name, Dir *parent) {
    Dir *newDir = (Dir*)malloc(sizeof(Dir));
    strcpy(newDir->name, name);
    newDir->firstChild = NULL;
    newDir->nextSibling = NULL;
    newDir->parent = parent;
    return newDir;
}

/*dodaj direktorij */
int md(Dir *current, char *name) {
    Dir *child = current->firstChild;

    Dir *newDir = createDir(name, current);

    if (!child) {
        current->firstChild = newDir;
        return 1;
    }

    while (child->nextSibling)
        child = child->nextSibling;

    child->nextSibling = newDir;
    return 1;
}

/* ispis sadržaja */
int dir(Dir *current) {
    Dir *child = current->firstChild;

    printf("\nSadrzaj direktorija %s:\n", current->name);
    if (!child) {
        printf("Prazno.\n");
        return 1;
    }

    while (child) {
        printf("[%s]\n", child->name);
        child = child->nextSibling;
    }
    return 1;
}

/* ulazak u poddirektorij */
Dir* cd(Dir *current, char *name) {
    Dir *child = current->firstChild;

    while (child) {
        if (strcmp(child->name, name) == 0)
            return child;
        child = child->nextSibling;
    }

    printf("Direktorij '%s' ne postoji!\n", name);
    return current;  
}

/* cd..: povratak*/
Dir* cdBack(Dir *current) {
    if (current->parent)
        return current->parent;

    printf("Vec si u root direktoriju!\n");
    return current;
}


int main() {
    char command[50], name[50];
    int choice;

    Dir *root = createDir("C:", NULL);
    Dir *current = root;

    while (1) {
        printf("\n Trenutni direktorij: %s \n", current->name);
        printf("1 - md (napravi direktorij)\n");
        printf("2 - cd dir (udji u direktorij)\n");
        printf("3 - cd.. (povratak)\n");
        printf("4 - dir (ispis)\n");
        printf("5 - izlaz\n");
        printf("Odabir: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Unesi ime direktorija: ");
            scanf("%s", name);
            md(current, name);
        }

        else if (choice == 2) {
            printf("Unesi ime direktorija za ulazak: ");
            scanf("%s", name);
            current = cd(current, name);
        }

        else if (choice == 3) {
            current = cdBack(current);
        }

        else if (choice == 4) {
            dir(current);
        }

        else if (choice == 5) {
            printf("Izlaz.\n");
            break;
        }

        else {
            printf("Pogresan unos!\n");
        }
    }

    return 0;
}
