/* AUTEUR : ADAM BOUJDAA                           */
/* DATE CREATION : 28/12/2025                     */
/*------------------------------------------------*/
/* DÉFINITIONS DES FONCTIONS déclarées dans le .h */
/*Elles permettent de charger un dictionnaire, avec tous les synonymes d'un mot,
d'analyser une phrase donnée pour en extraire la commande exécutable 
pour ensuit l'envoyer au module de simulation*/  // A IMPLÉMENTER !


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 
#include "dictionnaire.h" 

// fct pr charger un dico depuis fichier txt 
// c = code interne, v = valeur numerique (pr les nombres) 
void charger_dico(Dico *d, const char *fic, char c, int v) { 
    d->code = c; 
    d->valeur = v; // valeur num si c'est un chiffre (entre -180 et 180 pour les rotations (TD ou TG), entre LONG_PIECE et LARG_PIECE pour les translations, sinon -200 (valeur arbitraire)
    //les valeurs par défaut pour les rotations sont de 90 pour la gauche sinon -90 pour la droite
    d->nb_syn = 0; 
    FILE *f = fopen(fic, "r"); 
    if (f == NULL) { 
        fprintf(stderr, "[ERREUR] Pb ouverture %s\n", fic); 
        return; 
    } 

    char buf[MAX_LIGNE]; 
    // lecture tant qu'on a place ds tab 
    while (fgets(buf, MAX_LIGNE, f) && d->nb_syn < MAX_SYN) { 
        buf[strcspn(buf, "\n")] = 0; // virer \n du mot
        if (strlen(buf) > 0) {   
            d->syn[d->nb_syn] = malloc(strlen(buf)+1);
            strcpy(d->syn[d->nb_syn], buf);
            d->nb_syn++;
        } 
    } 
    fclose(f); 
} 

// fct nettoyage (minuscule) 
void net_txt(char *s) { 
    for (int i = 0; s[i]; i++) { 
        s[i] = tolower((unsigned char)s[i]); 
    } 
} 

// fct cherche mot ds tableau de dicos 
// return index du mot dans le dico si trouvé ou -1 
int trouv_mot(char *m, Dico tab[], int n) { 
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < tab[i].nb_syn; j++) { 
            if (strcmp(m, tab[i].syn[j]) == 0) return i; 
        } 
    } 
    return -1; 
} 

// fct principale parsing , autant d'args pour la modularité 
// phrase        : phrase brute saisie par l'utilisateur ou issue du TTS
// actions[]     : dictionnaire des actions (avancer, reculer, tourner)
// nb_actions    : nombre d'actions possibles 3
// directions[]  : dictionnaire des directions (gauche, droite)
// nb_directions : nombre de directions possibles : 2
// nombres[]     : dictionnaire des nombres (un, deux, trois, ...)
// nb_nombres    : nombre de nombres reconnus : 20
//
// La fonction transforme une phrase en une suite de commandes : normalisée du type : [A:-:5] [T:D:90] avancer (tout droit car '-' au lieu de G ou D) de 5 metres puis tourner à droite de 90 degrés
void traiter_cmd(char *phrase, Dico actions[], int nb_actions, Dico directions[], int nb_directions, Dico nombres[], int nb_nombres){
    // on nettoye la phrase => tt en minuscule
    net_txt(phrase);

    // découpage de la phrase mot par mot (en tokens)
    char *mot_courant = strtok(phrase, " ");

    // Variables de l'instruction en cours de construction
    char action_courante = 0;       // Code de l'action (A, R, T)
    char direction_courante = 0;    // Code direction (G, D)
    int valeur_courante = -1;       // Valeur numérique (distance / angle)

    // Analyse de tous les mots de la phrase
    while(mot_courant != NULL){

        int indice;
        // 1) Recherche dans le dictionnaire des actions
        indice = trouv_mot(mot_courant, actions, nb_actions);
        if(indice != -1)
        {
            // Si une action était déjà en cours, on l'affiche avant d'en commencer une nouvelle
            if(action_courante != 0)
                printf("[%c:", action_courante);
                if(direction_courante != 0)
                    printf("%c:", direction_courante);
                else
                    printf("-:");

                if(valeur_courante != -1)
                    printf("%d] ", valeur_courante);
                else
                    printf("0] ");

            // Initialisation de la nouvelle action
            action_courante = actions[indice].code;
            direction_courante = 0;
            valeur_courante = -1;
        }
        else {
            // 2) Recherche dans le dictionnaire des directions
            indice = trouv_mot(mot_courant, directions, nb_directions);
            if(indice != -1){
                direction_courante = directions[indice].code;
            }
            else {
                // 3) Recherche dans le dictionnaire des nombres
                indice = trouv_mot(mot_courant, nombres, nb_nombres);
                if(indice != -1){
                    valeur_courante = nombres[indice].valeur;
                }
                else{
                    // 4) Vérification si le mot est un nombre écrit en chiffres
                    int est_nombre = 1;
                    for(int i = 0; mot_courant[i]; i++)
                        if(!isdigit(mot_courant[i])) est_nombre = 0;

                    if(est_nombre)
                        valeur_courante = atoi(mot_courant);  //transforme en int 
                }
            }
        }

        mot_courant = strtok(NULL, " ");
    }

    // Affichage de la dernière action détectée
    if(action_courante != 0)
        printf("[%c:%c:%d]\n", action_courante, (direction_courante ? direction_courante : '-'), (valeur_courante != -1 ? valeur_courante : 0));
    else {
        printf("Commande inconnue\n");
    }
}


void liberer_dico(Dico *d){
    for(int i=0;i<d->nb_syn;i++) {
        free(d->syn[i]);
    }
}