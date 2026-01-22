#Auteur : Adam BOUJDAA
#Date : Décembre 2025
# Description : Ce module permet de transcrire en français (ou anglais) les commandes vocales pour les renvoyer à la partie traitement de texte

import speech_recognition as sr
import pyaudio
from gtts import gTTS
import os

LANGUAGE = "fr-FR"  #en-EN pour anglais
r = sr.Recognizer()
micro = sr.Microphone()

while True:
    with micro as source:
        print("Donnez votre commande vocale")
        try :  #sert pour les exceptions après
            audio_data = r.listen(source)
            print("Fini d'écouter, traitement en cours...")
            transcription = r.recognize_google(audio_data, language=LANGUAGE)
            print("Vous avez dit: ", transcription)

            commande = open("commande.txt", "w")  #dans la 2ème paire de guillemets, a=append, w=overwwrite
            historique = open("historique.txt", "a")

            # PLUS TARD APRÈS INTÉGRATION DU MODULE DE SIMULATION :
            # f = open("../module_simulation/commande.txt", "w") # plus tard aussi
            # f = open("../module_simulation/historique.txt", "a") #dans la 2ème paire de guillemets, a=append,, w=overwwrite

            if transcription.lower() != "exit":  #pour ne pas écrire exit ds les files de commande et historique
                commande.write(transcription + "\n")
                historique.write(transcription + "\n")

            commande.close()
            historique.close()

            if transcription.lower() == "exit":
                break

        #GESTION D'ERREURS DE SPEECH RECOGNITION :
        except sr.UnknownValueError:  # Si Google ne comprend pas (bruit, marmonnement), on ne fait rien
            print("Pas compris, parlez plus fort / clairement.")
        except sr.RequestError:  # Si internet coupe
            print("Erreur de connexion Google.")
