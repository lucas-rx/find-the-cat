CC = gcc 
CFLAGS = -Wall -Wformat

main:
	$(CC) -c $(CFLAGS) src/*.c MegaMimes/src/MegaMimes.c;
	$(CC) -o ftc *.o;

clean:
	rm -rf ftc *.o

safe:
	$(CC) -c $(CFLAGS) -std=c99 -Wextra -pedantic -g3 src/*.c MegaMimes/src/MegaMimes.c;
	$(CC) -o ftc *.o;
