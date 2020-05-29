CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean test

all: assemble emulate testme

assemble: assemble.o
	$(CC) $(CFLAGS) assemble.o -o assemble
emulate: emulate.o emulate_util.o
	$(CC) $(CFLAGS) emulate.o emulate_util.o -o emulate
testme: testme.o emulate_util.o
	$(CC) $(CFLAGS) testme.o emulate_util.o -o testme

emulate.o: emulate_util.h emulate.c
	$(CC) $(CFLAGS) -c emulate.c
emulate_util.o: emulate_util.h emulate_util.c
	$(CC) $(CFLAGS) -c emulate_util.c
testme.o: emulate_util.h testme.c
	$(CC) $(CFLAGS) -c testme.c

test:
	./emulate ~/arm11_testsuite/test_cases/add01
	./testme | grep ^T


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f testme
