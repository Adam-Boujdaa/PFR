#include "utils.h"
#include <stdio.h>

/* lecture simple d'un entier */
int read_int(void)
{
    int n;

    if (scanf("%d", &n) == 1)
        return n;

    /* vide le buffer en cas d'erreur */
    while (getchar() != '\n');
    return -1;
}

