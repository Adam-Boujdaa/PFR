/* ============================================================
   Fichier  : main.c
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : CHENTIR Hakim
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce fichier contient le point d'entrée du programme.
   Il initialise l'application en appelant la sélection de
   langue puis lance le menu principal de l'interface.
   ------------------------------------------------------------
                        RÔLE DU MODULE
   ------------------------------------------------------------
   démarrer l'application
   initialiser la langue de l'interface
   lancer le menu principal de navigation
   ============================================================ */

#include "menu.h"
#include "module_image.h"


/* fonction principale */
int main(void)
{
    choisir_langue();
    menu_principal();
    return 0;
}

