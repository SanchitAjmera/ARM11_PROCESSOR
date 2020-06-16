#ifndef PLAYER_H
#define PLAYER_H
#include <stdbool.h>

typedef struct player_t player_t;

#include "../../game_util.h"

// Struct to store details about player;
struct player_t {
  item_t **inventory;
  int itemCount;
  int health;
  int cash;
};

extern player_t *initialisePlayer();
extern bool moveRoom(state *currentState, char *dirName);

#endif
