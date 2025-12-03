#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "module_image.h"


int IMAGE[NB_MAX][NB_MAX];

void test_prog(void)
{
	printf("TEST DU MODULE : MODULE_IMAGE\n");
}

int niv_gris_valide(int niv_gris)
{
	if (niv_gris < 1 || niv_gris > 255)
		return -1;
	else if (niv_gris != (1 << (int)log2(niv_gris)))
		return 0;
	else
		return 1;
}

int afficher_image_codee(int niv_gris)
{
	int hauteur, largeur, seuil, pixel;

	int valide = niv_gris_valide(niv_gris);
	if (valide == 1)
	{
		scanf("%d %d", &hauteur, &largeur);
		seuil = 256 / niv_gris;
		printf("%d %d\n", hauteur, largeur);

		for (int i = 0; i < hauteur; i++)
		{
			for (int j = 0; j < largeur; j++)
			{
				scanf("%d", &pixel);
				printf("%d ", pixel / seuil);
			}
			printf("\n");
		}
	}
	return valide;
}

int afficher_image_codee_bis(int ligne, int colonne, int niv_gris)
{
	int seuil;
	int valide = niv_gris_valide(niv_gris);
	if (valide == 1)
	{
		seuil = 256 / niv_gris;
		printf("%d %d\n", ligne, colonne);

		for (int i = 0; i < ligne; i++)
		{
			for (int j = 0; j < colonne; j++)
			{
				printf("%d ", IMAGE[i][j] / seuil);
			}
			printf("\n");
		}
	}
	return valide;
}

int lire_image(int ligne, int colonne)
{
	for (int i = 0; i < ligne; i++)
	{
		for (int j = 0; j < colonne; j++)
		{
			if (scanf("%d", &IMAGE[i][j]) != 1)
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
			printf("%d ", IMAGE[i][j]);
		}
		printf("\n");
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
				printf("%d ", IMAGE[i][j]);
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
				printf("%d ", IMAGE[i][j]);
			}
			printf("\n");
		}
	}
	return 1;
}

int histogramme(int ligne, int colonne)
{
	int freq[256] = {0};
	int total = ligne * colonne;

	for (int i = 0; i < ligne; i++)
	{
		for (int j = 0; j < colonne; j++)
		{
			freq[IMAGE[i][j]]++;
		}
	}

	printf("Histogramme (niveau de gris - fréquence - pourcentage):\n");
	for (int k = 0; k < 256; k++)
	{
		if (freq[k] > 0)
		{
			double pourcentage = (freq[k] * 100.0) / total;
			printf("Niveau %3d : %5d pixels (%6.2f%%)\n", k, freq[k], pourcentage);
		}
	}
	return 1;
}
