#ifndef GAME_UTIL_H
#define GAME_UTIL_H

typedef struct item_t item_t;
typedef struct state state;
#include "../characters/boss/boss.h"
#include "../characters/player/player.h"
#include "../constants.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// enum for position in rooms

// enum for
typedef enum {
  PEAR_INDEX = 0,
  KEYBOARD_INDEX = 1,
  MOUSE_INDEX = 2,
  MONITOR_INDEX = 3,
  CASH_INDEX = 4,
  PASS_INDEX = 5,
  BUYAPPLE_INDEX = 6,
  TESCO_INDEX = 7,
  COFFEE_INDEX = 8,
  RUM_INDEX = 9,
  GUZZI_INDEX = 10,
  SUPREME_INDEX = 11,
  DRIP_INDEX = 12,
  BLM_INDEX = 13

} ItemIndex;

typedef enum {
  SOUTH = 0,
  EAST = 1,
  NORTH = 2,
  WEST = 3,
  CENTRE = 4
} RoomPosition;

// enum for Items stored by person in inventory with respective cost
typedef enum { FOOD, KEYBOARD, MOUSE, MONITOR, CASH, PASS } Item;

// enum for properties of products
typedef enum { NONE, EDIBLE, VALUABLE = 2, BUYABLE = 4 } Property;

// enum for room
typedef enum { LOBBY, LAB, LECTURE_HALL, FUSION, HARRODS } RoomName;

// SOUTH EAST WEST NORTH CENTRE

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
  FIGHT,
  FRESH
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
  int cost;
  int hash;
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
  boss_t *boss;
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
    {"exit", EXIT}, {"pickup", PICKUP},   {"drop", DROP},   {"buy", BUY},
    {"move", MOVE}, {"view", VIEW},       {"save", SAVE},   {"help", HELP},
    {"skip", SKIP}, {"consume", CONSUME}, {"fight", FIGHT}, {"clear", FRESH}};

// Supported Items
static const item_t gameItems[] = {
    {"pear", FOOD, 0, EDIBLE, "A pear. Increases health by 5 when eaten!", 5},
    {"keyboard", 1, KEYBOARD, VALUABLE,
     "A keyboard. A programmer's best friend."},
    {"mouse", MOUSE, 2, NONE, "A mouse. Click and scroll for days."},
    {"monitor", MONITOR, 3, VALUABLE,
     "A monitor. Can't see your seg faults without it!"},
    {"cash", CASH, 4, VALUABLE,
     "Cash. I wonder what I could buy around here..."},
    {"pass", PASS, 5, VALUABLE, "You shall not pass"},
    {"Pot Noodles", FOOD, 6, (BUYABLE | EDIBLE),
     "Pay 5 HuxCoins to get some Pot Noodles! (Health += 5)", 5},
    {"tesco meal deal", FOOD, 7, (BUYABLE | EDIBLE),
     "Pay 20 HuxCoins for a Tesco meal deal! (Health += 20)", 20},
    {"coffee", FOOD, 8, (BUYABLE | EDIBLE),
     "Pay 10 HuxCoins to get through those lectures! (Health += 10)", 10},
    {"rum & coke", FOOD, 9, (BUYABLE | EDIBLE),
     "Pay 50 HuxCoins and get drunk ;) ! (Health += 50)", 50},
    {"Guzzi Belt", FOOD, 10, (BUYABLE),
     "Bank transfer us our uni fees for 2021 and cop the 'Guzzi Belt'", 100},
    {"Supreme Hoodie", FOOD, 11, BUYABLE,
     " Flex like an international student everytime you go to lectures!", 40},
    {"Drip", FOOD, 12, BUYABLE,
     "Drip too hard don't stand to close for only 30 Huxcoins", 30},
    {"BLM Badge", FOOD, 13, BUYABLE,
     "Support da movement with a free BLM badge!", 0}};

static const char *descriptionTable[] = {
    "Every game needs a spawn point right?",
    "There's a DOCSOC notice board but that should be the least of your "
    "worries ",
    "This leads to the empty LAB.",
    "This stairway leads to the LECTURE HALL.",
    "The begginning of the end...",
    "The screens are red... prepare yourself for a LEXIS test ",
    "Just some dusty computers.",
    "You see a computer running Windows and the sight nearly blinds you with"
    "disgust ",
    "You see printers, shame they never work.",
    "I spot a wild konstantinos, enter attack konstantinos...",
    "The stairway to the back of the lobby.",
    "Huxley 308 is quiet... a bit too quiet.",
    "Just empty seats...",
    "There's crisp packets underneath the seats.",
    "You see the bars on the windows... reminds you why no one"
    "likes Huxley ",

    "Pot Noodles... a life support for any student. Replenish your"
    "health here ",
    "You see the sofas in the JCR. It reminds you to rest (save)",
    "There's a broken vending machine - wait for part II to use.",
    "The food on display looks wonderful... shame you can't eat it all.",
    "You see the imperial gift shop.",

    "Gucci belt on sale!",
    "You see Fredo through a window, eating pasta.",
    "You see a Supreme coat to battle the coldness.",
    "You see the ugliest top but its stat buffs are amazing! Come back when "
    "the devs have implemented a gear system ",
    "Just some more over priced clothing..."};

extern building_t *initialiseBuilding(room_t **out);
// extern void giveRoomDescriptions(room_t *rooms[], const char
// *descriptions[]);
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
extern void randomlyPlaceItems(item_t *items[], room_t *rooms[]);
extern void placeBuyableItems(item_t *items[], room_t *rooms[]);
extern void getCommand(char *com, char *arg);
extern char *reduceCommand(char *arg);
#endif
