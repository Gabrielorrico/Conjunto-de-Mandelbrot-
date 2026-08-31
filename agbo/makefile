CC = gcc
CFLAGS = -fopenmp -pthread -lm -Wall -Wextra -g 

all: mandelbrot

mandelbrot: src/main.c
	$(CC) src/main.c -o mandelbrot $(CFLAGS)

clean: 
	rm -f src/*.o mandelbrot *.pgm times.txt

.PHONY: all clean



