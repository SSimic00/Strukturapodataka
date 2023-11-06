#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SIZE 50

struct Polynomial;
typedef struct Polynomial* Position;
typedef struct Polynomial {
    int coeff, exp;
    Position next;
} Poly;

int ReadPolynomialsFromInput(Position Head1, Position Head2, char* InputFile);
Position CreatePolynomial(int coeff, int exp);
int AddPolynomials(Position Head1, Position Head2, Position HeadResult);
int MultiplyPolynomials(Position Head1, Position Head2, Position HeadResult);
int SortedInsert(Position Head, Position NewElement);
int InsertAfter(Position P, Position NewElement);
int ReadLines(Position Head, char* buffer);
int RemoveZeroCoefficients(Position Head);
int PrintPolynomial(Position P);
int FreeMemory(Position Head);

int main() {
    char InputFile[MAX_SIZE] = { 0 };
    int choice = 1;

    Poly Head1 = { .coeff = 0, .exp = 0, .next = NULL };
    Poly Head2 = { .coeff = 0, .exp = 0, .next = NULL };
    Poly HeadSum = { .coeff = 0, .exp = 0, .next = NULL };
    Poly HeadProduct = { .coeff = 0, .exp = 0, .next = NULL };

    Position P1 = &Head1;
    Position P2 = &Head2;
    Position PSum = &HeadSum;
    Position PProduct = &HeadProduct;

    printf("Enter the name of the input file: ");
    scanf(" %s", InputFile);

    while (choice != 0) {
       
        printf("What do you want to do with the polynomials?\n");
        printf("Add them - 1\n");
        printf("Multiply them - 2\n");
        printf("Exit the program - 0\n\n");
        scanf(" %d", &choice);

        switch (choice) {
        case 0:
            break;

        case 1:
            if (ReadPolynomialsFromInput(P1, P2, InputFile) != EXIT_SUCCESS) break;
            printf("Polynomials are:\n");
            printf("\n1st polynomial:\n");
            PrintPolynomial(P1);
            printf("\n2nd polynomial:\n");
            PrintPolynomial(P2);
            printf("\nSum of polynomials:\n");
            AddPolynomials(P1, P2, PSum);
            PrintPolynomial(PSum);
            system("pause > nul");
            FreeMemory(P1);
            FreeMemory(P2);
            FreeMemory(PSum);
            break;

        case 2:
            if (ReadPolynomialsFromInput(P1, P2, InputFile) != EXIT_SUCCESS) break;
            printf("Polynomials are:\n");
            printf("\n1st polynomial:\n");
            PrintPolynomial(P1);
            printf("\n2nd polynomial:\n");
            PrintPolynomial(P2);
            printf("\nProduct of polynomials:\n");
            MultiplyPolynomials(P1, P2, PProduct);
            PrintPolynomial(PProduct);
            system("pause > nul");
            FreeMemory(P1);
            FreeMemory(P2);
            FreeMemory(PProduct);
            break;

        default:
            printf("Select 0,1,2 please\n");
            system("pause > nul");
            break;
        }
    }

    return EXIT_SUCCESS;
}

int ReadPolynomialsFromInput(Position Head1, Position Head2, char* InputFile) {
    FILE* fp = NULL;
    char buffer[MAX_LINE] = { 0 };

    fp = fopen(InputFile, "r");
    if (!fp) {
        perror("Unable to open the file!\n");
        system("pause > nul");
        return -1;
    }

    fgets(buffer, MAX_LINE, fp);
    ReadLines(Head1, buffer);
    RemoveZeroCoefficients(Head1);

    fgets(buffer, MAX_LINE, fp);
    ReadLines(Head2, buffer);
    RemoveZeroCoefficients(Head2);

    printf("\nData has been read from %s, press ENTER to display the results!\n\n", InputFile);

    fclose(fp);
    system("pause > nul");

    return EXIT_SUCCESS;
}

int ReadLines(Position Head, char* buffer) {
    int coeff, exp;
    int counter = 0;
    int bytesRead = 0;

    while (strlen(buffer) > 0) {
        bytesRead = sscanf(buffer, " %d %d %n", &coeff, &exp, &counter);

        if (bytesRead == 2 && coeff != 0)
            SortedInsert(Head, CreatePolynomial(coeff, exp));

        buffer += counter;
    }

    return EXIT_SUCCESS;
}

int RemoveZeroCoefficients(Position Head) {
    Position temp1 = Head;
    Position temp2 = NULL;

    while (temp1->next != NULL) {
        if (temp1->next->coeff == 0) {
            temp2 = temp1->next;
            temp1->next = temp2->next;
            free(temp2);
        }
        else
            temp1 = temp1->next;
    }

    return EXIT_SUCCESS;
}

Position CreatePolynomial(int coeff, int exp) {
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(Poly));

    newElement->coeff = coeff;
    newElement->exp = exp;
    newElement->next = NULL;

    return newElement;
}

int SortedInsert(Position Head, Position newElement) {
    Position temp1 = Head;
    Position temp2 = newElement;
    int n = 0;

    while (temp1->next != NULL && temp1->next->exp > temp2->exp)
        temp1 = temp1->next;

    if (temp1->next != NULL && temp1->next->exp == temp2->exp) {
        temp1->next->coeff += temp2->coeff;
        n = temp1->coeff;

        if (n == 0)
            RemoveZeroCoefficients(temp1);
    }
    else {
        CreatePolynomial(temp2->coeff, temp2->exp);
        InsertAfter(temp1, temp2);
    }

    return EXIT_SUCCESS;
}

int InsertAfter(Position P, Position newElement) {
    newElement->next = P->next;
    P->next = newElement;

    return EXIT_SUCCESS;
}

int AddPolynomials(Position Head1, Position Head2, Position HeadResult) {
    Position P1 = Head1->next;
    Position P2 = Head2->next;
    Position PResult = HeadResult;

    Position newElement = NULL;
    Position temp = NULL;

    while (P1 != NULL && P2 != NULL) {
        if (P1->exp > P2->exp) {
            newElement = CreatePolynomial(P1->coeff, P1->exp);
            if (!newElement)
                return EXIT_FAILURE;
            SortedInsert(PResult, newElement);
            P1 = P1->next;
        }
        else if (P1->exp == P2->exp) {
            newElement = CreatePolynomial(P1->coeff + P2->coeff, P1->exp);
            if (!newElement)
                return EXIT_FAILURE;
            SortedInsert(PResult, newElement);
            P1 = P1->next;
            P2 = P2->next;
        }
        else {
            newElement = CreatePolynomial(P2->coeff, P2->exp);
            if (!newElement)
                return EXIT_FAILURE;
            SortedInsert(PResult, newElement);
            P2 = P2->next;
        }
    }

    if (P1 != NULL)
        temp = P1;
    else
        temp = P2;

    while (temp != NULL) {
        newElement = CreatePolynomial(temp->coeff, temp->exp);
        if (!newElement)
            return EXIT_FAILURE;
        SortedInsert(PResult, newElement);
        temp = temp->next;
    }

    RemoveZeroCoefficients(PResult);

    return EXIT_SUCCESS;
}

int MultiplyPolynomials(Position Head1, Position Head2, Position HeadResult) {
    Position P1 = Head1->next;
    Position P2 = Head2->next;
    Position PResult = HeadResult;

    Position Start = P2;

    Position newElement = NULL;

    while (P1 != NULL) {
        P2 = Start;
        while (P2 != NULL) {
            newElement = CreatePolynomial(P1->coeff * P2->coeff, P1->exp + P2->exp);
            if (!newElement)
                return EXIT_FAILURE;

            SortedInsert(PResult, newElement);
            P2 = P2->next;
        }
        P1 = P1->next;
    }

    RemoveZeroCoefficients(PResult);

    return EXIT_SUCCESS;
}

int PrintPolynomial(Position P) {
    Position temp = P->next;

    while (temp) {
        printf("%dx^%d ", temp->coeff, temp->exp);
        temp = temp->next;
    }

    printf("\n");

    return EXIT_SUCCESS;
}

int FreeMemory(Position Head) {
    Position P = Head;
    Position temp = NULL;

    if (P->next != NULL) {
        while (P->next != NULL) {
            temp = P->next;
            P->next = temp->next;
            free(temp);
        }
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}
