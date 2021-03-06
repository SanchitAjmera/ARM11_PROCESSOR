#include "game_utils/game_util.h"
#include "print_utils/print_util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testBool(bool cond, char *test_name) {
  printf("T: %-35s : %s\n", test_name, cond ? "OK" : "FAIL");
}

void testSaveFiles() {
  room_t **saveMap = malloc(sizeof(room_t *) * 25);
  building_t *saveHuxley = initialiseBuilding(saveMap);

  state *saveState = initialiseState(saveHuxley->startRoom);
  int result = saveGameState("test_save_file.dat", saveState, saveMap);
  testBool(result == 0, "File saves without error");

  room_t **loadMap = malloc(sizeof(room_t *) * 25);
  building_t *loadHuxley = initialiseBuilding(loadMap);

  state *loadState = malloc(sizeof(state));
  result = loadGameState("test_save_file.dat", loadState, loadMap);
  testBool(result == 0, "File loads without error");

  int usernameCmp =
      strcmp(saveState->profile.username, loadState->profile.username);
  testBool(usernameCmp == 0, "Usernames match");

  room_t *loadCurrRoom = loadState->currentRoom;
  room_t *saveCurrRoom = saveState->currentRoom;
  testBool((loadCurrRoom->current_room == saveCurrRoom->current_room) &&
               (loadCurrRoom->position == saveCurrRoom->position),
           "Player location matches");

  player_t *savePlayer = saveState->player;
  player_t *loadPlayer = loadState->player;
  testBool(savePlayer->itemCount == loadPlayer->itemCount, "Item counts match");
  testBool(savePlayer->health == loadPlayer->health, "Health matches");
  testBool(savePlayer->cash == loadPlayer->cash, "Cash matches");

  for (int i = 0; i < loadPlayer->itemCount; i++) {
    item_t *loadItem = loadPlayer->inventory[i];
    item_t *saveItem = savePlayer->inventory[i];
    printf("Testing inventory item %i\n", i + 1);
    testBool(loadItem->hash == saveItem->hash, "Inventory item hash matches");
  }

  for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
    room_t *loadRoom = loadMap[i];
    room_t *saveRoom = saveMap[i];
    printf("Testing room %i\n", i + 1);
    testBool(loadRoom->itemCount == saveRoom->itemCount,
             "Room's item count matches");
    for (int j = 0; j < loadRoom->itemCount; j++) {
      printf("  Testing room %i item %i\n", i + 1, j + 1);
      item_t *loadItem = loadRoom->items[j];
      item_t *saveItem = saveRoom->items[j];
      testBool(loadItem->hash == saveItem->hash, "Room item hash matches");
    }
  }

  free(saveMap);
  freeState(saveState);
  freeBuilding(saveHuxley);
  free(loadMap);
  freeState(loadState);
  freeBuilding(loadHuxley);
}

// TODO:
void testLookup() { printf("%s", gameItems[0].key); }

int main(void) {
  printf("-------Testing-Save-Files-------\n");
  testSaveFiles();
  return EXIT_SUCCESS;
}
