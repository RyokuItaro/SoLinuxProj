all:
	gcc -o daemon $(wildcard *.c) -lm -fcompare-debug-second -w -I headers

.PHONY: clean
clean:
	rm -f *.o