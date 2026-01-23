#include <stdio.h>
#include <stdlib.h>
#include "module_image.h"
#include "objet.h"



int main(int argc, char *argv[])
{

	

	FILE *in = fopen(argv[1], "r");
	if (!in)
	{
		perror("fopen input");
		return 1;
	}

	FILE *out = fopen(argv[2], "w");
	if (!out)
	{
		perror("fopen output");
		fclose(in);
		return 1;
	}

	Objet objets[5];

    Image img = image_lire(in);

	Image mask = image_masque_objets(img, 70);

	//image_enregistrer(stdout, mask);


	int nb_objets = image_trouver_objets(mask, objets, 300);

	printf("Nombre d'objets trouves: %d\n", nb_objets);

	for (int i = 0; i < nb_objets; i++) {
		Point centre = objet_trouver_centre(objets[i]);
		Pixel coul = image_trouver_couleur(img, mask, objets[i]);
		image_dessiner_boite_englobante(img, objets[i], coul);
		printf("Objet %d : Centre X=%d, Centre Y=%d, aire=%d Couleur=%s \n",
				i + 1,
				centre.x,
				centre.y,
				objets[i].aire,
				image_pixel_to_nom(coul));
}

	image_enregistrer(out, img);

	free(img);
	free(mask);
	fclose(in);
	fclose(out);

	return 0;
}
