CC      = gcc
CFLAGS  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate testme

assemble: assemble.o

emulate: emulate.o emulate_util.o

testme: testme.o emulate_util.o

emulate.o: emulate_util.h

emulate_util.o: emulate_util.h

testme.o: emulate_util.h

test:
	./emulate ~/arm11_testsuite/test_cases/add01
	./testme | grep ^T

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
