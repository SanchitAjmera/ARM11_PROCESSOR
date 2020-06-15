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

  // comm = lowercase(command);
  // arg = lowercase(argument);

  strcpy(command, comm);
  if (arg) {
    strcpy(argument, arg);
  }
}

void playGame(state *currentState) {
  bool play = true;
  // TODO: validate ptr on these
  char *command = malloc(sizeof(char) * 30);
  char *argument = malloc(sizeof(char) * 30);

  while (play) {
    getCommand(command, argument);
    Command type = lookup(commandsTable, COMMAND_NUM, command);
    switch (type) {
    case EXIT:
      free(command);
      free(argument);
      quit();
    case PICKUP:
      break;
    case DROP:
      break;
    case MOVE:
      break;

    case BUY:
      break;
    default:
      break;
    }
  }

  free(command);
  free(argument);
}

int main(void) {

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
      // TODO: validate huxley ptr
      building_t *huxley = initialiseBuilding();
      state *playerState = initialiseState(huxley->start_room);
      playGame(playerState);

      printBuildingDetails(huxley);
      printf("\n");
      printStateDetails(playerState);

      freeState(playerState);
      freeBuilding(huxley);

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

  exit(EXIT_SUCCESS);
}
