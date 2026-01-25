#ifndef OBJET_H_INCLUS
#define OBJET_H_INCLUS

typedef struct {
    int x, y;
} Point;


typedef struct {
    int min_x, min_y;
    int max_x, max_y;
    int rayon;
    Point centre;
    int aire;
} Objet;


#endif