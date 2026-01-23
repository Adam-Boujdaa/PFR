#include "pile.h"
#include <stdio.h>
#include <stdlib.h>

PILE init_PILE() {
    return NULL;
}


int PILE_estVide(PILE p) {
    return (p == NULL);
}

PILE emPILE(PILE p, Point o) {
    PILE nouvelle_cellule = (PILE)malloc(sizeof(PILE));
    nouvelle_cellule->elem = o;
    if (p == NULL) {
        p = nouvelle_cellule;
        nouvelle_cellule->suivant = NULL;
        return p;
    } else {
        nouvelle_cellule->suivant = p;
        p = nouvelle_cellule;
        return p;
    }
}

PILE dePILE(PILE p, Point *e) {
    if (p == NULL) {
        fprintf(stderr, "[IMAGE] Erreur : Pile vide, impossible de dépiler.\n");
        return NULL;
    }

    *e = p->elem;
    p = p->suivant;
    return p;
}

