#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct tree* Position;

typedef struct tree {
    int el;
    Position L;
    Position R;
} Tree;

Position Insert(Position curr, int element);
int PrintInorder(Position curr);
int PrintPostorder(Position curr);
int PrintPreorder(Position curr);
int Depth(Position curr);
int PrintLevel(Position curr, int level);
int PrintLevelorder(Position curr);
Position FindElement(Position curr, int element);
Position Delete(Position curr, int element);
Position FindMin(Position curr);
void FreeTree(Position curr);

int main()
{
    Position root = NULL;
    int n, element;
    Position temp = NULL; // Premjesteno iznad do-while petlje

    do
    {
        printf("Choose the wanted operation:\n");
        printf("1 - add a new element to the tree\n");
        printf("2 - print elements (inorder)\n");
        printf("3 - print elements (postorder)\n");
        printf("4 - print elements (preorder)\n");
        printf("5 - print elements (level order)\n");
        printf("6 - search element\n");
        printf("7 - delete element\n");
        printf("8 - exit\n");

        scanf("%d", &n);
        system("cls");

        switch (n)
        {
        case 1:
            printf("Enter the element you want to add to the tree:\n");
            scanf("%d", &element);
            root = Insert(root, element);
            break;
        case 2:
            PrintInorder(root);
            break;
        case 3:
            PrintPostorder(root);
            break;
        case 4:
            PrintPreorder(root);
            break;
        case 5:
            PrintLevelorder(root);
            break;
        case 6:
            printf("Enter the wanted element:\n");
            scanf("%d", &element);
            temp = FindElement(root, element);
            if (temp)
                printf("Element %d exists in this tree!\n", temp->el);
            else
                printf("Element not found in the tree.\n");
            break;
        case 7:
            printf("Enter the element you want to delete:\n");
            scanf("%d", &element);
            root = Delete(root, element);
            break;
        case 8:
            FreeTree(root);
            return 0;

        default:
            printf("Wrong choice, try again!\n");
        }
    } while (n != 8);

    return EXIT_SUCCESS;
}

void FreeTree(Position curr) {
    if (curr != NULL)
    {
        FreeTree(curr->L);
        FreeTree(curr->R);
        free(curr);
    }
}

Position Insert(Position curr, int element) {
    if (curr == NULL) {
        curr = (Position)malloc(sizeof(Tree));
        if (curr == NULL) {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        curr->el = element;
        curr->L = NULL;
        curr->R = NULL;
        return curr;
    }
    else if (element > curr->el) {
        curr->R = Insert(curr->R, element);
    }
    else if (element < curr->el) {
        curr->L = Insert(curr->L, element);
    }
    return curr;
}

int PrintInorder(Position curr) {
    if (curr) {
        PrintInorder(curr->L);
        printf("%d\n", curr->el);
        PrintInorder(curr->R);
    }
    return EXIT_SUCCESS;
}

int PrintPostorder(Position curr) {
    if (curr)
    {
        PrintPostorder(curr->L);
        PrintPostorder(curr->R);
        printf("%d\n", curr->el);
    }
    return EXIT_SUCCESS;
}

int PrintPreorder(Position curr) {
    if (curr)
    {
        printf("%d\n", curr->el);
        PrintPreorder(curr->L);
        PrintPreorder(curr->R);
    }
    return EXIT_SUCCESS;
}

int PrintLevelorder(Position curr) {
    int d = Depth(curr);
    int i;
    for (i = 1; i <= d; i++)
        PrintLevel(curr, i);

    return EXIT_SUCCESS;
}

int PrintLevel(Position curr, int level)
{
    if (curr == NULL)
        return EXIT_SUCCESS;

    if (level == 1)
        printf("%d\n", curr->el);
    else
    {
        PrintLevel(curr->L, level - 1);
        PrintLevel(curr->R, level - 1);
    }

    return EXIT_SUCCESS;
}

int Depth(Position curr)
{
    if (curr == NULL)
        return 0;

    int DepthL = Depth(curr->L);
    int DepthR = Depth(curr->R);

    return (DepthL > DepthR) ? (DepthL + 1) : (DepthR + 1);
}

Position FindElement(Position curr, int element)
{
    if (!curr) {
        return NULL;
    }
    else if (element < curr->el)
        return FindElement(curr->L, element);
    else if (element > curr->el)
        return FindElement(curr->R, element);

    return curr;
}

Position Delete(Position curr, int element) {
    Position temp;
    if (!curr)
        return curr;
    else if (element < curr->el)
        curr->L = Delete(curr->L, element);
    else if (element > curr->el)
        curr->R = Delete(curr->R, element);
    else
    {
        if (curr->L && curr->R)
        {
            temp = FindMin(curr->R);
            curr->el = temp->el;
            curr->R = Delete(curr->R, temp->el);
        }
        else
        {
            temp = curr;
            if (!curr->L)
                curr = curr->R;
            else if (!curr->R)
                curr = curr->L;
            free(temp);
        }
    }
    return curr;
}

Position FindMin(Position curr)
{
    while (curr->L)
        curr = curr->L;
    return curr;
}
