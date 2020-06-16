#include "../../game_util.h"

player_t *initialisePlayer() {
  player_t *newPlayer = malloc(sizeof(*newPlayer));
  checkPtr(newPlayer);
  newPlayer->inventory = calloc(ITEM_NUM, sizeof(item_t));
  checkPtr(newPlayer->inventory);
  newPlayer->health = MAX_HEALTH;
  newPlayer->cash = INITIAL_CASH;
  newPlayer->itemCount = 0;
  return newPlayer;
}

// returns index of item if it is in the list. TODO: remove magic numbers
int findItem(item_t **inventory, int itemCount, char *itemName) {
  for (int i = 0; i < itemCount; i++) {
    if (!strcmp(inventory[i]->key, itemName)) {
      return i;
    }
  }
  return FIND_FAIL;
}

bool pickUpItem(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->currentRoom->items[item->name]) {
    printf("This item could not be found here!\n");
    return false;
  }
  if (currentState->player->inventory[item->name] == item) {
    printf("%s is already in your inventory!", itemName);
  } else {
    currentState->player->inventory[item->name] =
        currentState->currentRoom->items[item->name];
    currentState->currentRoom->items[item->name] = REMOVED;
    currentState->currentRoom->itemCount--;
    currentState->player->itemCount++;
    printf("%s has been picked up\n", itemName);
  }
  return true;
}

bool dropItem(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->player->inventory[item->name]) {
    printf("You do not have this item to drop: %s\n", itemName);
    return false;
  }
  currentState->currentRoom->items[item->name] =
      currentState->player->inventory[item->name];
  currentState->player->inventory[item->name] = REMOVED;
  currentState->player->itemCount--;
  currentState->currentRoom->itemCount++;
  printf("%s has been dropped!\n", itemName);

  return true;
}

int roomItemTraversal(room_t *room, const item_t *item) {
  for (int i = 0; i < room->itemCount; i++) {
    if (room->items[i]->name == item->name) {
      return i;
    }
  }
  return -1;
}
// Todo:
bool buyItem(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  int index = roomItemTraversal(currentState->currentRoom, item);
  if (index != FIND_FAIL) {
    if (currentState->player->inventory[item->name]) {
      printf("inventory is full\n");
      return false;
    }
    if (currentState->player->cash < item->cost) {
      printf("your a broke boy\n");
      return false;
    }
    currentState->player->inventory[item->name] =
        currentState->currentRoom->items[index];
    currentState->currentRoom->items[index] = REMOVED;
    return true;
  }
  printf("sorry %s out of stock\n", itemName);
  return false;
}

bool moveRoom(state *currentState, char *dirName) {
  int direction = lookup(directionTable, DIR_NUM, dirName);

  if (currentState->currentRoom->adjacent_rooms[direction]) {
    currentState->currentRoom =
        currentState->currentRoom->adjacent_rooms[direction];
    return true;
  }
  printf("sorry there is no room here\n");
  return false;
}
