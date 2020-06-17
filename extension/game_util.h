#ifndef GAME_UTIL_H
#define GAME_UTIL_H

typedef struct item_t item_t;
typedef struct state state;
#include "characters/player/player.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ROOM_HISTORY (5)
#define ROOM_COUNT (5)
#define CLEAR ("clear")
#define ITEM_NUM (10)
#define LOOKUP_FAILURE (NULL)
#define INITIAL_CASH (0)
#define MAX_HEALTH (100)
#define FIND_FAIL (-1)
#define REMOVED (NULL)
#define MAX_PROPERTY (8)
#define PROPERTY_NUM (4)
#define USERNAME_CHAR_LIMIT (20)
#define TOTAL_ROOM_COUNT (25)
#define COMMAND_NUM (11)
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
} ItemIndex;

typedef enum {
  EAST = 1,
  WEST = 3,
  NORTH = 2,
  SOUTH = 0,
  CENTRE = 4
} RoomPosition;

// enum for Items stored by person in inventory with respective cost
typedef enum { FOOD, KEYBOARD, MOUSE, MONITOR, CASH, PASS } Item;

// enum for properties of products
typedef enum { NONE, EDIBLE, VALUABLE = 2, BUYABLE = 4 } Property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } RoomName;

// enum for character type
typedef enum { BATMAN, UTA } Character;

// enum for menu choices
typedef enum { INVALID, NEW_GAME, LOAD_GAME, QUIT } Menu;

// enum for supported commands
typedef enum {
  EXIT,
  PICKUP,
  DROP,
  BUY,
  MOVE,
  VIEW,
  SAVE,
  HELP,
  SKIP,
  CONSUME,
  FIGHT
} Command;

// generic (string, enum) struct for lookups
typedef struct {
  char *key;
  int value;
} pair_t;

// struct for Items and their properties
struct item_t {
  char *key;
  Item name;
  ItemIndex gameItemID;
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

struct state {
  player_t *player;
  room_t *currentRoom; // struct for room_t structure

  struct {
    char *username;
    Character character;
    int score;
  } profile;
};
// Room_name room_history[MAX_ROOM_HISTORY];

static const pair_t propertyTable[] = {{"edible", EDIBLE},
                                       {"none", NONE},
                                       {"valuable", VALUABLE},
                                       {"buyable", BUYABLE}};

static const pair_t directionTable[] = {
    {"north", NORTH}, {"south", SOUTH}, {"east", EAST}, {"west", WEST}};

//
static const pair_t commandsTable[] = {
    {"exit", EXIT}, {"pickup", PICKUP},   {"drop", DROP},  {"buy", BUY},
    {"move", MOVE}, {"view", VIEW},       {"save", SAVE},  {"help", HELP},
    {"skip", SKIP}, {"consume", CONSUME}, {"fight", FIGHT}};

// Supported Items
static const item_t gameItems[] = {
    {"apple", FOOD, 0, EDIBLE, "An apple. Increases health by 5 when eaten!"},
    {"keyboard", 1, KEYBOARD, VALUABLE,
     "A keyboard. A programmer's best friend."},
    {"mouse", MOUSE, 2, NONE, "A mouse. Click and scroll for days."},
    {"monitor", MONITOR, 3, VALUABLE,
     "A monitor. Can't see your seg faults without it!"},
    {"cash", CASH, 4, VALUABLE,
     "Cash. I wonder what I could buy around here..."},
    {"pass", PASS, 5, VALUABLE, "You shall not pass"},
    {"apple", FOOD, 6, (BUYABLE | EDIBLE),
     "Pay 5 HuxCoins to get an apple! (Health += 5)"},
    {"tesco meal deal", FOOD, 7, (BUYABLE | EDIBLE),
     "Pay 20 HuxCoins for a Tesco meal deal! (Health += 20)"},
    {"coffee", FOOD, 8, (BUYABLE | EDIBLE),
     "Pay 10 HuxCoins for some coffee to get through those lectures! (Health "
     "+= 10)"},
    {"rum & coke", FOOD, 9, (BUYABLE | EDIBLE),
     "Pay 50 HuxCoins and get drunk ;) ! (Health += 50)"}};

extern building_t *initialiseBuilding(room_t **out);
extern void freeBuilding(building_t *huxley);
extern state *initialiseState(room_t *initialRoom);
extern item_t *initialiseItem(ItemIndex gameItemID);
extern void freeState(state *state1);
extern void quit();
extern bool hasProperty(Property property, const item_t *item);
extern int lookup(const pair_t table[], const int size, const char *key);
extern const item_t *itemLookup(const item_t table[], const int size,
                                const char *key);
extern void lowercase(char *in);
extern void checkPtr(const void *ptr);
extern int loadGameState(const char *fname, state *playerState,
                         room_t **worldMap);
extern int saveGameState(const char *fname, state *playerState,
                         room_t **worldMap);

#endif
