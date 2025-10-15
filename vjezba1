#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char ime[50];
    char prezime[50];
    int bodovi;
} Student;

int brojRedakaUDatoteci(FILE* fp) {
    int br = 0;
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        br++;
    rewind(fp);
    return br;
}//funkcija za čitanje koliko ima redaka u datoteci

int main() {
  FILE* fp = fopen("studenti1.txt", "r");
    if (!fp) {
        printf("Greška: datoteka se ne može otvoriti!\n");
        return 1;
    }

    int brojStudenata = brojRedakaUDatoteci(fp); //broj studenata jednak je broju redaka u datoteci
    printf("Broj studenata u datoteci: %d\n", brojStudenata);

    if (brojStudenata <= 0) {
        printf("Datoteka je prazna!\n");
        fclose(fp);
        return 1;
    }

    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));//dinamička alokacija za niz struktura studenata
    if (studenti == NULL) {
        printf("Greška: neuspjela alokacija memorije!\n");
        fclose(fp);
        return 1;
    }

    int i;
    for (i = 0; i < brojStudenata; i++) {
        if (fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi) != 3) {/*/ukoliko nema 3 elementa(2 stringa i broja pojavit će se greška prii čitanju retka*/
            printf("Greška pri čitanju retka %d!\n", i + 1);
            break;
        }
    }

    fclose(fp);

    printf("\n%-10s %-10s %-10s %-10s\n", "Ime", "Prezime", "Apsolutno", "Relativno (%)");

    for (i = 0; i < brojStudenata; i++) {
      double relativni = (studenti[i].bodovi / 50.0) * 100.0;//max broj bodova na ispitu je 50
        printf("%-10s %-10s %-10d %-10.2f\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativni);
    }

    free(studenti);//oslobađanje dinamički alocirane memorije
    return 0;
}
