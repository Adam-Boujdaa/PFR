#Auteur : Adam BOUJDAA
#Date : Décembre 2024
# Description : Ce module permet de transcrire en français (ou anglais) les commandes vocales pour les renvoyer à la partie traitement de texte

import speech_recognition as sr
import pyaudio
from gtts import gTTS
import os



#PARTIE GEMINI POUR GÉRER LES ERREURS ALSA QUI SATURAIENT LE TERMINAL

from ctypes import *
from contextlib import contextmanager

# --- PARTIE SILENCIEUX ALSA (Supression des messages rouges) ---
ERROR_HANDLER_FUNC = CFUNCTYPE(None, c_char_p, c_int, c_char_p, c_int, c_char_p)
def py_error_handler(filename, line, function, err, fmt):
    pass
c_error_handler = ERROR_HANDLER_FUNC(py_error_handler)

@contextmanager
def no_alsa_error():
    try:
        asound = cdll.LoadLibrary('libasound.so')
        asound.snd_lib_error_set_handler(c_error_handler)
        yield
        asound.snd_lib_error_set_handler(None)
    except:
        yield
# ---------------------------------------------------------------


# Partie Adam :

LANGUAGE = "fr-FR"
r = sr.Recognizer()

# On utilise le gestionnaire d'erreurs d'avant ici
with no_alsa_error():
    micro = sr.Microphone()


with micro as source:
    print("Speech Recognition active. Parlez maintenant.")
    
    
    try: #sert pour les exceptions après
        audio_data = r.listen(source, timeout=5)
        print("Listening finished, Interpreting your command...")
        transcription = r.recognize_google(audio_data, language=LANGUAGE)
        print(f"You have said: {transcription}")

        commande = open("output/pre_traitement/commande.txt", "w") # w=overwrite
        historique = open("output/pre_traitement/historique.txt", "a") #a = append, pour garder les commandes d'avant
    
        if transcription.lower() != "exit":  #pour ne pas écrire exit ds les files de commande et historique
            commande.write(transcription + "\n")
            historique.write(transcription + "\n")

            commande.close()
            historique.close()

        if transcription.lower() == "exit":
            print("Exit command received. Exiting...")
            exit()

    #GESTION D'ERREURS DE SPEECH RECOGNITION :
    except sr.UnknownValueError:
        print("Speak louder and/or clearer svp.")
    except sr.RequestError:
        print("Connection error.")
    except sr.WaitTimeoutError:
        print("No sound detected.")