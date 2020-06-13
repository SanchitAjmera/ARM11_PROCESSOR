#include "util.h"
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

// initialises room
room *initialiseRoom(room_name current_room) {
  room *this_room = malloc(sizeof(*this_room));
  this_room->current_room = current_room;
  this_room->adjacent_rooms = malloc(sizeof(*this_room->adjacent_rooms));
  this_room->adjacent_room_count = 0;
  return this_room;
}

// connects first room to second room
void connectRoom(room *first, room *second) {
  first->adjacent_rooms[first->adjacent_room_count] = second;
  first->adjacent_room_count++;
}

// function purely for testing ouptut of connect room_name
void printRoom(room_name name) {
  switch (name) {
  case LOBBY:
    printf("LOBBY\n");
  case LAB:
    printf("LAB\n");
  case HARRODS:
    printf("HARRODS\n");
  case FUSION:
    printf("FUSION\n");
  case LECTURE_HALL:
    printf("LECTURE_HALL\n");
  }
}

void freeRoom(room *room1) {
  if (room1 == NULL) {
    return;
  }
  if (room1->adjacent_rooms == NULL) {
    free(room1);
  }
  for (int i = 0; i < room1->adjacent_room_count; i++) {
    freeRoom((room1->adjacent_rooms[i]));
  }
  free(room1);
}
// changes room of person and pushes current room into room histroy of player
void changeRoom(state *person, room dest_room) {}
