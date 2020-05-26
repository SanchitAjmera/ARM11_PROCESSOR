CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o

emulate.o: emulate.h

test:
	./emulate ~/arm11_testsuite/test_cases/add01

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
