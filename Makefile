all:ppm_main

ppm_main: ppm_main.cpp images.o
	g++ -o ppm_main ppm_main.cpp images.o

images.o: images.h images.cpp
	g++ -c -o images.o images.cpp


clean:
	rm *.o
