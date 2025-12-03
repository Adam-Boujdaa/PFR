#include <stdio.h>
#include <stdlib.h>
#include "module_image.h"

int main()
{
	int niv_gris = 64;
	int ligne, colonne, canaux;

	printf("Mode mémorisation. Saisir le nombre de lignes, de colonnes et de canaux de l'image : \n");
	scanf("%d %d %d", &ligne, &colonne, &canaux);

	if (ligne > NB_MAX || colonne > NB_MAX || ligne <= 0 || colonne <= 0)
	{
		fprintf(stderr, "Erreur : dimensions de l'image dépassent les limites autorisées (%d x %d) ou sont nulles\n", NB_MAX, NB_MAX);
		return 1;
	}

	int ok = lire_image(ligne, colonne, canaux);
	if (!ok)
	{
		fprintf(stderr, "Erreur lors de la lecture des pixels (entrée inattendue)\n");
		return 1;
	}

	int res = quantisation(ligne, colonne, 64);
	if (res == -1)
	{
		fprintf(stderr, "Erreur : niv_gris hors intervalle [1..255]\n");
	}
	else if (res == 0)
	{
		fprintf(stderr, "Erreur : niv_gris non puissance de 2\n");
	}

	return !res;
}
