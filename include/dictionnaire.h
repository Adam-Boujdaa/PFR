
/* AUTEUR : ADAM BOUJDAA                           */
/* DATE CREATION : 28/12/2025                     */
/*------------------------------------------------*/
/* DÉCLARATION DES FONCTIONS définies dans le .c */

#ifndef DICTIONNAIRE_H 
#define DICTIONNAIRE_H 
#define MAX_LIGNE 256 // nb max de caractères par ligne
#define MAX_SYN 100

// Enum types actions
typedef enum {
    CMD_AVANCE = 'A',
    CMD_RECULE = 'R',
    CMD_TOURNE_GAUCHE = 'G',
    CMD_TOURNE_DROITE = 'D',
    CMD_STOP = 'S',
    CMD_INCONNUE = '?'
} TypeAction;


// Enum pr les unités
typedef enum {
    UNIT_DEFAULT,
    UNIT_M,         // metres
    UNIT_CM,        // centimetres
    UNIT_DM,        // decimetre
    UNIT_DEG,       // degres
    UNIT_PIED,      // pieds
    UNIT_POUCE      // pouces
} TypeUnite;



// struct pr le dictionnaire 
typedef struct { 
    char code; // code commande (A, R, TD, TG, N), A= Avancer, R=reculer, D,G= Tourner à droite / gauche, N = nombre, U=unité 
    //on utilise pas l'énum en haut car ds le cas où c'est un nombre, ça sera juste N et ce n'est pas une action
    int valeur; // valeur num si c'est un chiffre entre -180 et 180 pour les rotations (TD ou TG), entre LONG_PIECE et LARG_PIECE pour les translations
    char *syn[MAX_SYN]; // tableau des synonymes, chargés depuis un txt lors de la fonction d'initialisation 
    int nb_syn; // nb synonymes chargés 
} Dico; 



// Struct pr une commande unitaire
typedef struct {
    TypeAction action;
    int valeur;         
    TypeUnite unite;    
} Commande;

// Noeud de la liste chainée
typedef struct Node {
    Commande data;
    struct Node *next;
} Node;


//LISTE CHAINÉE DE COMMANDES
typedef struct {
    Node *head;
    Node *tail;
    int taille;
} ListeCommandes;


//-----------------------------------------
// PROTOTYPES FONCTIONS : 


// --------------- DICTIONNAIRE ET OUTILS : 
// fct d'initialisation pr charger un dico depuis fichier txt 
// d=tableau de dictionnaires, il y en aura 1 pour les actions, 1 pour les directions(droite gauche) et un pour les nombres
// c = code interne, v = valeur numerique (pr les nombres)
void charger_dico(Dico *d, const char *fic, char c, int v);  

// fct nettoyage => mets toutes les lettres du en minuscule pour faciliter le traitement après
void nettoyer_txt(char *s); 

// fct pour chercher un mot ds un tableau de dicos 
// return index du dico trouvé ou -1
int trouver_mot(char *m, Dico tab[], int n); //m=mot

//fct pr liberer la mémoire => éviter les fuites mémoire
void liberer_dico(Dico *d);


//----------------------- LISTE : 
ListeCommandes* init_liste();
void ajouter_cmd(ListeCommandes *l, TypeAction act, int val, TypeUnite unit);
void liberer_liste(ListeCommandes *l);
void afficher_liste(ListeCommandes *l);


// fct principale du traitement 
//act = dictionnaire des actions, na = nombre d'actions possibles (3 car avancer ,recouler ou tourner)
//dir = dico des directions, nd = nb de de directions possibles (2 car droite ou gauche)
ListeCommandes* traiter_cmd(char *phrase, Dico actions[], int nb_actions, Dico directions[], int nb_directions, Dico nombres[], int nb_nombres, Dico unites[], int nb_unites);

#endif