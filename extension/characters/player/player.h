#ifndef PLAYER_H
#define PLAYER_H

#include "game_util.h"

// Struct to store details about player;
typedef struct {
  item_t **inventory;
  int itemCount;
  int health;
  int cash;
} player_t;

#endif
