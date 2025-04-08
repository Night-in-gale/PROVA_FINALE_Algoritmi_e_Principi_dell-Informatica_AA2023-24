#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CODAVUOTA NULL
#define MAX_LUNGHEZZA 30
#define LUNGHEZZA_HASH 4096

//struct per il magazzino dei rifornimenti
typedef struct lotto lotto;
struct lotto {

    int	quantita;
    int	datadiscadenza;
    lotto* next;
};

typedef struct ingrediente ingrediente;
struct ingrediente {
    char nome_ingrediente[MAX_LUNGHEZZA];
    int	qtot;
    lotto* listalotti;
    ingrediente* next;
    ingrediente* prev;
};

typedef struct hashmagazzino hashmagazzino;
struct hashmagazzino {
    ingrediente* next;
};

//struct per le ricette
typedef struct ingrediente_ricetta ingrediente_ricetta;
struct ingrediente_ricetta {
    int	quantita;
    ingrediente* nodo_ingrediente;
    ingrediente_ricetta* next;
};

typedef struct ricetta ricetta;
struct ricetta {
    char nome_ricetta[MAX_LUNGHEZZA];
    int	peso_ricetta;
    ingrediente_ricetta* ingredienti;
    ricetta* next;
    ricetta* prev;
};

typedef struct hashricette hashricette;
struct hashricette {
    ricetta* next;
};


//boolean
typedef enum statoordine statoordine;
enum statoordine {
    inattesa = 0,
    pronto = 1,
    rifiutato = 2
};

typedef struct ordine ordine;
struct ordine {
    char nomeordine[MAX_LUNGHEZZA];
    int	quantita;
    int	arrivo; //istante ordine
    int	peso;
    ricetta* ricettaordine;
    ordine* next;
    ordine* prev;
};

//strutture dati
hashricette ricettario[LUNGHEZZA_HASH];
hashmagazzino magazzino[LUNGHEZZA_HASH];
ordine* listapronti;
ordine* listainattesa;
ordine* codapronti;
ordine* codainattesa;

ingrediente_ricetta* DistruggiListaIngredienti (ingrediente_ricetta* testa) {
    if (testa == NULL) {
        return NULL;
    }
    else {
        ingrediente_ricetta* temp;
        temp = testa->next;
        while (temp != NULL) {    
            free (testa);
            testa = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            free (testa);		
            testa = NULL;
            return testa;
        }
    }
    return testa;
}

ricetta* DistruggiListaRicette (ricetta* testa) {
    if (testa == NULL) {
        return NULL;
    }
    else {
        ricetta* temp;
        temp = testa->next;
        while (temp != NULL) {
            free (testa);
            testa = temp;
            temp = temp->next;
        }
        if (temp == NULL) { 
            free (testa);
            testa = NULL;
            return testa;
        }
    }
    return testa;
}

ingrediente* DistruggiListaMagazzino (ingrediente* testa) {
    if (testa == NULL) {
        return NULL;
    }
    else {
        ingrediente* temp;
        temp = testa->next;
        while (temp != NULL) { 
            free (testa);
            testa = temp;
            temp = temp->next;
        }
        if (temp == NULL) {  
            free (testa);
            testa = NULL;
            return testa;
        }
    }
    return testa;
}

/*liste ordini*/

ordine* DistruggiListaOrdini (ordine* testa) {
    if (testa == NULL) {
        return NULL;
    }
    else {
        ordine* temp;
        temp = testa->next;
        while (temp != NULL) {
            free (testa);
            testa = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            free (testa);
            testa = NULL;
            return testa;
        }
    }
    return testa;
}

lotto* DistruggiListaLotti (lotto* testa) {
    if (testa == NULL) {
        return NULL;
    }
    else {
        lotto* temp;
        temp = testa->next;
        while (temp != NULL) { 
            free (testa);
            testa = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            free (testa);
            testa = NULL;
            return testa;
        }
    }
    return testa;
}

/*la stringa con valore ASCII minore possibile con i caratteri usabili è "0", con valore ASCII 48*/

int CalcolaHash (char* stringa) {
    int h = 0;
    int i = 0;
    
    while (stringa[i] != '\0') {
        h = h + stringa[i];
        i++;
    }
    return h - 48;
}

void DistruggiTutto (hashricette* ricettario, hashmagazzino* magazzino, ordine* listapronti, ordine* listainattesa) {
    int h = 0;
    while (h < LUNGHEZZA_HASH) {
        if (ricettario[h].next == NULL) {
            h++;
        }
        else {
            ricetta* y;
            y = ricettario[h].next;
            while (y != NULL) {
                ingrediente_ricetta* x;
                x = y->ingredienti;
                x = DistruggiListaIngredienti (x);
                y = y->next;
            }
            ricettario[h].next = DistruggiListaRicette (ricettario[h].next);
            h++;
        }
    }

    while (h < LUNGHEZZA_HASH) {
        if (magazzino[h].next == NULL) {
            h++;
        }
        else {
            ingrediente* y;
            y = magazzino[h].next;
            while (y != NULL) {
                lotto* x;
                x = y->listalotti;
                x = DistruggiListaLotti (x);
                y = y->next;
            }
            magazzino[h].next = DistruggiListaMagazzino (magazzino[h].next);
            h++;
        }
    }

    listapronti = DistruggiListaOrdini (listapronti);
    listainattesa = DistruggiListaOrdini (listainattesa);

    return;
}

/*aggiungi_ricetta*/

ricetta* CreaNodoRicetta (char* nomericetta) {
    ricetta* nodo;
    nodo = malloc(sizeof(ricetta));
    strcpy (nodo->nome_ricetta, nomericetta);
    nodo->peso_ricetta = 0;
    nodo->ingredienti = NULL;
    nodo->next = NULL;
    nodo->prev = NULL;
    return nodo;
}

ingrediente* CreaNodoIngredienteMagazzino (char* nome) {
    ingrediente* nodo;
    nodo = malloc(sizeof(ingrediente));
    strcpy (nodo->nome_ingrediente, nome);
    nodo->qtot = 0;
    nodo->listalotti = NULL;
    nodo->next = NULL;
    nodo->prev = NULL;
    return nodo;
}

ingrediente_ricetta* CreaNodoIngredienteRicetta (int q, ingrediente* ingredientemagazzino) {
    ingrediente_ricetta* nodo;
    nodo = malloc(sizeof(ingrediente_ricetta));
    nodo->quantita = q;
    nodo->nodo_ingrediente = ingredientemagazzino;
    nodo->next = NULL;
    return nodo;
}

ingrediente_ricetta* AggiungiIngrediente (ingrediente_ricetta* lista, int q, ingrediente* ingredientemagazzino) {
    ingrediente_ricetta* ingrediente;
    ingrediente_ricetta* temp; 
    ingrediente = CreaNodoIngredienteRicetta (q, ingredientemagazzino);
    temp = lista;
    lista = ingrediente;
    ingrediente->next = temp;
    return lista;
}

void CreaListaIngredienti (ricetta* ricetta, FILE* fp) {
    char c;
    char nomeingr[MAX_LUNGHEZZA];
    int quantita;
    int h;
    int i;
    ingrediente* y;
    
    c = fgetc(fp);
    while (c != '\n' && c != EOF) {
        if (fscanf (fp, "%s", nomeingr) == 1) {
            if (fscanf (fp, "%d", &quantita) == 1) {
                h = CalcolaHash (nomeingr);
                //l'ingrediente della ricetta non è presente in magazzino, va aggiunto
                if (magazzino[h].next == NULL) {
                    magazzino[h].next = CreaNodoIngredienteMagazzino (nomeingr);
                    ricetta->ingredienti = AggiungiIngrediente (ricetta->ingredienti, quantita, magazzino[h].next);
                }
                else {
                    y = magazzino[h].next;
                    i = strcmp (nomeingr, y->nome_ingrediente);
                    while (i != 0 && y->next != NULL) {
                        y = y->next;
                        i = strcmp (nomeingr, y->nome_ingrediente);
                    }
                    //l'ingrediente della ricetta è già presente in magazzino all'indirizzo y
                    if (i == 0) {
                        ricetta->ingredienti = AggiungiIngrediente (ricetta->ingredienti, quantita, y);
                    }
                    //l'ingrediente della ricetta non è presente in magazzino, va aggiunto
                    else if (y->next == NULL) {
                        y->next = CreaNodoIngredienteMagazzino (nomeingr);
                        y->next->prev = y;
                        ricetta->ingredienti = AggiungiIngrediente (ricetta->ingredienti, quantita, y->next);
                    }
                }
                ricetta->peso_ricetta = ricetta->peso_ricetta + quantita;
                c = fgetc(fp);
            }
            else {
	  			printf ("\nerrore di lettura del file");
            }
        }
        else {
  			printf ("\nerrore di lettura del file");
        }
    }
    return;
}

void AggiungiRicetta (hashricette* ricettario, FILE* fp) {
    char nomericetta[MAX_LUNGHEZZA];
    
    if (fscanf (fp, "%s", nomericetta) == 1) {
        int h;
        h = CalcolaHash (nomericetta);
        if (ricettario[h].next == NULL) {
            ricettario[h].next = CreaNodoRicetta (nomericetta);
            CreaListaIngredienti (ricettario[h].next, fp);
            printf ("aggiunta\n");
            return;
        }
        else {
            int i;
            i = strcmp (ricettario[h].next->nome_ricetta, nomericetta);
            if (i == 0) {
                printf ("ignorato\n");
                return;
            }
            ricetta* y;
            y = ricettario[h].next;
            while (i != 0 && y->next != NULL) {
                y = y->next;
                i = strcmp (y->nome_ricetta, nomericetta);
            }
            if (i == 0) {
                printf ("ignorato\n");
                return;
            }
            if (y->next == NULL) {
                y->next = CreaNodoRicetta (nomericetta);
                y->next->prev = y;
                CreaListaIngredienti (y->next, fp);
                printf ("aggiunta\n");
                return;
            }
        }
    }
    return;
}

/*rimuovi_ricetta*/

statoordine ControllaSeInSospeso (ordine* testa, char* chiave) {
    int i; 
    if (testa == NULL) {
        return pronto;
    }
    while (testa != NULL) {
        i = strcmp (testa->nomeordine, chiave);
        if (i == 0) {
            return inattesa;
        }
        else {
            testa = testa->next;
        }
    }
    return pronto;
}

void RimuoviRicetta (hashricette* ricettario, ordine* listainattesa, ordine* listapronti, FILE* fp) {
    char nomericetta[MAX_LUNGHEZZA];
	statoordine stato;

	int i;
    
    if (fscanf(fp, "%s", nomericetta) == 1) {
        nomericetta[strlen(nomericetta)] = '\0';
        stato = ControllaSeInSospeso (listainattesa, nomericetta);
        if (stato == inattesa) {
            printf ("ordini in sospeso\n");
            return;
        }
        stato = ControllaSeInSospeso (listapronti, nomericetta);
        if (stato == inattesa) {
            printf ("ordini in sospeso\n");
            return;
        }

        int h;
        ricetta* y;    
        h = CalcolaHash (nomericetta);
        if (ricettario[h].next == NULL) {
            printf ("non presente\n");
            return;
        } 
        i = strcmp (ricettario[h].next->nome_ricetta, nomericetta);
        if (i == 0) {
            ricetta* y;
            y = ricettario[h].next->next;
            DistruggiListaIngredienti (ricettario[h].next->ingredienti);
            free (ricettario[h].next);
            ricettario[h].next = y;
            if (y != NULL) {
                y->prev = NULL;
            }
            printf ("rimossa\n");
            return;
        }
        //i = strcmp (ricettario[h].next->nome_ricetta, nomericetta);
        y = ricettario[h].next;
        while (i != 0 && y != NULL) {
            y = y->next;
            if (y != NULL) {
                i = strcmp (y->nome_ricetta, nomericetta);
            }
        }
        if (y == NULL) {
            printf ("non presente\n");
            return;
        }
        if (i == 0) {
            ricetta* daeliminare;
            daeliminare = y;
            y->prev->next = y->next;
            if (y->next != NULL) {
                y->next->prev = y->prev;
            }
            daeliminare->ingredienti = DistruggiListaIngredienti (daeliminare->ingredienti);
            free (daeliminare);
            daeliminare = NULL;
            printf ("rimossa\n");
            return;
        }
    }
    return;
}


/*rifornimento*/

lotto* CreaNodoLotto (int quantita, int scadenza) {
    lotto* nodo;  
    nodo = malloc(sizeof(lotto));
    nodo->quantita = quantita;
    nodo->datadiscadenza = scadenza;
    nodo->next = NULL;
    return nodo;
}

lotto* AggiungiLotto (ingrediente* nodo, lotto* testa, int quantita, int scadenza, int t) {
    lotto* y;
    if (scadenza <= t) {
	    return testa;
    }
    //non ci sono elementi
    if (testa == NULL) {
    	lotto* nuovolotto;
        nuovolotto = CreaNodoLotto (quantita, scadenza);
        testa = nuovolotto;
        nodo->qtot = nodo->qtot + quantita;
        return testa;
    }
    //aggiorna la quantita del nodo corrente
    if (testa->datadiscadenza == scadenza) {
        testa->quantita = testa->quantita + quantita;
        nodo->qtot = nodo->qtot + quantita;
        return testa;
    }
    //aggiunge il nodo in testa
    if (testa->datadiscadenza > scadenza) {
        y = testa;
        lotto* nuovolotto;
        nuovolotto = CreaNodoLotto (quantita, scadenza);
        testa = nuovolotto;
        nuovolotto->next = y;
        nodo->qtot = nodo->qtot + quantita;
        return testa;
    }
    if (testa->datadiscadenza < scadenza) {
        //c'è un solo nodo, aggiunge il nodo in coda
        if (testa->next == NULL) {
            lotto* nuovolotto;
            nuovolotto = CreaNodoLotto (quantita, scadenza);
            testa->next = nuovolotto;
            nodo->qtot = nodo->qtot + quantita;
            return testa;
        }
        else {
            y = testa;
            //ci sono almeno due nodi
            while (y->next != NULL) {
                //scorre di un elemento nella lista
                if (y->next->datadiscadenza < scadenza) {
                    y = y->next;
                }
                //agiorno la quantita del nodo corrente
                else if (y->next->datadiscadenza == scadenza) {
                    y->next->quantita = y->next->quantita + quantita;
                    nodo->qtot = nodo->qtot + quantita;
                    return testa;
                }
                //aggiunge il nuovo nodo tra i due nodi
                else if (y->next->datadiscadenza > scadenza) {
                    lotto* nuovolotto;
                    nuovolotto = CreaNodoLotto (quantita, scadenza);
                    nuovolotto->next = y->next;
                    y->next = nuovolotto;
                    nodo->qtot = nodo->qtot + quantita;
                    return testa;
                }
            }
            if (y->next == NULL) {
                lotto* nuovolotto;
                nuovolotto = CreaNodoLotto (quantita, scadenza);
                y->next = nuovolotto;
                nodo->qtot = nodo->qtot + quantita;
                return testa;
            }
        }
    }
    return testa;
}

ingrediente* CreaNodoIngrediente (char* nome, int scadenza, int t) {
    ingrediente* nodo;
    if (scadenza <= t) {
        return NULL;
    }
    nodo = malloc(sizeof(ingrediente));
    strcpy (nodo->nome_ingrediente, nome);
    nodo->qtot = 0;
    nodo->listalotti = NULL;
    nodo->next = NULL;
    nodo->prev = NULL;
    return nodo;
}

hashmagazzino* AggiungiIngredienteMagazzino (hashmagazzino* magazzino, FILE* fp, int istante) {
    char c;
    c = fgetc(fp);
    while (c != '\n' && c != EOF) {
        char nomeingrediente[MAX_LUNGHEZZA];
        int q;
        int t;
        int value = fscanf (fp, "%s", nomeingrediente);
        if (value == 1) {
            int value1 = fscanf (fp, "%d %d", &q, &t);
            if (value1 == 2) {
                int h;
                h = CalcolaHash (nomeingrediente);
                if (magazzino[h].next == NULL) {
                    ingrediente* nodo;
                    nodo = CreaNodoIngrediente (nomeingrediente, t, istante);
                    if (nodo != NULL) {
                        magazzino[h].next = nodo;
                        magazzino[h].next->listalotti = AggiungiLotto (magazzino[h].next, magazzino[h].next->listalotti, q, t, istante); 
                    }
                }
                else {
                    int i;
                    i = strcmp (magazzino[h].next->nome_ingrediente, nomeingrediente);
                    if (i == 0) {
                        magazzino[h].next->listalotti = AggiungiLotto (magazzino[h].next, magazzino[h].next->listalotti, q, t, istante);
                    }
                    else {
                        ingrediente* y;
                        y = magazzino[h].next;
                        while (i != 0 && y->next != NULL) {
                            y = y->next;
                            i = strcmp (y->nome_ingrediente, nomeingrediente);
                        }
                        if (i == 0) {
                            y->listalotti = AggiungiLotto (y, y->listalotti, q, t, istante);
                        }
                        else {
                            y->next = CreaNodoIngrediente (nomeingrediente, t, istante);
                            if (y->next != NULL) {
                                y->next->prev = y;
                                y->next->listalotti = AggiungiLotto (y->next, y->next->listalotti, q, t, istante);
                            }
                        }
                    }
                }
            }
        }
        c = fgetc (fp);
    }
    return magazzino;
}

/*ordine*/ 

void ControllaScadenze (ingrediente* nodo, int t) {   
    lotto* x;       
    x = nodo->listalotti;
    while (x != NULL && x->datadiscadenza <= t) {
        nodo->qtot = nodo->qtot - x->quantita;
        nodo->listalotti = nodo->listalotti->next;
        free(x);
        x = nodo->listalotti;
    }
    return;
}

void PreparaOrdine (ordine* nodo, ricetta* ric) {
    int qnecessaria;
    ingrediente_ricetta* ingredientecercato;
    ingredientecercato = ric->ingredienti;
    //scorro tutti gli ingredienti della ricetta
    while (ingredientecercato != NULL) {
        qnecessaria = ingredientecercato->quantita * nodo->quantita;
        lotto* lotti;
        lotti = ingredientecercato->nodo_ingrediente->listalotti;
        while (qnecessaria > 0) {
            if (lotti->quantita <= qnecessaria) {
                ingredientecercato->nodo_ingrediente->qtot = ingredientecercato->nodo_ingrediente->qtot - lotti->quantita;
                qnecessaria = qnecessaria - lotti->quantita;
                ingredientecercato->nodo_ingrediente->listalotti = ingredientecercato->nodo_ingrediente->listalotti->next;
                free (lotti);
                lotti = ingredientecercato->nodo_ingrediente->listalotti;
            }
            else if (lotti->quantita > qnecessaria) {
                lotti->quantita = lotti->quantita - qnecessaria;
                ingredientecercato->nodo_ingrediente->qtot = ingredientecercato->nodo_ingrediente->qtot - qnecessaria;
                qnecessaria = 0;
            }
        }
        ingredientecercato = ingredientecercato->next;         
    }
    return;
}

statoordine ControllaSePreparabile (ordine* nodo, ricetta* ric, int t) {
    int qnecessaria;
    statoordine stato;
    ingrediente_ricetta* ingredientecercato;
    ingredientecercato = ric->ingredienti;
    //scorro tutti gli ingredienti della ricetta
    while (ingredientecercato != NULL) {
        qnecessaria = ingredientecercato->quantita * nodo->quantita;
        lotto* lotti;
        lotti = ingredientecercato->nodo_ingrediente->listalotti;
        if (lotti != NULL && lotti->datadiscadenza <= t) {
            ControllaScadenze (ingredientecercato->nodo_ingrediente, t);
        }
        if (qnecessaria <= ingredientecercato->nodo_ingrediente->qtot) {
            ingredientecercato = ingredientecercato->next;
            if (ingredientecercato == NULL) {
                PreparaOrdine (nodo, ric);
                return pronto;
            }
        }
        else  {
            stato = inattesa;
            break;
        }
    }
    return stato;
}

statoordine ComandoOrdine (ordine* nodo, int t) {  
    statoordine stato;
    stato = ControllaSePreparabile (nodo, nodo->ricettaordine, t); //se preparabile la prepara e reestituisce pronto, se no inattesa
    nodo->peso = nodo->ricettaordine->peso_ricetta * nodo->quantita;
    return stato;
}

void AggiungiInCodaAttesa (ordine* nodo) {
    if (listainattesa == NULL) {
        listainattesa = nodo;
        nodo->prev = NULL;
        nodo->next = NULL;
        codainattesa = listainattesa;
    }
    else {
        codainattesa->next = nodo;
        nodo->prev = codainattesa;
        nodo->next = NULL;
        codainattesa = nodo;
        //printf ("\nnodo: %d %s, codaattesa: %d %s", nodo->arrivo, nodo->nomeordine, codainattesa->arrivo, codainattesa->nomeordine);
    }
    return;
}

void AggiungiInCodaPronti (ordine* nodo) {
    if (listapronti == NULL) {
        listapronti = nodo;
        nodo->prev = NULL;
        nodo->next = NULL;
        codapronti = listapronti;
        //printf ("\nnodo: %d %s, codapronti: %d %s", nodo->arrivo, nodo->nomeordine, codapronti->arrivo, codapronti->nomeordine);
    }
    else {
        codapronti->next = nodo;
        nodo->prev = codapronti;
        nodo->next = NULL;
        codapronti = nodo;
    }
    return;
}


void AggiungiNodoInPronti (ordine* nodo) {
    //printf ("\naggiugni nodo");
    ordine* y;
    //lista vuota
    if (listapronti == NULL) {
        listapronti = nodo;
        nodo->prev = NULL;
        nodo->next = NULL;
        codapronti = listapronti;
        //printf ("\nnodo: %d %s, codapronti: %d %s", nodo->arrivo, nodo->nomeordine, codapronti->arrivo, codapronti->nomeordine);
        return;
    }
    y = listapronti;
    //aggiungo in testa
    if (y->arrivo > nodo->arrivo) {
        nodo->next = y;
        nodo->prev = NULL;
        y->prev = nodo;
        listapronti = nodo;
        //printf ("\nnodo: %d %s, codapronti: %d %s", nodo->arrivo, nodo->nomeordine, codapronti->arrivo, codapronti->nomeordine);
        return;
    }
    while (y != NULL && y->next != NULL) {
        if (y->arrivo < nodo->arrivo) {
            y = y->next;
        }
        //inserisco tra i due nodi
        else {
            nodo->next = y;
            nodo->prev = y->prev;
            y->prev->next = nodo;
            y->prev = nodo;
            //printf ("\nnodo: %d %s, codapronti: %d %s", nodo->arrivo, nodo->nomeordine, codapronti->arrivo, codapronti->nomeordine);
            return;
        }
    }
    if (y->next == NULL) {
        if (y->arrivo < nodo->arrivo) {
            y->next = nodo;
            nodo->prev = y;
            codapronti = nodo;
            //printf ("\nnodo: %d %s, codapronti: %d %s", nodo->arrivo, nodo->nomeordine, codapronti->arrivo, codapronti->nomeordine);
            return;
        }
        else {
            nodo->next = y;
            nodo->prev = y->prev;
            y->prev->next = nodo;
            y->prev = nodo;
            //printf ("\nnodo: %d %s, codapronti: %d %s", nodo->arrivo, nodo->nomeordine, codapronti->arrivo, codapronti->nomeordine);
            return;
        }
    }
    return;
}

void SpostaOrdine (ordine* nodo) {
    ordine* y;
    //l'ordine da spostare è in testa alla listainattesa
    if (nodo->prev == NULL) {
        y = nodo;
        listainattesa = listainattesa->next;
        if (listainattesa != NULL) {
            listainattesa->prev = NULL;
        }
        y->prev = NULL;
        y->next = NULL;
        AggiungiNodoInPronti (y);
        //listapronti = AggiungiNodo (listapronti, y);
        return;
    }
    else {
        y = nodo;
        nodo->prev->next = nodo->next;
        if (nodo->next != NULL) {
            nodo->next->prev = nodo->prev;
        }
        else {
            codainattesa = nodo->prev;
        }
        y->prev = NULL;
        y->next = NULL;
        AggiungiNodoInPronti (y);
        //listapronti = AggiungiNodo (listapronti, y);
        return;
    }
    return;
}

ordine* AggiungiNodoCorriere (ordine* testalista, ordine* nodo) {
    if (testalista == NULL) {
        testalista = nodo;
        nodo->prev = NULL;
        return testalista;
    }
    ordine* y;
    y = testalista;
    //printf ("\nsto aggiungendo a lista corriere il nodo %d (peso) %d %s %d", nodo->peso, nodo->arrivo, nodo->nomeordine, nodo->quantita);
    int p;
    int t;
    p = nodo->peso;
    t = nodo->arrivo;
    //aggiunta in testa
    if (y->peso < p) {
        nodo->next = y;
        nodo->prev = NULL;
        testalista = nodo;
        return testalista;
    }
    if (y->peso == p) {
        //aggiunta in testa
        if (y->arrivo > t) {
            nodo->next = y;
            nodo->prev = NULL;
            testalista = nodo;
            return testalista;
        }
        else {
            if (y->next == NULL) {
                y->next = nodo;
                nodo->prev = y;
                return testalista;
            }
            else {
                while (y->next != NULL) {
                    if (y->next->peso < p) {
                        nodo->next = y->next;
                        nodo->prev = y;
                        y->next = nodo;
                        nodo->next->prev = nodo;
                        return testalista;
                    }
                    else if (y->next->peso == p) {
                        if (y->next->arrivo > t) {
                            nodo->next = y->next;
                            nodo->prev = y;
                            y->next = nodo;
                            nodo->next->prev = nodo;
                            return testalista;
                        }
                        else y = y->next;
                    }
                }
                if (y->next == NULL) {
                    y->next = nodo;
                    nodo->prev = y;
                    return testalista;
                }
            }
        }
    }
    else if (y->peso > p) {
        while (y->next != NULL) {
            if (y->next->peso > p) {
                y = y->next;
            }
            else if (y->next->peso == p) {
                if (y->next->arrivo > t) {
                    nodo->next = y->next;
                    nodo->prev = y;
                    y->next = nodo;
                    nodo->next->prev =nodo;
                    return testalista;
                }
                else y = y->next;
            }
            else if (y->next->peso < p) {
                nodo->next = y->next;
                nodo->prev = y;
                y->next = nodo;
                nodo->next->prev =nodo;
                return testalista;
            }
        }
        if (y->next == NULL) {
            y->next = nodo;
            nodo->prev = y;
            return testalista;
        }
    }
    return testalista;
}

ricetta* RicercaRicetta (char* nome) {
    int h;
    int i;
    ricetta* ric;
    h = CalcolaHash (nome);
    if (ricettario[h].next == NULL) {
        return NULL;
    }
    i = strcmp (ricettario[h].next->nome_ricetta, nome);
    ric = ricettario[h].next;
    while (i != 0 && ric->next != NULL) {
        ric = ric->next;
        i = strcmp (ric->nome_ricetta, nome);
    }
    if (i == 0) {
        return ric;
    }
    else return NULL;
}

ordine* CreaNodoOrdine (char* nome, int q, int t) {
    ordine* nodo;  
    nodo = malloc(sizeof(ordine));
    strcpy (nodo->nomeordine, nome);
    nodo->quantita = q;
    nodo->arrivo = t;
    nodo->next = NULL;
    nodo->prev = NULL;
    nodo->ricettaordine = RicercaRicetta (nome);
    return nodo;
}

int main() {
    FILE* fp;
    fp = stdin;    
    if( fp==NULL ) {
        perror("Errore in apertura del file");
        exit(1);
    }

    int corriere, capienza;
    int returnvalue;
    //int i;
    int t = -1;
    int tcorriere = 0;
    char comando[MAX_LUNGHEZZA];
    int scmp;

    ordine* listacorriere;
    listapronti = NULL;
    listainattesa = NULL;
    listacorriere = NULL;
    codapronti = NULL;
    codainattesa = NULL; 

    //salvataggio dati: corriere e capienza
    returnvalue = fscanf (fp, "%d %d", &corriere, &capienza);
    if ((returnvalue) == 2) {
        //riconoscimento comandi
        while(!feof(fp)) {
            if (fscanf(fp, "%s", comando) == 1) {
                scmp = strcmp(comando, "aggiungi_ricetta");
                if (scmp == 0) {
                    t++;
                    tcorriere++;
                    //ControllaScadenze (magazzino, t);
                    AggiungiRicetta (ricettario, fp);
                }
                scmp = strcmp(comando, "rimuovi_ricetta");
                if (scmp == 0) {
                    t++;
                    tcorriere++;
                    //ControllaScadenze (magazzino, t);
                    RimuoviRicetta (ricettario, listainattesa, listapronti, fp);
                }
                scmp = strcmp(comando, "rifornimento");
                if (scmp == 0) {
                    t++;
                    tcorriere++;
                    //ControllaScadenze (magazzino, t);
                    AggiungiIngredienteMagazzino (magazzino, fp, t);
                    printf ("rifornito\n");   
                    ordine* y;
                    y = listainattesa;
                    while (y != NULL) {
                        codainattesa = y;
                        statoordine stato;
                        stato = ControllaSePreparabile (y, y->ricettaordine, t);
                        if (stato == pronto) {
                            ordine* x;
                            x = y->next;
                            SpostaOrdine (y);
                            y = x;
                        }
                        else y = y->next;
                    }
                    if (listainattesa == NULL) {
                        codainattesa = NULL;
                    }
                    //printf ("\nt = %d", t);
                }
                scmp = strcmp(comando, "ordine");
                if (scmp == 0) {
                    char nome[MAX_LUNGHEZZA];
                    int q;
                    t++;
                    tcorriere++;
                    //ControllaScadenze (magazzino, t);
                    //printf ("\n%d ordine", t);
                    if (fscanf (fp, "%s", nome) == 1) {
                        if (fscanf (fp, "%d", &q) == 1) {
                            //creo nodo ordine
                            ordine* nodoordine;
                            statoordine stato;
                            nodoordine = CreaNodoOrdine (nome, q, t);
                            if (nodoordine->ricettaordine == NULL) {
                                printf ("rifiutato\n");
                            }
                            else {
                                //salvo lo stato dell'ordine
                                stato = ComandoOrdine (nodoordine, t);
                                if (stato == inattesa) {
                                    AggiungiInCodaAttesa (nodoordine);
                                    printf ("accettato\n");
                                }
                                else if (stato == pronto) {
                                    AggiungiInCodaPronti (nodoordine);
                                    printf ("accettato\n");
                                }
                            }
                        }
                        else {
                            printf ("\nerrore di lettura del file 1");
                        }
                    } 
                    else {
                        printf ("\nerrore di lettura del file 2");
                    }
                }
            }    
            if (tcorriere % corriere == 0 && tcorriere != 0) {
                //printf ("\ncorriere: t = %d", t);
                int capienzadisponibile = capienza;
                if (listapronti == NULL) {
                    printf ("camioncino vuoto\n");
                }
                else {
                    ordine* y;
                    y = listapronti;
                    while (y != NULL && y->peso <= capienzadisponibile) {
                        ordine* nodo;
                        nodo = y;
                        listapronti = listapronti->next;
                        if (listapronti != NULL) {
                            listapronti->prev = NULL;
                        }
                        else {
                            codapronti = NULL;
                        }
                        nodo->next = NULL;
                        nodo->prev = NULL;
                        listacorriere = AggiungiNodoCorriere (listacorriere, nodo);
                        capienzadisponibile = capienzadisponibile - nodo->peso;
                        y = listapronti;
                    }
                    if (listacorriere == NULL) {
                        printf ("camioncino vuoto\n");
                    }
                    while (listacorriere != NULL) {
                        y = listacorriere->next;
                        printf ("%d %s %d\n", listacorriere->arrivo, listacorriere->nomeordine, listacorriere->quantita);
                        free (listacorriere);
                        listacorriere = y;
                    }
                }
            }


            /*ordine* y;
            y = listainattesa;
            if (y == NULL) {
                printf ("\nlista in attesa vuota");
            }
            else {
                printf ("\nlista in attesa: ");
                printf ("\n%d %s %d", y->arrivo, y->nomeordine, y->quantita);
                while (y->next != NULL) {
                    y = y->next;
                    printf ("\n%d %s %d", y->arrivo, y->nomeordine, y->quantita);
                }
            }

            y = listapronti;
            if (y == NULL) {
                printf ("\nlista pronti vuota");
            }
            else {
                printf ("\nlista pronti: ");
                printf ("\n%d %s %d", y->arrivo, y->nomeordine, y->quantita);
                while (y->next != NULL) {
                    y = y->next;
                    printf ("\n%d %s %d", y->arrivo, y->nomeordine, y->quantita);
                }
            }*/

        
        }

        fclose(fp);
        //DistruggiTutto (ricettario, magazzino, listapronti, listainattesa);

        return 0;
    }
}