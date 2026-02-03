/* ============================================================
   Fichier  : config.c
   Projet   : PFR1 – Projet Fil Rouge
   Module   : Configuration / Internationalisation
   Auteur   : CHENTIR Hakim
   Version  : v2.0
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce fichier gère le système de traduction de l'application.
   Il charge les fichiers de langue (.txt) et permet d'obtenir
   les traductions des clés en fonction de la langue choisie.
   ------------------------------------------------------------
                        RÔLE DU MODULE
   ------------------------------------------------------------
   charger les fichiers de configuration linguistique
   stocker les paires clé/valeur en mémoire
   fournir les traductions via la fonction config()
   permettre le changement de langue de l'interface
   ============================================================ */

#include "config.h"
#include <stdio.h>
#include <string.h>

/* nombre max de traductions */
#define MAX 100

/* tableaux pour stocker clés et valeurs */
static char keys[MAX][50];
static char values[MAX][200];
static int count = 0;

/* charge un fichier de langue */
void charger_config(const char* file) {
    FILE* f = fopen(file, "r");
    char line[256];
    char* eq;

    if (!f)
        return;

    while (fgets(line, sizeof(line), f)) {
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
const char* config(const char* key) {
    int i;

    for (i = 0; i < count; i++) {
        if (strcmp(keys[i], key) == 0)
            return values[i];
    }

    return key;
}
