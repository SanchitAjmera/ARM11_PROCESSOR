
void printCommmands(state *currentState) {
  system(CLEAR);
  printf("                    view map      ");
  for (int i = 0; i < ITEM_NUM; i++) {
    if (currentState->currentRoom->items[i]) {
    }
  }
}

void printHealth(state *currentState) {
  for (int i = 0; i < currentState->player->health / 5; i++) {
    printf("#");
  }
  for (int i = 20; i > currentState->player->health / 5; i--) {
    printf("-");
  }

  printf("  %d", currentState->player->health);
}

void printInventory(state *currentState) {
  printf("                        ");
  int printed = 0;
  for (int i = 0; i < 6; i++) {
    if (currentState->player->inventory[i]) {
      if (printed != 0) {
        printf("|");
      }
      printf(" %s ", currentState->player->inventory[i]->key);
      printed++;
    }
  }
  if (printed == 0) {
    printf("EMPTY INVENTORY");
  }
  printf("\n");
}

void printProperties(state *currentState, char *itemName) {
  const item_t *item = itemLookup(gameItems, ITEM_NUM, itemName);
  if (!item || !currentState->player->inventory[item->name]) {
    printf("%s is not in your inventory!\n", itemName);
    return;
  }
  for (int i = 1; i <= MAX_PROPERTY; i <<= 1) {
    if (hasProperty(i, item)) {
      printf(" %s |", getPropertyStr(i));
    }
  }
  printf("\n");
}

void printPlayer(state *currentState) {
  printHealth(currentState);
  printCash(currentState);
  printInventory(currentState);
}
