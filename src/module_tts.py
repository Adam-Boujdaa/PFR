#Auteur : Adam BOUJDAA
#Date : Janvier 2026
# Description : Module TTS - Reconnaissance vocale en Python

import speech_recognition as sr
import os
import sys  # <--- INDISPENSABLE pour que le silencieux fonctionne !
from contextlib import contextmanager

# --- SILENCIEUX NUCLÉAIRE (Anti-erreurs ALSA/JACK) ---
@contextmanager
def ignore_stderr():
    try:
        devnull = os.open(os.devnull, os.O_WRONLY)
        old_stderr = os.dup(2)
        sys.stderr.flush()
        os.dup2(devnull, 2)
        try:
            yield
        finally:
            os.dup2(old_stderr, 2)
            os.close(old_stderr)
            os.close(devnull)
    except Exception:
        yield 

# --- MAIN ---

LANGUAGE = "fr-FR"
r = sr.Recognizer()

# On englobe tout pour ne pas avoir de textes rouges
with ignore_stderr():
    try:
        micro = sr.Microphone()
        with micro as source:
            print("Speech Recognition active. Parlez maintenant. \nRemarque : dites 'exit' pour quitter, ou 'clavier' pour passer au mode clavier.")

            r.adjust_for_ambient_noise(source, duration=0.5)
            
            try: #sert pr les exceptions après (comme en java)
                audio_data = r.listen(source, timeout=5)
                print("Listening finished, interpreting your command...")
                
                transcription = r.recognize_google(audio_data, language=LANGUAGE)
                print(f"You've said : {transcription}")

                # Chemins alignés avec ton main.c
                path_cmd = "output/pre_traitement/commande.txt"
                path_hist = "output/pre_traitement/historique.txt"

                os.makedirs(os.path.dirname(path_cmd), exist_ok=True)

                # Ecriture
                with open(path_cmd, "w") as f: #w = overwrite
                    f.write(transcription)
                
                with open(path_hist, "a") as f: #a = append
                    f.write(transcription + "\n")
                    
            #gestion des erreurs
            except sr.WaitTimeoutError:
                print("Aucun son détecté.")
            except sr.UnknownValueError:
                print("Pas compris. Parlez plus fort.")
            except sr.RequestError:
                print("Erreur de connexion internet.")

    except Exception as e:
        print(f"Erreur critique Python : {e}")