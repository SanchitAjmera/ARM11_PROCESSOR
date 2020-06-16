CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all clean

# all: emulate_fetch.o emulate_decode.o emluate_execute.o
libs:
	cd fetch; make;
	cd decode; make;
	cd execute; make;
	ar rcs libemulator.a decode/emulate_decode.o execute/emulate_execute.o fetch/emulate_fetch.o

clean:
		rm -f $(wildcard *.o)
		cd fetch; make clean;
		cd decode; make clean;
		cd execute; make clean;
