CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: emulateARM.o lib

emulateARM.o:
lib:
	ar rcs libemulateARM.a emulateARM.o ../../src/emulator/libemulator.a ../../src/assembler/libassembler.a ../../src/common/util.o

clean:
		rm -f $(wildcard *.o)
