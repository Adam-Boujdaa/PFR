
/* AUTEUR : ADAM BOUJDAA                           */
/* DATE CREATION : 28/12/2025                     */
/*------------------------------------------------*/
/* DÉCLARATION DES FONCTIONS définies dans le .c */

#ifndef DICTIONNAIRE_H 
#define DICTIONNAIRE_H 
#define MAX_LIGNE 256 
#define MAX_SYN 100 // Augmenté pr supporter tes fichiers larges 

// struct pr le dictionnaire 
typedef struct { 
    char code; // code commande (A, R, TD, TG, N), A= Avancer, R=reculer, TD,TG= Tourner à droite / gauche, N = nombre 
    int valeur; // valeur num si c'est un chiffre entre -180 et 180 pour les rotations (TD ou TG), entre LONG_PIECE et LARG_PIECE pour les translations
    char *syn[MAX_SYN]; // tableau des synonymes, chargés depuis un txt lors de la fonction d'initialisation 
    int nb_syn; // nb synonymes chargés 
} Dico; 

//-----------------------------------------
// PROTOTYPES FONCTIONS : 

// fct d'initialisation pr charger un dico depuis fichier txt 
// d=tableau de dictionnaires, il y en aura 1 pour les actions, 1 pour les directions(droite gauche) et un pour les nombres
// c = code interne, v = valeur numerique (pr les nombres)
void charger_dico(Dico *d, const char *fic, char c, int v);  

// fct nettoyage => mets toutes les lettres du en minuscule pour faciliter le traitement après
void net_txt(char *s); 

// fct pour chercher un mot ds un tableau de dicos 
// return index du dico trouvé ou -1
int trouver_mot(char *m, Dico tab[], int n); //m=mot

// fct principale du traitement 
//act = dictionnaire des actions, na = nombre d'actions possibles (3 car avancer ,recouler ou tourner)
//dir = dico des directions, nd = nb de de directions possibles (2 car droite ou gauche)
//nb = dico de nombres , nn = nombre de nombres possibles (20 car la pièce simulée est de 10x10m, on garde de la marge au cas où)
void traiter_cmd(char *phrase, Dico act[], int na, Dico dir[], int nd, Dico nb[], int nn); 

//fct pr liberer la mémoire => éviter les fuites mémoire
void liberer_dico(Dico *d);

#endif