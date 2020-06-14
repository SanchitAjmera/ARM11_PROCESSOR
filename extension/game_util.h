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
typedef enum { EAST, WEST, NORTH, SOUTH } room_position;

// enum for items stored by person in inventory with respective cost
typedef enum {
  KEYBOARD,
  PASS,
  TELEPORTER,
  KATSU_CURRY,
  TESCO_MEAL_DEAL,
  MONEY1,
  MONEY2,
  MONEY3
} item;

// enum for properties of products
typedef enum { CONSUMABLE, PROP2 } property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } room_name;

// enum for character type
typedef enum { BATMAN, UTA } character;

// struct for items and their properties
typedef struct item_t {
  item name;
  property *properties;
} item_t;

// structure for rooms
// valid for non cyclical room structure
typedef struct room_t {
  room_name current_room;
  struct room_t **adjacent_rooms;
  room_position position;
  int adjacent_room_count;
  char *description;
  item_t **items;
  int item_count;

} room_t;

// structure for building consisting of room_history
typedef struct building_t {
  room_t *start_room;
} building_t;

typedef struct {
  item_t *inventory;
  int cash;
  int health;
  room_t curr_room_node; // struct for room_t structure

  struct {
    char *username;
    character character;
  } profile;

  room_name room_history[MAX_ROOM_HISTORY];
} state;

extern building_t *initialiseBuilding();
extern void freeBuilding(building_t *huxley);
