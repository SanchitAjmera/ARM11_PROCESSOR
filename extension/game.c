#include "print_util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {

  building_t *huxley = initialiseBuilding();

  state *playerState = initialiseState(huxley->start_room);

  // printBuildingDetails(huxley);

  freeState(playerState);

  freeBuilding(huxley);

  exit(EXIT_SUCCESS);
}
