

char *getPropertyStr(Property property) {
  for (int i = 1; i < PROPERTY_NUM; i++) {
    if (propertyTable[i].value == property) {
      return propertyTable[i].key;
    }
  }
  return LOOKUP_FAILURE;
}

void printItemDetails(item_t **items) {
  printf("                                                             ROOM "
         "ITEMS\n");
  printf("                                                             ");
  for (int i = 0; i < ITEM_NUM; i++) {
    if (items[i]) {
      printf("%s \n", items[i]->key);
      printf("                                                             ");
    }
  }
}

void printCash(state *currentState) {
  printf("HuxCoins: $%d", currentState->player->cash);
}
