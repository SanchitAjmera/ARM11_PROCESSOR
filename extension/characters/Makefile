CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

libs:
	cd player; make

clean:
		rm -f $(wildcard *.o)
		cd player; make clean
