
/* AUTEUR : ADAM BOUJDAA                            */
/* DATE CREATION : 28/12/2025                      */
/*------------------------------------------------*/
/* FICHIER DE TESTS UNITAIRES DE DICTIONNAIRE.C  */

#include <stdio.h>
#include "dictionnaire.h"

int main(){
    Dico act[1];
    charger_dico(&act[0], "verbes/avancer.txt", 'A', 0);

    printf("Test chargement dico : %d synonymes\n", act[0].nb_syn);

    char test1[]="Avancer 5";
    traiter_cmd(test1, act,1,NULL,0,NULL,0);

    char test2[]="AVANCE 10";
    traiter_cmd(test2, act,1,NULL,0,NULL,0);

    liberer_dico(&act[0]);
    return 0;
}
