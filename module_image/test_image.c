#include <stdio.h>
#include "module_image.h"
#include <stdlib.h>

int main()
{
	int ligne, colonne, canaux;

	FILE *in = fopen("image.txt", "r");

	FILE *out = fopen("mask.txt", "w");

	//printf("Saisir dimensions (lignes colonnes canaux) : ");
	fscanf(in, "%d %d %d", &ligne, &colonne, &canaux);

    Image img = image_lire(in, ligne, colonne, canaux);

	Image mask = image_masque_objets(img, 45);

	free(img);

	image_enregistrer(out, mask);
	
	fclose(in);
	fclose(out);

	return 0;
}
