#include "util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  room *lobby = initialiseRoom(LOBBY);
  room *lecture_hall = initialiseRoom(LECTURE_HALL);
  room *fusion = initialiseRoom(FUSION);
  room *lab = initialiseRoom(LAB);

  connectRoom(lobby, lecture_hall);
  connectRoom(lobby, lab);
  connectRoom(lobby, fusion);

  printRoom(lobby->current_room);
  freeRoom(lobby);

  exit(EXIT_SUCCESS);
}
