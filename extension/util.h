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

// structure for rooms
typedef struct room {
  room_name current_room;
  struct room **adjacent_rooms;
  int adjacent_room_count;
  char *description;
} room;

// structure for building consisting of room_history
typedef struct building {
  room *start_room;
} building;

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

extern building *initialiseBuilding();
extern void printRoomDetails(room *room1);
extern void freeBuilding(building *huxley);
