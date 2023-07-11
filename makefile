CFLAGS = -Wall -Wextra -g
OBJ = gol_demo.o gol_random.o gol_fsrandom.c libterm.o
DEPS = libterm.h

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)

all: demo random fsrandom

demo: gol_demo.o libterm.o
	gcc -o $@ $^ $(CFLAGS)

random: gol_random.o
	gcc -o $@ $^ $(CFLAGS)

fsrandom: gol_fsrandom.c
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) demo random fsrandom
