#include "print_util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function for printing out name of room
void printRoomName(room_t *room1) {

  switch (room1->current_room) {
  case LOBBY:
    printf("LOBBY\n");
    break;
  case LAB:
    printf("LAB\n");
    break;
  case HARRODS:
    printf("HARRODS\n");
    break;
  case FUSION:
    printf("FUSION\n");
    break;
  case LECTURE_HALL:
    printf("LECTURE_HALL\n");
    break;
  }
}

// function for printing out names of adjacent rooms
void printAdjacentRooms(room_t *room1) {
  for (int i = 0; i < room1->adjacent_room_count; i++) {
    printRoomName(room1->adjacent_rooms[i]);
  }
}

// function purely for testing ouptut of connect room_name
void printRoomDetails(room_t *room1) {
  printRoomName(room1);
  printf("%d\n", room1->adjacent_room_count);
  printAdjacentRooms(room1);
}

bool containsRoom(room_t *printedRooms[4], int size, room_t *room) {
  for (int i = 0; i < size; i++) {
    if (room->current_room == printedRooms[i]->current_room) {
      return true;
    }
  }
  return false;
}

void printBuildingDetails(building_t *huxley) {
  room_t *printedRooms[ROOM_COUNT];
  int size = 1;
  room_t *room = huxley->start_room;
  printedRooms[0] = room;
  While(size < ROOM_COUNT) {
    for (int i = 0; i < room->adjacent_room_count; i++) {
      if (!containsRoom(printedRooms, size, room->adjacent_rooms[i])) {
        printedRooms[size] = (room->adjacent_rooms[i]);
        size++;
      }
    }
  }

  for (int i = 0; i < ROOM_COUNT; i++) {
    printRoomDetails(printedRooms[i]);
    printf("\n");
  }
}
