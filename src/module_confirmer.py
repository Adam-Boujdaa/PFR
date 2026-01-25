# Auteur : Adam BOUJDAA
# Description : Traduction inverse (txt -> vocal) pour confirmation pour les commandes longues

import os
import sys
from gtts import gTTS

# Dictionnaires de traduction inverse
ACTIONS = {'A': "Avance", 
           'R': "Recule", 
           'T': "Tourne", 
           'S': "Stop"}
DIRECTIONS = {'D': "à droite", 
              'G': "à gauche", 
              '-': "", 
              '0': ""}
UNITES = {'m': "mètres", 
           'c': "centimètres", 
           'd': "degrés", 
           'f': "pieds", 
           '-': ""}

def lire_et_traduire(chemin_fichier):
    phrase_complete = "Confirmez cette commande : "
    
    try: #au cas où le fichier s'ouvre pas bien / exitse pas
        with open(chemin_fichier, "r") as f: # r= read mode
            liste_lignes = f.readlines() #lit toutes les lignes du fichier et les stocke dans la liste 
            
            for ligne in liste_lignes:
                parties = ligne.strip().split(',') #on enlève les espaces et on split par virgule
                act_code, dir_code, val, unit_code = parties[0], parties[1], parties[2], parties[3] #on prends les 4 parties de la commande élémentaire et on stocke
                
                # on récupère les traductions correspondantes aux keys dans les dicos
                action = ACTIONS.get(act_code, "Action inconnue")
                direction = DIRECTIONS.get(dir_code, "")
                unite = UNITES.get(unit_code, "")
                
                # construction de la phrase (ex: "Avance de 1.00 mètres")
                segment = f"{action} {direction} de {val} {unite}. "
                
                # nettoyage (enlever les "de 0" ou doubles espaces)
                segment = segment.replace("  ", " ").replace("de 0 ", "").replace(" .", ".")
                phrase_complete += segment + " puis " #on construit la phrase complète avec puis entre chaque commande
                
    except FileNotFoundError:
        print("Erreur : Fichier commande introuvable.")
        return


    phrase_complete = phrase_complete.rsplit(" puis ", 1)[0] #coupe le dernier "puis"
    
    print("\n" + "="*40)
    print("CONFIRMATION REQUISE :")
    print(phrase_complete)
    print("="*40)
    
    # génération audio
    try:
        tts = gTTS(text=phrase_complete, lang='fr')
        tts.save("output/confirmation.mp3")
        
        # lecture du fichier audio
        os.system("mpg123 -q output/confirmation.mp3 > /dev/null 2>&1") # on redirige vers /dev/null pour ne pas polluer le terminal
    except Exception as e:
        print(f"Erreur Audio (TTS) : {e}")


#lire le fichier de commande post-traitement et traduire pr demander la confirmation
lire_et_traduire("output/post_traitement/commande.txt")