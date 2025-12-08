#include <stdio.h>
#include <stdlib.h>
#include <string.h> // pr strlen() et strcmp
#include <ctype.h> // obligé pour isdigit()


#define CHEMIN_VERBES "./verbes/"    //chemin des fichiers de synonymes
#define MAX_CMD 20                   //nb max d'actions dans une commande
#define LONG_CMD 10                  //longueur max d'une commande en elle meme (exemple 90D = tourne de 90° droite, A20 = Avancer de 20mètres

//tableau d'actions, en statique pr l'instant , on fera peut etre une file dynamique ou juste un tableau dynamique après 
char tab_actions[MAX_CMD][LONG_CMD];
int idx_act = 0; //index de l'action courante



//vérifie si un mot est un verbe (se termine en ez,er,é) => c'est simplifié pour la 
// démo pour voir si le client est d'accord avec cette façon de faire dans la globalité
int est_verbe(char *mot) 
{
    int lg_mot = strlen(mot);
    
    if (lg_mot < 2) {return 0;} //un mot de 2 charactères est forcément pas un verbe (en français en tout cas) + pr voir si on peut tester la terminaison du mot

    char *fin_mot = mot + lg_mot; // l'adresse de la fin du mot ds le tableau de char est à l'adresse du premier char + longueur du mot (cf cours)

    // Tests des terminaisons en "er" et "ez" et "e"
    if (lg_mot >= 2) {
        if (strcmp(fin_mot - 2, "ez") == 0) return 1;
        else if (strcmp(fin_mot - 2, "er") == 0) return 1;
        else if (mot[lg_mot - 1] == 'e') return 1;
    }

    else {
    return 0;
    }

}

int synonyme_dans_avancer(mot){
    construire un truc qui va faire grep dans le terminal dans le fichier, si le retour est non vide alors {
        return 1;
    }
    else {
        return 0;
    }
}

//IDEM POUR SYNONYME DANS RECULER, TOURNER, et toutes autres actions primitives 

char *get_prefixes(instruction) {
    while (pas_parcouru_entièrement()){  
        mot=defiler(instruction);     //on utilisera une file pour utiliser vérif si elle est vide facilement
        if (synonyme_dans_avancer(mot)) enfiler(file_ordre_final,"A"); 
        if (synonyme_dans_reculer(mot)) enfiler(file_ordre_final,"R");
        if (synonyme_dans_tourner(mot)) enfiler(file_ordre_final,"T");
    }

}

// A VOIR SI ON METS AUSSI LA QUANTIFICATION (EX 5 MÈTRES POUR AVANCER) 
// DANS LA FONCTION GET PREFIXES OU BIEN SI ON APPEND CETTE VALEUR JUSTE APRÈS AVEC UNE AUTRE FONCTION
