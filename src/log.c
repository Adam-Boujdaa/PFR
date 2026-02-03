/* ============================================================
   Fichier  : log.c
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : CHENTIR Hakim
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce fichier gère les logs de l'application.
   Chaque action importante est enregistrée dans le fichier
   "logs.txt" avec la date et l'heure.
   ------------------------------------------------------------
                        RÔLE DU MODULE
   ------------------------------------------------------------
   * mémoriser les actions de l'utilisateur
   * aider à comprendre le fonctionnement du programme
   * permettre à l'administrateur de consulter l'historique
   ============================================================ */

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

