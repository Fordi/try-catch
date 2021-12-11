CC=gcc

CC=gcc
CFLAGS=-I.
DEPS = try-catch.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

try-catch-ex: try-catch.o try-catch-ex.o
	$(CC) -o try-catch-ex try-catch.o try-catch-ex.o 

clean:
	rm try-catch-ex *.o
