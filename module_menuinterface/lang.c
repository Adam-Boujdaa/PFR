#include "lang.h"
#include <stdio.h>
#include <string.h>

/* nombre max de traductions */
#define MAX 100

/* tableaux pour stocker clés et valeurs */
static char keys[MAX][50];
static char values[MAX][200];
static int count = 0;

/* charge un fichier de langue */
void lang_load(const char *file)
{
    FILE *f = fopen(file, "r");
    char line[256];
    char *eq;

    if (!f)
        return;

    count = 0;

    while (fgets(line, sizeof(line), f))
    {
        eq = strchr(line, '=');
        if (!eq)
            continue;

        *eq = '\0';
        strcpy(keys[count], line);
        strcpy(values[count], eq + 1);

        /* enlève le retour à la ligne */
        values[count][strcspn(values[count], "\n")] = 0;

        count++;
    }

    fclose(f);
}

/* cherche une clé et retourne la traduction */
const char *lang(const char *key)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (strcmp(keys[i], key) == 0)
            return values[i];
    }

    return key;
}

