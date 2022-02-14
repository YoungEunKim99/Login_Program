all: cli srv

CC = gcc

cli: cli.o
	gcc -o cli cli.o

cli.o: cli.c
	gcc -c  cli.c 

srv: srv.o
	gcc -o srv srv.o

srv.o: srv.c
	gcc -c srv.c 
