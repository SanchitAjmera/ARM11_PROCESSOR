CC      = gcc
CFLAGS 	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean test

all: libs assemble emulate testme

assemble: assemble.o assembler/assemble_util.o assembler/file_lines.o assembler/symbol_table.o
emulate: emulate.o emulator/emulate_util.o
testme: testme.o emulator/emulate_util.o
libs:
	cd assembler; make
	cd emulator; make

test:
	./emulate ~/arm11_testsuite/test_cases/add01
	./assemble ~/arm11_testsuite/test_cases/b01.s
	./testme | grep ^T

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	rm -f testme
	cd emulator; make clean
	cd assembler; make clean
