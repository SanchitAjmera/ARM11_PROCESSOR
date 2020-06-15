#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ROOM_HISTORY (5)
#define ROOM_COUNT (5)
#define CLEAR ("clear")

// enum for position in rooms
typedef enum { EAST, WEST, NORTH, SOUTH, CENTRE } RoomPosition;

// enum for Items stored by person in inventory with respective cost
typedef enum { APPLE, KEYBOARD, MOUSE, MONITOR, CASH, PASS } Item;

// enum for properties of products
typedef enum { EDIBLE = 1, THROWABLE = 2, VALUABLE = 4, BUYABLE = 8 } Property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } RoomName;

// enum for character type
typedef enum { BATMAN, UTA } character;

// struct for Items and their properties
typedef struct Item_t {
  char *key;
  Item name;
  Property properties;
  char *description;
  int hash;
} Item_t;

// structure for rooms
// valid for non cyclical room structure
typedef struct room_t {
  RoomName current_room;
  struct room_t **adjacent_rooms;
  RoomPosition position;
  int adjacent_room_count;
  char *description;
  Item_t **Items;
  int Item_count;

} room_t;

// structure for building consisting of room_history
typedef struct building_t {
  room_t *start_room;
} building_t;

typedef struct {
  Item_t *inventory;
  int cash;
  int health;
  room_t curr_room_node; // struct for room_t structure

  struct {
    char *username;
    character character;
  } profile;

  RoomName room_history[MAX_ROOM_HISTORY];
} state;

static const Item_t gameItems[] = {
    {"apple", APPLE, EDIBLE, "An apple. Increases health by 5 when eaten!", 0},
    {"keyboard", KEYBOARD, THROWABLE, "A keyboard. A programmer's best friend.",
     0},
    {"mouse", MOUSE, THROWABLE, "A mouse. Click and scroll for days.", 0},
    {"monitor", MONITOR, THROWABLE,
     "A monitor. Can't see your seg faults without it!", 0},
    {"cash", CASH, VALUABLE, "Cash. I wonder what I could buy around here...",
     0},
    {"pass", PASS, THROWABLE,
     "pass which lets you access the lab and battle the BOSSMAN", 0}};

extern building_t *initialiseBuilding();
extern void freeBuilding(building_t *huxley);
