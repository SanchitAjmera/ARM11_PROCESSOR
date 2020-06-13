#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROOM_HISTORY (5)
#define ROOM_COUNT (5)

// enum for position in room
typedef enum { LEFT, CENTRE, RIGHT, TOP, BOTTOM } position;

// enum for items stored by person in inventory
typedef enum { ITEM1, ITEM2, ETC } item;

// enum for properties of products
typedef enum { PROP1, PROP2, PROP3 } property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } room_name;

// enum for character type
typedef enum { BATMAN, TONY, UTA } character;

// enum for bosses
typedef enum { KONSTANTINOS, LANCASTER, TONY } bosses;

// struct for items and their properties
typedef struct item_t {

  item name;
  property *properties;

} item_t;

// structure for rooms
// valid for no cyclical room structure
typedef struct room_t {
  room_name current_room;

  room_t **adjacent_rooms;
  int adjacent_room_count;
  char *description;
  item_t **items;

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
