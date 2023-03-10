CFLAGS = -Wall -Wextra -g

all: demo random fsrandom

demo: gol_demo.c
	gcc -o demo gol_demo.c $(CFLAGS)

random: gol_random.c
	gcc -o random gol_random.c $(CFLAGS)

fsrandom: gol_fsrandom.c
	gcc -o fsrandom gol_fsrandom.c $(CFLAGS)

clean:
	rm demo random fsrandom
