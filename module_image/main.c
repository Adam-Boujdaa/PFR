#include <stdio.h>
#include <stdlib.h>
#include "module_image.h"


int main(void)
{
	int choix = 0;
	int niv_gris;
	int ligne, colonne;

	/* séquence de test du module */
	printf("DEBUT DE TEST MODULE\n");
	test_prog();

	/* Choix de la version à tester */
	printf("Choisir le mode de test :\n");
	printf("  1 - affichage au fil de l'eau\n");
	printf("  2 - affichage avec mémorisation\n");
	printf("  3 - affichage image miroir\n");
	printf("  4 - rotation image\n");
	printf("  5 - histogramme\n");
	printf("Entrez 1 à 5 : ");
	if (scanf("%d", &choix) != 1)
	{
		printf("Entrée invalide\n");
		return 1;
	}

	if (choix == 1)
	{
		printf("Mode \"au fil de l'eau\" choisi. Saisir le nombre de niveaux de gris (puissance de 2 entre 1 et 255) : ");
		scanf("%d", &niv_gris);

		int res = afficher_image_codee(niv_gris);
		if (res == -1)
			fprintf(stderr, "Erreur : niv_gris hors intervalle [1..255]\n");
		else if (res == 0)
			fprintf(stderr, "Erreur : niv_gris non puissance de 2\n");
		return (res == 1) ? 0 : 1;
	}
	else if (choix == 2)
	{
		printf("Mode mémorisation choisi. Saisir le nombre de lignes et de colonnes de l'image : \n");
		scanf("%d %d", &ligne, &colonne);

		if (ligne > NB_MAX || colonne > NB_MAX || ligne <= 0 || colonne <= 0)
		{
			fprintf(stderr, "Erreur : dimensions de l'image dépassent les limites autorisées (%d x %d) ou sont nulles\n", NB_MAX, NB_MAX);
			return 1;
		}

		printf("Saisir le nombre de niveaux de gris (puissance de 2 entre 1 et 255) : ");
		scanf("%d", &niv_gris);

		int ok = lire_image(ligne, colonne);
		if (!ok)
		{
			fprintf(stderr, "Erreur lors de la lecture des pixels (entrée inattendue)\n");
			return 1;
		}

		int res = afficher_image_codee_bis(ligne, colonne, niv_gris);
		if (res == -1)
			fprintf(stderr, "Erreur : niv_gris hors intervalle [1..255]\n");
		else if (res == 0)
			fprintf(stderr,"Erreur : niv_gris non puissance de 2\n");

		return !res;
	}
	else
	{
		printf("Choix inconnu\n");
		return 1;
	}
}
