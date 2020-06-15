#include "game_util.h"
#include "print_util.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// TODO:

void getCommand(char *command, char *argument) {
  char input[30];
  printf(" >> ");
  fgets(input, sizeof(input), stdin);

  command = strtok(input, " ");
  argument = strtok(NULL, " ");
}

void playGame(state *currentState) {}

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
