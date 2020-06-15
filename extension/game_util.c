#include "game_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROOM_POSITION_NUMBER (5)
#define TOTAL_ITEM_COUNT (14)
#define TOTAL_APPLE_COUNT (5)
#define TOTAL_CASH_COUNT (5)
#define TOTAL_ROOM_COUNT (25)
#define CASH_ITEM_INDEX (4)
#define APPLE_ITEM_INDEX (0)
#define KEYBOARD_ITEM_INDEX (1)
#define MOUSE_ITEM_INDEX (2)
#define MONITOR_ITEM_INDEX (3)
#define PASS_ITEM_INDEX (5)
// shows player their inventory of Items
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

// randomly places items in room
void randomlyPlaceItems(Item_t *items[], room_t *rooms[]) {
  int *randomCashLocations = malloc(sizeof(int) * TOTAL_CASH_COUNT);
  int *randomAppleLocations = malloc(sizeof(int) * TOTAL_APPLE_COUNT);
  // array for random locations of apples and cash
  randomiseArray(randomAppleLocations, TOTAL_APPLE_COUNT, TOTAL_ROOM_COUNT);
  randomiseArray(randomCashLocations, TOTAL_CASH_COUNT, TOTAL_ROOM_COUNT);
  // dynamically addes apple and cash items into random rooms
  for (int i = 0; i < 5; i++) {
    rooms[randomCashLocations[i]]
        ->Items[rooms[randomCashLocations[i]]->Item_count] = items[i];
    rooms[randomAppleLocations[i]]
        ->Items[rooms[randomAppleLocations[i]]->Item_count] = items[i + 5];
  }
  // 4 IS THE NUMBER OF OTHER ITEMS APART FROM CASH & APPLES
  // added other items randomly around lobby
  int *randomOtherItemLocations = malloc(sizeof(int) * 4);
  randomiseArray(randomOtherItemLocations, 4, ROOM_POSITION_NUMBER);
  for (int i = 10; i < TOTAL_ITEM_COUNT; i++) {
    rooms[randomOtherItemLocations[i - 10]]
        ->Items[rooms[randomOtherItemLocations[i - 10]]->Item_count] = items[i];
  }
  free(randomCashLocations);
  free(randomAppleLocations);
  free(randomOtherItemLocations);
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
Item_t *initialiseItem(Item_t gameItem) {
  Item_t *item = malloc(sizeof(*item));
  item->key = gameItem.key;
  item->name = gameItem.name;
  item->properties = gameItem.properties;
  item->description = gameItem.description;
  return item;
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
  room->Item_count = 0;
  room->Items = malloc(sizeof(Item_t) * 10);
  assert(room->Items);
  return room;
}

// initialises all rooms in building
building_t *initialiseBuilding() {

  building_t *huxley = malloc(sizeof(*huxley));

  huxley->start_room = malloc(sizeof(room_t));
  assert(huxley->start_room);

  // initialise Items to put in rooms
  // initilaising 5 game apples
  Item_t *apple1, *apple2, *apple3, *apple4, *apple5;
  apple1 = apple2 = apple3 = apple4 = apple5 =
      initialiseItem(gameItems[APPLE_ITEM_INDEX]);
  // initialising 5 game cash bundles
  Item_t *cash1, *cash2, *cash3, *cash4, *cash5;
  cash1 = cash2 = cash3 = cash4 = cash5 =
      initialiseItem(gameItems[CASH_ITEM_INDEX]);
  // initialising keyboard
  Item_t *keyboard = initialiseItem(gameItems[KEYBOARD_ITEM_INDEX]);
  // initialising mouse
  Item_t *mouse = initialiseItem(gameItems[MOUSE_ITEM_INDEX]);
  // initialing monitor
  Item_t *monitor = initialiseItem(gameItems[MONITOR_ITEM_INDEX]);
  // initialising pass to lab
  Item_t *pass = initialiseItem(gameItems[PASS_ITEM_INDEX]);

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

  room_t *roomArray[TOTAL_ROOM_COUNT] = {
      lobbySouth,      lobbyEast,        lobbyWest,        lobbyNorth,
      lobbyCentre,     labEast,          labNorth,         labWest,
      labSouth,        labCentre,        fusionNorth,      fusionEast,
      fusionWest,      fusionSouth,      fusionCentre,     lectureHallEast,
      lectureHallWest, lectureHallNorth, lectureHallSouth, lectureHallCentre,
      harrodsSouth,    harrodsEast,      harrodsWest,      harrodsCentre,
      harrodsNorth

  };

  Item_t *itemArray[TOTAL_ITEM_COUNT] = {
      cash1,  cash2,  cash3,  cash4,    cash5, apple1,  apple2,
      apple3, apple4, apple5, keyboard, mouse, monitor, pass};

  randomlyPlaceItems(itemArray, roomArray);

  huxley->start_room = lobbySouth;
  //  free(itemArray);
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
        freeRoom(room1, room1->adjacent_rooms[i]);
      }
    }
    free(room1->adjacent_rooms);
  }

  if (room1->Items != NULL) {
    for (int i = 0; i < room1->Item_count; i++) {
      free(room1->Items[i]);
    }
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
