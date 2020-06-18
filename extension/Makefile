CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic


.PHONY: all

all: libs game testme

game: emulate_ARM/emulate_ARM.o ../src/emulator/libemulator.a ../src/assembler/libassembler.a ../src/common/util.o print_utils/libprint_utils.a game_utils/libgame_util.a characters/player/player.o battle/battle.o characters/boss/boss.o
testme: testme.o game_utils/libgame_util.a characters/player/player.o battle/battle.o characters/boss/boss.o emulate_ARM/emulate_ARM.o print_utils/libprint_utils.a ../src/emulator/libemulator.a ../src/assembler/libassembler.a ../src/common/util.o
testme.o:
libs:
	cd ../src; make
	cd emulate_ARM; make
	cd game_utils; make
	cd print_utils; make
	cd characters; make
	cd battle; make


test:
	./testme | grep ^T:

clean:
		rm -f $(wildcard *.o)
		rm -f game
		rm -f testme
		rm -f test_save_file.dat
		cd ../src; make clean
		cd characters; make clean
		cd battle; make clean
		cd emulate_ARM; make clean
		cd game_utils; make clean
		cd print_utils; make clean
