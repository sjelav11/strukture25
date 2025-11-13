#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
(VEZANA LISTA I BUBBLE SORT MAKNUT)
typedef struct {
    float coef;
    int exp;
} Term;

void sortiraj(Term *p, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].exp < p[j].exp) {
                Term tmp = p[i];
                p[i] = p[j];
                p[j] = tmp;
            }
}

Term *ucitajPolinom(FILE *f, int *n) {
    char linija[256];
    *n = 0;
    Term *p = NULL;

    if (!fgets(linija, sizeof(linija), f))  // pročitaj jedan red (jedan polinom)
        return NULL;

    char *tok = strtok(linija, " \t\n");
    while (tok) {
        float c = atof(tok);
        tok = strtok(NULL, " \t\n");
        if (!tok) break; // nema eksponenta
        int e = atoi(tok);

        p = realloc(p, (*n + 1) * sizeof(Term));
        p[*n].coef = c;
        p[*n].exp = e;
        (*n)++;

        tok = strtok(NULL, " \t\n");
    }

    return p;
}

void ispisi(Term *p, int n) {
    for (int i = 0; i < n; i++) {
        if (fabs(p[i].coef) < 1e-6) continue;
        if (i > 0 && p[i].coef > 0) printf(" + ");
        else if (p[i].coef < 0) printf(" - ");
        printf("%.f*x^%d", fabs(p[i].coef), p[i].exp);
    }
    printf("\n");
}

Term *zbroji(Term *p1, int n1, Term *p2, int n2, int *nz) {
    Term *rez = NULL;
    int i = 0, j = 0;
    *nz = 0;

    while (i < n1 && j < n2) {
        Term novi;
        if (p1[i].exp > p2[j].exp) novi = p1[i++];
        else if (p1[i].exp < p2[j].exp) novi = p2[j++];
        else {
            novi.exp = p1[i].exp;
            novi.coef = p1[i].coef + p2[j].coef;
            i++; j++;
        }
        rez = realloc(rez, (*nz + 1) * sizeof(Term));
        rez[*nz] = novi;
        (*nz)++;
    }

    while (i < n1) {
        rez = realloc(rez, (*nz + 1) * sizeof(Term));
        rez[*nz] = p1[i++];
        (*nz)++;
    }

    while (j < n2) {
        rez = realloc(rez, (*nz + 1) * sizeof(Term));
        rez[*nz] = p2[j++];
        (*nz)++;
    }

    return rez;
}

Term *pomnozi(Term *p1, int n1, Term *p2, int n2, int *nm) {
    Term *rez = NULL;
    *nm = 0;

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            int exp = p1[i].exp + p2[j].exp;
            float coef = p1[i].coef * p2[j].coef;

            int found = 0;
            for (int k = 0; k < *nm; k++) {
                if (rez[k].exp == exp) {
                    rez[k].coef += coef;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                rez = realloc(rez, (*nm + 1) * sizeof(Term));
                rez[*nm].exp = exp;
                rez[*nm].coef = coef;
                (*nm)++;
            }
        }
    }

    sortiraj(rez, *nm);
    return rez;
}

int main() {
    FILE *f = fopen("polinomi.txt", "r");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return 1;
    }

    int n1, n2, nz, nm;
    Term *p1 = ucitajPolinom(f, &n1);
    Term *p2 = ucitajPolinom(f, &n2);
    fclose(f);

    if (!p1 || !p2) {
        printf("Greska: ne mogu ucitati polinome!\n");
        return 1;
    }

    sortiraj(p1, n1);
    sortiraj(p2, n2);

    printf("P1(x) = "); ispisi(p1,n1);
    printf("P2(x) = "); ispisi(p2, n2);

    Term *zbroj = zbroji(p1, n1, p2, n2, &nz);
    Term *umnozak = pomnozi(p1, n1, p2, n2, &nm);

    printf("\nZbroj: "); ispisi(zbroj, nz);
    printf("Umnozak: "); ispisi(umnozak, nm);

    free(p1);
    free(p2);
    free(zbroj);
    free(umnozak);
    return 0;
}
