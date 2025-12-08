#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "module_image.h"

Pixel IMAGE[NB_MAX][NB_MAX];

int niv_gris_valide(int niv_gris)
{
	printf("niv_gris: %d\n", niv_gris);
	if (niv_gris < 1 || niv_gris > 255)
		return -1;
	else if (niv_gris != (1 << (int)log2(niv_gris)))
		return 0;
	else
		return 1;
}

int afficher_image(int hauteur, int largeur, int canaux)
{
	printf("%d %d %d\n", hauteur, largeur, canaux);
	for (int i = 0; i < hauteur; i++)
	{
		for (int j = 0; j < largeur; j++)
		{
			printf("%d %d %d ", IMAGE[i][j].r, IMAGE[i][j].g, IMAGE[i][j].b);
		}
		printf("\n");
	}
	return 1;
}

int quantification(int ligne, int colonne, int niv_gris)
{
	int seuil;
	int valide = niv_gris_valide(niv_gris);
	if (valide == 1)
	{
		seuil = 256 / niv_gris;

		for (int i = 0; i < ligne; i++)
		{
			for (int j = 0; j < colonne; j++)
			{
				IMAGE[i][j].r /= seuil;
				IMAGE[i][j].g /= seuil;
				IMAGE[i][j].b /= seuil;
			}
		}
	}
	return valide;
}

int lire_image(int ligne, int colonne, int canaux)
{
	for (int i = 0; i < ligne; i++)
	{
		for (int j = 0; j < colonne; j++)
		{
			if (canaux == 3)
			{
				scanf("%d %d %d", &IMAGE[i][j].r, &IMAGE[i][j].g, &IMAGE[i][j].b);
			}
			else if (canaux == 1)
			{
				int gris;
				scanf("%d", &gris);
				IMAGE[i][j].r = gris;
				IMAGE[i][j].g = gris;
				IMAGE[i][j].b = gris;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

int image_miroir(int ligne, int colonne)
{
	printf("%d %d\n", ligne, colonne);

	for (int i = 0; i < ligne; i++)
	{
		for (int j = colonne - 1; j >= 0; j--)
		{
			printf("%d %d %d ", IMAGE[i][j].r, IMAGE[i][j].g, IMAGE[i][j].b);
		}
		printf("\n");
	}
	return 1;
}

int binarisation(int ligne, int colonne)
{
	for (int i = 0; i < ligne; i++)
	{
		for (int j = 0; j < colonne; j++)
		{
			int bin = ((IMAGE[i][j].r + IMAGE[i][j].g + IMAGE[i][j].b) / 3 >= 128) ? 255 : 0;
			IMAGE[i][j].r = bin;
			IMAGE[i][j].g = bin;
			IMAGE[i][j].b = bin;
		}
	}
	return 1;
}

int rotation_image(int ligne, int colonne, int angle)
{
	if (angle != 90 && angle != 180)
	{
		fprintf(stderr, "Erreur : angle doit être 90 ou 180\n");
		return -1;
	}

	if (angle == 90)
	{
		printf("%d %d\n", colonne, ligne);
		for (int j = 0; j < colonne; j++)
		{
			for (int i = ligne - 1; i >= 0; i--)
			{
				printf("%d %d %d ", IMAGE[i][j].r, IMAGE[i][j].g, IMAGE[i][j].b);
			}
			printf("\n");
		}
	}
	else if (angle == 180)
	{
		printf("%d %d\n", ligne, colonne);
		for (int i = ligne - 1; i >= 0; i--)
		{
			for (int j = colonne - 1; j >= 0; j--)
			{
				printf("%d %d %d ", IMAGE[i][j].r, IMAGE[i][j].g, IMAGE[i][j].b);
			}
			printf("\n");
		}
	}
	return 1;
}

int histogramme(int ligne, int colonne)
{
	int freq_r[256] = {0};
	int freq_g[256] = {0};
	int freq_b[256] = {0};
	int total = ligne * colonne;

	for (int i = 0; i < ligne; i++)
	{
		for (int j = 0; j < colonne; j++)
		{
			freq_r[IMAGE[i][j].r]++;
			freq_g[IMAGE[i][j].g]++;
			freq_b[IMAGE[i][j].b]++;
		}
	}

	printf("Histogramme Rouge (niveau - fréquence - pourcentage):\n");
	for (int k = 0; k < 256; k++)
	{
		if (freq_r[k] > 0)
		{
			double pourcentage = (freq_r[k] * 100.0) / total;
			printf("Niveau %3d : %5d pixels (%6.2f%%)\n", k, freq_r[k], pourcentage);
		}
	}

	printf("\nHistogramme Vert (niveau - fréquence - pourcentage):\n");
	for (int k = 0; k < 256; k++)
	{
		if (freq_g[k] > 0)
		{
			double pourcentage = (freq_g[k] * 100.0) / total;
			printf("Niveau %3d : %5d pixels (%6.2f%%)\n", k, freq_g[k], pourcentage);
		}
	}

	printf("\nHistogramme Bleu (niveau - fréquence - pourcentage):\n");
	for (int k = 0; k < 256; k++)
	{
		if (freq_b[k] > 0)
		{
			double pourcentage = (freq_b[k] * 100.0) / total;
			printf("Niveau %3d : %5d pixels (%6.2f%%)\n", k, freq_b[k], pourcentage);
		}
	}
	return 1;
}
