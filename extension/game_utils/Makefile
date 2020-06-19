CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LIB			= file_util.o game_util.o item_util.o room_util.o
LIBNAME	= libgame_util.a

.PHONY: all clean

all: $(LIB) lib
file_util.o:
game_util.o:
item_util.o:
room_util.o:
lib:
	ar rcs $(LIBNAME) $(LIB)

clean:
		rm -f $(wildcard *.o)
		rm -f $(LIBNAME)
