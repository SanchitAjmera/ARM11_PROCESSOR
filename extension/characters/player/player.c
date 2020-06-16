#include "../../game_util.h"

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

//--------------------------Start of Prints-----------------------------------
/* Prints To be moved into print_utils maybe */
void printHealth(state *currentState) {
  printf("Your current health is: %d\n", currentState->player->health);
}

void printCash(state *currentState) {
  printf("You have £%d cash to spend\n", currentState->player->cash);
}

void printInventory(state *currentState) {
  printf("The items in your inventory are:");
  for (int i = 0; i < player->item_count; i++) {
    printf(" %s|", player->inventory[i]->name);
  }
  printf("\n");
}

char *getPropertyStr(Property property) {
  for (int i = 1; i < PROPERTY_NUM; i++) {
    if (propertyTable[i]->value == property) {
      return propertyTable[i]->key;
    }
    return LOOKUP_FAILURE;
  }
}

void printProperties(state *currentState, char *itemName) {
<<<<<<< HEAD
  item_t *item = lookup(gameItems, ITEM_NUM, itemName);
=======
  item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
>>>>>>> extension-commands
  if (!item || !currentState->player->inventory[item->name]) {
    printf("%s is not in your inventory!\n", itemName);
    return;
  }
  for (int i = 1; i <= MAX_PROPERTY; i << 1) {
    if (hasProperty(i, item)) {
      printf(" %s |", getPropertyStr(i));
<<<<<<< HEAD
    }
  }
  printf("\n");
}

void printPlayer(state *currentState) {
  printHealth(currentState);
  printCash(currentState);
  printInventory(currentState);
}

//--------------------------End of Prints--------------------------------

// returns index of item if it is in the list. TODO: remove magic numbers
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
    currentState->curr_room_node->item_count--;
    currentState->player->inventory[item->name] = item;
    currentState->player->itemCount++;
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
  currentState->player->itemCount--;
  currentState->curr_room_node->items[item->name] = item;
  currentState->curr_room_node->item_count++;
  printf("%s has been dropped!\n", itemName);

  return true;
}

int roomItemTraversal(room_t *room, item_t *item) {
  for (int i = 0; i < room->itemCount; i++) {
    if (room->items[i]->name == item->name) {
      return i;
    }
  }
  return -1;
}
// Todo:
bool buyItem(state *currentState, char *itemName) {
  item_t *item = lookup(gameItems, ITEM_NUM, itemName);
  int index = roomItemTraversal(currentState->curr_room_node, item);
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
        currentState->curr_room_node->items[index];
    currentState->curr_room_node->items[index] = REMOVED;
    return true;
  }
  printf("sorry %s out of stock\n", itemName);
  return false;
}

bool moveRoom(state *currentState, char *dirName) {
  int direction = lookup(propertyTable, DIR_NUM, dirName);

  if (currentState->curr_room_node->adjacent_rooms[direction]) {
    currentState->curr_room_node =
        currentState->curr_room_node->adjacent_rooms[direction];
    return true;
  }
  printf("sorry there is no room here");
  return false;
}
=======
    }
  }
  printf("\n");
}

void printPlayer(state *currentState) {
  printHealth(currentState);
  printCash(currentState);
  printInventory(currentState);
}

//--------------------------End of Prints--------------------------------

// returns index of item if it is in the list. TODO: remove magic numbers
int findItem(item_t **inventory, int item_count, char *itemName) {
  for (int i = 0; i < item_count; i++) {
    if (!strcmp(inventory[i]->key, itemName)) {
      return i;
    }
  }
  return FIND_FAIL;
}

bool pickUpItem(state *currentState, char *itemName) {
  item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->curr_room_node->items[item->name]) {
    printf("This item could not be found here!\n");
    return false;
  }
  if (currentState->player->inventory[item->name] == item) {
    printf("%s is already in your inventory!", itemName);
  } else {
    currentState->curr_room_node->items[item->name] = REMOVED;
    currentState->curr_room_node->item_count--;
    currentState->player->inventory[item->name] = item;
    currentState->player->itemCount++;
    printf("%s has been picked up\n", itemName);
  }
  return true;
}

bool dropItem(state *currentState, char *itemName) {
  item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->player->inventory[item->name]) {
    printf("You do not have this item to drop: %s\n", itemName);
    return false;
  }
  currentState->player->inventory[item->name] = REMOVED;
  currentState->player->itemCount--;
  currentState->curr_room_node->items[item->name] = item;
  currentState->curr_room_node->item_count++;
  printf("%s has been dropped!\n", itemName);

  return true;
}
// Todo:
bool buyItem(state *currentState, char *itemName) {}
>>>>>>> extension-commands
