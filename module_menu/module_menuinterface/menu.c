#include "menu.h"
#include "utils.h"
#include "lang.h"
#include "log.h"
#include <stdio.h>
#include "utils.h"

/* choix de la langue au démarrage */
void choisir_langue(void)
{
    int choix = -1;

    while (choix != 0 && choix != 1)
    {
        printf("0 - Français\n");
        printf("1 - English\n");
        printf("Choix : ");
        choix = read_int();
    }

    if (choix == 0)
        lang_load("lang_fr.conf");
    else
        lang_load("lang_en.conf");
}

/* menu principal */
void menu_principal(void)
{
    int choix = -1;

    while (choix != 0)
    {
        printf("\n=== %s ===\n", lang("MAIN_MENU"));
        printf("1 - %s\n", lang("USER_MODE"));
        printf("2 - %s\n", lang("ADMIN_MODE"));
        printf("0 - %s\n", lang("QUIT"));
        printf("%s ", lang("CHOICE"));

        choix = read_int();

        if (choix == 1)
            menu_utilisateur();
        else if (choix == 2)
            menu_admin();
        else if (choix == 0)
            printf("%s\n", lang("EXIT"));
        else
            printf("%s\n", lang("INVALID"));
    }
}

/* menu utilisateur */
void menu_utilisateur(void)
{
    int choix = -1;

    while (choix != 0)
    {
        printf("\n=== %s ===\n", lang("USER_MENU"));
        printf("1 - %s\n", lang("TEXT_CMD"));
        printf("2 - %s\n", lang("SIMULATION"));
        printf("3 - %s\n", lang("IMAGE"));
        printf("0 - %s\n", lang("BACK"));
        printf("%s ", lang("CHOICE"));

        choix = read_int();

        if (choix == 1)
        {
            printf("Commande textuelle choisie\n");
            log_msg("Utilisateur : commande textuelle");
        }
        else if (choix == 2)
        {
            printf("Simulation choisie\n");
            log_msg("Utilisateur : simulation");
        }
        else if (choix == 3)
        {
            printf("Traitement image choisi\n");
            log_msg("Utilisateur : image");
        }
        else if (choix == 0)
            printf("Retour menu principal\n");
        else
            printf("%s\n", lang("INVALID"));
    }
}

/* menu administrateur */
void menu_admin(void)
{
    int choix = -1;

    /* verification identifiant / mot de passe */
    if (verifier_admin() != 1)
    {
        /* acces refuse -> retour direct */
        return;
    }

    /* si on arrive ici, l'admin est valide */
    while (choix != 0)
    {
        printf("\n=== %s ===\n", lang("ADMIN_MENU"));
        printf("1 - %s\n", lang("VIEW_LOGS"));
        printf("0 - %s\n", lang("BACK"));
        printf("%s ", lang("CHOICE"));

        choix = read_int();

        if (choix == 1)
            show_logs();
        else if (choix == 0)
            printf("%s\n", lang("BACK"));
        else
            printf("%s\n", lang("INVALID"));
    }
}

