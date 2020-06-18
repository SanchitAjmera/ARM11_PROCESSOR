
CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic


.PHONY: all

all: libs game testme

game: print_util.o game_util.o characters/player/player.o characters/boss/boss.o battle/battle.o emulateARM/emulateARM.o ../src/emulator/libemulator.a ../src/assembler/libassembler.a ../src/common/util.o
testme: testme.o game_util.o print_util.o characters/player/player.o battle/battle.o characters/boss/boss.o emulateARM/emulateARM.o ../src/emulator/libemulator.a ../src/assembler/libassembler.a ../src/common/util.o
print_util.o: print_util.h constants.h game_util.h
game_util.o: game_util.h constants.h
testme.o: game_util.h print_util.h
emulateARM.o:
libs:
	cd characters; make
	cd battle; make
	cd emulateARM; make

test:
	./testme | grep ^T:

clean:
		rm -f $(wildcard *.o)
		rm -f game
		rm -f testme
		rm -f test_save_file.dat
		cd characters; make clean
		cd battle; make clean
		cd emulateARM; make clean
