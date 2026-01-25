#ifndef CONFIG_H
#define CONFIG_H

/* charge le fichier de configuration */
void charger_config(const char *file);

/* retourne le texte associé à une clé */
const char *config(const char *key);

#endif

