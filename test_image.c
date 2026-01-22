#include <stdio.h>
#include <stdlib.h>
#include "module_image.h"
#include "objet.h"



int main(int argc, char *argv[])
{
	int ligne, colonne, canaux;

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

	//printf("Saisir dimensions (lignes colonnes canaux) : ");
	fscanf(in, "%d %d %d", &ligne, &colonne, &canaux);

    Image img = image_lire(in, ligne, colonne, canaux);

	Image mask = image_masque_objets(img, 70);


	int nb_objets = image_trouver_objets(mask, objets, 300);

	printf("Nombre d'objets trouves: %d\n", nb_objets);

	for (int i = 0; i < nb_objets; i++)
	{
		printf("Objet %d : min_x=%d, min_y=%d, max_x=%d, max_y=%d, aire=%d\n",
			   i + 1,
			   objets[i].min_x,
			   objets[i].min_y,
			   objets[i].max_x,
			   objets[i].max_y,
			   objets[i].aire);
		Pixel coul = image_trouver_couleur(img, mask, objets[i]);
		printf("  Couleur moyenne : R=%d, G=%d, B=%d\n", coul.r, coul.g, coul.b);
		printf("  Couleur nommee : %s\n", image_pixel_to_nom(coul));

		for (int y = 0; y < ligne; y++)
		{
			for (int x = 0; x < colonne; x++)
			{
				if (((x == objets[i].min_x || x == objets[i].max_x) && y >= objets[i].min_y && y <= objets[i].max_y)
					|| ((y == objets[i].min_y || y == objets[i].max_y) && x >= objets[i].min_x && x <= objets[i].max_x))
				{
					img->data[y][x] = coul;
				}

			}
		}
	}	

	image_enregistrer(out, img);

	free(img);
	free(mask);
	fclose(in);
	fclose(out);

	return 0;
}
