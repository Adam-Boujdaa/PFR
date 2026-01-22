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


//-------------------------------- DICTIONNAIRE ET OUTILS POUR : 

// fct pr charger un dico depuis fichier txt 
// c = code interne, v = valeur numerique (pr les nombres) 
void charger_dico(Dico *d, const char *fic, char c, int v) { 
    d->code = c; 
    d->valeur = v; // valeur num si c'est un chiffre (entre -180 et 180 pour les rotations (TD ou TG), entre LONG_PIECE et LARG_PIECE pour les translations, sinon -200 (valeur arbitraire)
    //les valeurs par défaut pour les rotations sont de 90 pour la gauche sinon -90 pour la droite
    d->nb_syn = 0; 
    FILE *f = fopen(fic, "r"); 
    if (f == NULL) { 
        fprintf(stderr, "TRACE : Pb ouverture %s\n", fic); 
        return; 
    } 

    char buf[MAX_LIGNE]; //buffer = zone temp pr stocker une ligne lue

    // lecture tant qu'on a place ds tab :
    while (fgets(buf, MAX_LIGNE, f) && d->nb_syn < MAX_SYN) {  //while fgets lits qquechose et qu'on a de la place dans le tableau de synonymes.
        buf[strcspn(buf, "\n")] = 0; // virer \n du mot
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
int trouv_mot(char *m, Dico tab[], int n) { 
    for (int i = 0; i < n; i++) { 
        for (int j = 0; j < tab[i].nb_syn; j++) { 
            if (strcmp(m, tab[i].syn[j]) == 0) return i; 
        } 
    } 
    return -1; 
} 

void liberer_dico_tab(Dico tab[], int n) {
    for(int i=0; i<n; i++){
        for(int j=0; j<tab[i].nb_syn; j++){
            free(tab[i].syn[j]);
        }
    }
}

//-------------------------------------- LISTE COMMANDE : 

ListeCommandes* init_liste() { //comme en tp de FIFO
    ListeCommandes *l = malloc(sizeof(ListeCommandes));
    l->head = NULL;
    l->tail = NULL;
    l->taille = 0;
    return l;
}

void ajouter_cmd(ListeCommandes *l, TypeAction act, int val, TypeUnite unit) {
    Node *n = malloc(sizeof(Node));
    n->data.action = act;
    n->data.unite = unit;
    n->next = NULL;

    // Valeurs par défaut si non précisé (-1)
    if (val == -1) {
        if (act == CMD_AVANCE || act == CMD_RECULE) n->data.valeur = 1; 
        else if (act == CMD_TOURNE_GAUCHE) n->data.valeur = 90; //on fait dans le sens trigo A voir si c'est compatible avec le module de simulation
        else if (act == CMD_TOURNE_DROITE) n->data.valeur = -90;
        else n->data.valeur = 0;
    } 
    else {
        n->data.valeur = val;
        }

    if (l->head == NULL) { 
        l->head = n; l->tail = n; 
    } 
    else { 
        l->tail->next = n;
        l->tail = n; 
    }
    l->taille++;
}

void liberer_liste(ListeCommandes *l) { //comme en tp aussi
    Node *tmp = l->head;
    while(tmp != NULL) {
        Node *suiv = tmp->next;
        free(tmp);
        tmp = suiv;
    }
    free(l);
}

void afficher_liste(ListeCommandes *l) {
    Node *tmp = l->head;
    printf("--- LISTE COMMANDES (%d) ---\n", l->taille);
    while(tmp != NULL) {
        printf("ACT:%c | VAL:%d | UNIT:%d\n", tmp->data.action, tmp->data.valeur, tmp->data.unite);
        tmp = tmp->next;
    }
    printf("----------------------------\n");
}

 
// ---------------------------------------------------- FONCTION TRAITEMENT DES COMMANDES :

// fct principale parsing , autant d'args pour la modularité 
// phrase        : phrase brute saisie par l'utilisateur ou issue du TTS
// actions[]     : dictionnaire des actions (avancer, reculer, tourner)
// nb_actions    : nombre d'actions possibles 3
// directions[]  : dictionnaire des directions (gauche, droite)
// nb_directions : nombre de directions possibles : 2
// nombres[]     : dictionnaire des nombres (un, deux, trois, ...)
// nb_nombres    : nombre de nombres reconnus : 20
// unites[]      : dictionnaire des unités (m, cm, degres...)
// nb_unites     : nombre d'unités possibles
//
// La fonction transforme une phrase en une liste chainée de commandes normalisées
ListeCommandes* traiter_cmd(char *phrase, Dico actions[], int nb_actions, Dico directions[], int nb_directions, Dico nombres[], int nb_nombres, Dico unites[], int nb_unites){
    
    ListeCommandes *liste = init_liste();

    // on nettoye la phrase => tt en minuscule
    net_txt(phrase);

    // découpage de la phrase mot par mot (en tokens)
    char *mot_courant = strtok(phrase, " ");

    // Variables de l'instruction en cours de construction (Tampons)
    TypeAction action_courante = 0;    // Code de l'action (A, R, T)
    TypeAction direction_courante = 0; // Code direction (G, D) pour modifier l'action
    int valeur_courante = -1;          // Valeur numérique (distance / angle)
    TypeUnite unite_courante = UNIT_DEFAULT; // Unité (m, cm, deg...)

    // Analyse de tous les mots de la phrase
    while(mot_courant != NULL){

        // --- GESTION DES CAS SPECIAUX (MACROS & SÉPARATEURS) ---

        // Cas Macro : carré
        if (strcmp(mot_courant, "carre") == 0) {
            // On injecte 4 cotés directement dans la liste
            for(int k=0; k<4; k++) {
                ajouter_cmd(liste, CMD_AVANCE, 1, UNIT_M); // 1m
                ajouter_cmd(liste, CMD_TOURNE_DROITE, 90, UNIT_DEG); // 90 deg
            }
            mot_courant = strtok(NULL, " ");
            continue; // On passe au mot suivant
        }

        // Cas Séparateur : "puis" ou "et"
        // Si on croise un séparateur, on valide l'action en cours avant de continuer
        if (strcmp(mot_courant, "puis") == 0 || strcmp(mot_courant, "et") == 0) {
            if (action_courante != 0) {
                // Logique de consolidation (fusion Action + Direction)
                char act_finale = action_courante;
                if(action_courante == 'T') {
                    if(direction_courante == 'G') act_finale = CMD_TOURNE_GAUCHE;
                    else if(direction_courante == 'D') act_finale = CMD_TOURNE_DROITE;
                }
                // Ajout dans la liste chainée
                ajouter_cmd(liste, act_finale, valeur_courante, unite_courante);
                
                // Reset des tampons pr la suite
                action_courante = 0; direction_courante = 0; valeur_courante = -1; unite_courante = UNIT_DEFAULT;
            }
            mot_courant = strtok(NULL, " ");
            continue;
        }

        // --- RECHERCHE DANS LES DICTIONNAIRES ---

        int indice;
        // 1) Recherche dans le dictionnaire des actions
        indice = trouv_mot(mot_courant, actions, nb_actions);
        if(indice != -1)
        {
            // Si une action était déjà en cours (ex: "Avance... Recule"), on la sauvegarde d'abord
            if(action_courante != 0) {
                char act_finale = action_courante;
                if(action_courante == 'T') {
                    if(direction_courante == 'G') act_finale = CMD_TOURNE_GAUCHE;
                    else if(direction_courante == 'D') act_finale = CMD_TOURNE_DROITE;
                }
                ajouter_cmd(liste, act_finale, valeur_courante, unite_courante);
                // Reset sauf pour la nouvelle action
                direction_courante = 0; valeur_courante = -1; unite_courante = UNIT_DEFAULT;
            }
            // Initialisation de la nouvelle action
            action_courante = actions[indice].code;
        }
        else {
            // 2) Recherche dans le dictionnaire des directions
            indice = trouv_mot(mot_courant, directions, nb_directions);
            if(indice != -1){
                direction_courante = directions[indice].code;
            }
            else {
                // 3) Recherche dans le dictionnaire des nombres (mots "dix")
                indice = trouv_mot(mot_courant, nombres, nb_nombres);
                if(indice != -1){
                    valeur_courante = nombres[indice].valeur;
                }
                else {
                    // 4) Recherche dans le dictionnaire des unités
                    indice = trouv_mot(mot_courant, unites, nb_unites);
                    if(indice != -1){
                         unite_courante = unites[indice].valeur;
                    }
                    else {
                        // 5) Vérification si le mot est un nombre écrit en chiffres ("10")
                        int est_nombre = 1;
                        for(int i = 0; mot_courant[i]; i++)
                            if(!isdigit(mot_courant[i])) est_nombre = 0;

                        if(est_nombre && strlen(mot_courant) > 0)
                            valeur_courante = atoi(mot_courant);  // transforme en int 
                    }
                }
            }
        }

        mot_courant = strtok(NULL, " ");
    }

    // Sauvegarde de la dernière action détectée (après la fin de la boucle)
    if(action_courante != 0) {
        char act_finale = action_courante;
        if(action_courante == 'T') {
            if(direction_courante == 'G') act_finale = CMD_TOURNE_GAUCHE;
            else if(direction_courante == 'D') act_finale = CMD_TOURNE_DROITE;
        }
        ajouter_cmd(liste, act_finale, valeur_courante, unite_courante);
    }

    return liste;
}