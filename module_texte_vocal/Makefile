# Édition de lien et création des exécutables .out
# ###############################################
Parseur.out : main.o dictionnaire.o
	gcc -o Parseur.out main.o dictionnaire.o

TestDico.out : test_dictionnaire.o dictionnaire.o
	gcc -o TestDico.out test_dictionnaire.o dictionnaire.o

# Création des fichiers objet .o
# ###############################
main.o : main.c dictionnaire.h
	gcc -c main.c

test_dictionnaire.o : test_dictionnaire.c dictionnaire.h
	gcc -c test_dictionnaire.c

dictionnaire.o : dictionnaire.c dictionnaire.h
	gcc -c dictionnaire.c

# Autres directives
# ########################
clean:
	rm -rf *.o *.out