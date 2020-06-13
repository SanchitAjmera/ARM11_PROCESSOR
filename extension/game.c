#include "util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

  building *huxley = initialiseBuilding();
  //  printRoomDetails(lobby);
  freeBuilding(huxley);

  exit(EXIT_SUCCESS);
}
