#ifndef PLAYER_H
#define PLAYER_H

#include "../../game_utils/game_util.h"
#include <stdbool.h>

// Struct to store details about player;
typedef struct {
  item_t **inventory;
  int itemCount;
  int health;
  int cash;
} player_t;

extern player_t *initialisePlayer(void);
extern bool moveRoom(state *currentState, char *dirName);
extern bool pickUpItem(state *currentState, char *itemName);
extern bool dropItem(state *currentState, char *itemName);
extern bool buyItem(state *currentState, char *itemName);
extern bool consume(state *currentState, char *itemName);
extern void freePlayer(player_t *player);
#endif
