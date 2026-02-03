/* ============================================================
   Fichier  : log.h
   Projet   : PFR1 – Projet Fil Rouge
   Auteur   : CHENTIR Hakim
   ------------------------------------------------------------
                        DESCRIPTION
   ------------------------------------------------------------
   Ce fichier définit l'interface du système de logs.
   Il permet d'enregistrer et de consulter l'historique
   des actions de l'application.
   ------------------------------------------------------------
                        RÔLE DU MODULE
   ------------------------------------------------------------
   Déclarer les fonctions d'écriture et de lecture des
   logs utilisées par l'ensemble de l'application.
   ============================================================ */

#ifndef LOG_H
#define LOG_H

/* écrit un message dans le fichier de log */
void log_msg(const char *msg);

/* affiche les logs */
void show_logs();

#endif

