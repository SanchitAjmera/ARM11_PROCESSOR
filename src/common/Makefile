CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: util.o

util.o: util.h

clean:
		rm -f $(wildcard *.o)
		rm -f assemble
		rm -f emulate
		rm -f testme
