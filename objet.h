#ifndef OBJET_H_INCLUS
#define OBJET_H_INCLUS

typedef struct {
    int x, y;
} Point;


typedef struct {
    int min_x, min_y;
    int max_x, max_y;
    int aire;
} Objet;

void objet_afficher(Objet* obj);

#endif