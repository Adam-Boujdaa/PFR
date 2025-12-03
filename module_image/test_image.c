#include <stdio.h>
#include "module_image.h"

int main()
{
	int ligne, colonne;

	printf("Saisir dimensions (lignes colonnes) : ");
	scanf("%d %d", &ligne, &colonne);

    lire_image(ligne, colonne);

	/* Test 1: image_miroir */
	printf("\n========== Test 1: image_miroir ==========\n");
	printf("Image miroir (colonnes inversées):\n");
	image_miroir(ligne, colonne);

	/* Test 2: rotation_image 90 degres */
	printf("\n========== Test 2: rotation_image (90 degres) ==========\n");
	printf("Image rotée de 90 degrés:\n");
	rotation_image(ligne, colonne, 90);

	/* Test 3: rotation_image 180 degres */
	printf("\n========== Test 3: rotation_image (180 degres) ==========\n");
	printf("Image rotée de 180 degrés:\n");
	rotation_image(ligne, colonne, 180);

	/* Test 4: histogramme */
	printf("\n========== Test 4: histogramme ==========\n");
	histogramme(ligne, colonne);

	return 0;
}
