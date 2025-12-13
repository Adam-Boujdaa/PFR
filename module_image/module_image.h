#ifndef MODULE_IMAGE_H_INCLUS
#define MODULE_IMAGE_H_INCLUS

#define MAX 300

typedef struct
{
	int r;
	int g;
	int b;
} Pixel;

typedef struct {
	int hauteur, largeur, canaux;
	Pixel data[MAX][MAX];
} s_Image;

typedef s_Image* Image;

Image image_init(int hauteur, int largeur, int canaux);
int image_niv_gris_valide(int niv_gris);
int image_enregistrer(FILE *f, Image img);
int quantification(Image img, int q);
Image image_lire(FILE *f, int ligne, int colonne, int canaux);
Image image_miroir(Image img);
Image image_binarisation(Image img);
Image image_masque_objets(Image img, int seuil);


#endif
