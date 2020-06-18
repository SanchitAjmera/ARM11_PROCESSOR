#include "game_util.h"
#include "print_util.h"
#include <stdio.h>

// traverses through all rooms and applies function pointer to each room
void roomTraverser(room_t *entranceRoom, room_t *room1,
                   void (*funcRooms)(room_t *)) {
  if (IS_NULL(room1)) {
    return;
  }
  if (!IS_NULL(room1->adjacent_rooms)) {
    for (int i = 0; i < 5; i++) {
      if (room1->adjacent_rooms[i]) {
        if (room1->adjacent_rooms[i] != entranceRoom) {
          roomTraverser(room1, room1->adjacent_rooms[i], funcRooms);
        }
      }
    }
  }
  (*funcRooms)(room1);
  printf("\n");
}

// funcion to print Room position
void printRoomPosition(room_t *room) {
  switch (room->position) {
  case NORTH:
    printf("North ");
    break;
  case SOUTH:
    printf("South ");
    break;
  case WEST:
    printf("West ");
    break;
  case EAST:
    printf("East ");
    break;
  case CENTRE:
    printf("Central ");
    break;
  }
}

// function for printing out name of room
void printRoomName(room_t *room1) {

  switch (room1->current_room) {
  case LOBBY:
    printf("Lobby");
    break;
  case LAB:
    printf("Lab");
    break;
  case HARRODS:
    printf("Harrods");
    break;
  case FUSION:
    printf("Fusion");
    break;
  case LECTURE_HALL:
    printf("Room 308");
    break;
  }
}

// function for printing out names of adjacent rooms
void printAdjacentRooms(room_t *room1) {
  for (int i = 0; i < 5; i++) {
    if (room1->adjacent_rooms[i]) {
      printRoomPosition(room1->adjacent_rooms[i]);
      printRoomName(room1->adjacent_rooms[i]);
      printf("\n");
      printf("                                                             ");
    }
  }
}

// function purely for testing ouptut of connect RoomName
void printRoomDetails(room_t *room1) {
  printRoomPosition(room1);
  printRoomName(room1);
  printf("Items in the room: ");
  printItemDetails(room1->items);
  printf("number of adjacent rooms: %d\n", room1->adjacent_room_count);
  printf("adjacent rooms:\n");
  printAdjacentRooms(room1);
}
