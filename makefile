CFLAGS = -Wall -g

all: gol.c
	gcc -o gol gol.c $(CFLAGS)

clean:
	rm -f gol
