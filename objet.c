#include <stdio.h>
#include "objet.h"

void objet_afficher(Objet *obj) {
    printf("Objet: min_x=%d, min_y=%d, max_x=%d, max_y=%d, aire=%d\n",
			   obj->min_x,
			   obj->min_y,
			   obj->max_x,
			   obj->max_y,
			   obj->aire);
}