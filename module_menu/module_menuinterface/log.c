#include "log.h"
#include <stdio.h>

/* ajoute une ligne dans logs.txt */
void log_msg(const char *msg)
{
    FILE *f = fopen("logs.txt", "a");

    if (!f)
        return;

    fprintf(f, "%s\n", msg);
    fclose(f);
}

/* affiche le contenu du fichier de log */
void show_logs(void)
{
    FILE *f = fopen("logs.txt", "r");
    char line[256];

    if (!f)
    {
        printf("Aucun log disponible\n");
        return;
    }

    while (fgets(line, sizeof(line), f))
        printf("%s", line);

    fclose(f);
}

