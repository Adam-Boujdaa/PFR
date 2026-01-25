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

	objet_afficher(img, mask, objets, nb_objets);

	image_enregistrer(out, img);

	free(img);
	free(mask);
	fclose(in);
	fclose(out);

	return 0;
}
