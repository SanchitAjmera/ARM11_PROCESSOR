#include "game_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  first->adjacent_rooms[first->adjacent_room_count] = second;
  second->adjacent_rooms[second->adjacent_room_count] = first;
  second->adjacent_room_count++;
  first->adjacent_room_count++;
}

// connects all position in room
void connectRoomPositions(room_t *r1, room_t *r2, room_t *r3, room_t *r4) {
  room_t *rooms[4] = {r1, r2, r3, r4};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i != j) {
        connectRoom(rooms[i], rooms[j]);
      }
    }
  }
}

// initialises room
room_t *initialiseRoom(room_name current_room, room_position position) {
  room_t *room = malloc(sizeof(*room));
  room->position = position;
  room->current_room = current_room;
  room->adjacent_rooms = malloc(sizeof(room_t) * 10);
  room->adjacent_room_count = 0;
  room->items = malloc(sizeof(item_t) * 20);
  room->desciptions = malloc(sizeof(char) * 100);
  room->visits = 0;
  return room;
}

building_t *initialiseBuilding() {
  building_t *huxley = malloc(sizeof(*huxley));
  huxley->start_room = malloc(sizeof(room_t));

  room_t *lobbyEast = initialiseRoom(LOBBY, EAST);
  room_t *lobbyWest = initialiseRoom(LOBBY, WEST);
  room_t *lobbyNorth = initialiseRoom(LOBBY, NORTH);
  room_t *lobbySouth = initialiseRoom(LOBBY, SOUTH);

  room_t *lectureHallEast = initialiseRoom(LECTURE_HALL, EAST);
  room_t *lectureHallWest = initialiseRoom(LECTURE_HALL, WEST);
  room_t *lectureHallNorth = initialiseRoom(LECTURE_HALL, NORTH);
  room_t *lectureHallSouth = initialiseRoom(LECTURE_HALL, SOUTH);

  room_t *fusionEast = initialiseRoom(FUSION, EAST);
  room_t *fusionWest = initialiseRoom(FUSION, WEST);
  room_t *fusionNorth = initialiseRoom(FUSION, NORTH);
  room_t *fusionSouth = initialiseRoom(FUSION, SOUTH);

  room_t *labEast = initialiseRoom(LAB, EAST);
  room_t *labWest = initialiseRoom(LAB, WEST);
  room_t *labNorth = initialiseRoom(LAB, NORTH);
  room_t *labSouth = initialiseRoom(LAB, SOUTH);

  connectRoomPositions(lobbyEast, lobbyWest, lobbySouth, lobbyNorth);
  connectRoomPositions(lectureHallEast, lectureHallWest, lectureHallSouth,
                       lectureHallSouth);
  connectRoomPositions(fusionWest, fusionEast, fusionNorth, fusionSouth);
  connectRoomPositions(labEast, labWest, labSouth, labNorth);

  connectRoom(lobbyWest, labEast);
  connectRoom(lobbyNorth, lectureHallSouth);
  connectRoom(lobbyNorth, fusionSouth);

  huxley->start_room = lobby;

  return huxley;
}

void freeRoom(room_t *entranceRoom, room_t *room1) {

  if (room1 == NULL) {
    return;
  }
  if (room1->adjacent_rooms != NULL) {
    for (int i = 0; i < room1->adjacent_room_count; i++) {
      if (room1->adjacent_rooms[i] != entranceRoom) {
        freeRoom(room1, room1->adjacent_rooms[i]);
      }
    }
    free(room1->adjacent_rooms);
  }
  free(room1);
}

void freeBuilding(building_t *huxley) {

  if (huxley == NULL) {
    return;
  }
  freeRoom(NULL, huxley->start_room);
  free(huxley);
}
// changes room of person and pushes current room into room histroy of player
void changeRoom(state *person, room_t dest_room) {}
