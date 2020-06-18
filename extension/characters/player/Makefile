CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: player.o

player.o: player.h ../../game_utils/game_util.h

clean:
		rm -f $(wildcard *.o)
