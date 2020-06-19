CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all clean

all: libs

libs:
	cd utils; make
	cd file_lines; make
	cd symbol_table; make

clean:
		rm -f $(wildcard *.o)
		cd utils; make clean
		cd file_lines; make clean
		cd symbol_table; make clean
