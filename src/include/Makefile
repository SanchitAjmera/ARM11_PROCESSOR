CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: symbol_table.o file_lines.o

symbol_table.o: symbol_table.c symbol_table.h constants.h
		$(CC) $(CFLAGS) -c symbol_table.c
file_lines.o: file_lines.h file_lines.c constants.h
		$(CC) $(CFLAGS) -c file_lines.c

clean:
		rm -f $(wildcard *.o)
		rm -f assemble
		rm -f emulate
		rm -f testme
