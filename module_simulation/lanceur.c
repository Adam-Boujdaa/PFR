#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Lancement de la simulation robot\n\n");
    
    //script Python
    int resultat = system("python3 /python/simulation.py");
    
    if (resultat == 0) {
        printf("\nSimulation terminée \n");
        return 0;
    } else {
        printf("\nErreur lors de la simulation \n");
        return 1;
    }
}
