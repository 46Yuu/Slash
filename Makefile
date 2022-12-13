CC = gcc
CFLAGS = -Wall -c

all: mystring exit cd pwd cext etoile slash run 

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

cext: cext.c
	$(CC) $(CFLAGS) cext.c 

cext: etoile.c
	$(CC) $(CFLAGS) etoile.c

run: slash.c
	$(CC) mystring.o exit.o cd.o pwd.o  etoile.o cext.o  slash.o -o slash -lreadline 

clean: 
	$(RM) slash.o mystring.o exit.o cd.o pwd.o etoile.o cext.o slash