CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

assemble_util.o: assemble_util.h ../../common/constants.h ../../common/util.h ../assemble_constants.h ../file_lines.h ../symbol_table.h
assemble_dpi.o: ../../common/constants.h ../../common/util.h ../assemble_constants.h assemble_util.h

clean:
		rm -f $(wildcard *.o)
