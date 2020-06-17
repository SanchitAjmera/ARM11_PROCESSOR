CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

boss_test: boss_test.o boss.o
boss_test.o: boss.h
boss.o: boss.h ../../battle/battle.h ../player/player.h boss_constants.h

clean:
		rm -f $(wildcard *.o)
		rm -f boss_test
