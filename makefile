all:
	gcc -o daemon $(wildcard *.c) -lm -I headers

.PHONY: clean
clean:
	rm -f *.o