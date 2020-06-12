#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOM_HISTORY (5)

// enum for items stored by person in inventory
typedef enum { ITEM1, ITEM2, ETC } item;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } room_name;

// enum for character type
typedef enum { BATMAN, TONY, UTA } character;

typedef struct room {
  room_name current_room;
  struct room *adjacent_rooms;
  int adjacent_room_count;
} room;

typedef struct {
  item *inventory;
  int cash;
  int health;
  room curr_room_node; // struct for room structure

  struct {
    char *username;
    character character;
  } profile;

  room_name room_history[MAX_ROOM_HISTORY];
} state;

extern room *initialiseRoom(room_name current_room);
extern void connectRoom(room *first, room second);
extern void printRoom(room_name name);
extern void freeRoom(room *room1);
