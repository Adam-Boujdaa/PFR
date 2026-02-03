/* ============================================================
   Fichier  : menu.h
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : CHENTIR Hakim
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce fichier définit l'interface du système de menus.
   Il déclare toutes les fonctions permettant de naviguer
   dans l'application et de gérer les interactions.
   ------------------------------------------------------------
                        RÔLE DU MODULE
   ------------------------------------------------------------
   Déclarer les fonctions de navigation, d'authentification
   et d'interaction utilisateur pour l'ensemble des menus.
   ============================================================ */

#ifndef MENU_H
#define MENU_H

/* fonctions des menus */
int read_int();
int verifier_mdp();
void choisir_langue();
void menu_principal();
void menu_utilisateur();
void menu_admin();
void menu_image();
void menu_commande_vocale();
void menu_commande_textulle();

#endif

