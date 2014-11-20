all:ppm_main

ppm_main: ppm_main.c ppm_fonctions.o
	g++ -o ppm_main ppm_main.c ppm_fonctions.o

ppm_fonctions.o: ppm_fonctions.h ppm_fonctions.c
	g++ -c -o ppm_fonctions.o ppm_fonctions.c


clean:
	rm *.o
