# Édition de lien et création de l'exécutable final
# #################################################
Parseur.out : main.o dictionnaire.o parseur.o
	gcc -o Parseur.out main.o dictionnaire.o parseur.o

# Création des fichiers objet .o
# ##############################

# Compilation du main
main.o : src/main.c include/dictionnaire.h include/parseur.h
	gcc -c src/main.c

# Compilation du module parseur
parseur.o : src/parseur.c include/parseur.h include/dictionnaire.h
	gcc -c src/parseur.c

# Compilation du module dictionnaire
dictionnaire.o : src/dictionnaire.c include/dictionnaire.h
	gcc -c src/dictionnaire.c

# Autres directives
# #################
clean:
	rm -rf *.o Parseur.out