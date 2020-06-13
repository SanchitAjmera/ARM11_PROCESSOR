#include "util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// shows player their inventory of items
void view_inventory(){};

// shows player their character and username
void view_profile(){};

// shows player their current room
void current_room(){};

// shows player room which they can visit
// possibly a map (like the lego land map)
void display_rooms(){};

// quits game also saving game into some history file
void quit(){};

// displays introduction to player
void introduction(){};

// connects first room to second room
void connectRoom(room *first, room *second) {
  first->adjacent_rooms[first->adjacent_room_count] = second;
  // second->adjacent_rooms[second->adjacent_room_count] = first;
  // second->adjacent_room_count++;
  first->adjacent_room_count++;
}

// initialises room
room *initialiseRoom(room_name current_room) {
  room *this_room = malloc(sizeof(*this_room));
  assert(this_room);
  this_room->current_room = current_room;
  this_room->adjacent_rooms = malloc(sizeof(room) * 5);
  assert(this_room->adjacent_rooms);
  this_room->adjacent_room_count = 0;

  return this_room;
}

building *initialiseBuilding() {
  building *huxley = malloc(sizeof(*huxley));
  huxley->start_room = malloc(sizeof(room));
  room *lobby = initialiseRoom(LOBBY);
  room *lecture_hall = initialiseRoom(LECTURE_HALL);
  room *fusion = initialiseRoom(FUSION);
  room *lab = initialiseRoom(LAB);

  connectRoom(lobby, lecture_hall);
  connectRoom(lobby, lab);
  connectRoom(lobby, fusion);

  huxley->start_room = lobby;

  return huxley;
}

void freeRoom(room *room1) {

  if (room1 == NULL) {
    return;
  }
  if (room1->adjacent_rooms != NULL) {
    for (int i = 0; i < room1->adjacent_room_count; i++) {
      freeRoom((room1->adjacent_rooms[i]));
    }
    free(room1->adjacent_rooms);
  }
  free(room1);
}

void freeBuilding(building *huxley) {

  if (huxley == NULL) {
    return;
  }
  freeRoom(huxley->start_room);
  free(huxley);
}
// changes room of person and pushes current room into room histroy of player
void changeRoom(state *person, room dest_room) {}
