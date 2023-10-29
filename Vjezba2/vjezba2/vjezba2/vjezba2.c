#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE (256)
#define ERROR (-1)
#define EXIT_SUCCESS 0
struct _person;
typedef struct _person* Position;

typedef struct _person {
    char name[MAX_LINE];
    char surname[MAX_LINE];
    int year;

    Position next;
} person;

Position Input(char*, char*, int);
Position Findbysurname(Position, char*);
Position findprevious(char*, Position);
int unosP(Position, Position);
int unosK(Position, Position);
int ispis(Position);
int delete(char*, Position);
int unosIza(char*, Position, Position);
int unosIspred(char*, Position, Position);
int unosDatoteka(char*, Position);
int ispisDatoteka(char*);
//a

int main() {
    person head = {
        .name = {0},
        .surname = {0},
        .year = 0,
        .next = NULL
    };
    Position P = NULL;
    Position Q = NULL;
    Position tempp = NULL;
    char name[MAX_LINE] = { 0 };
    char surname[MAX_LINE] = { 0 };
    char temp[MAX_LINE] = { 0 };
    char filename[MAX_LINE] = { 0 };
    int year = 0, poz = 0, br = 0, izbor = 0, status = 0;

    printf("How many people do you want to enter? ");
    scanf("%d", &br);
    printf("\n");

    for (int i = 0; i < br; i++)
    {
        printf("Where do you want to place the person?\n");
        printf("\t1 - top\n\t2 - bottom\n");
        scanf("%d", &izbor);

        printf("\n");
        printf("Name: ");
        scanf(" %s", name);
        printf("Surname: ");
        scanf(" %s", surname);
        printf("Year: ");
        scanf("%d", &year);
        printf("\n");

        P = Input(name, surname, year);

        switch (izbor) {
        case 1:
        {
            unosP(&head, P);
            break;
        }

        case 2:
        {
            unosK(&head, P);
            break;
        }
        }

    }
    ispis(&head);

    printf("\nEnter the surname before which you want to enter the element: ");
    scanf(" %s", temp);

    printf("\n");
    printf("Name: ");
    scanf(" %s", name);
    printf("Surname: ");
    scanf(" %s", surname);
    printf("Year: ");
    scanf("%d", &year);
    printf("\n");

    P = Input(name, surname, year);
    status = unosIspred(temp, &head, P);
    if (status != EXIT_SUCCESS)
        return EXIT_SUCCESS;
    ispis(&head);

    printf("\nEnter the surname after which you want to enter the element: ");
    scanf(" %s", temp);

    printf("\n");
    printf("Name: ");
    scanf(" %s", name);
    printf("Surname: ");
    scanf(" %s", surname);
    printf("Year: ");
    scanf("%d", &year);
    printf("\n");

    P = Input(name, surname, year);
    unosIza(temp, &head, P);
    ispis(&head);

    printf("\nWrite the surname you want to find: ");
    scanf(" %s", temp);

    Q = Findbysurname(&head, temp);
    printf("%s %s %d\n", Q->name, Q->surname, Q->year);

    printf("\nWrite the surname you want to delete: ");
    scanf(" %s", temp);
    delete(temp, &head);
    ispis(&head);
    printf("\n");

    printf("Name of the file: ");
    scanf(" %s", filename);
    printf("\n");

    unosDatoteka(filename, &head);
    ispisDatoteka(filename);

    ispis(&head);

    while (head.next != NULL) {
        tempp = head.next;
        head.next = head.next->next;
        free(tempp);
    }

    return EXIT_SUCCESS;
}

Position Input(char* name, char* surname, int year) {
    Position p = NULL;
    p = (Position)malloc(sizeof(person));
    strcpy(p->name, name);
    strcpy(p->surname, surname);
    p->year = year;
    p->next = NULL;
    if (!p) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    return p;
}

int unosP(Position head, Position p) {
    p->next = head->next;
    head->next = p;
    return EXIT_SUCCESS;
}

int ispis(Position head) {
    Position p = NULL;
    Position q = NULL;
    p = head->next;
    int i = 0;
    while (p != NULL) {
        q = p->next;
        printf("%d. %s %s %d\n", i + 1, p->name, p->surname, p->year);
        p = q;
        i++;
    }
    return EXIT_SUCCESS;
}

int unosK(Position head, Position p) {
    while (head->next != NULL) {
        head = head->next;
    }
    p->next = head->next;
    head->next = p;
    return EXIT_SUCCESS;
}

Position Findbysurname(Position head, char* prez) {
    Position q = NULL;
    q = head->next;
    while (q != NULL) {
        if (strcmp(q->surname, prez) == 0) {
            return q;
        }
        q = q->next;
    }
    printf("Search failed.\n");
    return ERROR;
}

Position findprevious(char* surname, Position head) {
    Position p = NULL, q = NULL;
    p = head->next;
    while (p != NULL && strcmp(p->surname, surname) != 0) {
        q = p;
        p = p->next;
    }
    if (p == NULL)
        return NULL;
    return q;
}

int delete(char* prez, Position head) {
    Position temp1 = head;
    while (temp1 != NULL) {
        if (strcmp(temp1->next->surname, prez) == 0) {
            Position temp2 = temp1->next;
            temp1->next = temp2->next;
            free(temp2);
            return EXIT_SUCCESS;
        }
        temp1 = temp1->next;
    }
    printf("Search failed.\n");
    return ERROR;
}

int unosIza(char* prez, Position head, Position p) {
    Position q = NULL;
    q = head->next;
    while (q != NULL) {
        if (strcmp(q->surname, prez) == 0) {
            p->next = q->next;
            q->next = p;
            return EXIT_SUCCESS;
        }
        q = q->next;
    }
    printf("Search failed.\n");
    return ERROR;
}

int unosIspred(char* surname, Position head, Position p) {
    Position prev = NULL;
    prev = findprevious(surname, head);
    p->next = prev->next;
    prev->next = p;
    return EXIT_SUCCESS;
}

int unosDatoteka(char* filename, Position head) {
    FILE* fp = NULL;
    fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening the file!\n");
        return ERROR;
    }
    head = head->next;
    while (head != NULL) {
        fprintf(fp, "\n%s %s %d", head->name, head->surname, head->year);
        head = head->next;
    }
    fclose(fp);
    return EXIT_SUCCESS;
}

int ispisDatoteka(char* filename) {
    char name[MAX_LINE] = { 0 };
    char surname[MAX_LINE] = { 0 };
    int year = 0;
    FILE* fp = NULL;
    int i = 1;
    fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening the file!\n");
        return ERROR;
    }
   
    while (fscanf(fp, "%s %s %d", name, surname, &year) == 3) {
        printf("\n%d. %s %s %d\n", i, name, surname, year);
        fflush(stdout);
        i++;
    }
    fclose(fp);
    return EXIT_SUCCESS;
}
