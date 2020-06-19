CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: lib
lib:
	cd boss; make;
	cd player; make;

clean:
		rm -f $(wildcard *.o)
		cd boss; make clean;
		cd player; make clean;
