#ifndef PILE_H_INCLUS
#define PILE_H_INCLUS

#include "module_image.h"

typedef struct elemCellule {
    Point elem;
    struct elemCellule* suivant;
} Cellule;

typedef Cellule* PILE;


PILE init_PILE();
void affiche_PILE(PILE p);
int PILE_estVide(PILE p);
PILE emPILE(PILE p, Point e);
PILE dePILE(PILE p, Point *e);

#endif