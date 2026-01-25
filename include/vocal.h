#ifndef VOCAL_H
#define VOCAL_H

// enum pour les langues
typedef enum { FR, EN } Langue;

void charger_dictionnaire_fr();
void charger_dictionnaire_en();
void traitement_mode_textuel(Langue langue);
void traitement_mode_vocal();


#endif