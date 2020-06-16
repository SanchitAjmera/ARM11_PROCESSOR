CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: all clean

# all: emulate_fetch.o emulate_decode.o emluate_execute.o

emulate_fetch.o: fetch/emulate_fetch.h emulate_constants.h ../common/constants.h ../common/util.h
emulate_decode.o: decode/emulate_decode.h emulate_constants.h ../common/constants.h ../common/util.h
emluate_execute.o: execute/emulate_execute.h emulate_constants.h ../common/constants.h ../common/util.h

clean:
		rm -f $(wildcard *.o)
