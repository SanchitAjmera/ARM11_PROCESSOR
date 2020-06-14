#include "game_utils.h"

// need ptr checks
player_t *initialisePlayer() {
  player_t *newPlayer = malloc(sizeof(*newPlayer));
  newPlayer->inventory = malloc(sizeof(*newPlayer->inventory));
  newPlayer->health = MAX_HEALTH;
  newPlayer->cash = INITIAL_CASH;
  newPlayer->item_count = 0;
  assert(newPlayer && newPlayer->inventory);
  return newPlayer;
}
