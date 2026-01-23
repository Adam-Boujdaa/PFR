menu.out: main.o menu.o utils.o lang.o log.o module_image.o pile.o
	gcc -o menu.out main.o menu.o utils.o lang.o log.o module_image.o pile.o -lm

module_image.o: module_image.c pile.o
	gcc -c module_image.c -o module_image.o -W -Wall -ansi -pedantic -std=c99

pile.o: pile.c pile.h
	gcc -c pile.c -o pile.o -W -Wall -ansi -pedantic -std=c99

main: main.o module_image.o
	gcc -o main.out main.o module_image.o -lm

test_image.o: test_image.c module_image.h
	gcc -c test_image.c -o test_image.o -W -Wall -ansi -pedantic -std=c99

test_image: test_image.o module_image.o pile.o
	gcc -o test_image.out test_image.o module_image.o pile.o -lm

# compilation de main.c
main.o: main.c menu.h
	gcc -c main.c

# compilation de menu.c
menu.o: menu.c menu.h utils.h lang.h log.h
	gcc -c menu.c

# compilation de utils.c
utils.o: utils.c utils.h
	gcc -c utils.c

# compilation de lang.c
lang.o: lang.c lang.h
	gcc -c lang.c

# compilation de log.c
log.o: log.c log.h
	gcc -c log.c

lanceur.out : lanceur.c
	gcc -o lanceur.out lanceur.c

# Édition de lien et création des exécutables .out
# ###############################################
Parseur.out : vocal.o dictionnaire.o
	gcc -o Parseur.out vocal.o dictionnaire.o

TestDico.out : test_dictionnaire.o dictionnaire.o
	gcc -o TestDico.out test_dictionnaire.o dictionnaire.o

# Création des fichiers objet .o
# ###############################
vocal.o : vocal.c dictionnaire.h
	gcc -c vocal.c

test_dictionnaire.o : test_dictionnaire.c dictionnaire.h
	gcc -c test_dictionnaire.c

dictionnaire.o : dictionnaire.c dictionnaire.h
	gcc -c dictionnaire.c


clean:
	rm -rf *.o *.out