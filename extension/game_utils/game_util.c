#include "game_util.h"
#include "../src/common/util.h"
#include "characters/boss/boss.h"
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
// general lookup
int lookup(const pair_t table[], const int size, const char *key) {
  for (int i = 0; i < size; i++) {
    if (!strcmp(table[i].key, key)) {
      return table[i].value;
    }
  }
  return -1;
} */

/*
char *strptr(const char *in) {
  char *out = malloc(sizeof(char) * (strlen(in) + 1));
  checkPtr(out);
  strcpy(out, in);
  return out;
} */

/*
int validatePtr(const void *ptr, char *message) {
  if (ptr == NULL) {
    printf("Error: %s", message);
    return -1;
  }
  return 0;
} */

/* Checks for failed memory allocation */
void checkPtr(const void *ptr) {
  if (IS_NULL(ptr)) {
    // TODO: save game
    printf("A memory error occurred. The game must end!");
    quit();
  }
}

// converts a string to lowercase
void lowercase(char *in) {
  if (IS_NULL(in)) {
    return;
  }

  for (int i = 0; i < strlen(in); i++) {
    in[i] = tolower(in[i]);
  }
  // if (!IS_NULL(in)) {
  //   for (char *string = in; *string; string++) {
  //     *string = tolower(*string);
  //   }
  // }
}

// shows player their inventory of Items

// Checks if an item has a certain property
bool hasProperty(Property property, const item_t *item) {
  return (property & item->properties);
}

// quits game - TODO: free all resources in this function;
void quit(void) {
  printf("Thanks for playing!\n");
  exit(EXIT_SUCCESS);
}

// displays introduction to player
void introduction() {}

// generates array of random numbers of length n
void randomiseArray(int randArray[], int length, int randMax) {
  srand(time(NULL));
  for (int i = 0; i < length; i++) {
    randArray[i] = rand() % randMax;
  }
}

// randomly places Items in room
void randomlyPlaceItems(item_t *items[], room_t *rooms[]) {
  int *randomCashLocations = malloc(sizeof(int) * TOTAL_CASH_COUNT);
  checkPtr(randomCashLocations);
  int *randomPearLocations = malloc(sizeof(int) * TOTAL_PEAR_COUNT);
  checkPtr(randomPearLocations);
  checkPtr(randomCashLocations);
  checkPtr(randomPearLocations);
  // array for random locations of apples and cash
  randomiseArray(randomPearLocations, TOTAL_PEAR_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  randomiseArray(randomCashLocations, TOTAL_CASH_COUNT,
                 TOTAL_ROOM_COUNT - TOTAL_SHOP_COUNT);
  // dynamically addes apple and cash Items into random rooms
  for (int i = 0; i < 5; i++) {
    rooms[randomCashLocations[i]]
        ->items[rooms[randomCashLocations[i]]->itemCount] = items[i];
    rooms[randomCashLocations[i]]->itemCount++;
    rooms[randomPearLocations[i]]
        ->items[rooms[randomPearLocations[i]]->itemCount] = items[i + 5];
    rooms[randomPearLocations[i]]->itemCount++;
  }
  // 4 IS THE NUMBER OF OTHER ItemS APART FROM CASH & APPLES
  // added other Items randomly around lobby
  int *randomOtherItemLocations = malloc(sizeof(int) * 4);
  checkPtr(randomOtherItemLocations);
  randomiseArray(randomOtherItemLocations, 4, ROOM_POSITION_NUMBER);
  for (int i = 10; i < TOTAL_ITEM_COUNT - TOTAL_BUYABLE_ITEM_COUNT; i++) {
    rooms[randomOtherItemLocations[i - 10]]
        ->items[rooms[randomOtherItemLocations[i - 10]]->itemCount] = items[i];
    rooms[randomOtherItemLocations[i - 10]]->itemCount++;
  }
  free(randomCashLocations);
  free(randomPearLocations);
  free(randomOtherItemLocations);
}

void placeBuyableItems(item_t *items[], room_t *rooms[]) {
  for (int i = 0; i < BUYABLE_ITEMS_IN_ROOM; i++) {
    rooms[FUSION_WEST_INDEX]->items[rooms[FUSION_WEST_INDEX]->itemCount] =
        items[i];
    rooms[FUSION_WEST_INDEX]->itemCount++;

    rooms[FUSION_EAST_INDEX]->items[rooms[FUSION_EAST_INDEX]->itemCount] =
        items[5 + i];
    rooms[FUSION_EAST_INDEX]->itemCount++;

    rooms[FUSION_SOUTH_INDEX]->items[rooms[FUSION_SOUTH_INDEX]->itemCount] =
        items[10 + i];
    rooms[FUSION_SOUTH_INDEX]->itemCount++;

    rooms[FUSION_NORTH_INDEX]->items[rooms[FUSION_NORTH_INDEX]->itemCount] =
        items[15 + i];
    rooms[FUSION_NORTH_INDEX]->itemCount++;
  }
  for (int i = 20; i < TOTAL_ROOM_COUNT - 1; i++) {
    rooms[i]->items[0] = items[i];
    rooms[i]->itemCount++;
  }
}

state *initialiseState(room_t *initialRoom) {
  state *initialState = malloc(sizeof(*initialState));
  checkPtr(initialState);
  initialState->player = initialisePlayer();
  initialState->currentRoom = initialRoom;
  char *username = malloc(sizeof(char) * USERNAME_CHAR_LIMIT);
  malloc(username);
  strcpy(username, "sanchit");
  // scanf("%s\n", username);
  initialState->profile.username = username;
  initialState->profile.character = UTA;
  initialState->profile.score = 0;
  return initialState;
}

void freeState(state *state1) {
  freePlayer(state1->player);
  free(state1);
}
