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

  // pointer to a location on heap storing an array of room pointers
  room_t **worldMap = malloc(sizeof(room_t *) * 25);
  building_t *huxley = initialiseBuilding(worldMap);

  state *playerState = initialiseState(huxley->start_room);
  // state *playerState = malloc(sizeof(state));
  saveGameState("sanchizzle", playerState, worldMap);
  // loadGameState("sanchizzle", playerState, worldMap);

  printBuildingDetails(huxley);

  printf("\n");

  printStateDetails(playerState);

  freeState(playerState);

  free(worldMap);
  freeBuilding(huxley);

  exit(EXIT_SUCCESS);
}
