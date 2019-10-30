/* Izlaz iz programa se ne vrsi na Esc, vec na  broj ponudjen u korisnickom meniju */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struktura koja predstavlja cvor liste */
struct node_t {
  char contents;
  struct node_t *pnext;
};

/* Struktura koja predstavlja deskriptor liste i sadrzi pokazivac na pocetak liste */
struct list_descriptor_t {
  struct node_t *phead; // pokazivac na pocetak liste (glava liste)
};

typedef struct node_t NODE;
typedef struct list_descriptor_t LIST_DESCRIPTOR;

/* Inicijalizacija liste - postavljanje glave na NULL */
void initialize(LIST_DESCRIPTOR *);

/* Oslobadjanje svih elemenata u listi i postavljanje glave na NULL */
void destroy(LIST_DESCRIPTOR *);

/* Dodavanje elemenata na kraj liste */
LIST_DESCRIPTOR *push_back(LIST_DESCRIPTOR *, char);  // dodaje element na kraj liste

/* Ispis elemenata liste */
void print_list(NODE *);

/* Trazenje elementa u listi */
NODE *find(LIST_DESCRIPTOR *, char); // proverava da li se karakter koji se dodaje vec nalazi u listi

/* Sortirano dodavanje elementa u listu */
LIST_DESCRIPTOR *add_sorted(LIST_DESCRIPTOR *, char); // dodaje karakter u listu, sortirajuci je 

/* Modifikacija elementa u listi */
LIST_DESCRIPTOR *modify(LIST_DESCRIPTOR *, char, char); // modifikuje karakter u listi

/* Brisanje elementa iz liste */
int delete(LIST_DESCRIPTOR *, char); // pomocna funkcija za proveru da li novi karakter vec nalazi u listi prilikom modifikacije

int main() {
  LIST_DESCRIPTOR descriptor;
	char c, c1;
	int izbor;
  
  do {
		printf("1. Inicijalizacija liste\n");
		printf("2. Dodavanje elementa na kraj\n");
		printf("3. Sortirano dodavanje elementa\n");
		printf("4. Ispis liste\n");
		printf("5. Izmena elementa\n");
		printf("6. Brisanje liste\n");
		printf("7. Izlazak iz programa\n");
	 	printf("Izabrati opciju: ");
		scanf("%d", &izbor);
		      
		switch(izbor) {
		 case 1: initialize(&descriptor); break;
		 case 2: printf("Unesite karakter za dodavanje na kraj: ");
		 				 __fpurge(stdin);
		 				 scanf("%c", &c);
		 				 push_back(&descriptor, c);
		  			 break;
		 case 3: printf("Unesite karakter za sortirano dodavanje: ");
		 				 __fpurge(stdin); 
		 				 scanf("%c", &c);
		 				 add_sorted(&descriptor, c);
		 				 break;
		 case 4: print_list(descriptor.phead); break;
		 case 5: printf("Unesite karakter koji zelite da izmenite: ");
		 				 __fpurge(stdin);
		 				 scanf("%c", &c);
		 				 printf("Unesite karakter kojim zelite da izmenite prethodno uneti: ");
		 				 __fpurge(stdin);
		 				 scanf("%c", &c1);
		 				 modify(&descriptor, c, c1);
		  			 break;
		 case 6: destroy(&descriptor); break;
		 case 7: break;
		 default: printf("Uneti vrednost izmedju 1 i 7\n");
		}
		printf("\n");
    
   } while(izbor != 7); 

  return 0;
}

void initialize(LIST_DESCRIPTOR *pdescriptor) {
  pdescriptor->phead = NULL;
}

NODE *find(LIST_DESCRIPTOR *pdescriptor, char contents){
	NODE *pcurr = pdescriptor->phead;
  while (pcurr != NULL) {
    if (pcurr->contents == contents){ 
    	return pcurr;
    }
    pcurr = pcurr->pnext;
  }
  return NULL;
}

NODE *new_node(char contents) {
  NODE *pnew = malloc(sizeof(NODE));
  pnew->contents = contents;
  pnew->pnext = NULL;
  return pnew;
}

LIST_DESCRIPTOR *push_back(LIST_DESCRIPTOR *pdescriptor, char contents) {
	if(find(pdescriptor, contents)){
		return;
	}
  NODE *pnew = new_node(contents);
  NODE *pcurr = pdescriptor->phead;
  if (pcurr == NULL) {
    pdescriptor->phead = pnew;
  } else {
    while (pcurr->pnext != NULL) {  // prolazak do poslednjeg elementa u listi
      pcurr = pcurr->pnext;
    }
    pcurr->pnext = pnew;
  }
  return pdescriptor;
}

void print_list(NODE *pcurr) {
	if(pcurr == NULL){ 
		printf("\n");
  	return;
  }
  if(pcurr != NULL){
  	printf("%c", pcurr->contents);
    if (pcurr->pnext != NULL) {
     printf(", ");
    }
  }
  print_list(pcurr->pnext); 
 
}

LIST_DESCRIPTOR *add_sorted(LIST_DESCRIPTOR *pdescriptor, char contents){
  if (find(pdescriptor, contents)){
		return;
	}
  NODE *pnew = new_node(contents);
  NODE *pcurr = pdescriptor->phead;
  if (pcurr == NULL) {
    pdescriptor->phead = pnew; // ukoliko je lista prazna, dodajemo na pocetak
    return pdescriptor;
  }
  if (pdescriptor->phead->contents > contents){ // ukoliko treba da dodamo na pocetak
    pnew->pnext = pdescriptor->phead;
 	  pdescriptor->phead = pnew;
 	  return pdescriptor;
  	}
	while (pcurr->pnext != NULL){ 
	  if (pnew->contents < pcurr->pnext->contents){ // proveravama da li je ascii kod novododatog cvora manji od trenutnog
		  pnew->pnext = pcurr->pnext; // ako jeste, umecemo ga u listu 
			pcurr->pnext = pnew; 
			break;
			}
			pcurr = pcurr->pnext;
  	}
  	if (pcurr->pnext == NULL){ // ukoliko treba da dodamo na kraj liste novi cvor
				pcurr->pnext = pnew;
			}
  
	return pdescriptor;
}

LIST_DESCRIPTOR *modify(LIST_DESCRIPTOR *pdescriptor, char oldValue, char newValue){
  NODE *pnode = find(pdescriptor, oldValue); 
  if (pnode && (delete(pdescriptor, oldValue) == 1)){
    add_sorted(pdescriptor, newValue);
  } 
	return pdescriptor;
}

int delete(LIST_DESCRIPTOR *pdescriptor, char contents){
  NODE *pcurr = pdescriptor->phead;
  if (pcurr == NULL){ // lista je prazna
    return 0;
   }
  if (pcurr->contents == contents){ // ako brisemo sa pocetka
        pdescriptor->phead = pcurr->pnext;
        free(pcurr);
        return 1;
    }
  pcurr = pdescriptor->phead;
  while (pcurr->pnext != NULL){ 
	  if (contents == pcurr->pnext->contents){ // brisemo iz sredine ili sa kraja
		 NODE *ptemp = pcurr->pnext;
     pcurr->pnext = pcurr->pnext->pnext;
     free(ptemp);
     return 1;
		}
		pcurr = pcurr->pnext;
  }
    return 0;
}

void destroy(LIST_DESCRIPTOR *pdescriptor) {
  NODE *ptemp = NULL;
  while (pdescriptor->phead) {
    ptemp = pdescriptor->phead;
    pdescriptor->phead = ptemp->pnext;
    free(ptemp);
  }
  initialize(pdescriptor);
}

