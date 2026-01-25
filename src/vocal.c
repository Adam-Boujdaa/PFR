#include "vocal.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionnaire.h"
#include "parseur.h"
#include "intercom.h"

// MODE texte ou vocal
char MODE = 'T';

// Dictionnaires
static Dico act[4];
static Dico dir[2];
static Dico macros[3];
static Dico nb[10];  
static Dico units[6];
static Dico liaisons[1];
static Dico liaisons_inv[1];

/* fonction unique pour charger dictionnaires selon la langue */
void charger_dictionnaire(Langue langue) {
    const char* prefix = (langue == FR) ? "FR" : "EN";
    char path[200];

    // Actions élémentaires
    sprintf(path, "PATH_%s_AVANCER", prefix);
    charger_dico(&act[0], config(path), 'A', 1);

    sprintf(path, "PATH_%s_RECULER", prefix);
    charger_dico(&act[1], config(path), 'R', 1);

    sprintf(path, "PATH_%s_TOURNER", prefix);
    charger_dico(&act[2], config(path), 'T', 0);

    sprintf(path, "PATH_%s_STOP", prefix);
    charger_dico(&act[3], config(path), 'S', 0);

    // Directions
    sprintf(path, "PATH_%s_GAUCHE", prefix);
    charger_dico(&dir[0], config(path), 'G', 90);

    sprintf(path, "PATH_%s_DROITE", prefix);
    charger_dico(&dir[1], config(path), 'D', -90);

    // Macros
    sprintf(path, "PATH_%s_CARRE", prefix);
    charger_dico(&macros[0], config(path), 'C', 0);

    sprintf(path, "PATH_%s_ZIGZAG", prefix);
    charger_dico(&macros[1], config(path), 'Z', 0);

    sprintf(path, "PATH_%s_DEMI_TOUR", prefix);
    charger_dico(&macros[2], config(path), 'U', 0);

    // Nombres
    char path_nb[200];
    for (int i = 1; i <= 10; i++) {
        sprintf(path_nb, config("PATH_%s_NB_FMT", prefix), i);
        charger_dico(&nb[i - 1], path_nb, 'N', i);
    }

    // Unités
    sprintf(path, "PATH_%s_METRES", prefix);
    charger_dico(&units[0], config(path), 'm', 0);
    sprintf(path, "PATH_%s_CM", prefix);
    charger_dico(&units[1], config(path), 'c', 0);
    sprintf(path, "PATH_%s_DM", prefix);
    charger_dico(&units[2], config(path), 'l', 0);
    sprintf(path, "PATH_%s_DEGRES", prefix);
    charger_dico(&units[3], config(path), 'd', 0);
    sprintf(path, "PATH_%s_PIEDS", prefix);
    charger_dico(&units[4], config(path), 'f', 0);
    sprintf(path, "PATH_%s_POUCES", prefix);
    charger_dico(&units[5], config(path), 'i', 0);
}

// Fonction principale de vocal
int main_voc(Langue langue) {
    liaisons[0].nb_syn = 0;
    liaisons_inv[0].nb_syn = 0;

    // Charge dictionnaires via config
    charger_dictionnaire(langue);

    // Prompt initial
    printf("%s\n", config(langue == FR ? "MSG_READY_FR" : "MSG_READY_EN"));

    if (MODE == 'T')
        traitement_mode_textuel(langue);
    else if (MODE == 'V')
        traitement_mode_vocal();

    return 0;
}

// Mode vocal
void traitement_mode_vocal() {
    ListeCommandes* liste = init_liste();
    char buffer[MAX_LIGNE];
    liaisons[0].nb_syn = 0;
    liaisons_inv[0].nb_syn = 0;

    while (1) {
        if (system(config("CMD_PY_TTS")) != 0) {
            printf("%s\n", config("ERR_TTS"));
            break;
        }

        printf("%s\n", config("VOC_ACTIVE"));

        FILE* f_vocal = fopen(config("FILE_CMD_VOCAL"), "r");
        if (f_vocal != NULL) {
            if (fgets(buffer, MAX_LIGNE, f_vocal) != NULL) {
                buffer[strcspn(buffer, "\n")] = 0;

                if (strcmp(buffer, config("CMD_EXIT")) == 0) {
                    fclose(f_vocal);
                    printf("%s\n", config("MSG_EXIT"));
                    break;
                }

                liste->nb_commandes = 0;
                traiter_cmd(buffer, macros, 3, liaisons, 1, liaisons_inv, 1,
                            act, 4, dir, 2, nb, 10, units, 6, liste);
                debug_afficherliste(liste);
                ecrire_commandes(liste, config("FILE_CMD_POST"));

                // Confirmation commandes longues
                FILE* fp = popen("wc -l < output/post_traitement/commande.txt", "r");
                if (fp) {
                    char result[16];
                    fgets(result, sizeof(result), fp);
                    int nb_lignes = atoi(result);
                    if (nb_lignes > 30) {
                        printf("%s\n", config("WARN_LONG_CMD"));
                        system(config("CMD_PY_CONFIRM"));
                        printf("%s\n", config("MSG_SPEAK"));
                        system(config("CMD_PY_TTS"));

                        FILE* fp_grep = popen("grep -i \"oui\" output/speech_to_text/commande.txt", "r");
                        int confirmation = 0;
                        char test_buffer[128];
                        if (fgets(test_buffer, sizeof(test_buffer), fp_grep)) {
                            confirmation = 1;
                        }
                        pclose(fp_grep);

                        if (confirmation)
                            printf("%s\n", config("CONFIRM_OK"));
                        else {
                            printf("%s\n", config("CONFIRM_KO"));
                            FILE* f_annul = fopen("output/post_traitement/commande.txt", "w");
                            if (f_annul) fclose(f_annul);
                        }
                    }
                    pclose(fp);
                }
            }
            fclose(f_vocal);

            // On vide le fichier pré-traitement
            f_vocal = fopen(config("FILE_CMD_VOCAL"), "w");
            if (f_vocal) fclose(f_vocal);
        } else {
            printf("%s\n", config("ERR_READ_CMD"));
        }
    }
    free_liste(liste);
}

// Mode textuel
void traitement_mode_textuel(Langue langue) {
    ListeCommandes* liste = init_liste();
    char buffer[MAX_LIGNE];
    liaisons[0].nb_syn = 0;
    liaisons_inv[0].nb_syn = 0;

    while (1) {
        printf("%s", config(langue == FR ? "PROMPT_CMD_FR" : "PROMPT_CMD_EN"));
        if (fgets(buffer, MAX_LIGNE, stdin) == NULL) break;
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, config("CMD_EXIT")) == 0) break;

        liste->nb_commandes = 0;
        traiter_cmd(buffer, macros, 3, liaisons, 1, liaisons_inv, 1,
                    act, 4, dir, 2, nb, 10, units, 6, liste);
        debug_afficherliste(liste);
        ecrire_commandes(liste, config("FILE_CMD_POST"));

        char bufffer_commande[1024];
        commmandes_str(liste, bufffer_commande, 1024);
        envoyer_commande(bufffer_commande);
    }
    free_liste(liste);
}
