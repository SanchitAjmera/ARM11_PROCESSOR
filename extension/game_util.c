#include "game_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ROOM_POSITION_NUMBER (5)
#define TOTAL_ITEM_COUNT (19)
#define TOTAL_APPLE_COUNT (5)
#define TOTAL_BUYABLE_APPLE_COUNT (5)
#define TOTAL_SHOP_COUNT (10)
#define TOTAL_CASH_COUNT (5)
#define TOTAL_ROOM_COUNT (25)
#define CASH_ITEM_INDEX (4)
#define APPLE_ITEM_INDEX (0) // I beg you dont change the _ITEM_INDEX's
#define KEYBOARD_ITEM_INDEX (1)
#define MOUSE_ITEM_INDEX (2)
#define MONITOR_ITEM_INDEX (3)
#define PASS_ITEM_INDEX (5)
#define BUYABLE_APPLE_ITEM_INDEX (6)
// start position of buyable apples in the item array
#define BUYABLE_APPLE_START_INDEX (14)
// start position of fusion rooms in room array
#define FUSION_START_INDEX (14)

char *strptr(const char *in) {
  char *out = malloc(sizeof(char) * (strlen(in) + 1));
  strcpy(out, in);
  return out;
}

int validatePtr(const void *ptr, const char *errorMsg) {
  if (ptr == NULL) {
    printf("Error: %s\n", errorMsg);
    return -1;
  }
  return 0;
}

/* loadGameState takes in a filename and a pointer to an uninitialised state.
   If such file does not exist or cannot be opened, return -1.
   Otherwise, it writes the file's contents to the state. */
int loadGameState(const char *fname, state *playerState, room_t **worldMap) {
  assert(playerState);
  if (access(fname, F_OK) == -1) { // File does not exist
    printf("File does not exist.\n");
    return -1;
  } else {
    FILE *file = fopen(fname, "rb");
    if (validatePtr(file, "could not open this save file.") == -1) {
      return -1;
    } else {
      // TODO: add error checking to each fread to ensure that the end of the
      // file isn't reached prematurely (save file must be corrupted in this
      // case)
      fread(playerState, sizeof(state), 1, file);

      playerState->player = malloc(sizeof(player_t));
      fread(playerState->player, sizeof(player_t), 1, file);

      /* Load inventory */
      int itemCount = playerState->player->itemCount;
      playerState->player->inventory = calloc(ITEM_NUM, sizeof(item_t));
      for (int i = 0; i < itemCount; i++) {
        Item item;
        fread(&item, sizeof(Item), 1, file);
        playerState->player->inventory[i] = initialiseItem(gameItems[item]);
        int hash;
        fread(&hash, sizeof(int), 1, file);
        playerState->player->inventory[i]->hash = hash;
      }

      /* Load player's current location */
      RoomName currentRoom;
      fread(&currentRoom, sizeof(RoomName), 1, file);
      RoomPosition currentPosition;
      fread(&currentPosition, sizeof(RoomPosition), 1, file);
      playerState->curr_room_node =
          worldMap[(currentRoom * 5) + currentPosition];

      /* Load items in each room */
      for (int i = 0; i < 25; i++) { // For each room in the map
        int itemCount;
        fread(&itemCount, sizeof(int), 1, file);
        for (int j = 0; j < itemCount; j++) {
          Item item;
          fread(&item, sizeof(Item), 1, file);
          int hash;
          fread(&hash, sizeof(int), 1, file);
          worldMap[i]->Items[j] = initialiseItem(gameItems[item]);
          worldMap[i]->Items[j]->hash = hash;
        }
      }
    }
    fclose(file);
    return 0;
  }
}

/* saveGameState takes in a filename and a pointer to the player's state.
   returns -1 if the file could not be opened. Otherwise, it writes
   the player's state to the file. */
int saveGameState(const char *fname, state *playerState) {
  FILE *fileOut = fopen(fname, "wb");
  if (validatePtr(fileOut, "something went wrong mate idk wagwaan") == -1) {
    return -1;
  } else {
    // TODO: rewrite this function to save the game's contents in the format
    // that loadGameState expects.
    fwrite(playerState, sizeof(state), 1, fileOut);
    fclose(fileOut);
  }
  return 0;
}

// shows player their inventory of Items
/* Returns respective int value; -1 for failure */
item_t *lookup(item_t table[], const int size, const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i].key, key)) {
      return &table[i];
    }
  }
  return LOOKUP_FAILURE;
}

bool hasProperty(Property property, item_t *item) {
  return (property & item->properties);
}

// shows player their inventory of items
void view_inventory() {}

// shows player their character and username
void view_profile() {}

// shows player their current room
void current_room() {}

// shows player room which they can visit
// possibly a map (like the lego land map)
void display_rooms() {}

// quits game also saving game into some history file
void quit() {}

// displays introduction to player
void introduction() {}

// generates array of random numbers of length n
void randomiseArray(int randArray[], int length, int randMax) {
  for (int i = 0; i < length; i++) {
    randArray[i] = rand() % randMax;
  }
}

// randomly places Items in room
void randomlyPlaceItems(item_t *Items[], room_t *rooms[]) {
  int *randomCashLocations = malloc(sizeof(int) * TOTAL_CASH_COUNT);
  int *randomAppleLocations = malloc(sizeof(int) * TOTAL_APPLE_COUNT);
  // array for random locations of apples and cash
  randomiseArray(randomAppleLocations, TOTAL_APPLE_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  randomiseArray(randomCashLocations, TOTAL_CASH_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  // dynamically addes apple and cash Items into random rooms
  for (int i = 0; i < 5; i++) {
    rooms[randomCashLocations[i]]
        ->Items[rooms[randomCashLocations[i]]->ItemCount] = Items[i];
    rooms[randomCashLocations[i]]->ItemCount++;
    rooms[randomAppleLocations[i]]
        ->Items[rooms[randomAppleLocations[i]]->ItemCount] = Items[i + 5];
    rooms[randomAppleLocations[i]]->ItemCount++;
  }
  // 4 IS THE NUMBER OF OTHER ItemS APART FROM CASH & APPLES
  // added other Items randomly around lobby
  int *randomOtherItemLocations = malloc(sizeof(int) * 4);
  randomiseArray(randomOtherItemLocations, 4, ROOM_POSITION_NUMBER);
  for (int i = 10; i < TOTAL_ITEM_COUNT - TOTAL_SHOP_COUNT; i++) {
    rooms[randomOtherItemLocations[i - 10]]
        ->Items[rooms[randomOtherItemLocations[i - 10]]->ItemCount] = Items[i];
    rooms[randomOtherItemLocations[i - 10]]->ItemCount++;
  }
  free(randomCashLocations);
  free(randomAppleLocations);
  free(randomOtherItemLocations);
}

void placeBuyableItems(item_t *items[], room_t *rooms[]) {
  for (int i = BUYABLE_APPLE_START_INDEX; i < TOTAL_ITEM_COUNT; i++) {
    rooms[FUSION_START_INDEX]->Items[rooms[FUSION_START_INDEX]->ItemCount] =
        items[i];
    rooms[FUSION_START_INDEX]->ItemCount++;
  }
}

// connects first room to second room
void connectRoom(room_t *first, room_t *second) {
  // assigns room to each other's adjacent rooms array
  first->adjacent_rooms[first->adjacent_room_count] = second;
  second->adjacent_rooms[second->adjacent_room_count] = first;

  // increments count of adjacent rooms
  second->adjacent_room_count++;
  first->adjacent_room_count++;
}

// connects all room positions to center room position r5
void connectRoomPositions(room_t *r1, room_t *r2, room_t *r3, room_t *r4,
                          room_t *r5) {
  room_t *roomArray[ROOM_POSITION_NUMBER - 1] = {r1, r2, r3, r4};
  for (int i = 0; i < ROOM_POSITION_NUMBER - 1; i++) {
    connectRoom(roomArray[i], r5);
  }
}

// initialese Items
item_t *initialiseItem(item_t gameItem) {
  item_t *Item = malloc(sizeof(*Item));
  Item->key = strptr(gameItem.key);
  Item->name = gameItem.name;
  Item->properties = gameItem.properties;
  Item->description = strptr(gameItem.description);
  return Item;
}

// initialises room
room_t *initialiseRoom(RoomName current_room, RoomPosition initial_position) {
  // allocates memory to room and adjacent room array
  room_t *room = malloc(sizeof(*room));

  room->current_room = current_room;
  room->position = initial_position;

  room->adjacent_room_count = 0;
  room->adjacent_rooms = malloc(sizeof(room_t) * 10);
  assert(room->adjacent_rooms);
  room->ItemCount = 0;
  room->Items = malloc(sizeof(item_t) * 20);
  assert(room->Items);
  return room;
}

/* Initialises all rooms in building.
    Takes in a pointer to an array of room_t pointers, and fills this
    array with all the rooms in the building */
building_t *initialiseBuilding(room_t **out) {

  building_t *huxley = malloc(sizeof(*huxley));

  huxley->start_room = malloc(sizeof(room_t));
  assert(huxley->start_room);

  // initialise Items to put in rooms
  // initilaising 5 game apples
  item_t *apple1 = initialiseItem(gameItems[APPLE_ITEM_INDEX]);
  item_t *apple2 = initialiseItem(gameItems[APPLE_ITEM_INDEX]);
  item_t *apple3 = initialiseItem(gameItems[APPLE_ITEM_INDEX]);
  item_t *apple4 = initialiseItem(gameItems[APPLE_ITEM_INDEX]);
  item_t *apple5 = initialiseItem(gameItems[APPLE_ITEM_INDEX]);
  // initialising Buyable apples
  item_t *buyApple1 = initialiseItem(gameItems[BUYABLE_APPLE_ITEM_INDEX]);
  item_t *buyApple2 = initialiseItem(gameItems[BUYABLE_APPLE_ITEM_INDEX]);
  item_t *buyApple3 = initialiseItem(gameItems[BUYABLE_APPLE_ITEM_INDEX]);
  item_t *buyApple4 = initialiseItem(gameItems[BUYABLE_APPLE_ITEM_INDEX]);
  item_t *buyApple5 = initialiseItem(gameItems[BUYABLE_APPLE_ITEM_INDEX]);
  // initialising 5 game cash bundles
  item_t *cash1 = initialiseItem(gameItems[CASH_ITEM_INDEX]);
  item_t *cash2 = initialiseItem(gameItems[CASH_ITEM_INDEX]);
  item_t *cash3 = initialiseItem(gameItems[CASH_ITEM_INDEX]);
  item_t *cash4 = initialiseItem(gameItems[CASH_ITEM_INDEX]);
  item_t *cash5 = initialiseItem(gameItems[CASH_ITEM_INDEX]);
  // initialising keyboard
  item_t *keyboard = initialiseItem(gameItems[KEYBOARD_ITEM_INDEX]);
  // initialising mouse
  item_t *mouse = initialiseItem(gameItems[MOUSE_ITEM_INDEX]);
  // initialing monitor
  item_t *monitor = initialiseItem(gameItems[MONITOR_ITEM_INDEX]);
  // initialising pass to lab
  item_t *pass = initialiseItem(gameItems[PASS_ITEM_INDEX]);

  // initialises all the rooms within the building
  room_t *lobbySouth = initialiseRoom(LOBBY, SOUTH);
  room_t *lobbyEast = initialiseRoom(LOBBY, EAST);
  room_t *lobbyWest = initialiseRoom(LOBBY, WEST);
  room_t *lobbyNorth = initialiseRoom(LOBBY, NORTH);
  room_t *lobbyCentre = initialiseRoom(LOBBY, CENTRE);

  room_t *labSouth = initialiseRoom(LAB, SOUTH);
  room_t *labEast = initialiseRoom(LAB, EAST);
  room_t *labWest = initialiseRoom(LAB, WEST);
  room_t *labNorth = initialiseRoom(LAB, NORTH);
  room_t *labCentre = initialiseRoom(LAB, CENTRE);

  room_t *fusionSouth = initialiseRoom(FUSION, SOUTH);
  room_t *fusionEast = initialiseRoom(FUSION, EAST);
  room_t *fusionWest = initialiseRoom(FUSION, WEST);
  room_t *fusionNorth = initialiseRoom(FUSION, NORTH);
  room_t *fusionCentre = initialiseRoom(FUSION, CENTRE);

  room_t *lectureHallSouth = initialiseRoom(LECTURE_HALL, SOUTH);
  room_t *lectureHallEast = initialiseRoom(LECTURE_HALL, EAST);
  room_t *lectureHallWest = initialiseRoom(LECTURE_HALL, WEST);
  room_t *lectureHallNorth = initialiseRoom(LECTURE_HALL, NORTH);
  room_t *lectureHallCentre = initialiseRoom(LECTURE_HALL, CENTRE);

  room_t *harrodsSouth = initialiseRoom(HARRODS, SOUTH);
  room_t *harrodsEast = initialiseRoom(HARRODS, EAST);
  room_t *harrodsWest = initialiseRoom(HARRODS, WEST);
  room_t *harrodsNorth = initialiseRoom(HARRODS, NORTH);
  room_t *harrodsCentre = initialiseRoom(HARRODS, CENTRE);

  // connecting room positions with centre of rooms
  connectRoomPositions(lobbySouth, lobbyEast, lobbyWest, lobbyNorth,
                       lobbyCentre);
  connectRoomPositions(labSouth, labEast, labWest, labNorth, labCentre);
  connectRoomPositions(lectureHallSouth, lectureHallEast, lectureHallWest,
                       lectureHallNorth, lectureHallCentre);
  connectRoomPositions(fusionSouth, fusionEast, fusionWest, fusionNorth,
                       fusionCentre);
  connectRoomPositions(harrodsSouth, harrodsEast, harrodsWest, harrodsNorth,
                       harrodsCentre);

  // connecting rooms which link to each other
  connectRoom(lobbyWest, labEast);
  connectRoom(lobbyNorth, lectureHallSouth);
  connectRoom(lobbyNorth, fusionSouth);
  connectRoom(fusionNorth, harrodsSouth);

  cash1->hash = 1;
  cash2->hash = 2;
  cash3->hash = 3;
  cash4->hash = 4;
  cash5->hash = 5;
  apple1->hash = 6;
  apple2->hash = 7;
  apple3->hash = 8;
  apple4->hash = 9;
  apple5->hash = 10;
  keyboard->hash = 11;
  mouse->hash = 12;
  monitor->hash = 13;
  pass->hash = 14;
  buyApple1->hash = 15;
  buyApple2->hash = 16;
  buyApple3->hash = 17;
  buyApple4->hash = 18;
  buyApple5->hash = 19;

  // Keep it in this order I beg
  room_t *roomArray[TOTAL_ROOM_COUNT] = {
      lobbyEast,        lobbyWest,        lobbyNorth,        lobbySouth,
      lobbyCentre,      labEast,          labWest,           labNorth,
      labSouth,         labCentre,        lectureHallEast,   lectureHallWest,
      lectureHallNorth, lectureHallSouth, lectureHallCentre, fusionEast,
      fusionWest,       fusionNorth,      fusionSouth,       fusionCentre,
      harrodsEast,      harrodsWest,      harrodsNorth,      harrodsSouth,
      harrodsCentre};
  for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
    out[i] = roomArray[i];
  }

  item_t *itemArray[TOTAL_ITEM_COUNT] = {
      cash1,     cash2,     cash3,     cash4,     cash5,    apple1,  apple2,
      apple3,    apple4,    apple5,    keyboard,  mouse,    monitor, pass,
      buyApple1, buyApple2, buyApple3, buyApple4, buyApple5};

  randomlyPlaceItems(itemArray, roomArray);

  placeBuyableItems(itemArray, roomArray);

  huxley->start_room = lobbySouth;
  //  free(roomArray);
  return huxley;
}

// frees room
void freeRoom(room_t *entranceRoom, room_t *room1) {
  if (room1 == NULL) {
    return;
  }

  if (room1->adjacent_rooms != NULL) {
    for (int i = 0; i < room1->adjacent_room_count; i++) {
      // check to stop function from repeatedly freeing the same rooms
      if (room1->adjacent_rooms[i] != entranceRoom) {
        freeRoom(entranceRoom, room1->adjacent_rooms[i]);
      }
    }
    free(room1->adjacent_rooms);
  }

  if (room1->Items != NULL) {
    for (int i = 0; i < room1->ItemCount; i++) {
      free(room1->Items[i]);
    }
    free(room1->Items);
  }

  free(room1);
}

// frees all the rooms in the building
void freeBuilding(building_t *huxley) {

  if (huxley == NULL) {
    return;
  }
  // calling recursive function to traverse through and free all the rooms
  freeRoom(NULL, huxley->start_room);
  free(huxley);
}
// changes room of person and pushes current room into room histroy of player
void changeRoom(state *person, room_t dest_room) {}

//
//
//
//
//
//
//
// --------------------------------------------------------------------------
// ----------- LUQMANS IMPLEMENTATION OF PLAYER FUNCTIONS--------------------
//
//
//
//
//
//
//
// need ptr checks
player_t *initialisePlayer() {
  player_t *newPlayer = malloc(sizeof(*newPlayer));
  newPlayer->inventory = calloc(ITEM_NUM, sizeof(item_t));
  newPlayer->health = MAX_HEALTH;
  newPlayer->cash = INITIAL_CASH;
  newPlayer->itemCount = 0;
  assert(newPlayer && newPlayer->inventory);
  return newPlayer;
}

state *initialiseState(room_t *initialRoom) {
  state *initialState = malloc(sizeof(*initialState));
  initialState->player = initialisePlayer();
  initialState->curr_room_node = initialRoom;
  char *username = "sanchit";
  // scanf("%s\n", username);
  initialState->profile.username = username;
  initialState->profile.character = UTA;
  initialState->profile.score = 0;

  return initialState;
}

void freePlayer(player_t *player) {
  // TODO: add a freeItem function
  if (player->inventory != NULL) {
    for (int i = 0; i < player->itemCount; i++) {
      free(player->inventory[i]);
    }
    free(player->inventory);
  }
  free(player);
}

void freeState(state *state1) {
  freePlayer(state1->player);
  free(state1);
}

//--------------------------Start of Prints-----------------------------------
/* Prints To be moved into print_utils maybe */
//--------------------------End of Prints--------------------------------

// returns index of Item if it is in the list. TODO: remove magic numbers
bool buyItem(state *currentState, char *ItemName) { return false; }
