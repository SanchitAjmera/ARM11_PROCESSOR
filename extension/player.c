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

/* To be moved into print_utils */
void printPlayer(state *currentState) {
  player_t *player = currentState->player;
  printf("Your current health is: %d\n", player->health);
  printf("You have £%d cash to spend\n", player->cash);
  printf("The items in your inventory are:");
  for (int i = 0; i < player->item_count; i++) {
    printf(" %s|", player->inventory[i]->name);
  }
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

bool pickUpItem(state *currentState, char *itemName) {
  item_t *item = lookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->curr_room_node->items[item->name]) {
    printf("This item could not be found here!\n");
    return false;
  }
  if (currentState->player->inventory[item->name] == item) {
    printf("%s is already in your inventory!", itemName);
  } else {
    currentState->curr_room_node->items[item->name] = REMOVED;
    currentState->player->inventory[item->name] = item;
    printf("%s has been picked up\n", itemName);
  }
  return true;
}

bool dropItem(state *currentState, char *itemName) {
  item_t *item = lookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->player->inventory[item->name]) {
    printf("You do not have this item to drop: %s\n", itemName);
    return false;
  }
  currentState->player->inventory[item->name] = REMOVED;
  currentState->curr_room_node->items[item->name] = item;
  printf("%s has been dropped!\n", itemName);
}
return true;
