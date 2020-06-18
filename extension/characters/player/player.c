#include "../../game_util.h"

int roomItemTraversal(room_t *room, const item_t *item) {
  if (IS_NULL(item)) {
    return -1;
  }
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

player_t *initialisePlayer(void) {
  player_t *newPlayer = malloc(sizeof(*newPlayer));
  checkPtr(newPlayer);
  newPlayer->inventory = calloc(ITEM_NUM, sizeof(item_t));
  checkPtr(newPlayer->inventory);
  newPlayer->health = 50; // MAX_HEALTH;
  newPlayer->cash = INITIAL_CASH;
  newPlayer->itemCount = 0;
  return newPlayer;
}

void freePlayer(player_t *player) {
  // TODO: add a freeItem function
  if (!IS_NULL(player->inventory)) {
    for (int i = 0; i < player->itemCount; i++) {
      free(player->inventory[i]);
    }
    free(player->inventory);
  }
  free(player);
}

bool pickUpItem(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  int index = roomItemTraversal(currentState->currentRoom, item);
  if (index == -1 || IS_NULL(item) ||
      IS_NULL(currentState->currentRoom->items[index])) {
    printf("'%s' could not be found here!\n", itemName);
    return false;
  }
  if (hasProperty(BUYABLE, item)) {
    printf("No stealing in this game...!\n");
    return false;
  }
  if (currentState->currentRoom->items[index]->name == CASH) {
    currentState->player->cash += currentState->currentRoom->items[index]->cost;
    printf("%d HuxCoins Added\n", item->cost);
    currentState->currentRoom->items[index] = REMOVED;
    return true;
  }
  if (currentState->player->inventory[item->name]) {
    printf("%s is already in your inventory!\n", itemName);
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
  if (IS_NULL(item) || IS_NULL(currentState->player->inventory[item->name])) {
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
  if (IS_NULL(item)) {
    printf("%s cannot be bought!\n", itemName);
    return false;
  }
  int index = roomItemTraversal(currentState->currentRoom, item);
  if (index == FIND_FAIL) {
    printf("sorry %s out of stock\n", itemName);
    return false;
  }
  if (currentState->player->inventory[item->name]) {
    printf("inventory is full\n");
    return false;
  }
  if (currentState->player->cash < item->cost) {
    printf("you're a broke boy\n");
    return false;
  }
  currentState->player->cash -= item->cost;
  currentState->player->inventory[item->name] =
      currentState->currentRoom->items[index];
  currentState->currentRoom->items[index] = REMOVED;
  return true;
}

bool hasItem(state *currentState, Item index) {
  return currentState->player->inventory[index] != NULL;
}

// TODO assigne macros
bool validateAccess(state *currentState, int index) {
  if (!currentState->currentRoom->adjacent_rooms[index]) {
    return false;
  }
  if (currentState->currentRoom->current_room == LOBBY &&
      currentState->currentRoom->adjacent_rooms[index]->current_room == LAB &&
      currentState->currentRoom->adjacent_rooms[index]->position == EAST) {
    return hasItem(currentState, PASS);
  }
  // valid
  return true;
}

bool moveRoom(state *currentState, char *dirName) {
  int direction = lookup(directionTable, DIR_NUM, dirName);
  if (direction == -1) {
    printf("'%s' is not a valid direction!\n", dirName);
    return false;
  }
  if (validateAccess(currentState, direction)) {
    currentState->currentRoom =
        currentState->currentRoom->adjacent_rooms[direction];
    return true;
  }
  printf("sorry there is no room here\n");
  return false;
}

bool consume(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (IS_NULL(item) || IS_NULL(currentState->player->inventory[item->name])) {
    printf("You don't have '%s' to eat\n", itemName);
    return false;
  }
  if (IS_NULL(hasProperty(EDIBLE, item))) {
    printf("You can't eat that!\n");
    return false;
  }
  printf("mmm... Tasty.\n");
  int increase = item->cost;
  if (currentState->player->health + increase >= 100) {
    printf("Max health reached!\n");
    currentState->player->health = MAX_HEALTH;
  } else {
    printf("Your health has increased by %d!\n", increase);
    currentState->player->health += increase;
  }
  currentState->player->inventory[item->name] = REMOVED;
  return true;
}
