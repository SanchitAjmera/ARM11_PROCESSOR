#include "game_util.h"
#include "../src/common/util.h"
#include "characters/boss/boss.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ROOM_POSITION_NUMBER (5)
#define TOTAL_ITEM_COUNT (38)
#define TOTAL_BUYABLE_ITEM_COUNT (24)
#define TOTAL_PEAR_COUNT (5)
#define TOTAL_BUYABLE_APPLE_COUNT (5)
#define TOTAL_SHOP_COUNT (10)
#define TOTAL_CASH_COUNT (5)
#define CASH_ITEM_INDEX (4)
#define PEAR_ITEM_INDEX (0) // I beg you dont change the _ITEM_INDEX's
#define KEYBOARD_ITEM_INDEX (1)
#define MOUSE_ITEM_INDEX (2)
#define MONITOR_ITEM_INDEX (3)
#define PASS_ITEM_INDEX (5)
#define BUYABLE_APPLE_ITEM_INDEX (6)
#define COFFEE_ITEM_INDEX (7)
#define TESCOMD_ITEM_INDEX (8)
#define RUM_ITEM_INDEX (9)
#define BUYABLE_ITEMS_IN_ROOM (5)
#define FUSION_NORTH_INDEX (15)
#define FUSION_WEST_INDEX (16)
#define FUSION_SOUTH_INDEX (17)
#define FUSION_EAST_INDEX (18)
#define HARRODS_START_INDEX (21)
#define BOSSKGK ("konstantinos")
#define BOSSTONY ("tony")

/*
char *strptr(const char *in) {
  char *out = malloc(sizeof(char) * (strlen(in) + 1));
  checkPtr(out);
  strcpy(out, in);
  return out;
} */

// converts a string to lowercase
void lowercase(char *in) {
  if (in) {
    for (char *string = in; *string; string++) {
      *string = tolower(*string);
    }
  }
}

/* Checks for failed memory allocation */
void checkPtr(const void *ptr) {
  if (ptr == NULL) {
    printf("A memory error occurred. The game must end!");
    quit();
  }
}

/*
int validatePtr(const void *ptr, char *message) {
  if (ptr == NULL) {
    printf("Error: %s", message);
    return -1;
  }
  return 0;
} */

/* loadGameState takes in a filename, a pointer to an uninitialised state,
   and an array of pointers to every room in the game.
   If such file does not exist or cannot be opened, return -1.
   Otherwise, it writes the file's contents to the state. */
int loadGameState(const char *fname, state *playerState, room_t **worldMap) {
  assert(playerState);
  if (access(fname, F_OK) == -1) { // File does not exist
    printf("File does not exist.\n");
    return -1;
  } else {
    FILE *file = fopen(fname, "rb");
    if (!file) {
      return -1;
    } else {
      // TODO: add error checking to each fread to ensure that the end of the
      // file isn't reached prematurely (save file must be corrupted in this
      // case)
      fread(playerState, sizeof(state), 1, file);

      playerState->player = malloc(sizeof(player_t));
      fread(playerState->player, sizeof(player_t), 1, file);

      /* Load player's username */
      playerState->profile.username =
          malloc(sizeof(char) * USERNAME_CHAR_LIMIT);
      fread(playerState->profile.username, sizeof(char), USERNAME_CHAR_LIMIT,
            file);

      /* Load inventory */
      int itemCount = playerState->player->itemCount;
      playerState->player->inventory = calloc(ITEM_NUM, sizeof(item_t));
      for (int i = 0; i < itemCount; i++) {
        ItemIndex gameItemID;
        fread(&gameItemID, sizeof(ItemIndex), 1, file);
        playerState->player->inventory[i] = initialiseItem(gameItemID);
        int hash;
        fread(&hash, sizeof(int), 1, file);
        playerState->player->inventory[i]->hash = hash;
      }

      /* Load player's current location */
      RoomName currentRoom;
      fread(&currentRoom, sizeof(RoomName), 1, file);
      RoomPosition currentPosition;
      fread(&currentPosition, sizeof(RoomPosition), 1, file);
      playerState->currentRoom = worldMap[(currentRoom * 5) + currentPosition];

      /* Load items in each room */
      for (int i = 0; i < TOTAL_ROOM_COUNT; i++) { // For each room in the map
        int itemCount;
        fread(&itemCount, sizeof(int), 1, file);
        worldMap[i]->itemCount = itemCount;
        for (int j = 0; j < itemCount; j++) {
          ItemIndex gameItemID;
          fread(&gameItemID, sizeof(ItemIndex), 1, file);
          int hash;
          fread(&hash, sizeof(int), 1, file);
          worldMap[i]->items[j] = initialiseItem(gameItemID);
          worldMap[i]->items[j]->hash = hash;
        }
      }
    }
    fclose(file);
    return 0;
  }
}

/* saveGameState takes in a filename, a pointer to the player's state,
   and an array of pointers to every room in the game.
   Returns -1 if the file could not be opened. Otherwise, it writes
   the player's state to the file. */
int saveGameState(const char *fname, state *playerState, room_t **worldMap) {
  FILE *fileOut = fopen(fname, "wb");
  if (!fileOut) {
    printf("Something went wrong with the save g, idk wagwaan.");
    return -1;
  } else {
    // Save state
    fwrite(playerState, sizeof(state), 1, fileOut);
    // Save player struct
    fwrite(playerState->player, sizeof(player_t), 1, fileOut);
    // Save username
    fwrite(playerState->profile.username, sizeof(char), USERNAME_CHAR_LIMIT,
           fileOut);
    // Save each item in player's inventory
    for (int i = 0; i < playerState->player->itemCount; i++) {
      item_t *item = playerState->player->inventory[i];
      fwrite(&item->gameItemID, sizeof(ItemIndex), 1, fileOut);
      fwrite(&item->hash, sizeof(int), 1, fileOut);
    }
    // Save player's location
    room_t *currentRoom = playerState->currentRoom;
    fwrite(&currentRoom->current_room, sizeof(RoomName), 1, fileOut);
    fwrite(&currentRoom->position, sizeof(RoomPosition), 1, fileOut);
    // Save items in each room
    for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
      room_t *room = worldMap[i];
      fwrite(&room->itemCount, sizeof(int), 1, fileOut);
      for (int j = 0; j < room->itemCount; j++) {
        item_t *item = room->items[j];
        fwrite(&item->gameItemID, sizeof(ItemIndex), 1, fileOut);
        fwrite(&item->hash, sizeof(int), 1, fileOut);
      }
    }
    fclose(fileOut);
  }
  return 0;
}

char *reduceCommand(char *argument) {
  if (IS_EMPTY(argument)) {
    return argument;
  }
  char *reduced = malloc(sizeof(argument));
  strcpy(reduced, argument);
  reduced = strtok(reduced, " ");
  return reduced;
}

void getCommand(char *command, char *argument) {
  char input[30];
  printf("         >> ");
  fgets(input, sizeof(input), stdin);

  // obtain user input
  char *comm = strtok(input, " ");
  char *args = strtok(NULL, "\n");

  // remove trailing new line characters
  comm = strtok(comm, "\n");

  // Make both into lowercase
  lowercase(comm);
  lowercase(args);

  // Copy results into given pointers
  if (!comm) {
    strcpy(command, "skip");
    return;
  }
  strcpy(command, comm);

  if (!args) {
    strcpy(argument, "");
    return;
  }
  while (isspace(*args)) {
    args++;
  }
  strcpy(argument, args);
}

//--------------Possible const error---------------------
// shows player their inventory of Items
/* Returns respective int value; -1 for failure */
const item_t *itemLookup(const item_t table[], const int size,
                         const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i].key, key)) {
      return table + i;
    }
  }
  return LOOKUP_FAILURE;
}
/*
// general lookup
int lookup(const pair_t table[], const int size, const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i].key, key)) {
      return table[i].value;
    }
  }
  return -1;
} */

// Checks if an item has a certain property
bool hasProperty(Property property, const item_t *item) {
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

// quits game - TODO: free all resources in this function;
void quit() {
  printf("Thanks for playing!\n");
  exit(EXIT_SUCCESS);
}

// displays introduction to player
void introduction() {}

// generates array of random numbers of length n
void randomiseArray(int randArray[], int length, int randMax) {
  srand(time(NULL));
  for (int i = 0; i < length; i++) {
    randArray[i] = rand() % randMax;
  }
}

// randomly places Items in room
void randomlyPlaceItems(item_t *items[], room_t *rooms[]) {
  int *randomCashLocations = malloc(sizeof(int) * TOTAL_CASH_COUNT);
  int *randomPearLocations = malloc(sizeof(int) * TOTAL_PEAR_COUNT);
  checkPtr(randomCashLocations);
  checkPtr(randomPearLocations);
  // array for random locations of apples and cash
  randomiseArray(randomPearLocations, TOTAL_PEAR_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  randomiseArray(randomCashLocations, TOTAL_CASH_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  // dynamically addes apple and cash Items into random rooms
  for (int i = 0; i < 5; i++) {
    rooms[randomCashLocations[i]]
        ->items[rooms[randomCashLocations[i]]->itemCount] = items[i];
    rooms[randomCashLocations[i]]->itemCount++;
    rooms[randomPearLocations[i]]
        ->items[rooms[randomPearLocations[i]]->itemCount] = items[i + 5];
    rooms[randomPearLocations[i]]->itemCount++;
  }
  // 4 IS THE NUMBER OF OTHER ItemS APART FROM CASH & APPLES
  // added other Items randomly around lobby
  int *randomOtherItemLocations = malloc(sizeof(int) * 4);
  checkPtr(randomOtherItemLocations);
  randomiseArray(randomOtherItemLocations, 4, ROOM_POSITION_NUMBER);
  for (int i = 10; i < TOTAL_ITEM_COUNT - TOTAL_BUYABLE_ITEM_COUNT; i++) {
    rooms[randomOtherItemLocations[i - 10]]
        ->items[rooms[randomOtherItemLocations[i - 10]]->itemCount] = items[i];
    rooms[randomOtherItemLocations[i - 10]]->itemCount++;
  }
  free(randomCashLocations);
  free(randomPearLocations);
  free(randomOtherItemLocations);
}

void placeBuyableItems(item_t *items[], room_t *rooms[]) {
  for (int i = 0; i < BUYABLE_ITEMS_IN_ROOM; i++) {
    rooms[FUSION_WEST_INDEX]->items[rooms[FUSION_WEST_INDEX]->itemCount] =
        items[i];
    rooms[FUSION_WEST_INDEX]->itemCount++;

    rooms[FUSION_EAST_INDEX]->items[rooms[FUSION_EAST_INDEX]->itemCount] =
        items[5 + i];
    rooms[FUSION_EAST_INDEX]->itemCount++;

    rooms[FUSION_SOUTH_INDEX]->items[rooms[FUSION_SOUTH_INDEX]->itemCount] =
        items[10 + i];
    rooms[FUSION_SOUTH_INDEX]->itemCount++;

    rooms[FUSION_NORTH_INDEX]->items[rooms[FUSION_NORTH_INDEX]->itemCount] =
        items[15 + i];
    rooms[FUSION_NORTH_INDEX]->itemCount++;
  }
  for (int i = 20; i < TOTAL_ROOM_COUNT - 1; i++) {
    rooms[i]->items[0] = items[i];
    rooms[i]->itemCount++;
  }
}

// connects first room to second room
void connectRoom(room_t *first, room_t *second) {
  switch (first->position) {
  case CENTRE:
    first->adjacent_rooms[second->position] = second;
    second->adjacent_rooms[((second->position) + 2) % 4] = first;
    break;
  // first: north lobby second south fusion
  default:
    first->adjacent_rooms[first->position] = second;
    second->adjacent_rooms[second->position] = first;
    break;
  }
  second->adjacent_room_count++;
  first->adjacent_room_count++;
}

// connects all room positions to center room position r5
void connectRoomPositions(room_t *r1, room_t *r2, room_t *r3, room_t *r4,
                          room_t *r5) {
  room_t *roomArray[ROOM_POSITION_NUMBER - 1] = {r1, r2, r3, r4};
  for (int i = 0; i < ROOM_POSITION_NUMBER - 1; i++) {
    connectRoom(r5, roomArray[i]);
  }
}

// initialese Items
item_t *initialiseItem(ItemIndex gameItemID) {
  item_t *item = malloc(sizeof(*item));
  checkPtr(item);
  item_t gameItem = gameItems[gameItemID];
  item->key = strptr(gameItem.key);
  item->name = gameItem.name;
  item->gameItemID = gameItemID;
  item->properties = gameItem.properties;
  item->description = strptr(gameItem.description);
  item->cost = gameItem.cost;
  if (gameItemID == CASH_INDEX) {
    item->cost = rand() % 1000;
  }
  return item;
}

void initialiseBuyableItem(item_t *items[]) {
  item_t *buyApple1 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple2 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple3 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple4 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple5 = initialiseItem(BUYAPPLE_INDEX);

  item_t *coffee1 = initialiseItem(COFFEE_INDEX);
  item_t *coffee2 = initialiseItem(COFFEE_INDEX);
  item_t *coffee3 = initialiseItem(COFFEE_INDEX);
  item_t *coffee4 = initialiseItem(COFFEE_INDEX);
  item_t *coffee5 = initialiseItem(COFFEE_INDEX);

  item_t *tescoMD1 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD2 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD3 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD4 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD5 = initialiseItem(TESCO_INDEX);

  item_t *rum1 = initialiseItem(RUM_INDEX);
  item_t *rum2 = initialiseItem(RUM_INDEX);
  item_t *rum3 = initialiseItem(RUM_INDEX);
  item_t *rum4 = initialiseItem(RUM_INDEX);
  item_t *rum5 = initialiseItem(RUM_INDEX);

  item_t *guzziBelt = initialiseItem(GUZZI_INDEX);
  item_t *supreme = initialiseItem(SUPREME_INDEX);
  item_t *drip = initialiseItem(DRIP_INDEX);
  item_t *blm = initialiseItem(BLM_INDEX);

  guzziBelt->hash = 100;
  supreme->hash = 101;
  drip->hash = 102;
  blm->hash = 103;

  items[0] = buyApple1;
  items[1] = buyApple2;
  items[2] = buyApple3;
  items[3] = buyApple4;
  items[4] = buyApple5;
  items[5] = coffee1;
  items[6] = coffee2;
  items[7] = coffee3;
  items[8] = coffee4;
  items[9] = coffee5;
  items[10] = tescoMD1;
  items[11] = tescoMD2;
  items[12] = tescoMD3;
  items[13] = tescoMD4;
  items[14] = tescoMD5;
  items[15] = rum1;
  items[16] = rum2;
  items[17] = rum3;
  items[18] = rum4;
  items[19] = rum5;
  items[20] = guzziBelt;
  items[21] = supreme;
  items[22] = drip;
  items[23] = blm;
}

// initialises room
room_t *initialiseRoom(RoomName current_room, RoomPosition initial_position) {
  // allocates memory to room and adjacent room array
  room_t *room = malloc(sizeof(*room));
  checkPtr(room);

  room->current_room = current_room;
  room->position = initial_position;

  room->adjacent_room_count = 0;
  room->adjacent_rooms = malloc(sizeof(room_t) * 10);
  checkPtr(room->adjacent_rooms);
  room->itemCount = 0;
  room->items = malloc(sizeof(item_t) * 20);
  checkPtr(room->items);
  room->boss = NULL;
  return room;
}

/* Initialises all rooms in building.
    Takes in a pointer to an array of room_t pointers, and fills this
    array with all the rooms in the building */
building_t *initialiseBuilding(room_t **out) {

  building_t *huxley = malloc(sizeof(*huxley));
  checkPtr(huxley);

  huxley->startRoom = malloc(sizeof(room_t));
  checkPtr(huxley->startRoom);

  // initialise Items to put in rooms
  // initilaising 5 game apples
  item_t *pear1 = initialiseItem(PEAR_INDEX);
  item_t *pear2 = initialiseItem(PEAR_INDEX);
  item_t *pear3 = initialiseItem(PEAR_INDEX);
  item_t *pear4 = initialiseItem(PEAR_INDEX);
  item_t *pear5 = initialiseItem(PEAR_INDEX);
  // initialising Buyable items
  item_t *buyableItemArray[TOTAL_BUYABLE_ITEM_COUNT];
  initialiseBuyableItem(buyableItemArray);
  // initialising 5 game cash bundles
  item_t *cash1 = initialiseItem(CASH_INDEX);
  item_t *cash2 = initialiseItem(CASH_INDEX);
  item_t *cash3 = initialiseItem(CASH_INDEX);
  item_t *cash4 = initialiseItem(CASH_INDEX);
  item_t *cash5 = initialiseItem(CASH_INDEX);
  // initialising keyboard
  item_t *keyboard = initialiseItem(KEYBOARD_INDEX);
  // initialising mouse
  item_t *mouse = initialiseItem(MOUSE_INDEX);
  // initialing monitor
  item_t *monitor = initialiseItem(MONITOR_INDEX);
  // initialising pass to lab
  item_t *pass = initialiseItem(PASS_INDEX);

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
  connectRoom(lobbyEast, fusionWest);
  connectRoom(fusionNorth, harrodsSouth);

  // initialising Bosses
  boss_t *kgk = createBoss(BOSSKGK);
  boss_t *tony = createBoss(BOSSTONY);

  // adding bosses to Rooms
  labCentre->boss = kgk;
  lectureHallCentre->boss = tony;
  // Keep it in this order I beg
  room_t *roomArray[TOTAL_ROOM_COUNT] = {
      lobbySouth,       lobbyEast,       lobbyNorth,        lobbyWest,
      lobbyCentre,      labSouth,        labEast,           labNorth,
      labWest,          labCentre,       lectureHallSouth,  lectureHallEast,
      lectureHallNorth, lectureHallWest, lectureHallCentre, fusionSouth,
      fusionEast,       fusionNorth,     fusionWest,        fusionCentre,
      harrodsSouth,     harrodsEast,     harrodsNorth,      harrodsWest,
      harrodsCentre};
  for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
    out[i] = roomArray[i];
  }

  item_t *itemArray[TOTAL_ITEM_COUNT - TOTAL_BUYABLE_ITEM_COUNT] = {
      cash1, cash2, cash3, cash4,    cash5, pear1,   pear2,
      pear3, pear4, pear5, keyboard, mouse, monitor, pass};

  randomlyPlaceItems(itemArray, roomArray);

  placeBuyableItems(buyableItemArray, roomArray);

  huxley->startRoom = lobbySouth;

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
        freeRoom(room1, room1->adjacent_rooms[i]);
      }
    }
    free(room1->adjacent_rooms);
  }

  if (room1->items != NULL) {
    for (int i = 0; i < room1->itemCount; i++) {
      free(room1->items[i]);
    }
    free(room1->items);
  }

  free(room1);
}

// frees all the rooms in the building
void freeBuilding(building_t *huxley) {

  if (huxley == NULL) {
    return;
  }
  // calling recursive function to traverse through and free all the rooms
  freeRoom(NULL, huxley->startRoom);
  free(huxley);
}

state *initialiseState(room_t *initialRoom) {
  state *initialState = malloc(sizeof(*initialState));
  checkPtr(initialState);
  initialState->player = initialisePlayer();
  initialState->currentRoom = initialRoom;
  char *username = malloc(sizeof(char) * USERNAME_CHAR_LIMIT);
  strcpy(username, "sanchit");
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
