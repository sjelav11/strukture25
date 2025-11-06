#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    double value;
    struct node* next;
} Node;

// Funkcije za stog
void push(Node** top, double value);
double pop(Node** top);
int isEmpty(Node* top);
void freeStack(Node** top);

// Funkcija za računanje postfiks izraza
double evaluatePostfix(const char* filename);

int main() {
    double result = evaluatePostfix("postfix.txt");
    printf("Rezultat izraza: %.2f\n", result);
    return 0;
}

// Dodavanje elementa na vrh stoga
void push(Node** top, double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Greška: neuspjela alokacija memorije!\n");
        exit(1);
    }
    newNode->value = value;
    newNode->next = *top;
    *top = newNode;
}

// Uklanjanje elementa s vrha stoga
double pop(Node** top) {
    if (isEmpty(*top)) {
        printf("Greška: stog je prazan!\n");
        exit(1);
    }
    Node* temp = *top;
    double value = temp->value;
    *top = temp->next;
    free(temp);
    return value;
}

// Provjera je li stog prazan
int isEmpty(Node* top) {
    return top == NULL;
}

// Oslobađanje memorije
void freeStack(Node** top) {
    while (!isEmpty(*top)) {
        pop(top);
    }
}

// Funkcija koja čita postfiks izraz iz datoteke i računa rezultat
double evaluatePostfix(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Greška: ne mogu otvoriti datoteku '%s'\n", filename);
        exit(1);
    }

    Node* stack = NULL;
    char token[50];

    // Čitanje izraza token po token (broj ili operator)
    while (fscanf(file, "%s", token) == 1) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // Ako je broj, pretvori u double i stavi na stog
            double num = atof(token);
            push(&stack, num);
        }
        else {
            // Inače je operator (+, -, *, /)
            double b = pop(&stack);
            double a = pop(&stack);
            double result;

            switch (token[0]) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Greška: dijeljenje s nulom!\n");
                    freeStack(&stack);
                    fclose(file);
                    exit(1);
                }
                result = a / b;
                break;
            default:
                printf("Nepoznat operator: %s\n", token);
                freeStack(&stack);
                fclose(file);
                exit(1);
            }
            push(&stack, result);
        }
    }

    fclose(file);

    double finalResult = pop(&stack);
    if (!isEmpty(stack)) {
        printf("Greška: višak brojeva u izrazu!\n");
        freeStack(&stack);
        exit(1);
    }

    return finalResult;
}

