#include "game_util.h"
#include "print_util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void getCommand(char *command, char *argument) {
  char input[30];
  printf(" >> ");
  fgets(input, sizeof(input), stdin);

  // obtain user input
  char *comm = strtok(input, " ");
  char *arg = strtok(NULL, " ");

  // remove trailing new line characters
  comm = strtok(comm, "\n");
  arg = strtok(arg, "\n");

  // Make both into lowercase
  lowercase(comm);
  lowercase(arg);

  // Copy results into given pointers
  strcpy(command, comm);
  if (arg) {
    strcpy(argument, arg);
  }
}

void playGame(state *currentState) {
  bool play = true;
  // TODO: validate ptr on these and look at size/magic numbers
  char *command = malloc(sizeof(char) * 30);
  char *argument = malloc(sizeof(char) * 30);
  checkPtr(command);
  checkPtr(argument);

  while (play) {
    getCommand(command, argument);
    Command type = lookup(commandsTable, COMMAND_NUM, command);
    switch (type) {
    case EXIT:
      // TODO: free all possible resources taken up by the game
      free(command);
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
      moveRoom(currentState, argument);
      printStateDetails(currentState);
      break;
    case VIEW:
      view(currentState, argument);
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

    default:
      printf("I'm not sure how to '%s'?\n", command);
      break;
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
  int choice = NONE;
  char menuChoice[100];
  printMenu();

  while (choice != NEW_GAME && choice != LOAD_GAME && choice != QUIT) {

    printf("Please enter a 1 (quit), 2 (new game) or 3 (load game)!\n");
    printf(" >> ");

    fgets(menuChoice, sizeof(menuChoice), stdin);
    choice = atoi(menuChoice);

    switch (choice) {
    case QUIT:
      quit();

    case NEW_GAME:
      printf("Prepare for a new adventure...\n");
      printf("dummy: %s\n", gameItems[0].key);
      // TODO: validate huxley ptr
      room_t **rooms = malloc(sizeof(*rooms) * ROOM_COUNT);
      building_t *huxley = initialiseBuilding(rooms);
      state *playerState = initialiseState(huxley->startRoom);
      // printBuildingDetails(huxley);
      // printf("\n");
      printStateDetails(playerState);
      playGame(playerState);

      freeState(playerState);
      freeBuilding(huxley);
      free(rooms);
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
      printf("Invalid choice entered!\n");
      break;
    }
  }
  printf("Done\n");

  freeBuilding(huxley);
  free(worldMap);
  exit(EXIT_SUCCESS);
}
