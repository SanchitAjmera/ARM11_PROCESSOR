CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all clean

all: libs symbol_table.o file_lines.o

symbol_table.o: symbol_table.h
file_lines.o: file_lines.h
libs:
	cd utils; make

clean:
		rm -f $(wildcard *.o)
		cd utils; make clean
