CFLAGS = -Wall -Wextra -g
OBJ = gol_demo.o gol_random.o gol_fsrandom.o libterm.o gol.o
DEPS = libterm.h gol.h

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)

all: demo random fsrandom

demo: gol_demo.o libterm.o gol.o
	gcc -o $@ $^ $(CFLAGS)

random: gol_random.o libterm.o gol.o
	gcc -o $@ $^ $(CFLAGS)

fsrandom: gol_fsrandom.o libterm.o gol.o
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) demo random fsrandom
