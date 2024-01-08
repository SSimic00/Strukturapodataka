#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EXIT_SUCCESS (0)
#define ERROR (-1)
#define min (10)
#define max (90)

typedef struct tree* Position;
typedef struct tree {
    int el;
    Position left;
    Position right;
} Tree;

Position insert(Position p, int el);
Position newNode(int el);
Position deleteTree(Position p);
int replace(Position p);
int inOrder(Position p);
int generateRand();
int writeInFile(char* filename, Position p);
int inOrderFile(FILE* fp, Position p);

int main() {
    Position root = NULL;
    Position q = NULL;
    char filename[256];

    printf("Filename: ");
    scanf(" %s", filename);

    root = insert(root, 2);
    root = insert(root, 5);
    root = insert(root, 7);
    root = insert(root, 8);
    root = insert(root, 11);
    root = insert(root, 1);
    root = insert(root, 4);
    root = insert(root, 2);
    root = insert(root, 3);
    root = insert(root, 7);

    inOrder(root);
    printf("\n");
    writeInFile(filename, root);

    int replacedSum = replace(root);
    printf("Sum of replaced values: %d\n", replacedSum);

    inOrder(root);
    printf("\n");
    writeInFile(filename, root);

    root = deleteTree(root);

    // Generating random numbers:
    srand(time(0));
    for (int i = 0; i < 10; i++) {
        root = insert(root, generateRand());
    }

    inOrder(root);
    printf("\n");
    writeInFile(filename, root);

    replacedSum = replace(root);
    printf("Sum of replaced values: %d\n", replacedSum);

    writeInFile(filename, root);
    inOrder(root);
    printf("\n");

    root = deleteTree(root);

    return EXIT_SUCCESS;
}

int generateRand() {
    return rand() % (max - min + 1) + min;
}

Position newNode(int el) {
    Position new_node = (Position)malloc(sizeof(Tree));
    if (!new_node) {
        printf("Memory allocation unsuccessful.\n");
        exit(EXIT_FAILURE);
    }
    new_node->el = el;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

Position insert(Position p, int el) {
    if (p == NULL) {
        return newNode(el);
    }

    if (el < p->el) {
        p->left = insert(p->left, el);
    }
    else if (el > p->el) {
        p->right = insert(p->right, el);
    }

    return p;
}

int inOrder(Position p) {
    if (p == NULL) {
        return EXIT_SUCCESS;
    }
    inOrder(p->left);
    printf("%d ", p->el);
    inOrder(p->right);
    return EXIT_SUCCESS;
}

int replace(Position p) {
    if (p == NULL) {
        return 0;
    }

    int leftSum = replace(p->left);
    int rightSum = replace(p->right);

    int sum = p->el + leftSum + rightSum;
    p->el = leftSum + rightSum;

    return sum;
}

int writeInFile(char* filename, Position p) {
    FILE* fp = fopen(filename, "a");
    if (!fp) {
        printf("Error while trying to open the file!\n");
        exit(ERROR);
    }
    inOrderFile(fp, p);
    fprintf(fp, "\n");
    fclose(fp);
    return EXIT_SUCCESS;
}

int inOrderFile(FILE* fp, Position p) {
    if (p == NULL) {
        return EXIT_SUCCESS;
    }
    inOrderFile(fp, p->left);
    fprintf(fp, "%d  ", p->el);
    inOrderFile(fp, p->right);
    return EXIT_SUCCESS;
}

Position deleteTree(Position p) {
    if (p == NULL) {
        return NULL;
    }
    p->left = deleteTree(p->left);
    p->right = deleteTree(p->right);
    free(p);
    return NULL;
}
