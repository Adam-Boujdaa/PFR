#include <stdio.h> 
#include <string.h> 
#include "dictionnaire.h" 

char MODE='T';   //mode texte par défaut pour l'instant, j'ai pas réussi à bien intégrer le mode vocale


int main() { 
    // 1. Chargement Actions (valeur default = 1 mètre) 
    Dico act[3]; 
    charger_dico(&act[0], "verbes/avancer.txt", 'A', 1); 
    charger_dico(&act[1], "verbes/reculer.txt", 'R', 1); 
    charger_dico(&act[2], "verbes/tourner.txt", 'T', 0); 

    // 2. Chargement Directions (valeur par défaut = 90) 
    Dico dir[2]; 
    charger_dico(&dir[0], "verbes/gauche.txt", 'G', 90); 
    charger_dico(&dir[1], "verbes/droite.txt", 'D', -90); 

    // 3. Chargement Nombres (valeur = chiffre)
    Dico nb[20]; 
    charger_dico(&nb[0], "nombres/un.txt", 'N', 1); 
    charger_dico(&nb[1], "nombres/deux.txt", 'N', 2); 
    charger_dico(&nb[2], "nombres/trois.txt", 'N', 3); 
    charger_dico(&nb[3], "nombres/quatre.txt", 'N', 4); 
    charger_dico(&nb[4], "nombres/cinq.txt", 'N', 5); 
    charger_dico(&nb[5], "nombres/six.txt", 'N', 6); 
    charger_dico(&nb[6], "nombres/sept.txt", 'N', 7); 
    charger_dico(&nb[7], "nombres/huit.txt", 'N', 8); 
    charger_dico(&nb[8], "nombres/neuf.txt", 'N', 9); 
    charger_dico(&nb[9], "nombres/dix.txt", 'N', 10); 
    charger_dico(&nb[10], "nombres/onze.txt", 'N', 11); 
    charger_dico(&nb[11], "nombres/douze.txt", 'N', 12); 
    charger_dico(&nb[12], "nombres/treize.txt", 'N', 13); 
    charger_dico(&nb[13], "nombres/quatorze.txt", 'N', 14); 
    charger_dico(&nb[14], "nombres/quinze.txt", 'N', 15); 
    charger_dico(&nb[15], "nombres/seize.txt", 'N', 16); 
    charger_dico(&nb[16], "nombres/dix_sept.txt", 'N', 17); 
    charger_dico(&nb[17], "nombres/dix_huit.txt", 'N', 18); 
    charger_dico(&nb[18], "nombres/dix_neuf.txt", 'N', 19); 
    charger_dico(&nb[19], "nombres/vingt.txt", 'N', 20);

    char buf[MAX_LIGNE]; 

    // boucle lecture 
    while(1) { 
        if(MODE='T'){  //mode textuel
            printf("\nCMD > "); 
            if (fgets(buf, MAX_LIGNE, stdin) == NULL) break; 
            buf[strcspn(buf, "\n")] = 0; // clean 
            if (strcmp(buf, "exit") == 0) break; 
            // appel fct avec les 3 types de dicos 
            traiter_cmd(buf, act, 3, dir, 2, nb, 10); 
        }
        else if (MODE='V') {  //mode vocale
            system("python3 module_tts.py"); //on éxécute le module tts, après il faudra ajouter un feedback audio + un delay pr l'intégration
        }
    } 
    return 0; 
}