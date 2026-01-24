
/* AUTEUR : ADAM BOUJDAA                           */
/* DATE CREATION : 28/12/2025                     */
/*------------------------------------------------*/
/* DÉCLARATION DES FONCTIONS définies dans le .c */

#ifndef DICTIONNAIRE_H 
#define DICTIONNAIRE_H 
#define MAX_LIGNE 256 // nb max de caractères par ligne
#define MAX_SYN 100 //nb max de synonymes dans un dictionnaire

//Enum pr les langues
typedef enum { FR, EN } Langue;


// Struct pr le dictionnaire 
typedef struct { 
    char code; // code commande (A, R, D, G, N), A= Avancer, R=reculer, D,G= Tourner à droite / gauche, N = nombre, U=unité, 
    //M =Macro, Lo = mot de liaison dans l'ordre chronologique normal,Li= dans le sens inverse
    int valeur; // valeur num si c'est un chiffre, sinon valeur par défaut de l'action (90degrés pour les rotations ou 1mètre pr les translaitons)
    char *syn[MAX_SYN]; // tableau des synonymes, chargés depuis un txt lors de la fonction d'initialisation 
    int nb_syn; // nb synonymes chargés 
} Dico; 



//-----------------------------------------
// PROTOTYPES FONCTIONS : 


// --------------- DICTIONNAIRE ET OUTILS : 
// fct d'initialisation pr charger un dico depuis fichier txt 
// d=tableau de dictionnaires, il y en aura 1 pour les actions, 1 pour les directions(droite gauche), 1 pour les nombres, 1 pour les unités, 1 pour les macros, 1 pour les mots de liaison (dans le sens normal et dans le sens inverse),
// c = code interne, v = valeur numerique (pr les nombres)
void charger_dico(Dico *d, const char *fic, char c, int v);  

// fct nettoyage => mets toutes les lettres du en minuscule pour faciliter le traitement après
void nettoyer_txt(char *s); 

// fct pour chercher un mot ds un tableau de dicos 
// return index du dico trouvé ou -1
int trouver_mot(char *m, Dico tab[], int n); //m=mot

//fct pr liberer la mémoire => éviter les fuites mémoire
void liberer_dico(Dico *d);

void liberer_tab_dico(Dico tab[], int n);

#endif