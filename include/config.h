/* ============================================================
   Fichier  : config.h
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : CHENTIR Hakim
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce fichier définit l'interface du système de traduction.
   Il permet de charger un fichier de langue et d'obtenir
   les traductions des clés.
   ------------------------------------------------------------
                        RÔLE DU MODULE
   ------------------------------------------------------------
   Déclarer les fonctions de chargement et d'accès aux
   traductions utilisées par l'ensemble de l'application.
   ============================================================ */


#ifndef CONFIG_H
#define CONFIG_H

/* charge le fichier de configuration */
void charger_config(const char *file);

/* retourne le texte associé à une clé */
const char *config(const char *key);

#endif

