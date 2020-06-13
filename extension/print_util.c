#include "util.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function for printing out name of room
void printRoomName(room *room1) {

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
void printAdjacentRooms(room *room1) {
  for (int i = 0; i < room1->adjacent_room_count; i++) {
    printRoomName(room1->adjacent_rooms[i]);
  }
}

// function purely for testing ouptut of connect room_name
void printRoomDetails(room *room1) {
  printRoomName(room1);
  printf("%d\n", room1->adjacent_room_count);
  printAdjacentRooms(room1);
}

void printBuildingDetails(building huxley) { room_name *printedRooms; }
