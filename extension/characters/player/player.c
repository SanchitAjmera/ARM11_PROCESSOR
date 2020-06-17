#include "../../game_util.h"

int roomItemTraversal(room_t *room, const item_t *item) {
  for (int i = 0; i < ITEM_NUM; i++) {
    if (room->items[i]) {
      if (room->items[i]->name == item->name) {
        return i;
      }
    }
  }
  return -1;
}

int findSpace(room_t *room, const item_t *item) {
  for (int i = 0; i < ITEM_NUM; i++) {
    if (!room->items[i]) {
      return i;
    }
  }
  return -1;
}

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

bool pickUpItem(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  int index = roomItemTraversal(currentState->currentRoom, item);
  if (!item || !currentState->currentRoom->items[index]) {
    printf("This item could not be found here!\n");
    return false;
  }
  if (currentState->player->inventory[item->name]) {
    printf("%s is already in your inventory!", itemName);
  } else {
    currentState->player->inventory[item->name] =
        currentState->currentRoom->items[index];
    currentState->currentRoom->items[index] = REMOVED;
    printf("%s has been picked up\n", itemName);
  }
  return true;
}

bool dropItem(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  int index = findSpace(currentState->currentRoom, item);
  if (!item || !currentState->player->inventory[item->name]) {
    printf("You do not have this item to drop: %s\n", itemName);
    return false;
  }
  currentState->currentRoom->items[index] =
      currentState->player->inventory[item->name];
  currentState->player->inventory[item->name] = REMOVED;
  printf("%s has been dropped!\n", itemName);

  return true;
}

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
  if (direction == -1) {
    printf("'%s' is not a valid direction!\n", dirName);
    return false;
  }
  if (currentState->currentRoom->adjacent_rooms[direction]) {
    currentState->currentRoom =
        currentState->currentRoom->adjacent_rooms[direction];
    return true;
  }
  printf("sorry there is no room here\n");
  return false;
}

bool consume(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!currentState->player->inventory[item->name]) {
    printf("You don't have '%s' to eat\n", itemName);
    return false;
  }
  if (!hasProperty(EDIBLE, item)) {
    printf("You can't eat that!\n");
    return false;
  }
  // TODO: calculate health increase
  int increase = 5;
  printf("mmm... Tasty.\n");
  printf("Your health has increased by %d!\n", increase);
  // item_t *remove = currentState->player->inventory[item->name];
  currentState->player->inventory[item->name] = REMOVED;
  // freeItem(remove);
  return true;
}

bool hasItem(state *currentState, Item index) {
  return currentState->player->inventory[index] != NULL;
}
