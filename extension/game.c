#include "game_util.h"
#include "print_util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {

  Menu choice = NONE;

  printf("Welcome to the ICL Text Adventure!\n");
  printf("What would you like to do?\n");
  printf("    1. Start a new game\n");
  printf("    2. Load a previously saved game\n");
  while (choice != NEW_GAME || choice != LOAD_GAME) {
    printf("Please enter a 1 (for new game) or 2 (for load game)!\n");
    printf(">> ");
    scanf("%d", choice);
  }
  switch (choice) {
  case NEW_GAME:
    printf("Prepare for a new adventure...\n");
    building_t *huxley = initialiseBuilding();
    state *playerState = initialiseState(huxley->start_room);
    break;
  case LOAD_GAME:
    printf("some other code");
    break;
  default:
    assert(false);
  }

  printBuildingDetails(huxley);

  printf("\n");

  printStateDetails(playerState);

  freeState(playerState);

  freeBuilding(huxley);

  exit(EXIT_SUCCESS);
}
