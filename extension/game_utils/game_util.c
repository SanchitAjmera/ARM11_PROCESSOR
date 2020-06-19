#include "game_util.h"
#include "../../src/common/util.h"
#include "../characters/boss/boss.h"
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
  for (char *string = in; *string; string++) {
    *string = tolower(*string);
  }
}

char *reduceCommand(char *argument) {
  if (IS_EMPTY(argument)) {
    return argument;
  }
  char *reduced = malloc(sizeof(argument));
  strcpy(reduced, argument);
  reduced = strtok(reduced, " ");
  return reduced;
}

void getCommand(char *command, char *argument) {
  char input[30];
  printf("         >> ");
  fgets(input, sizeof(input), stdin);

  // obtain user input
  char *comm = strtok(input, " ");
  char *args = strtok(NULL, "\n");

  // remove trailing new line characters
  comm = strtok(comm, "\n");

  // Make both into lowercase
  lowercase(comm);
  lowercase(args);

  // Copy results into given pointers
  if (!comm) {
    strcpy(command, "skip");
    return;
  }
  strcpy(command, comm);

  if (!args) {
    strcpy(argument, "");
    return;
  }
  while (isspace(*args)) {
    args++;
  }
  strcpy(argument, args);
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

state *initialiseState(room_t *initialRoom) {
  state *initialState = malloc(sizeof(*initialState));
  checkPtr(initialState);
  initialState->player = initialisePlayer();
  initialState->currentRoom = initialRoom;
  char *username = malloc(sizeof(char) * USERNAME_CHAR_LIMIT);
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
