#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11
#define MAX_NAME_LENGTH 50

struct cities;
typedef struct cities* Position;
typedef struct cities {
    char name[MAX_NAME_LENGTH];
    Position next;
    int residents;
} Cities;

struct countries;
typedef struct countries* Tree;
typedef struct countries {
    char name[MAX_NAME_LENGTH];
    Tree left;
    Tree right;
    Position city;
} Countries;

struct hash_table_node;
typedef struct hash_table_node* HashTableNodePtr;
typedef HashTableNodePtr HashTable[TABLE_SIZE];

struct hash_table_node {
    Tree country;
    HashTableNodePtr next;
};

int hash_function(char* key);
void insert_into_hash_table(HashTable table, char* countryName, char* fileName);
void AddCity(Position cityList, char* fileName);

Tree AddCountry(Tree root, char* countryName, char* fileName);
Tree SearchCountry(Tree root, char* countryName);
void PrintCountriesList(Position cityList);
void PrintCountriesTree(Tree root);
void PrintCities(Position cityList);

int main() {
    HashTable hashTable;
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    char countryName[MAX_NAME_LENGTH];
    char fileName[MAX_NAME_LENGTH];
    FILE* file = fopen("countries.txt", "r");

    if (file == NULL) {
        perror("ERROR: Unable to open drzave.txt");
        return -1;
    }

    while (fscanf(file, "%s %s", countryName, fileName) == 2) {
        insert_into_hash_table(hashTable, countryName, fileName);
    }

    if (ferror(file)) {
        perror("ERROR: Error reading from drzave.txt");
    }

    fclose(file);

    printf("Sorted Linked List of Countries:\n");
    // Print countries and cities from the hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashTableNodePtr current = hashTable[i];
        while (current != NULL) {
            printf("\n%s:\n", current->country->name);
            PrintCities(current->country->city->next);
            current = current->next;
        }
    }

    printf("\n\nSorted Tree of Countries:\n");
    // Print countries and cities from the hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashTableNodePtr current = hashTable[i];
        while (current != NULL) {
            PrintCities(current->country->city->next);
            current = current->next;
        }
    }

    return 0;
}

void insert_into_hash_table(HashTable table, char* countryName, char* fileName) {
    int index = hash_function(countryName);

    Tree root = table[index] != NULL ? table[index]->country : NULL;

    Tree newCountry = AddCountry(root, countryName, fileName);

    HashTableNodePtr newNode = (HashTableNodePtr)malloc(sizeof(struct hash_table_node));
    if (newNode == NULL) {
        printf("Memory allocation unsuccessful.\n");
        exit(EXIT_FAILURE);
    }

    newNode->country = newCountry;
    newNode->next = table[index];
    table[index] = newNode;
}

int hash_function(char* key) {
    int sum = 0;
    for (int i = 0; i < 5 && key[i] != '\0'; i++) {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

Tree AddCountry(Tree root, char* countryName, char* fileName) {
    Tree newNode = (Tree)malloc(sizeof(Countries));
    if (newNode == NULL) {
        printf("Memory allocation unsuccessful.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->name, countryName);
    newNode->city = (Position)malloc(sizeof(Cities));
    if (newNode->city == NULL) {
        printf("Memory allocation unsuccessful.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->city->name, "");
    newNode->city->residents = 0;
    newNode->city->next = NULL;

    AddCity(newNode->city, fileName);

    newNode->left = NULL;
    newNode->right = NULL;

    if (root == NULL) {
        return newNode;
    }

    Tree current = root;
    Tree parent = NULL;

    while (current != NULL) {
        parent = current;
        if (strcmp(countryName, current->name) < 0) {
            current = current->left;
        }
        else if (strcmp(countryName, current->name) > 0) {
            current = current->right;
        }
        else {
            // Duplicate country name
            free(newNode->city);
            free(newNode);
            return root;
        }
    }

    if (strcmp(countryName, parent->name) < 0) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    return root;
}

void AddCity(Position cityList, char* fileName) {
    char cityName[MAX_NAME_LENGTH];
    int population = 0;

    Position current = cityList;
    Position prev = cityList;

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("ERROR: File %s cannot be opened.\n", fileName);
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%49s %d", cityName, &population) == 2) {
        Position newNode = (Position)malloc(sizeof(Cities));
        if (newNode == NULL) {
            printf("Memory allocation unsuccessful.\n");
            exit(EXIT_FAILURE);
        }

        strcpy(newNode->name, cityName);
        newNode->residents = population;

        while (current != NULL) {
            if (current->next == NULL || newNode->residents < current->residents ||
                (newNode->residents == current->residents && strcmp(newNode->name, current->name) < 0)) {
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

void PrintCities(Position cityList) {
    while (cityList != NULL) {
        printf("%s - %d residents\n", cityList->name, cityList->residents);
        cityList = cityList->next;
    }
}

