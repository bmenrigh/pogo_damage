CC=gcc

CFLAGS=-std=c99 -Wall -Wextra -march=native -O0 -frounding-math
LIBS=-lm

main: damage

damage: damage.o
	$(CC) $(CFLAGS) -o damage damage.c $(LIBS)

damage.o: damage.c
	$(CC) $(CFLAGS) -c damage.c

clean:
	rm -f damage
	rm -f *.o
	rm -f *~
	rm -f \#*

