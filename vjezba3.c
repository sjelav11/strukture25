#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct person* next;
} Person;

// Funkcije
Person* createPerson(const char* firstName, const char* lastName, int birthYear);
void addAtBeginning(Person** head, const char* firstName, const char* lastName, int birthYear);
void addAtEnd(Person** head, const char* firstName, const char* lastName, int birthYear);
void printList(Person* head);
Person* findByLastName(Person* head, const char* lastName);
void deleteByLastName(Person** head, const char* lastName);

// Novi dijelovi
void addAfter(Person* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);
void addBefore(Person** head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear);
void sortByLastName(Person** head);
void writeToFile(Person* head, const char* filename);
void readFromFile(Person** head, const char* filename);


Person* createPerson(const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Error: memory allocation failed.\n");
        return NULL;
    }
    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

void addAtBeginning(Person** head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = *head;
    *head = newPerson;
}

void addAtEnd(Person** head, const char* firstName, const char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    if (*head == NULL) {
        *head = newPerson;
        return;
    }
    Person* temp = *head;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newPerson;
}

void printList(Person* head) {
    Person* temp = head;
    while (temp) {
        printf("%s %s (%d)\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
}

Person* findByLastName(Person* head, const char* lastName) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void deleteByLastName(Person** head, const char* lastName) {
    Person* temp = *head;
    Person* prev = NULL;

    while (temp && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Person with last name '%s' not found.\n", lastName);
        return;
    }

    if (!prev)
        *head = temp->next;
    else
        prev->next = temp->next;

    free(temp);
}

// 🔹 A) Dodavanje iza određenog elementa
void addAfter(Person* head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Person* target = findByLastName(head, targetLastName);
    if (!target) {
        printf("Person with last name '%s' not found.\n", targetLastName);
        return;
    }

    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = target->next;
    target->next = newPerson;
}

// 🔹 B) Dodavanje ispred određenog elementa
void addBefore(Person** head, const char* targetLastName, const char* firstName, const char* lastName, int birthYear) {
    Person* temp = *head;
    Person* prev = NULL;

    while (temp && strcmp(temp->lastName, targetLastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Person with last name '%s' not found.\n", targetLastName);
        return;
    }

    Person* newPerson = createPerson(firstName, lastName, birthYear);

    if (!prev) { // ako se ubacuje na početak
        newPerson->next = *head;
        *head = newPerson;
    }
    else {
        prev->next = newPerson;
        newPerson->next = temp;
    }
}

// 🔹 C) Sortiranje po prezimenu (Bubble sort)
void sortByLastName(Person** head) {
    if (*head == NULL) return;
    int swapped;
    Person* ptr1;
    Person* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;

        while (ptr1->next != lptr) {
            if (strcmp(ptr1->lastName, ptr1->next->lastName) > 0) {
                // zamjena podataka
                char tempFirst[50], tempLast[50];
                int tempYear;

                strcpy(tempFirst, ptr1->firstName);
                strcpy(tempLast, ptr1->lastName);
                tempYear = ptr1->birthYear;

                strcpy(ptr1->firstName, ptr1->next->firstName);
                strcpy(ptr1->lastName, ptr1->next->lastName);
                ptr1->birthYear = ptr1->next->birthYear;

                strcpy(ptr1->next->firstName, tempFirst);
                strcpy(ptr1->next->lastName, tempLast);
                ptr1->next->birthYear = tempYear;

                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// 🔹 D) Spremanje liste u datoteku
void writeToFile(Person* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }
    Person* temp = head;
    while (temp) {
        fprintf(fp, "%s %s %d\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
    fclose(fp);
    printf("List saved to '%s'\n", filename);
}

// 🔹 E) Učitavanje liste iz datoteke
void readFromFile(Person** head, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    char fName[50], lName[50];
    int year;
    while (fscanf(fp, "%s %s %d", fName, lName, &year) == 3) {
        addAtEnd(head, fName, lName, year);
    }

    fclose(fp);
    printf("List loaded from '%s'\n", filename);
}


int main() {
    Person* head = NULL;

    addAtBeginning(&head, "Marko", "Maric", 1988);
    addAtEnd(&head, "Ivana", "Ivic", 1999);
    addAtEnd(&head, "Ana", "Anic", 1995);

    printf("Original list:\n");
    printList(head);

    addAfter(head, "Maric", "Luka", "Lovric", 1992);
    addBefore(&head, "Ivic", "Petar", "Peric", 1990);

    printf("\nAfter adding before and after:\n");
    printList(head);

    sortByLastName(&head);
    printf("\nSorted list:\n");
    printList(head);

    writeToFile(head, "people.txt");

    // Test učitavanja
    Person* newList = NULL;
    readFromFile(&newList, "people.txt");

    printf("\nLoaded list from file:\n");
    printList(newList);

    return 0;
}
