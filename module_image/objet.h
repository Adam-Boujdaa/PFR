#ifndef OBJET_H_INCLUS
#define OBJET_H_INCLUS

#define MAX_OBJ 5

typedef struct {
    int x, y;
} Point;

typedef struct {
    int min_x, min_y;
    int max_x, max_y;
    int aire;
} Objet;

typedef struct {
    int n_objets;
    Objet objets[MAX_OBJ];
} Objets;

void objet_afficher(Objet* obj);

#endif