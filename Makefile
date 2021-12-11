CC=gcc

CC=gcc
CFLAGS=-I.
DEPS = try-catch.h

build/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build/try-catch-ex: build/try-catch.o build/try-catch-ex.o
	$(CC) -o build/try-catch-ex build/try-catch.o build/try-catch-ex.o 

clean:
	rm build/*
