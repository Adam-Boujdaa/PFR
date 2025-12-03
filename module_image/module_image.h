#ifndef MODULE_IMAGE_H_INCLUS
#define MODULE_IMAGE_H_INCLUS

#define NB_MAX 16

extern int IMAGE[NB_MAX][NB_MAX];


void test_prog(void);
int niv_gris_valide(int niv_gris);
int afficher_image_codee(int);
int afficher_image_codee_bis(int ligne, int colonne, int niv_gris);
int lire_image(int ligne, int colonne);

int image_miroir(int ligne, int colonne);
int rotation_image(int ligne, int colonne, int angle);
int histogramme(int ligne, int colonne);

#endif
