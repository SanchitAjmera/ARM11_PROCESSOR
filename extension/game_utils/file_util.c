#include "../../src/common/util.h"
#include "../characters/boss/boss.h"
#include "game_util.h"

/* loadGameState takes in a filename, a pointer to an uninitialised state,
   and an array of pointers to every room in the game.
   If such file does not exist or cannot be opened, return -1.
   Otherwise, it writes the file's contents to the state. */
int loadGameState(const char *fname, state *playerState, room_t **worldMap) {
  // PRE: playerState is not NULL
  if (access(fname, F_OK) == -1) { // File does not exist
    printf("File does not exist.\n");
    return -1;
  } else {
    FILE *file = fopen(fname, "rb");
    if (IS_NULL(file)) {
      return -1;
    } else {
      // TODO: add error checking to each fread to ensure that the end of the
      // file isn't reached prematurely (save file must be corrupted in this
      // case)
      fread(playerState, sizeof(state), 1, file);

      playerState->player = malloc(sizeof(player_t));
      fread(playerState->player, sizeof(player_t), 1, file);

      /* Load player's username */
      playerState->profile.username =
          malloc(sizeof(char) * USERNAME_CHAR_LIMIT);
      checkPtr(playerState->profile.username);
      fread(playerState->profile.username, sizeof(char), USERNAME_CHAR_LIMIT,
            file);

      /* Load inventory */
      int itemCount = playerState->player->itemCount;
      playerState->player->inventory = calloc(ITEM_NUM, sizeof(item_t));
      checkPtr(playerState->player->inventory);
      for (int i = 0; i < itemCount; i++) {
        ItemIndex gameItemID;
        fread(&gameItemID, sizeof(ItemIndex), 1, file);
        playerState->player->inventory[i] = initialiseItem(gameItemID);
        int hash;
        fread(&hash, sizeof(int), 1, file);
        playerState->player->inventory[i]->hash = hash;
      }

      /* Load player's current location */
      RoomName currentRoom;
      fread(&currentRoom, sizeof(RoomName), 1, file);
      RoomPosition currentPosition;
      fread(&currentPosition, sizeof(RoomPosition), 1, file);
      playerState->currentRoom = worldMap[(currentRoom * 5) + currentPosition];

      /* Load items in each room */
      for (int i = 0; i < TOTAL_ROOM_COUNT; i++) { // For each room in the map
        int itemCount;
        fread(&itemCount, sizeof(int), 1, file);
        worldMap[i]->itemCount = itemCount;
        for (int j = 0; j < itemCount; j++) {
          ItemIndex gameItemID;
          fread(&gameItemID, sizeof(ItemIndex), 1, file);
          int hash;
          fread(&hash, sizeof(int), 1, file);
          worldMap[i]->items[j] = initialiseItem(gameItemID);
          worldMap[i]->items[j]->hash = hash;
        }
      }
    }
    fclose(file);
    return 0;
  }
}

/* saveGameState takes in a filename, a pointer to the player's state,
   and an array of pointers to every room in the game.
   Returns -1 if the file could not be opened. Otherwise, it writes
   the player's state to the file. */
int saveGameState(const char *fname, state *playerState, room_t **worldMap) {
  FILE *fileOut = fopen(fname, "wb");
  if (IS_NULL(fileOut)) {
    printf("Something went wrong with the save file.");
    return -1;
  } else {
    // Save state
    fwrite(playerState, sizeof(state), 1, fileOut);
    // Save player struct
    fwrite(playerState->player, sizeof(player_t), 1, fileOut);
    // Save username
    fwrite(playerState->profile.username, sizeof(char), USERNAME_CHAR_LIMIT,
           fileOut);
    // Save each item in player's inventory
    for (int i = 0; i < playerState->player->itemCount; i++) {
      item_t *item = playerState->player->inventory[i];
      fwrite(&item->gameItemID, sizeof(ItemIndex), 1, fileOut);
      fwrite(&item->hash, sizeof(int), 1, fileOut);
    }
    // Save player's location
    room_t *currentRoom = playerState->currentRoom;
    fwrite(&currentRoom->current_room, sizeof(RoomName), 1, fileOut);
    fwrite(&currentRoom->position, sizeof(RoomPosition), 1, fileOut);
    // Save items in each room
    for (int i = 0; i < TOTAL_ROOM_COUNT; i++) {
      room_t *room = worldMap[i];
      fwrite(&room->itemCount, sizeof(int), 1, fileOut);
      for (int j = 0; j < room->itemCount; j++) {
        item_t *item = room->items[j];
        fwrite(&item->gameItemID, sizeof(ItemIndex), 1, fileOut);
        fwrite(&item->hash, sizeof(int), 1, fileOut);
      }
    }
    fclose(fileOut);
  }
  return 0;
}
