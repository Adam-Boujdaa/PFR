module_image.out: module_image.o main.o
	gcc -o module_image.out module_image.o main.o -lm

module_image.o: module_image.c 
	gcc -c module_image.c -o module_image.o -W -Wall -ansi -pedantic -std=c99

main.o: main.c module_image.h
	gcc -c main.c -o main.o -W -Wall -ansi -pedantic

main: main.o module_image.o
	gcc -o main.out main.o module_image.o -lm

test_image.o: test_image.c module_image.h
	gcc -c test_image.c -o test_image.o -W -Wall -ansi -pedantic -std=c99

test_image: test_image.o module_image.o
	gcc -o test_image.out test_image.o module_image.o -lm

clean:
	rm -rf *.o *.out
	
	
				