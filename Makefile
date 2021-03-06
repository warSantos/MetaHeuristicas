CC=gcc
#FLAGS=-g -Wall -c
FLAGS=-c -O2

main:	rep.o	sa.o	main.c
	gcc main.c rep.o sa.o -o main.out -lm

sa.o:	rep.o	src/sa.c	headers/sa.h
	gcc $(FLAGS) src/sa.c

rep.o:	src/rep.c	headers/rep.h 
	gcc $(FLAGS) src/rep.c

clean:
	rm -rf *.o main.out
