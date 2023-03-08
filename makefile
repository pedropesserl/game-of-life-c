CFLAGS = -Wall -g

all: demo random

demo: gol_demo.c
	gcc -o demo gol_demo.c $(CFLAGS)

random: gol_random.c
	gcc -o random gol_random.c $(CFLAGS)

clean:
	rm demo random
