CC			= gcc
CFLAGS	= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LIB			= print_items_util.o print_kgk_util.o print_player_util.o print_room_util.o print_util.o

.PHONY: all clean

all: $(LIB) lib
print_items_util.o:
print_kgk_util.o:
print_player_util.o:
print_room_util.o:
print_util.o:
lib:
	ar rcs libprint_utils.a $(LIB)

clean:
		rm -f $(wildcard *.o)
		rm -f libprint_utils.a
