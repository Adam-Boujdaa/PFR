#on active le venv pour pouvoir lancer le script python avec les extensions nécéssaires
python3 -m venv venv_pour_tts #on crée le venv car git ne peut pas le laisser etre push 
source venv_pour_tts/bin/activate
#on installe les dépendances ou cas où elles ne sont pas installées
pip install SpeechRecognition PyAudio gTTS

#compliation
make clean
make -f Makefile Parseur.out
