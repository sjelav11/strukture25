#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node* Position;

typedef struct Node {
    int value;
    Position left;
    Position right;
} Node;

Position createNode(int value) {
    Position newNode = (Position)malloc(sizeof(Node));
    if (!newNode)
        return NULL;

    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

Position insert(Position root, int value) {
    if (root == NULL)
        return createNode(value);

    if (root->left == NULL)
        root->left = insert(root->left, value);
    else
        root->right = insert(root->right, value);

    return root;
}

int inorder(Position root, FILE* fp) {
    if (root == NULL)
        return 0;

    inorder(root->left, fp);
    fprintf(fp, "%d ", root->value);
    inorder(root->right, fp);

    return 0;
}

int replace(Position root) {
    int leftSum = 0, rightSum = 0;
    int oldValue = 0;
    
    if (root == NULL)
        return 0;
        
    leftSum = replace(root->left);
    rightSum = replace(root->right);
    
    oldValue = root->value;
    root->value = leftSum + rightSum;
    return root->value + oldValue;
}

int main() {
    Position root = NULL;
    FILE* fp;
    int i;
    int n = 10;

    srand((unsigned int)time(NULL));

    for (i = 0; i < n; i++) {
        int x = rand() % 81 + 10;
        root = insert(root, x);
    }

    fp = fopen("stablo.txt", "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    fprintf(fp, "Inorder nakon funkcije insert:\n");
    inorder(root, fp);
    fprintf(fp, "\n");

    replace(root);

    fprintf(fp, "Inorder nakon funkcije replace:\n");
    inorder(root, fp);

    fclose(fp);

    printf("Rezultati su upisani u stablo.txt\n");
    return 0;
}
