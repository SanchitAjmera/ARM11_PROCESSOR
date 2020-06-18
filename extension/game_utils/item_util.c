#include "item_util.h"
#include "../../src/common/util.h"
#include "game_util.h"

/* Returns respective int value; -1 for failure */
const item_t *itemLookup(const item_t table[], const int size,
                         const char *key) {
  for (int i = 0; i < size; i++) {
    if (strcmp(table[i].key, key) == 0) {
      return table + i;
    }
  }
  return LOOKUP_FAILURE;
}

// initialese Items
item_t *initialiseItem(ItemIndex gameItemID) {
  item_t *item = malloc(sizeof(*item));
  checkPtr(item);
  item_t gameItem = gameItems[gameItemID];
  item->key = strptr(gameItem.key);
  item->name = gameItem.name;
  item->gameItemID = gameItemID;
  item->properties = gameItem.properties;
  item->description = strptr(gameItem.description);
  item->cost = gameItem.cost;
  if (gameItemID == CASH_INDEX) {
    item->cost = rand() % 1000;
  }
  return item;
}

void initialiseBuyableItem(item_t *items[]) {
  item_t *buyApple1 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple2 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple3 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple4 = initialiseItem(BUYAPPLE_INDEX);
  item_t *buyApple5 = initialiseItem(BUYAPPLE_INDEX);

  item_t *coffee1 = initialiseItem(COFFEE_INDEX);
  item_t *coffee2 = initialiseItem(COFFEE_INDEX);
  item_t *coffee3 = initialiseItem(COFFEE_INDEX);
  item_t *coffee4 = initialiseItem(COFFEE_INDEX);
  item_t *coffee5 = initialiseItem(COFFEE_INDEX);

  item_t *tescoMD1 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD2 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD3 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD4 = initialiseItem(TESCO_INDEX);
  item_t *tescoMD5 = initialiseItem(TESCO_INDEX);

  item_t *rum1 = initialiseItem(RUM_INDEX);
  item_t *rum2 = initialiseItem(RUM_INDEX);
  item_t *rum3 = initialiseItem(RUM_INDEX);
  item_t *rum4 = initialiseItem(RUM_INDEX);
  item_t *rum5 = initialiseItem(RUM_INDEX);

  item_t *guzziBelt = initialiseItem(GUZZI_INDEX);
  item_t *supreme = initialiseItem(SUPREME_INDEX);
  item_t *drip = initialiseItem(DRIP_INDEX);
  item_t *blm = initialiseItem(BLM_INDEX);

  guzziBelt->hash = 100;
  supreme->hash = 101;
  drip->hash = 102;
  blm->hash = 103;

  items[0] = buyApple1;
  items[1] = buyApple2;
  items[2] = buyApple3;
  items[3] = buyApple4;
  items[4] = buyApple5;
  items[5] = coffee1;
  items[6] = coffee2;
  items[7] = coffee3;
  items[8] = coffee4;
  items[9] = coffee5;
  items[10] = tescoMD1;
  items[11] = tescoMD2;
  items[12] = tescoMD3;
  items[13] = tescoMD4;
  items[14] = tescoMD5;
  items[15] = rum1;
  items[16] = rum2;
  items[17] = rum3;
  items[18] = rum4;
  items[19] = rum5;
  items[20] = guzziBelt;
  items[21] = supreme;
  items[22] = drip;
  items[23] = blm;
}
