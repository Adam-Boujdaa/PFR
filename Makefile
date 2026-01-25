CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Répertoires
SRC = src
INC = include
OBJ = obj
TEST = test

main.out: $(OBJ)/main.o $(OBJ)/menu.o $(OBJ)/config.o $(OBJ)/log.o $(OBJ)/module_image.o $(OBJ)/pile.o
	$(CC) $(CFLAGS) -o main.out $(OBJ)/main.o $(OBJ)/menu.o $(OBJ)/config.o $(OBJ)/log.o $(OBJ)/module_image.o $(OBJ)/pile.o -lm

lanceur.out : lanceur.c
	$(CC) $(CFLAGS) -o lanceur.out $(SRC)/lanceur.c

Parseur.out : $(OBJ)/vocal.o $(OBJ)/dictionnaire.o
	$(CC) $(CFLAGS) -o Parseur.out $(OBJ)/vocal.o $(OBJ)/dictionnaire.o

TestDico.out : $(OBJ)/test_dictionnaire.o $(OBJ)/dictionnaire.o
	$(CC) $(CFLAGS) -o TestDico.out $(OBJ)/test_dictionnaire.o $(OBJ)/dictionnaire.o

test_image.out: $(OBJ)/test_image.o $(OBJ)/module_image.o $(OBJ)/pile.o
	$(CC) $(CFLAGS) -o test_image.out $(OBJ)/test_image.o $(OBJ)/module_image.o $(OBJ)/pile.o -lm


$(OBJ)/module_image.o: $(SRC)/module_image.c
	$(CC) $(CFLAGS) -c $(SRC)/module_image.c -o $(OBJ)/module_image.o

$(OBJ)/pile.o: $(SRC)/pile.c $(INC)/pile.h
	$(CC) $(CFLAGS) -c $(SRC)/pile.c -o $(OBJ)/pile.o

$(OBJ)/test_image.o: $(TEST)/test_image.c $(INC)/module_image.h
	$(CC) $(CFLAGS) -c $(TEST)/test_image.c -o $(OBJ)/test_image.o

$(OBJ)/main.o: $(SRC)/main.c $(INC)/menu.h
	$(CC) $(CFLAGS) -c $(SRC)/main.c -o $(OBJ)/main.o

$(OBJ)/menu.o: $(SRC)/menu.c $(INC)/menu.h $(INC)/config.h $(INC)/log.h
	$(CC) $(CFLAGS) -c $(SRC)/menu.c -o $(OBJ)/menu.o

$(OBJ)/config.o: $(SRC)/config.c $(INC)/config.h
	$(CC) $(CFLAGS) -c $(SRC)/config.c -o $(OBJ)/config.o

$(OBJ)/log.o: $(SRC)/log.c $(INC)/log.h
	$(CC) $(CFLAGS) -c $(SRC)/log.c -o $(OBJ)/log.o

$(OBJ)/vocal.o : $(SRC)/vocal.c $(INC)/dictionnaire.h
	$(CC) $(CFLAGS) -c $(SRC)/vocal.c -o $(OBJ)/vocal.o

$(OBJ)/test_dictionnaire.o : $(TEST)/test_dictionnaire.c $(INC)/dictionnaire.h
	$(CC) $(CFLAGS) -c $(TEST)/test_dictionnaire.c -o $(OBJ)/test_dictionnaire.o

$(OBJ)/dictionnaire.o : $(SRC)/dictionnaire.c $(INC)/dictionnaire.h
	$(CC) $(CFLAGS) -c $(SRC)/dictionnaire.c -o $(OBJ)/dictionnaire.o

# Création de l'environnement virtuel Python et installation des dépendances
env:
	mkdir -p $(OBJ)
	python3 -m venv .venv
	source .venv/bin/activate
	pip install SpeechRecognition PyAudio gTTS



clean:
	rm -rf $(OBJ)/*.o *.out