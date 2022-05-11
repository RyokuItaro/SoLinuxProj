SOURCES := $(wildcard *.c)

all:
	gcc -o demon $(SOURCES) -I headers -lm

clean:
	rm demon
	
install: all
	cp demon /usr/bin

uninstall:
	rm /usr/bin/demon
