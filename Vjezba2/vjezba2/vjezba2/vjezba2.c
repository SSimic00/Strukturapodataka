#define _CRT_SECURE_NO_WARNINGS
#define FILE_NOT_OPENED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100



// dedinicija strukture osoba

typedef struct Osoba {
	char Ime[MAX_SIZE];
	char Prezime[MAX_SIZE];
	int Godrod;
    struct Osoba* next;
}Osoba;

//funkcija za dodavanje novih osoba na pocetak
Osoba* dodajpocetak(Osoba* Head, char* ime, char* prezime, int Godrod) {
    Osoba* novaOsoba = (Osoba*)malloc(sizeof(Osoba));
    if (novaOsoba == NULL) {
        printf("Greška alokacije memorije\n");
        return -1;
    }

    strcpy(novaOsoba->Ime, ime);
    strcpy(novaOsoba->Prezime, prezime);
    novaOsoba->Godrod = Godrod;
    novaOsoba->next = Head;

    return novaOsoba;
}
//funkcija za dodavanje osoba na kraju liste
Osoba* dodajkraj(Osoba* head, char* ime, char* prezime, int godrod) 
{
    Osoba* novaOsoba = (Osoba*)malloc(sizeof(Osoba));
    if (novaOsoba == NULL){
        printf("Greška alokacie memorije za novu osobu\n");
        return -1;
    }

    strcpy(novaOsoba->Ime, ime);
    strcpy(novaOsoba->Prezime, prezime);
    novaOsoba->Godrod = godrod;
    novaOsoba->next = NULL;

    if (head == NULL) {
        return novaOsoba;
    }

    Osoba* temp = head;
    while (temp->next != NULL) {
        temp= temp->next;
    }

    temp->next = novaOsoba;

    return head;
}
//funckija za ispis osoba 
void ispis(Osoba* head) {
    Osoba* temp = head;
    printf("Lista osoba:\n");
    while (temp != NULL) {
        printf("Ime: %s,Prezime: %s,Godina: %d\n", temp->Ime, temp->Prezime, temp->Godrod);
        temp = temp->next;
    }
}
int main() {
    Osoba* head = NULL; // Inicijalizirajte glavu liste na NULL

    // Dodajemo osobe na početak liste
    head = dodajpocetak(head, "Petar", "lulic", 2023);
    head = dodajpocetak(head, "Petar", "Pan", 1885);
    head = dodajpocetak(head, "Duje", "Pan", 2100);

    // Dodajemo osobe na kraj liste
   head = dodajkraj(head, "Luka", "Lukic", 1907);
   head = dodajkraj(head, "Marko", "Markic", 1935);
   head = dodajkraj(head, "Ana", "Lukic", 1988);
   
   ispis(head);

    return 0;
}