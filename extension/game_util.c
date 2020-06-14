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

// initialises room
room_t *initialiseRoom(room_name current_room) {
  room_t *room = malloc(sizeof(*room));
  room->current_room = current_room;
  room->adjacent_rooms = malloc(sizeof(room_t) * 10);
  room->adjacent_room_count = 0;
  room->item_count = 0;
  room->items = malloc(sizeof(item_t) * 10);
  return room;
}

building_t *initialiseBuilding() {
  building_t *huxley = malloc(sizeof(*huxley));
  huxley->start_room = malloc(sizeof(room_t));

  room_t *lobby = initialiseRoom(LOBBY);
  room_t *lab = initialiseRoom(LAB);
  room_t *fusion = initialiseRoom(FUSION);
  room_t *lectureHall = initialiseRoom(LECTURE_HALL);

  connectRoom(lobby, lab);
  connectRoom(lobby, lectureHall);
  connectRoom(lobby, fusion);

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
