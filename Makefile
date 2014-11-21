all:ppm_main

ppm_main: ppm_main.cpp ppm_fonctions.o
	g++ -o ppm_main ppm_main.cpp ppm_fonctions.o

ppm_fonctions.o: ppm_fonctions.h ppm_fonctions.cpp
	g++ -c -o ppm_fonctions.o ppm_fonctions.cpp


clean:
	rm *.o
