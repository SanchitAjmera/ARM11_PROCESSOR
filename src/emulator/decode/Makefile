CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

emulate_decode.o: ../emulate_constants.h ../emulate_util.h ../../common/constants.h ../../common/util.h emulate_decode.h

clean:
		rm -f $(wildcard *.o)
