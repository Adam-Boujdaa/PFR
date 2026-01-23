#include "menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lang.h"
#include "log.h"
#include "module_image.h"
#include "utils.h"

#define MAX_OBJETS 10

#define NETTOYER() printf("\033[2J\033[H")

/* choix de la langue au démarrage */
void choisir_langue(void) {
    int choix = -1;

    while (choix != 0 && choix != 1) {
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
void menu_principal(void) {
    NETTOYER();

    int choix = -1;

    while (choix != 0) {
        printf("\n=== %s ===\n", lang("MAIN_MENU"));
        printf("1 - %s\n", lang("USER_MODE"));
        printf("2 - %s\n", lang("ADMIN_MODE"));
        printf("0 - %s\n", lang("QUIT"));
        printf("%s ", lang("CHOICE"));

        choix = read_int();

        switch (choix) {
            case 1:
                menu_utilisateur();
                break;
            case 2:
                menu_admin();
                break;
            case 0:
                printf("%s\n", lang("EXIT"));
                break;
            default:
                printf("%s\n", lang("INVALID"));
        }
    }
}

/* menu utilisateur */
void menu_utilisateur() {
    NETTOYER();
    int choix = -1;

    while (choix != 0) {
        printf("\n=== %s ===\n", lang("USER_MENU"));
        printf("1 - %s\n", lang("TEXT_CMD"));
        printf("2 - %s\n", lang("SIMULATION"));
        printf("3 - %s\n", lang("IMAGE"));
        printf("0 - %s\n", lang("BACK"));
        printf("%s ", lang("CHOICE"));

        choix = read_int();

        switch (choix) {
            case 1:
                printf("Commande textuelle choisie\n");
                log_msg("Utilisateur : commande textuelle");
                break;
            case 2:
                printf("Simulation choisie\n");
                log_msg("Utilisateur : simulation");
                break;
            case 3:
                printf("Traitement image choisi\n");
                log_msg("Utilisateur : image");
                menu_image();
                break;
            case 0:
                printf("Retour menu principal\n");
                break;
            default:
                printf("%s\n", lang("INVALID"));
        }
    }
}

/* menu administrateur */
void menu_admin() {
    NETTOYER();
    int choix = -1;

    while (choix != 0) {
        printf("\n=== %s ===\n", lang("ADMIN_MENU"));
        printf("1 - %s\n", lang("VIEW_LOGS"));
        printf("0 - %s\n", lang("BACK"));
        printf("%s ", lang("CHOICE"));

        choix = read_int();

        switch (choix) {
            case 1:
                printf("Affichage des logs choisi\n");
                log_msg("Administrateur : affichage des logs");
                show_logs();
                break;

            case 0:
                printf("Retour menu principal\n");
                break;

            default:
                printf("%s\n", lang("INVALID"));
                break;
        }
    }
}

void menu_image() {
    NETTOYER();
    int choix = -1;
    int image_chargee = 0;
    const char chemin_img[256];
    Image img;

    while (choix != 0) {
        printf("\n=== %s ===\n", lang("IMAGE_MENU"));

        if (!image_chargee) {
            printf("1 - %s\n", lang("LOAD_IMAGE"));
            printf("0 - %s\n", lang("BACK"));
            printf("%s ", lang("CHOICE"));

            choix = read_int();

            switch (choix) {
                case 1:
                    printf("Chargement d'image choisi\n");
                    log_msg("Utilisateur : chargement d'image");

                    choix = -1;

                    printf("Entrez le chemin du fichier image à charger : ");
                    scanf("%s", chemin_img);

                    printf("Chemin de l'image : %s\n", chemin_img);

                    FILE* in = fopen(chemin_img, "r");
                    if (!in) {
                        fprintf(stderr, "Erreur ouverture fichier image %s\n", chemin_img);
                        log_msg("Erreur ouverture fichier image");
                        sleep(5);
                        menu_image();
                    }

                    image_chargee = 1;
                    img = image_lire(in);

                    fclose(in);

                    break;
                case 0:
                    printf("Retour menu utilisateur\n");
                    break;

                default:
                    printf("%s\n", lang("INVALID"));
                    break;
            }
        } else {
            /* Menu after image is loaded */
            printf("\n=== %s ===\n", chemin_img);
            printf("1 - %s\n", lang("FIND_OBJECTS"));
            printf("2 - %s\n", lang("CONVERT_GRAYSCALE"));
            printf("3 - %s\n", lang("FLIP_VERTICAL"));
            printf("4 - %s\n", lang("QUANT"));
            printf("5 - %s\n", lang("SAVE_IMAGE"));
            printf("0 - %s\n", lang("BACK"));
            printf("%s ", lang("CHOICE"));

            choix = read_int();

            switch (choix) {
                case 1:
                    printf("Détection d'objets en cours...\n");
                    log_msg("Utilisateur : détection d'objets");

                    Objet objets[MAX_OBJETS];
                    Image mask = image_masque_objets(img, 70);
                    int nb_objets = image_trouver_objets(mask, objets, 300);

                    printf("Nombre d'objets trouves: %d\n", nb_objets);

                    for (int i = 0; i < nb_objets; i++) {
                        Point centre = objet_trouver_centre(objets[i]);
                        Pixel coul = image_trouver_couleur(img, mask, objets[i]);
                        image_dessiner_boite_englobante(img, objets[i], coul);
                        printf("Objet %d : Centre X=%d, Centre Y=%d, aire=%d Couleur=%s \n",
                               i + 1,
                               centre.x,
                               centre.y,
                               objets[i].aire,
                               image_pixel_to_nom(coul));
                    }

                    break;

                case 2: // CONVERSION NIVEAUX DE GRIS
                    printf("Binarisation en cours...\n");
                    log_msg("Utilisateur : binarisation");

                    img = image_binarisation(img);

                    printf("%s\n", lang("CONVERTED"));

                    break;

                case 3: // RETOURNEMENT VERTICAL
                    printf("Retournement vertical en cours...\n");
                    log_msg("Utilisateur : retournement vertical");

                    img = image_miroir(img);

                    printf("%s\n", lang("CONVERTED"));

                    break;

                case 4:  // QUANTIFICATION IMAGE
                    printf("Quantification en cours...\n");
                    log_msg("Utilisateur : quantification");

                    int q;
                    printf("Entrez le niveau de quantification (puissance de 2 entre 1 et 255) : ");
                    q = read_int();

                    img = image_quantification(img, q);

                    printf("%s\n", lang("CONVERTED"));

                    break;

                case 5:  // SAUVEGARDE IMAGE
                    char chemin_sauve[250];
                    char chemin_txt[256];
                    char chemin_jpg[256];
                    char commande_creer[512];
                    char commande_ouvrir[512];

                    printf("Sauvegarde de l'image en cours...\n");
                    log_msg("Utilisateur : sauvegarde d'image");

                    printf("Entrez le chemin du fichier image à sauvegarder (sans extension): ");
                    scanf("%s", chemin_sauve);

                    sprintf(chemin_txt, "%s.txt", chemin_sauve);
                    sprintf(chemin_jpg, "%s.jpg", chemin_sauve);

                    sprintf(commande_creer, "python3 creer_image.py %s %s", chemin_txt, chemin_jpg);
                    sprintf(commande_ouvrir, "xdg-open %s", chemin_jpg);

                    FILE* out = fopen(chemin_txt, "w");
                    if (!out) {
                        fprintf(stderr, "Erreur ouverture fichier image en écriture\n");
                        log_msg("Erreur ouverture fichier image en écriture");
                        free(img);
                        sleep(5);  // attendre 5 secondes avant de revenir au menu pour voir le message
                        menu_image();
                    }

                    image_enregistrer(out, img);

                    fclose(out);

                    system(commande_creer);
                    system(commande_ouvrir);

                    printf("Image sauvegardée avec succès\n");
                    break;

                case 0:
                    printf("Retour menu utilisateur\n");

                    image_chargee = 0;
                    free(img);
                    if (mask != NULL) free(mask);

                    break;

                default:
                    printf("%s\n", lang("INVALID"));
                    break;
            }
        }
    }
}