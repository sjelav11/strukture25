#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct person* next;  // pokazivač na sljedeći element liste
} Person;

//korištene funkcije
Person* createPerson(const char* firstName, const char* lastName, int birthYear);
void addAtBeginning(Person** head, const char* firstName, const char* lastName, int birthYear);
void addAtEnd(Person** head, const char* firstName, const char* lastName, int birthYear);
void printList(Person* head);
Person* findByLastName(Person* head, const char* lastName);
void deleteByLastName(Person** head, const char* lastName);

Person* createPerson(const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greška, neuspjela alokacija memorije.\n");
        return NULL;
    }
    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

// A) Dodavanje na početak liste
void addAtBeginning(Person** head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return;
    newPerson->next = *head;
    *head = newPerson;
}

// B) Ispis liste
void printList(Person* head) {
    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s (%d)\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
}

// C) Dodavanje na kraj liste
void addAtEnd(Person** head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    if (!newPerson) return;

    if (*head == NULL) {
        *head = newPerson;
        return;
    }

    Person* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newPerson;
}

// D) Pronalazak osobe po prezimenu
Person* findByLastName(Person* head, const char* lastName) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// E) Brisanje osobu po prezimenu
void deleteByLastName(Person** head, const char* lastName) {
    Person* temp = *head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba sa prezimenom '%s' nije pronadjena.\n", lastName);
        return;
    }

    if (prev == NULL)
        *head = temp->next;  
    else
        prev->next = temp->next;  

    free(temp);
}


int main() {
    Person* head = NULL;  // početak liste

    addAtBeginning(&head, "Ante", "Bilic", 2002);
    addAtBeginning(&head, "Luka", "Tesic", 2000);
    addAtEnd(&head, "Mate", "Divic", 1999);

    printf("Lista:\n");
    printList(head);

    Person* found = findByLastName(head, "Divic");
    if (found != NULL)
        printf("\nPronadjen: %s %s (%d)\n", found->firstName, found->lastName, found->birthYear);
    else
        printf("\nOsoba nije pronadjena.\n");

    deleteByLastName(&head, "Divic");

    printf("\nNakon brisanja:\n");
    printList(head);

    return 0;
}
