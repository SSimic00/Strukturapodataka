#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct cities;
typedef struct cities* Position;
typedef struct cities {
    char name[50];
    Position next;
    int residents;
} Cities;


struct countries;
typedef struct countries* Tree;
typedef struct countries {
    char name[50];
    Tree left;
    Tree right;
    Position city;
} Countries;
int Search(Tree root, char* countryName, int population);
void AddCity(Position cityList, char* fileName);
Tree AddCountry(Tree root, char* countryName, char* fileName);
Tree SearchCountry(Tree root, char* countryName);
void PrintCountries(Tree root);
void PrintCities(Position cityList);
int main()
{
    Tree root = NULL;
    char countryName[50];
    char fileName[50];
    FILE* file = NULL;

    char cityName[50];
    int population = 0;
    file = fopen("Countries.txt", "r");
    if (file == NULL)
    {
        perror("ERROR: Unable to open countries.txt");
        return -1;
    }
    while (fscanf(file, "%s %s", countryName, fileName) == 2)
    {
    root = AddCountry(root, countryName, fileName);
    }

    if (ferror(file))
    {
        perror("ERROR: Errorreading from countries.txt");
    }
    fclose(file);
    printf("COUNTRIES:\n");
    PrintCountries(root);

    printf("\nEnter the name of the wanted country: ");
    scanf("%s", countryName);

    printf("Enter the minimum population: ");
    scanf("%d", &population);

    Search(root, countryName, population);

    return 0;
}

Tree AddCountry(Tree root, char* countryName, char* fileName)
{
    Tree newNode = (Tree)malloc(sizeof(Countries));
    if (newNode == NULL)
    {
        printf("Memory allocation unsuccessful.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->name, countryName);
    newNode->city = (Position)malloc(sizeof(Cities));
    if (newNode->city == NULL)
    {
        printf("Memory allocation unsuccessful.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->city->name, "");
    newNode->city->residents = 0;
    newNode->city->next = NULL;

    AddCity(newNode->city, fileName);

    newNode->left = NULL;
    newNode->right = NULL;

    if (root == NULL)
    {
        return newNode;
    }

    Tree current = root;
    Tree parent = NULL;

    while (current != NULL)
    {
        parent = current;
        if (strcmp(countryName, current->name) < 0)
        {
            current = current->left;
        }
        else if (strcmp(countryName, current->name) > 0)
        {
            current = current->right;
        }
        else
        {
            // Duplicate country name
            free(newNode->city);
            free(newNode);
            return root;
        }
    }

    if (strcmp(countryName, parent->name) < 0)
    {
        parent->left = newNode;
    }
    else
    {
        parent->right = newNode;
    }

    return root;
}

void AddCity(Position cityList, char* fileName)
{
    char cityName[50];
    int population = 0;

    Position current = cityList;
    Position prev = cityList;

    FILE* file = fopen(fileName, "r");
    if (file == NULL)
    {
        printf("ERROR: File %s cannot be opened.\n", fileName);
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%s %d", cityName, &population) == 2)
    {
        Position newNode = (Position)malloc(sizeof(Cities));
        if (newNode == NULL)
        {
            printf("Memory allocation unsuccessful.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(newNode->name, cityName);
        newNode->residents = population;

        while (current != NULL)
        {
            if (current->next == NULL || newNode->residents < current->residents ||
                (newNode->residents == current->residents && strcmp(newNode->name, current->name) < 0))
            {
                newNode->next = current->next;
                current->next = newNode;
                break;
            }

            prev = current;
            current = current->next;
        }

        prev = cityList;
        current = cityList;
    }

    fclose(file);
}

Tree SearchCountry(Tree root, char* countryName)
{
    if (root == NULL || strcmp(root->name, countryName) == 0)
    {
        return root;
    }

    if (strcmp(countryName, root->name) < 0)
    {
        return SearchCountry(root->left, countryName);
    }
    else
    {
        return SearchCountry(root->right, countryName);
    }
}

int Search(Tree S, char* NameOfCountry, int residents)
{
    Position q = NULL;
    Tree R = NULL;

    printf("Cities with wanted value of population:\n");

    R = SearchCountry(S, NameOfCountry);

    if (R == NULL)
    {
        printf("\nCountry doesn't exist.\n");
        return 0;
    }
    q = R->city->next;

    if (q == NULL)
    {
        printf("Country has no city.\n");
        return 0;
    }

    while (q != NULL)
    {
        if (q->residents >= residents)
            printf("\t%s %d\n", q->name, q->residents);
        q = q->next;
    }

    return 0;
}


void PrintCities(Position cityList)
{
    while (cityList != NULL)
    {
        printf("%s - %d residents\n", cityList->name, cityList->residents);
        cityList = cityList->next;
    }
}

void PrintCountries(Tree root)
{
    if (root == NULL)
    {
        return;
    }

    PrintCountries(root->left);
    printf("\n%s:\n", root->name);
    PrintCities(root->city->next);
    PrintCountries(root->right);
}
