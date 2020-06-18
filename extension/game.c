#include "game_util.h"
#include "print_util.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void playGame(state *currentState) {
  printStateDetails(currentState);
  bool play = true;
  // TODO: validate ptr on these and look at size/magic numbers
  char *command = malloc(sizeof(char) * 30);
  char *argument = malloc(sizeof(char) * 30);
  checkPtr(command);
  checkPtr(argument);

  while (play) {
    getCommand(command, argument);
    char *reduced = reduceCommand(argument);
    Command type = lookup(commandsTable, COMMAND_NUM, command);

    if (type != FIND_FAIL && !SINGLE(type) && IS_EMPTY(argument)) {
      printf("I think you need to type more for this command...\n");
      continue;
    }

    switch (type) {
    case SKIP:
      printf("It might be useful to type a command...\n");
      break;
    case EXIT:
      // TODO: free all possible resources taken up by the game
      free(command);
      free(reduced);
      free(argument);
      quit();
    case PICKUP:
      pickUpItem(currentState, argument);
      printStateDetails(currentState);
      break;
    case DROP:
      dropItem(currentState, argument);
      printStateDetails(currentState);
      break;
    case MOVE:
      moveRoom(currentState, reduced);
      printStateDetails(currentState);

      break;
    case VIEW:
      view(currentState, reduced);
      // printInventory(currentState);
      break;
    case BUY:
      buyItem(currentState, argument);
      printStateDetails(currentState);
      break;
    case SAVE:
      //  saveGameState(argument, currentState, );
      break;
    case HELP:
      printf("Help is on its way -- need function\n");
      break;
    case CONSUME:
      consume(currentState, argument);
      printStateDetails(currentState);
      break;
    case FIGHT:
      fight(currentState, reduced);
      break;
    case FRESH:
      system(CLEAR);
      printf("The screen was cleared!\n");
      break;
    default:
      printf("I'm not sure how to '%s'?\n", command);
    }
  }

  free(command);
  free(argument);
}

int main(void) {

  // pointer to a location on heap storing an array of room pointers
  room_t **worldMap = malloc(sizeof(room_t *) * 25);
  building_t *huxley = initialiseBuilding(worldMap);
  state *playerState = initialiseState(huxley->startRoom);
  // state *playerState = malloc(sizeof(state));
  saveGameState("sanchizzle", playerState, worldMap);
  // loadGameState("sanchizzle", playerState, worldMap);
  int choice = INVALID;
  char menuChoice[100];

  int choices = 0;
  while (choice != NEW_GAME && choice != LOAD_GAME && choice != QUIT) {

    printMenu((choices > 0));
    fgets(menuChoice, sizeof(menuChoice), stdin);
    choice = atoi(menuChoice);

    switch (choice) {
    case QUIT:
      quit();

    case NEW_GAME:
      printPreparingGame();

      // TODO: validate huxley ptr

      playGame(playerState);

      break;

    case LOAD_GAME:
      printf("Enter the load file name: ");

      char fileName[100];
      fgets(fileName, sizeof(fileName), stdin);

      printf("File loaded: %s\n", fileName);

      int result = -1; // loadGame(fileName); // to be checked
      if (result == -1) {
        printf("File loading failed!\n");
        printf("You have been taken back to the menu.\n");
        choice = NONE;
      }
      break;

    default:
      printInvalid();
      continue;
    }
    printf("Done\n");
  }
  freeState(playerState);
  freeBuilding(huxley);
  free(worldMap);
}
