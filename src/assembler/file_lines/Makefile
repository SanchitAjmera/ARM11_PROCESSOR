CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all clean

all: file_lines.o

file_lines.o: file_lines.h

clean:
		rm -f $(wildcard *.o)
