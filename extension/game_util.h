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
<<<<<<< HEAD
#define Item_NUM (5)
=======
#define ITEM_NUM (5)
>>>>>>> extension-player
#define LOOKUP_FAILURE (NULL)
#define INITIAL_CASH (0)
#define MAX_HEALTH (100)
#define FIND_FAIL (-1)
#define REMOVED (NULL)
#define MAX_PROPERTY (8)
<<<<<<< HEAD

// enum for position in rooms
typedef enum { EAST, WEST, NORTH, SOUTH, CENTRE } RoomPosition;

// enum for Items stored by person in inventory with respective cost
typedef enum { APPLE, KEYBOARD, MOUSE, MONITOR, CASH, PASS } Item;
=======
#define PROPERTY_NUM (4)

// enum for items stored by person in inventory
typedef enum { APPLE, KEYBOARD, MOUSE, MONITOR, CASH } Item;
>>>>>>> extension-player

// enum for properties of products
typedef enum { EDIBLE = 1, THROWABLE = 2, VALUABLE = 4, BUYABLE = 8 } Property;

// enum for room
<<<<<<< HEAD
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } RoomName;

// enum for character type
typedef enum { BATMAN, UTA } Character;
=======
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } Room_name;

// enum for character type
typedef enum { BATMAN, UTA } Character;

// generic (string, enum) struct for lookups
typedef struct {
  char *key;
  int value;
} pair_t;

// struct for items and their properties
typedef struct item_t {

  char *key;
  Item name;
  uint8_t properties;
  char *description;
>>>>>>> extension-player

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
<<<<<<< HEAD
  RoomName current_room;
=======
  Room_name current_room;
>>>>>>> extension-player
  struct room_t **adjacent_rooms;
  RoomPosition position;
  int adjacent_room_count;
  char *description;
  Item_t **Items;
  int ItemCount;

} room_t;

// structure for building consisting of room_history
typedef struct building_t {
  room_t *start_room;
} building_t;

typedef struct {
<<<<<<< HEAD
  Item_t **inventory;
  int ItemCount;
=======
  item_t **inventory;
  int itemCount;
>>>>>>> extension-player
  int health;
  int cash;
} player_t;

typedef struct {
  player_t *player;
  room_t *curr_room_node; // struct for room_t structure

  struct {
    char *username;
    Character character;
    int score;
  } profile;

<<<<<<< HEAD
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
=======
  Room_name room_history[MAX_ROOM_HISTORY];
} state;

static const pair_t propertyTable[] = {{"edible", EDIBLE},
                                       {"throwable", THROWABLE},
                                       {"valuable", VALUABLE},
                                       {"buyable", BUYABLE}};
// Supported Items
static const item_t gameItems[] = {
    {"apple", APPLE, EDIBLE, "An apple. Increases health by 5 when eaten!"},
    {"keyboard", KEYBOARD, THROWABLE,
     "A keyboard. A programmer's best friend."},
    {"mouse", MOUSE, THROWABLE, "A mouse. Click and scroll for days."},
    {"monitor", MONITOR, THROWABLE,
     "A monitor. Can't see your seg faults without it!"},
    {"cash", CASH, VALUABLE, "Cash. I wonder what I could buy around here..."}};
>>>>>>> extension-player

extern building_t *initialiseBuilding();
extern void freeBuilding(building_t *huxley);
extern state *initialiseState(room_t *initialRoom);
extern void freeState(state *state1);