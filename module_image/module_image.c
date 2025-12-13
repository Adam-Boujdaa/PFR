#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "module_image.h"

int max3(int a, int b, int c)
{
	int max = a;

	if (b > max)
		max = b;
	if (c > max)
		max = c;

	return max;
}

int min3(int a, int b, int c)
{
	int min = a;

	if (b < min)
		min = b;
	if (c < min)
		min = c;

	return min;
}

Image image_init(int hauteur, int largeur, int canaux)
{
	Image img = (Image)malloc(sizeof(s_Image));
	img->hauteur = hauteur;
	img->largeur = largeur;
	img->canaux = canaux;
	for (int y = 0; y < hauteur; y++)
	{
		for (int x = 0; x < largeur; x++)
		{
			img->data[y][x] = (Pixel){0, 0, 0};
		}
	}
	return img;
}

int image_niv_gris_valide(int niv_gris)
{
	printf("niv_gris: %d\n", niv_gris);

	if (niv_gris < 1 || niv_gris > 255)
		return -1;
	else if (niv_gris != (1 << (int)log2(niv_gris)))
		return 0;
	else
		return 1;
}

int image_enregistrer(FILE *f, Image img)
{
	fprintf(f, "%d %d %d\n", img->hauteur, img->largeur, img->canaux);

	for (int c = 0; c < img->canaux; c++)
	{
		for (int i = 0; i < img->hauteur; i++)
		{
			for (int j = 0; j < img->largeur; j++)
			{
				if (c == 0)
					fprintf(f, "%d ", img->data[i][j].r);
				else if (c == 1)
					fprintf(f, "%d ", img->data[i][j].g);
				else
					fprintf(f, "%d ", img->data[i][j].b);
			}
			fprintf(f, "\n");
		}
	}
	return 1;
}

int quantification(Image img, int q)
{
	int seuil;
	int valide = image_niv_gris_valide(q);

	if (valide == 1)
	{
		seuil = 256 / q;

		for (int i = 0; i < img->hauteur; i++)
		{
			for (int j = 0; j < img->largeur; j++)
			{
				img->data[i][j].r = (img->data[i][j].r / seuil);
				img->data[i][j].g = (img->data[i][j].g / seuil);
				img->data[i][j].b = (img->data[i][j].b / seuil);
			}
		}
	}
	return valide;
}

Image image_lire(FILE *f, int ligne, int colonne, int canaux)
{
	Image img = image_init(ligne, colonne, canaux);

	for (int c = 0; c < canaux; c++)
	{
		for (int i = 0; i < ligne; i++)
		{
			for (int j = 0; j < colonne; j++)
			{
				if (c == 0)
				{
					fscanf(f, "%d ", &img->data[i][j].r);
				}
				else if (c == 1)
				{
					fscanf(f, "%d ", &img->data[i][j].g);
				}
				else
				{
					fscanf(f, "%d ", &img->data[i][j].b);
				}
			}
		}
	}
	return img;
}

Image image_miroir(Image img)
{
	Image img_miroir = image_init(img->hauteur, img->largeur, img->canaux);

	for (int y = 0; y < img->hauteur; y++)
	{
		for (int x = 0; x < img->largeur; x++)
		{
			img_miroir->data[y][img->largeur - 1 - x] = img->data[y][x];
		}
	}
	return img_miroir;
}

Image image_binarisation(Image img)
{
	Image img_bin = image_init(img->hauteur, img->largeur, img->canaux);
	int bin;

	for (int i = 0; i < img->hauteur; i++)
	{
		for (int j = 0; j < img->largeur; j++)
		{
			if (img->canaux == 3)
			{
				bin = ((img->data[i][j].r + img->data[i][j].g + img->data[i][j].b) / 3 >= 128) ? 255 : 0;
			}
			else
			{
				bin = (img->data[i][j].r >= 128) ? 255 : 0;
			}

			img_bin->data[i][j].r = bin;
			img_bin->data[i][j].g = bin;
			img_bin->data[i][j].b = bin;
		}
	}

	return img_bin;
}

Image image_masque_objets(Image img, int seuil)
{
	Image mask = image_init(img->hauteur, img->largeur, 1);
	int mn, mx;

	for (int y = 0; y < img->hauteur; y++)
	{
		for (int x = 0; x < img->largeur; x++)
		{
			mx = max3(img->data[y][x].r, img->data[y][x].g, img->data[y][x].b);
			mn = min3(img->data[y][x].r, img->data[y][x].g, img->data[y][x].b);

			if (mx - mn > seuil)
				mask->data[y][x] = (Pixel){255, 255, 255};
		}
	}
	return mask;
}