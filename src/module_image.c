/* ============================================================
   Fichier  : module_image.c
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : SAHLI Aziz
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce module permet la manipulation des images : on peut binariser,
   touner, quantifier, et trouver les objets.
   ============================================================ */


#include "module_image.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "objet.h"
#include "pile.h"

static int max3(int a, int b, int c) {
    int max = a;

    if (b > max)
        max = b;
    if (c > max)
        max = c;

    return max;
}

static int min3(int a, int b, int c) {
    int min = a;

    if (b < min)
        min = b;
    if (c < min)
        min = c;

    return min;
}

// Initialiser la structure image
Image image_init(int hauteur, int largeur, int canaux) {
    Image img = (Image)malloc(sizeof(s_Image));
    img->hauteur = hauteur;
    img->largeur = largeur;
    img->canaux = canaux;
    for (int y = 0; y < hauteur; y++) {
        for (int x = 0; x < largeur; x++) {
            img->data[y][x] = (Pixel){0, 0, 0};
        }
    }
    return img;
}

// Savoir si le niveau de gris est valide, uttile pour la quantification
int image_niv_gris_valide(int niv_gris) {
    if (niv_gris < 1 || niv_gris > 255)
        return -1;
    else if (niv_gris != (1 << (int)log2(niv_gris)))
        return 0;
    else
        return 1;
}

// Enregistrer l'image dans un fichier .txt
int image_enregistrer(FILE* f, Image img) {
    fprintf(f, "%d %d %d\n", img->hauteur, img->largeur, img->canaux);

    for (int c = 0; c < img->canaux; c++) {
        for (int i = 0; i < img->hauteur; i++) {
            for (int j = 0; j < img->largeur; j++) {
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

// Quantifier l'image : réduire l'espace de couleurs
Image image_quantification(Image img, int q) {
    Image quant = image_init(img->hauteur, img->largeur, img->canaux);
    int seuil, valide = image_niv_gris_valide(q);

    if (valide == 1) {
        seuil = 256 / q;

        for (int i = 0; i < img->hauteur; i++) {
            for (int j = 0; j < img->largeur; j++) {
                quant->data[i][j].r = (img->data[i][j].r / seuil) * seuil + seuil / 2;
                quant->data[i][j].g = (img->data[i][j].g / seuil) * seuil + seuil / 2;
                quant->data[i][j].b = (img->data[i][j].b / seuil) * seuil + seuil / 2;
            }
        }
    }
    return quant;
}

// Lecture de l'image à partir d'un fichier .txt
Image image_lire(FILE* f) {
    int ligne, colonne, canaux;

    fscanf(f, "%d %d %d", &ligne, &colonne, &canaux);

    Image img = image_init(ligne, colonne, canaux);

    for (int c = 0; c < canaux; c++) {
        for (int i = 0; i < ligne; i++) {
            for (int j = 0; j < colonne; j++) {
                if (c == 0) {
                    fscanf(f, "%hhu ", &img->data[i][j].r);
                } else if (c == 1) {
                    fscanf(f, "%hhu ", &img->data[i][j].g);
                } else {
                    fscanf(f, "%hhu ", &img->data[i][j].b);
                }
            }
        }
    }
    return img;
}

// Retourner l'image horizontalement
Image image_miroir(Image img) {
    Image img_miroir = image_init(img->hauteur, img->largeur, img->canaux);

    for (int y = 0; y < img->hauteur; y++) {
        for (int x = 0; x < img->largeur; x++) {
            img_miroir->data[y][img->largeur - 1 - x] = img->data[y][x];
        }
    }
    return img_miroir;
}

// Binariser l'image : remettre en noir et blanc
Image image_binarisation(Image img) {
    Image img_bin = image_init(img->hauteur, img->largeur, img->canaux);
    int bin;

    for (int i = 0; i < img->hauteur; i++) {
        for (int j = 0; j < img->largeur; j++) {
            if (img->canaux == 3) {
                bin = ((img->data[i][j].r + img->data[i][j].g + img->data[i][j].b) / 3 >= 128) ? 255 : 0;
            } else {
                bin = (img->data[i][j].r >= 128) ? 255 : 0;
            }

            img_bin->data[i][j] = (Pixel){bin, bin, bin};
        }
    }

    return img_bin;
}

// Touner l'image 90 degrés dans le sens horaire
Image image_tourner(Image img) {
    Image img_rot = image_init(img->largeur, img->hauteur, img->canaux);

    for (int y = 0; y < img->hauteur; y++) {
        for (int x = 0; x < img->largeur; x++) {
            img_rot->data[x][img->hauteur - 1 - y] = img->data[y][x];
        }
    }
    return img_rot;
}

// Faire la binarisation de l'image de manière à prendre en compte le fond non uniforme
// On considère les pixels qui ont un pic de +seuil dans l'un des canaux comme un pixel de l'objet
Image image_masque_objets(Image img, int seuil) {
    Image mask = image_init(img->hauteur, img->largeur, 1);
    int mn, mx;

    for (int y = 0; y < img->hauteur; y++) {
        for (int x = 0; x < img->largeur; x++) {
            mx = max3(img->data[y][x].r, img->data[y][x].g, img->data[y][x].b);
            mn = min3(img->data[y][x].r, img->data[y][x].g, img->data[y][x].b);

            if (mx - mn > seuil)
                mask->data[y][x] = (Pixel){255, 255, 255};
        }
    }
    return mask;
}

// Trouver les objets et stocker les informations dans le struct Objet
// fait un parcours des pixels blancs (objets) et regroupe les pixels faisant partie d'un même objet
int image_trouver_objets(Image img, Objet* tab_objets, int aire_min) {
    Image labels = image_init(img->hauteur, img->largeur, 1);
    int object_courant = 1;
    int n_objets = 0;

    // les directions des pixels voisins (connectivité 8)
    int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

    PILE p = init_PILE();

    for (int y = 0; y < img->hauteur; y++) {
        for (int x = 0; x < img->largeur; x++) {
            if (img->data[y][x].r == 255 && labels->data[y][x].r == 0) {
                p = emPILE(p, (Point){x, y});
                labels->data[y][x].r = object_courant;

                Objet obj;
                obj.min_x = obj.max_x = x;
                obj.min_y = obj.max_y = y;
                obj.aire = 0;

                while (!PILE_estVide(p)) {
                    Point pt;
                    p = dePILE(p, &pt);
                    int cx = pt.x;
                    int cy = pt.y;

                    obj.aire++;

                    if (cx < obj.min_x) obj.min_x = cx;
                    if (cx > obj.max_x) obj.max_x = cx;
                    if (cy < obj.min_y) obj.min_y = cy;
                    if (cy > obj.max_y) obj.max_y = cy;

                    for (int k = 0; k < 8; k++) {
                        int nx = cx + dx[k];
                        int ny = cy + dy[k];

                        if (nx >= 0 && nx < img->largeur && ny >= 0 && ny < img->hauteur) {
                            if (img->data[ny][nx].r == 255 && labels->data[ny][nx].r == 0) {
                                labels->data[ny][nx].r = object_courant;
                                p = emPILE(p, (Point){nx, ny});
                            }
                        }
                    }
                }

                if (obj.aire >= aire_min) {
                    obj.rayon = (obj.max_x - obj.min_x + 1) / 2;
                    obj.centre = (Point){(obj.min_x + obj.max_x) / 2, (obj.min_y + obj.max_y) / 2};
                    tab_objets[n_objets] = obj;
                    n_objets++;
                    object_courant++;
                }
            }
        }
    }

    return n_objets;
}

// Calculer la couleur moyenne d'un objet donné
Pixel image_trouver_couleur(Image img, Image mask, Objet obj) {
    img = image_quantification(img, 16);
    int somme_r = 0, somme_g = 0, somme_b = 0;
    int c = 0;

    for (int y = obj.min_y; y <= obj.max_y; y++) {
        for (int x = obj.min_x; x <= obj.max_x; x++) {
            if (mask->data[y][x].r == 255) {
                somme_r += img->data[y][x].r;
                somme_g += img->data[y][x].g;
                somme_b += img->data[y][x].b;
                c++;
            }
        }
    }

    Pixel coul_moyenne;
    coul_moyenne.r = somme_r / c;
    coul_moyenne.g = somme_g / c;
    coul_moyenne.b = somme_b / c;

    return coul_moyenne;
}

// Dessiner la boite englobante sur l'objet donné en paramètre
void image_dessiner_boite_englobante(Image img, Objet obj, Pixel couleur_bordure) {
    for (int y = 0; y < img->hauteur; y++) {
        for (int x = 0; x < img->largeur; x++) {
            if (((x == obj.min_x || x == obj.max_x) && y >= obj.min_y && y <= obj.max_y) ||
                ((y == obj.min_y || y == obj.max_y) && x >= obj.min_x && x <= obj.max_x)) {
                img->data[y][x] = couleur_bordure;
            }
        }
    }
}

// Retourne une nouvelle image contenat uniquement l'objet donné en paramètre
Image image_segmenter_objet(Image img, Objet obj) {
    int hauteur, largeur;

    hauteur = obj.max_y - obj.min_y + 1;
    largeur = obj.max_x - obj.min_x + 1;

    Image seg = image_init(hauteur, largeur, img->canaux);

    for (int y = obj.min_y; y <= obj.max_y; y++) {
        for (int x = obj.min_x; x <= obj.max_x; x++) {
            seg->data[y][x] = img->data[y][x];
        }
    }
    return seg;
}

// Convertit la valeur RVB de l'objet à une couleur nommée
const char* image_pixel_to_nom(Pixel coul) {
    typedef struct {
        const char* nom;
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } CouleurNom;

    const CouleurNom COULEURS[] = {
        // Les caractères bizarres sont des séquences qui permettent de
        // colorier le texte dans le terminal
        {"\x1B[31mRouge\x1B[0m", 255, 0, 0},
        {"\x1B[32mVert\x1B[0m", 0, 255, 0},
        {"\x1B[34mBleu\x1B[0m", 0, 0, 255},
        {"\x1B[33mJaune\x1B[0m", 255, 255, 0},
        {"\x1B[38;5;208mOrange\x1B[0m", 255, 65, 0},
        {"Blanc", 255, 255, 255}};

    int n_couleurs = sizeof(COULEURS) / sizeof(CouleurNom);

    const char* nom_couleur = "Inconnu";
    int dist_min = 26000;

    for (int i = 0; i < n_couleurs; i++) {
        CouleurNom cour = COULEURS[i];

        int d_r = coul.r - cour.r;
        int d_g = coul.g - cour.g;
        int d_b = coul.b - cour.b;

        int dist = (d_r * d_r) + (d_g * d_g) + (d_b * d_b);

        if (dist < dist_min) {
            dist_min = dist;
            nom_couleur = cour.nom;
        }
    }

    return nom_couleur;
}


// Afficher les informations des objets dans le terminal
void objet_afficher(Image img, Image mask, Objet* objets, int nb_objets) {
    for (int i = 0; i < nb_objets; i++) {
        Pixel coul = image_trouver_couleur(img, mask, objets[i]);
        char* est_balle = objet_est_balle(objets[i]) ? "Oui" : "Non";
        image_dessiner_boite_englobante(img, objets[i], coul);
        printf("\033[1;4mObjet %d :\033[0m\n\tCentre : X=%d Y=%d, rayon=%d, aire=%d, Couleur=%s, Balle?=%s\n",
               i + 1,
               objets[i].centre.x,
               objets[i].centre.y,
               objets[i].rayon,
               objets[i].aire,
               image_pixel_to_nom(coul),
               est_balle);
    }
}

// Reconnaaitre si l'objet est une balle ou non
int objet_est_balle(Objet obj) {
    int aire_boite_eng = (obj.max_x - obj.min_x + 1) * (obj.max_y - obj.min_y + 1);
    float rapport = (float)obj.aire / (float)aire_boite_eng;
    if (fabs(rapport - 0.785) < 0.02) {
        return 1;
    } else {
        return 0;
    }
}