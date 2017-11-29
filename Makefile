CC=gcc
CFLAGS=-c -O2 -Wall -g

all: main

main: main.o
	$(CC) main.o -o main -lpthread

main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	/bin/rm -f main *.o *.gz

run:
	./main 8 4 10

tarball:
	# put your tar command here
	# tar -cvzf <lastname>.tar.gz *
