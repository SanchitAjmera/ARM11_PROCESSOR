CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

emulate_execute.o: emulate_execute_dpi.o ../emulate_constants.h ../emulate_util.h ../../common/constants.h ../../common/util.h emulate_execute_dpi.h emulate_execute.h
emulate_execute_dpi.o: ../emulate_constants.h ../emulate_util.h ../../common/constants.h ../../common/util.h

clean:
		rm -f $(wildcard *.o)
