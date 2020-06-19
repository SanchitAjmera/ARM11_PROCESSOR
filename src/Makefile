CC      	= gcc
CFLAGS 		= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
EMULATE 	= emulator/fetch/emulate_fetch.o emulator/decode/emulate_decode.o emulator/execute/emulate_execute.o
ASSEMBLE 	= assembler/assemble_util.o assembler/file_lines.o assembler/symbol_table.o

.SUFFIXES: .c .o

.PHONY: all clean test

all: libs assemble emulate testme

assemble: assemble.o assembler/libassembler.a common/util.o
emulate: emulate.o emulator/libemulator.a common/util.o
testme: testme.o $(EMULATE) common/util.o

libs:
	cd assembler; make;
	cd emulator; make;
	cd common; make;

test:
	./testme | grep ^T

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f testme
	cd emulator; make clean;
	cd assembler; make clean;
	cd common; make clean;
