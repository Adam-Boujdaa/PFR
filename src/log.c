#include <stdio.h>
#include <time.h>
#include "log.h"


/* ajoute une ligne dans logs.txt */
void log_msg(const char *msg)
{
    FILE *f = fopen("logs.txt", "a");

    if (!f)
        return;

    time_t t = time(NULL); // Récupère le temps actuel
    char * t_str = ctime(&t); // Convertit le temps en chaîne de caractères
    t_str[24] = '\0'; // Enleve le retour à la ligne

    fprintf(f, "[%s] %s\n", t_str, msg);
    fclose(f);
}

/* affiche le contenu du fichier de log */
void show_logs()
{
    FILE *f = fopen("logs.txt", "r");
    char ligne[256];

    if (!f)
    {
        printf("Aucun log disponible\n");
        return;
    }

    while (fgets(ligne, sizeof(ligne), f))
        printf("%s", ligne);

    fclose(f);
}

