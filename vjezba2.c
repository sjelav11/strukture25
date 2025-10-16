struct St* Position;

typedef struct St{
char name[50];
char surname[50];
int years;
Position next;
};

newPerson= //A-alokacija
  newPerson = {
  ...next=NULL;
  }
newPerson->next=q->next;
q->next=newPerson;
