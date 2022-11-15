CC = gcc
CFLAGS = -Wall -o 

all: slash exit cd pwd

slash: slash.c
	$(CC) $(CFLAGS) slash slash.c -lreadline 

exit: exit.c
	$(CC) $(CFLAGS) exit exit.c

cd: cd.c 
	$(CC) $(CFLAGS) cd cd.c

pwd: pwd.c
	$(CC) $(CFLAGS) pwd pwd.c

clean: 
	$(RM) slash exit cd pwd 