
/* AUTEUR : ADAM BOUJDAA                           */
/* DATE CREATION : 28/12/2025                     */
/*------------------------------------------------*/
/* DÉCLARATION DES FONCTIONS définies dans le .c */

#ifndef PARSEUR_H 
#define PARSEUR_H 
#include "dictionnaire.h"

#define MAX_COMMANDES 200  //nb max de commandes élémentaires à la fois=taille du tableau statiuque 



// Struct pr une commande unitaire
typedef struct {
    char action; //A = avancer, R= Reculer, T = tourner ...
    char direction; //D = droite, G = gauche
    float valeur;      //valeur par défaut sinon celle saisie  
    char unite;    //enum dans dictionnaire.h
} Commande;

//liste statique des commandes+ nb de commandes (oui j'aime les pointeurs)
typedef struct {
    Commande cmnds[MAX_COMMANDES];
    int nb_commandes;
} ListeCommandes;

//initialisation d'une commande solo :
void init_commande(Commande *c);
void free_commande(Commande *c);


//----------------------- LISTE : 
ListeCommandes* init_liste();
void ajouter_commande(Commande c, ListeCommandes *l); //passage par adresse pr bien modif la liste de commandes
void debug_afficherliste(ListeCommandes *l);
void free_liste(ListeCommandes *l);




// fct principale du traitement 

void traiter_cmd(char *phrase, 
    Dico macros[], int nb_macros, //macro = dico des macros (carré, etc)
    Dico mots_liaison[], int nb_mots_liaison, //mots_liaison = dico des mots de liaison (puis, et)
    Dico mots_liaison_inv[], int nb_mots_liaison_inv, //mots_liaison_inv = dico des mots de liaison dans le sens inverse (d'abord, ensuite)
    Dico actions[], int nb_actions, //act = dictionnaire des actions, nb_actions=3 car avancer ,recouler ou tourner)
    Dico directions[], int nb_directions, //dir = dico des directions, nd = nb de de directions possibles (2 car droite ou gauche)
    Dico nombres[], int nb_nombres, //idem pour les nombres
    Dico unites[], int nb_unites, //et pour les unités
    ListeCommandes *liste); //liste des commandes à remplir

void ecrire_commandes(ListeCommandes *l, const char* fichier);
void commmandes_str(ListeCommandes *l, char *buffer, size_t buffer_size);

#endif