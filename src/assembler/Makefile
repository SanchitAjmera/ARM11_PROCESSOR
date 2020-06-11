CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: symbol_table.o file_lines.o assemble_util.o

symbol_table.o: symbol_table.h
file_lines.o: file_lines.h
assemble_util.o: assemble_util.h

clean:
		rm -f $(wildcard *.o)
		rm -f assemble
		rm -f emulate
		rm -f testme
