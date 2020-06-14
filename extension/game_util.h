#include "player.h"
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
#define PROPERTY_NUM (4)
#define LOOKUP_FAILURE (NULL)

// enum for items stored by person in inventory
typedef enum { APPLE, KEYBOARD, MOUSE, MONITOR, CASH } Item;

// enum for properties of products
typedef enum { EDIBLE, THROWABLE, VALUABLE } Property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } Room_name;

// enum for character type
typedef enum { BATMAN, UTA } Character;

// struct for items and their properties
typedef struct item_t {

  char *key;
  item name;
  Property properties[];
  int propertySize;
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
  item_t **inventory;
  int health;
  int cash;
} player_t;

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

// Supported Items
static const item_t gameItems[] = {
    {"apple",
     APPLE,
     {EDIBLE},
     1,
     "An apple. Increases health by 5 when eaten!"},
    {"keyboard",
     KEYBOARD,
     {THROWABLE},
     1,
     "A keyboard. A programmer's best friend."},
    {"mouse", MOUSE, {THROWABLE}, 1, "A mouse. Click and scroll for days."},
    {"monitor",
     MONITOR,
     {THROWABLE},
     1,
     "A monitor. Can't see your seg faults without it!"},
    {"cash",
     CASH,
     {VALUABLE},
     1,
     "Cash. I wonder what I could buy around here..."}};

extern building_t *initialiseBuilding();
extern void freeBuilding(building_t *huxley);
extern void printRemaining(void);
