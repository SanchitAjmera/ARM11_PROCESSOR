#ifndef GAME_UTIL_H
#define GAME_UTIL_H

typedef struct item_t item_t;
#include "characters/player/player.h"
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
#define ITEM_NUM (5)
#define LOOKUP_FAILURE (NULL)
#define INITIAL_CASH (0)
#define MAX_HEALTH (100)
#define FIND_FAIL (-1)
#define REMOVED (NULL)
#define MAX_PROPERTY (8)
#define PROPERTY_NUM (4)
#define USERNAME_CHAR_LIMIT (20)
#define TOTAL_ROOM_COUNT (25)
#define COMMAND_NUM (5)
#define DIR_NUM (4)

// enum for position in rooms

// enum for
typedef enum {
  APPLE_INDEX = 0,
  KEYBOARD_INDEX = 1,
  MOUSE_INDEX = 2,
  MONITOR_INDEX = 3,
  CASH_INDEX = 4,
  PASS_INDEX = 5,
  BUYAPPLE_INDEX = 6,
  TESCO_INDEX = 7,
  COFFEE_INDEX = 8,
  RUM_INDEX = 9
} Index;

typedef enum { EAST = 0, WEST = 1, NORTH = 2, SOUTH = 3, CENTRE } RoomPosition;

// enum for Items stored by person in inventory with respective cost
typedef enum { FOOD = 0, KEYBOARD, MOUSE, MONITOR, CASH, PASS } Item;

// enum for properties of products
typedef enum { EDIBLE = 1, THROWABLE = 2, VALUABLE = 4, BUYABLE = 8 } Property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } RoomName;

// enum for character type
typedef enum { BATMAN, UTA } Character;

// enum for menu choices
typedef enum { NONE, QUIT, NEW_GAME, LOAD_GAME } Menu;

// enum for supported commands
typedef enum { EXIT, PICKUP, DROP, BUY, MOVE, VIEW } Command;

// generic (string, enum) struct for lookups
typedef struct {
  char *key;
  int value;
} pair_t;

// struct for Items and their properties
struct item_t {
  char *key;
  Item name;
  uint8_t properties;
  char *description;
  int hash;
  int cost;
};

// structure for rooms
// valid for non cyclical room structure
typedef struct room_t {
  RoomName current_room;
  struct room_t **adjacent_rooms;
  RoomPosition position;
  int adjacent_room_count;
  char *description;
  item_t **items;
  int itemCount;
  int id;
} room_t;

// structure for building consisting of room_history
typedef struct building_t {
  room_t *startRoom;
} building_t;

typedef struct {
  player_t *player;
  room_t *currentRoom; // struct for room_t structure

  struct {
    char *username;
    Character character;
    int score;
  } profile;

  // Room_name room_history[MAX_ROOM_HISTORY];
} state;

static const pair_t propertyTable[] = {{"edible", EDIBLE},
                                       {"throwable", THROWABLE},
                                       {"valuable", VALUABLE},
                                       {"buyable", BUYABLE}};

static const pair_t directionTable[] = {
    {"north", NORTH}, {"south", SOUTH}, {"east", EAST}, {"west", WEST}};

//
static const pair_t commandsTable[] = {{"exit", EXIT}, {"pickup", PICKUP},
                                       {"drop", DROP}, {"buy", BUY},
                                       {"move", MOVE}, {"view", VIEW}};

// Supported Items
static item_t gameItems[] = {
    {"apple", FOOD, EDIBLE, "An apple. Increases health by 5 when eaten!"},
    {"keyboard", KEYBOARD, THROWABLE,
     "A keyboard. A programmer's best friend."},
    {"mouse", MOUSE, THROWABLE, "A mouse. Click and scroll for days."},
    {"monitor", MONITOR, THROWABLE,
     "A monitor. Can't see your seg faults without it!"},
    {"cash", CASH, VALUABLE, "Cash. I wonder what I could buy around here..."},
    {"pass", PASS, THROWABLE, "You shall not pass"},
    {"apple", FOOD, (BUYABLE | EDIBLE),
     "Pay 5 HuxCoins to get an apple! (Health += 5)"},
    {"Tesco Meal Deal", FOOD, (BUYABLE | EDIBLE),
     "Pay 20 HuxCoins for a Tesco meal deal! (Health += 20)"},
    {"Coffee", FOOD, (BUYABLE | EDIBLE),
     "Pay 10 HuxCoins for some coffee to get through those lectures! (Health "
     "+= 10)"},
    {"Rum & Coke", FOOD, (BUYABLE | EDIBLE),
     "Pay 50 HuxCoins and get drunk ;) ! (Health += 50)"}};

extern building_t *initialiseBuilding(room_t **out);
extern void freeBuilding(building_t *huxley);
extern state *initialiseState(room_t *initialRoom);
extern item_t *initialiseItem(item_t gameItem);
extern void freeState(state *state1);
extern void quit();
extern bool hasProperty(Property property, item_t *item);
extern int lookup(const pair_t table[], const int size, const char *key);
extern item_t *itemLookup(item_t table[], const int size, const char *key);
extern void lowercase(char *in);
extern void checkPtr(const void *ptr);
extern int loadGameState(const char *fname, state *playerState,
                         room_t **worldMap);
extern int saveGameState(const char *fname, state *playerState,
                         room_t **worldMap);

#endif
