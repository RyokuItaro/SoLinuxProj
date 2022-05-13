all:
	gcc -o daemon $(wildcard *.c) -I headers -lm

clean:
	rm daemon
