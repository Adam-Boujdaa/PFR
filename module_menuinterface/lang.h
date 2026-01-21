#ifndef LANG_H
#define LANG_H

/* charge le fichier de langue */
void lang_load(const char *file);

/* retourne le texte associé à une clé */
const char *lang(const char *key);

#endif

