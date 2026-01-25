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


//-------------------------------- DICTIONNAIRE ET OUTILS UTILES : 

// fct pr charger un dico depuis fichier txt 
// c = code interne, v = valeur numerique (pr les nombres) 
void charger_dico(Dico *d, const char *fic, char c, int v) { 
    d->code = c; 
    d->valeur = v; // valeur num si c'est un chiffre (entre -180 et 180 pour les rotations (TD ou TG), entre LONG_PIECE et LARG_PIECE pour les translations, sinon -200 (valeur arbitraire)
    //les valeurs par défaut pour les rotations sont de 90 pour la gauche sinon -90 pour la droite
    d->nb_syn = 0; 
    FILE *f = fopen(fic, "r"); //on ouvre en mode read
    if (f == NULL) { 
        fprintf(stderr, "TRACE : Pb ouverture %s\n", fic); 
        return; 
    } 

    char buf[MAX_LIGNE]; //création du buffer = zone temp pr stocker une ligne lue

    // lecture tant qu'on a place ds tab :
    while (fgets(buf, MAX_LIGNE, f) && (d->nb_syn < MAX_SYN)) {  //pendant que fgets lits qquechose et qu'on a de la place dans le tableau de synonymes.
        buf[strcspn(buf, "\n")] = 0; // virer \n du mot
        buf[strcspn(buf, "\r")] = 0; // virer \r du mot (pr windows)
        if (strlen(buf) > 0) {   //si la ligne n'est pas vide, pr éviter ne pas charger de lignes vides 
            d->syn[d->nb_syn] = malloc(strlen(buf)+1); // on enregistre le synonyme dans le dico, +1 pr laisser de l'espace pour le \0 qui sert à montrer la fin du mot
            strcpy(d->syn[d->nb_syn], buf); //copier le string dans buffer vers la case allouée juste avant
            d->nb_syn++; //incrémenter pr le prochaint synonyme
        } 
    } 
    fclose(f); //close le fichier
} 

// fct nettoyage (minuscule) 
void nettoyer_txt(char *s) { 
    for (int i = 0; s[i]; i++) { 
        s[i] = tolower((unsigned char)s[i]); //obligé d'utiliser unsigned pour éviter des bugs avec les caractères avec accents
    } 
} 

// fct cherche mot ds tableau de dicos 
// return index du mot dans le dico si trouvé ou -1 
int trouver_mot(char *m, Dico tab[], int n) { 
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < tab[i].nb_syn; j++) { 
            if (strcmp(m, tab[i].syn[j]) == 0) return i;  //comparer le mot cherché avec tt les synonymes du dico, si il y a 0 différence, strcmp(..)=0
        } 
    } 
    return -1; 
} 

// libérer unseul dico
void liberer_dico(Dico *d) {
    for(int i=0; i < d->nb_syn; i++) {
        free(d->syn[i]);
    }
    d->nb_syn = 0;
}


//fct pr liberer tt les dicos dans un tableau de dictionnaires => éviter les fuites mémoire
void liberer_tab_dico(Dico tab[], int n) {
    for(int i=0; i<n; i++){
        liberer_dico(&tab[i]);
    }
}