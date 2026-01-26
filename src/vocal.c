// AUTEUR : ADAM BOUJDAA
// DATE CREATION : Janvier 2026
// fichier principal de traitement texte

#include "vocal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "dictionnaire.h"
#include "parseur.h"
#include "intercom.h"


static Dico act[4];
static Dico dir[2];
static Dico macros[3];
static Dico nb[10];  // 10 fichiers de nombres mtn
static Dico units[6];
static Dico liaisons[1];
static Dico liaisons_inv[1];

void charger_dictionnaire_fr() {
    // Actions
    charger_dico(&act[0], "word_dictionnary/lexique_fr/actions/elementaires/avancer.txt", 'A', 1);
    charger_dico(&act[1], "word_dictionnary/lexique_fr/actions/elementaires/reculer.txt", 'R', 1);
    charger_dico(&act[2], "word_dictionnary/lexique_fr/actions/elementaires/tourner.txt", 'T', 0);
    charger_dico(&act[3], "word_dictionnary/lexique_fr/actions/elementaires/stop.txt", 'S', 0);

    // Directions
    charger_dico(&dir[0], "word_dictionnary/lexique_fr/actions/elementaires/gauche.txt", 'G', 90);
    charger_dico(&dir[1], "word_dictionnary/lexique_fr/actions/elementaires/droite.txt", 'D', -90);

    // Macros
    charger_dico(&macros[0], "word_dictionnary/lexique_fr/actions/macros/carre.txt", 'C', 0);
    charger_dico(&macros[1], "word_dictionnary/lexique_fr/actions/macros/zigzag.txt", 'Z', 0);
    charger_dico(&macros[2], "word_dictionnary/lexique_fr/actions/macros/demi_tour.txt", 'U', 0);

    // Nombres
    char path_nb[100];
    for (int i = 1; i <= 10; i++) {
        sprintf(path_nb, "word_dictionnary/lexique_fr/nombres/%d.txt", i);  // sprintf fais la concaténation envoyée dans path_nb
        charger_dico(&nb[i - 1], path_nb, 'N', i);
    }

    // Unités
    charger_dico(&units[0], "word_dictionnary/lexique_fr/unites/metres.txt", 'm', 0);
    charger_dico(&units[1], "word_dictionnary/lexique_fr/unites/centimetres.txt", 'c', 0);
    charger_dico(&units[2], "word_dictionnary/lexique_fr/unites/decimetres.txt", 'l', 0);
    charger_dico(&units[3], "word_dictionnary/lexique_fr/unites/degres.txt", 'd', 0);
    charger_dico(&units[4], "word_dictionnary/lexique_fr/unites/pieds.txt", 'f', 0);
    charger_dico(&units[5], "word_dictionnary/lexique_fr/unites/pouces.txt", 'i', 0);
}

void charger_dictionnaire_en() {
    // Actions
    charger_dico(&act[0], "word_dictionnary/lexique_en/actions/elementaires/avancer.txt", 'A', 1);
    charger_dico(&act[1], "word_dictionnary/lexique_en/actions/elementaires/reculer.txt", 'R', 1);
    charger_dico(&act[2], "word_dictionnary/lexique_en/actions/elementaires/tourner.txt", 'T', 0);
    charger_dico(&act[3], "word_dictionnary/lexique_en/actions/elementaires/stop.txt", 'S', 0);

    // Directions
    charger_dico(&dir[0], "word_dictionnary/lexique_en/actions/elementaires/gauche.txt", 'G', 90);
    charger_dico(&dir[1], "word_dictionnary/lexique_en/actions/elementaires/droite.txt", 'D', -90);

    // Macros
    charger_dico(&macros[0], "word_dictionnary/lexique_en/actions/macros/carre.txt", 'C', 0);
    charger_dico(&macros[1], "word_dictionnary/lexique_en/actions/macros/zigzag.txt", 'Z', 0);
    charger_dico(&macros[2], "word_dictionnary/lexique_en/actions/macros/demi_tour.txt", 'U', 0);

    // Nombres
    char path_nb[100];
    for (int i = 1; i <= 10; i++) {
        sprintf(path_nb, "word_dictionnary/lexique_en/nombres/%d.txt", i);
        charger_dico(&nb[i - 1], path_nb, 'N', i);
    }

    // Unités
    charger_dico(&units[0], "word_dictionnary/lexique_en/unites/metres.txt", 'm', 0);
    charger_dico(&units[1], "word_dictionnary/lexique_en/unites/centimetres.txt", 'c', 0);
    charger_dico(&units[2], "word_dictionnary/lexique_en/unites/decimetres.txt", 'l', 0);  /// arbitraire juste pour garder en char au lieu de char* (trop de segmentaqtion faults sinon et long à faire dans le temps imaprti)
    charger_dico(&units[3], "word_dictionnary/lexique_en/unites/degres.txt", 'd', 0);
    charger_dico(&units[4], "word_dictionnary/lexique_en/unites/pieds.txt", 'f', 0);
    charger_dico(&units[5], "word_dictionnary/lexique_en/unites/pouces.txt", 'i', 0);
}


void traitement_mode_vocal() {
    ListeCommandes* liste = init_liste();
    char buffer[MAX_LIGNE];

    liaisons[0].nb_syn = 0;
    liaisons_inv[0].nb_syn = 0;

    while (1) {
        // Appel du script Python ds le venv directement
        int retour = system("python3 src/module_tts.py");

        if (retour != 0) {
            printf("Erreur lors de l'execution du module vocal\n");
            break;
        }

        printf("%s\n", config("VOC_ACTIVE"));

        // une fois le script exécuté, on lit le fichier généré par Python
        FILE* f_vocal = fopen("output/pre_traitement/commande.txt", "r");

        if (f_vocal != NULL) {
            if (fgets(buffer, MAX_LIGNE, f_vocal) != NULL) {  // lis une ligne et retourne NULL si erreur ou fin de fichier => si y'a des lignes on boucle
                buffer[strcspn(buffer, "\n")] = 0;            // Nettoie en enlevant le \n
                printf(">>> Transcription reçue : \"%s\"\n", buffer);

                // Si c'est exit on quitte, sinon on traite
                if (strcmp(buffer, "exit") == 0) {
                    fclose(f_vocal);
                    printf("Vous avez dit 'exit'...\n");
                    break;
                }

                // meme logique que pour le mode texte
                liste->nb_commandes = 0;
                traiter_cmd(buffer, macros, 3, liaisons, 1, liaisons_inv, 1, act, 4, dir, 2, nb, 10, units, 6, liste);
                debug_afficherliste(liste);
                ecrire_commandes(liste, "output/post_traitement/commande.txt");
                char bufffer_commande[1024];
                commmandes_str(liste, bufffer_commande, 1024);
                envoyer_commande(bufffer_commande);
                // PARTIE CONFIRMATION POUR LES COMMANDES LONGUES
                // popen pour lire le retour d'une commande shell
                FILE* fp = popen("wc -l < output/post_traitement/commande.txt", "r");
                if (fp != NULL) {  // on vérifie que le terminal s'est bien ouvert
                    char result[16];
                    fgets(result, sizeof(result), fp);  // lit la sortie de la commande{
                    int nb_lignes = atoi(result);       // convertir en int

                    // si il y a plus de 30 lignes, on demande confirmation
                    if (nb_lignes > 30) {
                        printf("\nWarning, long command detected (%d elementary instructions) !\n", nb_lignes);

                        // on lance la lecture audio de la commande en la traduisant via python
                        system("python3 src/module_confirm.py");

                        // écouter la confirmation vocale
                        printf("\n>>> PARLEZ MAINTENANT (Dites 'OUI' ou 'NON')...\n");
                        system("python3 src/module_tts.py");

                        // FINIR ICI !!!!!

                        // on vérifie la réponse dans le fichier généré par le TTS
                        FILE* fp_grep = popen("grep -i \"oui\" output/speech_to_text/commande.txt", "r");
                        int confirmation = 0;

                        char test_buffer[128];
                        if (fgets(test_buffer, sizeof(test_buffer), fp_grep) != NULL) {
                            confirmation = 1;
                        }
                        pclose(fp_grep);

                        if (confirmation) {
                            printf(">>> CONFIRMATION VOCALE REÇUE. Exécution en cours...\n");
                        } else {
                            printf(">>> ANNULATION (Vous avez dit : '%s').\n", test_buffer);
                            printf(">>> La commande a été effacée.\n");

                            // On vide le fichier de commande pour ne pas l'envoyer à la simu
                            FILE* f_annul = fopen("output/post_traitement/commande.txt", "w");
                            if (f_annul) fclose(f_annul);
                        }
                    }
                    pclose(fp);
                }
            }
            fclose(f_vocal);
            /// on efface le fichier pour ne pas traiter 2x la mm commande :
            f_vocal = fopen("output/pre_traitement/commande.txt", "w");  // écrase le contenu avec rien = le vider
            fclose(f_vocal);
        } else {
            printf("Erreur : Impossible de lire le fichier de commande vocale.\n");
        }
    }
    free_liste(liste);
}

void traitement_mode_textuel(Langue langue) {
    ListeCommandes* liste = init_liste();
    char buffer[MAX_LIGNE];

    liaisons[0].nb_syn = 0;
    liaisons_inv[0].nb_syn = 0;

    while (1) {
        if (langue == FR)
            printf("\nENTRER COMMANDE (maximum 200 actions individuelles)\nÉcrire \"exit\" pour quitter\n>>> ");
        else {
            printf("\nENTER COMMAND (maximum 200 individual actions) >> ");
        }

        if (fgets(buffer, MAX_LIGNE, stdin) == NULL) break;

        buffer[strcspn(buffer, "\n")] = 0;  // nettoyer le \n
        if (strcmp(buffer, "exit") == 0) {
            break;
        }  // si le mot est exit on quitte

        liste->nb_commandes = 0;  // Reset manuel

        traiter_cmd(buffer, macros, 3, liaisons, 1, liaisons_inv, 1, act, 4, dir, 2, nb, 10, units, 6, liste);

        debug_afficherliste(liste);
        ecrire_commandes(liste, "output/commande.txt");
        char bufffer_commande[1024];
        commmandes_str(liste, bufffer_commande, 1024);
        envoyer_commande(bufffer_commande);
    }
    free_liste(liste);
}