CC=gcc

FLAGS=-O1 -Wno-unused-result
LDFLAGS=-fopenmp

all: scale sched

scale: scale.c
	$(CC) $(FLAGS) scale.c -o scale $(LDFLAGS)

sched: sched.c
	$(CC) $(FLAGS) sched.c -o sched $(LDFLAGS)

clean:
	rm -rf scale sched
