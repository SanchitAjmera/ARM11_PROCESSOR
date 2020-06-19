CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all

all: emulate_ARM.o lib

emulate_ARM.o:
	$(CC) $(CFLAGS) ../../src/assembler/symbol_table.c emulator_ARM.c -c emulate_ARM.o
lib:
	ar rcs libemulate_ARM.a emulate_ARM.o ../../src/emulator/libemulator.a ../../src/assembler/libassembler.a ../../src/common/util.o

clean:
		rm -f $(wildcard *.o)
		rm -f $(wildcard *.a)
