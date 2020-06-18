#include "print_util.h"
#include <stdio.h>

#define CLEAR ("clear")

void printPreparingGame(void) {
  for (int i = 0; i < 32; i++) {
    system(CLEAR);
    printf("\n\n\n\n\n\n\n\n");
    printf("                       Prepare for a new adventure...");
    printf("\n\n\n                       ");
    for (int j = 0; j < i; j++) {
      printf("#");
    }
    for (int j = 30; j >= i; j--) {
      printf("-");
    }
    printf("\n");
    fflush(stdout);
    usleep(70000);
  }
  printf("\n\n");
}

void printMenu(bool isInvalid) {
  system(CLEAR);
  printf("\n\n\n\n\n\n");
  printf("                          Welcome to the ICL Text Adventure!\n\n");
  printf("                          What would you like to do?\n\n");
  if (!isInvalid) {
    usleep(500000);
  }
  fflush(stdout);
  printf("                              [1] NEW GAME\n");
  printf("                              [2] LOAD GAME\n");
  printf("                              [3] QUIT\n\n");
  if (isInvalid) {
    printf("                          You have entered an invalid choice");
  }
  printf("\n\n                          >> ");
}

void printInvalid(void) {
  printf("\n                            Invalid choice entered!\n");
  usleep(800000);
}

// prints map of current and adjancent room on top right of terminal

bool containsRoom(room_t *printedRooms[4], int size, room_t *room) {
  for (int i = 0; i < size; i++) {
    if (room->current_room == printedRooms[i]->current_room) {
      return true;
    }
  }
  return false;
}

void printStateDetails(state *state1) {
  system(CLEAR);
  printf("\n");
  // printing health
  printHealth(state1);
  printf("                                    CURRENT ROOM:\n");
  // printing score
  printf("Score:     %d", state1->profile.score);
  printf("                                                 ");
  // printing current room details
  printRoomPosition(state1->currentRoom);
  printRoomName(state1->currentRoom);
  printf("\n");
  // printing cash
  printCash(state1);
  printf("\n");

  printf(
      "                                                             ADJACENT "
      "ROOMS:\n");
  // printing adjacent rooms
  printf("                                                             ");
  printAdjacentRooms(state1->currentRoom);
  printf("\n");
  printItemDetails(state1->currentRoom->items);
  int totalLines =
      state1->currentRoom->adjacent_room_count + state1->currentRoom->itemCount;
  for (int i = 0; i < (8 - totalLines); i++) {
    printf("\n");
  }
  printInventory(state1);
  printf("\n\n");
}

// printing the map -------------------------------------------------------

void checkRoom(state *currentState, RoomName roomName, RoomPosition pos) {
  if (currentState->currentRoom->current_room == roomName &&
      currentState->currentRoom->position == pos) {
    printf("X");
  } else {
    printf(" ");
  }
}

void printMap(state *currentState) {
  system(CLEAR);
  printf("\n\n\n\n");
  printf("                                 @@@@@@@@@                         "
         "     \n");
  printf("                                 @       @                         "
         "     \n");
  printf("                                 @   ");
  checkRoom(currentState, LECTURE_HALL, NORTH);
  printf("   @            \n");
  printf("                                 @       @                         "
         "   \n");
  printf("                         @@@@@@@@@@@@@@@@@@@@@@@@@      \n");
  printf("                         @       @       @       @              \n");
  printf("                         @   ");
  checkRoom(currentState, LECTURE_HALL, WEST);
  printf("   @   ");
  checkRoom(currentState, LECTURE_HALL, CENTRE);
  printf("   @   ");
  checkRoom(currentState, LECTURE_HALL, EAST);
  printf("   @        \n");
  printf("                         @       @       @       @                 "
         "     \n");
  printf("                         @@@@@@@@@@@@@@@@@@@@@@@@@                 "
         "   \n");
  printf("                                 @       @                         "
         "     \n");
  printf("                                 @   ");
  checkRoom(currentState, LECTURE_HALL, SOUTH);
  printf("   @                        \n");
  printf("                                 @       @                         "
         "     \n");
  printf("               @@@@@@@@@         @@@@@@@@@       @@@@@@@@@       \n");
  printf("               @       @         @       @       @       @    \n");
  printf("               @   ");
  checkRoom(currentState, LAB, NORTH);
  printf("   @         @   ");
  checkRoom(currentState, LOBBY, NORTH);
  printf("   @       @   ");
  checkRoom(currentState, FUSION, NORTH);
  printf("   @       \n");
  printf("               @       @         @       @       @       @   \n");
  printf(" "
         "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
         "@@@@\n");
  printf(" @       @       @       @       @       @       @       @       @ "
         "      @ \n");
  printf(" @       @       @       @       @   X   @       @       @       @ "
         "      @ \n");
  printf(" @       @       @       @       @       @       @       @       @ "
         "      @ \n");
  printf(" "
         "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
         "@@@@\n");
  printf("                                 @       @                 \n");
  printf("                                 @   X   @                 \n");
  printf("                                 @       @                   \n");
  printf("                                 @@@@@@@@@               \n");
}

// --------------------------------------------------------------------------

void view(state *currentState, char *argument) {
  if (strcmp(argument, "room") == 0) {
    printRoomDetails(currentState->currentRoom);
  } else if (strcmp(argument, "inventory") == 0) {
    printInventory(currentState);
  } else if (strcmp(argument, "map") == 0) {
    printMap(currentState);
  }
}
