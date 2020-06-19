CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all clean

all: symbol_table.o

symbol_table.o: symbol_table.h

clean:
		rm -f $(wildcard *.o)
