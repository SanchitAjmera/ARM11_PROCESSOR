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
room initialiseRoom(room_name current_room) {
  struct room *this_room = malloc(sizeof(room));
  this_room->current_room = current_room;
  this_room->adjacent_rooms = NULL;
}

// connects first room to second room
void connectRoom(room *first, room *second) {
  room *adjacent_rooms = first->adjacent_rooms;
}

// changes room of person and pushes current room into room histroy of player
void changeRoom(state *person, room dest_room) {
  room current_room = person->current_room;
}

int main(void) { return 1; }
