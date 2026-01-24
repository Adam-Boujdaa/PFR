
/* AUTEUR : ADAM BOUJDAA                            */
/* DATE CREATION : 21/01/2026                      */
/*------------------------------------------------*/
/* Fichier c contenant les définitions de fonctions pour le traitement texte, on le sépare du dictionnaire  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/parseur.h"
#include "../include/dictionnaire.h"


// Initialise la liste de commandes
ListeCommandes* init_liste() {
    ListeCommandes *l = malloc(sizeof(ListeCommandes));
    if (l != NULL) { //au cas où il n'y a plus de place ds la memoire
        l->nb_commandes = 0;
    }
    return l;
}

void free_liste(ListeCommandes *l) {
    if (l != NULL) {
        l->nb_commandes = 0;
        // Pas besoin de free les éléments internes car c'est un tableau statique
        free(l);
    }
}

void init_commande(Commande *c) {
    if (c != NULL) {
        c->action = 0;
        c->direction = 0;
        c->valeur = -1;         // -1 => pas de valeur saisie
        c->unite = 0;
    }
}


// Ajoute une commande à la liste (si de la place est disponible) + attribue des valeurs par défaut si il n'y en a pas
void ajouter_commande(Commande c, ListeCommandes *l) {
    if (l->nb_commandes < MAX_COMMANDES) {
        if (c.action == 'S') { // STOP
            c.valeur = 0;
            c.direction = '-';
            c.unite = '-'; // Pas d'unité pour un stop
        }
        
        else {
        // si pas de valeur alors ça reste à -1
            if (c.valeur == -1) {
                if (c.action == 'A' || c.action == 'R') { //si on avance ou recule c'est 1m par défaut
                    c.valeur = 1; 
                    if(c.unite == 0) c.unite = 'm'; //mètres
                }

                else if (c.action == 'T') {
                    if (c.direction == 'G') c.valeur = 90; //sens trigo
                    else if (c.direction == 'D') c.valeur = -90; 
                    else c.valeur = -90; //tourne a droite par défaut si il n'y a pas de direction
                }
                if (c.unite == 0) {
                    if (c.action == 'A' || c.action == 'R') {
                        c.unite = 'm'; //mètres par défaut pour avancer/recule
                    } else if (c.action == 'T') {
                        c.unite = 'd'; //degrés par défaut pour tourner
                    }
                }
            }
        }

        l->cmnds[l->nb_commandes] = c;
        l->nb_commandes++;
    } else {
        fprintf(stderr, "TRACE : Liste de commandes pleine, éxécutez ce qu'il y a dans la liste puis recréez en une.\n");
    }
}

// Affiche la liste pour le débuggage
void debug_afficherliste(ListeCommandes *l) {
    fprintf(stderr,"--- TRACE : DEBUG LISTE (%d commandes) ---\n", l->nb_commandes);
    for(int i=0; i < l->nb_commandes; i++) {
        fprintf(stderr,"CMND N°%d ACT:%c | DIR:%c | VAL:%.2f | UNIT:%c\n", i, l->cmnds[i].action, l->cmnds[i].direction, l->cmnds[i].valeur, l->cmnds[i].unite);
    }
    fprintf(stderr,"----------------------------------\n");
}

// Fonction principale de parsing
void traiter_cmd(char *phrase, 
    Dico macros[], int nb_macros, //macro = dico des macros (carré, etc)
    Dico mots_liaison[], int nb_mots_liaison, //mots_liaison = dico des mots de liaison (puis, et)
    Dico mots_liaison_inv[], int nb_mots_liaison_inv, //mots_liaison_inv = dico des mots de liaison dans le sens inverse (d'abord, ensuite)
    Dico actions[], int nb_actions, //act = dictionnaire des actions, nb_actions=3 car avancer ,recouler ou tourner)
    Dico directions[], int nb_directions, //dir = dico des directions, nd = nb de de directions possibles (2 car droite ou gauche)
    Dico nombres[], int nb_nombres, //idem pour les nombres
    Dico unites[], int nb_unites, //et pour les unités
    ListeCommandes *liste){
       
        // Nettoyage
        nettoyer_txt(phrase);
        char *mot_courant = strtok(phrase, " "); //Tokenize par espace 

        // commande en cours de construction
        Commande cmd_temp;
        init_commande(&cmd_temp);

        // Bool pr savoir si une action est détectée => sert pr la boucle
        int action_detectee = 0;

        while(mot_courant != NULL) {

            // Gestion des macros 
            int idx = trouver_mot(mot_courant, macros, nb_macros); //on cherche dans les dictionnaires de macros si le mot est dedans
            if (idx != -1) { //si on a trouvé une correspondance au mot donné dans un des dico de macros

                // Si une commande était en cours, on la valide d'abord
                if (action_detectee) {
                    ajouter_commande(cmd_temp, liste);
                    init_commande(&cmd_temp);
                    action_detectee = 0;
                }

                char code_macro = macros[idx].code;
                //logique des macros codé en dur par manque de temps
                if (code_macro == 'C') { // Carré
                        for(int k=0; k<4; k++) {
                            Commande c1 = {'A', 0, 0.5, 'm'}; ajouter_commande(c1, liste); //avancer tout droit de 1mètre
                            Commande c2 = {'T', 'D', 90, 'd'}; ajouter_commande(c2, liste); //tourner de 90degrés à droite
                        }
                    }
                    else if (code_macro == 'Z') { // Zigzag
                        Commande c1 = {'A', 0, 0.5, 'm'}; ajouter_commande(c1, liste);
                        Commande c2 = {'T', 'D', 45, 'd'}; ajouter_commande(c2, liste);
                        Commande c3 = {'A', 0, 0.5, 'm'}; ajouter_commande(c3, liste);
                        Commande c4 = {'T', 'G', 45, 'd'}; ajouter_commande(c4, liste);
                    }
                    else if (code_macro == 'U') { // Demi-tour
                        Commande c = {'T', 0, 180, 'd'}; 
                        ajouter_commande(c, liste);
                    }
                    
                    mot_courant = strtok(NULL, " "); //on passe au mot suivant
                    continue; //on skip tous les autres cas de mots pour passer directent au prochain mot=> on revient au début de la boucle
            } 
            
            // Gestion des mots de liaison dans le sens normal, le sens inverse à implémenter via dicos plus tard en utilisant fonction d'inversement entre 2 commandes ds la liste en utilisant des commandes* temp
            if (strcmp(mot_courant, "puis") == 0 || strcmp(mot_courant, "et") == 0) { //strcmp = string compare => cherche les diff entre 2 string
                if (action_detectee) {
                    ajouter_commande(cmd_temp, liste); // ajouter la commande en cours
                    init_commande(&cmd_temp); // reset
                    action_detectee = 0;
                }
                mot_courant = strtok(NULL, " ");
                continue;
            }

            //on cherche dans le tab de dico d'actions si c'est une action :

            idx = trouver_mot(mot_courant, actions, nb_actions);
            if (idx != -1) {
               //on sauvegarde la commande d'avant si il y ena une
                if (action_detectee) {
                    ajouter_commande(cmd_temp, liste);
                    init_commande(&cmd_temp); 
                }
                cmd_temp.action = actions[idx].code; 
                action_detectee = 1;
            }

            //on cherche si c'est une direction 
            else {
                idx = trouver_mot(mot_courant, directions, nb_directions);
                if (idx != -1) {
                    cmd_temp.direction = directions[idx].code;
                }
            
                else {
                    // on cherche si c'est un nombre écrit en lettres ou mal transcris
                    idx = trouver_mot(mot_courant, nombres, nb_nombres);
                    if (idx != -1) {
                        cmd_temp.valeur = nombres[idx].valeur;
                    }
                    else {
                        // si c'est une unité
                        idx = trouver_mot(mot_courant, unites, nb_unites);
                        if (idx != -1) {
                            cmd_temp.unite = unites[idx].code;
                        }
                        else {
                            // si c'est un nombre (écrit en chiffres)
                            if (isdigit(mot_courant[0]) || (mot_courant[0] == '-' && isdigit(mot_courant[1]))) {
                                cmd_temp.valeur = atoi(mot_courant); //convertit le string en int
                            }
                        }
                    }
                }
            }
            mot_courant = strtok(NULL, " "); //passer au mot suivant
        }

        // Ajouter la dernière commande, celle de la fin du while
        if (action_detectee) {
            ajouter_commande(cmd_temp, liste); //on ajoute la commande en cours
            init_commande(&cmd_temp); //reset
        }    
    }

// Écriture formatée CSV : Action,Direction,Valeur,Unite
void ecrire_commandes(ListeCommandes *l, const char* fichier) {
    FILE *f = fopen(fichier, "w");
    if (f == NULL) {
        perror("Erreur ouverture fichier sortie");
        return;
    }

    for (int i = 0; i < l->nb_commandes; i++) {
        char act = l->cmnds[i].action;
        char dir = l->cmnds[i].direction;
        float val = l->cmnds[i].valeur;
        int unit = l->cmnds[i].unite;

        // Si pas de direction, on met un tiret '-'
        char dir_str = (dir == 0) ? '-' : dir;

        // Format demandé : A,-,2,unite
        fprintf(f, "%c,%c,%.2f,%d\n", act, dir_str, val, unit);
    }
    
    // Ajout d'une ligne pour indiquer la fin si nécessaire ou flush
    fclose(f);
    printf("Commandes exportees vers %s\n", fichier);
}