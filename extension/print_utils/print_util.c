#include "print_util.h"
#include <stdio.h>

#define CLEAR ("clear")

void printPreparingGame(void) {
  for (int i = 0; i < 32; i++) {
    system(CLEAR);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                       Prepare for a new adventure...");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("                       ");
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
  printf("\n");
  printf("\n");
}

void printMenu(bool isInvalid) {
  system(CLEAR);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("                          Welcome to the ICL Text Adventure!\n\n");
  printf("                          What would you like to do?\n\n");
  if (!isInvalid) {
    usleep(500000);
  }
  fflush(stdout);
  printf("                              [1] NEW GAME\n");
  printf("                              [2] LOAD GAME\n");
  printf("                              [3] QUIT\n");
  printf("\n");
  if (isInvalid) {
    printf("                          You have entered an invalid choice\n");
  } else {
    printf("\n");
  }
  printf("\n");
  printf("                          >> ");
  return;
}

void printInvalid(void) {
  printf("\n                            Invalid choice entered!\n");
  usleep(800000);
}

void printItemDetails(item_t **items) {
  printf("                                                             ROOM "
         "ITEMS\n");
  printf("                                                             ");
  for (int i = 0; i < ITEM_NUM; i++) {
    if (items[i]) {
      printf("%s ", items[i]->key);
      printf("\n");
      printf("                                                             ");
    }
  }
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
