#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int el;
    struct Node *left;
    struct Node *right;
} Node;

/* Kreiranje čvora */
Node* createNode(int el) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->el = el;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insert(Node *root, int el) {
    if (root == NULL)
        return createNode(el);

    if (el < root->el)
        root->left = insert(root->left, el);
    else if (el > root->el)
        root->right = insert(root->right, el);

    return root;
}

/* Traženje elementa */
Node* find(Node *root, int el) {
    if (root == NULL || root->el == el)
        return root;

    if (el < root->el)
        return find(root->left, el);
    else
        return find(root->right, el);
}

Node* findMin(Node *root) {
    while (root && root->left)
        root = root->left;
    return root;
}

/*Brisanje elementa */
Node* delete(Node *root, int el) {
    if (root == NULL)
        return NULL;

    if (el < root->el)
        root->left = delete(root->left, el);
    else if (el > root->el)
        root->right = delete(root->right, el);
    else {
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        Node *temp = findMin(root->right);
        root->el = temp->el;
        root->right = delete(root->right, temp->el);
    }
    return root;
}

/* Inorder */
int inorder(Node *root) {
    if (root) {
        inorder(root->left);
        printf("%d ", root->el);
        inorder(root->right);
    }
    return 0;
}

/* Preorder */
int preorder(Node *root) {
    if (root) {
        printf("%d ", root->el);
        preorder(root->left);
        preorder(root->right);
    }
    return 0;
}

/*Postorder*/
int postorder(Node *root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->el);
    }
    return 0;
}

/*Level order*/
int levelOrder(Node *root) {
    if (!root) return 0;

    Node *queue[100];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        Node *curr = queue[front++];
        printf("%d ", curr->el);

        if (curr->left)
            queue[rear++] = curr->left;
        if (curr->right)
            queue[rear++] = curr->right;
    }
    return 0;
}

int main() {
    Node *root = NULL;
    int choice, value;

    while (1) {
        printf("\n--- BST MENU ---\n");
        printf("1 - Insert element\n");
        printf("2 - Find element\n");
        printf("3 - Delete element\n");
        printf("4 - Inorder\n");
        printf("5 - Preorder\n");
        printf("6 - Postorder\n");
        printf("7 - Level order\n");
        printf("8 - Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value: ");
            scanf("%d", &value);
            root = insert(root, value);
            break;

        case 2:
            printf("Enter value: ");
            scanf("%d", &value);
            if (find(root, value))
                printf("Element FOUND.\n");
            else
                printf("Element NOT FOUND.\n");
            break;

        case 3:
            printf("Enter value: ");
            scanf("%d", &value);
            root = delete(root, value);
            break;

        case 4:
            printf("Inorder: ");
            inorder(root);
            printf("\n");
            break;

        case 5:
            printf("Preorder: ");
            preorder(root);
            printf("\n");
            break;

        case 6:
            printf("Postorder: ");
            postorder(root);
            printf("\n");
            break;

        case 7:
            printf("Level order: ");
            levelOrder(root);
            printf("\n");
            break;

        case 8:
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }
}
