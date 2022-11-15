CC = gcc
CFLAGS = -Wall -c

all: mystring exit cd pwd slash run

mystring : mystring.c
	$(CC) $(CFLAGS) mystring.c
	
exit: exit.c
	$(CC) $(CFLAGS) exit.c

cd: cd.c 
	$(CC) $(CFLAGS) cd.c

pwd: pwd.c
	$(CC) $(CFLAGS) pwd.c
	
slash: slash.c
	$(CC) $(CFLAGS) slash.c 

run: slash.c
	$(CC) mystring.o exit.o cd.o pwd.o slash.o -o slash -lreadline 

clean: 
	$(RM) slash.o mystring.o exit.o cd.o pwd.o slash 