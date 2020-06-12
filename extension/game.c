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

  connectRoom(lobby, *lecture_hall);
  connectRoom(lobby, *lab);
  connectRoom(lobby, *fusion);
  connectRoom(fusion, *lobby);
  connectRoom(lecture_hall, *lobby);
  connectRoom(lab, *lobby);

  printRoom(lobby->current_room);
  freeRoom(lobby);
  freeRoom(lab);
  freeRoom(lecture_hall);
  freeRoom(fusion);

  exit(EXIT_SUCCESS);
}
