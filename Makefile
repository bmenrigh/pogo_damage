CC=gcc

CFLAGS=-std=c99 -Wall -Wextra -march=native -O0 -g
#CFLAGS=-std=c99 -Wall -Wextra -march=native -O0 -frounding-math
LIBS=-lm

main: cp

cp: cp.o
	$(CC) $(CFLAGS) -o cp cp.c $(LIBS)

cp.o: cp.c
	$(CC) $(CFLAGS) -c cp.c

clean:
	rm -f cp
	rm -f *.o
	rm -f *~
	rm -f \#*

