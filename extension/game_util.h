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

// enum for items stored by person in inventory
typedef enum { APPLE, KEYBOARD, MOUSE, MONITOR, CASH } item;

// enum for properties of products
typedef enum { EDIBLE, THROWABLE, VALUABLE, USABLE } property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } room_name;

// enum for character type
typedef enum { BATMAN, UTA } character;

// struct for items and their properties
typedef struct item_t {

  item name;
  property *properties;
  char *description;

} item_t;

// structure for rooms
// valid for no cyclical room structure
typedef struct room_t {
  room_name current_room;
  struct room_t **adjacent_rooms;
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
  player_t player;
  room_t curr_room_node; // struct for room_t structure

  struct {
    char *username;
    character character;
    int score;
  } profile;

  room_name room_history[MAX_ROOM_HISTORY];
} state;

const item_t[] = {{APPLE,
                   {EDIBLE},
                   {"An apple. Increases health by 5 when eaten!"}},
                  {KEYBOARD, {USABLE}}

}

                 extern building_t *
                 initialiseBuilding();
extern void freeBuilding(building_t *huxley);
extern void printRemaining(void);
