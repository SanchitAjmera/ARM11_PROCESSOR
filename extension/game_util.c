#include "game_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROOM_POSITION_NUMBER (5)
#define TOTAL_ITEM_COUNT (10)
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

// initialises room
room_t *initialiseRoom(room_name current_room, room_position initial_position) {
  // allocates memory to room and adjacent room array
  room_t *room = malloc(sizeof(*room));

  room->current_room = current_room;
  room->position = initial_position;

  room->adjacent_room_count = 0;
  room->adjacent_rooms = malloc(sizeof(room_t) * 10);
  assert(room->adjacent_rooms);
  room->item_count = 0;
  room->items = malloc(sizeof(item_t) * 10);
  assert(room->items);
  return room;
}

// initialises all rooms in building
building_t *initialiseBuilding() {

  building_t *huxley = malloc(sizeof(*huxley));

  huxley->start_room = malloc(sizeof(room_t));
  assert(huxley->start_room);

  // initialise items to put in rooms
  // initilaising 5 game apples
  item_t apple1 = gameItems[0];
  item_t apple2 = gameItems[0];
  item_t apple3 = gameItems[0];
  item_t apple4 = gameItems[0];
  item_t apple5 = gameItems[0];
  // initialising 5 game cash bundles
  item_t cash1 = gameItem[4];
  item_t cash1 = gameItem[4];
  item_t cash1 = gameItem[4];
  item_t cash1 = gameItem[4];
  item_t cash1 = gameItem[4];
  // initialising keyboard
  item_t keyboard = gameItem[1];
  // initialising mouse
  item_t mouse = gameItem[2];
  // initialing monitor
  item_t monitor = gameItem[3];
  // initialising pass to lab
  item_t pass = gameItem[5];
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

  huxley->start_room = lobbySouth;

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
    for (int i = 0; i < room1->item_count; i++) {
      free(room1->items[i]);
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
