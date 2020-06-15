CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: game

game: print_util.o game_util.o
print_util.o: print_util.h constants.h
game_util.o: game_util.h constants.h

clean:
		rm -f $(wildcard *.o)
		rm -f game
