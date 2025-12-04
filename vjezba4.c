#define_CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int coef;
    int exp;
    struct Node *next;
} Node;

typedef Node* Position;

Position InsertSorted(Position head, int coef, int exp) {
    Position newNode, p = head, prev = NULL;

    if (coef == 0) return head;

    while (p != NULL && p->exp > exp) {
        prev = p;
        p = p->next;
    }

    if (p != NULL && p->exp == exp) {
        p->coef += coef;

        if (p->coef == 0) {
            if (prev) prev->next = p->next;
            else head = p->next;
            free(p);
        }
        return head;
    }

    newNode = (Position)malloc(sizeof(Node));
    newNode->coef = coef;
    newNode->exp = exp;
    newNode->next = p;

    if (prev == NULL)
        head = newNode;
    else
        prev->next = newNode;

    return head;
}

Position LoadPolynomial(Position P, FILE *fp) {
    int c, e;
    while (fscanf(fp, "%d %d", &c, &e) == 2) {
        P = InsertSorted(P, c, e);
        if (fgetc(fp) == '\n') break;
    }
    return P;
}

Position Add(Position P, Position Q) {
    Position R = NULL;

    while (P) {
        R = InsertSorted(R, P->coef, P->exp);
        P = P->next;
    }
    while (Q) {
        R = InsertSorted(R, Q->coef, Q->exp);
        Q = Q->next;
    }

    return R;
}


Position Multiply(Position P, Position Q) {
    Position R = NULL, qtemp;

    while (P) {
        qtemp = Q;
        while (qtemp) {
            R = InsertSorted(R, P->coef * qtemp->coef, P->exp + qtemp->exp);
            qtemp = qtemp->next;
        }
        P = P->next;
    }

    return R;
}

int PrintPoly(Position P) {
    if (!P) {
        printf("0\n");
        return 0;
    }

    while (P) {
        printf("%dx^%d", P->coef, P->exp);
        if (P->next) printf(" + ");
        P = P->next;
    }

    printf("\n");
    return 0;
}

int main() {
    Position P1 = NULL, P2 = NULL, Sum = NULL, Product = NULL;
    FILE *fp = fopen("polinomi.txt","r");

    if (!fp) {
        printf("Error opening file!\n");
        return -1;
    }

    P1 = LoadPolynomial(P1, fp);
    P2 = LoadPolynomial(P2, fp);
    fclose(fp);

    printf("Polynomial 1: ");
    PrintPoly(P1);

    printf("Polynomial 2: ");
    PrintPoly(P2);

    Sum = Add(P1, P2);
    printf("\nSum: ");
    PrintPoly(Sum);

    Product = Multiply(P1, P2);
    printf("Product: ");
    PrintPoly(Product);

    return 0;
}
