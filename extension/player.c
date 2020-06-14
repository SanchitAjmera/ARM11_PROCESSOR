#include "game_utils.h"

// need ptr checks
player_t *initialisePlayer() {
  player_t *newPlayer = malloc(sizeof(*newPlayer));
  newPlayer->inventory = calloc(ITEM_NUM, sizeof(item_t)));
  newPlayer->health = MAX_HEALTH;
  newPlayer->cash = INITIAL_CASH;
  newPlayer->item_count = 0;
  assert(newPlayer && newPlayer->inventory);
  return newPlayer;
}

// returns index of item if it is in the list TODO: remove magic numbers
int findItem(item_t **inventory, int item_count, char *itemName) {
  for (int i = 0; i < item_count; i++) {
    if (!strcmp(inventory[i]->key, itemName)) {
      return i;
    }
  }
  return FIND_FAIL;
}
