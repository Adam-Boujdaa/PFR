// AUTEUR : ADAM BOUJDAA
// DATE CREATION : Janvier 2026
// fichier principal de traitement texte

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/dictionnaire.h"
#include "../include/parseur.h"

// À LIRE DANS LE FICHIER CONFIG PLUS TARD
Langue LANGUE_CHOISIE = FR; 
char MODE = 'V'; 

int main() {
    printf("=== TRACE: CHARGEMENT DES DICOS ===\n");


    Dico act[4];
    Dico dir[2];
    Dico macros[3];
    Dico nb[10]; //10 fichiers de nombres mtn      
    Dico units[6];
    Dico liaisons[1]; 
    Dico liaisons_inv[1];

    // Initialisation des liaisons à vide => à implémenter plus tard 
    liaisons[0].nb_syn = 0;
    liaisons_inv[0].nb_syn = 0;

    // CHARGEMENT EN DUR SELON LA LANGUE
    if (LANGUE_CHOISIE == FR) {
        printf("=== Chargement Langue : FRANCAIS ===\n");

        // Actions
        charger_dico(&act[0], "word_dictionnary/lexique_fr/actions/elementaires/avancer.txt", 'A', 1);
        charger_dico(&act[1], "word_dictionnary/lexique_fr/actions/elementaires/reculer.txt", 'R', 1);
        charger_dico(&act[2], "word_dictionnary/lexique_fr/actions/elementaires/tourner.txt", 'T', 0);
        charger_dico(&act[3], "word_dictionnary/lexique_fr/actions/elementaires/stop.txt",    'S', 0);

        // Directions
        charger_dico(&dir[0], "word_dictionnary/lexique_fr/actions/elementaires/gauche.txt", 'G', 90);
        charger_dico(&dir[1], "word_dictionnary/lexique_fr/actions/elementaires/droite.txt", 'D', -90);

        // Macros
        charger_dico(&macros[0], "word_dictionnary/lexique_fr/actions/macros/carre.txt", 'C', 0);
        charger_dico(&macros[1], "word_dictionnary/lexique_fr/actions/macros/zigzag.txt", 'Z', 0);
        charger_dico(&macros[2], "word_dictionnary/lexique_fr/actions/macros/demi_tour.txt", 'U', 0);

        // Nombres
        char path_nb[100];
        for(int i=1; i<=10; i++) {
            sprintf(path_nb, "word_dictionnary/lexique_fr/nombres/%d.txt", i); //sprintf fais la concaténation envoyée dans path_nb
            charger_dico(&nb[i-1], path_nb, 'N', i);
        }

        // Unités
        charger_dico(&units[0], "word_dictionnary/lexique_fr/unites/metres.txt",      'm', 0);
        charger_dico(&units[1], "word_dictionnary/lexique_fr/unites/centimetres.txt", 'c', 0);
        charger_dico(&units[2], "word_dictionnary/lexique_fr/unites/decimetres.txt",  'l', 0);
        charger_dico(&units[3], "word_dictionnary/lexique_fr/unites/degres.txt",      'd', 0);
        charger_dico(&units[4], "word_dictionnary/lexique_fr/unites/pieds.txt",       'p', 0);
        charger_dico(&units[5], "word_dictionnary/lexique_fr/unites/pouces.txt",      'i', 0);
    }
    else {
        printf("=== Loading Language : ENGLISH ===\n");

        // Actions
        charger_dico(&act[0], "word_dictionnary/lexique_en/actions/elementaires/avancer.txt", 'A', 1);
        charger_dico(&act[1], "word_dictionnary/lexique_en/actions/elementaires/reculer.txt", 'R', 1);
        charger_dico(&act[2], "word_dictionnary/lexique_en/actions/elementaires/tourner.txt", 'T', 0);
        charger_dico(&act[3], "word_dictionnary/lexique_en/actions/elementaires/stop.txt",    'S', 0);

        // Directions
        charger_dico(&dir[0], "word_dictionnary/lexique_en/actions/elementaires/gauche.txt", 'G', 90);
        charger_dico(&dir[1], "word_dictionnary/lexique_en/actions/elementaires/droite.txt", 'D', -90);

        // Macros
        charger_dico(&macros[0], "word_dictionnary/lexique_en/actions/macros/carre.txt", 'C', 0);
        charger_dico(&macros[1], "word_dictionnary/lexique_en/actions/macros/zigzag.txt", 'Z', 0);
        charger_dico(&macros[2], "word_dictionnary/lexique_en/actions/macros/demi_tour.txt", 'U', 0);

        // Nombres
        char path_nb[100];
        for(int i=1; i<=10; i++) {
            sprintf(path_nb, "word_dictionnary/lexique_en/nombres/%d.txt", i);
            charger_dico(&nb[i-1], path_nb, 'N', i);
        }

        // Unités
        charger_dico(&units[0], "word_dictionnary/lexique_en/unites/metres.txt",      'm', 0);
        charger_dico(&units[1], "word_dictionnary/lexique_en/unites/centimetres.txt", 'c', 0);
        charger_dico(&units[2], "word_dictionnary/lexique_en/unites/decimetres.txt",  'l', 0);
        charger_dico(&units[3], "word_dictionnary/lexique_en/unites/degres.txt",      'd', 0);
        charger_dico(&units[4], "word_dictionnary/lexique_en/unites/pieds.txt",       'p', 0);
        charger_dico(&units[5], "word_dictionnary/lexique_en/unites/pouces.txt",      'i', 0);
    }

    // Init pour le parsing
    ListeCommandes *liste = init_liste();
    char buffer[MAX_LIGNE];

    if (LANGUE_CHOISIE == FR)
        printf("Prêt à lire vos commandes, écrire exit pour sortir\n");
    else {
        printf("Ready to read your commands, type exit to quit\n");
    }

    // Boucle principale
    while(1) {
        if(MODE == 'T') {
            if (LANGUE_CHOISIE == FR)
                printf("\nENTRER COMMANDE (maximum 200 actions individuelles) >>> ");
            else   {
                printf("\nENTER COMMAND (maximum 200 individual actions) >> ");
            }

            if (fgets(buffer, MAX_LIGNE, stdin) == NULL) break;
            
            buffer[strcspn(buffer, "\n")] = 0; //nettoyer le \n
            if (strcmp(buffer, "exit") == 0) break; // si le mot est exit on quitte

            liste->nb_commandes = 0; // Reset manuel

            traiter_cmd(buffer, macros, 3, liaisons, 1, liaisons_inv, 1, act, 4, dir, 2, nb, 10, units, 6, liste);

            debug_afficherliste(liste);
            ecrire_commandes(liste, "output/commande.txt");
        } 
        else if (MODE == 'V') {
            printf("Mode vocal active... (Lancement Python)\n");
            
            // Appel du script Python ds le venv directement
            int retour = system("./venv_pour_tts/bin/python3 src/module_tts.py");
            
            if (retour != 0) {
                printf("Erreur lors de l'execution du module vocal.\n");
                MODE = 'T'; // Retourner au mode textuel en cas d'erreur
                continue;
            }

            // une fois le script exécuté, on lit le fichier généré par Python
            FILE *f_vocal = fopen("output/pre_traitement/commande.txt", "r");
            
            if (f_vocal != NULL) {
                if (fgets(buffer, MAX_LIGNE, f_vocal) != NULL) { //lis une ligne et retourne NULL si erreur ou fin de fichier => si y'a des lignes on boucle
                    buffer[strcspn(buffer, "\n")] = 0; // Nettoie en enlevant le \n
                    printf(">>> Transcription reçue : \"%s\"\n", buffer);

                    // Si c'est exit on quitte, sinon on traite
                    if (strcmp(buffer, "exit") == 0) break;

                    // meme logique que pour le mode texte
                    liste->nb_commandes = 0;
                    traiter_cmd(buffer, macros, 3, liaisons, 1, liaisons_inv, 1, act, 4, dir, 2, nb, 10, units, 6, liste);
                    debug_afficherliste(liste);
                    ecrire_commandes(liste, "output/commande.txt");
                }
                fclose(f_vocal);
            } else {
                printf("Erreur : Impossible de lire le fichier de commande vocale.\n");
            }

            // Retour automatique en mode texte après une commande vocale
            MODE = 'T';
        }
        
    }
    free_liste(liste); 
    return 0;
}