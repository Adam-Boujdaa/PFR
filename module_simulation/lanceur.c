#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    
    // Vérification des arguments
    if (argc != 4) {
        printf("Usage: %s <config.txt> <salle.txt> <commandes.txt>\n", argv[0]);
        printf("\nExemple:\n");
        printf("  %s ../config/simulation.txt ../config/salle_test.txt ../scripts/commandes_tests.txt\n", argv[0]);
        return 1;
    }
    
    // Récupération des arguments
    char* fichier_config = argv[1];
    char* fichier_salle = argv[2];
    char* fichier_commandes = argv[3];
    
    printf("Configuration : %s\n", fichier_config);
    printf("Salle         : %s\n", fichier_salle);
    printf("Commandes     : %s\n", fichier_commandes);
    printf("\nLancement de la simulation...\n\n");
    
    // Construction de la commande
    char commande[1024];
    snprintf(commande, sizeof(commande), 
             "python3 simulation.py %s %s %s",
             fichier_config, fichier_salle, fichier_commandes);
    
    // Exécution
    int resultat = system(commande);
    
    if (resultat == 0) {
        printf("\n✓ Simulation terminée avec succès\n");
        return 0;
    } else {
        printf("\n✗ Erreur lors de l'exécution\n");
        return 1;
    }
}
