#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item {
    char name[50];
    int quantity;
    float price;
    struct Item *next;
} Item;

typedef struct Bill {
    char date[11]; 
    Item *items;
    struct Bill *next;
} Bill;


Item* InsertItemSorted(Item *head, char *name, int quantity, float price) {
    Item *newNode = (Item*)malloc(sizeof(Item));
    strcpy(newNode->name, name);
    newNode->quantity = quantity;
    newNode->price = price;
    newNode->next = NULL;

    Item *prev = NULL, *curr = head;

    while (curr != NULL && strcmp(curr->name, name) < 0) {
        prev = curr;
        curr = curr->next;
    }

    newNode->next = curr;

    if (prev == NULL) return newNode;

    prev->next = newNode;
    return head;
}


Bill* InsertBillSorted(Bill *head, char *date) {
    Bill *newNode = (Bill*)malloc(sizeof(Bill));
    strcpy(newNode->date, date);
    newNode->items = NULL;
    newNode->next = NULL;

    Bill *prev = NULL, *curr = head;

    while (curr != NULL && strcmp(curr->date, date) < 0) {
        prev = curr;
        curr = curr->next;
    }

    newNode->next = curr;

    if (prev == NULL) return newNode;

    prev->next = newNode;
    return head;
}

/* učitavanje računa*/
Bill* LoadAllBills() {
    FILE *fp = fopen("racuni.txt", "r");
    if (!fp) {
        printf("Error: Cannot open racuni.txt!\n");
        return NULL;
    }

    Bill *head = NULL;
    char line[200], name[50], date[20];
    int quantity;
    float price;

    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) < 5) continue; // preskoči prazne linije

        // Ako linija izgleda kao datum -> novi račun
        if (strchr(line, '-') != NULL) {
            line[strcspn(line, "\n")] = '\0';
            strcpy(date, line);
            head = InsertBillSorted(head, date);


            Bill *curr = head;
            while (curr && strcmp(curr->date, date) != 0) curr = curr->next;

            // čitaj artikle dok ne naiđeš na prazan red ili novi datum
            while (fgets(line, sizeof(line), fp)) {
                if (strlen(line) < 3 || strchr(line, '-') != NULL) {
                    fseek(fp, -strlen(line), SEEK_CUR);
                    break;
                }

                sscanf(line, "%[^,],%d,%f", name, &quantity, &price);
                curr->items = InsertItemSorted(curr->items, name, quantity, price);
            }
        }
    }

    fclose(fp);
    return head;
}

int Query(Bill *head, char *itemName, char *startDate, char *endDate) {
    int totalQty = 0;
    float totalCost = 0;

    Bill *b = head;
    while (b) {
        if (strcmp(b->date, startDate) >= 0 && strcmp(b->date, endDate) <= 0) {
            Item *i = b->items;
            while (i) {
                if (strcmp(i->name, itemName) == 0) {
                    totalQty += i->quantity;
                    totalCost += i->quantity * i->price;
                }
                i = i->next;
            }
        }
        b = b->next;
    }
    printf("\n");
    printf("Artikl: %s\n", itemName);
    printf("Razdoblje: %s -> %s\n", startDate, endDate);
    printf("Ukupna količina: %d\n", totalQty);
    printf("Ukupno potrošeno: %.2f EUR\n", totalCost);

    return 0;
}


int main() {
    Bill *bills = LoadAllBills();
    char item[50], date1[20], date2[20];

    printf("Upisi naziv artikla: ");
    scanf("%s", item);

    printf("Pocetni datum (YYYY-MM-DD): ");
    scanf("%s", date1);

    printf("Zavrsni datum (YYYY-MM-DD): ");
    scanf("%s", date2);

    Query(bills, item, date1, date2);

    return 0;
}
