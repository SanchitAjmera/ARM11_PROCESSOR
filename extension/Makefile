CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: libs game testme assembler

assembler: assembler.o ../src/emulator/libemulator.a ../src/assembler/libassembler.a ../src/common/util.o
game: print_util.o game_util.o characters/player/player.o
testme: testme.o game_util.o print_util.o characters/player/player.o
print_util.o: print_util.h constants.h game_util.h
game_util.o: game_util.h constants.h
testme.o: game_util.h print_util.h
assembler.o:
libs:
	cd characters; make

test:
	./testme | grep ^T:

clean:
		rm -f $(wildcard *.o)
		rm -f game
		rm -f testme
		cd characters; make clean
